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
  YYSYMBOL_deprecated_decl_base = 187,     /* deprecated_decl_base  */
  YYSYMBOL_module_decl_start = 188,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 189,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 190,           /* access_control  */
  YYSYMBOL_opt_prototype = 191,            /* opt_prototype  */
  YYSYMBOL_include_access_control = 192,   /* include_access_control  */
  YYSYMBOL_include_module_stmt = 193,      /* include_module_stmt  */
  YYSYMBOL_194_1 = 194,                    /* $@1  */
  YYSYMBOL_block_stmt = 195,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 196,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 197,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 198,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 199,              /* opt_only_ls  */
  YYSYMBOL_except_ls = 200,                /* except_ls  */
  YYSYMBOL_use_access_control = 201,       /* use_access_control  */
  YYSYMBOL_use_stmt = 202,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 203,              /* import_stmt  */
  YYSYMBOL_import_expr = 204,              /* import_expr  */
  YYSYMBOL_import_ls = 205,                /* import_ls  */
  YYSYMBOL_require_stmt = 206,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 207,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 208,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 209,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 210,                /* ident_def  */
  YYSYMBOL_ident_use = 211,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 212,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 213,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 214,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 215,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 216,              /* return_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 217, /* deprecated_class_level_stmt  */
  YYSYMBOL_class_level_stmt = 218,         /* class_level_stmt  */
  YYSYMBOL_219_2 = 219,                    /* @2  */
  YYSYMBOL_private_decl = 220,             /* private_decl  */
  YYSYMBOL_forwarding_stmt = 221,          /* forwarding_stmt  */
  YYSYMBOL_extern_export_decl_stmt = 222,  /* extern_export_decl_stmt  */
  YYSYMBOL_223_3 = 223,                    /* $@3  */
  YYSYMBOL_224_4 = 224,                    /* $@4  */
  YYSYMBOL_225_5 = 225,                    /* $@5  */
  YYSYMBOL_226_6 = 226,                    /* $@6  */
  YYSYMBOL_227_7 = 227,                    /* $@7  */
  YYSYMBOL_228_8 = 228,                    /* $@8  */
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
  YYSYMBOL_defer_stmt = 241,               /* defer_stmt  */
  YYSYMBOL_try_stmt = 242,                 /* try_stmt  */
  YYSYMBOL_catch_stmt_ls = 243,            /* catch_stmt_ls  */
  YYSYMBOL_catch_stmt = 244,               /* catch_stmt  */
  YYSYMBOL_catch_expr = 245,               /* catch_expr  */
  YYSYMBOL_throw_stmt = 246,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 247,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 248,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 249,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 250,          /* class_decl_stmt  */
  YYSYMBOL_class_tag = 251,                /* class_tag  */
  YYSYMBOL_opt_inherit = 252,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 253,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 254,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 255,              /* enum_header  */
  YYSYMBOL_enum_ls = 256,                  /* enum_ls  */
  YYSYMBOL_deprecated_enum_item = 257,     /* deprecated_enum_item  */
  YYSYMBOL_enum_item = 258,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 259,         /* lambda_decl_expr  */
  YYSYMBOL_260_9 = 260,                    /* $@9  */
  YYSYMBOL_261_10 = 261,                   /* $@10  */
  YYSYMBOL_linkage_spec = 262,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt = 263,             /* fn_decl_stmt  */
  YYSYMBOL_264_11 = 264,                   /* $@11  */
  YYSYMBOL_265_12 = 265,                   /* $@12  */
  YYSYMBOL_fn_decl_stmt_inner = 266,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_receiver_expr = 267,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 268,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 269,           /* assignop_ident  */
  YYSYMBOL_opt_formal_ls = 270,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 271,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 272,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 273,                /* formal_ls  */
  YYSYMBOL_formal = 274,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 275,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 276,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 277,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 278,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 279,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 280,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 281,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 282,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 283,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 284,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 285,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 286, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 287,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 288,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 289,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_inner = 290, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 291,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 292,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 293,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 294,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 295,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 296, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 297, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 298,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 299,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 300,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 301,                 /* opt_type  */
  YYSYMBOL_array_type = 302,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 303, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 304,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 305,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 306,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 307,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 308,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 309,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 310,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 311,                /* actual_ls  */
  YYSYMBOL_actual_expr = 312,              /* actual_expr  */
  YYSYMBOL_ident_expr = 313,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 314,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 315,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 316,                 /* for_expr  */
  YYSYMBOL_cond_expr = 317,                /* cond_expr  */
  YYSYMBOL_nil_expr = 318,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 319,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 320,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 321,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 322,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 323,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 324,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 325,              /* intent_expr  */
  YYSYMBOL_shadow_var_prefix = 326,        /* shadow_var_prefix  */
  YYSYMBOL_io_expr = 327,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 328,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 329,                 /* new_expr  */
  YYSYMBOL_let_expr = 330,                 /* let_expr  */
  YYSYMBOL_expr = 331,                     /* expr  */
  YYSYMBOL_opt_expr = 332,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 333,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 334,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 335,           /* call_base_expr  */
  YYSYMBOL_call_expr = 336,                /* call_expr  */
  YYSYMBOL_dot_expr = 337,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 338,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 339,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 340,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 341,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 342,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 343,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 344,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 345,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 346,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 347       /* reduce_scan_op_expr  */
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

#line 522 "bison-chapel.cpp"

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
#define YYLAST   21464

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  168
/* YYNRULES -- Number of rules.  */
#define YYNRULES  709
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1279

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

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   517,   517,   522,   523,   529,   530,   535,   536,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   577,   578,   580,   585,   586,   590,   603,   608,
     613,   621,   622,   623,   627,   628,   632,   633,   634,   639,
     638,   659,   660,   661,   666,   667,   672,   677,   682,   686,
     695,   700,   705,   710,   714,   718,   726,   731,   735,   740,
     744,   745,   746,   750,   751,   752,   753,   754,   755,   756,
     760,   765,   766,   767,   771,   772,   776,   780,   782,   784,
     786,   788,   790,   797,   798,   802,   803,   804,   805,   806,
     807,   810,   811,   812,   813,   814,   815,   827,   828,   839,
     840,   841,   842,   843,   844,   845,   846,   847,   848,   849,
     850,   851,   852,   853,   854,   855,   856,   857,   861,   862,
     863,   864,   865,   866,   867,   868,   869,   870,   871,   872,
     879,   880,   881,   882,   886,   887,   891,   892,   896,   897,
     899,   904,   905,   906,   916,   916,   921,   922,   923,   924,
     925,   926,   927,   931,   932,   933,   934,   939,   938,   955,
     954,   972,   971,   988,   987,  1005,  1004,  1020,  1019,  1035,
    1039,  1044,  1052,  1063,  1070,  1071,  1072,  1073,  1074,  1075,
    1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,
    1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,
    1096,  1097,  1103,  1109,  1115,  1121,  1128,  1135,  1139,  1146,
    1150,  1151,  1152,  1153,  1155,  1156,  1157,  1158,  1160,  1162,
    1164,  1166,  1171,  1172,  1176,  1178,  1186,  1187,  1192,  1197,
    1198,  1199,  1200,  1201,  1202,  1203,  1204,  1205,  1206,  1207,
    1208,  1209,  1216,  1217,  1218,  1219,  1228,  1229,  1233,  1235,
    1238,  1244,  1246,  1249,  1255,  1258,  1259,  1260,  1261,  1262,
    1263,  1267,  1268,  1272,  1273,  1274,  1278,  1279,  1283,  1286,
    1288,  1293,  1294,  1298,  1300,  1302,  1309,  1319,  1333,  1338,
    1343,  1351,  1352,  1357,  1358,  1360,  1365,  1381,  1388,  1397,
    1405,  1409,  1416,  1417,  1419,  1424,  1425,  1430,  1435,  1429,
    1462,  1465,  1469,  1477,  1487,  1476,  1526,  1530,  1535,  1539,
    1544,  1551,  1552,  1556,  1557,  1558,  1559,  1560,  1561,  1562,
    1563,  1564,  1565,  1566,  1567,  1568,  1569,  1570,  1571,  1572,
    1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,  1581,  1582,
    1583,  1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,
    1596,  1597,  1598,  1602,  1603,  1607,  1611,  1612,  1616,  1617,
    1621,  1623,  1625,  1627,  1629,  1631,  1636,  1637,  1641,  1642,
    1643,  1644,  1645,  1646,  1647,  1648,  1649,  1653,  1654,  1655,
    1656,  1657,  1658,  1662,  1663,  1664,  1668,  1669,  1670,  1671,
    1672,  1673,  1677,  1678,  1681,  1682,  1686,  1687,  1691,  1695,
    1696,  1697,  1705,  1706,  1708,  1710,  1712,  1717,  1719,  1724,
    1725,  1726,  1727,  1728,  1729,  1730,  1734,  1736,  1741,  1743,
    1745,  1750,  1763,  1780,  1781,  1783,  1788,  1789,  1790,  1791,
    1792,  1796,  1802,  1810,  1811,  1819,  1821,  1826,  1828,  1830,
    1835,  1837,  1839,  1846,  1847,  1848,  1853,  1855,  1857,  1861,
    1865,  1867,  1871,  1879,  1880,  1881,  1882,  1883,  1888,  1889,
    1890,  1891,  1892,  1912,  1916,  1920,  1928,  1935,  1936,  1937,
    1941,  1943,  1949,  1951,  1953,  1958,  1959,  1960,  1961,  1962,
    1968,  1969,  1970,  1971,  1975,  1976,  1980,  1981,  1982,  1986,
    1987,  1991,  1992,  1996,  1997,  2001,  2002,  2003,  2004,  2008,
    2009,  2020,  2022,  2024,  2030,  2031,  2032,  2033,  2034,  2035,
    2037,  2039,  2041,  2043,  2045,  2047,  2050,  2052,  2054,  2056,
    2058,  2060,  2062,  2064,  2067,  2069,  2074,  2076,  2078,  2080,
    2082,  2084,  2086,  2088,  2090,  2092,  2094,  2096,  2098,  2105,
    2111,  2117,  2123,  2132,  2142,  2150,  2151,  2152,  2153,  2154,
    2155,  2156,  2157,  2162,  2163,  2167,  2171,  2172,  2176,  2180,
    2181,  2185,  2189,  2193,  2200,  2201,  2202,  2203,  2204,  2205,
    2209,  2210,  2215,  2217,  2221,  2225,  2229,  2237,  2242,  2248,
    2254,  2261,  2271,  2279,  2280,  2281,  2282,  2283,  2284,  2285,
    2286,  2287,  2288,  2290,  2292,  2294,  2309,  2311,  2313,  2315,
    2320,  2321,  2325,  2326,  2327,  2331,  2332,  2333,  2334,  2343,
    2344,  2345,  2346,  2347,  2351,  2352,  2353,  2357,  2358,  2359,
    2360,  2361,  2369,  2370,  2371,  2372,  2376,  2377,  2381,  2382,
    2386,  2387,  2388,  2389,  2390,  2391,  2392,  2393,  2395,  2397,
    2398,  2399,  2403,  2411,  2412,  2416,  2417,  2418,  2419,  2420,
    2421,  2422,  2423,  2424,  2425,  2426,  2427,  2428,  2429,  2430,
    2431,  2432,  2433,  2434,  2435,  2436,  2437,  2438,  2443,  2444,
    2445,  2446,  2447,  2448,  2449,  2453,  2454,  2455,  2456,  2460,
    2461,  2462,  2463,  2468,  2469,  2470,  2471,  2472,  2473,  2474
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
  "toplevel_stmt", "pragma_ls", "stmt", "deprecated_decl_stmt",
  "deprecated_decl_base", "module_decl_start", "module_decl_stmt",
  "access_control", "opt_prototype", "include_access_control",
  "include_module_stmt", "$@1", "block_stmt", "stmt_ls", "renames_ls",
  "use_renames_ls", "opt_only_ls", "except_ls", "use_access_control",
  "use_stmt", "import_stmt", "import_expr", "import_ls", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt",
  "deprecated_class_level_stmt", "class_level_stmt", "@2", "private_decl",
  "forwarding_stmt", "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
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
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434
};
#endif

#define YYPACT_NINF (-1086)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-660)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1086,    91,  3600, -1086,   -41,    68, -1086, -1086, -1086, -1086,
   -1086, -1086,  5001,     8,   364,   174, 14944,   360, 21075,     8,
   11583,   376,   222,    51,   364,  5001, 11583,   878,  5001,   107,
   21109, 11755,  7953,   424,  8643, 10027, 10027,  7087,  8815,   438,
   -1086,   229, -1086,   453, 21195, 21195, 21195, -1086, 15310, 10199,
     493, 11583,    48, -1086,   508,   547, 11583, -1086, 14944, -1086,
   11583,   409,   300,   407,  2738,   548, 21229, -1086, 10373,  8125,
   11583, 10199, 14944, 11583,   428,   554,   443,  5001,   561, 11583,
     578, 11927, 11927, 21195,   582, -1086, 14944, -1086,   583,  8815,
   11583, -1086, 11583, -1086, 11583, -1086, -1086, 14462, 11583, -1086,
   11583, -1086, -1086, -1086,  3957,  7261,  8989, 11583, -1086,  4827,
   -1086, -1086, -1086,   425, -1086,   572, -1086, -1086,   154, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086,   603, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, 21195, -1086, 21195,   179,   225, -1086, -1086,
   15310, -1086,   496, -1086,   499, -1086, -1086,   502,   509,   521,
   11583,   515,   517, 20272,  2034,   284,   518,   519, -1086, -1086,
     361, -1086, -1086, -1086, -1086, -1086,   261, -1086, -1086, 20272,
     513,  5001, -1086, -1086,   522, 11583, -1086, -1086, 11583, 11583,
   11583, 21195, -1086, 11583, 10373, 10373,   631,   471, -1086, -1086,
   -1086, -1086,   331,   472, -1086, -1086,   525, 16907, 21195, 15310,
   -1086,   527, -1086,   366, 20272,  2872, -1086, -1086,  9161,   452,
    3346, -1086, -1086,   577,  8299,   610, 21315, 20272,   244,   210,
   -1086, 21349, 21195, -1086,   244, 21195,   530,    16, 16494,    42,
   16329,    42, 16411,    41, -1086, 16577, 21195, 21195,   -48, 15582,
     462,  8299, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086,   531, -1086,   390,  5001,
     535,  3192,    50,    65, -1086,  5001, -1086, -1086, 16988,   743,
   -1086,   536,   537, -1086, 16988,   331,   743, -1086,  8299,  1904,
   -1086, -1086, -1086,   384, 20272, 11583, 11583, -1086, 20272,   534,
   17562, -1086, 16988,   331, 20272,   539,  8299, -1086, 20272, 17645,
   -1086, -1086, 17727,  2366, -1086, -1086, 17808,   587,   553,   331,
      16, 16988, 17889,   489,   489,   973,   743,   743,   187, -1086,
   -1086,  4131,    86, -1086, 11583, -1086,   117,   119, -1086,   -36,
     116, 17970,   -45,   973,   704, -1086,  4305, -1086,   660, 11583,
   11583, 21195,   585,   557, -1086, -1086, -1086, -1086,   405,   481,
   -1086, 11583,   584, 11583, 11583, 11583, 10027, 10027, 11583,   492,
   11583, 11583, 11583, 11583, 11583,   658, 14462, 11583, 11583, 11583,
   11583, 11583, 11583, 11583, 11583, 11583, 11583, 11583, 11583, 11583,
   11583, 11583, 11583,   662, -1086, -1086, -1086, -1086, -1086,  9333,
    9333, -1086, -1086, -1086, -1086,  9333, -1086, -1086,  9333,  9333,
    8299,  8299, 10027, 10027,  7781, -1086, -1086, 17070, 17151, 18051,
     565,    74, 21195,  4479, -1086, 10027,    16,   568,   411, -1086,
   11583, -1086, -1086, 11583,   615, -1086,   574,   595, -1086, -1086,
   -1086, 21195, -1086, 15310, -1086, -1086, 21195,   580, 21195, -1086,
   15310,   700, 10373, -1086,  5175, 10027, -1086,   579, -1086,    16,
    5349, 10027, -1086,    16, -1086,    16, 10027, -1086,    16, 12099,
   11583, -1086,   625,   632,  5001,   723,  5001, -1086,   724, 11583,
   -1086, -1086,   572,   589,  8299, 21195, -1086, -1086,   417, -1086,
   -1086,  3192, -1086,   617,   597, -1086, 12271,   643, 11583, 15310,
   -1086, -1086, 11583, 11583, -1086,   598, -1086, 10373, -1086, 20272,
   20272, -1086,    33, -1086,  8299,   599, -1086,   751, -1086,   751,
   -1086, 12443,   628, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
    9507, -1086, 18132,  7435, -1086,  7609, -1086,  5001,   600, 10027,
    9681,  3783,   602, 11583, 10545, -1086, -1086,   388, -1086,  4653,
   21195, -1086,   412, 18213,   437, 16660,   251, 10373,   608,  3042,
      37, -1086, 18294, 16948, 16948,   375, -1086,   375, -1086,   375,
   20795,   645,  1197,  1501,   331,   489, -1086,   607, -1086, -1086,
   -1086, -1086, -1086,   973,  2863,   375,   576,   576, 16948,   576,
     576,  1285,   489,  2863, 20753,  1285,   743,   743,   489,   973,
     616,   622,   624,   629,   630,   637,   609,   605, -1086,   375,
   -1086,   375,    23, -1086, -1086, -1086,   137, -1086,  1639, 20423,
     368, 12615, 10027, 12787, 10027, 11583,  8299, 10027, 15224,   611,
       8, 18375, -1086, -1086, -1086, 20272, 18456,  8299, -1086,  8299,
   21195,   585,   442, 21195, 21195,   585, -1086,   585,   448, 11583,
     167,  8815, 20272,    29, 17232,  7781, -1086,  8815, 20272,    77,
   16742, -1086, -1086,    42, 16825, -1086,   641,   650,   642, 18537,
     650,   651, 11583, 11583,   774,  5001,   779, 18618,  5001, 17314,
     758, -1086,   249, -1086,   254, -1086,   186, -1086, -1086, -1086,
   -1086, -1086, -1086,   435,   680,   655, -1086, 15396, -1086,   415,
     657,  3192,    50,    -9,    34, 11583, 11583,  6915, -1086, -1086,
     524,  8471, -1086, 20272, -1086, 18699, 18780, -1086, -1086, 20272,
     664,   164,   665, -1086,  3403, -1086, -1086,   487, 21195, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086,  5001,   -35, 17397, -1086,
   -1086, 20272,  5001, 20272, -1086, 18861, -1086, -1086, -1086, 11583,
   -1086,    70,    84, 11583, -1086, 10717, 12099, 11583, -1086,  8299,
     685,  1473,   668, 20955,   708,    95, -1086, -1086,   750, -1086,
   -1086, -1086, -1086, 14295,   669, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086,  7781, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,    35, 10027,
   10027, 11583,   807, 18942, 11583,   809, 19023,   268,   670, 19104,
    8299,    16,    16, -1086, -1086, -1086, -1086,   585,   677, -1086,
     585,   585,   681,   682, -1086, 16988, -1086, 15665,  5523, -1086,
    5697, -1086,   274, -1086, 15748,  5871, -1086,    16,  6045, -1086,
      16, -1086,    16, -1086,    16, 11583, -1086, 11583, -1086, 20272,
   20272,  5001, -1086,  5001, 11583, -1086,  5001,   814, 21195,   689,
   21195,   525, -1086, -1086, 21195,   538, -1086,  3192,   710,   772,
   -1086, -1086, -1086,   130, -1086, -1086,   643,   688,    96, -1086,
   -1086,   692,   693, -1086,  6219, 10373, -1086, -1086, -1086, 21195,
   -1086,   725,   525, -1086, -1086,  6393,   696,  6567,   697, -1086,
   11583, -1086, -1086, 11583, 19185,    39, 17480,   711,   712,   275,
     702,  2283, -1086, 21195, -1086, 11583, 20989, -1086, -1086,   415,
     706,   423, -1086,   731, -1086,   733,   734,   745,   738,   741,
   -1086,   744,   760,   756,   761,   762,   458,   768,   764,   771,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086, -1086, -1086, 11583, -1086,   776,   780,   773,   706,   706,
   -1086, -1086, -1086,   643,   291,   316, 19267, 12959, 13131, 19348,
   13303, 13475, -1086, 13647, 13819,   344, -1086, -1086,   752, -1086,
     755,   757, -1086, -1086, -1086,  5001,  8815, 20272,  8815, 20272,
    7781, -1086,  5001,  8815, 20272, -1086,  8815, 20272, -1086, -1086,
   -1086, 19429, 20272, -1086, -1086, 20272,   890,  5001,   763, -1086,
   -1086, -1086,   710, -1086,   765, 10891,   105, -1086,   -11, -1086,
   -1086, 10027, 15084,  8299,  8299,  5001, -1086,    83,   766, 11583,
   -1086,  8815, -1086, 20272,  5001,  8815, -1086, 20272,  5001, 20272,
     279, 11063, 12099, 11583, 12099, 11583, -1086, -1086,   769, -1086,
    2021, -1086,  1904, -1086,  1217, -1086, -1086, -1086, 20272, -1086,
     141,   417, -1086, 19510, -1086, 15500, -1086, -1086, -1086, 11583,
   11583, 11583, 11583, 11583, 11583, 11583, 11583, -1086, -1086,  2804,
   -1086, -1086, 20427, 20574, 18618, 15831, 15914, -1086, 18618, 15997,
   16080, 11583,  5001, -1086, -1086,   105,   710,  9855, -1086, -1086,
   -1086,   168, 10373, -1086, -1086,   153, 11583,    18, 19591, -1086,
     977,   767,   770,   577, -1086,   525, 20272, 16163, -1086, 16246,
   -1086, -1086, -1086, 20272,   781,   782,   783,   786, -1086,  2502,
   -1086, -1086, -1086, 13991,   824,   785, -1086,   793,   795,   706,
     706, 19672, 19757, 19838, 19919, 20000, 20081, 20640, -1086, 20714,
   20780, -1086, -1086,  5001,  5001,  5001,  5001, 20272, -1086, -1086,
   -1086,   105, 11237,   127, -1086, 20272, -1086,   102, -1086,   -28,
   -1086,   277, 20175, -1086, -1086, -1086, 13819,   789,   790, -1086,
    5001,  5001, -1086, -1086, -1086, -1086, -1086,  6741, -1086, -1086,
     227, -1086,   -11, -1086, -1086, -1086, 11583, 11583, 11583, 11583,
   11583, 11583, -1086, -1086, -1086, 18618, 18618, 18618, 18618, -1086,
   -1086, -1086, -1086, -1086,   269, 10027, 14633, -1086, 11583,   153,
     102,   102,   102,   102,   102,   102,   153,  1318, -1086, -1086,
   18618, 18618,   777, 14163,   151,   177, 20232, -1086, -1086, 20272,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086,   791, -1086, -1086,
     334, 14804, -1086, -1086, -1086, 11411, -1086,   382, -1086
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   117,   652,   653,   654,   648,
     649,   655,     0,   573,   103,   138,   542,   145,   544,   573,
       0,   144,     0,   447,   103,     0,     0,   320,     0,   266,
     139,   620,   620,   646,     0,     0,     0,     0,     0,   143,
      59,   267,   321,   140,     0,     0,     0,   317,     0,     0,
     147,     0,   592,   564,   656,   148,     0,   322,   536,   446,
       0,     0,     0,   164,   320,   142,   545,   448,     0,     0,
       0,     0,   540,     0,     0,   146,     0,     0,   118,     0,
     647,     0,     0,     0,   141,   300,   538,   450,   149,     0,
       0,   705,     0,   707,     0,   708,   709,   619,     0,   706,
     703,   523,   161,   704,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    45,    54,    10,    11,     0,    12,
      13,    14,    15,   519,   520,    25,    46,   162,   171,   172,
      16,    20,    17,    19,     0,   261,    18,   611,    22,    23,
      24,    21,   170,     0,   168,     0,   608,     0,   166,   169,
       0,   167,   625,   604,   521,   605,   526,   524,     0,     0,
       0,   609,   610,     0,   525,     0,   626,   627,   628,   650,
     651,   603,   528,   527,   606,   607,     0,    41,    27,   534,
       0,     0,   574,   104,     0,     0,   544,   139,     0,     0,
       0,     0,   545,     0,     0,     0,     0,   608,   625,   524,
     609,   610,   543,   525,   626,   627,     0,   573,     0,     0,
     449,     0,   274,     0,   504,   320,   298,   308,   620,   164,
     320,   299,    44,     0,   511,   648,   545,   621,   320,   648,
     193,   545,     0,   181,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   176,     0,     0,     0,     0,     0,
      56,   511,   111,   119,   131,   125,   124,   133,   114,   123,
     134,   120,   135,   112,   136,   129,   122,   130,   128,   126,
     127,   113,   115,   121,   132,   137,     0,   116,     0,     0,
       0,     0,     0,     0,   453,     0,   155,    35,     0,   690,
     596,   593,   594,   595,     0,   537,   691,     7,   511,   320,
     163,   418,   501,     0,   500,     0,     0,   156,   624,     0,
       0,    38,     0,   541,   529,     0,   511,    39,   535,     0,
     281,   277,     0,   525,   281,   278,     0,   443,     0,   539,
       0,     0,     0,   692,   694,   617,   689,   688,     0,    61,
      64,     0,     0,   506,     0,   508,     0,     0,   507,     0,
       0,   500,     0,   618,     0,     6,     0,    55,     0,     0,
       0,     0,   301,     0,   404,   405,   403,   323,     0,   522,
      26,     0,   597,     0,     0,     0,     0,     0,     0,   693,
       0,     0,     0,     0,     0,     0,   616,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   361,   368,   369,   370,   365,   367,     0,
       0,   363,   366,   364,   362,     0,   372,   371,     0,     0,
     511,   511,     0,     0,     0,    28,    29,     0,     0,     0,
       0,     0,     0,     0,    30,     0,     0,     0,     0,    31,
       0,    32,    43,     0,   519,   517,     0,   512,   513,   518,
     187,     0,   190,     0,   179,   183,     0,     0,     0,   189,
       0,     0,     0,   203,     0,     0,   202,     0,   211,     0,
       0,     0,   209,     0,   219,     0,     0,   217,     0,     0,
      76,   173,     0,     0,     0,   235,     0,   361,   231,     0,
      58,    57,    54,     0,     0,     0,   245,    33,   386,   318,
     457,     0,   458,   460,     0,   482,     0,   463,     0,     0,
     154,    34,     0,     0,    36,     0,   165,     0,    96,   622,
     623,   157,     0,    37,     0,     0,   288,   279,   275,   280,
     276,     0,   441,   438,   196,   195,    40,    63,    62,    65,
       0,   657,     0,     0,   642,     0,   644,     0,     0,     0,
       0,     0,     0,     0,     0,   661,     8,     0,    48,     0,
       0,    94,     0,    91,     0,    70,   272,     0,     0,     0,
     397,   452,   572,   685,   684,   687,   696,   695,   700,   699,
     681,   678,   679,   680,   613,   668,   117,     0,   639,   640,
     118,   638,   637,   614,   672,   683,   677,   675,   686,   676,
     674,   666,   671,   673,   682,   665,   669,   670,   667,   615,
       0,     0,     0,     0,     0,     0,     0,     0,   698,   697,
     702,   701,   584,   585,   587,   589,     0,   576,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   659,   272,
     573,   573,   199,   439,   451,   505,     0,     0,   531,     0,
       0,   301,     0,     0,     0,   301,   440,   301,     0,     0,
       0,     0,   548,     0,     0,     0,   212,     0,   554,     0,
       0,   210,   220,     0,     0,   218,   704,    79,     0,    66,
      77,     0,     0,     0,   234,     0,   230,     0,     0,     0,
       0,   530,     0,   248,     0,   246,   391,   388,   389,   390,
     394,   395,   396,   386,   379,     0,   376,     0,   387,   406,
       0,   461,     0,   152,   153,   151,   150,     0,   481,   480,
     604,     0,   455,   602,   454,     0,     0,   636,   503,   502,
       0,     0,     0,   532,     0,   282,   445,   604,     0,   658,
     612,   643,   509,   645,   510,   227,     0,     0,     0,   660,
     225,   558,     0,   663,   662,     0,    50,    49,    47,     0,
      90,     0,     0,     0,    83,     0,     0,    76,   269,     0,
     302,     0,     0,     0,   315,     0,   309,   312,   401,   398,
     399,   402,   324,     0,     0,   102,   100,   101,    99,    98,
      97,   634,   635,   586,   588,     0,   575,   138,   145,   144,
     143,   140,   147,   148,   142,   146,   141,   149,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,   515,   516,   514,   301,     0,   192,
     301,   301,     0,     0,   191,     0,   229,     0,     0,   201,
       0,   200,     0,   579,     0,     0,   207,     0,     0,   205,
       0,   215,     0,   213,     0,     0,   174,     0,   175,   243,
     242,     0,   237,     0,     0,   233,     0,   239,     0,   271,
       0,     0,   392,   393,     0,   386,   375,     0,   495,   407,
     410,   409,   411,     0,   459,   462,   463,     0,     0,   464,
     465,     0,     0,   290,     0,     0,   289,   292,   533,     0,
     283,   286,     0,   442,   228,     0,     0,     0,     0,   226,
       0,    95,    92,     0,    73,    72,    71,     0,     0,     0,
       0,   320,   307,     0,   314,     0,   310,   306,   400,   406,
     373,   105,   355,   119,   353,   125,   124,   108,   123,   120,
     358,   135,   106,   136,   122,   126,   107,   109,   121,   137,
     352,   334,   337,   335,   336,   359,   347,   338,   351,   343,
     341,   354,   357,   342,   340,   345,   350,   339,   344,   348,
     349,   346,   356,     0,   333,     0,   110,     0,   373,   373,
     331,   641,   577,   463,   625,   625,     0,     0,     0,     0,
       0,     0,   271,     0,     0,     0,   198,   197,     0,   303,
       0,     0,   303,   303,   204,     0,     0,   547,     0,   546,
       0,   578,     0,     0,   553,   208,     0,   552,   206,   216,
     214,    68,    67,   236,   232,   563,   238,     0,     0,   268,
     247,   244,   495,   377,     0,     0,   463,   408,   422,   456,
     486,     0,   659,   511,   511,     0,   294,     0,     0,     0,
     284,     0,   223,   560,     0,     0,   221,   559,     0,   664,
       0,     0,     0,    76,     0,    76,    84,    87,   273,   297,
     320,   164,   320,   296,   320,   304,   158,   313,   316,   311,
       0,   386,   330,     0,   360,     0,   326,   327,   581,     0,
       0,     0,     0,     0,     0,     0,     0,   273,   303,   320,
     303,   303,   320,   320,   551,     0,     0,   580,   557,     0,
       0,     0,     0,   241,    60,   463,   495,     0,   498,   497,
     499,   604,   419,   382,   380,     0,     0,     0,     0,   484,
     604,     0,     0,   295,   293,     0,   287,     0,   224,     0,
     222,    93,    75,    74,     0,     0,     0,     0,   270,   320,
     160,   305,   477,     0,   412,     0,   332,   105,   107,   373,
     373,     0,     0,     0,     0,     0,     0,   320,   186,   320,
     320,   178,   182,     0,     0,     0,     0,    69,   240,   383,
     381,   463,   487,     0,   421,   420,   436,     0,   437,   424,
     427,     0,   423,   416,   417,   319,     0,   598,   599,   285,
       0,     0,    86,    89,    85,    88,   159,     0,   476,   475,
     604,   413,   422,   374,   328,   329,     0,     0,     0,     0,
       0,     0,   188,   180,   184,   550,   549,   556,   555,   385,
     384,   489,   490,   492,   604,     0,   659,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   604,   600,   601,
     562,   561,     0,   467,     0,     0,     0,   491,   493,   426,
     428,   429,   432,   433,   434,   430,   431,   425,   472,   470,
     604,   659,   414,   325,   415,   487,   471,   604,   494
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1086, -1086, -1086,     1,     4,  2692, -1086,     5, -1086, -1086,
   -1086,   463, -1086, -1086, -1086,   618,   588,  -465, -1086,  -737,
    -735, -1086, -1086, -1086,   176, -1086, -1086,   233,   934, -1086,
    2477,  -170,  -766, -1086,  -990,  1149, -1085,  -115,  -886, -1086,
     -63, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086, -1086,
   -1086,   131, -1086,   875, -1086, -1086,    98,  1288, -1086, -1086,
   -1086, -1086, -1086,   646, -1086,    67, -1086, -1086, -1086, -1086,
   -1086, -1086,  -554,  -696, -1086, -1086, -1086,    45,  -730,  1394,
   -1086, -1086, -1086,   211, -1086, -1086, -1086, -1086,  -113,  -154,
    -951, -1086, -1086,  -108,    99,   280, -1086, -1086, -1086,    55,
   -1086, -1086,  -278,    28,  -915,  -232,  -261,  -252,  -423, -1086,
    -203, -1086,    -1,   951,  -137,   482, -1086,  -485,  -846,  -916,
   -1086,  -694,  -524, -1018, -1083,  -980,   -27, -1086,   129, -1086,
    -196,  -482,  -491,   788,  -468, -1086, -1086, -1086,  1580, -1086,
     -10, -1086, -1086,   -90, -1086,  -654, -1086, -1086, -1086,  1766,
    1996,   -12,   959,    66,  1044, -1086,  2186,  2241, -1086, -1086,
   -1086, -1086, -1086, -1086, -1086, -1086, -1086,  -422
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   340,  1074,   686,   111,   112,   113,   114,
     115,   358,   492,   116,   250,   117,   341,   677,   564,   681,
     678,   118,   119,   120,   561,   562,   121,   122,   184,   974,
     282,   123,   277,   124,   718,   287,   125,  1075,   126,   299,
     127,   128,   129,   456,   653,   458,   654,   451,   650,   130,
     131,   818,   132,   248,   133,   694,   695,   196,   135,   136,
     137,   138,   139,   527,   735,   902,   140,   141,   731,   897,
     142,   143,   568,   921,   144,   145,   775,   776,   777,   197,
     280,   709,   147,   148,   570,   929,   782,   977,   978,   489,
    1082,   499,   704,   705,   706,   707,   708,   783,   367,   883,
    1212,  1273,  1195,   445,  1123,  1127,  1189,  1190,  1191,   149,
     328,   532,   150,   151,   283,   284,   503,   504,   722,  1209,
    1154,   507,   719,  1232,  1120,  1036,   342,   213,   346,   347,
     446,   447,   448,   198,   153,   154,   155,   156,   199,   158,
     181,   182,   626,   469,   842,   627,   628,   159,   160,   200,
     201,   163,   234,   449,   203,   165,   204,   205,   168,   169,
     170,   171,   352,   172,   173,   174,   175,   176
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     179,   300,   630,   108,   202,   437,   109,   736,   207,   206,
     419,   843,   692,   368,   214,   680,   710,   976,   886,   227,
     227,   209,   238,   240,   242,   245,   249,  1086,  1087,   457,
     918,   917,   222,   732,   730,  1076,   505,   288,   720,   289,
    1039,   303,  1194,   924,   294,  1118,   295,  -266,   296,   285,
    1125,   505,  1115,   484,   444,   493,   304,   308,   310,   312,
     313,   314,   838,   737,   778,   318,   290,   319,    23,   322,
     326,   793,   438,   586,   329,   285,  1062,   331,   332,   350,
     333,   444,   334,   467,   467,   335,   336,   586,   337,  1238,
    -267,     3,   304,   308,   351,   353,   302,   828,  -478,  1233,
     554,   832,   515,   833,   587,  1186,  1126,  -291,   109,    69,
     845,  1063,   794,   508,   983,   779,   285,  1239,   588,    59,
     525,   433,   637,   177,  -249,   291,   780,   180,   444,   555,
      67,   505,   302,   345,   302,   309,  1181,  1088,   547,   905,
     210,   982,  1152,   292,  1041,   781,   444,   589,   372,  -291,
     473,   475,   478,  1258,   817,    87,  1186,   300,   826,  -478,
     293,   467,   224,  1208,   549,   237,   239,   241,   431,   419,
    1194,   348,   590,   179,  -478,  1235,   427,   428,   429,   506,
    -478,   318,   304,   351,  1150,   433,   590,   433,  1076,  1038,
    1124,  -478,   591,  1077,   506,  -478,   467,   436,   433,   637,
    1179,   359,  -473,  -291,  1188,   251,   227,  -478,  1214,  1215,
     509,   433,   308,  1076,  -478,   592,  1076,  1076,  1257,   550,
     442,  -478,   302,   302,   616,   617,   885,   453,   517,   721,
    -250,   540,  -473,   460,   872,   467,   516,   349,   894,   308,
     926,   550,   209,   630,  1187,  -473,   433,  -478,   638,    23,
     444,   444,   433,   913,  1122,  1188,   541,  1278,  -473,   548,
     552,   550,   543,  1206,   545,   927,  1229,   360,    69,  1180,
    1042,    23,   550,   998,   506,   873,  1000,  1001,   224,   364,
     895,  1076,   795,  1076,  1076,  1153,   308,   919,  -474,   544,
     551,   546,  -496,   519,   520,    42,   550,   454,   365,  -478,
      59,  1236,   680,  1099,   308,  -473,  1102,  1103,   366,   796,
    -473,    67,   517,  -496,   321,   325,   652,  -496,  -474,   976,
      57,   455,    59,   658,   444,  1271,  1145,  1144,  1147,  1146,
     208,  -474,   542,    67,   896,  1230,    87,  1269,   995,   836,
    -496,  1272,   539,  -569,  -474,   109,   433,   563,   565,   422,
    -632,   177,  -632,   423,   444,  1276,  1107,   537,    87,   572,
     109,   573,   574,   575,   577,   579,   580,   183,   581,   582,
     583,   584,   585,   630,   593,   594,   595,   596,   597,   598,
     599,   600,   601,   602,   603,   604,   605,   606,   607,   608,
     609,  -474,  1034,  -488,   649,  -466,  -474,   308,   308,   870,
     251,  1240,  1167,   308,  1169,  1170,   308,   308,   308,   308,
     619,   621,   629,   649,  -488,   768,  -256,   297,  -488,  1010,
     649,   869,   769,   641,   855,  -466,   871,  1241,   645,  1242,
    1243,   646,  -255,  1244,  1245,   660,  -583,   109,  -466,   452,
     992,  -488,   879,  -468,   696,   459,  1011,  1068,  1060,  1141,
     304,  -466,   662,   664,   -82,   420,   810,   421,   668,   670,
     423,  -582,   696,  -583,   674,   697,   444,   679,   679,   698,
     379,   298,   -53,  -468,   687,   611,   612,   689,   385,   444,
    -264,   613,   308,   697,   614,   615,  -468,   698,  -582,   649,
     302,   -53,   699,   880,  -254,   700,   723,    61,  -466,  -468,
     725,   726,   703,  -466,   881,   729,   701,   576,   578,  -251,
     699,   440,   308,   700,   379,   354,  1097,   -53,  1129,   383,
     -82,   375,   385,   882,   701,   702,   388,   315,   729,   517,
     441,   308,  -633,   308,  -633,   903,   -53,   748,   729,   751,
     770,   753,   755,   702,   490,   728,  -468,   491,   518,  -258,
     509,  -468,   177,   618,   620,   304,   509,   759,   756,   433,
     539,   495,  -111,   109,  -262,   696,   640,  1121,   728,   571,
    -117,   345,  -479,   345,  1130,   644,   760,   376,   728,   847,
     850,   377,   763,   852,   854,  -479,   697,   509,   630,  -378,
     698,   912,   592,   509,   356,   302,   663,  -113,   680,   444,
     680,   764,   669,  -259,  -253,  -118,   829,   673,   375,   348,
    -257,   348,   834,   699,   316,  -479,   700,  -263,    61,   662,
     813,   668,   816,   687,   308,   819,   751,   701,   379,   979,
     821,   822,  -444,   383,  -265,   308,   385,   308,  -252,  -260,
     388,  -479,  -632,  -629,  -632,  -629,   702,   835,  -479,   837,
     444,  -444,  -631,   629,  -631,   844,   357,   906,   908,   361,
    -566,   586,   369,  -630,   376,  -630,  -565,   286,   377,  -479,
     859,   860,   742,   370,   744,   824,   371,   375,   587,  -570,
     747,  -571,  -568,  -567,   424,  1210,   426,   432,  -479,   286,
     888,   439,   588,  -479,   433,   443,  -479,   450,   521,   320,
     324,   462,   494,   314,   318,   351,   498,   512,   513,   308,
     524,   531,   556,   825,  1234,   379,   380,   533,   381,   382,
     383,   589,   384,   385,   386,   560,   569,   388,  1247,   567,
     610,   385,   643,   376,   394,   395,   636,   377,   398,   647,
     649,   399,   400,   401,   656,   302,   648,   563,   659,   682,
     665,   914,   402,   916,   679,   679,   683,   308,   685,   688,
     590,   691,   711,   812,  1237,   815,   591,   721,  1234,   712,
     727,   733,   734,   738,   746,   375,   752,   771,   784,   792,
     785,   791,   820,   629,   379,  1270,   786,   890,   787,   383,
     152,   384,   385,   788,   789,   855,   388,   577,   619,   986,
     152,   790,   989,  1277,   395,   -78,   856,  1234,   308,   861,
     399,   400,   401,   152,   863,   858,   152,  1261,  1262,  1263,
    1264,  1265,  1266,   868,   434,   875,  1007,   876,  1009,   884,
     517,   376,   925,  1014,   893,   377,  1017,   898,   922,   928,
     987,   981,   990,  1021,   993,  1022,   999,  1131,  1132,  1027,
    1002,  1003,  1025,  1029,  1035,   286,   286,   286,   286,   286,
     286,  1037,  1040,  1043,  1044,   152,   485,   488,  1047,  1049,
    1054,  1058,  1069,   444,   444,  1066,  1067,  1081,  -138,   703,
    -145,  -144,   379,   304,  -114,  -143,   215,   383,  -140,   384,
     385,  -147,   152,  1053,   388,  1057,   496,   152,  1059,  -112,
     216,   679,   395,  -148,    22,    23,   286,  -115,  -142,  -146,
     401,  -141,   286,  1078,   217,  1084,    31,   218,  -149,  -116,
    1085,  1098,    37,   302,  1100,  1112,  1101,  1114,  1211,    42,
     286,  1160,  -111,  1148,  -113,   911,  1239,  1116,  1135,  1197,
     576,   618,  1198,   -51,   559,  1202,  1203,  1204,   286,   286,
    1205,  1268,  1248,  1249,    57,   690,    59,  1213,   211,  1151,
     219,  1083,   -51,   220,   330,   221,  1048,    67,  1030,   152,
     529,  1079,  1159,  1155,  1033,  1007,  1009,  1274,  1014,  1017,
    1255,  1053,  1057,   874,  1080,  1267,    83,  1260,   244,    85,
     228,   724,    87,  1104,  1105,     0,  1106,     0,   629,     0,
    1108,  1109,     0,     0,  1110,   375,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -483,     0,     0,     0,  1128,
     751,   308,   308,     0,     0,     0,     0,  1136,  -483,  1137,
       0,     0,   102,  1139,     0,     0,   164,     0,     0,  1143,
     679,   679,   679,   679,   286,     0,   164,     0,     0,     0,
       0,   376,     0,  1119,     0,   377,     0,   152,  -483,   164,
       0,     0,   164,   152,     0,     0,     0,  1104,  1161,  1162,
    1108,  1163,  1164,  1165,  1166,   703,     0,   286,     0,     0,
    1183,   286,     0,   286,  -483,     0,   286,     0,     0,  1177,
       0,  -483,     0,     0,     0,   351,     0,     0,     0,     0,
    1185,     0,   379,   380,  1192,   381,   382,   383,     0,   384,
     385,   164,  -483,     0,   388,   323,   323,     0,     0,   152,
       0,   394,   395,     0,     0,   398,     0,     0,   399,   400,
     401,  -483,     0,     0,   152,   302,  -483,     0,   164,  -483,
    1184,     0,     0,   164,     0,     0,     0,     0,     0,     0,
       0,  1225,  1226,  1227,  1228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1254,     0,     0,     0,  1057,     0,     0,     0,  1250,  1251,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
       0,     0,     0,     0,  1225,  1226,  1227,  1228,  1250,  1251,
    1231,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,   152,     0,  1256,   751,   164,  1259,     0,     0,   375,
       0,     0,     0,     0,     0,   302,     0,     0,     0,   216,
       0,     0,     0,    22,    23,     0,     0,     0,  1070,     0,
       0,     0,   152,   217,     0,    31,   218,     0,   152,   751,
       0,    37,     0,  1057,  1231,     0,     0,     0,    42,     0,
       0,     0,   152,     0,   152,     0,     0,     0,     0,     0,
       0,   286,   286,     0,     0,   376,     0,   286,   286,   377,
     134,   286,   286,    57,     0,    59,     0,   354,     0,  1071,
     134,     0,  1072,  1231,   221,     0,    67,   867,     0,     0,
       0,     0,     0,   134,     0,     0,   134,   375,     0,     0,
       0,     0,     0,   164,     0,    83,     0,     0,    85,   164,
       0,    87,     0,     0,     0,   152,   379,   380,     0,   152,
     382,   383,     0,   384,   385,     0,     0,   152,   388,     0,
       0,     0,   900,     0,     0,     0,   395,     0,     0,     0,
       0,     0,   399,   400,   401,   134,  -485,     0,     0,     0,
       0,     0,     0,   376,     0,     0,     0,   377,     0,  -485,
       0,   102,     0,     0,     0,   164,   463,   466,   468,   472,
     474,   477,   134,     0,     0,     0,   146,   134,     0,     0,
     164,     0,     0,     0,     0,     0,   146,     0,     0,  -485,
       0,     0,     0,     0,     0,     0,   808,     0,     0,   146,
       0,     0,   146,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,     0,     0,  -485,   388,   511,     0,   286,
     286,     0,  -485,   514,   395,     0,     0,     0,     0,     0,
     399,   400,   401,   286,     0,   488,     0,     0,     0,     0,
       0,   523,   488,  -485,     0,   286,     0,     0,   286,   134,
     286,   146,   286,   152,   920,     0,   152,   164,     0,   534,
     535,     0,  -485,     0,     0,     0,     0,  -485,     0,  1031,
    -485,     0,     0,     0,     0,  -303,     0,     0,   146,  -303,
    -303,     0,     0,   146,  -303,     0,     0,     0,   164,  -303,
       0,  -303,  -303,     0,   164,     0,     0,  -303,     0,     0,
    1050,     0,     0,     0,  -303,     0,     0,  -303,   164,     0,
     164,     0,     0,   375,   152,     0,     0,     0,     0,     0,
     152,     0,     0,     0,     0,     0,  -303,     0,     0,  -303,
       0,  -303,     0,  -303,     0,  -303,  -303,     0,  -303,     0,
    -303,     0,  -303,     0,     0,     0,     0,   134,     0,     0,
       0,   980,     0,   134,     0,   146,     0,     0,     0,     0,
       0,  -303,   157,     0,  -303,   642,     0,  -303,     0,   376,
       0,   164,   157,   377,     0,   164,     0,   984,   985,     0,
       0,     0,     0,   164,     0,   157,     0,     0,   157,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   666,     0,
       0,     0,   671,     0,   672,     0,   152,   675,   152,   134,
       0,     0,     0,   152,     0,     0,   152,  -303,     0,     0,
     379,   380,   586,  -303,   134,   383,     0,   384,   385,   152,
       0,   152,   388,     0,   152,     0,   797,   157,     0,   798,
     395,     0,     0,     0,   799,   286,   399,   400,   401,     0,
       0,     0,     0,   146,     0,   187,     0,     0,     0,   146,
       0,     0,   152,     0,   157,   800,     0,     0,     0,   157,
       0,     0,   801,   152,     0,   152,     0,     0,     0,     0,
       0,     0,   802,     0,     0,     0,     0,     0,     0,   803,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   134,     0,   488,   488,   804,     0,   488,   488,   164,
       0,     0,   164,     0,     0,   146,     0,   805,     0,     0,
       0,   590,     0,     0,     0,  1193,     0,     0,   806,     0,
     146,     0,   134,  1199,   807,   488,     0,   488,   134,     0,
       0,   157,     0,     0,     0,     0,     0,     0,   161,     0,
       0,     0,   134,     0,   134,     0,     0,     0,   161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     164,   161,     0,   152,   161,     0,   164,     0,     0,     0,
     152,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   839,   841,     0,   152,     0,     0,   846,   849,
       0,     0,   851,   853,     0,     0,     0,   146,     0,     0,
       0,     0,     0,   152,     0,   134,     0,     0,     0,   134,
       0,     0,   152,   161,     0,     0,   152,   134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   146,   157,
       0,     0,     0,     0,   146,   157,     0,     0,     0,     0,
     161,     0,     0,  1193,     0,   161,     0,     0,   146,     0,
     146,     0,   164,     0,   164,     0,     0,     0,     0,   164,
       0,     0,   164,     0,     0,     0,     0,     0,     0,     0,
     152,     0,     0,     0,     0,   164,     0,   164,     0,     0,
     164,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   157,     0,     0,     0,     0,   216,     0,     0,     0,
      22,    23,     0,     0,     0,     0,   157,     0,   164,     0,
     217,   146,    31,   218,     0,   146,     0,   161,    37,   164,
       0,   164,     0,   146,     0,    42,     0,     0,     0,     0,
       0,   152,   152,   152,   152,     0,     0,     0,     0,     0,
     996,   997,     0,   134,     0,     0,   134,     0,     0,     0,
      57,     0,    59,     0,  1004,     0,     0,     0,   152,   152,
       0,   221,     0,    67,     0,     0,  1015,     0,   162,  1018,
       0,  1019,     0,  1020,     0,     0,     0,     0,   162,     0,
       0,     0,    83,   157,     0,    85,     0,     0,    87,     0,
       0,   162,     0,     0,   162,     0,     0,     0,     0,  1149,
       0,     0,     0,     0,   134,     0,     0,     0,     0,     0,
     134,     0,     0,   216,   157,   161,     0,    22,    23,   164,
     157,   161,     0,     0,     0,     0,   164,   217,     0,    31,
     218,     0,     0,     0,   157,    37,   157,     0,     0,     0,
       0,   164,    42,   162,     0,     0,     0,     0,     0,   146,
       0,     0,   146,     0,     0,     0,     0,     0,     0,   164,
       0,     0,     0,     0,     0,     0,     0,    57,   164,    59,
     162,     0,   164,  1071,     0,   162,  1072,   161,   221,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   161,     0,     0,     0,   134,   157,   134,    83,
       0,   157,    85,   134,     0,    87,   134,     0,     0,   157,
     146,     0,     0,     0,     0,     0,   146,     0,     0,   134,
       0,   134,     0,     0,   134,     0,   164,     0,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,     0,     0,     0,     0,   162,     0,     0,
       0,     0,   134,     0,     0,   102,     0,     0,   166,  -590,
       0,     0,     0,   134,     0,   134,  1134,     0,   166,   161,
       0,     0,   418,     0,     0,  -629,     0,  -629,     0,     0,
       0,   166,     0,     0,   166,     0,     0,   164,   164,   164,
     164,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     161,     0,   146,     0,   146,     0,   161,     0,     0,   146,
       0,     0,   146,   167,   164,   164,     0,     0,     0,     0,
     161,     0,   161,   167,     0,   146,     0,   146,     0,     0,
     146,     0,     0,   166,     0,   157,   167,     0,   157,   167,
       0,     0,     0,     0,     0,   162,     0,     0,     0,     0,
       0,   162,     0,     0,     0,     0,     0,     0,   146,     0,
     166,     0,     0,   134,     0,   166,     0,     0,     0,   146,
     134,   146,     0,     0,     0,   216,     0,     0,     0,    22,
      23,     0,     0,   161,  1070,   134,     0,   161,   167,   217,
       0,    31,   218,     0,     0,   161,   157,    37,     0,     0,
       0,     0,   157,   134,    42,     0,     0,   162,     0,     0,
       0,     0,   134,     0,     0,   167,   134,     0,     0,     0,
     167,     0,   162,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,    61,     0,  1071,     0,   166,  1072,     0,
     221,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,   146,
     134,     0,     0,     0,     0,     0,   146,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   157,     0,
     157,   146,   167,     0,     0,   157,     0,     0,   157,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
       0,   157,     0,   157,     0,     0,   157,   102,   146,     0,
       0,   161,   146,  1073,   161,     0,     0,     0,     0,     0,
     162,   134,   134,   134,   134,   166,   162,     0,     0,     0,
       0,   166,     0,     0,   157,     0,     0,     0,     0,     0,
     162,     0,   162,     0,     0,   157,     0,   157,   134,   134,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,     0,   146,     0,     0,     0,
       0,     0,   161,     0,     0,     0,     0,     0,   161,     0,
     167,   276,   278,   279,   216,     0,   167,   166,    22,    23,
       0,     0,     0,     0,   418,     0,     0,  -629,   217,  -629,
      31,   218,   166,   162,     0,     0,    37,   162,     0,     0,
       0,     0,     0,    42,     0,   162,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,   146,   146,   146,
     146,     0,     0,     0,     0,     0,     0,     0,    57,     0,
      59,     0,   167,     0,  1071,   157,     0,  1072,     0,   221,
       0,    67,   157,     0,   146,   146,     0,   167,     0,     0,
       0,     0,     0,     0,   161,     0,   161,   157,     0,     0,
      83,   161,     0,    85,   161,     0,    87,     0,     0,   166,
     362,     0,   363,     0,     0,   157,     0,   161,     0,   161,
       0,     0,   161,     0,   157,     0,     0,     0,   157,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,     0,     0,     0,     0,     0,   166,     0,     0,     0,
     161,     0,     0,     0,     0,     0,   102,     0,   430,     0,
     166,   161,   166,   161,   167,     0,     0,     0,     0,     0,
       0,   162,     0,     0,   162,   327,     0,     0,     0,     0,
       0,     0,   157,     0,   110,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,   167,     0,     0,     0,   327,
       0,   167,   461,     0,     0,     0,     0,   212,     0,     0,
     223,     0,     0,   482,   483,   167,     0,   167,     0,     0,
       0,     0,     0,   166,     0,     0,     0,   166,     0,     0,
       0,     0,   162,     0,     0,   166,     0,     0,   162,     0,
       0,     0,     0,   157,   157,   157,   157,     0,   502,     0,
     216,     0,     0,     0,    22,    23,     0,     0,     0,   317,
       0,   161,     0,     0,   217,     0,    31,   218,   161,     0,
     157,   157,    37,     0,     0,   -81,     0,     0,   167,    42,
       0,     0,   167,   161,     0,     0,   110,     0,     0,     0,
     167,   355,     0,   -52,     0,     0,     0,     0,     0,     0,
       0,   161,     0,     0,    57,     0,    59,     0,     0,     0,
     161,     0,   -52,     0,   161,   221,   216,    67,     0,     0,
      22,    23,     0,     0,   162,  1070,   162,     0,   566,     0,
     217,   162,    31,   218,   162,     0,    83,     0,    37,    85,
       0,   -81,    87,     0,     0,    42,     0,   162,     0,   162,
       0,     0,   162,     0,     0,     0,     0,     0,     0,     0,
       0,   166,     0,   425,   166,     0,     0,     0,   161,     0,
      57,     0,    59,     0,    61,     0,  1071,     0,     0,  1072,
     162,   221,     0,    67,   216,   375,     0,     0,    22,    23,
       0,   162,     0,   162,     0,     0,     0,     0,   217,   639,
      31,   218,    83,     0,     0,    85,    37,     0,    87,     0,
       0,     0,     0,    42,     0,     0,   167,     0,   651,   167,
       0,     0,   166,   655,     0,   657,     0,   -51,   166,   161,
     161,   161,   161,     0,     0,     0,     0,     0,    57,     0,
      59,   376,     0,     0,   219,   377,   -51,   220,     0,   221,
       0,    67,     0,     0,     0,     0,   161,   161,   102,     0,
       0,   497,   693,     0,  1168,     0,     0,   510,   502,     0,
      83,     0,     0,    85,     0,     0,    87,   167,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
       0,   162,   379,   380,     0,   381,   382,   383,   162,   384,
     385,   386,     0,     0,   388,   389,   390,     0,     0,   392,
     393,   394,   395,   162,   166,   398,   166,     0,   399,   400,
     401,   166,     0,   110,   166,     0,   102,   758,     0,   402,
       0,   162,     0,   772,     0,   252,   774,   166,   110,   166,
     162,     0,   166,     0,   162,     0,     0,     0,     0,   253,
     254,     0,   255,     0,     0,     0,     0,   256,     0,     0,
       0,     0,     0,   773,     0,     0,   257,     0,     0,   167,
     166,   167,   258,     0,     0,     0,   167,     0,   259,   167,
       0,   166,   260,   166,     0,   261,     0,     0,     0,     0,
       0,     0,   167,     0,   167,   262,     0,   167,   162,     0,
       0,   263,   264,     0,     0,     0,     0,     0,     0,   265,
       0,     0,     0,     0,     0,   110,     0,   827,   266,     0,
     830,   831,     0,     0,     0,   167,     0,   267,   268,     0,
     269,     0,   270,     0,   271,     0,   167,   272,   167,     0,
       0,   273,     0,     0,   274,     0,   510,   275,     0,     0,
       0,     0,   510,     0,     0,     0,     0,     0,     0,   162,
     162,   162,   162,     0,     0,     0,   684,     0,     0,     0,
       0,     0,     0,     0,   878,     0,     0,     0,   502,     0,
       0,   166,     0,     0,     0,   252,   162,   162,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   253,
     254,   901,   255,   166,     0,   327,     0,   256,     0,     0,
       0,     0,     0,     0,     0,     0,   257,     0,     0,     0,
       0,   166,   258,     0,     0,     0,     0,     0,   259,   745,
     166,     0,   260,   750,   166,   261,   167,     0,     0,     0,
     774,   110,     0,   167,     0,   262,     0,     0,     0,     0,
     975,   263,   264,     0,     0,     0,     0,     0,   167,   265,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
       0,     0,     0,     0,     0,     0,   167,   267,   268,     0,
     269,     0,   270,     0,   271,   167,     0,   272,   166,   167,
       0,   273,   500,     0,   274,     0,     0,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1028,     0,   693,     0,     0,
       0,  1032,     0,   167,   502,     0,     0,     0,     0,   166,
     166,   166,   166,   501,     0,     0,     0,     0,   216,     0,
       0,     0,    22,    23,     0,     0,   901,   862,     0,     0,
     865,     0,   217,     0,    31,   218,   166,   166,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    42,     0,     0,
     774,     0,     0,   774,     0,     0,   252,     0,     0,     0,
       0,   -52,     0,     0,   167,   167,   167,   167,     0,     0,
     253,   254,    57,   255,    59,     0,     0,     0,   256,     0,
     -52,     0,     0,   221,     0,    67,     0,   257,   904,     0,
       0,   167,   167,   258,   909,     0,     0,     0,     0,   259,
       0,     0,     0,   260,    83,     0,   261,    85,     0,     0,
      87,     0,     0,     0,     0,     0,   262,     0,     0,     0,
       0,     0,   263,   264,     0,     0,     0,     0,     0,     0,
     265,     0,     0,     0,     0,     0,     0,     0,     0,   266,
       0,     0,     0,     0,     0,     0,     0,     0,   267,   268,
       0,   269,     0,   270,     0,   271,     0,     0,   272,     0,
       0,     0,   273,     0,     0,   274,     0,     0,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,     0,   510,     0,     0,     0,     0,   510,     0,     0,
     510,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1023,     0,  1024,     0,     0,  1026,     0,
       0,     0,   975,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,   899,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1046,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1052,     0,  1056,
      -2,     4,     0,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -80,     0,    40,
      41,    42,     0,    43,  -320,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -320,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -320,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -80,    87,    88,     0,     0,    89,  1113,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1133,     0,    92,
      93,    94,    95,    96,     0,     0,  1138,     0,    97,     0,
    1140,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     0,   107,     0,     0,     0,
       0,     0,     0,     0,     4,     0,     5,     0,     6,     7,
       8,     9,    10,    11,     0,  -659,     0,    12,    13,    14,
      15,    16,  -659,    17,  1178,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,  -659,    28,    29,  -659,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -80,     0,    40,    41,    42,     0,    43,  -320,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -51,    52,
      53,     0,    54,    55,    56,     0,  -320,     0,     0,    57,
      58,    59,    60,     0,    62,    63,  -320,   -51,    64,    65,
      66,  -659,    67,    68,    69,  -659,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -80,    87,    88,     0,
       0,    89,     0,    90,     0,     0,  -659,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -659,  -659,    94,  -659,  -659,  -659,  -659,  -659,
    -659,  -659,     0,  -659,  -659,  -659,  -659,  -659,     0,  -659,
    -659,  -659,  -659,  -659,  -659,  -659,  -659,   102,  -659,  -659,
    -659,     0,   104,  -659,   105,     0,   106,     0,   338,  -659,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -80,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -320,     0,     0,    57,    58,    59,    60,    61,    62,    63,
    -320,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -80,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,   339,   105,     0,
     106,     0,     4,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -80,     0,
      40,    41,    42,     0,    43,  -320,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -51,    52,    53,     0,
      54,    55,    56,     0,  -320,     0,     0,    57,    58,    59,
      60,    61,    62,    63,  -320,   -51,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -80,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,   538,   105,     0,   106,     0,   557,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -80,     0,    40,    41,    42,     0,    43,  -320,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -51,    52,    53,     0,    54,    55,    56,     0,  -320,     0,
       0,    57,    58,    59,    60,    61,    62,    63,  -320,   -51,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -80,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,   558,   105,     0,   106,     0,
     338,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -80,     0,    40,    41,
      42,     0,    43,  -320,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -51,    52,    53,     0,    54,    55,
      56,     0,  -320,     0,     0,    57,    58,    59,    60,    61,
      62,    63,  -320,   -51,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -80,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,   339,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -80,     0,    40,    41,    42,     0,    43,  -320,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -51,    52,
      53,     0,    54,    55,    56,     0,  -320,     0,     0,    57,
      58,    59,    60,    61,    62,    63,  -320,   -51,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -80,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,   757,   105,     0,   106,     0,     4,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -80,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -320,     0,     0,    57,    58,    59,    60,   354,    62,    63,
    -320,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -80,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,     4,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -80,     0,
      40,    41,    42,     0,    43,  -320,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -51,    52,    53,     0,
      54,    55,    56,     0,  -320,     0,     0,    57,    58,    59,
      60,     0,    62,    63,  -320,   -51,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -80,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,     0,   105,     0,   106,     0,     4,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
     661,    39,   -80,     0,    40,    41,    42,     0,    43,  -320,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -51,    52,    53,     0,    54,    55,    56,     0,  -320,     0,
       0,    57,    58,    59,    60,     0,    62,    63,  -320,   -51,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -80,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,     0,   105,     0,   106,     0,
       4,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,   667,    39,   -80,     0,    40,    41,
      42,     0,    43,  -320,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -51,    52,    53,     0,    54,    55,
      56,     0,  -320,     0,     0,    57,    58,    59,    60,     0,
      62,    63,  -320,   -51,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -80,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,     0,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1006,    39,
     -80,     0,    40,    41,    42,     0,    43,  -320,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -51,    52,
      53,     0,    54,    55,    56,     0,  -320,     0,     0,    57,
      58,    59,    60,     0,    62,    63,  -320,   -51,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -80,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,     0,   105,     0,   106,     0,     4,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1008,    39,   -80,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -320,     0,     0,    57,    58,    59,    60,     0,    62,    63,
    -320,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -80,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,     4,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1013,    39,   -80,     0,
      40,    41,    42,     0,    43,  -320,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -51,    52,    53,     0,
      54,    55,    56,     0,  -320,     0,     0,    57,    58,    59,
      60,     0,    62,    63,  -320,   -51,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -80,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,     0,   105,     0,   106,     0,     4,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1016,    39,   -80,     0,    40,    41,    42,     0,    43,  -320,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -51,    52,    53,     0,    54,    55,    56,     0,  -320,     0,
       0,    57,    58,    59,    60,     0,    62,    63,  -320,   -51,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -80,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,     0,   105,     0,   106,     0,
       4,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,  1045,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -80,     0,    40,    41,
      42,     0,    43,  -320,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -51,    52,    53,     0,    54,    55,
      56,     0,  -320,     0,     0,    57,    58,    59,    60,     0,
      62,    63,  -320,   -51,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -80,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,     0,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1051,    39,
     -80,     0,    40,    41,    42,     0,    43,  -320,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -51,    52,
      53,     0,    54,    55,    56,     0,  -320,     0,     0,    57,
      58,    59,    60,     0,    62,    63,  -320,   -51,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -80,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,     0,   105,     0,   106,     0,     4,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1055,    39,   -80,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -320,     0,     0,    57,    58,    59,    60,     0,    62,    63,
    -320,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -80,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,  1252,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,  1253,   887,   107,     5,   301,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,   243,    23,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,    59,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,    67,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,    87,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     0,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,   305,   306,     0,
      84,   343,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
     344,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     0,   107,     5,   301,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,   305,   306,     0,    84,   343,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,   741,   195,     0,
       0,   107,     5,   301,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,   305,   306,     0,    84,   343,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,   743,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,   622,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,   623,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
     624,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,   625,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,   229,    10,    11,   230,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     231,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,   232,    84,     0,   233,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,   305,   306,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   307,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       0,   107,     5,   301,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,   305,   306,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,   889,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,   305,   306,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,   235,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,   236,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,   246,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,   247,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       0,   107,     5,   301,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
     236,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,   229,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   231,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,   232,
      84,     0,   233,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,   305,
     306,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     0,   107,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,   739,   105,     0,
     195,     0,     0,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,   749,     0,   107,     5,   301,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,  1182,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,   236,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,   285,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   104,     0,
     105,     0,   195,     0,     0,   107,     5,   301,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,   754,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,   915,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     0,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   713,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,   714,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,   715,    74,    75,
      76,   716,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,  1117,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,  1142,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     0,   107,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
    1117,     0,     0,   107,     5,   301,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,  1096,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,   225,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     190,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   226,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   104,     0,   105,     0,
     195,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,   190,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   676,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   713,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,   714,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,   715,    74,    75,
      76,   716,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   717,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   717,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
     811,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,   814,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,  1090,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,  1091,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,   195,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,  1093,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   194,     0,   105,     0,   195,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   185,
       0,     0,    15,    16,     0,    17,     0,   186,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   187,     0,     0,     0,    33,   188,   189,     0,     0,
    1094,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   191,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   192,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   193,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   194,     0,   105,     0,   195,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   185,     0,     0,    15,    16,     0,    17,     0,   186,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   187,     0,     0,     0,    33,   188,   189,
       0,     0,  1095,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   191,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   192,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   193,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   194,     0,   105,     0,
     195,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   185,     0,     0,    15,    16,     0,    17,
       0,   186,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   187,     0,     0,     0,    33,
     188,   189,     0,     0,  1096,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   191,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   192,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   193,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   713,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,   714,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,   715,    74,    75,
      76,   716,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     194,     0,   105,     0,  1207,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   930,     0,   931,     0,
       0,     0,    92,    93,    94,    95,    96,   932,     0,     0,
       0,    97,   933,   254,   934,   935,     0,     0,     0,     0,
     936,    98,     0,     0,    99,   100,   101,     0,     0,   257,
     103,   187,     0,     0,   105,   937,  1207,     0,     0,   107,
       0,   938,     0,     0,     0,   260,     0,     0,   939,     0,
     940,     0,     0,     0,     0,     0,     0,     0,   941,     0,
       0,     0,     0,     0,   942,   943,     0,     0,     0,     0,
       0,     0,   265,     0,     0,     0,     0,     0,     0,     0,
       0,   944,     0,     0,     0,     0,     0,     0,     0,     0,
     267,   268,     0,   945,     0,   270,     0,   946,     0,     0,
     947,     0,     0,     0,   948,     0,     0,   274,     0,     0,
     949,     0,     0,     0,     0,     0,     0,     0,     0,   487,
     404,   405,   406,   407,   408,     0,     0,   411,   412,   413,
     414,     0,   416,   417,   950,   951,   952,   953,   954,   955,
       0,   956,     0,     0,     0,   957,   958,   959,   960,   961,
     962,   963,   964,   965,   966,   967,     0,   968,     0,     0,
     969,   970,   971,   972,     0,     5,   973,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   185,     0,     0,    15,
      16,     0,    17,     0,   186,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   187,     0,
       0,     0,    33,   188,   189,     0,     0,   190,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   191,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   192,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   193,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,     0,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,     5,   301,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   185,     0,     0,
      15,    16,     0,    17,     0,   186,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   187,
       0,     0,     0,    33,   188,   189,     0,     0,   190,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   191,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     192,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   193,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,  -487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,     0,  -487,     0,
       0,     0,  -487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,   194,     0,   105,  -487,  1117,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,  -469,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,  -469,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,  -469,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,  -469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,   101,  -469,    21,
       0,     0,     0,  -469,     0,   105,     0,  1207,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,     0,    94,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,   101,     0,    21,
       0,     0,     0,   194,     0,   105,     0,   195,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,   101,     0,    21,
       0,     0,     0,   194,     0,   105,     0,   717,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,   252,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,   253,   254,    80,
     255,     0,     0,    84,     0,   256,    86,     0,     0,    88,
       0,     0,     0,     0,   257,     0,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,   259,     0,     0,     0,
     260,     0,     0,   261,     0,    94,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,     0,     0,   263,
     264,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,   105,   266,   195,     0,   252,
       0,     0,     0,     0,     0,   267,   268,     0,   269,     0,
     270,     0,   271,   253,   254,   272,   255,     0,     0,   273,
       0,   256,   274,     0,     0,   275,     0,     0,     0,     0,
     257,     0,     0,     0,     0,     0,   258,     0,     0,     0,
       0,     0,   259,     0,     0,     0,   260,     0,     0,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,     0,     0,     0,     0,   263,   264,     0,     0,     0,
       0,     0,     0,   265,     0,     0,     0,     0,     0,     0,
       0,   281,   266,     0,     0,     0,     0,     0,     0,     0,
       0,   267,   268,     0,   269,     0,   270,     0,   271,     0,
       0,   272,     0,  1157,     0,   273,     0,     0,   274,     0,
       0,   275,   932,     0,     0,     0,     0,   253,   254,   934,
     255,     0,     0,     0,     0,   256,     0,     0,     0,     0,
       0,     0,     0,     0,   257,     0,     0,     0,     0,     0,
     937,     0,     0,     0,     0,     0,   259,     0,     0,     0,
     260,     0,     0,   261,     0,   940,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,   877,     0,   942,
     264,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   266,     0,     0,     0,
       0,     0,     0,     0,   373,   267,   268,     0,   269,     0,
     270,   374,  1158,     0,     0,   947,     0,     0,     0,   273,
       0,     0,   274,     0,   375,   275,     0,     0,     0,     0,
       0,     0,     0,     0,   487,   404,   405,   406,   407,   408,
       0,     0,   411,   412,   413,   414,     0,   416,   417,   950,
     951,   952,   953,   954,   955,     0,   956,     0,     0,     0,
     957,   958,   959,   960,   961,   962,   963,   964,   965,   966,
     967,     0,   968,     0,     0,   969,   970,   971,   972,     0,
     376,     0,     0,     0,   377,     0,     0,   373,     0,     0,
       0,     0,     0,   486,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,   378,   487,   404,   405,   406,
     407,   408,     0,     0,   411,   412,   413,   414,     0,   416,
     417,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   433,     0,   376,     0,     0,     0,   377,   402,     0,
     373,     0,     0,     0,     0,     0,  1005,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,   378,   487,
     404,   405,   406,   407,   408,     0,     0,   411,   412,   413,
     414,     0,   416,   417,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,   433,     0,   376,     0,     0,     0,
     377,   402,     0,   373,     0,     0,     0,     0,     0,  1012,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,   378,   487,   404,   405,   406,   407,   408,     0,     0,
     411,   412,   413,   414,     0,   416,   417,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   433,     0,   376,
       0,     0,     0,   377,   402,     0,   373,     0,     0,     0,
       0,     0,  1173,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,   378,   487,   404,   405,   406,   407,
     408,     0,     0,   411,   412,   413,   414,     0,   416,   417,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     433,     0,   376,     0,     0,     0,   377,   402,     0,   373,
       0,     0,     0,     0,     0,  1174,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,   378,   487,   404,
     405,   406,   407,   408,     0,     0,   411,   412,   413,   414,
       0,   416,   417,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   433,     0,   376,     0,     0,     0,   377,
     402,     0,   373,     0,     0,     0,     0,     0,  1175,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
     378,   487,   404,   405,   406,   407,   408,     0,     0,   411,
     412,   413,   414,     0,   416,   417,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   433,     0,   376,     0,
       0,     0,   377,   402,     0,   373,     0,     0,     0,     0,
       0,  1176,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,   378,   487,   404,   405,   406,   407,   408,
       0,     0,   411,   412,   413,   414,     0,   416,   417,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   433,
       0,   376,     0,     0,     0,   377,   402,     0,   373,     0,
       0,     0,     0,     0,  1200,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,   378,   487,   404,   405,
     406,   407,   408,     0,     0,   411,   412,   413,   414,     0,
     416,   417,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,     0,   433,     0,   376,     0,     0,     0,   377,   402,
       0,   373,     0,     0,     0,     0,     0,  1201,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   470,     0,     0,     0,     0,     0,     0,   378,
     487,   404,   405,   406,   407,   408,     0,   471,   411,   412,
     413,   414,     0,   416,   417,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,     0,
       0,   399,   400,   401,     0,   433,     0,   376,     0,     0,
       0,   377,   402,   373,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   285,     0,     0,     0,   467,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   433,   376,
       0,     0,     0,   377,     0,   402,   373,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,   464,     0,     0,
     467,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,   465,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     433,     0,   376,     0,     0,     0,   377,   402,     0,   373,
       0,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,   479,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,   480,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   433,     0,   376,     0,     0,     0,   377,
     402,     0,   373,   765,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,   766,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,   767,   392,   393,   394,   395,   396,   397,   398,
       0,   481,   399,   400,   401,     0,     0,     0,   376,     0,
       0,     0,   377,   402,   373,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   848,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,     0,
     376,     0,     0,     0,   377,     0,   402,   373,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   285,     0,
       0,   467,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   433,     0,   376,     0,     0,     0,   377,   402,   373,
       0,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,   467,     0,     0,     0,   378,     0,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
     375,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,   433,   376,     0,     0,     0,   377,
     373,   402,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   285,     0,     0,     0,     0,   180,     0,     0,     0,
     378,     0,     0,     0,     0,     0,   376,     0,     0,     0,
     377,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,   378,   399,   400,   401,     0,   376,     0,     0,     0,
     377,     0,   373,   402,     0,     0,     0,   379,   380,   374,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   375,   631,   392,   393,   394,   395,   396,   397,
     398,   378,     0,   399,   400,   401,     0,     0,   632,     0,
       0,     0,     0,     0,   402,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   433,   376,     0,
       0,     0,   377,   373,   402,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,   633,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,   634,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,     0,   377,   373,     0,   402,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   840,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     376,     0,     0,     0,   377,     0,   373,   402,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   433,   376,     0,     0,     0,   377,     0,   402,   373,
       0,     0,     0,     0,     0,   866,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   433,     0,   376,     0,     0,     0,   377,
     402,     0,   373,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,   467,  1064,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,  1065,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,     0,     0,   376,     0,
       0,   907,   377,   402,   373,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,     0,
     376,     0,     0,     0,   377,     0,   402,   373,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   522,     0,   376,     0,     0,     0,   377,   402,   373,
       0,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,   526,
     399,   400,   401,     0,     0,   376,     0,     0,     0,   377,
     373,   402,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,   528,   399,   400,   401,     0,   376,     0,     0,     0,
     377,   373,     0,   402,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,   530,   399,   400,   401,     0,   376,     0,     0,
       0,   377,   373,     0,   402,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,   536,   399,   400,   401,     0,   376,     0,
       0,     0,   377,   373,     0,   402,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,   553,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,     0,   377,   373,     0,   402,     0,     0,     0,
       0,   374,   635,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     376,     0,     0,     0,   377,   373,   761,   402,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   376,     0,     0,   740,   377,   373,     0,   402,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     762,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,     0,   376,     0,     0,     0,   377,   373,     0,   402,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,  -591,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   376,     0,     0,     0,   377,   373,     0,
     402,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,   180,     0,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,   376,     0,     0,     0,   377,   373,
     857,   402,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,     0,
     823,   399,   400,   401,     0,   376,     0,     0,     0,   377,
     373,     0,   402,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,   864,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   376,     0,     0,     0,
     377,   373,     0,   402,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   376,     0,     0,
       0,   377,   373,     0,   402,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   376,     0,
       0,   891,   377,   373,     0,   402,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,   892,   377,   373,     0,   402,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   988,     0,     0,     0,     0,
       0,     0,     0,   910,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     376,     0,     0,     0,   377,   373,     0,   402,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   991,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,   376,     0,     0,     0,   377,   373,     0,   402,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,     0,   376,     0,     0,     0,   377,   373,  1061,   402,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,     0,     0,   399,
     400,   401,     0,   376,     0,     0,     0,   377,   994,   373,
     402,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,     0,   376,     0,     0,     0,   377,
     373,   402,     0,     0,     0,     0,     0,   374,  1089,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   376,     0,     0,     0,
     377,   373,  1111,   402,     0,     0,     0,     0,   374,  1092,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   376,     0,     0,
       0,   377,   373,     0,   402,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   376,     0,
       0,     0,   377,   373,     0,   402,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,  1156,   377,   373,     0,   402,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
     376,     0,     0,     0,   377,  1196,     0,   402,     0,   373,
       0,     0,     0,  1216,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
       0,     0,     0,     0,     0,   376,     0,     0,   402,   377,
     373,     0,     0,     0,     0,     0,     0,   374,  1217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   376,     0,     0,     0,
     377,   373,     0,   402,     0,     0,     0,     0,   374,  1218,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,   376,     0,     0,
       0,   377,   373,     0,   402,     0,     0,     0,     0,   374,
    1219,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   376,     0,
       0,     0,   377,   373,     0,   402,     0,     0,     0,     0,
     374,  1220,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,   376,
       0,     0,     0,   377,     0,     0,   402,     0,     0,     0,
       0,     0,  1221,     0,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,  1246,   392,   393,   394,
     395,   396,   397,   398,   373,     0,   399,   400,   401,     0,
       0,   374,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,   376,   375,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
     376,   384,   385,   386,   377,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,   378,     0,     0,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   378,     0,   399,   400,   401,
       0,     0,     0,     0,     0,     0,  1275,     0,   402,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   373,     0,   399,   400,   401,
       0,     0,   374,     0,     0,     0,     0,     0,   402,   216,
       0,     0,     0,    22,    23,   375,     0,     0,  1070,     0,
       0,     0,     0,   217,     0,    31,   218,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,    61,     0,  1071,
       0,   809,  1072,     0,   221,   377,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,   102,     0,     0,     0,     0,   216,  1171,     0,   402,
      22,    23,     0,     0,     0,  1070,     0,     0,     0,     0,
     217,     0,    31,   218,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,  1071,     0,     0,  1072,
       0,   221,   216,    67,     0,     0,    22,    23,     0,     0,
       0,  1070,     0,     0,     0,     0,   217,     0,    31,   218,
       0,     0,    83,     0,    37,    85,     0,     0,    87,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,  1071,     0,     0,  1072,     0,   221,     0,    67,
       0,     0,     0,     0,     0,     0,   216,     0,   102,     0,
      22,    23,     0,     0,  1172,  1070,     0,     0,    83,     0,
     217,    85,    31,   218,    87,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,  1071,     0,     0,  1072,
       0,   221,   216,    67,   102,     0,    22,    23,     0,     0,
    1222,  1070,     0,     0,     0,     0,   217,     0,    31,   218,
       0,     0,    83,     0,    37,    85,     0,   375,    87,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   376,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,  1071,     0,     0,  1072,     0,   221,     0,    67,
       0,     0,     0,     0,     0,     0,   378,     0,   102,     0,
       0,     0,     0,   376,  1223,     0,     0,   377,    83,     0,
       0,    85,   379,   380,    87,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,     0,     0,   392,
     393,   394,   395,   396,     0,   398,     0,     0,   399,   400,
     401,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,   102,   387,   388,   389,   390,     0,
    1224,   392,   393,   394,   395,   396,     0,   398,   252,     0,
     399,   400,   401,   923,     0,     0,     0,     0,     0,     0,
       0,   402,   253,   254,     0,   255,     0,     0,     0,     0,
     256,     0,     0,     0,     0,     0,     0,     0,     0,   257,
       0,     0,   252,     0,     0,   258,     0,     0,     0,     0,
       0,   259,     0,     0,     0,   260,   253,   254,   261,   255,
       0,     0,     0,     0,   256,     0,     0,     0,   262,     0,
     773,     0,     0,   257,   263,   264,     0,     0,     0,   258,
       0,     0,   265,     0,     0,   259,     0,     0,     0,   260,
       0,   266,   261,     0,     0,     0,     0,     0,     0,     0,
     267,   268,   262,   269,     0,   270,     0,   271,   263,   264,
     272,     0,     0,     0,   273,     0,   265,   274,     0,     0,
     275,     0,     0,     0,     0,   266,     0,     0,  -298,     0,
       0,     0,     0,     0,   267,   268,     0,   269,     0,   270,
       0,   271,  -298,  -298,   272,  -298,     0,     0,   273,     0,
    -298,   274,     0,     0,   275,     0,     0,     0,     0,  -298,
       0,     0,  -308,     0,     0,  -298,     0,     0,     0,     0,
       0,  -298,     0,     0,     0,  -298,  -308,  -308,  -298,  -308,
       0,     0,     0,     0,  -308,     0,     0,     0,  -298,     0,
       0,     0,     0,  -308,  -298,  -298,     0,     0,     0,  -308,
       0,     0,  -298,     0,     0,  -308,     0,     0,     0,  -308,
       0,  -298,  -308,     0,     0,     0,     0,     0,     0,     0,
    -298,  -298,  -308,  -298,     0,  -298,     0,  -298,  -308,  -308,
    -298,     0,     0,     0,  -298,     0,  -308,  -298,     0,     0,
    -298,     0,     0,     0,     0,  -308,     0,     0,   252,     0,
       0,     0,     0,     0,  -308,  -308,     0,  -308,     0,  -308,
       0,  -308,   253,   254,  -308,   255,     0,     0,  -308,     0,
     256,  -308,     0,     0,  -308,     0,     0,     0,     0,   257,
       0,     0,  -299,     0,     0,   258,     0,     0,     0,     0,
       0,   259,     0,     0,     0,   260,  -299,  -299,   261,  -299,
       0,     0,     0,     0,  -299,     0,     0,     0,   262,     0,
       0,     0,     0,  -299,   263,   264,     0,     0,     0,  -299,
       0,     0,   265,     0,     0,  -299,     0,     0,     0,  -299,
       0,   266,  -299,     0,     0,     0,     0,     0,     0,     0,
     267,   268,  -299,   269,     0,   270,     0,   271,  -299,  -299,
     272,     0,     0,     0,   273,     0,  -299,   274,     0,     0,
     275,     0,     0,     0,     0,  -299,     0,     0,  -185,     0,
       0,     0,     0,     0,  -299,  -299,     0,  -299,     0,  -299,
       0,  -299,  -185,  -185,  -299,  -185,     0,     0,  -299,     0,
    -185,  -299,     0,     0,  -299,     0,     0,     0,     0,  -185,
       0,     0,  -177,     0,     0,  -185,     0,     0,     0,     0,
       0,  -185,     0,     0,     0,  -185,  -177,  -177,  -185,  -177,
       0,     0,     0,     0,  -177,     0,     0,     0,  -185,     0,
       0,     0,     0,  -177,  -185,  -185,     0,     0,     0,  -177,
       0,     0,  -185,     0,     0,  -177,     0,     0,     0,  -177,
       0,  -185,  -177,     0,     0,     0,     0,     0,     0,     0,
    -185,  -185,  -177,  -185,     0,  -185,     0,  -185,  -177,  -177,
    -185,     0,     0,     0,  -185,     0,  -177,  -185,     0,     0,
    -185,     0,     0,     0,     0,  -177,     0,     0,     0,     0,
       0,     0,     0,     0,  -177,  -177,     0,  -177,     0,  -177,
       0,  -177,     0,     0,  -177,     0,     0,     0,  -177,     0,
       0,  -177,     0,     0,  -177
};

static const yytype_int16 yycheck[] =
{
      12,    64,   424,     2,    16,   208,     2,   531,    20,    19,
     164,   665,   494,   150,    26,   480,   501,   783,   712,    31,
      32,    22,    34,    35,    36,    37,    38,   978,   979,   232,
     767,   766,    27,   524,     1,   921,     1,    49,   506,    51,
     886,    68,  1127,   773,    56,  1035,    58,    56,    60,    33,
      61,     1,  1032,   101,   224,   251,    68,    69,    70,    71,
      72,    73,    33,   531,    27,    77,    18,    79,    27,    81,
      82,    48,   209,     3,    86,    33,    37,    89,    90,   106,
      92,   251,    94,   119,   119,    97,    98,     3,   100,   117,
      56,     0,   104,   105,   106,   107,    68,   651,    48,  1182,
     145,   655,   298,   657,    20,     3,   117,    74,   104,    91,
      33,    72,    89,    48,   808,    78,    33,   145,    34,    78,
     316,   169,    48,   164,    56,    77,    89,   119,   298,   174,
      89,     1,   104,   105,   106,    69,  1116,   983,   174,   174,
      89,   795,     1,    95,    48,   108,   316,    63,   160,   116,
     240,   241,   242,  1236,   636,   114,     3,   220,   649,   124,
     112,   119,   171,  1153,    48,    34,    35,    36,   195,   323,
    1255,   105,   102,   185,   124,    48,   188,   189,   190,   144,
     145,   193,   194,   195,  1070,   169,   102,   169,  1074,   883,
    1036,    61,   108,   923,   144,   145,   119,   207,   169,    48,
    1115,    47,    61,   170,   102,   171,   218,   172,  1159,  1160,
     145,   169,   224,  1099,   164,   385,  1102,  1103,  1236,   145,
     215,    91,   194,   195,   420,   421,   711,   228,   145,   124,
      56,   145,    91,   234,    48,   119,   299,   106,    74,   251,
     145,   145,   243,   665,    91,   104,   169,   117,   174,    27,
     420,   421,   169,   169,   149,   102,   170,  1275,   117,   349,
     350,   145,   145,  1149,   145,   170,  1181,   113,    91,  1115,
     174,    27,   145,   827,   144,    89,   830,   831,   171,    54,
     116,  1167,   145,  1169,  1170,   144,   298,   769,    61,   172,
     174,   172,   124,   305,   306,    51,   145,    87,    73,   169,
      78,   174,   767,   999,   316,   164,  1002,  1003,    83,   172,
     169,    89,   145,   145,    81,    82,   453,   149,    91,  1085,
      76,   111,    78,   460,   494,   174,  1063,  1062,  1065,  1064,
     108,   104,   344,    89,   170,  1181,   114,  1253,   820,   172,
     172,   164,   341,   164,   117,   341,   169,   359,   360,    88,
     171,   164,   173,    92,   524,  1271,  1010,   170,   114,   371,
     356,   373,   374,   375,   376,   377,   378,     3,   380,   381,
     382,   383,   384,   795,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   164,   877,   124,   145,    61,   169,   409,   410,   145,
     171,   124,  1098,   415,  1100,  1101,   418,   419,   420,   421,
     422,   423,   424,   145,   145,   164,    56,     8,   149,   145,
     145,   172,   171,   435,   145,    91,   172,   150,   440,   152,
     153,   443,    56,   156,   157,   462,   145,   433,   104,   228,
     172,   172,    27,    61,    27,   234,   172,   172,   913,   170,
     462,   117,   464,   465,    47,   171,    88,   173,   470,   471,
      92,   145,    27,   172,   476,    48,   636,   479,   480,    52,
     139,   171,    65,    91,   486,   409,   410,   489,   147,   649,
      56,   415,   494,    48,   418,   419,   104,    52,   172,   145,
     462,    84,    75,    78,    56,    78,   508,    80,   164,   117,
     512,   513,   498,   169,    89,   517,    89,   376,   377,    56,
      75,   145,   524,    78,   139,    80,   172,    65,  1042,   144,
     113,    32,   147,   108,    89,   108,   151,    99,   540,   145,
     164,   543,   171,   545,   173,   738,    84,   549,   550,   551,
     567,   553,   554,   108,    82,   517,   164,    85,   164,    56,
     145,   169,   164,   422,   423,   567,   145,   145,   170,   169,
     559,   171,   139,   559,    56,    27,   435,  1035,   540,   164,
     147,   543,    48,   545,  1042,   164,   164,    88,   550,   669,
     670,    92,   145,   673,   674,    61,    48,   145,  1010,   172,
      52,   761,   762,   145,   169,   567,   465,   139,  1063,   769,
    1065,   164,   471,    56,    56,   147,   164,   476,    32,   543,
      56,   545,   164,    75,   171,    91,    78,    56,    80,   631,
     632,   633,   634,   635,   636,   637,   638,    89,   139,   783,
     640,   641,   145,   144,    56,   647,   147,   649,    56,    56,
     151,   117,   171,   171,   173,   173,   108,   659,   124,   661,
     820,   164,   171,   665,   173,   667,    84,   747,   748,    56,
     164,     3,   163,   171,    88,   173,   164,    49,    92,   145,
     682,   683,   543,   164,   545,   647,   155,    32,    20,   164,
     549,   164,   164,   164,   171,  1153,   164,    56,   164,    71,
     717,   164,    34,   169,   169,   118,   172,    87,   164,    81,
      82,   171,   171,   715,   716,   717,   171,   171,   171,   721,
     171,   124,     8,   647,  1182,   139,   140,   164,   142,   143,
     144,    63,   146,   147,   148,    65,   169,   151,  1196,   144,
      68,   147,   164,    88,   158,   159,   171,    92,   162,   124,
     145,   165,   166,   167,   164,   717,   172,   759,    48,   124,
     171,   763,   176,   765,   766,   767,   124,   769,    35,    35,
     102,   172,   145,   632,  1187,   634,   108,   124,  1236,   172,
     172,   172,    21,   145,   174,    32,   174,   169,   171,   174,
     164,   172,   171,   795,   139,  1253,   164,   721,   164,   144,
       2,   146,   147,   164,   164,   145,   151,   809,   810,   811,
      12,   164,   814,  1271,   159,   164,   164,  1275,   820,    35,
     165,   166,   167,    25,    35,   164,    28,  1240,  1241,  1242,
    1243,  1244,  1245,    65,   206,   145,   838,   172,   840,   172,
     145,    88,   124,   845,   170,    92,   848,   172,   170,    89,
      33,   172,    33,   855,   174,   857,   169,  1043,  1044,    35,
     169,   169,   864,   164,   144,   237,   238,   239,   240,   241,
     242,    89,   174,   171,   171,    77,   248,   249,   895,   144,
     174,   174,   170,  1043,  1044,   164,   164,   171,   147,   875,
     147,   147,   139,   895,   139,   147,     8,   144,   147,   146,
     147,   147,   104,   905,   151,   907,   278,   109,   910,   139,
      22,   913,   159,   147,    26,    27,   288,   139,   147,   147,
     167,   147,   294,   925,    36,   139,    38,    39,   147,   139,
     147,   169,    44,   895,   169,    35,   169,   164,   104,    51,
     312,  1085,   139,   164,   139,   759,   145,   172,   172,   172,
     809,   810,   172,    65,   356,   164,   164,   164,   330,   331,
     164,   174,   163,   163,    76,   492,    78,   172,    24,  1074,
      82,   973,    84,    85,    89,    87,   899,    89,   870,   181,
     324,   926,  1085,  1081,   875,   987,   988,  1255,   990,   991,
    1212,   993,   994,   703,   929,  1246,   108,  1239,    37,   111,
      31,   509,   114,  1005,  1006,    -1,  1008,    -1,  1010,    -1,
    1012,  1013,    -1,    -1,  1016,    32,    -1,    -1,    -1,  1072,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,  1041,
    1042,  1043,  1044,    -1,    -1,    -1,    -1,  1049,    61,  1051,
      -1,    -1,   164,  1055,    -1,    -1,     2,    -1,    -1,  1061,
    1062,  1063,  1064,  1065,   436,    -1,    12,    -1,    -1,    -1,
      -1,    88,    -1,  1035,    -1,    92,    -1,   279,    91,    25,
      -1,    -1,    28,   285,    -1,    -1,    -1,  1089,  1090,  1091,
    1092,  1093,  1094,  1095,  1096,  1081,    -1,   469,    -1,    -1,
    1117,   473,    -1,   475,   117,    -1,   478,    -1,    -1,  1111,
      -1,   124,    -1,    -1,    -1,  1117,    -1,    -1,    -1,    -1,
    1122,    -1,   139,   140,  1126,   142,   143,   144,    -1,   146,
     147,    77,   145,    -1,   151,    81,    82,    -1,    -1,   341,
      -1,   158,   159,    -1,    -1,   162,    -1,    -1,   165,   166,
     167,   164,    -1,    -1,   356,  1117,   169,    -1,   104,   172,
    1122,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1173,  1174,  1175,  1176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1207,    -1,    -1,    -1,  1196,    -1,    -1,    -1,  1200,  1201,
      -1,    -1,    -1,    -1,    -1,  1207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1216,  1217,  1218,  1219,  1220,  1221,
    1182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,   433,    -1,  1235,  1236,   181,  1238,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,  1207,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   464,    36,    -1,    38,    39,    -1,   470,  1271,
      -1,    44,    -1,  1275,  1236,    -1,    -1,    -1,    51,    -1,
      -1,    -1,   484,    -1,   486,    -1,    -1,    -1,    -1,    -1,
      -1,   663,   664,    -1,    -1,    88,    -1,   669,   670,    92,
       2,   673,   674,    76,    -1,    78,    -1,    80,    -1,    82,
      12,    -1,    85,  1275,    87,    -1,    89,   689,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    32,    -1,    -1,
      -1,    -1,    -1,   279,    -1,   108,    -1,    -1,   111,   285,
      -1,   114,    -1,    -1,    -1,   547,   139,   140,    -1,   551,
     143,   144,    -1,   146,   147,    -1,    -1,   559,   151,    -1,
      -1,    -1,   734,    -1,    -1,    -1,   159,    -1,    -1,    -1,
      -1,    -1,   165,   166,   167,    77,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    61,
      -1,   164,    -1,    -1,    -1,   341,   237,   238,   239,   240,
     241,   242,   104,    -1,    -1,    -1,     2,   109,    -1,    -1,
     356,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,   628,    -1,    -1,    25,
      -1,    -1,    28,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,    -1,    -1,   117,   151,   288,    -1,   821,
     822,    -1,   124,   294,   159,    -1,    -1,    -1,    -1,    -1,
     165,   166,   167,   835,    -1,   837,    -1,    -1,    -1,    -1,
      -1,   312,   844,   145,    -1,   847,    -1,    -1,   850,   181,
     852,    77,   854,   685,     1,    -1,   688,   433,    -1,   330,
     331,    -1,   164,    -1,    -1,    -1,    -1,   169,    -1,   871,
     172,    -1,    -1,    -1,    -1,    22,    -1,    -1,   104,    26,
      27,    -1,    -1,   109,    31,    -1,    -1,    -1,   464,    36,
      -1,    38,    39,    -1,   470,    -1,    -1,    44,    -1,    -1,
     902,    -1,    -1,    -1,    51,    -1,    -1,    54,   484,    -1,
     486,    -1,    -1,    32,   746,    -1,    -1,    -1,    -1,    -1,
     752,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    82,    83,    -1,    85,    -1,
      87,    -1,    89,    -1,    -1,    -1,    -1,   279,    -1,    -1,
      -1,   783,    -1,   285,    -1,   181,    -1,    -1,    -1,    -1,
      -1,   108,     2,    -1,   111,   436,    -1,   114,    -1,    88,
      -1,   547,    12,    92,    -1,   551,    -1,   809,   810,    -1,
      -1,    -1,    -1,   559,    -1,    25,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   469,    -1,
      -1,    -1,   473,    -1,   475,    -1,   838,   478,   840,   341,
      -1,    -1,    -1,   845,    -1,    -1,   848,   164,    -1,    -1,
     139,   140,     3,   170,   356,   144,    -1,   146,   147,   861,
      -1,   863,   151,    -1,   866,    -1,    17,    77,    -1,    20,
     159,    -1,    -1,    -1,    25,  1047,   165,   166,   167,    -1,
      -1,    -1,    -1,   279,    -1,    36,    -1,    -1,    -1,   285,
      -1,    -1,   894,    -1,   104,    46,    -1,    -1,    -1,   109,
      -1,    -1,    53,   905,    -1,   907,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   433,    -1,  1105,  1106,    86,    -1,  1109,  1110,   685,
      -1,    -1,   688,    -1,    -1,   341,    -1,    98,    -1,    -1,
      -1,   102,    -1,    -1,    -1,  1127,    -1,    -1,   109,    -1,
     356,    -1,   464,  1135,   115,  1137,    -1,  1139,   470,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,   484,    -1,   486,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     746,    25,    -1,  1005,    28,    -1,   752,    -1,    -1,    -1,
    1012,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   663,   664,    -1,  1027,    -1,    -1,   669,   670,
      -1,    -1,   673,   674,    -1,    -1,    -1,   433,    -1,    -1,
      -1,    -1,    -1,  1045,    -1,   547,    -1,    -1,    -1,   551,
      -1,    -1,  1054,    77,    -1,    -1,  1058,   559,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   464,   279,
      -1,    -1,    -1,    -1,   470,   285,    -1,    -1,    -1,    -1,
     104,    -1,    -1,  1255,    -1,   109,    -1,    -1,   484,    -1,
     486,    -1,   838,    -1,   840,    -1,    -1,    -1,    -1,   845,
      -1,    -1,   848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1112,    -1,    -1,    -1,    -1,   861,    -1,   863,    -1,    -1,
     866,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   341,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,   356,    -1,   894,    -1,
      36,   547,    38,    39,    -1,   551,    -1,   181,    44,   905,
      -1,   907,    -1,   559,    -1,    51,    -1,    -1,    -1,    -1,
      -1,  1173,  1174,  1175,  1176,    -1,    -1,    -1,    -1,    -1,
     821,   822,    -1,   685,    -1,    -1,   688,    -1,    -1,    -1,
      76,    -1,    78,    -1,   835,    -1,    -1,    -1,  1200,  1201,
      -1,    87,    -1,    89,    -1,    -1,   847,    -1,     2,   850,
      -1,   852,    -1,   854,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,   108,   433,    -1,   111,    -1,    -1,   114,    -1,
      -1,    25,    -1,    -1,    28,    -1,    -1,    -1,    -1,     8,
      -1,    -1,    -1,    -1,   746,    -1,    -1,    -1,    -1,    -1,
     752,    -1,    -1,    22,   464,   279,    -1,    26,    27,  1005,
     470,   285,    -1,    -1,    -1,    -1,  1012,    36,    -1,    38,
      39,    -1,    -1,    -1,   484,    44,   486,    -1,    -1,    -1,
      -1,  1027,    51,    77,    -1,    -1,    -1,    -1,    -1,   685,
      -1,    -1,   688,    -1,    -1,    -1,    -1,    -1,    -1,  1045,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,  1054,    78,
     104,    -1,  1058,    82,    -1,   109,    85,   341,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   356,    -1,    -1,    -1,   838,   547,   840,   108,
      -1,   551,   111,   845,    -1,   114,   848,    -1,    -1,   559,
     746,    -1,    -1,    -1,    -1,    -1,   752,    -1,    -1,   861,
      -1,   863,    -1,    -1,   866,    -1,  1112,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,   894,    -1,    -1,   164,    -1,    -1,     2,   155,
      -1,    -1,    -1,   905,    -1,   907,  1047,    -1,    12,   433,
      -1,    -1,   168,    -1,    -1,   171,    -1,   173,    -1,    -1,
      -1,    25,    -1,    -1,    28,    -1,    -1,  1173,  1174,  1175,
    1176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     464,    -1,   838,    -1,   840,    -1,   470,    -1,    -1,   845,
      -1,    -1,   848,     2,  1200,  1201,    -1,    -1,    -1,    -1,
     484,    -1,   486,    12,    -1,   861,    -1,   863,    -1,    -1,
     866,    -1,    -1,    77,    -1,   685,    25,    -1,   688,    28,
      -1,    -1,    -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,
      -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,   894,    -1,
     104,    -1,    -1,  1005,    -1,   109,    -1,    -1,    -1,   905,
    1012,   907,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,   547,    31,  1027,    -1,   551,    77,    36,
      -1,    38,    39,    -1,    -1,   559,   746,    44,    -1,    -1,
      -1,    -1,   752,  1045,    51,    -1,    -1,   341,    -1,    -1,
      -1,    -1,  1054,    -1,    -1,   104,  1058,    -1,    -1,    -1,
     109,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    82,    -1,   181,    85,    -1,
      87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,  1005,
    1112,    -1,    -1,    -1,    -1,    -1,  1012,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   838,    -1,
     840,  1027,   181,    -1,    -1,   845,    -1,    -1,   848,   433,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1045,
      -1,   861,    -1,   863,    -1,    -1,   866,   164,  1054,    -1,
      -1,   685,  1058,   170,   688,    -1,    -1,    -1,    -1,    -1,
     464,  1173,  1174,  1175,  1176,   279,   470,    -1,    -1,    -1,
      -1,   285,    -1,    -1,   894,    -1,    -1,    -1,    -1,    -1,
     484,    -1,   486,    -1,    -1,   905,    -1,   907,  1200,  1201,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,  1112,    -1,    -1,    -1,
      -1,    -1,   746,    -1,    -1,    -1,    -1,    -1,   752,    -1,
     279,    44,    45,    46,    22,    -1,   285,   341,    26,    27,
      -1,    -1,    -1,    -1,   168,    -1,    -1,   171,    36,   173,
      38,    39,   356,   547,    -1,    -1,    44,   551,    -1,    -1,
      -1,    -1,    -1,    51,    -1,   559,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,  1173,  1174,  1175,
    1176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,   341,    -1,    82,  1005,    -1,    85,    -1,    87,
      -1,    89,  1012,    -1,  1200,  1201,    -1,   356,    -1,    -1,
      -1,    -1,    -1,    -1,   838,    -1,   840,  1027,    -1,    -1,
     108,   845,    -1,   111,   848,    -1,   114,    -1,    -1,   433,
     143,    -1,   145,    -1,    -1,  1045,    -1,   861,    -1,   863,
      -1,    -1,   866,    -1,  1054,    -1,    -1,    -1,  1058,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     464,    -1,    -1,    -1,    -1,    -1,   470,    -1,    -1,    -1,
     894,    -1,    -1,    -1,    -1,    -1,   164,    -1,   191,    -1,
     484,   905,   486,   907,   433,    -1,    -1,    -1,    -1,    -1,
      -1,   685,    -1,    -1,   688,   208,    -1,    -1,    -1,    -1,
      -1,    -1,  1112,    -1,     2,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,   464,    -1,    -1,    -1,   232,
      -1,   470,   235,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      28,    -1,    -1,   246,   247,   484,    -1,   486,    -1,    -1,
      -1,    -1,    -1,   547,    -1,    -1,    -1,   551,    -1,    -1,
      -1,    -1,   746,    -1,    -1,   559,    -1,    -1,   752,    -1,
      -1,    -1,    -1,  1173,  1174,  1175,  1176,    -1,   281,    -1,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    77,
      -1,  1005,    -1,    -1,    36,    -1,    38,    39,  1012,    -1,
    1200,  1201,    44,    -1,    -1,    47,    -1,    -1,   547,    51,
      -1,    -1,   551,  1027,    -1,    -1,   104,    -1,    -1,    -1,
     559,   109,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1045,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,
    1054,    -1,    84,    -1,  1058,    87,    22,    89,    -1,    -1,
      26,    27,    -1,    -1,   838,    31,   840,    -1,   361,    -1,
      36,   845,    38,    39,   848,    -1,   108,    -1,    44,   111,
      -1,   113,   114,    -1,    -1,    51,    -1,   861,    -1,   863,
      -1,    -1,   866,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   685,    -1,   181,   688,    -1,    -1,    -1,  1112,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
     894,    87,    -1,    89,    22,    32,    -1,    -1,    26,    27,
      -1,   905,    -1,   907,    -1,    -1,    -1,    -1,    36,   432,
      38,    39,   108,    -1,    -1,   111,    44,    -1,   114,    -1,
      -1,    -1,    -1,    51,    -1,    -1,   685,    -1,   451,   688,
      -1,    -1,   746,   456,    -1,   458,    -1,    65,   752,  1173,
    1174,  1175,  1176,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    88,    -1,    -1,    82,    92,    84,    85,    -1,    87,
      -1,    89,    -1,    -1,    -1,    -1,  1200,  1201,   164,    -1,
      -1,   279,   495,    -1,   170,    -1,    -1,   285,   501,    -1,
     108,    -1,    -1,   111,    -1,    -1,   114,   746,    -1,    -1,
      -1,    -1,    -1,   752,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1005,   139,   140,    -1,   142,   143,   144,  1012,   146,
     147,   148,    -1,    -1,   151,   152,   153,    -1,    -1,   156,
     157,   158,   159,  1027,   838,   162,   840,    -1,   165,   166,
     167,   845,    -1,   341,   848,    -1,   164,   560,    -1,   176,
      -1,  1045,    -1,     1,    -1,     3,   569,   861,   356,   863,
    1054,    -1,   866,    -1,  1058,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,   838,
     894,   840,    40,    -1,    -1,    -1,   845,    -1,    46,   848,
      -1,   905,    50,   907,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,   861,    -1,   863,    63,    -1,   866,  1112,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,   433,    -1,   650,    86,    -1,
     653,   654,    -1,    -1,    -1,   894,    -1,    95,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,   905,   105,   907,    -1,
      -1,   109,    -1,    -1,   112,    -1,   464,   115,    -1,    -1,
      -1,    -1,   470,    -1,    -1,    -1,    -1,    -1,    -1,  1173,
    1174,  1175,  1176,    -1,    -1,    -1,   484,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   707,    -1,    -1,    -1,   711,    -1,
      -1,  1005,    -1,    -1,    -1,     3,  1200,  1201,  1012,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,   734,    20,  1027,    -1,   738,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,  1045,    40,    -1,    -1,    -1,    -1,    -1,    46,   547,
    1054,    -1,    50,   551,  1058,    53,  1005,    -1,    -1,    -1,
     773,   559,    -1,  1012,    -1,    63,    -1,    -1,    -1,    -1,
     783,    69,    70,    -1,    -1,    -1,    -1,    -1,  1027,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1045,    95,    96,    -1,
      98,    -1,   100,    -1,   102,  1054,    -1,   105,  1112,  1058,
      -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   868,    -1,   870,    -1,    -1,
      -1,   874,    -1,  1112,   877,    -1,    -1,    -1,    -1,  1173,
    1174,  1175,  1176,   171,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,   899,   685,    -1,    -1,
     688,    -1,    36,    -1,    38,    39,  1200,  1201,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
     923,    -1,    -1,   926,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    65,    -1,    -1,  1173,  1174,  1175,  1176,    -1,    -1,
      17,    18,    76,    20,    78,    -1,    -1,    -1,    25,    -1,
      84,    -1,    -1,    87,    -1,    89,    -1,    34,   746,    -1,
      -1,  1200,  1201,    40,   752,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,   108,    -1,    53,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     838,    -1,   840,    -1,    -1,    -1,    -1,   845,    -1,    -1,
     848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   861,    -1,   863,    -1,    -1,   866,    -1,
      -1,    -1,  1085,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   894,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   905,    -1,   907,
       0,     1,    -1,     3,    -1,     5,     6,     7,     8,     9,
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
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,  1027,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1045,    -1,   139,
     140,   141,   142,   143,    -1,    -1,  1054,    -1,   148,    -1,
    1058,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,  1112,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,   169,   170,   171,    -1,   173,    -1,     1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,   170,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,   170,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,   170,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,     4,     5,     6,     7,     8,
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
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,   174,     1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    78,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,    -1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,   106,   107,    -1,   109,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,   172,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,   172,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    48,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,   109,    -1,   111,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,   106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
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
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,
     109,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,   106,
     107,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,   170,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
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
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,   174,    -1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,   174,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,   176,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,   174,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
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
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
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
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
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
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
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
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
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
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
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
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
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
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
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
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    12,    -1,    -1,
      -1,   148,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    34,
     167,    36,    -1,    -1,   171,    40,   173,    -1,    -1,   176,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
      -1,   146,    -1,    -1,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,   162,    -1,    -1,
     165,   166,   167,   168,    -1,     3,   171,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,   169,    -1,   171,   172,   173,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    61,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,   104,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   163,   164,    25,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,   141,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   163,    -1,    25,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   163,    -1,    25,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    17,    18,   105,
      20,    -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    86,   173,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,
     100,    -1,   102,    17,    18,   105,    20,    -1,    -1,   109,
      -1,    25,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,    -1,
      -1,   105,    -1,     3,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,   171,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    95,    96,    -1,    98,    -1,
     100,    19,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    32,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,    -1,   146,    -1,    -1,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,   162,    -1,    -1,   165,   166,   167,   168,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   101,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,
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
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,    -1,    48,   132,   133,
     134,   135,    -1,   137,   138,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,   119,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    88,
      -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
     119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    37,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    72,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    72,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,   165,   166,   167,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      32,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    88,    -1,    -1,    -1,    92,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,   123,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    -1,    12,   176,    -1,    -1,    -1,   139,   140,    19,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,    32,    33,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,   169,    88,    -1,
      -1,    -1,    92,    12,   176,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    -1,    12,   176,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    88,    -1,    -1,    -1,    92,    -1,   176,    12,
      -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   119,    37,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    72,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    -1,    88,    -1,
      -1,   174,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    13,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,   172,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,
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
     164,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
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
      -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    13,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,   174,    12,
     176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    13,   176,    -1,    -1,    -1,    -1,    19,   101,
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
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,   174,    -1,   176,    -1,    12,
      -1,    -1,    -1,   101,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,   176,    92,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    61,   156,   157,   158,
     159,   160,   161,   162,    12,    -1,   165,   166,   167,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    88,    32,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      88,   146,   147,   148,    92,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    12,    -1,   165,   166,   167,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    22,
      -1,    -1,    -1,    26,    27,    32,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,
      -1,    88,    85,    -1,    87,    92,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,   164,    -1,    -1,    -1,    -1,    22,   170,    -1,   176,
      26,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    87,    22,    89,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,   108,    -1,    44,   111,    -1,    -1,   114,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    85,    -1,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   164,    -1,
      26,    27,    -1,    -1,   170,    31,    -1,    -1,   108,    -1,
      36,   111,    38,    39,   114,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    87,    22,    89,   164,    -1,    26,    27,    -1,    -1,
     170,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,   108,    -1,    44,   111,    -1,    32,   114,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    85,    -1,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   164,    -1,
      -1,    -1,    -1,    88,   170,    -1,    -1,    92,   108,    -1,
      -1,   111,   139,   140,   114,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,    -1,    -1,   156,
     157,   158,   159,   160,    -1,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,   164,   150,   151,   152,   153,    -1,
     170,   156,   157,   158,   159,   160,    -1,   162,     3,    -1,
     165,   166,   167,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,     3,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    17,    18,    53,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,
      31,    -1,    -1,    34,    69,    70,    -1,    -1,    -1,    40,
      -1,    -1,    77,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    86,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    63,    98,    -1,   100,    -1,   102,    69,    70,
     105,    -1,    -1,    -1,   109,    -1,    77,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    86,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,
      -1,   102,    17,    18,   105,    20,    -1,    -1,   109,    -1,
      25,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    34,
      -1,    -1,     3,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    17,    18,    53,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    34,    69,    70,    -1,    -1,    -1,    40,
      -1,    -1,    77,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    86,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    63,    98,    -1,   100,    -1,   102,    69,    70,
     105,    -1,    -1,    -1,   109,    -1,    77,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    86,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,
      -1,   102,    17,    18,   105,    20,    -1,    -1,   109,    -1,
      25,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    34,
      -1,    -1,     3,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    17,    18,    53,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    34,    69,    70,    -1,    -1,    -1,    40,
      -1,    -1,    77,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    86,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    63,    98,    -1,   100,    -1,   102,    69,    70,
     105,    -1,    -1,    -1,   109,    -1,    77,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    86,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,
      -1,   102,    17,    18,   105,    20,    -1,    -1,   109,    -1,
      25,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    34,
      -1,    -1,     3,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    17,    18,    53,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    34,    69,    70,    -1,    -1,    -1,    40,
      -1,    -1,    77,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    86,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    63,    98,    -1,   100,    -1,   102,    69,    70,
     105,    -1,    -1,    -1,   109,    -1,    77,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115
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
     185,   186,   187,   188,   189,   190,   193,   195,   201,   202,
     203,   206,   207,   211,   213,   216,   218,   220,   221,   222,
     229,   230,   232,   234,   237,   238,   239,   240,   241,   242,
     246,   247,   250,   251,   254,   255,   259,   262,   263,   289,
     292,   293,   313,   314,   315,   316,   317,   318,   319,   327,
     328,   329,   330,   331,   334,   335,   336,   337,   338,   339,
     340,   341,   343,   344,   345,   346,   347,   164,   185,   331,
     119,   320,   321,     3,   208,    14,    22,    36,    41,    42,
      45,    56,    87,   100,   169,   173,   237,   259,   313,   318,
     329,   330,   331,   334,   336,   337,   320,   331,   108,   292,
      89,   208,   185,   307,   331,     8,    22,    36,    39,    82,
      85,    87,   187,   185,   171,     8,    87,   331,   332,     8,
      11,    87,   108,   111,   332,    78,   121,   231,   331,   231,
     331,   231,   331,    26,   293,   331,    27,   114,   233,   331,
     194,   171,     3,    17,    18,    20,    25,    34,    40,    46,
      50,    53,    63,    69,    70,    77,    86,    95,    96,    98,
     100,   102,   105,   109,   112,   115,   210,   212,   210,   210,
     260,   171,   210,   294,   295,    33,   195,   215,   331,   331,
      18,    77,    95,   112,   331,   331,   331,     8,   171,   219,
     220,     4,   283,   306,   331,   106,   107,   164,   331,   333,
     331,   215,   331,   331,   331,    99,   171,   185,   331,   331,
     195,   207,   331,   334,   195,   207,   331,   210,   290,   331,
     233,   331,   331,   331,   331,   331,   331,   331,     1,   170,
     183,   196,   306,   110,   149,   283,   308,   309,   333,   231,
     306,   331,   342,   331,    80,   185,   169,    84,   191,    47,
     113,    56,   210,   210,    54,    73,    83,   278,   294,   163,
     164,   155,   331,    12,    19,    32,    88,    92,   123,   139,
     140,   142,   143,   144,   146,   147,   148,   150,   151,   152,
     153,   154,   156,   157,   158,   159,   160,   161,   162,   165,
     166,   167,   176,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   168,   269,
     171,   173,    88,    92,   171,   185,   164,   331,   331,   331,
     210,   306,    56,   169,   195,    48,   320,   290,   294,   164,
     145,   164,   187,   118,   211,   283,   310,   311,   312,   333,
      87,   227,   263,   292,    87,   111,   223,   290,   225,   263,
     292,   210,   171,   215,    33,    48,   215,   119,   215,   323,
      33,    48,   215,   323,   215,   323,    48,   215,   323,    37,
      72,   164,   210,   210,   101,   195,   101,   124,   195,   269,
      82,    85,   192,   310,   171,   171,   195,   185,   171,   271,
     110,   171,   210,   296,   297,     1,   144,   301,    48,   145,
     185,   215,   171,   171,   215,   310,   220,   145,   164,   331,
     331,   164,   169,   215,   171,   310,   164,   243,   164,   243,
     164,   124,   291,   164,   215,   215,   164,   170,   170,   183,
     145,   170,   331,   145,   172,   145,   172,   174,   323,    48,
     145,   174,   323,   122,   145,   174,     8,     1,   170,   196,
      65,   204,   205,   331,   198,   331,   210,   144,   252,   169,
     264,   164,   331,   331,   331,   331,   231,   331,   231,   331,
     331,   331,   331,   331,   331,   331,     3,    20,    34,    63,
     102,   108,   211,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
      68,   333,   333,   333,   333,   333,   310,   310,   231,   331,
     231,   331,    27,    48,    89,   114,   322,   325,   326,   331,
     347,    33,    48,    33,    48,   101,   171,    48,   174,   210,
     231,   331,   215,   164,   164,   331,   331,   124,   172,   145,
     228,   210,   294,   224,   226,   210,   164,   210,   294,    48,
     306,    45,   331,   231,   331,   171,   215,    45,   331,   231,
     331,   215,   215,   231,   331,   215,   167,   197,   200,   331,
     197,   199,   124,   124,   185,    35,   185,   331,    35,   331,
     191,   172,   311,   210,   235,   236,    27,    48,    52,    75,
      78,    89,   108,   184,   272,   273,   274,   275,   276,   261,
     297,   145,   172,    34,    50,    96,   100,   173,   214,   302,
     314,   124,   298,   331,   295,   331,   331,   172,   283,   331,
       1,   248,   312,   172,    21,   244,   302,   314,   145,   170,
     172,   172,   308,   172,   308,   185,   174,   231,   331,   174,
     185,   331,   174,   331,   174,   331,   170,   170,   210,   145,
     164,    13,   147,   145,   164,    13,    37,    72,   164,   171,
     306,   169,     1,    31,   210,   256,   257,   258,    27,    78,
      89,   108,   266,   277,   171,   164,   164,   164,   164,   164,
     164,   172,   174,    48,    89,   145,   172,    17,    20,    25,
      46,    53,    63,    70,    86,    98,   109,   115,   313,    88,
      88,    45,   231,   331,    45,   231,   331,   311,   231,   331,
     171,   320,   320,   164,   283,   333,   312,   210,   252,   164,
     210,   210,   252,   252,   164,   331,   172,   331,    33,   215,
      33,   215,   324,   325,   331,    33,   215,   323,    33,   215,
     323,   215,   323,   215,   323,   145,   164,    13,   164,   331,
     331,    35,   185,    35,    35,   185,   101,   195,    65,   172,
     145,   172,    48,    89,   275,   145,   172,   171,   210,    27,
      78,    89,   108,   279,   172,   297,   301,     1,   306,    68,
     333,   172,   172,   170,    74,   116,   170,   249,   172,   171,
     195,   210,   245,   290,   185,   174,   323,   174,   323,   185,
     122,   204,   211,   169,   331,   110,   331,   200,   199,   311,
       1,   253,   170,     8,   258,   124,   145,   170,    89,   265,
       1,     3,    12,    17,    19,    20,    25,    40,    46,    53,
      55,    63,    69,    70,    86,    98,   102,   105,   109,   115,
     139,   140,   141,   142,   143,   144,   146,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   162,   165,
     166,   167,   168,   171,   209,   210,   212,   267,   268,   269,
     313,   172,   325,   301,   313,   313,   331,    33,    33,   331,
      33,    33,   172,   174,   174,   311,   215,   215,   252,   169,
     252,   252,   169,   169,   215,   101,    45,   331,    45,   331,
     145,   172,   101,    45,   331,   215,    45,   331,   215,   215,
     215,   331,   331,   185,   185,   331,   185,    35,   210,   164,
     236,   195,   210,   274,   297,   144,   305,    89,   301,   298,
     174,    48,   174,   171,   171,    33,   185,   306,   245,   144,
     195,    45,   185,   331,   174,    45,   185,   331,   174,   331,
     197,    13,    37,    72,    37,    72,   164,   164,   172,   170,
      31,    82,    85,   170,   184,   217,   218,   258,   331,   257,
     279,   171,   270,   331,   139,   147,   270,   270,   298,   101,
      45,    45,   101,    45,    45,    45,    45,   172,   169,   253,
     169,   169,   253,   253,   331,   331,   331,   325,   331,   331,
     331,    13,    35,   185,   164,   305,   172,   173,   214,   283,
     304,   314,   149,   284,   298,    61,   117,   285,   331,   302,
     314,   310,   310,   185,   215,   172,   331,   331,   185,   331,
     185,   170,   110,   331,   200,   199,   200,   199,   164,     8,
     218,   217,     1,   144,   300,   273,   172,     3,   102,   268,
     269,   331,   331,   331,   331,   331,   331,   253,   170,   253,
     253,   170,   170,   101,   101,   101,   101,   331,   185,   284,
     298,   305,   174,   306,   283,   331,     3,    91,   102,   286,
     287,   288,   331,   195,   216,   282,   174,   172,   172,   195,
     101,   101,   164,   164,   164,   164,   218,   173,   214,   299,
     314,   104,   280,   172,   270,   270,   101,   101,   101,   101,
     101,   101,   170,   170,   170,   331,   331,   331,   331,   284,
     298,   283,   303,   304,   314,    48,   174,   288,   117,   145,
     124,   150,   152,   153,   156,   157,    61,   314,   163,   163,
     331,   331,     1,   174,   306,   285,   331,   303,   304,   331,
     287,   288,   288,   288,   288,   288,   288,   286,   174,   299,
     314,   174,   164,   281,   282,   174,   299,   314,   303
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   180,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   186,   186,   186,   187,   187,   188,   189,   189,
     189,   190,   190,   190,   191,   191,   192,   192,   192,   194,
     193,   195,   195,   195,   196,   196,   197,   197,   197,   197,
     198,   198,   198,   198,   198,   198,   199,   199,   200,   200,
     201,   201,   201,   202,   202,   202,   202,   202,   202,   202,
     203,   204,   204,   204,   205,   205,   206,   207,   207,   207,
     207,   207,   207,   208,   208,   209,   209,   209,   209,   209,
     209,   210,   210,   210,   210,   210,   210,   211,   211,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     214,   214,   214,   214,   215,   215,   216,   216,   217,   217,
     217,   218,   218,   218,   219,   218,   220,   220,   220,   220,
     220,   220,   220,   221,   221,   221,   221,   223,   222,   224,
     222,   225,   222,   226,   222,   227,   222,   228,   222,   222,
     222,   222,   222,   229,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   231,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   233,   233,   234,   234,   235,   235,   236,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   238,   238,   238,   238,   238,   238,   239,   239,
     239,   240,   240,   240,   241,   242,   242,   242,   242,   242,
     242,   243,   243,   244,   244,   244,   245,   245,   246,   247,
     247,   248,   248,   249,   249,   249,   250,   250,   251,   251,
     251,   252,   252,   253,   253,   253,   254,   254,   255,   256,
     256,   256,   257,   257,   257,   258,   258,   260,   261,   259,
     262,   262,   262,   264,   265,   263,   266,   266,   266,   266,
     266,   267,   267,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   270,   270,   271,   272,   272,   273,   273,
     274,   274,   274,   274,   274,   274,   275,   275,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   277,   277,   277,
     277,   277,   277,   278,   278,   278,   279,   279,   279,   279,
     279,   279,   280,   280,   281,   281,   282,   282,   283,   284,
     284,   284,   285,   285,   285,   285,   285,   286,   286,   287,
     287,   287,   287,   287,   287,   287,   288,   288,   289,   289,
     289,   290,   290,   291,   291,   291,   292,   292,   292,   292,
     292,   293,   293,   294,   294,   295,   295,   296,   296,   296,
     297,   297,   297,   298,   298,   298,   299,   299,   299,   299,
     299,   299,   299,   300,   300,   300,   300,   300,   301,   301,
     301,   301,   301,   302,   302,   302,   302,   303,   303,   303,
     304,   304,   304,   304,   304,   305,   305,   305,   305,   305,
     306,   306,   306,   306,   307,   307,   308,   308,   308,   309,
     309,   310,   310,   311,   311,   312,   312,   312,   312,   313,
     313,   314,   314,   314,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   317,   318,   319,   319,   319,   319,   319,
     319,   319,   319,   320,   320,   321,   322,   322,   323,   324,
     324,   325,   325,   325,   326,   326,   326,   326,   326,   326,
     327,   327,   328,   328,   328,   328,   328,   329,   329,   329,
     329,   329,   330,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     332,   332,   333,   333,   333,   334,   334,   334,   334,   335,
     335,   335,   335,   335,   336,   336,   336,   337,   337,   337,
     337,   337,   338,   338,   338,   338,   339,   339,   340,   340,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   342,   342,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   344,   344,
     344,   344,   344,   344,   344,   345,   345,   345,   345,   346,
     346,   346,   346,   347,   347,   347,   347,   347,   347,   347
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     2,     2,
       3,     2,     1,     3,     2,     1,     1,     4,     3,     4,
       4,     0,     1,     1,     0,     1,     0,     1,     1,     0,
       7,     2,     3,     3,     1,     2,     1,     3,     3,     5,
       1,     3,     3,     3,     5,     5,     0,     1,     1,     1,
       0,     1,     1,     4,     6,     8,     8,     6,     8,     8,
       4,     1,     3,     5,     1,     3,     3,     4,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     3,     1,     3,
       2,     1,     1,     2,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     5,     5,     2,     0,     8,     0,
       9,     0,     8,     0,     9,     0,     8,     0,     9,     3,
       3,     5,     5,     2,     5,     3,     3,     6,     6,     4,
       5,     5,     3,     3,     6,     5,     6,     5,     6,     3,
       4,     3,     4,     5,     6,     5,     6,     3,     4,     3,
       4,     6,     7,     6,     7,     4,     5,     4,     5,     4,
       4,     3,     6,     5,     4,     3,     6,     5,     6,     5,
       8,     7,     4,     4,     6,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     6,     4,
       7,     5,     3,     6,     2,     3,     3,     2,     2,     3,
       3,     0,     2,     2,     3,     5,     1,     3,     3,     5,
       5,     0,     2,     3,     2,     3,     6,     6,     1,     1,
       1,     0,     2,     0,     2,     3,     5,     5,     1,     1,
       2,     3,     1,     3,     2,     1,     3,     0,     0,     8,
       0,     1,     1,     0,     0,    10,     3,     3,     5,     5,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     3,     1,     3,     0,     1,
       4,     5,     4,     5,     6,     6,     0,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     0,     2,     2,     4,     4,     1,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     3,     4,
       4,     2,     4,     0,     2,     2,     1,     1,     1,     2,
       1,     4,     3,     1,     3,     3,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     3,     2,     4,     3,
       3,     4,     3,     0,     2,     2,     2,     1,     0,     2,
       2,     2,     1,     4,     4,     6,     3,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       1,     1,     3,     3,     1,     3,     1,     1,     1,     3,
       3,     0,     1,     1,     3,     3,     3,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     6,     6,     4,     9,
       9,     7,     6,     6,     4,     9,     9,     7,     4,     6,
       6,     9,     9,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     4,     1,     3,     4,     1,
       3,     4,     3,     3,     1,     1,     2,     1,     2,     1,
       1,     3,     1,     2,     2,     2,     2,     2,     8,     8,
       9,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     3,     2,     2,     2,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     4,     4,     4,     3,     3,     3,
       3,     5,     3,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     3,
       4,     3,     4,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1
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
#line 517 "chapel.ypp"
                                       { yyblock = (yyval.pblockstmt); }
#line 6672 "bison-chapel.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 522 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6678 "bison-chapel.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 523 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6684 "bison-chapel.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 530 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6690 "bison-chapel.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 535 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6696 "bison-chapel.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 536 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6702 "bison-chapel.cpp"
    break;

  case 26: /* stmt: stmt_level_expr TSEMI  */
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6708 "bison-chapel.cpp"
    break;

  case 27: /* stmt: TATOMIC stmt  */
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6714 "bison-chapel.cpp"
    break;

  case 28: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6720 "bison-chapel.cpp"
    break;

  case 29: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6726 "bison-chapel.cpp"
    break;

  case 30: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 562 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6732 "bison-chapel.cpp"
    break;

  case 31: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 563 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6738 "bison-chapel.cpp"
    break;

  case 32: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 564 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6744 "bison-chapel.cpp"
    break;

  case 33: /* stmt: TLABEL ident_def stmt  */
#line 565 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6750 "bison-chapel.cpp"
    break;

  case 34: /* stmt: TLOCAL expr do_stmt  */
#line 566 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6756 "bison-chapel.cpp"
    break;

  case 35: /* stmt: TLOCAL do_stmt  */
#line 567 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6762 "bison-chapel.cpp"
    break;

  case 36: /* stmt: TON expr do_stmt  */
#line 568 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6768 "bison-chapel.cpp"
    break;

  case 37: /* stmt: TSERIAL expr do_stmt  */
#line 569 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6774 "bison-chapel.cpp"
    break;

  case 38: /* stmt: TSERIAL do_stmt  */
#line 570 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6780 "bison-chapel.cpp"
    break;

  case 39: /* stmt: TSYNC stmt  */
#line 571 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6786 "bison-chapel.cpp"
    break;

  case 40: /* stmt: TYIELD expr TSEMI  */
#line 572 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6792 "bison-chapel.cpp"
    break;

  case 41: /* stmt: error TSEMI  */
#line 573 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6798 "bison-chapel.cpp"
    break;

  case 43: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 579 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 6804 "bison-chapel.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 581 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 6810 "bison-chapel.cpp"
    break;

  case 47: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 591 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6824 "bison-chapel.cpp"
    break;

  case 48: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 604 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6833 "bison-chapel.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 609 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6842 "bison-chapel.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 614 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6851 "bison-chapel.cpp"
    break;

  case 51: /* access_control: %empty  */
#line 621 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6857 "bison-chapel.cpp"
    break;

  case 52: /* access_control: TPUBLIC  */
#line 622 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6863 "bison-chapel.cpp"
    break;

  case 53: /* access_control: TPRIVATE  */
#line 623 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6869 "bison-chapel.cpp"
    break;

  case 54: /* opt_prototype: %empty  */
#line 627 "chapel.ypp"
             { (yyval.b) = false; }
#line 6875 "bison-chapel.cpp"
    break;

  case 55: /* opt_prototype: TPROTOTYPE  */
#line 628 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6881 "bison-chapel.cpp"
    break;

  case 56: /* include_access_control: %empty  */
#line 632 "chapel.ypp"
           { (yyval.b) = false; }
#line 6887 "bison-chapel.cpp"
    break;

  case 57: /* include_access_control: TPUBLIC  */
#line 633 "chapel.ypp"
           { (yyval.b) = false; }
#line 6893 "bison-chapel.cpp"
    break;

  case 58: /* include_access_control: TPRIVATE  */
#line 634 "chapel.ypp"
           { (yyval.b) = true; }
#line 6899 "bison-chapel.cpp"
    break;

  case 59: /* $@1: %empty  */
#line 639 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6908 "bison-chapel.cpp"
    break;

  case 60: /* include_module_stmt: TINCLUDE $@1 include_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 644 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6916 "bison-chapel.cpp"
    break;

  case 61: /* block_stmt: TLCBR TRCBR  */
#line 659 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6922 "bison-chapel.cpp"
    break;

  case 62: /* block_stmt: TLCBR stmt_ls TRCBR  */
#line 660 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6928 "bison-chapel.cpp"
    break;

  case 63: /* block_stmt: TLCBR error TRCBR  */
#line 661 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6934 "bison-chapel.cpp"
    break;

  case 64: /* stmt_ls: toplevel_stmt  */
#line 666 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6940 "bison-chapel.cpp"
    break;

  case 65: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 667 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6946 "bison-chapel.cpp"
    break;

  case 66: /* renames_ls: expr  */
#line 672 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6956 "bison-chapel.cpp"
    break;

  case 67: /* renames_ls: expr TAS expr  */
#line 677 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6966 "bison-chapel.cpp"
    break;

  case 68: /* renames_ls: renames_ls TCOMMA expr  */
#line 682 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6975 "bison-chapel.cpp"
    break;

  case 69: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 686 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6984 "bison-chapel.cpp"
    break;

  case 70: /* use_renames_ls: expr  */
#line 695 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6994 "bison-chapel.cpp"
    break;

  case 71: /* use_renames_ls: expr TAS expr  */
#line 700 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7004 "bison-chapel.cpp"
    break;

  case 72: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 705 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7014 "bison-chapel.cpp"
    break;

  case 73: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 710 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 7023 "bison-chapel.cpp"
    break;

  case 74: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 714 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7032 "bison-chapel.cpp"
    break;

  case 75: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 718 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7041 "bison-chapel.cpp"
    break;

  case 76: /* opt_only_ls: %empty  */
#line 726 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7051 "bison-chapel.cpp"
    break;

  case 78: /* except_ls: TSTAR  */
#line 735 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7061 "bison-chapel.cpp"
    break;

  case 80: /* use_access_control: %empty  */
#line 744 "chapel.ypp"
           { (yyval.b) = true; }
#line 7067 "bison-chapel.cpp"
    break;

  case 81: /* use_access_control: TPUBLIC  */
#line 745 "chapel.ypp"
           { (yyval.b) = false; }
#line 7073 "bison-chapel.cpp"
    break;

  case 82: /* use_access_control: TPRIVATE  */
#line 746 "chapel.ypp"
           { (yyval.b) = true; }
#line 7079 "bison-chapel.cpp"
    break;

  case 83: /* use_stmt: use_access_control TUSE use_renames_ls TSEMI  */
#line 750 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7085 "bison-chapel.cpp"
    break;

  case 84: /* use_stmt: use_access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 751 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7091 "bison-chapel.cpp"
    break;

  case 85: /* use_stmt: use_access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 752 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7097 "bison-chapel.cpp"
    break;

  case 86: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 753 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7103 "bison-chapel.cpp"
    break;

  case 87: /* use_stmt: use_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 754 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7109 "bison-chapel.cpp"
    break;

  case 88: /* use_stmt: use_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 755 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7115 "bison-chapel.cpp"
    break;

  case 89: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 756 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7121 "bison-chapel.cpp"
    break;

  case 90: /* import_stmt: use_access_control TIMPORT import_ls TSEMI  */
#line 760 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7128 "bison-chapel.cpp"
    break;

  case 91: /* import_expr: expr  */
#line 765 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7134 "bison-chapel.cpp"
    break;

  case 92: /* import_expr: expr TAS ident_use  */
#line 766 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7140 "bison-chapel.cpp"
    break;

  case 93: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 767 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7146 "bison-chapel.cpp"
    break;

  case 94: /* import_ls: import_expr  */
#line 771 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7152 "bison-chapel.cpp"
    break;

  case 95: /* import_ls: import_ls TCOMMA import_expr  */
#line 772 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7158 "bison-chapel.cpp"
    break;

  case 96: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 776 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7164 "bison-chapel.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 781 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7170 "bison-chapel.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 783 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7176 "bison-chapel.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 785 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7182 "bison-chapel.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 787 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7188 "bison-chapel.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 789 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7194 "bison-chapel.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 791 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7200 "bison-chapel.cpp"
    break;

  case 103: /* opt_label_ident: %empty  */
#line 797 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7206 "bison-chapel.cpp"
    break;

  case 104: /* opt_label_ident: TIDENT  */
#line 798 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7212 "bison-chapel.cpp"
    break;

  case 105: /* ident_fn_def: TIDENT  */
#line 802 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7218 "bison-chapel.cpp"
    break;

  case 106: /* ident_fn_def: TNONE  */
#line 803 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7224 "bison-chapel.cpp"
    break;

  case 107: /* ident_fn_def: TTHIS  */
#line 804 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7230 "bison-chapel.cpp"
    break;

  case 108: /* ident_fn_def: TFALSE  */
#line 805 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7236 "bison-chapel.cpp"
    break;

  case 109: /* ident_fn_def: TTRUE  */
#line 806 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7242 "bison-chapel.cpp"
    break;

  case 110: /* ident_fn_def: internal_type_ident_def  */
#line 807 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7248 "bison-chapel.cpp"
    break;

  case 111: /* ident_def: TIDENT  */
#line 810 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7254 "bison-chapel.cpp"
    break;

  case 112: /* ident_def: TNONE  */
#line 811 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7260 "bison-chapel.cpp"
    break;

  case 113: /* ident_def: TTHIS  */
#line 812 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7266 "bison-chapel.cpp"
    break;

  case 114: /* ident_def: TFALSE  */
#line 813 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7272 "bison-chapel.cpp"
    break;

  case 115: /* ident_def: TTRUE  */
#line 814 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7278 "bison-chapel.cpp"
    break;

  case 116: /* ident_def: internal_type_ident_def  */
#line 815 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7284 "bison-chapel.cpp"
    break;

  case 117: /* ident_use: TIDENT  */
#line 827 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7290 "bison-chapel.cpp"
    break;

  case 118: /* ident_use: TTHIS  */
#line 828 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7296 "bison-chapel.cpp"
    break;

  case 119: /* internal_type_ident_def: TBOOL  */
#line 839 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7302 "bison-chapel.cpp"
    break;

  case 120: /* internal_type_ident_def: TINT  */
#line 840 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7308 "bison-chapel.cpp"
    break;

  case 121: /* internal_type_ident_def: TUINT  */
#line 841 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7314 "bison-chapel.cpp"
    break;

  case 122: /* internal_type_ident_def: TREAL  */
#line 842 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7320 "bison-chapel.cpp"
    break;

  case 123: /* internal_type_ident_def: TIMAG  */
#line 843 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7326 "bison-chapel.cpp"
    break;

  case 124: /* internal_type_ident_def: TCOMPLEX  */
#line 844 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7332 "bison-chapel.cpp"
    break;

  case 125: /* internal_type_ident_def: TBYTES  */
#line 845 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7338 "bison-chapel.cpp"
    break;

  case 126: /* internal_type_ident_def: TSTRING  */
#line 846 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7344 "bison-chapel.cpp"
    break;

  case 127: /* internal_type_ident_def: TSYNC  */
#line 847 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7350 "bison-chapel.cpp"
    break;

  case 128: /* internal_type_ident_def: TSINGLE  */
#line 848 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7356 "bison-chapel.cpp"
    break;

  case 129: /* internal_type_ident_def: TOWNED  */
#line 849 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7362 "bison-chapel.cpp"
    break;

  case 130: /* internal_type_ident_def: TSHARED  */
#line 850 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7368 "bison-chapel.cpp"
    break;

  case 131: /* internal_type_ident_def: TBORROWED  */
#line 851 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7374 "bison-chapel.cpp"
    break;

  case 132: /* internal_type_ident_def: TUNMANAGED  */
#line 852 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7380 "bison-chapel.cpp"
    break;

  case 133: /* internal_type_ident_def: TDOMAIN  */
#line 853 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7386 "bison-chapel.cpp"
    break;

  case 134: /* internal_type_ident_def: TINDEX  */
#line 854 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7392 "bison-chapel.cpp"
    break;

  case 135: /* internal_type_ident_def: TLOCALE  */
#line 855 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7398 "bison-chapel.cpp"
    break;

  case 136: /* internal_type_ident_def: TNOTHING  */
#line 856 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7404 "bison-chapel.cpp"
    break;

  case 137: /* internal_type_ident_def: TVOID  */
#line 857 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7410 "bison-chapel.cpp"
    break;

  case 138: /* scalar_type: TBOOL  */
#line 861 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7416 "bison-chapel.cpp"
    break;

  case 139: /* scalar_type: TENUM  */
#line 862 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7422 "bison-chapel.cpp"
    break;

  case 140: /* scalar_type: TINT  */
#line 863 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7428 "bison-chapel.cpp"
    break;

  case 141: /* scalar_type: TUINT  */
#line 864 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7434 "bison-chapel.cpp"
    break;

  case 142: /* scalar_type: TREAL  */
#line 865 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7440 "bison-chapel.cpp"
    break;

  case 143: /* scalar_type: TIMAG  */
#line 866 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7446 "bison-chapel.cpp"
    break;

  case 144: /* scalar_type: TCOMPLEX  */
#line 867 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7452 "bison-chapel.cpp"
    break;

  case 145: /* scalar_type: TBYTES  */
#line 868 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7458 "bison-chapel.cpp"
    break;

  case 146: /* scalar_type: TSTRING  */
#line 869 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7464 "bison-chapel.cpp"
    break;

  case 147: /* scalar_type: TLOCALE  */
#line 870 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7470 "bison-chapel.cpp"
    break;

  case 148: /* scalar_type: TNOTHING  */
#line 871 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7476 "bison-chapel.cpp"
    break;

  case 149: /* scalar_type: TVOID  */
#line 872 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7482 "bison-chapel.cpp"
    break;

  case 150: /* reserved_type_ident_use: TSYNC  */
#line 879 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7488 "bison-chapel.cpp"
    break;

  case 151: /* reserved_type_ident_use: TSINGLE  */
#line 880 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7494 "bison-chapel.cpp"
    break;

  case 152: /* reserved_type_ident_use: TDOMAIN  */
#line 881 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7500 "bison-chapel.cpp"
    break;

  case 153: /* reserved_type_ident_use: TINDEX  */
#line 882 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7506 "bison-chapel.cpp"
    break;

  case 154: /* do_stmt: TDO stmt  */
#line 886 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7512 "bison-chapel.cpp"
    break;

  case 155: /* do_stmt: block_stmt  */
#line 887 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7518 "bison-chapel.cpp"
    break;

  case 156: /* return_stmt: TRETURN TSEMI  */
#line 891 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7524 "bison-chapel.cpp"
    break;

  case 157: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 892 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7530 "bison-chapel.cpp"
    break;

  case 159: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 898 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 7536 "bison-chapel.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 900 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 7542 "bison-chapel.cpp"
    break;

  case 161: /* class_level_stmt: TSEMI  */
#line 904 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7548 "bison-chapel.cpp"
    break;

  case 163: /* class_level_stmt: TPUBLIC private_decl  */
#line 906 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7554 "bison-chapel.cpp"
    break;

  case 164: /* @2: %empty  */
#line 916 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7560 "bison-chapel.cpp"
    break;

  case 165: /* class_level_stmt: TPRIVATE @2 private_decl  */
#line 917 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7566 "bison-chapel.cpp"
    break;

  case 173: /* forwarding_stmt: TFORWARDING expr TSEMI  */
#line 931 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7572 "bison-chapel.cpp"
    break;

  case 174: /* forwarding_stmt: TFORWARDING expr TEXCEPT except_ls TSEMI  */
#line 932 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7578 "bison-chapel.cpp"
    break;

  case 175: /* forwarding_stmt: TFORWARDING expr TONLY opt_only_ls TSEMI  */
#line 933 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7584 "bison-chapel.cpp"
    break;

  case 176: /* forwarding_stmt: TFORWARDING var_decl_stmt  */
#line 934 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7590 "bison-chapel.cpp"
    break;

  case 177: /* $@3: %empty  */
#line 939 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7599 "bison-chapel.cpp"
    break;

  case 178: /* extern_export_decl_stmt: TEXTERN TRECORD $@3 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 944 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7613 "bison-chapel.cpp"
    break;

  case 179: /* $@4: %empty  */
#line 955 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7622 "bison-chapel.cpp"
    break;

  case 180: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TRECORD $@4 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 960 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7637 "bison-chapel.cpp"
    break;

  case 181: /* $@5: %empty  */
#line 972 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7646 "bison-chapel.cpp"
    break;

  case 182: /* extern_export_decl_stmt: TEXTERN TUNION $@5 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 977 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7660 "bison-chapel.cpp"
    break;

  case 183: /* $@6: %empty  */
#line 988 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7669 "bison-chapel.cpp"
    break;

  case 184: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TUNION $@6 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 993 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7684 "bison-chapel.cpp"
    break;

  case 185: /* $@7: %empty  */
#line 1005 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7693 "bison-chapel.cpp"
    break;

  case 186: /* extern_export_decl_stmt: TEXPORT TRECORD $@7 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1010 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7707 "bison-chapel.cpp"
    break;

  case 187: /* $@8: %empty  */
#line 1020 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7716 "bison-chapel.cpp"
    break;

  case 188: /* extern_export_decl_stmt: TEXPORT STRINGLITERAL TRECORD $@8 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1025 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7730 "bison-chapel.cpp"
    break;

  case 189: /* extern_export_decl_stmt: TEXTERN opt_expr fn_decl_stmt  */
#line 1036 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7738 "bison-chapel.cpp"
    break;

  case 190: /* extern_export_decl_stmt: TEXPORT opt_expr fn_decl_stmt  */
#line 1040 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7746 "bison-chapel.cpp"
    break;

  case 191: /* extern_export_decl_stmt: TEXTERN opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1045 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7758 "bison-chapel.cpp"
    break;

  case 192: /* extern_export_decl_stmt: TEXPORT opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1053 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7770 "bison-chapel.cpp"
    break;

  case 193: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1064 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7778 "bison-chapel.cpp"
    break;

  case 194: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1070 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7784 "bison-chapel.cpp"
    break;

  case 195: /* loop_stmt: TWHILE expr do_stmt  */
#line 1071 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7790 "bison-chapel.cpp"
    break;

  case 196: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1072 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7796 "bison-chapel.cpp"
    break;

  case 197: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1073 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7802 "bison-chapel.cpp"
    break;

  case 198: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1074 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7808 "bison-chapel.cpp"
    break;

  case 199: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1075 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7814 "bison-chapel.cpp"
    break;

  case 200: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1076 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7820 "bison-chapel.cpp"
    break;

  case 201: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1077 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7826 "bison-chapel.cpp"
    break;

  case 202: /* loop_stmt: TFOR expr do_stmt  */
#line 1078 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7832 "bison-chapel.cpp"
    break;

  case 203: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1079 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7838 "bison-chapel.cpp"
    break;

  case 204: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1080 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7844 "bison-chapel.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1081 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7850 "bison-chapel.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1082 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7856 "bison-chapel.cpp"
    break;

  case 207: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1083 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7862 "bison-chapel.cpp"
    break;

  case 208: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1084 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7868 "bison-chapel.cpp"
    break;

  case 209: /* loop_stmt: TFORALL expr do_stmt  */
#line 1085 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7874 "bison-chapel.cpp"
    break;

  case 210: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1086 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7880 "bison-chapel.cpp"
    break;

  case 211: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1087 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7886 "bison-chapel.cpp"
    break;

  case 212: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1088 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7892 "bison-chapel.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1089 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7898 "bison-chapel.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1090 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7904 "bison-chapel.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1091 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7910 "bison-chapel.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1092 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7916 "bison-chapel.cpp"
    break;

  case 217: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1093 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7922 "bison-chapel.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1094 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7928 "bison-chapel.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1095 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7934 "bison-chapel.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1096 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7940 "bison-chapel.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1098 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7950 "bison-chapel.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1104 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7960 "bison-chapel.cpp"
    break;

  case 223: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1110 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7970 "bison-chapel.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1116 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7980 "bison-chapel.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1122 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7991 "bison-chapel.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1129 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8002 "bison-chapel.cpp"
    break;

  case 227: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1136 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8010 "bison-chapel.cpp"
    break;

  case 228: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1140 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8018 "bison-chapel.cpp"
    break;

  case 229: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1146 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 8024 "bison-chapel.cpp"
    break;

  case 230: /* if_stmt: TIF expr TTHEN stmt  */
#line 1150 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8030 "bison-chapel.cpp"
    break;

  case 231: /* if_stmt: TIF expr block_stmt  */
#line 1151 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8036 "bison-chapel.cpp"
    break;

  case 232: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1152 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8042 "bison-chapel.cpp"
    break;

  case 233: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1153 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8048 "bison-chapel.cpp"
    break;

  case 234: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1155 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8054 "bison-chapel.cpp"
    break;

  case 235: /* if_stmt: TIF ifvar block_stmt  */
#line 1156 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8060 "bison-chapel.cpp"
    break;

  case 236: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1157 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8066 "bison-chapel.cpp"
    break;

  case 237: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1158 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8072 "bison-chapel.cpp"
    break;

  case 238: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1160 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8079 "bison-chapel.cpp"
    break;

  case 239: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1162 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8086 "bison-chapel.cpp"
    break;

  case 240: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1164 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8093 "bison-chapel.cpp"
    break;

  case 241: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1166 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8100 "bison-chapel.cpp"
    break;

  case 242: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1171 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 8106 "bison-chapel.cpp"
    break;

  case 243: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1172 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 8112 "bison-chapel.cpp"
    break;

  case 244: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1177 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 8118 "bison-chapel.cpp"
    break;

  case 245: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1179 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8127 "bison-chapel.cpp"
    break;

  case 246: /* ifc_formal_ls: ifc_formal  */
#line 1186 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8133 "bison-chapel.cpp"
    break;

  case 247: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1187 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8139 "bison-chapel.cpp"
    break;

  case 248: /* ifc_formal: ident_def  */
#line 1192 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8145 "bison-chapel.cpp"
    break;

  case 249: /* implements_type_ident: TIDENT  */
#line 1197 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 8151 "bison-chapel.cpp"
    break;

  case 250: /* implements_type_ident: TBOOL  */
#line 1198 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 8157 "bison-chapel.cpp"
    break;

  case 251: /* implements_type_ident: TINT  */
#line 1199 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 8163 "bison-chapel.cpp"
    break;

  case 252: /* implements_type_ident: TUINT  */
#line 1200 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 8169 "bison-chapel.cpp"
    break;

  case 253: /* implements_type_ident: TREAL  */
#line 1201 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 8175 "bison-chapel.cpp"
    break;

  case 254: /* implements_type_ident: TIMAG  */
#line 1202 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 8181 "bison-chapel.cpp"
    break;

  case 255: /* implements_type_ident: TCOMPLEX  */
#line 1203 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 8187 "bison-chapel.cpp"
    break;

  case 256: /* implements_type_ident: TBYTES  */
#line 1204 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 8193 "bison-chapel.cpp"
    break;

  case 257: /* implements_type_ident: TSTRING  */
#line 1205 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 8199 "bison-chapel.cpp"
    break;

  case 258: /* implements_type_ident: TLOCALE  */
#line 1206 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 8205 "bison-chapel.cpp"
    break;

  case 259: /* implements_type_ident: TNOTHING  */
#line 1207 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 8211 "bison-chapel.cpp"
    break;

  case 260: /* implements_type_ident: TVOID  */
#line 1208 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 8217 "bison-chapel.cpp"
    break;

  case 261: /* implements_type_ident: implements_type_error_ident  */
#line 1210 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8224 "bison-chapel.cpp"
    break;

  case 262: /* implements_type_error_ident: TNONE  */
#line 1216 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 8230 "bison-chapel.cpp"
    break;

  case 263: /* implements_type_error_ident: TTHIS  */
#line 1217 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 8236 "bison-chapel.cpp"
    break;

  case 264: /* implements_type_error_ident: TFALSE  */
#line 1218 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 8242 "bison-chapel.cpp"
    break;

  case 265: /* implements_type_error_ident: TTRUE  */
#line 1219 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 8248 "bison-chapel.cpp"
    break;

  case 266: /* implements_type_error_ident: TDOMAIN  */
#line 1228 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 8254 "bison-chapel.cpp"
    break;

  case 267: /* implements_type_error_ident: TINDEX  */
#line 1229 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 8260 "bison-chapel.cpp"
    break;

  case 268: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1234 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8266 "bison-chapel.cpp"
    break;

  case 269: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1236 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8273 "bison-chapel.cpp"
    break;

  case 270: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1239 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8280 "bison-chapel.cpp"
    break;

  case 271: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1245 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8286 "bison-chapel.cpp"
    break;

  case 272: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1247 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8293 "bison-chapel.cpp"
    break;

  case 273: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1250 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8300 "bison-chapel.cpp"
    break;

  case 274: /* defer_stmt: TDEFER stmt  */
#line 1255 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8306 "bison-chapel.cpp"
    break;

  case 275: /* try_stmt: TTRY expr TSEMI  */
#line 1258 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 8312 "bison-chapel.cpp"
    break;

  case 276: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1259 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 8318 "bison-chapel.cpp"
    break;

  case 277: /* try_stmt: TTRY assignment_stmt  */
#line 1260 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 8324 "bison-chapel.cpp"
    break;

  case 278: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1261 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 8330 "bison-chapel.cpp"
    break;

  case 279: /* try_stmt: TTRY block_stmt catch_stmt_ls  */
#line 1262 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8336 "bison-chapel.cpp"
    break;

  case 280: /* try_stmt: TTRYBANG block_stmt catch_stmt_ls  */
#line 1263 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8342 "bison-chapel.cpp"
    break;

  case 281: /* catch_stmt_ls: %empty  */
#line 1267 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8348 "bison-chapel.cpp"
    break;

  case 282: /* catch_stmt_ls: catch_stmt_ls catch_stmt  */
#line 1268 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8354 "bison-chapel.cpp"
    break;

  case 283: /* catch_stmt: TCATCH block_stmt  */
#line 1272 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8360 "bison-chapel.cpp"
    break;

  case 284: /* catch_stmt: TCATCH catch_expr block_stmt  */
#line 1273 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8366 "bison-chapel.cpp"
    break;

  case 285: /* catch_stmt: TCATCH TLP catch_expr TRP block_stmt  */
#line 1274 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8372 "bison-chapel.cpp"
    break;

  case 286: /* catch_expr: ident_def  */
#line 1278 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8378 "bison-chapel.cpp"
    break;

  case 287: /* catch_expr: ident_def TCOLON expr  */
#line 1279 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8384 "bison-chapel.cpp"
    break;

  case 288: /* throw_stmt: TTHROW expr TSEMI  */
#line 1283 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8390 "bison-chapel.cpp"
    break;

  case 289: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1287 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8396 "bison-chapel.cpp"
    break;

  case 290: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1289 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8402 "bison-chapel.cpp"
    break;

  case 291: /* when_stmt_ls: %empty  */
#line 1293 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8408 "bison-chapel.cpp"
    break;

  case 292: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1294 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8414 "bison-chapel.cpp"
    break;

  case 293: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1299 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8420 "bison-chapel.cpp"
    break;

  case 294: /* when_stmt: TOTHERWISE stmt  */
#line 1301 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8426 "bison-chapel.cpp"
    break;

  case 295: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1303 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8432 "bison-chapel.cpp"
    break;

  case 296: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1310 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8446 "bison-chapel.cpp"
    break;

  case 297: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR error TRCBR  */
#line 1320 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8460 "bison-chapel.cpp"
    break;

  case 298: /* class_tag: TCLASS  */
#line 1333 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8470 "bison-chapel.cpp"
    break;

  case 299: /* class_tag: TRECORD  */
#line 1338 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8480 "bison-chapel.cpp"
    break;

  case 300: /* class_tag: TUNION  */
#line 1343 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8490 "bison-chapel.cpp"
    break;

  case 301: /* opt_inherit: %empty  */
#line 1351 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8496 "bison-chapel.cpp"
    break;

  case 302: /* opt_inherit: TCOLON expr_ls  */
#line 1352 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8502 "bison-chapel.cpp"
    break;

  case 303: /* class_level_stmt_ls: %empty  */
#line 1357 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8508 "bison-chapel.cpp"
    break;

  case 304: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1359 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8514 "bison-chapel.cpp"
    break;

  case 305: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1361 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8520 "bison-chapel.cpp"
    break;

  case 306: /* enum_decl_stmt: enum_header ident_def TLCBR enum_ls TRCBR  */
#line 1366 "chapel.ypp"
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
#line 8540 "bison-chapel.cpp"
    break;

  case 307: /* enum_decl_stmt: enum_header ident_def TLCBR error TRCBR  */
#line 1382 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8548 "bison-chapel.cpp"
    break;

  case 308: /* enum_header: TENUM  */
#line 1389 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8558 "bison-chapel.cpp"
    break;

  case 309: /* enum_ls: deprecated_enum_item  */
#line 1398 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8570 "bison-chapel.cpp"
    break;

  case 310: /* enum_ls: enum_ls TCOMMA  */
#line 1406 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8578 "bison-chapel.cpp"
    break;

  case 311: /* enum_ls: enum_ls TCOMMA deprecated_enum_item  */
#line 1410 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8586 "bison-chapel.cpp"
    break;

  case 313: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1418 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr), (yyvsp[-1].pch)); }
#line 8592 "bison-chapel.cpp"
    break;

  case 314: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1420 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr)); }
#line 8598 "bison-chapel.cpp"
    break;

  case 315: /* enum_item: ident_def  */
#line 1424 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8604 "bison-chapel.cpp"
    break;

  case 316: /* enum_item: ident_def TASSIGN expr  */
#line 1425 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8610 "bison-chapel.cpp"
    break;

  case 317: /* $@9: %empty  */
#line 1430 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8619 "bison-chapel.cpp"
    break;

  case 318: /* $@10: %empty  */
#line 1435 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8628 "bison-chapel.cpp"
    break;

  case 319: /* lambda_decl_expr: TLAMBDA $@9 req_formal_ls $@10 opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1440 "chapel.ypp"
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
#line 8650 "bison-chapel.cpp"
    break;

  case 320: /* linkage_spec: %empty  */
#line 1462 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8658 "bison-chapel.cpp"
    break;

  case 321: /* linkage_spec: TINLINE  */
#line 1465 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8667 "bison-chapel.cpp"
    break;

  case 322: /* linkage_spec: TOVERRIDE  */
#line 1469 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8676 "bison-chapel.cpp"
    break;

  case 323: /* $@11: %empty  */
#line 1477 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8690 "bison-chapel.cpp"
    break;

  case 324: /* $@12: %empty  */
#line 1487 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8701 "bison-chapel.cpp"
    break;

  case 325: /* fn_decl_stmt: linkage_spec proc_iter_or_op $@11 fn_decl_stmt_inner $@12 opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where opt_function_body_stmt  */
#line 1494 "chapel.ypp"
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
#line 8735 "bison-chapel.cpp"
    break;

  case 326: /* fn_decl_stmt_inner: opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1527 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8743 "bison-chapel.cpp"
    break;

  case 327: /* fn_decl_stmt_inner: opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1531 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8752 "bison-chapel.cpp"
    break;

  case 328: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1536 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8760 "bison-chapel.cpp"
    break;

  case 329: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1540 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8769 "bison-chapel.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: opt_this_intent_tag error opt_formal_ls  */
#line 1545 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8777 "bison-chapel.cpp"
    break;

  case 332: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1552 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8783 "bison-chapel.cpp"
    break;

  case 333: /* fn_ident: ident_fn_def  */
#line 1556 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8789 "bison-chapel.cpp"
    break;

  case 334: /* fn_ident: TBAND  */
#line 1557 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8795 "bison-chapel.cpp"
    break;

  case 335: /* fn_ident: TBOR  */
#line 1558 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8801 "bison-chapel.cpp"
    break;

  case 336: /* fn_ident: TBXOR  */
#line 1559 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8807 "bison-chapel.cpp"
    break;

  case 337: /* fn_ident: TBNOT  */
#line 1560 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8813 "bison-chapel.cpp"
    break;

  case 338: /* fn_ident: TEQUAL  */
#line 1561 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8819 "bison-chapel.cpp"
    break;

  case 339: /* fn_ident: TNOTEQUAL  */
#line 1562 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8825 "bison-chapel.cpp"
    break;

  case 340: /* fn_ident: TLESSEQUAL  */
#line 1563 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8831 "bison-chapel.cpp"
    break;

  case 341: /* fn_ident: TGREATEREQUAL  */
#line 1564 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8837 "bison-chapel.cpp"
    break;

  case 342: /* fn_ident: TLESS  */
#line 1565 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8843 "bison-chapel.cpp"
    break;

  case 343: /* fn_ident: TGREATER  */
#line 1566 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8849 "bison-chapel.cpp"
    break;

  case 344: /* fn_ident: TPLUS  */
#line 1567 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8855 "bison-chapel.cpp"
    break;

  case 345: /* fn_ident: TMINUS  */
#line 1568 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8861 "bison-chapel.cpp"
    break;

  case 346: /* fn_ident: TSTAR  */
#line 1569 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8867 "bison-chapel.cpp"
    break;

  case 347: /* fn_ident: TDIVIDE  */
#line 1570 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8873 "bison-chapel.cpp"
    break;

  case 348: /* fn_ident: TSHIFTLEFT  */
#line 1571 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8879 "bison-chapel.cpp"
    break;

  case 349: /* fn_ident: TSHIFTRIGHT  */
#line 1572 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8885 "bison-chapel.cpp"
    break;

  case 350: /* fn_ident: TMOD  */
#line 1573 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8891 "bison-chapel.cpp"
    break;

  case 351: /* fn_ident: TEXP  */
#line 1574 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8897 "bison-chapel.cpp"
    break;

  case 352: /* fn_ident: TBANG  */
#line 1575 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8903 "bison-chapel.cpp"
    break;

  case 353: /* fn_ident: TBY  */
#line 1576 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8909 "bison-chapel.cpp"
    break;

  case 354: /* fn_ident: THASH  */
#line 1577 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8915 "bison-chapel.cpp"
    break;

  case 355: /* fn_ident: TALIGN  */
#line 1578 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8921 "bison-chapel.cpp"
    break;

  case 356: /* fn_ident: TSWAP  */
#line 1579 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8927 "bison-chapel.cpp"
    break;

  case 357: /* fn_ident: TIO  */
#line 1580 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8933 "bison-chapel.cpp"
    break;

  case 358: /* fn_ident: TINITEQUALS  */
#line 1581 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8939 "bison-chapel.cpp"
    break;

  case 359: /* fn_ident: TCOLON  */
#line 1582 "chapel.ypp"
                 { (yyval.pch) = ":"; }
#line 8945 "bison-chapel.cpp"
    break;

  case 360: /* fn_ident: ident_def TBANG  */
#line 1583 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8951 "bison-chapel.cpp"
    break;

  case 361: /* assignop_ident: TASSIGN  */
#line 1587 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8957 "bison-chapel.cpp"
    break;

  case 362: /* assignop_ident: TASSIGNPLUS  */
#line 1588 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8963 "bison-chapel.cpp"
    break;

  case 363: /* assignop_ident: TASSIGNMINUS  */
#line 1589 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8969 "bison-chapel.cpp"
    break;

  case 364: /* assignop_ident: TASSIGNMULTIPLY  */
#line 1590 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8975 "bison-chapel.cpp"
    break;

  case 365: /* assignop_ident: TASSIGNDIVIDE  */
#line 1591 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8981 "bison-chapel.cpp"
    break;

  case 366: /* assignop_ident: TASSIGNMOD  */
#line 1592 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8987 "bison-chapel.cpp"
    break;

  case 367: /* assignop_ident: TASSIGNEXP  */
#line 1593 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8993 "bison-chapel.cpp"
    break;

  case 368: /* assignop_ident: TASSIGNBAND  */
#line 1594 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8999 "bison-chapel.cpp"
    break;

  case 369: /* assignop_ident: TASSIGNBOR  */
#line 1595 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 9005 "bison-chapel.cpp"
    break;

  case 370: /* assignop_ident: TASSIGNBXOR  */
#line 1596 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 9011 "bison-chapel.cpp"
    break;

  case 371: /* assignop_ident: TASSIGNSR  */
#line 1597 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 9017 "bison-chapel.cpp"
    break;

  case 372: /* assignop_ident: TASSIGNSL  */
#line 1598 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 9023 "bison-chapel.cpp"
    break;

  case 373: /* opt_formal_ls: %empty  */
#line 1602 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 9029 "bison-chapel.cpp"
    break;

  case 374: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1603 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9035 "bison-chapel.cpp"
    break;

  case 375: /* req_formal_ls: TLP formal_ls TRP  */
#line 1607 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9041 "bison-chapel.cpp"
    break;

  case 376: /* formal_ls_inner: formal  */
#line 1611 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 9047 "bison-chapel.cpp"
    break;

  case 377: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 1612 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 9053 "bison-chapel.cpp"
    break;

  case 378: /* formal_ls: %empty  */
#line 1616 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 9059 "bison-chapel.cpp"
    break;

  case 379: /* formal_ls: formal_ls_inner  */
#line 1617 "chapel.ypp"
                           { (yyval.pfnsymbol) = (yyvsp[0].pfnsymbol); }
#line 9065 "bison-chapel.cpp"
    break;

  case 380: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1622 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 9071 "bison-chapel.cpp"
    break;

  case 381: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1624 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 9077 "bison-chapel.cpp"
    break;

  case 382: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1626 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 9083 "bison-chapel.cpp"
    break;

  case 383: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1628 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 9089 "bison-chapel.cpp"
    break;

  case 384: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1630 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9095 "bison-chapel.cpp"
    break;

  case 385: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1632 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 9101 "bison-chapel.cpp"
    break;

  case 386: /* opt_intent_tag: %empty  */
#line 1636 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 9107 "bison-chapel.cpp"
    break;

  case 387: /* opt_intent_tag: required_intent_tag  */
#line 1637 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 9113 "bison-chapel.cpp"
    break;

  case 388: /* required_intent_tag: TIN  */
#line 1641 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 9119 "bison-chapel.cpp"
    break;

  case 389: /* required_intent_tag: TINOUT  */
#line 1642 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 9125 "bison-chapel.cpp"
    break;

  case 390: /* required_intent_tag: TOUT  */
#line 1643 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 9131 "bison-chapel.cpp"
    break;

  case 391: /* required_intent_tag: TCONST  */
#line 1644 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 9137 "bison-chapel.cpp"
    break;

  case 392: /* required_intent_tag: TCONST TIN  */
#line 1645 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 9143 "bison-chapel.cpp"
    break;

  case 393: /* required_intent_tag: TCONST TREF  */
#line 1646 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 9149 "bison-chapel.cpp"
    break;

  case 394: /* required_intent_tag: TPARAM  */
#line 1647 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 9155 "bison-chapel.cpp"
    break;

  case 395: /* required_intent_tag: TREF  */
#line 1648 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 9161 "bison-chapel.cpp"
    break;

  case 396: /* required_intent_tag: TTYPE  */
#line 1649 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 9167 "bison-chapel.cpp"
    break;

  case 397: /* opt_this_intent_tag: %empty  */
#line 1653 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 9173 "bison-chapel.cpp"
    break;

  case 398: /* opt_this_intent_tag: TPARAM  */
#line 1654 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 9179 "bison-chapel.cpp"
    break;

  case 399: /* opt_this_intent_tag: TREF  */
#line 1655 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 9185 "bison-chapel.cpp"
    break;

  case 400: /* opt_this_intent_tag: TCONST TREF  */
#line 1656 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 9191 "bison-chapel.cpp"
    break;

  case 401: /* opt_this_intent_tag: TCONST  */
#line 1657 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 9197 "bison-chapel.cpp"
    break;

  case 402: /* opt_this_intent_tag: TTYPE  */
#line 1658 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 9203 "bison-chapel.cpp"
    break;

  case 403: /* proc_iter_or_op: TPROC  */
#line 1662 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 9209 "bison-chapel.cpp"
    break;

  case 404: /* proc_iter_or_op: TITER  */
#line 1663 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 9215 "bison-chapel.cpp"
    break;

  case 405: /* proc_iter_or_op: TOPERATOR  */
#line 1664 "chapel.ypp"
            { (yyval.procIterOp) = ProcIterOp_OP; }
#line 9221 "bison-chapel.cpp"
    break;

  case 406: /* opt_ret_tag: %empty  */
#line 1668 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9227 "bison-chapel.cpp"
    break;

  case 407: /* opt_ret_tag: TCONST  */
#line 1669 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9233 "bison-chapel.cpp"
    break;

  case 408: /* opt_ret_tag: TCONST TREF  */
#line 1670 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 9239 "bison-chapel.cpp"
    break;

  case 409: /* opt_ret_tag: TREF  */
#line 1671 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 9245 "bison-chapel.cpp"
    break;

  case 410: /* opt_ret_tag: TPARAM  */
#line 1672 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 9251 "bison-chapel.cpp"
    break;

  case 411: /* opt_ret_tag: TTYPE  */
#line 1673 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 9257 "bison-chapel.cpp"
    break;

  case 412: /* opt_throws_error: %empty  */
#line 1677 "chapel.ypp"
          { (yyval.b) = false; }
#line 9263 "bison-chapel.cpp"
    break;

  case 413: /* opt_throws_error: TTHROWS  */
#line 1678 "chapel.ypp"
          { (yyval.b) = true;  }
#line 9269 "bison-chapel.cpp"
    break;

  case 414: /* opt_function_body_stmt: TSEMI  */
#line 1681 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 9275 "bison-chapel.cpp"
    break;

  case 417: /* function_body_stmt: return_stmt  */
#line 1687 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9281 "bison-chapel.cpp"
    break;

  case 418: /* query_expr: TQUERIEDIDENT  */
#line 1691 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9287 "bison-chapel.cpp"
    break;

  case 419: /* var_arg_expr: TDOTDOTDOT  */
#line 1695 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9293 "bison-chapel.cpp"
    break;

  case 420: /* var_arg_expr: TDOTDOTDOT expr  */
#line 1696 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9299 "bison-chapel.cpp"
    break;

  case 421: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 1697 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9309 "bison-chapel.cpp"
    break;

  case 422: /* opt_lifetime_where: %empty  */
#line 1705 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9315 "bison-chapel.cpp"
    break;

  case 423: /* opt_lifetime_where: TWHERE expr  */
#line 1707 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9321 "bison-chapel.cpp"
    break;

  case 424: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 1709 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9327 "bison-chapel.cpp"
    break;

  case 425: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 1711 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9333 "bison-chapel.cpp"
    break;

  case 426: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 1713 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9339 "bison-chapel.cpp"
    break;

  case 427: /* lifetime_components_expr: lifetime_expr  */
#line 1718 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9345 "bison-chapel.cpp"
    break;

  case 428: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 1720 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9351 "bison-chapel.cpp"
    break;

  case 429: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 1724 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9357 "bison-chapel.cpp"
    break;

  case 430: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 1725 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9363 "bison-chapel.cpp"
    break;

  case 431: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 1726 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9369 "bison-chapel.cpp"
    break;

  case 432: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 1727 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9375 "bison-chapel.cpp"
    break;

  case 433: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 1728 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9381 "bison-chapel.cpp"
    break;

  case 434: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 1729 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9387 "bison-chapel.cpp"
    break;

  case 435: /* lifetime_expr: TRETURN lifetime_ident  */
#line 1730 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9393 "bison-chapel.cpp"
    break;

  case 436: /* lifetime_ident: TIDENT  */
#line 1735 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9399 "bison-chapel.cpp"
    break;

  case 437: /* lifetime_ident: TTHIS  */
#line 1737 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9405 "bison-chapel.cpp"
    break;

  case 438: /* type_alias_decl_stmt: TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1742 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9411 "bison-chapel.cpp"
    break;

  case 439: /* type_alias_decl_stmt: TCONFIG TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1744 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9417 "bison-chapel.cpp"
    break;

  case 440: /* type_alias_decl_stmt: TEXTERN TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1746 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9423 "bison-chapel.cpp"
    break;

  case 441: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 1751 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9440 "bison-chapel.cpp"
    break;

  case 442: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 1764 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9458 "bison-chapel.cpp"
    break;

  case 443: /* opt_init_type: %empty  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9464 "bison-chapel.cpp"
    break;

  case 444: /* opt_init_type: TASSIGN type_level_expr  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9470 "bison-chapel.cpp"
    break;

  case 445: /* opt_init_type: TASSIGN array_type  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9476 "bison-chapel.cpp"
    break;

  case 446: /* var_decl_type: TPARAM  */
#line 1788 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9482 "bison-chapel.cpp"
    break;

  case 447: /* var_decl_type: TCONST  */
#line 1789 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9488 "bison-chapel.cpp"
    break;

  case 448: /* var_decl_type: TREF  */
#line 1790 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9494 "bison-chapel.cpp"
    break;

  case 449: /* var_decl_type: TCONST TREF  */
#line 1791 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9500 "bison-chapel.cpp"
    break;

  case 450: /* var_decl_type: TVAR  */
#line 1792 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9506 "bison-chapel.cpp"
    break;

  case 451: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1797 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9516 "bison-chapel.cpp"
    break;

  case 452: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1803 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9525 "bison-chapel.cpp"
    break;

  case 454: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 1812 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9534 "bison-chapel.cpp"
    break;

  case 455: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 1820 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9540 "bison-chapel.cpp"
    break;

  case 456: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 1822 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9546 "bison-chapel.cpp"
    break;

  case 457: /* tuple_var_decl_component: TUNDERSCORE  */
#line 1827 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9552 "bison-chapel.cpp"
    break;

  case 458: /* tuple_var_decl_component: ident_def  */
#line 1829 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9558 "bison-chapel.cpp"
    break;

  case 459: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 1831 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9564 "bison-chapel.cpp"
    break;

  case 460: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 1836 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9570 "bison-chapel.cpp"
    break;

  case 461: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 1838 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9576 "bison-chapel.cpp"
    break;

  case 462: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 1840 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9582 "bison-chapel.cpp"
    break;

  case 463: /* opt_init_expr: %empty  */
#line 1846 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9588 "bison-chapel.cpp"
    break;

  case 464: /* opt_init_expr: TASSIGN TNOINIT  */
#line 1847 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9594 "bison-chapel.cpp"
    break;

  case 465: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 1848 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9600 "bison-chapel.cpp"
    break;

  case 466: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 1854 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9606 "bison-chapel.cpp"
    break;

  case 467: /* ret_array_type: TLSBR TRSBR  */
#line 1856 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9612 "bison-chapel.cpp"
    break;

  case 468: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9620 "bison-chapel.cpp"
    break;

  case 469: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9628 "bison-chapel.cpp"
    break;

  case 470: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 1866 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9634 "bison-chapel.cpp"
    break;

  case 471: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 1868 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9642 "bison-chapel.cpp"
    break;

  case 472: /* ret_array_type: TLSBR error TRSBR  */
#line 1872 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9650 "bison-chapel.cpp"
    break;

  case 473: /* opt_ret_type: %empty  */
#line 1879 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9656 "bison-chapel.cpp"
    break;

  case 474: /* opt_ret_type: TCOLON type_level_expr  */
#line 1880 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9662 "bison-chapel.cpp"
    break;

  case 475: /* opt_ret_type: TCOLON ret_array_type  */
#line 1881 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9668 "bison-chapel.cpp"
    break;

  case 476: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 1882 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9674 "bison-chapel.cpp"
    break;

  case 477: /* opt_ret_type: error  */
#line 1883 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9680 "bison-chapel.cpp"
    break;

  case 478: /* opt_type: %empty  */
#line 1888 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9686 "bison-chapel.cpp"
    break;

  case 479: /* opt_type: TCOLON type_level_expr  */
#line 1889 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9692 "bison-chapel.cpp"
    break;

  case 480: /* opt_type: TCOLON array_type  */
#line 1890 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9698 "bison-chapel.cpp"
    break;

  case 481: /* opt_type: TCOLON reserved_type_ident_use  */
#line 1891 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9704 "bison-chapel.cpp"
    break;

  case 482: /* opt_type: error  */
#line 1892 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9710 "bison-chapel.cpp"
    break;

  case 483: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1913 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9718 "bison-chapel.cpp"
    break;

  case 484: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 1917 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9726 "bison-chapel.cpp"
    break;

  case 485: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 1921 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9738 "bison-chapel.cpp"
    break;

  case 486: /* array_type: TLSBR error TRSBR  */
#line 1929 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9746 "bison-chapel.cpp"
    break;

  case 487: /* opt_formal_array_elt_type: %empty  */
#line 1935 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9752 "bison-chapel.cpp"
    break;

  case 488: /* opt_formal_array_elt_type: type_level_expr  */
#line 1936 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9758 "bison-chapel.cpp"
    break;

  case 489: /* opt_formal_array_elt_type: query_expr  */
#line 1937 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9764 "bison-chapel.cpp"
    break;

  case 490: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9770 "bison-chapel.cpp"
    break;

  case 491: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 1944 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9776 "bison-chapel.cpp"
    break;

  case 492: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 1950 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9782 "bison-chapel.cpp"
    break;

  case 493: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 1952 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9788 "bison-chapel.cpp"
    break;

  case 494: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 1954 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9794 "bison-chapel.cpp"
    break;

  case 495: /* opt_formal_type: %empty  */
#line 1958 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9800 "bison-chapel.cpp"
    break;

  case 496: /* opt_formal_type: TCOLON type_level_expr  */
#line 1959 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9806 "bison-chapel.cpp"
    break;

  case 497: /* opt_formal_type: TCOLON query_expr  */
#line 1960 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9812 "bison-chapel.cpp"
    break;

  case 498: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 1961 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9818 "bison-chapel.cpp"
    break;

  case 499: /* opt_formal_type: TCOLON formal_array_type  */
#line 1962 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9824 "bison-chapel.cpp"
    break;

  case 500: /* expr_ls: expr  */
#line 1968 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9830 "bison-chapel.cpp"
    break;

  case 501: /* expr_ls: query_expr  */
#line 1969 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9836 "bison-chapel.cpp"
    break;

  case 502: /* expr_ls: expr_ls TCOMMA expr  */
#line 1970 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9842 "bison-chapel.cpp"
    break;

  case 503: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 1971 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9848 "bison-chapel.cpp"
    break;

  case 504: /* simple_expr_ls: expr  */
#line 1975 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9854 "bison-chapel.cpp"
    break;

  case 505: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 1976 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9860 "bison-chapel.cpp"
    break;

  case 506: /* tuple_component: TUNDERSCORE  */
#line 1980 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9866 "bison-chapel.cpp"
    break;

  case 507: /* tuple_component: opt_try_expr  */
#line 1981 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9872 "bison-chapel.cpp"
    break;

  case 508: /* tuple_component: query_expr  */
#line 1982 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9878 "bison-chapel.cpp"
    break;

  case 509: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 1986 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9884 "bison-chapel.cpp"
    break;

  case 510: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 1987 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9890 "bison-chapel.cpp"
    break;

  case 511: /* opt_actual_ls: %empty  */
#line 1991 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9896 "bison-chapel.cpp"
    break;

  case 513: /* actual_ls: actual_expr  */
#line 1996 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9902 "bison-chapel.cpp"
    break;

  case 514: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 1997 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9908 "bison-chapel.cpp"
    break;

  case 515: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2001 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9914 "bison-chapel.cpp"
    break;

  case 516: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2002 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9920 "bison-chapel.cpp"
    break;

  case 517: /* actual_expr: query_expr  */
#line 2003 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9926 "bison-chapel.cpp"
    break;

  case 518: /* actual_expr: opt_try_expr  */
#line 2004 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9932 "bison-chapel.cpp"
    break;

  case 519: /* ident_expr: ident_use  */
#line 2008 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9938 "bison-chapel.cpp"
    break;

  case 520: /* ident_expr: scalar_type  */
#line 2009 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9944 "bison-chapel.cpp"
    break;

  case 521: /* type_level_expr: sub_type_level_expr  */
#line 2021 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9950 "bison-chapel.cpp"
    break;

  case 522: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2023 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9956 "bison-chapel.cpp"
    break;

  case 523: /* type_level_expr: TQUESTION  */
#line 2025 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9962 "bison-chapel.cpp"
    break;

  case 529: /* sub_type_level_expr: TSINGLE expr  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9968 "bison-chapel.cpp"
    break;

  case 530: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2038 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9974 "bison-chapel.cpp"
    break;

  case 531: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9980 "bison-chapel.cpp"
    break;

  case 532: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2042 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9986 "bison-chapel.cpp"
    break;

  case 533: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2044 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pexpr)); }
#line 9992 "bison-chapel.cpp"
    break;

  case 534: /* sub_type_level_expr: TATOMIC expr  */
#line 2046 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9998 "bison-chapel.cpp"
    break;

  case 535: /* sub_type_level_expr: TSYNC expr  */
#line 2048 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 10004 "bison-chapel.cpp"
    break;

  case 536: /* sub_type_level_expr: TOWNED  */
#line 2051 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 10010 "bison-chapel.cpp"
    break;

  case 537: /* sub_type_level_expr: TOWNED expr  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 10016 "bison-chapel.cpp"
    break;

  case 538: /* sub_type_level_expr: TUNMANAGED  */
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 10022 "bison-chapel.cpp"
    break;

  case 539: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10028 "bison-chapel.cpp"
    break;

  case 540: /* sub_type_level_expr: TSHARED  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 10034 "bison-chapel.cpp"
    break;

  case 541: /* sub_type_level_expr: TSHARED expr  */
#line 2061 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 10040 "bison-chapel.cpp"
    break;

  case 542: /* sub_type_level_expr: TBORROWED  */
#line 2063 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 10046 "bison-chapel.cpp"
    break;

  case 543: /* sub_type_level_expr: TBORROWED expr  */
#line 2065 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10052 "bison-chapel.cpp"
    break;

  case 544: /* sub_type_level_expr: TCLASS  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 10058 "bison-chapel.cpp"
    break;

  case 545: /* sub_type_level_expr: TRECORD  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 10064 "bison-chapel.cpp"
    break;

  case 546: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10070 "bison-chapel.cpp"
    break;

  case 547: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2077 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10076 "bison-chapel.cpp"
    break;

  case 548: /* for_expr: TFOR expr TDO expr  */
#line 2079 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10082 "bison-chapel.cpp"
    break;

  case 549: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2081 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10088 "bison-chapel.cpp"
    break;

  case 550: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10094 "bison-chapel.cpp"
    break;

  case 551: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2085 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10100 "bison-chapel.cpp"
    break;

  case 552: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10106 "bison-chapel.cpp"
    break;

  case 553: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10112 "bison-chapel.cpp"
    break;

  case 554: /* for_expr: TFORALL expr TDO expr  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10118 "bison-chapel.cpp"
    break;

  case 555: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10124 "bison-chapel.cpp"
    break;

  case 556: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10130 "bison-chapel.cpp"
    break;

  case 557: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2097 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10136 "bison-chapel.cpp"
    break;

  case 558: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2099 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 10147 "bison-chapel.cpp"
    break;

  case 559: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2106 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 10157 "bison-chapel.cpp"
    break;

  case 560: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2112 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10167 "bison-chapel.cpp"
    break;

  case 561: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2118 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10177 "bison-chapel.cpp"
    break;

  case 562: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2124 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10187 "bison-chapel.cpp"
    break;

  case 563: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2133 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10193 "bison-chapel.cpp"
    break;

  case 564: /* nil_expr: TNIL  */
#line 2142 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 10199 "bison-chapel.cpp"
    break;

  case 572: /* stmt_level_expr: io_expr TIO expr  */
#line 2158 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10205 "bison-chapel.cpp"
    break;

  case 573: /* opt_task_intent_ls: %empty  */
#line 2162 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 10211 "bison-chapel.cpp"
    break;

  case 575: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2167 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10217 "bison-chapel.cpp"
    break;

  case 576: /* task_intent_ls: intent_expr  */
#line 2171 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10223 "bison-chapel.cpp"
    break;

  case 577: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2172 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10229 "bison-chapel.cpp"
    break;

  case 578: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2176 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10235 "bison-chapel.cpp"
    break;

  case 579: /* forall_intent_ls: intent_expr  */
#line 2180 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10241 "bison-chapel.cpp"
    break;

  case 580: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2181 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10247 "bison-chapel.cpp"
    break;

  case 581: /* intent_expr: shadow_var_prefix ident_expr opt_type opt_init_expr  */
#line 2186 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10255 "bison-chapel.cpp"
    break;

  case 582: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2190 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10263 "bison-chapel.cpp"
    break;

  case 583: /* intent_expr: expr TREDUCE ident_expr  */
#line 2194 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10271 "bison-chapel.cpp"
    break;

  case 584: /* shadow_var_prefix: TCONST  */
#line 2200 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10277 "bison-chapel.cpp"
    break;

  case 585: /* shadow_var_prefix: TIN  */
#line 2201 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10283 "bison-chapel.cpp"
    break;

  case 586: /* shadow_var_prefix: TCONST TIN  */
#line 2202 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10289 "bison-chapel.cpp"
    break;

  case 587: /* shadow_var_prefix: TREF  */
#line 2203 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10295 "bison-chapel.cpp"
    break;

  case 588: /* shadow_var_prefix: TCONST TREF  */
#line 2204 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10301 "bison-chapel.cpp"
    break;

  case 589: /* shadow_var_prefix: TVAR  */
#line 2205 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10307 "bison-chapel.cpp"
    break;

  case 591: /* io_expr: io_expr TIO expr  */
#line 2211 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10313 "bison-chapel.cpp"
    break;

  case 592: /* new_maybe_decorated: TNEW  */
#line 2216 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10319 "bison-chapel.cpp"
    break;

  case 593: /* new_maybe_decorated: TNEW TOWNED  */
#line 2218 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10327 "bison-chapel.cpp"
    break;

  case 594: /* new_maybe_decorated: TNEW TSHARED  */
#line 2222 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10335 "bison-chapel.cpp"
    break;

  case 595: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2226 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10343 "bison-chapel.cpp"
    break;

  case 596: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2230 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10351 "bison-chapel.cpp"
    break;

  case 597: /* new_expr: new_maybe_decorated expr  */
#line 2238 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10358 "bison-chapel.cpp"
    break;

  case 598: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2243 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10368 "bison-chapel.cpp"
    break;

  case 599: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2249 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10378 "bison-chapel.cpp"
    break;

  case 600: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2255 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10389 "bison-chapel.cpp"
    break;

  case 601: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2262 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10400 "bison-chapel.cpp"
    break;

  case 602: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2272 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10406 "bison-chapel.cpp"
    break;

  case 612: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2289 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10412 "bison-chapel.cpp"
    break;

  case 613: /* expr: expr TCOLON expr  */
#line 2291 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10418 "bison-chapel.cpp"
    break;

  case 614: /* expr: expr TDOTDOT expr  */
#line 2293 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10424 "bison-chapel.cpp"
    break;

  case 615: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2295 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10430 "bison-chapel.cpp"
    break;

  case 616: /* expr: expr TDOTDOT  */
#line 2310 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10436 "bison-chapel.cpp"
    break;

  case 617: /* expr: TDOTDOT expr  */
#line 2312 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10442 "bison-chapel.cpp"
    break;

  case 618: /* expr: TDOTDOTOPENHIGH expr  */
#line 2314 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10448 "bison-chapel.cpp"
    break;

  case 619: /* expr: TDOTDOT  */
#line 2316 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10454 "bison-chapel.cpp"
    break;

  case 620: /* opt_expr: %empty  */
#line 2320 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10460 "bison-chapel.cpp"
    break;

  case 621: /* opt_expr: expr  */
#line 2321 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10466 "bison-chapel.cpp"
    break;

  case 622: /* opt_try_expr: TTRY expr  */
#line 2325 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10472 "bison-chapel.cpp"
    break;

  case 623: /* opt_try_expr: TTRYBANG expr  */
#line 2326 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10478 "bison-chapel.cpp"
    break;

  case 624: /* opt_try_expr: expr  */
#line 2327 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10484 "bison-chapel.cpp"
    break;

  case 629: /* call_base_expr: lhs_expr  */
#line 2343 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10490 "bison-chapel.cpp"
    break;

  case 630: /* call_base_expr: expr TBANG  */
#line 2344 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10496 "bison-chapel.cpp"
    break;

  case 631: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2345 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10502 "bison-chapel.cpp"
    break;

  case 632: /* call_base_expr: lambda_decl_expr  */
#line 2346 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10508 "bison-chapel.cpp"
    break;

  case 634: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2351 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10514 "bison-chapel.cpp"
    break;

  case 635: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2352 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10520 "bison-chapel.cpp"
    break;

  case 636: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2353 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10526 "bison-chapel.cpp"
    break;

  case 637: /* dot_expr: expr TDOT ident_use  */
#line 2357 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10532 "bison-chapel.cpp"
    break;

  case 638: /* dot_expr: expr TDOT TTYPE  */
#line 2358 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10538 "bison-chapel.cpp"
    break;

  case 639: /* dot_expr: expr TDOT TDOMAIN  */
#line 2359 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10544 "bison-chapel.cpp"
    break;

  case 640: /* dot_expr: expr TDOT TLOCALE  */
#line 2360 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10550 "bison-chapel.cpp"
    break;

  case 641: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2361 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10556 "bison-chapel.cpp"
    break;

  case 642: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2369 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10562 "bison-chapel.cpp"
    break;

  case 643: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2370 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10568 "bison-chapel.cpp"
    break;

  case 644: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2371 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10574 "bison-chapel.cpp"
    break;

  case 645: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2372 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10580 "bison-chapel.cpp"
    break;

  case 646: /* bool_literal: TFALSE  */
#line 2376 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10586 "bison-chapel.cpp"
    break;

  case 647: /* bool_literal: TTRUE  */
#line 2377 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10592 "bison-chapel.cpp"
    break;

  case 648: /* str_bytes_literal: STRINGLITERAL  */
#line 2381 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10598 "bison-chapel.cpp"
    break;

  case 649: /* str_bytes_literal: BYTESLITERAL  */
#line 2382 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10604 "bison-chapel.cpp"
    break;

  case 652: /* literal_expr: INTLITERAL  */
#line 2388 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10610 "bison-chapel.cpp"
    break;

  case 653: /* literal_expr: REALLITERAL  */
#line 2389 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10616 "bison-chapel.cpp"
    break;

  case 654: /* literal_expr: IMAGLITERAL  */
#line 2390 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10622 "bison-chapel.cpp"
    break;

  case 655: /* literal_expr: CSTRINGLITERAL  */
#line 2391 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10628 "bison-chapel.cpp"
    break;

  case 656: /* literal_expr: TNONE  */
#line 2392 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10634 "bison-chapel.cpp"
    break;

  case 657: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2393 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10641 "bison-chapel.cpp"
    break;

  case 658: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2395 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10648 "bison-chapel.cpp"
    break;

  case 659: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2397 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10654 "bison-chapel.cpp"
    break;

  case 660: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2398 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10660 "bison-chapel.cpp"
    break;

  case 661: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2400 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10668 "bison-chapel.cpp"
    break;

  case 662: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2404 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10676 "bison-chapel.cpp"
    break;

  case 663: /* assoc_expr_ls: expr TALIAS expr  */
#line 2411 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10682 "bison-chapel.cpp"
    break;

  case 664: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2412 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10688 "bison-chapel.cpp"
    break;

  case 665: /* binary_op_expr: expr TPLUS expr  */
#line 2416 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10694 "bison-chapel.cpp"
    break;

  case 666: /* binary_op_expr: expr TMINUS expr  */
#line 2417 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10700 "bison-chapel.cpp"
    break;

  case 667: /* binary_op_expr: expr TSTAR expr  */
#line 2418 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10706 "bison-chapel.cpp"
    break;

  case 668: /* binary_op_expr: expr TDIVIDE expr  */
#line 2419 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10712 "bison-chapel.cpp"
    break;

  case 669: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2420 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10718 "bison-chapel.cpp"
    break;

  case 670: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2421 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10724 "bison-chapel.cpp"
    break;

  case 671: /* binary_op_expr: expr TMOD expr  */
#line 2422 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10730 "bison-chapel.cpp"
    break;

  case 672: /* binary_op_expr: expr TEQUAL expr  */
#line 2423 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10736 "bison-chapel.cpp"
    break;

  case 673: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2424 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10742 "bison-chapel.cpp"
    break;

  case 674: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2425 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10748 "bison-chapel.cpp"
    break;

  case 675: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2426 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10754 "bison-chapel.cpp"
    break;

  case 676: /* binary_op_expr: expr TLESS expr  */
#line 2427 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10760 "bison-chapel.cpp"
    break;

  case 677: /* binary_op_expr: expr TGREATER expr  */
#line 2428 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10766 "bison-chapel.cpp"
    break;

  case 678: /* binary_op_expr: expr TBAND expr  */
#line 2429 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10772 "bison-chapel.cpp"
    break;

  case 679: /* binary_op_expr: expr TBOR expr  */
#line 2430 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10778 "bison-chapel.cpp"
    break;

  case 680: /* binary_op_expr: expr TBXOR expr  */
#line 2431 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10784 "bison-chapel.cpp"
    break;

  case 681: /* binary_op_expr: expr TAND expr  */
#line 2432 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10790 "bison-chapel.cpp"
    break;

  case 682: /* binary_op_expr: expr TOR expr  */
#line 2433 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10796 "bison-chapel.cpp"
    break;

  case 683: /* binary_op_expr: expr TEXP expr  */
#line 2434 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10802 "bison-chapel.cpp"
    break;

  case 684: /* binary_op_expr: expr TBY expr  */
#line 2435 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10808 "bison-chapel.cpp"
    break;

  case 685: /* binary_op_expr: expr TALIGN expr  */
#line 2436 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10814 "bison-chapel.cpp"
    break;

  case 686: /* binary_op_expr: expr THASH expr  */
#line 2437 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10820 "bison-chapel.cpp"
    break;

  case 687: /* binary_op_expr: expr TDMAPPED expr  */
#line 2438 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10827 "bison-chapel.cpp"
    break;

  case 688: /* unary_op_expr: TPLUS expr  */
#line 2443 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10833 "bison-chapel.cpp"
    break;

  case 689: /* unary_op_expr: TMINUS expr  */
#line 2444 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10839 "bison-chapel.cpp"
    break;

  case 690: /* unary_op_expr: TMINUSMINUS expr  */
#line 2445 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10845 "bison-chapel.cpp"
    break;

  case 691: /* unary_op_expr: TPLUSPLUS expr  */
#line 2446 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10851 "bison-chapel.cpp"
    break;

  case 692: /* unary_op_expr: TBANG expr  */
#line 2447 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10857 "bison-chapel.cpp"
    break;

  case 693: /* unary_op_expr: expr TBANG  */
#line 2448 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10863 "bison-chapel.cpp"
    break;

  case 694: /* unary_op_expr: TBNOT expr  */
#line 2449 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10869 "bison-chapel.cpp"
    break;

  case 695: /* reduce_expr: expr TREDUCE expr  */
#line 2453 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10875 "bison-chapel.cpp"
    break;

  case 696: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2454 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10881 "bison-chapel.cpp"
    break;

  case 697: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2455 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10887 "bison-chapel.cpp"
    break;

  case 698: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2456 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10893 "bison-chapel.cpp"
    break;

  case 699: /* scan_expr: expr TSCAN expr  */
#line 2460 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10899 "bison-chapel.cpp"
    break;

  case 700: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2461 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10905 "bison-chapel.cpp"
    break;

  case 701: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2462 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10911 "bison-chapel.cpp"
    break;

  case 702: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2463 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10917 "bison-chapel.cpp"
    break;

  case 703: /* reduce_scan_op_expr: TPLUS  */
#line 2468 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10923 "bison-chapel.cpp"
    break;

  case 704: /* reduce_scan_op_expr: TSTAR  */
#line 2469 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10929 "bison-chapel.cpp"
    break;

  case 705: /* reduce_scan_op_expr: TAND  */
#line 2470 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10935 "bison-chapel.cpp"
    break;

  case 706: /* reduce_scan_op_expr: TOR  */
#line 2471 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10941 "bison-chapel.cpp"
    break;

  case 707: /* reduce_scan_op_expr: TBAND  */
#line 2472 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10947 "bison-chapel.cpp"
    break;

  case 708: /* reduce_scan_op_expr: TBOR  */
#line 2473 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10953 "bison-chapel.cpp"
    break;

  case 709: /* reduce_scan_op_expr: TBXOR  */
#line 2474 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10959 "bison-chapel.cpp"
    break;


#line 10963 "bison-chapel.cpp"

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
