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
  YYSYMBOL_extern_block_stmt = 227,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 228,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 229,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 230,                  /* if_stmt  */
  YYSYMBOL_ifvar = 231,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 232,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 233,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 234,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 235,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 236, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 237,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 238,           /* ifc_constraint  */
  YYSYMBOL_defer_stmt = 239,               /* defer_stmt  */
  YYSYMBOL_try_stmt = 240,                 /* try_stmt  */
  YYSYMBOL_catch_stmt_ls = 241,            /* catch_stmt_ls  */
  YYSYMBOL_catch_stmt = 242,               /* catch_stmt  */
  YYSYMBOL_catch_expr = 243,               /* catch_expr  */
  YYSYMBOL_throw_stmt = 244,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 245,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 246,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 247,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 248,          /* class_decl_stmt  */
  YYSYMBOL_class_tag = 249,                /* class_tag  */
  YYSYMBOL_opt_inherit = 250,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 251,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 252,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 253,              /* enum_header  */
  YYSYMBOL_enum_ls = 254,                  /* enum_ls  */
  YYSYMBOL_deprecated_enum_item = 255,     /* deprecated_enum_item  */
  YYSYMBOL_enum_item = 256,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 257,         /* lambda_decl_expr  */
  YYSYMBOL_258_7 = 258,                    /* $@7  */
  YYSYMBOL_259_8 = 259,                    /* $@8  */
  YYSYMBOL_linkage_spec = 260,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt = 261,             /* fn_decl_stmt  */
  YYSYMBOL_262_9 = 262,                    /* $@9  */
  YYSYMBOL_263_10 = 263,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 264,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_receiver_expr = 265,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 266,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 267,           /* assignop_ident  */
  YYSYMBOL_opt_formal_ls = 268,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 269,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 270,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 271,                /* formal_ls  */
  YYSYMBOL_formal = 272,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 273,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 274,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 275,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 276,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 277,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 278,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 279,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 280,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 281,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 282,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 283,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 284, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 285,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 286,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 287,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_inner = 288, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 289,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 290,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 291,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 292,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 293,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 294, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 295, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 296,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 297,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 298,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 299,                 /* opt_type  */
  YYSYMBOL_array_type = 300,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 301, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 302,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 303,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 304,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 305,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 306,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 307,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 308,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 309,                /* actual_ls  */
  YYSYMBOL_actual_expr = 310,              /* actual_expr  */
  YYSYMBOL_ident_expr = 311,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 312,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 313,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 314,                 /* for_expr  */
  YYSYMBOL_cond_expr = 315,                /* cond_expr  */
  YYSYMBOL_nil_expr = 316,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 317,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 318,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 319,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 320,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 321,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 322,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 323,              /* intent_expr  */
  YYSYMBOL_shadow_var_prefix = 324,        /* shadow_var_prefix  */
  YYSYMBOL_io_expr = 325,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 326,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 327,                 /* new_expr  */
  YYSYMBOL_let_expr = 328,                 /* let_expr  */
  YYSYMBOL_expr = 329,                     /* expr  */
  YYSYMBOL_opt_expr = 330,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 331,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 332,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 333,           /* call_base_expr  */
  YYSYMBOL_call_expr = 334,                /* call_expr  */
  YYSYMBOL_dot_expr = 335,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 336,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 337,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 338,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 339,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 340,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 341,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 342,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 343,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 344,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 345       /* reduce_scan_op_expr  */
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

#line 520 "bison-chapel.cpp"

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
#define YYLAST   21414

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  166
/* YYNRULES -- Number of rules.  */
#define YYNRULES  705
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1265

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
     925,   926,   927,   931,   932,   933,   934,   939,   938,   954,
     953,   970,   969,   985,   984,  1000,  1004,  1009,  1017,  1028,
    1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,
    1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1068,  1074,
    1080,  1086,  1093,  1100,  1104,  1111,  1115,  1116,  1117,  1118,
    1120,  1121,  1122,  1123,  1125,  1127,  1129,  1131,  1136,  1137,
    1141,  1143,  1151,  1152,  1157,  1162,  1163,  1164,  1165,  1166,
    1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1181,  1182,
    1183,  1184,  1193,  1194,  1198,  1200,  1203,  1209,  1211,  1214,
    1220,  1223,  1224,  1225,  1226,  1227,  1228,  1232,  1233,  1237,
    1238,  1239,  1243,  1244,  1248,  1251,  1253,  1258,  1259,  1263,
    1265,  1267,  1274,  1284,  1298,  1303,  1308,  1316,  1317,  1322,
    1323,  1325,  1330,  1346,  1353,  1362,  1370,  1374,  1381,  1382,
    1384,  1389,  1390,  1395,  1400,  1394,  1427,  1430,  1434,  1442,
    1452,  1441,  1491,  1495,  1500,  1504,  1509,  1516,  1517,  1521,
    1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,  1531,
    1532,  1533,  1534,  1535,  1536,  1537,  1538,  1539,  1540,  1541,
    1542,  1543,  1544,  1545,  1546,  1547,  1548,  1552,  1553,  1554,
    1555,  1556,  1557,  1558,  1559,  1560,  1561,  1562,  1563,  1567,
    1568,  1572,  1576,  1577,  1581,  1582,  1586,  1588,  1590,  1592,
    1594,  1596,  1601,  1602,  1606,  1607,  1608,  1609,  1610,  1611,
    1612,  1613,  1614,  1618,  1619,  1620,  1621,  1622,  1623,  1627,
    1628,  1629,  1633,  1634,  1635,  1636,  1637,  1638,  1642,  1643,
    1646,  1647,  1651,  1652,  1656,  1660,  1661,  1662,  1670,  1671,
    1673,  1675,  1677,  1682,  1684,  1689,  1690,  1691,  1692,  1693,
    1694,  1695,  1699,  1701,  1706,  1708,  1710,  1715,  1728,  1745,
    1746,  1748,  1753,  1754,  1755,  1756,  1757,  1761,  1767,  1775,
    1776,  1784,  1786,  1791,  1793,  1795,  1800,  1802,  1804,  1811,
    1812,  1813,  1818,  1820,  1822,  1826,  1830,  1832,  1836,  1844,
    1845,  1846,  1847,  1848,  1853,  1854,  1855,  1856,  1857,  1877,
    1881,  1885,  1893,  1900,  1901,  1902,  1906,  1908,  1914,  1916,
    1918,  1923,  1924,  1925,  1926,  1927,  1933,  1934,  1935,  1936,
    1940,  1941,  1945,  1946,  1947,  1951,  1952,  1956,  1957,  1961,
    1962,  1966,  1967,  1968,  1969,  1973,  1974,  1985,  1987,  1989,
    1995,  1996,  1997,  1998,  1999,  2000,  2002,  2004,  2006,  2008,
    2010,  2012,  2015,  2017,  2019,  2021,  2023,  2025,  2027,  2029,
    2032,  2034,  2039,  2041,  2043,  2045,  2047,  2049,  2051,  2053,
    2055,  2057,  2059,  2061,  2063,  2070,  2076,  2082,  2088,  2097,
    2107,  2115,  2116,  2117,  2118,  2119,  2120,  2121,  2122,  2127,
    2128,  2132,  2136,  2137,  2141,  2145,  2146,  2150,  2154,  2158,
    2165,  2166,  2167,  2168,  2169,  2170,  2174,  2175,  2180,  2182,
    2186,  2190,  2194,  2202,  2207,  2213,  2219,  2226,  2236,  2244,
    2245,  2246,  2247,  2248,  2249,  2250,  2251,  2252,  2253,  2255,
    2257,  2259,  2274,  2276,  2278,  2280,  2285,  2286,  2290,  2291,
    2292,  2296,  2297,  2298,  2299,  2308,  2309,  2310,  2311,  2312,
    2316,  2317,  2318,  2322,  2323,  2324,  2325,  2326,  2334,  2335,
    2336,  2337,  2341,  2342,  2346,  2347,  2351,  2352,  2353,  2354,
    2355,  2356,  2357,  2358,  2360,  2362,  2363,  2364,  2368,  2376,
    2377,  2381,  2382,  2383,  2384,  2385,  2386,  2387,  2388,  2389,
    2390,  2391,  2392,  2393,  2394,  2395,  2396,  2397,  2398,  2399,
    2400,  2401,  2402,  2403,  2408,  2409,  2410,  2411,  2412,  2413,
    2414,  2418,  2419,  2420,  2421,  2425,  2426,  2427,  2428,  2433,
    2434,  2435,  2436,  2437,  2438,  2439
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
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "deprecated_enum_item", "enum_item", "lambda_decl_expr",
  "$@7", "$@8", "linkage_spec", "fn_decl_stmt", "$@9", "$@10",
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

#define YYPACT_NINF (-1144)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-656)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1144,   231,  3610, -1144,   125,   287, -1144, -1144, -1144, -1144,
   -1144, -1144,  5002,   333,   347,   427, 14771,   436, 20873,   333,
   11410,   448,    37,   377,   347,  5002, 11410,  1373,  5002,   360,
   20959, 11582,  8298,   460,  8470,  9682,  9682,  6914,  8642,   508,
   -1144,   396, -1144,   546, 20993, 20993, 20993, -1144,  3246,  9854,
     551, 11410,   323, -1144,   552,   569, 11410, -1144, 14771, -1144,
   11410,   618,   458,   207, 21245,   574, 21079, -1144, 10028,  7780,
   11410,  9854, 14771, 11410,   532,   579,   465,  5002,   583, 11410,
     584, 11754, 11754, 20993,   589, -1144, 14771, -1144,   591,  8642,
   11410, -1144, 11410, -1144, 11410, -1144, -1144, 14289, 11410, -1144,
   11410, -1144, -1144, -1144,  3958,  7088,  8816, 11410, -1144,  4828,
   -1144, -1144, -1144,   468, -1144,   578, -1144, -1144,   115, -1144,
   -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144, -1144,   597, -1144, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144, 20993, -1144, 20993,   -57,   254, -1144, -1144,
    3246, -1144,   492, -1144,   501, -1144, -1144,   503,   506,   522,
   11410,   515,   520, 20013,  3074,   372,   521,   523, -1144, -1144,
     380, -1144, -1144, -1144, -1144, -1144,   413, -1144, -1144, 20013,
     524,  5002, -1144, -1144,   527, 11410, -1144, -1144, 11410, 11410,
   11410, 20993, -1144, 11410, 10028, 10028,   630,   404, -1144, -1144,
   -1144, -1144,   129,   405, -1144, -1144,   533, 16648, 20993,  3246,
   -1144,   528, -1144,   -34, 20013, 20577, -1144, -1144, 10200,   215,
   21300, -1144, -1144,   575,  7954,   614, 21113, 20013,   228,   622,
   -1144, 21199, 20993,   228, 20993,   534,    10, 16235,    21, 16070,
      21, 16152,   212, -1144, 16318, 20993, 20993,   -11, 15323,   335,
    7954, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144, -1144, -1144,   539, -1144,   417,  5002,   541,
    1486,   101,    -3, -1144,  5002, -1144, -1144, 16729,   567, -1144,
     544,   550, -1144, 16729,   129,   567, -1144,  7954,  3144, -1144,
   -1144, -1144,   138, 20013, 11410, 11410, -1144, 20013,   558, 17303,
   -1144, 16729,   129, 20013,   553,  7954, -1144, 20013, 17386, -1144,
   -1144, 17468,  3238, -1144, -1144, 17549,   599,   565,   129,    10,
   16729, 17630,   581,   581,  2073,   567,   567,   -51, -1144, -1144,
    4132,   -48, -1144, 11410, -1144,   -41,   122, -1144,   139,    20,
   17711,   -62,  2073,   719, -1144,  4306, -1144,   676, 11410, 11410,
   20993,   601,   582, -1144, -1144, -1144, -1144,   339,   416, -1144,
   11410,   596, 11410, 11410, 11410,  9682,  9682, 11410,   430, 11410,
   11410, 11410, 11410, 11410,   654, 14289, 11410, 11410, 11410, 11410,
   11410, 11410, 11410, 11410, 11410, 11410, 11410, 11410, 11410, 11410,
   11410, 11410,   679, -1144, -1144, -1144, -1144, -1144,  8988,  8988,
   -1144, -1144, -1144, -1144,  8988, -1144, -1144,  8988,  8988,  7954,
    7954,  9682,  9682,  7608, -1144, -1144, 16811, 16892, 17792,   586,
       1, 20993,  4480, -1144,  9682,    10,   590,   342, -1144, 11410,
   -1144, -1144, 11410,   631, -1144,   588,   613, -1144, -1144, -1144,
   20993, -1144,  3246, -1144, 20993,   600, -1144,  3246,   713, 10028,
   -1144,  5176,  9682, -1144,   592, -1144,    10,  5350,  9682, -1144,
      10, -1144,    10,  9682, -1144,    10, 11926, 11410, -1144,   644,
     647,  5002,   739,  5002, -1144,   740, 11410, -1144, -1144,   578,
     607,  7954, 20993, -1144, -1144,   410, -1144, -1144,  1486, -1144,
     635,   609, -1144, 12098,   658, 11410,  3246, -1144, -1144, 11410,
   11410, -1144,   611, -1144, 10028, -1144, 20013, 20013, -1144,    34,
   -1144,  7954,   612, -1144,   764, -1144,   764, -1144, 12270,   643,
   -1144, -1144, -1144, -1144, -1144, -1144, -1144,  9162, -1144, 17873,
    7262, -1144,  7436, -1144,  5002,   615,  9682,  9336,  3784,   616,
   11410, 10372, -1144, -1144,   262, -1144,  4654, 20993, -1144,   349,
   17954,   362, 16401,   -54, 10028,   627, 20719,   345, -1144, 18035,
   16689, 16689,   507, -1144,   507, -1144,   507, 20396,   189,  1957,
    2368,   129,   581, -1144,   624, -1144, -1144, -1144, -1144, -1144,
    2073, 20435,   507,  2001,  2001, 16689,  2001,  2001,  2219,   581,
   20435, 20354,  2219,   567,   567,   581,  2073,   641,   645,   646,
     649,   651,   652,   619,   625, -1144,   507, -1144,   507,   186,
   -1144, -1144, -1144,   126, -1144,  2784, 20164,   422, 12442,  9682,
   12614,  9682, 11410,  7954,  9682, 15051,   636,   333, 18116, -1144,
   -1144, -1144, 20013, 18197,  7954, -1144,  7954, 20993,   601,   376,
   20993,   601, -1144,   382, 11410,   160,  8642, 20013,    29, 16973,
    7608, -1144,  8642, 20013,    40, 16483, -1144, -1144,    21, 16566,
   -1144,   653,   673,   656, 18278,   673,   659, 11410, 11410,   787,
    5002,   789, 18359,  5002, 17055,   762, -1144,   185, -1144,   194,
   -1144,   208, -1144, -1144, -1144, -1144, -1144, -1144,   411,   685,
     661, -1144, 15137, -1144,   365,   662,  1486,   101,   -18,   -15,
   11410, 11410,  6742, -1144, -1144,   351,  8126, -1144, 20013, -1144,
   18440, 18521, -1144, -1144, 20013,   665,   -27,   667, -1144,  1760,
   -1144, -1144,   384, 20993, -1144, -1144, -1144, -1144, -1144, -1144,
   -1144,  5002,   141, 17138, -1144, -1144, 20013,  5002, 20013, -1144,
   18602, -1144, -1144, -1144, 11410, -1144,    97,   100, 11410, -1144,
   10544, 11926, 11410, -1144,  7954,   686,  1220,   670, 20753,   717,
     246, -1144, -1144,   748, -1144, -1144, -1144, -1144, 14122,   671,
   -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144,
    7608, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144,    71,  9682,  9682, 11410,   809, 18683, 11410,
     811, 18764,   256,   672, 18845,  7954,    10,    10, -1144, -1144,
   -1144, -1144,   601,   678, -1144,   601,   680, -1144, 16729, -1144,
   15406,  5524, -1144,  5698, -1144,   276, -1144, 15489,  5872, -1144,
      10,  6046, -1144,    10, -1144,    10, -1144,    10, 11410, -1144,
   11410, -1144, 20013, 20013,  5002, -1144,  5002, 11410, -1144,  5002,
     816, 20993,   688, 20993,   533, -1144, -1144, 20993,   563, -1144,
    1486,   709,   765, -1144, -1144, -1144,   105, -1144, -1144,   658,
     681,    79, -1144, -1144,   687,   689, -1144,  6220, 10028, -1144,
   -1144, -1144, 20993, -1144,   715,   533, -1144, -1144,  6394,   683,
    6568,   694, -1144, 11410, -1144, -1144, 11410, 18926,    61, 17221,
     697,   705,   279,   701,  2837, -1144, 20993, -1144, 11410, 20839,
   -1144, -1144,   365,   704,   206, -1144,   725, -1144,   731,   732,
     741,   736,   737, -1144,   738,   751,   745,   746,   749,   402,
     756,   750,   752, -1144, -1144, -1144, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144, -1144, -1144, -1144, 11410, -1144,   759,   761,
     754,   704,   704, -1144, -1144, -1144,   658,   288,   295, 19008,
   12786, 12958, 19089, 13130, 13302, -1144, 13474, 13646,   300, -1144,
   -1144,   718, -1144,   742, -1144, -1144,  5002,  8642, 20013,  8642,
   20013,  7608, -1144,  5002,  8642, 20013, -1144,  8642, 20013, -1144,
   -1144, -1144, 19170, 20013, -1144, -1144, 20013,   870,  5002,   744,
   -1144, -1144, -1144,   709, -1144,   743, 10718,   210, -1144,   146,
   -1144, -1144,  9682, 14911,  7954,  7954,  5002, -1144,    22,   753,
   11410, -1144,  8642, -1144, 20013,  5002,  8642, -1144, 20013,  5002,
   20013,   259, 10890, 11926, 11410, 11926, 11410, -1144, -1144,   757,
   -1144,  2503, -1144,  3144, -1144, 20643, -1144, -1144, -1144, 20013,
   -1144,    83,   410, -1144, 19251, -1144, 15241, -1144, -1144, -1144,
   11410, 11410, 11410, 11410, 11410, 11410, 11410, 11410, -1144, -1144,
    2991, -1144, 20168, 18359, 15572, 15655, -1144, 18359, 15738, 15821,
   11410,  5002, -1144, -1144,   210,   709,  9510, -1144, -1144, -1144,
     420, 10028, -1144, -1144,   149, 11410,   153, 19332, -1144,   642,
     758,   763,   575, -1144,   533, 20013, 15904, -1144, 15987, -1144,
   -1144, -1144, 20013,   760,   767,   769,   770, -1144, 20188, -1144,
   -1144, -1144, 13818,   806,   771, -1144,   775,   777,   704,   704,
   19413, 19498, 19579, 19660, 19741, 19822, 20315, -1144, 20381, -1144,
    5002,  5002,  5002,  5002, 20013, -1144, -1144, -1144,   210, 11064,
      87, -1144, 20013, -1144,   135, -1144,    -8, -1144,   620, 19916,
   -1144, -1144, -1144, 13646,   755,   766, -1144,  5002,  5002, -1144,
   -1144, -1144, -1144, -1144,  3432, -1144, -1144,   234, -1144,   146,
   -1144, -1144, -1144, 11410, 11410, 11410, 11410, 11410, 11410, -1144,
   -1144, 18359, 18359, 18359, 18359, -1144, -1144, -1144, -1144, -1144,
     461,  9682, 14460, -1144, 11410,   149,   135,   135,   135,   135,
     135,   135,   149,   841, -1144, -1144, 18359, 18359,   768, 13990,
      88,     0, 19973, -1144, -1144, 20013, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144,   772, -1144, -1144,   353, 14631, -1144, -1144,
   -1144, 11238, -1144,   378, -1144
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   117,   648,   649,   650,   644,
     645,   651,     0,   569,   103,   138,   538,   145,   540,   569,
       0,   144,     0,   443,   103,     0,     0,   316,     0,   262,
     139,   616,   616,   642,     0,     0,     0,     0,     0,   143,
      59,   263,   317,   140,     0,     0,     0,   313,     0,     0,
     147,     0,   588,   560,   652,   148,     0,   318,   532,   442,
       0,     0,     0,   164,   316,   142,   541,   444,     0,     0,
       0,     0,   536,     0,     0,   146,     0,     0,   118,     0,
     643,     0,     0,     0,   141,   296,   534,   446,   149,     0,
       0,   701,     0,   703,     0,   704,   705,   615,     0,   702,
     699,   519,   161,   700,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    45,    54,    10,    11,     0,    12,
      13,    14,    15,   515,   516,    25,    46,   162,   171,   172,
      16,    20,    17,    19,     0,   257,    18,   607,    22,    23,
      24,    21,   170,     0,   168,     0,   604,     0,   166,   169,
       0,   167,   621,   600,   517,   601,   522,   520,     0,     0,
       0,   605,   606,     0,   521,     0,   622,   623,   624,   646,
     647,   599,   524,   523,   602,   603,     0,    41,    27,   530,
       0,     0,   570,   104,     0,     0,   540,   139,     0,     0,
       0,     0,   541,     0,     0,     0,     0,   604,   621,   520,
     605,   606,   539,   521,   622,   623,     0,   569,     0,     0,
     445,     0,   270,     0,   500,   316,   294,   304,   616,   164,
     316,   295,    44,     0,   507,   644,   541,   617,   316,   644,
     189,   541,     0,   316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   176,     0,     0,     0,     0,     0,    56,
     507,   111,   119,   131,   125,   124,   133,   114,   123,   134,
     120,   135,   112,   136,   129,   122,   130,   128,   126,   127,
     113,   115,   121,   132,   137,     0,   116,     0,     0,     0,
       0,     0,     0,   449,     0,   155,    35,     0,   686,   592,
     589,   590,   591,     0,   533,   687,     7,   507,   316,   163,
     414,   497,     0,   496,     0,     0,   156,   620,     0,     0,
      38,     0,   537,   525,     0,   507,    39,   531,     0,   277,
     273,     0,   521,   277,   274,     0,   439,     0,   535,     0,
       0,     0,   688,   690,   613,   685,   684,     0,    61,    64,
       0,     0,   502,     0,   504,     0,     0,   503,     0,     0,
     496,     0,   614,     0,     6,     0,    55,     0,     0,     0,
       0,   297,     0,   400,   401,   399,   319,     0,   518,    26,
       0,   593,     0,     0,     0,     0,     0,     0,   689,     0,
       0,     0,     0,     0,     0,   612,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   357,   364,   365,   366,   361,   363,     0,     0,
     359,   362,   360,   358,     0,   368,   367,     0,     0,   507,
     507,     0,     0,     0,    28,    29,     0,     0,     0,     0,
       0,     0,     0,    30,     0,     0,     0,     0,    31,     0,
      32,    43,     0,   515,   513,     0,   508,   509,   514,   183,
       0,   186,     0,   179,     0,     0,   185,     0,     0,     0,
     199,     0,     0,   198,     0,   207,     0,     0,     0,   205,
       0,   215,     0,     0,   213,     0,     0,    76,   173,     0,
       0,     0,   231,     0,   357,   227,     0,    58,    57,    54,
       0,     0,     0,   241,    33,   382,   314,   453,     0,   454,
     456,     0,   478,     0,   459,     0,     0,   154,    34,     0,
       0,    36,     0,   165,     0,    96,   618,   619,   157,     0,
      37,     0,     0,   284,   275,   271,   276,   272,     0,   437,
     434,   192,   191,    40,    63,    62,    65,     0,   653,     0,
       0,   638,     0,   640,     0,     0,     0,     0,     0,     0,
       0,     0,   657,     8,     0,    48,     0,     0,    94,     0,
      91,     0,    70,   268,     0,     0,     0,   393,   448,   568,
     681,   680,   683,   692,   691,   696,   695,   677,   674,   675,
     676,   609,   664,   117,     0,   635,   636,   118,   634,   633,
     610,   668,   679,   673,   671,   682,   672,   670,   662,   667,
     669,   678,   661,   665,   666,   663,   611,     0,     0,     0,
       0,     0,     0,     0,     0,   694,   693,   698,   697,   580,
     581,   583,   585,     0,   572,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   655,   268,   569,   569,   195,
     435,   447,   501,     0,     0,   527,     0,     0,   297,     0,
       0,   297,   436,     0,     0,     0,     0,   544,     0,     0,
       0,   208,     0,   550,     0,     0,   206,   216,     0,     0,
     214,   700,    79,     0,    66,    77,     0,     0,     0,   230,
       0,   226,     0,     0,     0,     0,   526,     0,   244,     0,
     242,   387,   384,   385,   386,   390,   391,   392,   382,   375,
       0,   372,     0,   383,   402,     0,   457,     0,   152,   153,
     151,   150,     0,   477,   476,   600,     0,   451,   598,   450,
       0,     0,   632,   499,   498,     0,     0,     0,   528,     0,
     278,   441,   600,     0,   654,   608,   639,   505,   641,   506,
     223,     0,     0,     0,   656,   221,   554,     0,   659,   658,
       0,    50,    49,    47,     0,    90,     0,     0,     0,    83,
       0,     0,    76,   265,     0,   298,     0,     0,     0,   311,
       0,   305,   308,   397,   394,   395,   398,   320,     0,     0,
     102,   100,   101,    99,    98,    97,   630,   631,   582,   584,
       0,   571,   138,   145,   144,   143,   140,   147,   148,   142,
     146,   141,   149,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   190,   511,
     512,   510,   297,     0,   188,   297,     0,   187,     0,   225,
       0,     0,   197,     0,   196,     0,   575,     0,     0,   203,
       0,     0,   201,     0,   211,     0,   209,     0,     0,   174,
       0,   175,   239,   238,     0,   233,     0,     0,   229,     0,
     235,     0,   267,     0,     0,   388,   389,     0,   382,   371,
       0,   491,   403,   406,   405,   407,     0,   455,   458,   459,
       0,     0,   460,   461,     0,     0,   286,     0,     0,   285,
     288,   529,     0,   279,   282,     0,   438,   224,     0,     0,
       0,     0,   222,     0,    95,    92,     0,    73,    72,    71,
       0,     0,     0,     0,   316,   303,     0,   310,     0,   306,
     302,   396,   402,   369,   105,   351,   119,   349,   125,   124,
     108,   123,   120,   354,   135,   106,   136,   122,   126,   107,
     109,   121,   137,   348,   330,   333,   331,   332,   355,   343,
     334,   347,   339,   337,   350,   353,   338,   336,   341,   346,
     335,   340,   344,   345,   342,   352,     0,   329,     0,   110,
       0,   369,   369,   327,   637,   573,   459,   621,   621,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,   194,
     193,     0,   299,     0,   299,   200,     0,     0,   543,     0,
     542,     0,   574,     0,     0,   549,   204,     0,   548,   202,
     212,   210,    68,    67,   232,   228,   559,   234,     0,     0,
     264,   243,   240,   491,   373,     0,     0,   459,   404,   418,
     452,   482,     0,   655,   507,   507,     0,   290,     0,     0,
       0,   280,     0,   219,   556,     0,     0,   217,   555,     0,
     660,     0,     0,     0,    76,     0,    76,    84,    87,   269,
     293,   316,   164,   316,   292,   316,   300,   158,   309,   312,
     307,     0,   382,   326,     0,   356,     0,   322,   323,   577,
       0,     0,     0,     0,     0,     0,     0,     0,   269,   299,
     316,   299,   316,   547,     0,     0,   576,   553,     0,     0,
       0,     0,   237,    60,   459,   491,     0,   494,   493,   495,
     600,   415,   378,   376,     0,     0,     0,     0,   480,   600,
       0,     0,   291,   289,     0,   283,     0,   220,     0,   218,
      93,    75,    74,     0,     0,     0,     0,   266,   316,   160,
     301,   473,     0,   408,     0,   328,   105,   107,   369,   369,
       0,     0,     0,     0,     0,     0,   316,   182,   316,   178,
       0,     0,     0,     0,    69,   236,   379,   377,   459,   483,
       0,   417,   416,   432,     0,   433,   420,   423,     0,   419,
     412,   413,   315,     0,   594,   595,   281,     0,     0,    86,
      89,    85,    88,   159,     0,   472,   471,   600,   409,   418,
     370,   324,   325,     0,     0,     0,     0,     0,     0,   184,
     180,   546,   545,   552,   551,   381,   380,   485,   486,   488,
     600,     0,   655,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,   596,   597,   558,   557,     0,   463,
       0,     0,     0,   487,   489,   422,   424,   425,   428,   429,
     430,   426,   427,   421,   468,   466,   600,   655,   410,   321,
     411,   483,   467,   600,   490
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1144, -1144, -1144,     4,  -478,  2385, -1144,     2, -1144, -1144,
   -1144,   431, -1144, -1144, -1144,   626,   568,  -475, -1144,  -747,
    -743, -1144, -1144, -1144,   172, -1144, -1144,   514,   904, -1144,
    2693,   -96,  -765, -1144,  -950,  2477, -1015,  -129,  -872, -1144,
     -63, -1144, -1144, -1144, -1144, -1144, -1144, -1144, -1144,   136,
   -1144,   855, -1144, -1144,    82,  1081, -1144, -1144, -1144, -1144,
   -1144,   623, -1144,    55, -1144, -1144, -1144, -1144, -1144, -1144,
    -552,  -676, -1144, -1144, -1144,    30,  -728,  1161, -1144, -1144,
   -1144,   213, -1144, -1144, -1144, -1144,  -128,  -154,  -938, -1144,
   -1144,  -122,    84,   253, -1144, -1144, -1144,    31, -1144, -1144,
    -284,    19,  -920,  -240,  -272,  -264,  -491, -1144,  -201, -1144,
       8,   925,  -138,   457, -1144,  -477,  -847,  -957, -1144,  -671,
    -525, -1143,  -873,  -902,   -40, -1144,    51, -1144,  -234,  -486,
    -416,   572,  -453, -1144, -1144, -1144,  1542, -1144,   -10, -1144,
   -1144,  -188, -1144,  -649, -1144, -1144, -1144,  1590,  1796,   -12,
     933,   154,   800, -1144,  2002,  2225, -1144, -1144, -1144, -1144,
   -1144, -1144, -1144, -1144, -1144,  -396
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   339,   109,   681,   111,   112,   113,   114,
     115,   357,   489,   116,   249,   117,   340,   672,   561,   676,
     673,   118,   119,   120,   558,   559,   121,   122,   184,   967,
     281,   123,   276,   124,   713,   286,   125,  1066,   126,   298,
     127,   128,   129,   454,   650,   450,   647,   130,   131,   813,
     132,   247,   133,   689,   690,   196,   135,   136,   137,   138,
     139,   524,   730,   895,   140,   141,   726,   890,   142,   143,
     565,   914,   144,   145,   770,   771,   772,   197,   279,   704,
     147,   148,   567,   922,   777,   970,   971,   486,  1073,   496,
     699,   700,   701,   702,   703,   778,   366,   876,  1199,  1259,
    1182,   444,  1112,  1116,  1176,  1177,  1178,   149,   327,   529,
     150,   151,   282,   283,   500,   501,   717,  1196,  1143,   504,
     714,  1218,  1109,  1027,   341,   213,   345,   346,   445,   446,
     447,   198,   153,   154,   155,   156,   199,   158,   181,   182,
     623,   466,   835,   624,   625,   159,   160,   200,   201,   163,
     233,   448,   203,   165,   204,   205,   168,   169,   170,   171,
     351,   172,   173,   174,   175,   176
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     179,   299,   675,   731,   202,   687,   108,   436,   207,   206,
     418,   836,   367,   969,   214,   911,   490,   698,   910,   227,
     227,   705,   237,   239,   241,   244,   248,   627,   302,   222,
     209,   455,  1030,  1077,  1078,   725,   879,   287,  -262,   288,
     917,  -263,  1067,   284,   293,   505,   294,   887,   295,   634,
     715,   470,   472,   475,   284,   284,   303,   307,   309,   311,
     312,   313,   831,   512,    23,   317,   349,   318,   546,   321,
     325,   437,   502,   838,   328,   732,  1107,   330,   331,  1243,
     332,   522,   333,   551,  1141,   334,   335,   301,   336,   888,
     481,    69,   303,   307,   350,   352,   823,   537,  1053,   826,
     583,  1181,   502,   583,   540,   727,   502,  -565,  -287,  1224,
     763,   439,   552,   177,  -628,    59,  -628,   764,  1264,   534,
     584,  1104,   538,   301,   344,   301,    67,  1032,   443,  1079,
     440,   541,   976,  1054,   585,  1221,   634,  1225,  1173,   464,
     464,   975,   506,   889,  -469,   208,   547,   812,   371,  -474,
    -287,    87,  1173,   224,   443,   430,   250,   299,   432,   464,
     545,   549,   358,   586,  1258,   547,  -474,   514,   418,   432,
     236,   238,   240,   179,  -469,   635,   426,   427,   428,   432,
    1113,   317,   303,   350,  1166,   613,   614,  -469,  1068,  1139,
     432,   432,  1195,  1067,   548,  -474,  -474,   435,   432,   587,
    -469,   443,   587,  1168,  -287,  1029,   227,  1114,   588,   432,
    1201,  1202,   307,   301,   301,   503,  -474,   441,  1067,   443,
    1067,   374,  -474,   308,   547,  -474,  1181,  1142,   359,   878,
     821,     3,   547,   547,   788,   513,   452,  1175,   307,    23,
    1174,   457,   348,  -474,    69,   503,  -474,  -469,  1215,   503,
     209,  1175,  -469,  1033,   -82,    23,   865,  1167,   464,   347,
     464,  1222,  1257,  1115,   627,  -474,  1193,   542,   378,   906,
     991,   790,   -53,   993,  -474,   789,   384,   375,   912,    42,
     -53,   376,  1255,   514,  1067,   307,  1067,   675,   589,   177,
      59,   -53,   516,   517,   543,  -470,  1219,   866,   791,   -53,
    1262,    67,   515,   307,    57,   514,    59,  1134,   363,  1136,
    1133,   969,  1135,   544,   649,   898,  1090,    67,  1092,   653,
     -82,  1216,   432,   443,   443,  -470,    87,   364,   378,   988,
     646,   539,   829,   382,   716,   383,   384,   365,  -470,   863,
     387,   289,    87,  -245,   536,  -111,   560,   562,   394,  1244,
     183,  -470,  1096,  -117,   398,   399,   400,   862,   569,  1111,
     570,   571,   572,   574,   576,   577,   864,   578,   579,   580,
     581,   582,   773,   590,   591,   592,   593,   594,   595,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     698,   919,   872,  1025,   627,   443,   307,   307,  -470,  -475,
     290,   646,   307,  -470,   848,   307,   307,   307,   307,   616,
     618,   626,  -475,  1156,  -462,  1158,   920,   487,   291,   655,
     488,  1001,   638,   774,   646,   443,   177,   642,   985,  1130,
     643,  1051,   751,  -579,   775,   292,  1065,   691,   691,  -464,
    -578,   451,  -475,   873,  -462,   646,   456,   303,  1002,   657,
     659,  1059,   180,   776,   874,   663,   665,  -462,   692,   692,
    -579,   669,   693,   693,   674,   674,   210,  -578,  -475,  -464,
    -462,   682,  1088,   875,   684,  -475,   840,   843,   301,   307,
     845,   847,  -464,  -246,   506,   694,   694,   506,   695,   695,
      61,   353,  -252,   718,   754,  -464,  -475,   720,   721,   696,
     696,   421,   724,   568,  -251,   422,   641,   758,  1118,   307,
     805,   573,   575,   755,   422,  -475,  -260,  -462,   697,   697,
    -475,   506,  -462,  -475,   765,   724,   759,   506,   307,  -440,
     307,   224,   896,   723,   743,   724,   746,   443,   748,   750,
     824,  -113,  -464,   419,  -492,   420,   827,  -464,  -440,  -118,
     443,  -629,   303,  -629,   899,   901,   723,   615,   617,   344,
     536,   344,   608,   609,  -250,  -492,   723,   250,   610,  -492,
     637,   611,   612,  1110,   152,  -628,  -625,  -628,  -625,   675,
    1119,   675,  -374,   301,   152,  -484,   432,  -627,   492,  -627,
     691,   737,  -492,   739,   698,   320,   324,   152,   658,   374,
     152,  -626,  -247,  -626,   664,   627,  -484,  -254,  -258,   668,
    -484,   692,  1065,   374,  1065,   693,   657,   808,   663,   811,
     682,   307,   814,   746,   972,  -255,   296,   816,   817,   297,
    -249,   314,   307,  -484,   307,  -253,   315,   355,   694,  -259,
    -261,   695,   828,    61,   830,  -248,   378,  -256,   626,   152,
     837,   382,   696,   360,   384,   375,  -562,   583,   387,   376,
     905,   589,   356,   819,   368,   852,   853,  -561,   443,   375,
     369,   697,   881,   376,   584,   285,   152,   370,  1065,  -566,
    1065,   152,   742,  1223,  -567,  -564,   431,  -563,   585,  1197,
    -479,   425,   438,   442,   347,   423,   347,   285,   313,   317,
     350,   449,   432,  -479,   307,   459,   378,   319,   323,   453,
     491,   382,   495,   383,   384,   509,  1220,   586,   387,   443,
     378,   510,   518,   528,   521,   382,   394,   553,   384,   530,
    1233,   301,   387,  -479,   400,  1247,  1248,  1249,  1250,  1251,
    1252,   557,   560,   384,  1226,   564,   907,   607,   909,   674,
     674,   566,   307,   152,   640,   644,   587,   633,   646,  -479,
     645,   654,   588,   660,   652,   807,  -479,   810,   677,  1220,
    1227,   678,  1228,  1229,   680,   683,  1230,  1231,   626,   686,
     706,   707,   716,   722,   728,   729,  1256,  -479,   733,   741,
     747,   786,   574,   616,   979,   779,   766,   982,   820,   787,
    1120,  1121,   164,   307,  1263,   780,  -479,   815,  1220,   781,
     782,  -479,   164,   783,  -479,   784,   785,   -78,   848,   998,
     849,  1000,   854,   851,   856,   164,  1005,   861,   164,  1008,
     868,   514,   433,   869,   877,   886,  1012,   921,  1013,   891,
     915,   918,   980,   974,   983,  1016,   986,   992,  1038,   994,
     152,  1018,  1020,  1026,  1028,  1031,   152,  1045,  1034,  1040,
    1035,  1057,   285,   285,   285,   285,   285,   285,  1049,  1058,
     883,  1060,  -138,   482,   485,  1072,   303,   164,  -145,  -144,
    -114,   322,   322,  -143,  -140,  -147,  1044,  1089,  1048,  -481,
    -112,  1050,  -148,  -142,   674,  -115,  -146,  -141,  1075,  -149,
    -116,  1076,  -481,   493,   164,  1101,  1069,   301,  1103,   164,
    1198,  1091,   152,   285,  -111,  1105,  -113,  1225,  1234,   285,
     685,  1137,  1149,   556,  1189,  1124,   904,   152,   211,  1235,
    1184,  1190,  -481,  1191,  1192,  1185,  1140,   285,   443,   443,
     573,   615,  1254,  1200,   329,  1021,   526,  1039,  1148,  1070,
    1144,   867,  1024,  1071,  1074,   285,   285,  1260,  -481,  1241,
    1253,  1246,   243,   719,   228,  -481,     0,     0,   998,  1000,
       0,  1005,  1008,     0,  1044,  1048,     0,     0,     0,     0,
       0,   164,     0,     0,  1093,  1094,  -481,  1095,     0,   626,
       0,  1097,  1098,     0,     0,  1099,     0,     0,     0,     0,
     299,     0,     0,     0,   152,  -481,     0,     0,     0,     0,
    -481,     0,     0,  -481,     0,     0,     0,     0,     0,     0,
    1117,   746,   307,   307,     0,     0,     0,     0,  1125,     0,
    1126,     0,     0,   152,  1128,     0,     0,     0,     0,   152,
    1132,   674,   674,   674,   674,  1108,     0,     0,     0,     0,
       0,     0,     0,   152,     0,   152,     0,     0,     0,     0,
       0,   285,     0,     0,     0,     0,  1170,     0,  1093,  1150,
    1151,  1097,  1152,  1153,  1154,  1155,     0,     0,   164,     0,
       0,     0,     0,   134,   164,     0,     0,     0,  1164,     0,
       0,     0,   285,   134,   350,     0,   285,     0,   285,  1172,
       0,   285,     0,  1179,     0,     0,   134,     0,     0,   134,
       0,     0,     0,     0,     0,     0,   152,     0,     0,     0,
     152,     0,     0,     0,     0,   301,     0,     0,   152,     0,
    1171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     164,     0,     0,     0,     0,     0,     0,     0,  1211,  1212,
    1213,  1214,     0,     0,  1240,   164,     0,     0,   134,     0,
       0,     0,     0,   146,     0,     0,     0,     0,     0,     0,
       0,  1048,     0,   146,     0,  1236,  1237,     0,     0,     0,
       0,     0,   350,     0,     0,   134,   146,     0,  1217,   146,
     134,  1211,  1212,  1213,  1214,  1236,  1237,   803,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1242,
     746,     0,  1245,   301,     0,     0,     0,     0,     0,     0,
       0,   913,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   164,     0,     0,     0,     0,     0,   146,     0,
       0,  1217,  -299,     0,     0,   746,  -299,  -299,     0,  1048,
       0,  -299,   152,     0,     0,   152,  -299,     0,  -299,  -299,
       0,   164,   134,     0,  -299,   146,     0,   164,     0,     0,
     146,  -299,     0,     0,  -299,     0,     0,     0,     0,     0,
    1217,   164,     0,   164,   285,   285,     0,     0,     0,     0,
     285,   285,     0,  -299,   285,   285,  -299,     0,  -299,     0,
    -299,     0,  -299,  -299,     0,  -299,     0,  -299,     0,  -299,
     860,     0,     0,   152,     0,     0,     0,     0,     0,   152,
       0,     0,     0,     0,     0,     0,     0,     0,  -299,     0,
       0,  -299,     0,     0,  -299,     0,     0,     0,     0,     0,
       0,     0,   146,     0,   164,     0,     0,     0,   164,     0,
     973,     0,     0,     0,     0,   893,   164,     0,     0,   134,
       0,     0,     0,     0,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   977,   978,     0,     0,
       0,   215,     0,     0,  -299,     0,     0,     0,     0,     0,
    -299,     0,     0,     0,     0,   216,     0,     0,     0,    22,
      23,     0,     0,   152,     0,   152,     0,     0,     0,   217,
     152,    31,   218,   152,     0,     0,     0,    37,     0,     0,
       0,   134,     0,     0,    42,     0,   152,     0,   152,     0,
       0,   152,     0,     0,     0,     0,   134,     0,   -51,   146,
       0,     0,   285,   285,     0,   146,     0,     0,     0,    57,
       0,    59,     0,     0,   285,   219,   485,   -51,   220,   152,
     221,     0,    67,   485,     0,     0,   285,     0,     0,   285,
     152,   285,   152,   285,     0,     0,     0,     0,     0,     0,
     164,    83,     0,   164,    85,     0,     0,    87,     0,   251,
    1022,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   146,     0,   252,   253,     0,   254,     0,     0,     0,
       0,   255,     0,   134,     0,     0,   146,     0,     0,     0,
     256,  1041,     0,     0,     0,     0,   257,     0,     0,     0,
       0,     0,   258,     0,     0,     0,   259,   102,     0,   260,
       0,   164,   134,     0,   157,     0,     0,   164,   134,   261,
       0,     0,     0,     0,   157,   262,   263,     0,     0,     0,
       0,     0,   134,   264,   134,     0,     0,   157,   152,     0,
     157,     0,   265,     0,     0,   152,     0,     0,     0,     0,
       0,   266,   267,     0,   268,     0,   269,     0,   270,     0,
     152,   271,   161,   146,     0,   272,   497,     0,   273,     0,
       0,   274,   161,     0,     0,     0,     0,     0,   152,     0,
       0,     0,     0,     0,     0,   161,     0,   152,   161,   157,
       0,   152,   146,     0,     0,   134,     0,     0,   146,   134,
       0,   164,     0,   164,     0,     0,     0,   134,   164,     0,
       0,   164,   146,     0,   146,     0,   157,     0,     0,     0,
       0,   157,     0,     0,   164,     0,   164,   498,     0,   164,
       0,     0,     0,     0,   285,     0,     0,   161,     0,     0,
       0,     0,     0,   152,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   164,     0,     0,
       0,     0,     0,     0,   161,     0,     0,     0,   164,   161,
     164,     0,     0,     0,     0,   146,     0,     0,     0,   146,
       0,     0,     0,     0,     0,     0,     0,   146,     0,     0,
     485,   485,     0,   157,   485,   485,     0,     0,     0,     0,
       0,     0,   152,   152,   152,   152,     0,     0,     0,     0,
       0,     0,  1180,     0,     0,     0,     0,     0,     0,     0,
    1186,     0,   485,     0,   485,     0,     0,     0,     0,   152,
     152,   134,     0,   251,   134,     0,     0,     0,     0,     0,
       0,   161,     0,     0,     0,     0,     0,   252,   253,     0,
     254,     0,     0,     0,     0,   255,     0,     0,     0,     0,
       0,     0,     0,     0,   256,     0,   164,     0,   162,     0,
     257,     0,     0,   164,     0,     0,   258,     0,   162,     0,
     259,     0,     0,   260,     0,     0,     0,     0,   164,     0,
     157,   162,   134,   261,   162,     0,   157,     0,   134,   262,
     263,     0,     0,     0,     0,     0,   164,   264,     0,     0,
       0,   146,     0,     0,   146,   164,   265,     0,     0,   164,
       0,     0,     0,     0,     0,   266,   267,     0,   268,     0,
     269,     0,   270,     0,     0,   271,     0,  1180,   161,   272,
       0,     0,   273,   162,   161,   274,     0,     0,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,     0,
     162,   164,   146,     0,     0,   162,     0,     0,   146,     0,
       0,     0,   134,     0,   134,     0,     0,     0,     0,   134,
       0,     0,   134,     0,     0,     0,     0,     0,     0,   432,
     161,   892,     0,     0,     0,   134,     0,   134,     0,     0,
     134,     0,     0,     0,     0,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     164,   164,   164,   164,     0,     0,     0,     0,   134,     0,
       0,     0,     0,     0,   157,     0,     0,   162,     0,   134,
       0,   134,     0,     0,     0,     0,     0,   164,   164,   374,
       0,     0,   146,     0,   146,     0,     0,     0,     0,   146,
       0,     0,   146,   157,   166,     0,     0,     0,     0,   157,
       0,     0,     0,     0,   166,   146,     0,   146,     0,     0,
     146,     0,   161,   157,     0,   157,     0,   166,     0,     0,
     166,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,     0,   146,   376,
       0,   161,     0,     0,     0,     0,     0,   161,     0,   146,
       0,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   161,     0,   161,   162,     0,     0,   134,     0,   166,
     162,     0,     0,     0,   134,     0,   157,     0,     0,   375,
     157,     0,     0,   376,     0,     0,   378,   379,   157,   134,
     381,   382,     0,   383,   384,   374,   166,     0,   387,     0,
       0,   166,     0,     0,     0,     0,   394,   134,     0,     0,
       0,     0,   398,   399,   400,     0,   134,     0,     0,     0,
     134,     0,     0,     0,   161,     0,   162,     0,   161,     0,
     378,   379,     0,   380,   381,   382,   161,   383,   384,   385,
       0,   162,   387,     0,     0,     0,     0,   146,     0,   393,
     394,   375,     0,   397,   146,   376,   398,   399,   400,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,   146,
       0,     0,   134,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   146,     0,     0,
       0,     0,     0,     0,     0,     0,   146,     0,     0,     0,
     146,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,     0,   157,     0,   387,   157,     0,   167,   162,     0,
       0,   393,   394,     0,     0,   397,     0,   167,   398,   399,
     400,   134,   134,   134,   134,     0,     0,     0,     0,     0,
     167,   374,     0,   167,     0,     0,     0,   162,     0,     0,
       0,     0,   146,   162,     0,     0,     0,     0,   134,   134,
     161,     0,     0,   161,     0,     0,     0,   162,     0,   162,
     166,     0,     0,   157,     0,     0,   166,     0,     0,   157,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   167,     0,     0,     0,     0,   375,     0,     0,
       0,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   146,   146,   146,   146,     0,     0,     0,     0,   167,
       0,   161,     0,     0,   167,     0,     0,   161,     0,     0,
     162,     0,   166,     0,   162,     0,     0,     0,   146,   146,
       0,     0,   162,     0,     0,     0,     0,   166,   378,   379,
       0,   380,   381,   382,     0,   383,   384,     0,     0,     0,
     387,     0,     0,   157,     0,   157,     0,     0,   394,     0,
     157,     0,     0,   157,   398,   399,   400,   110,     0,     0,
       0,     0,     0,     0,     0,     0,   157,   178,   157,     0,
     374,   157,     0,     0,     0,     0,   167,     0,     0,     0,
     212,     0,     0,   223,     0,     0,     0,     0,     0,     0,
       0,   161,     0,   161,     0,     0,     0,     0,   161,   157,
       0,   161,     0,     0,   166,     0,     0,     0,     0,     0,
     157,     0,   157,     0,   161,     0,   161,     0,     0,   161,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
     376,     0,   316,   166,     0,     0,     0,     0,     0,   166,
       0,     0,     0,     0,     0,     0,   162,   161,     0,   162,
       0,     0,     0,   166,     0,   166,     0,     0,   161,   110,
     161,     0,     0,     0,   354,     0,     0,     0,     0,     0,
       0,     0,     0,   167,     0,     0,     0,   378,   379,   167,
       0,  1138,   382,     0,   383,   384,     0,     0,     0,   387,
       0,     0,     0,     0,     0,   216,     0,   394,     0,    22,
      23,     0,     0,   398,   399,   400,     0,   162,   157,   217,
       0,    31,   218,   162,     0,   157,   166,    37,   310,     0,
     166,     0,     0,     0,    42,     0,     0,     0,   166,     0,
     157,     0,     0,     0,     0,   167,   424,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   157,    57,
     167,    59,     0,     0,     0,  1062,   161,   157,  1063,     0,
     221,   157,    67,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   161,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,   161,   162,     0,   162,
       0,     0,     0,     0,   162,   161,     0,   162,     0,   161,
       0,     0,     0,   157,     0,     0,     0,     0,     0,     0,
     162,     0,   162,     0,     0,   162,     0,   167,     0,     0,
       0,     0,     0,   494,     0,     0,     0,   102,     0,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,   162,     0,   166,   167,     0,     0,     0,
       0,   161,   167,     0,   162,     0,   162,     0,     0,     0,
       0,     0,   157,   157,   157,   157,   167,     0,   167,     0,
       0,     0,     0,   460,   463,   465,   469,   471,   474,     0,
       0,     0,     0,     0,     0,   110,     0,     0,     0,   157,
     157,     0,     0,     0,     0,     0,     0,   275,   277,   278,
     110,     0,     0,   166,     0,     0,     0,     0,     0,   166,
     161,   161,   161,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   508,     0,     0,     0,     0,   167,
     511,     0,     0,   167,     0,     0,   326,   161,   161,     0,
       0,   167,     0,     0,     0,     0,     0,   583,   520,     0,
       0,     0,   162,     0,     0,     0,     0,     0,     0,   162,
       0,   792,     0,     0,   793,     0,   531,   532,     0,   794,
       0,     0,     0,     0,   162,     0,     0,   110,     0,     0,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     795,     0,   162,   166,     0,   166,   361,   796,   362,     0,
     166,   162,     0,   166,     0,   162,   507,   797,     0,     0,
       0,     0,   507,     0,   798,     0,   166,     0,   166,   216,
       0,   166,     0,    22,    23,     0,   679,     0,  1061,     0,
     799,     0,     0,   217,     0,    31,   218,     0,     0,     0,
       0,    37,   800,     0,   429,     0,   587,     0,    42,   166,
       0,     0,     0,   801,     0,     0,     0,   162,     0,   802,
     166,   326,   166,     0,     0,   167,     0,     0,   167,     0,
       0,     0,   639,    57,     0,    59,     0,    61,     0,  1062,
       0,     0,  1063,     0,   221,   326,    67,   458,     0,   740,
       0,     0,     0,   745,     0,     0,     0,     0,   479,   480,
       0,   110,     0,   661,     0,    83,     0,   666,    85,   667,
       0,    87,   670,     0,     0,     0,   162,   162,   162,   162,
       0,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,   167,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   162,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,     0,
       0,   102,     0,     0,     0,   166,     0,  1064,     0,     0,
       0,     0,     0,   216,     0,     0,     0,    22,    23,     0,
     166,     0,  1061,     0,     0,     0,     0,   217,     0,    31,
     218,     0,     0,     0,     0,    37,     0,     0,   166,     0,
       0,     0,    42,     0,     0,     0,     0,   166,     0,     0,
       0,   166,     0,   563,     0,     0,   167,     0,   167,     0,
       0,     0,     0,   167,     0,   855,   167,    57,   858,    59,
       0,    61,     0,  1062,     0,     0,  1063,     0,   221,   167,
      67,   167,     0,     0,   167,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
       0,     0,    85,   166,     0,    87,     0,     0,     0,     0,
       0,     0,   167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   167,   636,   167,   897,     0,     0,     0,
       0,     0,   902,     0,     0,   832,   834,     0,     0,     0,
       0,   839,   842,   648,     0,   844,   846,   651,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,     0,
       0,  1157,   166,   166,   166,   166,   216,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,    31,   218,     0,   688,     0,     0,    37,   166,
     166,   499,     0,     0,     0,    42,     0,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,     0,     0,     0,   507,     0,   507,     0,
      57,   167,    59,   507,     0,     0,   507,     0,   167,  -586,
       0,   221,     0,    67,     0,     0,     0,     0,     0,  1014,
       0,  1015,   417,   167,  1017,  -625,     0,  -625,     0,   251,
     753,     0,    83,     0,     0,    85,     0,     0,    87,   769,
       0,   167,     0,   252,   253,     0,   254,     0,     0,     0,
     167,   255,  1037,     0,   167,     0,     0,     0,     0,     0,
     256,     0,     0,  1043,     0,  1047,   257,     0,     0,     0,
       0,     0,   258,   989,   990,     0,   259,     0,     0,   260,
       0,     0,     0,     0,     0,   995,     0,     0,     0,   261,
       0,     0,     0,     0,     0,   262,   263,  1006,     0,     0,
    1009,     0,  1010,   264,  1011,     0,   167,     0,     0,     0,
       0,     0,   265,     0,     0,     0,     0,     0,     0,     0,
     822,   266,   267,   825,   268,     0,   269,     0,   270,     0,
       0,   271,     0,     0,     0,   272,     0,     0,   273,     0,
       0,   274,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,     0,     0,     0,
       0,     0,     0,     0,     0,   167,   167,   167,   167,     0,
       0,     0,     0,     0,     0,   871,     0,     0,     0,   499,
       0,     0,     0,  1102,     0,     0,   417,     0,     0,  -625,
       0,  -625,   167,   167,     0,     0,     0,   280,     0,     0,
       0,  1122,   894,     0,     0,     0,   326,     0,     0,     0,
    1127,     0,     0,  1238,  1129,     5,   300,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   185,     0,     0,    15,
      16,     0,    17,     0,   186,     0,     0,    21,     0,     0,
       0,   769,     0,     0,     0,     0,    29,     0,   187,     0,
       0,   968,    33,   188,   189,     0,     0,   190,    39,     0,
       0,     0,    41,     0,     0,    43,  1165,     0,   191,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,  1123,     0,     0,    65,   192,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   193,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,  1019,    91,   688,     0,     0,     0,
    1023,     0,     0,   499,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,   894,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,  1239,     0,   107,   769,
      -2,     4,   769,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -80,     0,    40,
      41,    42,     0,    43,  -316,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -316,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -316,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -80,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,   968,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,  -655,     0,    12,    13,
      14,    15,    16,  -655,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,  -655,    28,    29,  -655,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -80,     0,    40,    41,    42,     0,    43,  -316,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -316,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -316,   -51,    64,
      65,    66,  -655,    67,    68,    69,  -655,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -80,    87,    88,
       0,     0,    89,     0,    90,     0,     0,  -655,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -655,  -655,    94,  -655,  -655,  -655,  -655,
    -655,  -655,  -655,     0,  -655,  -655,  -655,  -655,  -655,     0,
    -655,  -655,  -655,  -655,  -655,  -655,  -655,  -655,   102,  -655,
    -655,  -655,     0,   104,  -655,   105,     0,   106,     0,   337,
    -655,     5,   300,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -80,     0,    40,    41,    42,
       0,    43,  -316,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -316,     0,     0,    57,    58,    59,    60,    61,    62,
      63,  -316,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -80,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,   338,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -80,
       0,    40,    41,    42,     0,    43,  -316,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -316,     0,     0,    57,    58,
      59,    60,    61,    62,    63,  -316,   -51,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -80,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,   535,   105,     0,   106,     0,   554,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -80,     0,    40,    41,    42,     0,    43,
    -316,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -316,
       0,     0,    57,    58,    59,    60,    61,    62,    63,  -316,
     -51,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -80,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,   555,   105,     0,   106,
       0,   337,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -80,     0,    40,
      41,    42,     0,    43,  -316,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -316,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -316,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -80,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
     338,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -80,     0,    40,    41,    42,     0,    43,  -316,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -316,     0,     0,
      57,    58,    59,    60,    61,    62,    63,  -316,   -51,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -80,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,   752,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -80,     0,    40,    41,    42,
       0,    43,  -316,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -316,     0,     0,    57,    58,    59,    60,   353,    62,
      63,  -316,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -80,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -80,
       0,    40,    41,    42,     0,    43,  -316,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -316,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -316,   -51,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -80,    87,    88,     0,     0,
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
      37,   656,    39,   -80,     0,    40,    41,    42,     0,    43,
    -316,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -316,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -316,
     -51,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -80,
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
      33,    34,    35,    36,    37,   662,    39,   -80,     0,    40,
      41,    42,     0,    43,  -316,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -316,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -316,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -80,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,   997,
      39,   -80,     0,    40,    41,    42,     0,    43,  -316,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -316,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -316,   -51,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -80,    87,    88,
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
      35,    36,    37,   999,    39,   -80,     0,    40,    41,    42,
       0,    43,  -316,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -316,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -316,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -80,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1004,    39,   -80,
       0,    40,    41,    42,     0,    43,  -316,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -316,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -316,   -51,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -80,    87,    88,     0,     0,
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
      37,  1007,    39,   -80,     0,    40,    41,    42,     0,    43,
    -316,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -316,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -316,
     -51,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -80,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,     0,   105,     0,   106,
       0,     4,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,  1036,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -80,     0,    40,
      41,    42,     0,    43,  -316,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -316,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -316,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -80,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1042,
      39,   -80,     0,    40,    41,    42,     0,    43,  -316,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -316,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -316,   -51,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -80,    87,    88,
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
      35,    36,    37,  1046,    39,   -80,     0,    40,    41,    42,
       0,    43,  -316,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -316,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -316,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -80,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,   880,   107,     5,   300,     6,     7,     8,
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
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
     242,    23,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,    59,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,    67,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,    87,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   194,     0,   105,     0,   195,     0,     0,
     107,     5,   300,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,   190,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   193,     0,
      78,     0,     0,    80,   304,   305,     0,    84,   342,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,   343,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,   195,     0,     0,   107,     5,   300,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   185,     0,     0,    15,
      16,     0,    17,     0,   186,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   187,     0,
       0,     0,    33,   188,   189,     0,     0,   190,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   191,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   192,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   193,     0,    78,     0,     0,    80,   304,   305,
       0,    84,   342,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,   736,   195,     0,     0,   107,     5,
     300,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     185,     0,     0,    15,    16,     0,    17,     0,   186,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   187,     0,     0,     0,    33,   188,   189,     0,
       0,   190,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   191,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   192,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   193,     0,    78,     0,
       0,    80,   304,   305,     0,    84,   342,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   194,     0,   105,   738,   195,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,   619,     0,     0,     0,     0,
       0,     0,    29,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,   190,    39,     0,   620,     0,    41,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,   621,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   193,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,   622,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,   195,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,   304,   305,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   306,     0,     0,   103,     0,   194,
       0,   105,     0,   195,     0,     0,   107,     5,   300,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
     304,   305,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   194,     0,   105,     0,   195,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     185,     0,     0,    15,    16,     0,    17,     0,   186,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   187,     0,     0,     0,    33,   188,   189,     0,
       0,   190,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   191,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,   882,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   192,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   193,     0,    78,     0,
       0,    80,   304,   305,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   194,     0,   105,     0,   195,
       0,     5,   107,     6,     7,     8,   229,    10,    11,   230,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,   190,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   231,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   193,     0,
      78,     0,     0,    80,     0,     0,   232,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,   195,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,   234,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,   235,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   194,
       0,   105,     0,   195,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   185,     0,     0,    15,
      16,     0,    17,     0,   186,     0,     0,    21,     0,   245,
       0,     0,     0,     0,     0,     0,    29,     0,   187,     0,
       0,     0,    33,   188,   189,     0,     0,   190,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   191,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   192,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   193,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,   246,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,     0,     0,   107,     5,
     300,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     185,     0,     0,    15,    16,     0,    17,     0,   186,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   187,     0,     0,     0,    33,   188,   189,     0,
       0,   190,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   191,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   192,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   193,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,   235,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   194,     0,   105,     0,   195,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,   190,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   193,     0,
      78,     0,     0,    80,   304,   305,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,   195,     0,     0,   107,     5,   300,     6,     7,     8,
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
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,   734,   105,     0,   195,     0,     0,   107,     5,
     300,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     185,     0,     0,    15,    16,     0,    17,     0,   186,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   187,     0,     0,     0,    33,   188,   189,     0,
       0,   190,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   191,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   192,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   193,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   194,     0,   105,     0,   195,
     744,     0,   107,     5,   300,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   194,
       0,   105,     0,   195,  1169,     5,   107,     6,     7,     8,
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
       0,     0,     0,   235,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   284,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   104,     0,   105,     0,   195,     0,     0,
     107,     5,   300,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,   190,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   193,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,   195,     0,     5,   107,     6,     7,     8,   229,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   231,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,   232,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   194,
       0,   105,     0,   195,     0,     5,   107,     6,     7,     8,
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
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,   749,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,   908,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   194,     0,   105,     0,   195,     0,     0,
     107,     5,   300,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   708,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,   190,    39,     0,     0,     0,   709,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,     0,     0,     0,
       0,     0,     0,    72,   710,    74,    75,    76,   711,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,  1106,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,     0,    84,
    1131,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   194,
       0,   105,     0,   195,     0,     0,   107,     5,   300,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   194,     0,   105,     0,  1106,     0,     0,
     107,     5,   300,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,  1087,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   193,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,   195,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   194,
       0,   105,     0,   195,     0,     5,   107,     6,     7,     8,
     225,    10,    11,     0,     0,     0,   185,     0,     0,    15,
      16,     0,    17,     0,   186,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   187,     0,
       0,     0,    33,   188,   189,     0,     0,   190,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   191,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   226,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   193,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   190,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   104,     0,   105,     0,   195,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     185,     0,     0,    15,    16,     0,    17,     0,   186,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   187,     0,     0,     0,    33,   188,   189,     0,
       0,   190,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   191,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   192,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   193,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   671,     0,   194,     0,   105,     0,   195,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   708,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,   190,    39,     0,     0,     0,   709,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,     0,     0,     0,
       0,     0,     0,    72,   710,    74,    75,    76,   711,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,   712,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   194,
       0,   105,     0,   712,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   185,     0,     0,    15,
      16,     0,    17,     0,   186,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   187,     0,
       0,     0,    33,   188,   189,     0,     0,   806,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   191,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   192,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   193,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,   809,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   194,     0,   105,     0,   195,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     185,     0,     0,    15,    16,     0,    17,     0,   186,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   187,     0,     0,     0,    33,   188,   189,     0,
       0,  1081,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   191,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   192,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   193,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   194,     0,   105,     0,   195,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,  1082,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   193,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,   195,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,  1084,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   194,
       0,   105,     0,   195,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   185,     0,     0,    15,
      16,     0,    17,     0,   186,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   187,     0,
       0,     0,    33,   188,   189,     0,     0,  1085,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   191,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   192,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   193,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   194,     0,   105,     0,   195,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   185,     0,
       0,    15,    16,     0,    17,     0,   186,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     187,     0,     0,     0,    33,   188,   189,     0,     0,  1086,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     191,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   192,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   193,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   194,     0,   105,     0,   195,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     185,     0,     0,    15,    16,     0,    17,     0,   186,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   187,     0,     0,     0,    33,   188,   189,     0,
       0,  1087,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   191,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   192,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   193,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   194,     0,   105,     0,   195,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   185,     0,     0,    15,    16,     0,    17,     0,
     186,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   708,     0,   187,     0,     0,     0,    33,   188,
     189,     0,     0,   190,    39,     0,     0,     0,   709,     0,
       0,    43,     0,     0,   191,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   192,     0,     0,     0,     0,
       0,     0,     0,    72,   710,    74,    75,    76,   711,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   194,     0,   105,
       0,  1194,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   923,     0,   924,     0,     0,     0,    92,
      93,    94,    95,    96,   925,     0,     0,     0,    97,   926,
     253,   927,   928,     0,     0,     0,     0,   929,    98,     0,
       0,    99,   100,   101,     0,     0,   256,   103,   187,     0,
       0,   105,   930,  1194,     0,     0,   107,     0,   931,     0,
       0,     0,   259,     0,     0,   932,     0,   933,     0,     0,
       0,     0,     0,     0,     0,   934,     0,     0,     0,     0,
       0,   935,   936,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,     0,   937,     0,
       0,     0,     0,     0,     0,     0,     0,   266,   267,     0,
     938,     0,   269,     0,   939,     0,     0,   940,     0,     0,
       0,   941,     0,     0,   273,     0,     0,   942,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   403,   404,   405,
     406,   407,     0,     0,   410,   411,   412,   413,     0,   415,
     416,   943,   944,   945,   946,   947,   948,     0,   949,     0,
       0,     0,   950,   951,   952,   953,   954,   955,   956,   957,
     958,   959,   960,     0,   961,     0,     0,   962,   963,   964,
     965,     0,     5,   966,     6,     7,     8,     9,    10,    11,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
       0,   100,   101,     0,     0,     0,   103,     0,   194,     0,
     105,     0,   195,     5,   300,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   185,     0,     0,    15,    16,     0,
      17,     0,   186,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   187,     0,     0,     0,
      33,   188,   189,     0,     0,   190,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   191,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   192,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     193,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,  -483,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,  -483,     0,     0,     0,  -483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,   194,
       0,   105,  -483,  1106,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,  -465,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,  -465,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,  -465,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,  -465,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,   101,  -465,    21,     0,     0,     0,
    -465,     0,   105,     0,  1194,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,     0,    94,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,   101,     0,    21,     0,     0,     0,
     194,     0,   105,     0,   195,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   185,     0,     0,    15,    16,
       0,    17,     0,   186,   101,     0,    21,     0,     0,     0,
     194,     0,   105,     0,   712,    29,     0,   187,     0,     0,
       0,    33,   188,   189,     0,     0,   190,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   191,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   192,     0,
     251,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   193,     0,    78,   252,   253,    80,   254,     0,     0,
      84,     0,   255,    86,     0,     0,    88,     0,     0,     0,
       0,   256,     0,     0,     0,     0,     0,   257,     0,     0,
       0,     0,     0,   258,     0,     0,     0,   259,     0,     0,
     260,     0,    94,     0,     0,     0,     0,     0,     0,     0,
     261,     0,     0,     0,     0,     0,   262,   263,     0,     0,
       0,     0,     0,     0,   264,     0,     0,     0,     0,     0,
       0,     0,   105,   265,   195,     0,     0,     0,     0,     0,
       0,     0,   266,   267,     0,   268,     0,   269,     0,   270,
       0,     0,   271,     0,  1146,     0,   272,     0,     0,   273,
       0,     0,   274,   925,     0,     0,     0,     0,   252,   253,
     927,   254,     0,     0,     0,     0,   255,     0,     0,     0,
       0,     0,     0,     0,     0,   256,     0,     0,     0,     0,
       0,   930,     0,     0,     0,     0,     0,   258,     0,     0,
       0,   259,     0,     0,   260,     0,   933,     0,     0,     0,
       0,     0,     0,     0,   261,     0,     0,     0,   870,     0,
     935,   263,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,   372,   266,   267,     0,   268,
       0,   269,   373,  1147,     0,     0,   940,     0,     0,     0,
     272,     0,     0,   273,     0,   374,   274,     0,     0,     0,
       0,     0,     0,     0,     0,   484,   403,   404,   405,   406,
     407,     0,     0,   410,   411,   412,   413,     0,   415,   416,
     943,   944,   945,   946,   947,   948,     0,   949,     0,     0,
       0,   950,   951,   952,   953,   954,   955,   956,   957,   958,
     959,   960,     0,   961,     0,     0,   962,   963,   964,   965,
       0,   375,     0,     0,     0,   376,     0,     0,   372,     0,
       0,     0,     0,     0,   483,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,   377,   484,   403,   404,
     405,   406,   407,     0,     0,   410,   411,   412,   413,     0,
     415,   416,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,     0,     0,   398,   399,
     400,     0,   432,     0,   375,     0,     0,     0,   376,   401,
       0,   372,     0,     0,     0,     0,     0,   996,   373,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,   377,
     484,   403,   404,   405,   406,   407,     0,     0,   410,   411,
     412,   413,     0,   415,   416,   378,   379,     0,   380,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,   389,
     390,     0,   391,   392,   393,   394,   395,   396,   397,     0,
       0,   398,   399,   400,     0,   432,     0,   375,     0,     0,
       0,   376,   401,     0,   372,     0,     0,     0,     0,     0,
    1003,   373,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,   377,   484,   403,   404,   405,   406,   407,     0,
       0,   410,   411,   412,   413,     0,   415,   416,   378,   379,
       0,   380,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,     0,     0,   398,   399,   400,     0,   432,     0,
     375,     0,     0,     0,   376,   401,     0,   372,     0,     0,
       0,     0,     0,  1160,   373,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,   377,   484,   403,   404,   405,
     406,   407,     0,     0,   410,   411,   412,   413,     0,   415,
     416,   378,   379,     0,   380,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,     0,     0,   398,   399,   400,
       0,   432,     0,   375,     0,     0,     0,   376,   401,     0,
     372,     0,     0,     0,     0,     0,  1161,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,   377,   484,
     403,   404,   405,   406,   407,     0,     0,   410,   411,   412,
     413,     0,   415,   416,   378,   379,     0,   380,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,   389,   390,
       0,   391,   392,   393,   394,   395,   396,   397,     0,     0,
     398,   399,   400,     0,   432,     0,   375,     0,     0,     0,
     376,   401,     0,   372,     0,     0,     0,     0,     0,  1162,
     373,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,   377,   484,   403,   404,   405,   406,   407,     0,     0,
     410,   411,   412,   413,     0,   415,   416,   378,   379,     0,
     380,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,     0,     0,   398,   399,   400,     0,   432,     0,   375,
       0,     0,     0,   376,   401,     0,   372,     0,     0,     0,
       0,     0,  1163,   373,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,   377,   484,   403,   404,   405,   406,
     407,     0,     0,   410,   411,   412,   413,     0,   415,   416,
     378,   379,     0,   380,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   398,   399,   400,     0,
     432,     0,   375,     0,     0,     0,   376,   401,     0,   372,
       0,     0,     0,     0,     0,  1187,   373,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,   377,   484,   403,
     404,   405,   406,   407,     0,     0,   410,   411,   412,   413,
       0,   415,   416,   378,   379,     0,   380,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,   389,   390,     0,
     391,   392,   393,   394,   395,   396,   397,     0,     0,   398,
     399,   400,     0,   432,     0,   375,     0,     0,     0,   376,
     401,     0,   372,     0,     0,     0,     0,     0,  1188,   373,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   374,   467,     0,     0,     0,     0,     0,     0,
     377,   484,   403,   404,   405,   406,   407,     0,   468,   410,
     411,   412,   413,     0,   415,   416,   378,   379,     0,   380,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
     389,   390,     0,   391,   392,   393,   394,   395,   396,   397,
       0,     0,   398,   399,   400,     0,   432,     0,   375,     0,
       0,     0,   376,   401,   372,     0,     0,     0,     0,     0,
       0,   373,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,   284,     0,     0,     0,   464,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
     473,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     379,     0,   380,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,   389,   390,     0,   391,   392,   393,   394,
     395,   396,   397,     0,     0,   398,   399,   400,     0,   432,
     375,     0,     0,     0,   376,     0,   401,   372,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   374,   461,     0,
       0,   464,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,   462,     0,     0,     0,     0,     0,     0,
       0,   378,   379,     0,   380,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,     0,     0,   398,   399,   400,
       0,   432,     0,   375,     0,     0,     0,   376,   401,     0,
     372,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,   476,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,   379,     0,   380,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,   389,   390,
     477,   391,   392,   393,   394,   395,   396,   397,     0,     0,
     398,   399,   400,     0,   432,     0,   375,     0,     0,     0,
     376,   401,     0,   372,   760,     0,     0,     0,     0,     0,
     373,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,   761,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,   379,     0,
     380,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,   389,   390,   762,   391,   392,   393,   394,   395,   396,
     397,     0,   478,   398,   399,   400,     0,     0,     0,   375,
       0,     0,     0,   376,   401,   372,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   374,   841,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,   380,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   398,   399,   400,     0,
       0,   375,     0,     0,     0,   376,     0,   401,   372,     0,
       0,     0,     0,     0,     0,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   374,   284,
       0,     0,   464,     0,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,     0,     0,   398,   399,
     400,     0,   432,     0,   375,     0,     0,     0,   376,   401,
     372,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,   464,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,   434,     0,     0,     0,
       0,     0,     0,     0,     0,   378,   379,     0,   380,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,   389,
     390,   374,   391,   392,   393,   394,   395,   396,   397,     0,
       0,   398,   399,   400,     0,   432,   375,     0,     0,     0,
     376,   372,   401,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   374,   284,     0,     0,     0,     0,   180,     0,     0,
       0,   377,     0,     0,     0,     0,     0,   375,     0,     0,
       0,   376,     0,     0,     0,     0,     0,   378,   379,     0,
     380,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,     0,   377,   398,   399,   400,     0,   375,     0,     0,
       0,   376,     0,   372,   401,     0,     0,     0,   378,   379,
     373,   380,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,   389,   374,   628,   391,   392,   393,   394,   395,
     396,   397,   377,     0,   398,   399,   400,     0,     0,   629,
       0,     0,     0,     0,     0,   401,     0,     0,   378,   379,
       0,   380,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,     0,     0,   398,   399,   400,     0,   432,   375,
       0,     0,     0,   376,   372,   401,     0,     0,     0,     0,
       0,   373,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,   630,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
     631,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,   380,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   398,   399,   400,     0,
     375,     0,     0,     0,   376,   372,     0,   401,     0,     0,
       0,     0,   373,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   374,   833,     0,     0,     0,
       0,     0,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   379,     0,   380,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,     0,     0,   398,   399,   400,
       0,   375,     0,     0,     0,   376,     0,   372,   401,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,     0,     0,   398,   399,
     400,     0,   432,   375,     0,     0,     0,   376,     0,   401,
     372,     0,     0,     0,     0,     0,   859,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,   379,     0,   380,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,   389,   390,
       0,   391,   392,   393,   394,   395,   396,   397,     0,     0,
     398,   399,   400,     0,   432,     0,   375,     0,     0,     0,
     376,   401,     0,   372,     0,     0,     0,     0,     0,     0,
     373,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,   464,  1055,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,   379,     0,
     380,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,   389,   390,  1056,   391,   392,   393,   394,   395,   396,
     397,     0,     0,   398,   399,   400,     0,     0,     0,   375,
       0,     0,   900,   376,   401,   372,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,   380,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   398,   399,   400,     0,
       0,   375,     0,     0,     0,   376,     0,   401,   372,     0,
       0,     0,     0,     0,     0,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,     0,     0,   398,   399,
     400,     0,   519,     0,   375,     0,     0,     0,   376,   401,
     372,     0,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,   379,     0,   380,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,   389,
     390,     0,   391,   392,   393,   394,   395,   396,   397,     0,
     523,   398,   399,   400,     0,     0,   375,     0,     0,     0,
     376,   372,   401,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,   379,     0,
     380,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,     0,   525,   398,   399,   400,     0,   375,     0,     0,
       0,   376,   372,     0,   401,     0,     0,     0,     0,   373,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,   379,
       0,   380,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,     0,   527,   398,   399,   400,     0,   375,     0,
       0,     0,   376,   372,     0,   401,     0,     0,     0,     0,
     373,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     379,     0,   380,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,   389,   390,     0,   391,   392,   393,   394,
     395,   396,   397,     0,   533,   398,   399,   400,     0,   375,
       0,     0,     0,   376,   372,     0,   401,     0,     0,     0,
       0,   373,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,   550,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,   380,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   398,   399,   400,     0,
     375,     0,     0,     0,   376,   372,     0,   401,     0,     0,
       0,     0,   373,   632,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   379,     0,   380,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,     0,     0,   398,   399,   400,
       0,   375,     0,     0,     0,   376,   372,   756,   401,     0,
       0,     0,     0,   373,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,     0,     0,   398,   399,
     400,     0,   375,     0,     0,   735,   376,   372,     0,   401,
       0,     0,     0,     0,   373,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,   379,     0,   380,   381,   382,     0,
     383,   757,   385,     0,   386,   387,   388,   389,   390,     0,
     391,   392,   393,   394,   395,   396,   397,     0,     0,   398,
     399,   400,     0,   375,     0,     0,     0,   376,   372,     0,
     401,     0,     0,     0,     0,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,   379,     0,   380,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,   389,   390,
    -587,   391,   392,   393,   394,   395,   396,   397,     0,     0,
     398,   399,   400,     0,   375,     0,     0,     0,   376,   372,
       0,   401,     0,     0,     0,     0,   373,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,   180,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,   379,     0,   380,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,   389,
     390,     0,   391,   392,   393,   394,   395,   396,   397,     0,
       0,   398,   399,   400,     0,   375,     0,     0,     0,   376,
     372,   850,   401,     0,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,   379,     0,   380,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
     389,   390,     0,   391,   392,   393,   394,   395,   396,   397,
       0,   818,   398,   399,   400,     0,   375,     0,     0,     0,
     376,   372,     0,   401,     0,     0,     0,     0,   373,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   374,     0,     0,   857,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,   379,     0,
     380,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,     0,     0,   398,   399,   400,     0,   375,     0,     0,
       0,   376,   372,     0,   401,     0,     0,     0,     0,   373,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,   379,
       0,   380,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,     0,     0,   398,   399,   400,     0,   375,     0,
       0,     0,   376,   372,     0,   401,     0,     0,     0,     0,
     373,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     379,     0,   380,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,   389,   390,     0,   391,   392,   393,   394,
     395,   396,   397,     0,     0,   398,   399,   400,     0,   375,
       0,     0,   884,   376,   372,     0,   401,     0,     0,     0,
       0,   373,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,   380,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   398,   399,   400,     0,
     375,     0,     0,   885,   376,   372,     0,   401,     0,     0,
       0,     0,   373,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   374,   981,     0,     0,     0,
       0,     0,     0,     0,   903,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   379,     0,   380,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,     0,     0,   398,   399,   400,
       0,   375,     0,     0,     0,   376,   372,     0,   401,     0,
       0,     0,     0,   373,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   374,   984,     0,     0,
       0,     0,     0,     0,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,     0,     0,   398,   399,
     400,     0,   375,     0,     0,     0,   376,   372,     0,   401,
       0,     0,     0,     0,   373,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,   379,     0,   380,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,   389,   390,     0,
     391,   392,   393,   394,   395,   396,   397,     0,     0,   398,
     399,   400,     0,   375,     0,     0,     0,   376,   372,  1052,
     401,     0,     0,     0,     0,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,   379,     0,   380,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,   389,   390,
       0,   391,   392,   393,   394,   395,   396,   397,     0,     0,
     398,   399,   400,     0,   375,     0,     0,     0,   376,   987,
     372,   401,     0,     0,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,   379,     0,   380,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,   389,
     390,     0,   391,   392,   393,   394,   395,   396,   397,     0,
       0,   398,   399,   400,     0,     0,   375,     0,     0,     0,
     376,   372,   401,     0,     0,     0,     0,     0,   373,  1080,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,   379,     0,
     380,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,     0,     0,   398,   399,   400,     0,   375,     0,     0,
       0,   376,   372,  1100,   401,     0,     0,     0,     0,   373,
    1083,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,   379,
       0,   380,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,     0,     0,   398,   399,   400,     0,   375,     0,
       0,     0,   376,   372,     0,   401,     0,     0,     0,     0,
     373,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     379,     0,   380,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,   389,   390,     0,   391,   392,   393,   394,
     395,   396,   397,     0,     0,   398,   399,   400,     0,   375,
       0,     0,     0,   376,   372,     0,   401,     0,     0,     0,
       0,   373,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,   380,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   398,   399,   400,     0,
     375,     0,     0,  1145,   376,   372,     0,   401,     0,     0,
       0,     0,   373,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   379,     0,   380,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   389,   390,     0,   391,   392,
     393,   394,   395,   396,   397,     0,     0,   398,   399,   400,
       0,   375,     0,     0,     0,   376,  1183,     0,   401,     0,
     372,     0,     0,     0,  1203,     0,     0,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,     0,     0,   398,   399,
     400,     0,     0,     0,     0,     0,   375,     0,     0,   401,
     376,   372,     0,     0,     0,     0,     0,     0,   373,  1204,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,   379,     0,
     380,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,   389,   390,     0,   391,   392,   393,   394,   395,   396,
     397,     0,     0,   398,   399,   400,     0,   375,     0,     0,
       0,   376,   372,     0,   401,     0,     0,     0,     0,   373,
    1205,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,   379,
       0,   380,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,   389,   390,     0,   391,   392,   393,   394,   395,
     396,   397,     0,     0,   398,   399,   400,     0,   375,     0,
       0,     0,   376,   372,     0,   401,     0,     0,     0,     0,
     373,  1206,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     379,     0,   380,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,   389,   390,     0,   391,   392,   393,   394,
     395,   396,   397,     0,     0,   398,   399,   400,     0,   375,
       0,     0,     0,   376,   372,     0,   401,     0,     0,     0,
       0,   373,  1207,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   379,     0,   380,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,   389,   390,     0,   391,   392,   393,
     394,   395,   396,   397,     0,     0,   398,   399,   400,     0,
     375,     0,     0,     0,   376,     0,     0,   401,     0,     0,
       0,     0,     0,  1208,     0,     0,     0,     0,   372,     0,
       0,     0,     0,     0,     0,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   377,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   379,     0,   380,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   389,   390,  1232,   391,   392,
     393,   394,   395,   396,   397,   372,     0,   398,   399,   400,
       0,     0,   373,     0,     0,     0,     0,     0,   401,     0,
       0,     0,     0,     0,   375,   374,     0,     0,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   372,     0,     0,     0,     0,
       0,     0,   373,     0,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,   379,     0,   380,   381,
     382,   375,   383,   384,   385,   376,   386,   387,   388,   389,
     390,     0,   391,   392,   393,   394,   395,   396,   397,     0,
       0,   398,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,     0,     0,   377,     0,     0,     0,
       0,   375,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   377,     0,   398,   399,
     400,     0,     0,     0,     0,     0,     0,  1261,     0,   401,
       0,     0,   378,   379,     0,   380,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,   389,   390,     0,   391,
     392,   393,   394,   395,   396,   397,   372,     0,   398,   399,
     400,     0,     0,   373,     0,     0,     0,     0,     0,   401,
     216,     0,     0,     0,    22,    23,   374,     0,     0,  1061,
       0,     0,     0,     0,   217,     0,    31,   218,     0,     0,
     216,     0,    37,     0,    22,    23,     0,     0,     0,    42,
       0,     0,     0,     0,   217,     0,    31,   218,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
    1062,     0,   804,  1063,     0,   221,   376,    67,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,     0,     0,
    1062,     0,     0,  1063,     0,   221,    83,    67,     0,    85,
       0,     0,    87,     0,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,   378,   379,     0,   380,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,   389,   390,     0,
     391,   392,   393,   394,   395,   396,   397,     0,     0,   398,
     399,   400,   102,     0,     0,     0,     0,   216,  1159,     0,
     401,    22,    23,     0,     0,     0,  1061,     0,     0,     0,
       0,   217,   102,    31,   218,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,  1062,     0,     0,
    1063,     0,   221,   216,    67,     0,     0,    22,    23,     0,
       0,     0,  1061,     0,     0,     0,     0,   217,     0,    31,
     218,     0,     0,    83,     0,    37,    85,     0,   374,    87,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,  1062,     0,     0,  1063,   374,   221,     0,
      67,     0,     0,     0,     0,     0,     0,   377,     0,   102,
       0,     0,     0,     0,   375,  1209,     0,     0,   376,    83,
       0,     0,    85,   378,   379,    87,   380,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,   389,     0,     0,
     391,   392,   393,   394,   395,     0,   397,     0,     0,   398,
     399,   400,     0,   375,     0,     0,     0,   376,     0,     0,
     401,     0,     0,     0,     0,   378,   379,     0,   380,   381,
     382,     0,   383,   384,   385,   102,   386,   387,   388,   389,
       0,  1210,   391,   392,   393,   394,   395,     0,   397,     0,
       0,   398,   399,   400,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,   378,   379,     0,   380,   381,   382,
       0,   383,   384,   385,     0,     0,   387,   388,   389,     0,
       0,   391,   392,   393,   394,     0,     0,   397,     0,   216,
     398,   399,   400,    22,    23,     0,     0,     0,     0,     0,
       0,   401,     0,   217,     0,    31,   218,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    59,     0,     0,     0,   219,
       0,   -51,   220,     0,   221,   216,    67,     0,     0,    22,
      23,     0,     0,     0,  1061,     0,     0,     0,     0,   217,
       0,    31,   218,     0,     0,    83,     0,    37,    85,     0,
       0,    87,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
     767,    59,   251,   353,     0,  1062,     0,     0,  1063,     0,
     221,     0,    67,     0,     0,     0,   252,   253,     0,   254,
       0,   102,     0,     0,   255,     0,     0,     0,     0,     0,
     768,    83,     0,   256,    85,     0,   251,    87,     0,   257,
       0,   916,     0,     0,     0,   258,     0,     0,     0,   259,
     252,   253,   260,   254,     0,     0,     0,     0,   255,     0,
       0,     0,   261,     0,     0,     0,     0,   256,   262,   263,
       0,     0,     0,   257,     0,     0,   264,     0,     0,   258,
       0,     0,     0,   259,     0,   265,   260,   102,     0,     0,
       0,     0,     0,     0,   266,   267,   261,   268,     0,   269,
       0,   270,   262,   263,   271,     0,     0,     0,   272,     0,
     264,   273,     0,     0,   274,     0,     0,     0,     0,   265,
       0,     0,   251,     0,     0,     0,     0,     0,   266,   267,
       0,   268,     0,   269,     0,   270,   252,   253,   271,   254,
       0,     0,   272,     0,   255,   273,     0,     0,   274,     0,
     768,     0,     0,   256,     0,     0,  -294,     0,     0,   257,
       0,     0,     0,     0,     0,   258,     0,     0,     0,   259,
    -294,  -294,   260,  -294,     0,     0,     0,     0,  -294,     0,
       0,     0,   261,     0,     0,     0,     0,  -294,   262,   263,
       0,     0,     0,  -294,     0,     0,   264,     0,     0,  -294,
       0,     0,     0,  -294,     0,   265,  -294,     0,     0,     0,
       0,     0,     0,     0,   266,   267,  -294,   268,     0,   269,
       0,   270,  -294,  -294,   271,     0,     0,     0,   272,     0,
    -294,   273,     0,     0,   274,     0,     0,     0,     0,  -294,
       0,     0,  -304,     0,     0,     0,     0,     0,  -294,  -294,
       0,  -294,     0,  -294,     0,  -294,  -304,  -304,  -294,  -304,
       0,     0,  -294,     0,  -304,  -294,     0,     0,  -294,     0,
       0,     0,     0,  -304,     0,     0,   251,     0,     0,  -304,
       0,     0,     0,     0,     0,  -304,     0,     0,     0,  -304,
     252,   253,  -304,   254,     0,     0,     0,     0,   255,     0,
       0,     0,  -304,     0,     0,     0,     0,   256,  -304,  -304,
       0,     0,     0,   257,     0,     0,  -304,     0,     0,   258,
       0,     0,     0,   259,     0,  -304,   260,     0,     0,     0,
       0,     0,     0,     0,  -304,  -304,   261,  -304,     0,  -304,
       0,  -304,   262,   263,  -304,     0,     0,     0,  -304,     0,
     264,  -304,     0,     0,  -304,     0,     0,     0,     0,   265,
       0,     0,  -295,     0,     0,     0,     0,     0,   266,   267,
       0,   268,     0,   269,     0,   270,  -295,  -295,   271,  -295,
       0,     0,   272,     0,  -295,   273,     0,     0,   274,     0,
       0,     0,     0,  -295,     0,     0,  -181,     0,     0,  -295,
       0,     0,     0,     0,     0,  -295,     0,     0,     0,  -295,
    -181,  -181,  -295,  -181,     0,     0,     0,     0,  -181,     0,
       0,     0,  -295,     0,     0,     0,     0,  -181,  -295,  -295,
       0,     0,     0,  -181,     0,     0,  -295,     0,     0,  -181,
       0,     0,     0,  -181,     0,  -295,  -181,     0,     0,     0,
       0,     0,     0,     0,  -295,  -295,  -181,  -295,     0,  -295,
       0,  -295,  -181,  -181,  -295,     0,     0,     0,  -295,     0,
    -181,  -295,     0,     0,  -295,     0,     0,     0,     0,  -181,
       0,     0,  -177,     0,     0,     0,     0,     0,  -181,  -181,
       0,  -181,     0,  -181,     0,  -181,  -177,  -177,  -181,  -177,
       0,     0,  -181,     0,  -177,  -181,     0,     0,  -181,     0,
       0,     0,     0,  -177,     0,     0,     0,     0,     0,  -177,
       0,     0,     0,     0,     0,  -177,     0,     0,     0,  -177,
       0,     0,  -177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -177,     0,     0,     0,     0,   216,  -177,  -177,
       0,    22,    23,     0,     0,     0,  -177,     0,     0,     0,
       0,   217,     0,    31,   218,  -177,     0,     0,     0,    37,
       0,     0,   -81,     0,  -177,  -177,    42,  -177,     0,  -177,
       0,  -177,     0,     0,  -177,     0,     0,     0,  -177,     0,
     -52,  -177,     0,     0,  -177,     0,     0,     0,     0,     0,
       0,    57,   216,    59,     0,     0,    22,    23,     0,   -52,
       0,     0,   221,     0,    67,     0,   217,     0,    31,   218,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,    42,     0,    83,     0,     0,    85,     0,   -81,    87,
       0,     0,     0,     0,     0,   -52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
       0,     0,     0,     0,   -52,     0,     0,   221,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87
};

static const yytype_int16 yycheck[] =
{
      12,    64,   477,   528,    16,   491,     2,   208,    20,    19,
     164,   660,   150,   778,    26,   762,   250,   495,   761,    31,
      32,   498,    34,    35,    36,    37,    38,   423,    68,    27,
      22,   232,   879,   971,   972,     1,   707,    49,    56,    51,
     768,    56,   914,    33,    56,    48,    58,    74,    60,    48,
     503,   239,   240,   241,    33,    33,    68,    69,    70,    71,
      72,    73,    33,   297,    27,    77,   106,    79,    48,    81,
      82,   209,     1,    33,    86,   528,  1026,    89,    90,  1222,
      92,   315,    94,   145,     1,    97,    98,    68,   100,   116,
     101,    91,   104,   105,   106,   107,   648,   145,    37,   651,
       3,  1116,     1,     3,   145,   521,     1,   164,    74,   117,
     164,   145,   174,   164,   171,    78,   173,   171,  1261,   170,
      20,  1023,   170,   104,   105,   106,    89,    48,   224,   976,
     164,   172,   803,    72,    34,    48,    48,   145,     3,   119,
     119,   790,   145,   170,    61,   108,   145,   633,   160,    48,
     116,   114,     3,   171,   250,   195,   171,   220,   169,   119,
     348,   349,    47,    63,   164,   145,    61,   145,   322,   169,
      34,    35,    36,   185,    91,   174,   188,   189,   190,   169,
    1027,   193,   194,   195,  1104,   419,   420,   104,   916,  1061,
     169,   169,  1142,  1065,   174,   124,    91,   207,   169,   102,
     117,   297,   102,  1105,   170,   876,   218,    61,   108,   169,
    1148,  1149,   224,   194,   195,   144,   145,   215,  1090,   315,
    1092,    32,   117,    69,   145,   124,  1241,   144,   113,   706,
     646,     0,   145,   145,    48,   298,   228,   102,   250,    27,
      91,   233,   106,   172,    91,   144,   145,   164,  1168,   144,
     242,   102,   169,   174,    47,    27,    48,  1104,   119,   105,
     119,   174,   174,   117,   660,   164,  1138,   145,   139,   169,
     822,   145,    65,   825,   169,    89,   147,    88,   764,    51,
      65,    92,  1239,   145,  1156,   297,  1158,   762,   384,   164,
      78,    84,   304,   305,   172,    61,  1169,    89,   172,    84,
    1257,    89,   164,   315,    76,   145,    78,  1054,    54,  1056,
    1053,  1076,  1055,   174,   452,   174,   992,    89,   994,   457,
     113,  1168,   169,   419,   420,    91,   114,    73,   139,   815,
     145,   343,   172,   144,   124,   146,   147,    83,   104,   145,
     151,    18,   114,    56,   340,   139,   358,   359,   159,  1222,
       3,   117,  1001,   147,   165,   166,   167,   172,   370,   149,
     372,   373,   374,   375,   376,   377,   172,   379,   380,   381,
     382,   383,    27,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     868,   145,    27,   870,   790,   491,   408,   409,   164,    48,
      77,   145,   414,   169,   145,   417,   418,   419,   420,   421,
     422,   423,    61,  1089,    61,  1091,   170,    82,    95,   459,
      85,   145,   434,    78,   145,   521,   164,   439,   172,   170,
     442,   906,   170,   145,    89,   112,   914,    27,    27,    61,
     145,   228,    91,    78,    91,   145,   233,   459,   172,   461,
     462,   172,   119,   108,    89,   467,   468,   104,    48,    48,
     172,   473,    52,    52,   476,   477,    89,   172,   117,    91,
     117,   483,   172,   108,   486,   124,   664,   665,   459,   491,
     668,   669,   104,    56,   145,    75,    75,   145,    78,    78,
      80,    80,    56,   505,   145,   117,   145,   509,   510,    89,
      89,    88,   514,   164,    56,    92,   164,   145,  1033,   521,
      88,   375,   376,   164,    92,   164,    56,   164,   108,   108,
     169,   145,   169,   172,   564,   537,   164,   145,   540,   145,
     542,   171,   733,   514,   546,   547,   548,   633,   550,   551,
     164,   139,   164,   171,   124,   173,   164,   169,   164,   147,
     646,   171,   564,   173,   742,   743,   537,   421,   422,   540,
     556,   542,   408,   409,    56,   145,   547,   171,   414,   149,
     434,   417,   418,  1026,     2,   171,   171,   173,   173,  1054,
    1033,  1056,   172,   564,    12,   124,   169,   171,   171,   173,
      27,   540,   172,   542,  1072,    81,    82,    25,   462,    32,
      28,   171,    56,   173,   468,  1001,   145,    56,    56,   473,
     149,    48,  1090,    32,  1092,    52,   628,   629,   630,   631,
     632,   633,   634,   635,   778,    56,     8,   637,   638,   171,
      56,    99,   644,   172,   646,    56,   171,   169,    75,    56,
      56,    78,   654,    80,   656,    56,   139,    56,   660,    77,
     662,   144,    89,    56,   147,    88,   164,     3,   151,    92,
     756,   757,    84,   644,   163,   677,   678,   164,   764,    88,
     164,   108,   712,    92,    20,    49,   104,   155,  1156,   164,
    1158,   109,   546,  1174,   164,   164,    56,   164,    34,  1142,
      48,   164,   164,   118,   540,   171,   542,    71,   710,   711,
     712,    87,   169,    61,   716,   171,   139,    81,    82,    87,
     171,   144,   171,   146,   147,   171,  1169,    63,   151,   815,
     139,   171,   164,   124,   171,   144,   159,     8,   147,   164,
    1183,   712,   151,    91,   167,  1226,  1227,  1228,  1229,  1230,
    1231,    65,   754,   147,   124,   144,   758,    68,   760,   761,
     762,   169,   764,   181,   164,   124,   102,   171,   145,   117,
     172,    48,   108,   171,   164,   629,   124,   631,   124,  1222,
     150,   124,   152,   153,    35,    35,   156,   157,   790,   172,
     145,   172,   124,   172,   172,    21,  1239,   145,   145,   174,
     174,   172,   804,   805,   806,   171,   169,   809,   644,   174,
    1034,  1035,     2,   815,  1257,   164,   164,   171,  1261,   164,
     164,   169,    12,   164,   172,   164,   164,   164,   145,   831,
     164,   833,    35,   164,    35,    25,   838,    65,    28,   841,
     145,   145,   206,   172,   172,   170,   848,    89,   850,   172,
     170,   124,    33,   172,    33,   857,   174,   169,   888,   169,
     278,    35,   164,   144,    89,   174,   284,   174,   171,   144,
     171,   164,   236,   237,   238,   239,   240,   241,   174,   164,
     716,   170,   147,   247,   248,   171,   888,    77,   147,   147,
     139,    81,    82,   147,   147,   147,   898,   169,   900,    48,
     139,   903,   147,   147,   906,   139,   147,   147,   139,   147,
     139,   147,    61,   277,   104,    35,   918,   888,   164,   109,
     104,   169,   340,   287,   139,   172,   139,   145,   163,   293,
     489,   164,  1076,   355,   164,   172,   754,   355,    24,   163,
     172,   164,    91,   164,   164,   172,  1065,   311,  1034,  1035,
     804,   805,   174,   172,    89,   863,   323,   892,  1076,   919,
    1072,   698,   868,   922,   966,   329,   330,  1241,   117,  1199,
    1232,  1225,    37,   506,    31,   124,    -1,    -1,   980,   981,
      -1,   983,   984,    -1,   986,   987,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,   996,   997,   145,   999,    -1,  1001,
      -1,  1003,  1004,    -1,    -1,  1007,    -1,    -1,    -1,    -1,
    1063,    -1,    -1,    -1,   432,   164,    -1,    -1,    -1,    -1,
     169,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
    1032,  1033,  1034,  1035,    -1,    -1,    -1,    -1,  1040,    -1,
    1042,    -1,    -1,   461,  1046,    -1,    -1,    -1,    -1,   467,
    1052,  1053,  1054,  1055,  1056,  1026,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   481,    -1,   483,    -1,    -1,    -1,    -1,
      -1,   435,    -1,    -1,    -1,    -1,  1106,    -1,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1087,    -1,    -1,   278,    -1,
      -1,    -1,    -1,     2,   284,    -1,    -1,    -1,  1100,    -1,
      -1,    -1,   466,    12,  1106,    -1,   470,    -1,   472,  1111,
      -1,   475,    -1,  1115,    -1,    -1,    25,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,   544,    -1,    -1,    -1,
     548,    -1,    -1,    -1,    -1,  1106,    -1,    -1,   556,    -1,
    1111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     340,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1160,  1161,
    1162,  1163,    -1,    -1,  1194,   355,    -1,    -1,    77,    -1,
      -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1183,    -1,    12,    -1,  1187,  1188,    -1,    -1,    -1,
      -1,    -1,  1194,    -1,    -1,   104,    25,    -1,  1169,    28,
     109,  1203,  1204,  1205,  1206,  1207,  1208,   625,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1221,
    1222,    -1,  1224,  1194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   432,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,  1222,    22,    -1,    -1,  1257,    26,    27,    -1,  1261,
      -1,    31,   680,    -1,    -1,   683,    36,    -1,    38,    39,
      -1,   461,   181,    -1,    44,   104,    -1,   467,    -1,    -1,
     109,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
    1261,   481,    -1,   483,   658,   659,    -1,    -1,    -1,    -1,
     664,   665,    -1,    73,   668,   669,    76,    -1,    78,    -1,
      80,    -1,    82,    83,    -1,    85,    -1,    87,    -1,    89,
     684,    -1,    -1,   741,    -1,    -1,    -1,    -1,    -1,   747,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,   544,    -1,    -1,    -1,   548,    -1,
     778,    -1,    -1,    -1,    -1,   729,   556,    -1,    -1,   278,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   804,   805,    -1,    -1,
      -1,     8,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,   831,    -1,   833,    -1,    -1,    -1,    36,
     838,    38,    39,   841,    -1,    -1,    -1,    44,    -1,    -1,
      -1,   340,    -1,    -1,    51,    -1,   854,    -1,   856,    -1,
      -1,   859,    -1,    -1,    -1,    -1,   355,    -1,    65,   278,
      -1,    -1,   816,   817,    -1,   284,    -1,    -1,    -1,    76,
      -1,    78,    -1,    -1,   828,    82,   830,    84,    85,   887,
      87,    -1,    89,   837,    -1,    -1,   840,    -1,    -1,   843,
     898,   845,   900,   847,    -1,    -1,    -1,    -1,    -1,    -1,
     680,   108,    -1,   683,   111,    -1,    -1,   114,    -1,     3,
     864,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   340,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,   432,    -1,    -1,   355,    -1,    -1,    -1,
      34,   895,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,   164,    -1,    53,
      -1,   741,   461,    -1,     2,    -1,    -1,   747,   467,    63,
      -1,    -1,    -1,    -1,    12,    69,    70,    -1,    -1,    -1,
      -1,    -1,   481,    77,   483,    -1,    -1,    25,   996,    -1,
      28,    -1,    86,    -1,    -1,  1003,    -1,    -1,    -1,    -1,
      -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,    -1,
    1018,   105,     2,   432,    -1,   109,   110,    -1,   112,    -1,
      -1,   115,    12,    -1,    -1,    -1,    -1,    -1,  1036,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,  1045,    28,    77,
      -1,  1049,   461,    -1,    -1,   544,    -1,    -1,   467,   548,
      -1,   831,    -1,   833,    -1,    -1,    -1,   556,   838,    -1,
      -1,   841,   481,    -1,   483,    -1,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,   854,    -1,   856,   171,    -1,   859,
      -1,    -1,    -1,    -1,  1038,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,  1101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   887,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,   898,   109,
     900,    -1,    -1,    -1,    -1,   544,    -1,    -1,    -1,   548,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   556,    -1,    -1,
    1094,  1095,    -1,   181,  1098,  1099,    -1,    -1,    -1,    -1,
      -1,    -1,  1160,  1161,  1162,  1163,    -1,    -1,    -1,    -1,
      -1,    -1,  1116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1124,    -1,  1126,    -1,  1128,    -1,    -1,    -1,    -1,  1187,
    1188,   680,    -1,     3,   683,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,   996,    -1,     2,    -1,
      40,    -1,    -1,  1003,    -1,    -1,    46,    -1,    12,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,  1018,    -1,
     278,    25,   741,    63,    28,    -1,   284,    -1,   747,    69,
      70,    -1,    -1,    -1,    -1,    -1,  1036,    77,    -1,    -1,
      -1,   680,    -1,    -1,   683,  1045,    86,    -1,    -1,  1049,
      -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,  1241,   278,   109,
      -1,    -1,   112,    77,   284,   115,    -1,    -1,    -1,    -1,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,    -1,    -1,
     104,  1101,   741,    -1,    -1,   109,    -1,    -1,   747,    -1,
      -1,    -1,   831,    -1,   833,    -1,    -1,    -1,    -1,   838,
      -1,    -1,   841,    -1,    -1,    -1,    -1,    -1,    -1,   169,
     340,   171,    -1,    -1,    -1,   854,    -1,   856,    -1,    -1,
     859,    -1,    -1,    -1,    -1,   355,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1160,  1161,  1162,  1163,    -1,    -1,    -1,    -1,   887,    -1,
      -1,    -1,    -1,    -1,   432,    -1,    -1,   181,    -1,   898,
      -1,   900,    -1,    -1,    -1,    -1,    -1,  1187,  1188,    32,
      -1,    -1,   831,    -1,   833,    -1,    -1,    -1,    -1,   838,
      -1,    -1,   841,   461,     2,    -1,    -1,    -1,    -1,   467,
      -1,    -1,    -1,    -1,    12,   854,    -1,   856,    -1,    -1,
     859,    -1,   432,   481,    -1,   483,    -1,    25,    -1,    -1,
      28,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,   887,    92,
      -1,   461,    -1,    -1,    -1,    -1,    -1,   467,    -1,   898,
      -1,   900,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   481,    -1,   483,   278,    -1,    -1,   996,    -1,    77,
     284,    -1,    -1,    -1,  1003,    -1,   544,    -1,    -1,    88,
     548,    -1,    -1,    92,    -1,    -1,   139,   140,   556,  1018,
     143,   144,    -1,   146,   147,    32,   104,    -1,   151,    -1,
      -1,   109,    -1,    -1,    -1,    -1,   159,  1036,    -1,    -1,
      -1,    -1,   165,   166,   167,    -1,  1045,    -1,    -1,    -1,
    1049,    -1,    -1,    -1,   544,    -1,   340,    -1,   548,    -1,
     139,   140,    -1,   142,   143,   144,   556,   146,   147,   148,
      -1,   355,   151,    -1,    -1,    -1,    -1,   996,    -1,   158,
     159,    88,    -1,   162,  1003,    92,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,  1018,
      -1,    -1,  1101,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1036,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1045,    -1,    -1,    -1,
    1049,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,    -1,   680,    -1,   151,   683,    -1,     2,   432,    -1,
      -1,   158,   159,    -1,    -1,   162,    -1,    12,   165,   166,
     167,  1160,  1161,  1162,  1163,    -1,    -1,    -1,    -1,    -1,
      25,    32,    -1,    28,    -1,    -1,    -1,   461,    -1,    -1,
      -1,    -1,  1101,   467,    -1,    -1,    -1,    -1,  1187,  1188,
     680,    -1,    -1,   683,    -1,    -1,    -1,   481,    -1,   483,
     278,    -1,    -1,   741,    -1,    -1,   284,    -1,    -1,   747,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1160,  1161,  1162,  1163,    -1,    -1,    -1,    -1,   104,
      -1,   741,    -1,    -1,   109,    -1,    -1,   747,    -1,    -1,
     544,    -1,   340,    -1,   548,    -1,    -1,    -1,  1187,  1188,
      -1,    -1,   556,    -1,    -1,    -1,    -1,   355,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,    -1,    -1,    -1,
     151,    -1,    -1,   831,    -1,   833,    -1,    -1,   159,    -1,
     838,    -1,    -1,   841,   165,   166,   167,     2,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   854,    12,   856,    -1,
      32,   859,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      25,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   831,    -1,   833,    -1,    -1,    -1,    -1,   838,   887,
      -1,   841,    -1,    -1,   432,    -1,    -1,    -1,    -1,    -1,
     898,    -1,   900,    -1,   854,    -1,   856,    -1,    -1,   859,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,    -1,    77,   461,    -1,    -1,    -1,    -1,    -1,   467,
      -1,    -1,    -1,    -1,    -1,    -1,   680,   887,    -1,   683,
      -1,    -1,    -1,   481,    -1,   483,    -1,    -1,   898,   104,
     900,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   278,    -1,    -1,    -1,   139,   140,   284,
      -1,     8,   144,    -1,   146,   147,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   159,    -1,    26,
      27,    -1,    -1,   165,   166,   167,    -1,   741,   996,    36,
      -1,    38,    39,   747,    -1,  1003,   544,    44,    71,    -1,
     548,    -1,    -1,    -1,    51,    -1,    -1,    -1,   556,    -1,
    1018,    -1,    -1,    -1,    -1,   340,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1036,    76,
     355,    78,    -1,    -1,    -1,    82,   996,  1045,    85,    -1,
      87,  1049,    89,  1003,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1018,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1036,   831,    -1,   833,
      -1,    -1,    -1,    -1,   838,  1045,    -1,   841,    -1,  1049,
      -1,    -1,    -1,  1101,    -1,    -1,    -1,    -1,    -1,    -1,
     854,    -1,   856,    -1,    -1,   859,    -1,   432,    -1,    -1,
      -1,    -1,    -1,   278,    -1,    -1,    -1,   164,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   680,   887,    -1,   683,   461,    -1,    -1,    -1,
      -1,  1101,   467,    -1,   898,    -1,   900,    -1,    -1,    -1,
      -1,    -1,  1160,  1161,  1162,  1163,   481,    -1,   483,    -1,
      -1,    -1,    -1,   236,   237,   238,   239,   240,   241,    -1,
      -1,    -1,    -1,    -1,    -1,   340,    -1,    -1,    -1,  1187,
    1188,    -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,
     355,    -1,    -1,   741,    -1,    -1,    -1,    -1,    -1,   747,
    1160,  1161,  1162,  1163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   287,    -1,    -1,    -1,    -1,   544,
     293,    -1,    -1,   548,    -1,    -1,    83,  1187,  1188,    -1,
      -1,   556,    -1,    -1,    -1,    -1,    -1,     3,   311,    -1,
      -1,    -1,   996,    -1,    -1,    -1,    -1,    -1,    -1,  1003,
      -1,    17,    -1,    -1,    20,    -1,   329,   330,    -1,    25,
      -1,    -1,    -1,    -1,  1018,    -1,    -1,   432,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,  1036,   831,    -1,   833,   143,    53,   145,    -1,
     838,  1045,    -1,   841,    -1,  1049,   461,    63,    -1,    -1,
      -1,    -1,   467,    -1,    70,    -1,   854,    -1,   856,    22,
      -1,   859,    -1,    26,    27,    -1,   481,    -1,    31,    -1,
      86,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    98,    -1,   191,    -1,   102,    -1,    51,   887,
      -1,    -1,    -1,   109,    -1,    -1,    -1,  1101,    -1,   115,
     898,   208,   900,    -1,    -1,   680,    -1,    -1,   683,    -1,
      -1,    -1,   435,    76,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    85,    -1,    87,   232,    89,   234,    -1,   544,
      -1,    -1,    -1,   548,    -1,    -1,    -1,    -1,   245,   246,
      -1,   556,    -1,   466,    -1,   108,    -1,   470,   111,   472,
      -1,   114,   475,    -1,    -1,    -1,  1160,  1161,  1162,  1163,
      -1,    -1,    -1,    -1,    -1,    -1,   741,    -1,    -1,    -1,
      -1,    -1,   747,   280,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1187,  1188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   996,    -1,
      -1,   164,    -1,    -1,    -1,  1003,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
    1018,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,  1036,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,  1045,    -1,    -1,
      -1,  1049,    -1,   360,    -1,    -1,   831,    -1,   833,    -1,
      -1,    -1,    -1,   838,    -1,   680,   841,    76,   683,    78,
      -1,    80,    -1,    82,    -1,    -1,    85,    -1,    87,   854,
      89,   856,    -1,    -1,   859,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,   111,  1101,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,   887,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   898,   431,   900,   741,    -1,    -1,    -1,
      -1,    -1,   747,    -1,    -1,   658,   659,    -1,    -1,    -1,
      -1,   664,   665,   450,    -1,   668,   669,   454,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,
      -1,   170,  1160,  1161,  1162,  1163,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,   492,    -1,    -1,    44,  1187,
    1188,   498,    -1,    -1,    -1,    51,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,   831,    -1,   833,    -1,
      76,   996,    78,   838,    -1,    -1,   841,    -1,  1003,   155,
      -1,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,   854,
      -1,   856,   168,  1018,   859,   171,    -1,   173,    -1,     3,
     557,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,   566,
      -1,  1036,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
    1045,    25,   887,    -1,  1049,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,   898,    -1,   900,    40,    -1,    -1,    -1,
      -1,    -1,    46,   816,   817,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,   828,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    69,    70,   840,    -1,    -1,
     843,    -1,   845,    77,   847,    -1,  1101,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     647,    95,    96,   650,    98,    -1,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1160,  1161,  1162,  1163,    -1,
      -1,    -1,    -1,    -1,    -1,   702,    -1,    -1,    -1,   706,
      -1,    -1,    -1,  1018,    -1,    -1,   168,    -1,    -1,   171,
      -1,   173,  1187,  1188,    -1,    -1,    -1,   171,    -1,    -1,
      -1,  1036,   729,    -1,    -1,    -1,   733,    -1,    -1,    -1,
    1045,    -1,    -1,     1,  1049,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,   768,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,   778,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,  1101,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,  1038,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,   861,   123,   863,    -1,    -1,    -1,
     867,    -1,    -1,   870,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,   892,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,   174,    -1,   176,   916,
       0,     1,   919,     3,    -1,     5,     6,     7,     8,     9,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,  1076,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,   169,   170,   171,    -1,   173,    -1,     1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   173,    -1,     1,   176,     3,     4,     5,     6,     7,
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
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    95,
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
     102,    -1,    -1,   105,   106,   107,    -1,   109,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,   149,    -1,    -1,
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
      98,    99,   100,    -1,   102,    -1,    -1,   105,   106,   107,
      -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,   172,   173,    -1,    -1,   176,     3,
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
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    48,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
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
     106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
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
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,   106,   107,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,   109,    -1,    -1,
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
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
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
      -1,   169,   170,   171,    -1,   173,    -1,    -1,   176,     3,
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
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
     174,    -1,   176,     3,     4,     5,     6,     7,     8,     9,
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
      -1,   171,    -1,   173,   174,     3,   176,     5,     6,     7,
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
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
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
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,   109,
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
      -1,   169,    -1,   171,    -1,   173,   174,     3,   176,     5,
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
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
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
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
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
      59,    60,    61,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,   104,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,   163,   164,    25,    -1,    -1,    -1,
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
     139,    -1,   141,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,   163,    -1,    25,    -1,    -1,    -1,
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
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    86,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,
      -1,    -1,   105,    -1,     3,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    12,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,   171,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    95,    96,    -1,    98,
      -1,   100,    19,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    32,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,    -1,   146,    -1,    -1,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,   162,    -1,    -1,   165,   166,   167,   168,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    12,    -1,
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
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,    -1,   137,   138,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,    -1,
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
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    48,   132,
     133,   134,   135,    -1,   137,   138,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,   119,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    37,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      72,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,
      92,   176,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    72,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,   165,   166,   167,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    32,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    88,    -1,    -1,    -1,
      92,    12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,   123,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    -1,    12,   176,    -1,    -1,    -1,   139,   140,
      19,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,    32,    33,   156,   157,   158,   159,   160,
     161,   162,   123,    -1,   165,   166,   167,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    88,
      -1,    -1,    -1,    92,    12,   176,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    88,    -1,    -1,    -1,    92,    -1,    12,   176,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      12,    13,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
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
      88,    -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,    -1,    -1,    -1,    -1,
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
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    13,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,   174,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,
      92,    12,   176,    -1,    -1,    -1,    -1,    -1,    19,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    13,   176,    -1,    -1,    -1,    -1,    19,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,   174,    -1,   176,    -1,
      12,    -1,    -1,    -1,   101,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,   176,
      92,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   101,
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
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    61,   156,   157,
     158,   159,   160,   161,   162,    12,    -1,   165,   166,   167,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    88,    32,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    88,   146,   147,   148,    92,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,   176,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    12,    -1,   165,   166,
     167,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   176,
      22,    -1,    -1,    -1,    26,    27,    32,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      22,    -1,    44,    -1,    26,    27,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      82,    -1,    88,    85,    -1,    87,    92,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,
      82,    -1,    -1,    85,    -1,    87,   108,    89,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,   164,    -1,    -1,    -1,    -1,    22,   170,    -1,
     176,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,   164,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      85,    -1,    87,    22,    89,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    -1,   108,    -1,    44,   111,    -1,    32,   114,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    85,    32,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   164,
      -1,    -1,    -1,    -1,    88,   170,    -1,    -1,    92,   108,
      -1,    -1,   111,   139,   140,   114,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,    -1,    -1,
     156,   157,   158,   159,   160,    -1,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
     176,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,   164,   150,   151,   152,   153,
      -1,   170,   156,   157,   158,   159,   160,    -1,   162,    -1,
      -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,    -1,   151,   152,   153,    -1,
      -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,    22,
     165,   166,   167,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,    82,
      -1,    84,    85,    -1,    87,    22,    89,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,   108,    -1,    44,   111,    -1,
      -1,   114,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
       1,    78,     3,    80,    -1,    82,    -1,    -1,    85,    -1,
      87,    -1,    89,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,   164,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,   108,    -1,    34,   111,    -1,     3,   114,    -1,    40,
      -1,     8,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,    70,
      -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    86,    53,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,   100,
      -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,    -1,
      77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    86,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    17,    18,   105,    20,
      -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,    -1,
      31,    -1,    -1,    34,    -1,    -1,     3,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,    70,
      -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    86,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,   100,
      -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,    -1,
      77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    86,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    17,    18,   105,    20,
      -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    34,    -1,    -1,     3,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,    70,
      -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    86,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,   100,
      -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,    -1,
      77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    86,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    17,    18,   105,    20,
      -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    34,    -1,    -1,     3,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,    70,
      -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    86,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,   100,
      -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,    -1,
      77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    86,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    17,    18,   105,    20,
      -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    22,    69,    70,
      -1,    26,    27,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    86,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    95,    96,    51,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      65,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    22,    78,    -1,    -1,    26,    27,    -1,    84,
      -1,    -1,    87,    -1,    89,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,   108,    -1,    -1,   111,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114
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
     227,   228,   230,   232,   235,   236,   237,   238,   239,   240,
     244,   245,   248,   249,   252,   253,   257,   260,   261,   287,
     290,   291,   311,   312,   313,   314,   315,   316,   317,   325,
     326,   327,   328,   329,   332,   333,   334,   335,   336,   337,
     338,   339,   341,   342,   343,   344,   345,   164,   185,   329,
     119,   318,   319,     3,   208,    14,    22,    36,    41,    42,
      45,    56,    87,   100,   169,   173,   235,   257,   311,   316,
     327,   328,   329,   332,   334,   335,   318,   329,   108,   290,
      89,   208,   185,   305,   329,     8,    22,    36,    39,    82,
      85,    87,   187,   185,   171,     8,    87,   329,   330,     8,
      11,    87,   108,   330,    78,   121,   229,   329,   229,   329,
     229,   329,    26,   291,   329,    27,   114,   231,   329,   194,
     171,     3,    17,    18,    20,    25,    34,    40,    46,    50,
      53,    63,    69,    70,    77,    86,    95,    96,    98,   100,
     102,   105,   109,   112,   115,   210,   212,   210,   210,   258,
     171,   210,   292,   293,    33,   195,   215,   329,   329,    18,
      77,    95,   112,   329,   329,   329,     8,   171,   219,   220,
       4,   281,   304,   329,   106,   107,   164,   329,   331,   329,
     215,   329,   329,   329,    99,   171,   185,   329,   329,   195,
     207,   329,   332,   195,   207,   329,   210,   288,   329,   231,
     329,   329,   329,   329,   329,   329,   329,     1,   170,   183,
     196,   304,   110,   149,   281,   306,   307,   331,   229,   304,
     329,   340,   329,    80,   185,   169,    84,   191,    47,   113,
      56,   210,   210,    54,    73,    83,   276,   292,   163,   164,
     155,   329,    12,    19,    32,    88,    92,   123,   139,   140,
     142,   143,   144,   146,   147,   148,   150,   151,   152,   153,
     154,   156,   157,   158,   159,   160,   161,   162,   165,   166,
     167,   176,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   168,   267,   171,
     173,    88,    92,   171,   185,   164,   329,   329,   329,   210,
     304,    56,   169,   195,    48,   318,   288,   292,   164,   145,
     164,   187,   118,   211,   281,   308,   309,   310,   331,    87,
     225,   261,   290,    87,   223,   288,   261,   290,   210,   171,
     215,    33,    48,   215,   119,   215,   321,    33,    48,   215,
     321,   215,   321,    48,   215,   321,    37,    72,   164,   210,
     210,   101,   195,   101,   124,   195,   267,    82,    85,   192,
     308,   171,   171,   195,   185,   171,   269,   110,   171,   210,
     294,   295,     1,   144,   299,    48,   145,   185,   215,   171,
     171,   215,   308,   220,   145,   164,   329,   329,   164,   169,
     215,   171,   308,   164,   241,   164,   241,   164,   124,   289,
     164,   215,   215,   164,   170,   170,   183,   145,   170,   329,
     145,   172,   145,   172,   174,   321,    48,   145,   174,   321,
     122,   145,   174,     8,     1,   170,   196,    65,   204,   205,
     329,   198,   329,   210,   144,   250,   169,   262,   164,   329,
     329,   329,   329,   229,   329,   229,   329,   329,   329,   329,
     329,   329,   329,     3,    20,    34,    63,   102,   108,   211,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,    68,   331,   331,
     331,   331,   331,   308,   308,   229,   329,   229,   329,    27,
      48,    89,   114,   320,   323,   324,   329,   345,    33,    48,
      33,    48,   101,   171,    48,   174,   210,   229,   329,   215,
     164,   164,   329,   329,   124,   172,   145,   226,   210,   292,
     224,   210,   164,   292,    48,   304,    45,   329,   229,   329,
     171,   215,    45,   329,   229,   329,   215,   215,   229,   329,
     215,   167,   197,   200,   329,   197,   199,   124,   124,   185,
      35,   185,   329,    35,   329,   191,   172,   309,   210,   233,
     234,    27,    48,    52,    75,    78,    89,   108,   184,   270,
     271,   272,   273,   274,   259,   295,   145,   172,    34,    50,
      96,   100,   173,   214,   300,   312,   124,   296,   329,   293,
     329,   329,   172,   281,   329,     1,   246,   310,   172,    21,
     242,   300,   312,   145,   170,   172,   172,   306,   172,   306,
     185,   174,   229,   329,   174,   185,   329,   174,   329,   174,
     329,   170,   170,   210,   145,   164,    13,   147,   145,   164,
      13,    37,    72,   164,   171,   304,   169,     1,    31,   210,
     254,   255,   256,    27,    78,    89,   108,   264,   275,   171,
     164,   164,   164,   164,   164,   164,   172,   174,    48,    89,
     145,   172,    17,    20,    25,    46,    53,    63,    70,    86,
      98,   109,   115,   311,    88,    88,    45,   229,   329,    45,
     229,   329,   309,   229,   329,   171,   318,   318,   164,   281,
     331,   310,   210,   250,   164,   210,   250,   164,   329,   172,
     329,    33,   215,    33,   215,   322,   323,   329,    33,   215,
     321,    33,   215,   321,   215,   321,   215,   321,   145,   164,
      13,   164,   329,   329,    35,   185,    35,    35,   185,   101,
     195,    65,   172,   145,   172,    48,    89,   273,   145,   172,
     171,   210,    27,    78,    89,   108,   277,   172,   295,   299,
       1,   304,    68,   331,   172,   172,   170,    74,   116,   170,
     247,   172,   171,   195,   210,   243,   288,   185,   174,   321,
     174,   321,   185,   122,   204,   211,   169,   329,   110,   329,
     200,   199,   309,     1,   251,   170,     8,   256,   124,   145,
     170,    89,   263,     1,     3,    12,    17,    19,    20,    25,
      40,    46,    53,    55,    63,    69,    70,    86,    98,   102,
     105,   109,   115,   139,   140,   141,   142,   143,   144,   146,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   162,   165,   166,   167,   168,   171,   209,   210,   212,
     265,   266,   267,   311,   172,   323,   299,   311,   311,   329,
      33,    33,   329,    33,    33,   172,   174,   174,   309,   215,
     215,   250,   169,   250,   169,   215,   101,    45,   329,    45,
     329,   145,   172,   101,    45,   329,   215,    45,   329,   215,
     215,   215,   329,   329,   185,   185,   329,   185,    35,   210,
     164,   234,   195,   210,   272,   295,   144,   303,    89,   299,
     296,   174,    48,   174,   171,   171,    33,   185,   304,   243,
     144,   195,    45,   185,   329,   174,    45,   185,   329,   174,
     329,   197,    13,    37,    72,    37,    72,   164,   164,   172,
     170,    31,    82,    85,   170,   184,   217,   218,   256,   329,
     255,   277,   171,   268,   329,   139,   147,   268,   268,   296,
     101,    45,    45,   101,    45,    45,    45,    45,   172,   169,
     251,   169,   251,   329,   329,   329,   323,   329,   329,   329,
      13,    35,   185,   164,   303,   172,   173,   214,   281,   302,
     312,   149,   282,   296,    61,   117,   283,   329,   300,   312,
     308,   308,   185,   215,   172,   329,   329,   185,   329,   185,
     170,   110,   329,   200,   199,   200,   199,   164,     8,   218,
     217,     1,   144,   298,   271,   172,     3,   102,   266,   267,
     329,   329,   329,   329,   329,   329,   251,   170,   251,   170,
     101,   101,   101,   101,   329,   185,   282,   296,   303,   174,
     304,   281,   329,     3,    91,   102,   284,   285,   286,   329,
     195,   216,   280,   174,   172,   172,   195,   101,   101,   164,
     164,   164,   164,   218,   173,   214,   297,   312,   104,   278,
     172,   268,   268,   101,   101,   101,   101,   101,   101,   170,
     170,   329,   329,   329,   329,   282,   296,   281,   301,   302,
     312,    48,   174,   286,   117,   145,   124,   150,   152,   153,
     156,   157,    61,   312,   163,   163,   329,   329,     1,   174,
     304,   283,   329,   301,   302,   329,   285,   286,   286,   286,
     286,   286,   286,   284,   174,   297,   312,   174,   164,   279,
     280,   174,   297,   312,   301
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
     222,   225,   222,   226,   222,   222,   222,   222,   222,   227,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   229,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   231,   231,
     232,   232,   233,   233,   234,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   236,   236,
     236,   236,   236,   236,   237,   237,   237,   238,   238,   238,
     239,   240,   240,   240,   240,   240,   240,   241,   241,   242,
     242,   242,   243,   243,   244,   245,   245,   246,   246,   247,
     247,   247,   248,   248,   249,   249,   249,   250,   250,   251,
     251,   251,   252,   252,   253,   254,   254,   254,   255,   255,
     255,   256,   256,   258,   259,   257,   260,   260,   260,   262,
     263,   261,   264,   264,   264,   264,   264,   265,   265,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   268,
     268,   269,   270,   270,   271,   271,   272,   272,   272,   272,
     272,   272,   273,   273,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   275,   275,   275,   275,   275,   275,   276,
     276,   276,   277,   277,   277,   277,   277,   277,   278,   278,
     279,   279,   280,   280,   281,   282,   282,   282,   283,   283,
     283,   283,   283,   284,   284,   285,   285,   285,   285,   285,
     285,   285,   286,   286,   287,   287,   287,   288,   288,   289,
     289,   289,   290,   290,   290,   290,   290,   291,   291,   292,
     292,   293,   293,   294,   294,   294,   295,   295,   295,   296,
     296,   296,   297,   297,   297,   297,   297,   297,   297,   298,
     298,   298,   298,   298,   299,   299,   299,   299,   299,   300,
     300,   300,   300,   301,   301,   301,   302,   302,   302,   302,
     302,   303,   303,   303,   303,   303,   304,   304,   304,   304,
     305,   305,   306,   306,   306,   307,   307,   308,   308,   309,
     309,   310,   310,   310,   310,   311,   311,   312,   312,   312,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   315,
     316,   317,   317,   317,   317,   317,   317,   317,   317,   318,
     318,   319,   320,   320,   321,   322,   322,   323,   323,   323,
     324,   324,   324,   324,   324,   324,   325,   325,   326,   326,
     326,   326,   326,   327,   327,   327,   327,   327,   328,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   330,   330,   331,   331,
     331,   332,   332,   332,   332,   333,   333,   333,   333,   333,
     334,   334,   334,   335,   335,   335,   335,   335,   336,   336,
     336,   336,   337,   337,   338,   338,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   340,
     340,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   342,   342,   342,   342,   342,   342,
     342,   343,   343,   343,   343,   344,   344,   344,   344,   345,
     345,   345,   345,   345,   345,   345
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
       9,     0,     8,     0,     9,     3,     3,     5,     5,     2,
       5,     3,     3,     6,     6,     4,     5,     5,     3,     3,
       6,     5,     6,     5,     6,     3,     4,     3,     4,     5,
       6,     5,     6,     3,     4,     3,     4,     6,     7,     6,
       7,     4,     5,     4,     5,     4,     4,     3,     6,     5,
       4,     3,     6,     5,     6,     5,     8,     7,     4,     4,
       6,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     4,     7,     5,     3,     6,
       2,     3,     3,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     3,     6,     6,     1,     1,     1,     0,     2,     0,
       2,     3,     5,     5,     1,     1,     2,     3,     1,     3,
       2,     1,     3,     0,     0,     8,     0,     1,     1,     0,
       0,    10,     3,     3,     5,     5,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     3,     1,     3,     0,     1,     4,     5,     4,     5,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     4,     4,     2,     4,     0,
       2,     2,     1,     1,     1,     2,     1,     4,     3,     1,
       3,     3,     5,     1,     1,     3,     1,     2,     3,     0,
       2,     2,     3,     2,     4,     3,     3,     4,     3,     0,
       2,     2,     2,     1,     0,     2,     2,     2,     1,     4,
       4,     6,     3,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     1,     2,     1,     2,     1,     2,     1,     2,
       1,     1,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     4,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     3,     1,     2,
       2,     2,     2,     2,     8,     8,     9,     9,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     3,     2,     2,     2,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     5,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     3,     4,     3,     4,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
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
    case 2:
      yypstate_clear (yyps);
      goto case_0;

    case_0:
    case 0:
      yyn = yypact[yystate];
      goto yyread_pushed_token;
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
#line 6643 "bison-chapel.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 522 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6649 "bison-chapel.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 523 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6655 "bison-chapel.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 530 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6661 "bison-chapel.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 535 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6667 "bison-chapel.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 536 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6673 "bison-chapel.cpp"
    break;

  case 26: /* stmt: stmt_level_expr TSEMI  */
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6679 "bison-chapel.cpp"
    break;

  case 27: /* stmt: TATOMIC stmt  */
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6685 "bison-chapel.cpp"
    break;

  case 28: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6691 "bison-chapel.cpp"
    break;

  case 29: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6697 "bison-chapel.cpp"
    break;

  case 30: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 562 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6703 "bison-chapel.cpp"
    break;

  case 31: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 563 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6709 "bison-chapel.cpp"
    break;

  case 32: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 564 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6715 "bison-chapel.cpp"
    break;

  case 33: /* stmt: TLABEL ident_def stmt  */
#line 565 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6721 "bison-chapel.cpp"
    break;

  case 34: /* stmt: TLOCAL expr do_stmt  */
#line 566 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6727 "bison-chapel.cpp"
    break;

  case 35: /* stmt: TLOCAL do_stmt  */
#line 567 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6733 "bison-chapel.cpp"
    break;

  case 36: /* stmt: TON expr do_stmt  */
#line 568 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6739 "bison-chapel.cpp"
    break;

  case 37: /* stmt: TSERIAL expr do_stmt  */
#line 569 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6745 "bison-chapel.cpp"
    break;

  case 38: /* stmt: TSERIAL do_stmt  */
#line 570 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6751 "bison-chapel.cpp"
    break;

  case 39: /* stmt: TSYNC stmt  */
#line 571 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6757 "bison-chapel.cpp"
    break;

  case 40: /* stmt: TYIELD expr TSEMI  */
#line 572 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6763 "bison-chapel.cpp"
    break;

  case 41: /* stmt: error TSEMI  */
#line 573 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6769 "bison-chapel.cpp"
    break;

  case 43: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 579 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 6775 "bison-chapel.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 581 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 6781 "bison-chapel.cpp"
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
#line 6795 "bison-chapel.cpp"
    break;

  case 48: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 604 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6804 "bison-chapel.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 609 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6813 "bison-chapel.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 614 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6822 "bison-chapel.cpp"
    break;

  case 51: /* access_control: %empty  */
#line 621 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6828 "bison-chapel.cpp"
    break;

  case 52: /* access_control: TPUBLIC  */
#line 622 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6834 "bison-chapel.cpp"
    break;

  case 53: /* access_control: TPRIVATE  */
#line 623 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6840 "bison-chapel.cpp"
    break;

  case 54: /* opt_prototype: %empty  */
#line 627 "chapel.ypp"
             { (yyval.b) = false; }
#line 6846 "bison-chapel.cpp"
    break;

  case 55: /* opt_prototype: TPROTOTYPE  */
#line 628 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6852 "bison-chapel.cpp"
    break;

  case 56: /* include_access_control: %empty  */
#line 632 "chapel.ypp"
           { (yyval.b) = false; }
#line 6858 "bison-chapel.cpp"
    break;

  case 57: /* include_access_control: TPUBLIC  */
#line 633 "chapel.ypp"
           { (yyval.b) = false; }
#line 6864 "bison-chapel.cpp"
    break;

  case 58: /* include_access_control: TPRIVATE  */
#line 634 "chapel.ypp"
           { (yyval.b) = true; }
#line 6870 "bison-chapel.cpp"
    break;

  case 59: /* $@1: %empty  */
#line 639 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6879 "bison-chapel.cpp"
    break;

  case 60: /* include_module_stmt: TINCLUDE $@1 include_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 644 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6887 "bison-chapel.cpp"
    break;

  case 61: /* block_stmt: TLCBR TRCBR  */
#line 659 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6893 "bison-chapel.cpp"
    break;

  case 62: /* block_stmt: TLCBR stmt_ls TRCBR  */
#line 660 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6899 "bison-chapel.cpp"
    break;

  case 63: /* block_stmt: TLCBR error TRCBR  */
#line 661 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6905 "bison-chapel.cpp"
    break;

  case 64: /* stmt_ls: toplevel_stmt  */
#line 666 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6911 "bison-chapel.cpp"
    break;

  case 65: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 667 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6917 "bison-chapel.cpp"
    break;

  case 66: /* renames_ls: expr  */
#line 672 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6927 "bison-chapel.cpp"
    break;

  case 67: /* renames_ls: expr TAS expr  */
#line 677 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6937 "bison-chapel.cpp"
    break;

  case 68: /* renames_ls: renames_ls TCOMMA expr  */
#line 682 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6946 "bison-chapel.cpp"
    break;

  case 69: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 686 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6955 "bison-chapel.cpp"
    break;

  case 70: /* use_renames_ls: expr  */
#line 695 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6965 "bison-chapel.cpp"
    break;

  case 71: /* use_renames_ls: expr TAS expr  */
#line 700 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6975 "bison-chapel.cpp"
    break;

  case 72: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 705 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6985 "bison-chapel.cpp"
    break;

  case 73: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 710 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6994 "bison-chapel.cpp"
    break;

  case 74: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 714 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7003 "bison-chapel.cpp"
    break;

  case 75: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 718 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7012 "bison-chapel.cpp"
    break;

  case 76: /* opt_only_ls: %empty  */
#line 726 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7022 "bison-chapel.cpp"
    break;

  case 78: /* except_ls: TSTAR  */
#line 735 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7032 "bison-chapel.cpp"
    break;

  case 80: /* use_access_control: %empty  */
#line 744 "chapel.ypp"
           { (yyval.b) = true; }
#line 7038 "bison-chapel.cpp"
    break;

  case 81: /* use_access_control: TPUBLIC  */
#line 745 "chapel.ypp"
           { (yyval.b) = false; }
#line 7044 "bison-chapel.cpp"
    break;

  case 82: /* use_access_control: TPRIVATE  */
#line 746 "chapel.ypp"
           { (yyval.b) = true; }
#line 7050 "bison-chapel.cpp"
    break;

  case 83: /* use_stmt: use_access_control TUSE use_renames_ls TSEMI  */
#line 750 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7056 "bison-chapel.cpp"
    break;

  case 84: /* use_stmt: use_access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 751 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7062 "bison-chapel.cpp"
    break;

  case 85: /* use_stmt: use_access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 752 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7068 "bison-chapel.cpp"
    break;

  case 86: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 753 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7074 "bison-chapel.cpp"
    break;

  case 87: /* use_stmt: use_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 754 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7080 "bison-chapel.cpp"
    break;

  case 88: /* use_stmt: use_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 755 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7086 "bison-chapel.cpp"
    break;

  case 89: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 756 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7092 "bison-chapel.cpp"
    break;

  case 90: /* import_stmt: use_access_control TIMPORT import_ls TSEMI  */
#line 760 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7099 "bison-chapel.cpp"
    break;

  case 91: /* import_expr: expr  */
#line 765 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7105 "bison-chapel.cpp"
    break;

  case 92: /* import_expr: expr TAS ident_use  */
#line 766 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7111 "bison-chapel.cpp"
    break;

  case 93: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 767 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7117 "bison-chapel.cpp"
    break;

  case 94: /* import_ls: import_expr  */
#line 771 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7123 "bison-chapel.cpp"
    break;

  case 95: /* import_ls: import_ls TCOMMA import_expr  */
#line 772 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7129 "bison-chapel.cpp"
    break;

  case 96: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 776 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7135 "bison-chapel.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 781 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7141 "bison-chapel.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 783 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7147 "bison-chapel.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 785 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7153 "bison-chapel.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 787 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7159 "bison-chapel.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 789 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7165 "bison-chapel.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 791 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7171 "bison-chapel.cpp"
    break;

  case 103: /* opt_label_ident: %empty  */
#line 797 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7177 "bison-chapel.cpp"
    break;

  case 104: /* opt_label_ident: TIDENT  */
#line 798 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7183 "bison-chapel.cpp"
    break;

  case 105: /* ident_fn_def: TIDENT  */
#line 802 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7189 "bison-chapel.cpp"
    break;

  case 106: /* ident_fn_def: TNONE  */
#line 803 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7195 "bison-chapel.cpp"
    break;

  case 107: /* ident_fn_def: TTHIS  */
#line 804 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7201 "bison-chapel.cpp"
    break;

  case 108: /* ident_fn_def: TFALSE  */
#line 805 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7207 "bison-chapel.cpp"
    break;

  case 109: /* ident_fn_def: TTRUE  */
#line 806 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7213 "bison-chapel.cpp"
    break;

  case 110: /* ident_fn_def: internal_type_ident_def  */
#line 807 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7219 "bison-chapel.cpp"
    break;

  case 111: /* ident_def: TIDENT  */
#line 810 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7225 "bison-chapel.cpp"
    break;

  case 112: /* ident_def: TNONE  */
#line 811 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7231 "bison-chapel.cpp"
    break;

  case 113: /* ident_def: TTHIS  */
#line 812 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7237 "bison-chapel.cpp"
    break;

  case 114: /* ident_def: TFALSE  */
#line 813 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7243 "bison-chapel.cpp"
    break;

  case 115: /* ident_def: TTRUE  */
#line 814 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7249 "bison-chapel.cpp"
    break;

  case 116: /* ident_def: internal_type_ident_def  */
#line 815 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7255 "bison-chapel.cpp"
    break;

  case 117: /* ident_use: TIDENT  */
#line 827 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7261 "bison-chapel.cpp"
    break;

  case 118: /* ident_use: TTHIS  */
#line 828 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7267 "bison-chapel.cpp"
    break;

  case 119: /* internal_type_ident_def: TBOOL  */
#line 839 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7273 "bison-chapel.cpp"
    break;

  case 120: /* internal_type_ident_def: TINT  */
#line 840 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7279 "bison-chapel.cpp"
    break;

  case 121: /* internal_type_ident_def: TUINT  */
#line 841 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7285 "bison-chapel.cpp"
    break;

  case 122: /* internal_type_ident_def: TREAL  */
#line 842 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7291 "bison-chapel.cpp"
    break;

  case 123: /* internal_type_ident_def: TIMAG  */
#line 843 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7297 "bison-chapel.cpp"
    break;

  case 124: /* internal_type_ident_def: TCOMPLEX  */
#line 844 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7303 "bison-chapel.cpp"
    break;

  case 125: /* internal_type_ident_def: TBYTES  */
#line 845 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7309 "bison-chapel.cpp"
    break;

  case 126: /* internal_type_ident_def: TSTRING  */
#line 846 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7315 "bison-chapel.cpp"
    break;

  case 127: /* internal_type_ident_def: TSYNC  */
#line 847 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7321 "bison-chapel.cpp"
    break;

  case 128: /* internal_type_ident_def: TSINGLE  */
#line 848 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7327 "bison-chapel.cpp"
    break;

  case 129: /* internal_type_ident_def: TOWNED  */
#line 849 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7333 "bison-chapel.cpp"
    break;

  case 130: /* internal_type_ident_def: TSHARED  */
#line 850 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7339 "bison-chapel.cpp"
    break;

  case 131: /* internal_type_ident_def: TBORROWED  */
#line 851 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7345 "bison-chapel.cpp"
    break;

  case 132: /* internal_type_ident_def: TUNMANAGED  */
#line 852 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7351 "bison-chapel.cpp"
    break;

  case 133: /* internal_type_ident_def: TDOMAIN  */
#line 853 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7357 "bison-chapel.cpp"
    break;

  case 134: /* internal_type_ident_def: TINDEX  */
#line 854 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7363 "bison-chapel.cpp"
    break;

  case 135: /* internal_type_ident_def: TLOCALE  */
#line 855 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7369 "bison-chapel.cpp"
    break;

  case 136: /* internal_type_ident_def: TNOTHING  */
#line 856 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7375 "bison-chapel.cpp"
    break;

  case 137: /* internal_type_ident_def: TVOID  */
#line 857 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7381 "bison-chapel.cpp"
    break;

  case 138: /* scalar_type: TBOOL  */
#line 861 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7387 "bison-chapel.cpp"
    break;

  case 139: /* scalar_type: TENUM  */
#line 862 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7393 "bison-chapel.cpp"
    break;

  case 140: /* scalar_type: TINT  */
#line 863 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7399 "bison-chapel.cpp"
    break;

  case 141: /* scalar_type: TUINT  */
#line 864 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7405 "bison-chapel.cpp"
    break;

  case 142: /* scalar_type: TREAL  */
#line 865 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7411 "bison-chapel.cpp"
    break;

  case 143: /* scalar_type: TIMAG  */
#line 866 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7417 "bison-chapel.cpp"
    break;

  case 144: /* scalar_type: TCOMPLEX  */
#line 867 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7423 "bison-chapel.cpp"
    break;

  case 145: /* scalar_type: TBYTES  */
#line 868 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7429 "bison-chapel.cpp"
    break;

  case 146: /* scalar_type: TSTRING  */
#line 869 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7435 "bison-chapel.cpp"
    break;

  case 147: /* scalar_type: TLOCALE  */
#line 870 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7441 "bison-chapel.cpp"
    break;

  case 148: /* scalar_type: TNOTHING  */
#line 871 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7447 "bison-chapel.cpp"
    break;

  case 149: /* scalar_type: TVOID  */
#line 872 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7453 "bison-chapel.cpp"
    break;

  case 150: /* reserved_type_ident_use: TSYNC  */
#line 879 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7459 "bison-chapel.cpp"
    break;

  case 151: /* reserved_type_ident_use: TSINGLE  */
#line 880 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7465 "bison-chapel.cpp"
    break;

  case 152: /* reserved_type_ident_use: TDOMAIN  */
#line 881 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7471 "bison-chapel.cpp"
    break;

  case 153: /* reserved_type_ident_use: TINDEX  */
#line 882 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7477 "bison-chapel.cpp"
    break;

  case 154: /* do_stmt: TDO stmt  */
#line 886 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7483 "bison-chapel.cpp"
    break;

  case 155: /* do_stmt: block_stmt  */
#line 887 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7489 "bison-chapel.cpp"
    break;

  case 156: /* return_stmt: TRETURN TSEMI  */
#line 891 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7495 "bison-chapel.cpp"
    break;

  case 157: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 892 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7501 "bison-chapel.cpp"
    break;

  case 159: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 898 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 7507 "bison-chapel.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 900 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 7513 "bison-chapel.cpp"
    break;

  case 161: /* class_level_stmt: TSEMI  */
#line 904 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7519 "bison-chapel.cpp"
    break;

  case 163: /* class_level_stmt: TPUBLIC private_decl  */
#line 906 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7525 "bison-chapel.cpp"
    break;

  case 164: /* @2: %empty  */
#line 916 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7531 "bison-chapel.cpp"
    break;

  case 165: /* class_level_stmt: TPRIVATE @2 private_decl  */
#line 917 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7537 "bison-chapel.cpp"
    break;

  case 173: /* forwarding_stmt: TFORWARDING expr TSEMI  */
#line 931 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7543 "bison-chapel.cpp"
    break;

  case 174: /* forwarding_stmt: TFORWARDING expr TEXCEPT except_ls TSEMI  */
#line 932 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7549 "bison-chapel.cpp"
    break;

  case 175: /* forwarding_stmt: TFORWARDING expr TONLY opt_only_ls TSEMI  */
#line 933 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7555 "bison-chapel.cpp"
    break;

  case 176: /* forwarding_stmt: TFORWARDING var_decl_stmt  */
#line 934 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7561 "bison-chapel.cpp"
    break;

  case 177: /* $@3: %empty  */
#line 939 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7570 "bison-chapel.cpp"
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
#line 7584 "bison-chapel.cpp"
    break;

  case 179: /* $@4: %empty  */
#line 954 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7593 "bison-chapel.cpp"
    break;

  case 180: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TRECORD $@4 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 959 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7608 "bison-chapel.cpp"
    break;

  case 181: /* $@5: %empty  */
#line 970 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7617 "bison-chapel.cpp"
    break;

  case 182: /* extern_export_decl_stmt: TEXPORT TRECORD $@5 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 975 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7631 "bison-chapel.cpp"
    break;

  case 183: /* $@6: %empty  */
#line 985 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7640 "bison-chapel.cpp"
    break;

  case 184: /* extern_export_decl_stmt: TEXPORT STRINGLITERAL TRECORD $@6 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 990 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7654 "bison-chapel.cpp"
    break;

  case 185: /* extern_export_decl_stmt: TEXTERN opt_expr fn_decl_stmt  */
#line 1001 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7662 "bison-chapel.cpp"
    break;

  case 186: /* extern_export_decl_stmt: TEXPORT opt_expr fn_decl_stmt  */
#line 1005 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7670 "bison-chapel.cpp"
    break;

  case 187: /* extern_export_decl_stmt: TEXTERN opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1010 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7682 "bison-chapel.cpp"
    break;

  case 188: /* extern_export_decl_stmt: TEXPORT opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1018 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7694 "bison-chapel.cpp"
    break;

  case 189: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1029 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7702 "bison-chapel.cpp"
    break;

  case 190: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1035 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7708 "bison-chapel.cpp"
    break;

  case 191: /* loop_stmt: TWHILE expr do_stmt  */
#line 1036 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7714 "bison-chapel.cpp"
    break;

  case 192: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1037 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7720 "bison-chapel.cpp"
    break;

  case 193: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1038 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7726 "bison-chapel.cpp"
    break;

  case 194: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1039 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7732 "bison-chapel.cpp"
    break;

  case 195: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1040 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7738 "bison-chapel.cpp"
    break;

  case 196: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1041 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7744 "bison-chapel.cpp"
    break;

  case 197: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1042 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7750 "bison-chapel.cpp"
    break;

  case 198: /* loop_stmt: TFOR expr do_stmt  */
#line 1043 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7756 "bison-chapel.cpp"
    break;

  case 199: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1044 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7762 "bison-chapel.cpp"
    break;

  case 200: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1045 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7768 "bison-chapel.cpp"
    break;

  case 201: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1046 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7774 "bison-chapel.cpp"
    break;

  case 202: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1047 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7780 "bison-chapel.cpp"
    break;

  case 203: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1048 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7786 "bison-chapel.cpp"
    break;

  case 204: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1049 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7792 "bison-chapel.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr do_stmt  */
#line 1050 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7798 "bison-chapel.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1051 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7804 "bison-chapel.cpp"
    break;

  case 207: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1052 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7810 "bison-chapel.cpp"
    break;

  case 208: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1053 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7816 "bison-chapel.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1054 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7822 "bison-chapel.cpp"
    break;

  case 210: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1055 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7828 "bison-chapel.cpp"
    break;

  case 211: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1056 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7834 "bison-chapel.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1057 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7840 "bison-chapel.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1058 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7846 "bison-chapel.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1059 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7852 "bison-chapel.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1060 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7858 "bison-chapel.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1061 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = foreachNotImplementedError(); }
#line 7864 "bison-chapel.cpp"
    break;

  case 217: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1063 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7874 "bison-chapel.cpp"
    break;

  case 218: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1069 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7884 "bison-chapel.cpp"
    break;

  case 219: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1075 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7894 "bison-chapel.cpp"
    break;

  case 220: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1081 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7904 "bison-chapel.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1087 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7915 "bison-chapel.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1094 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7926 "bison-chapel.cpp"
    break;

  case 223: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1101 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7934 "bison-chapel.cpp"
    break;

  case 224: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1105 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7942 "bison-chapel.cpp"
    break;

  case 225: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1111 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7948 "bison-chapel.cpp"
    break;

  case 226: /* if_stmt: TIF expr TTHEN stmt  */
#line 1115 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7954 "bison-chapel.cpp"
    break;

  case 227: /* if_stmt: TIF expr block_stmt  */
#line 1116 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7960 "bison-chapel.cpp"
    break;

  case 228: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1117 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7966 "bison-chapel.cpp"
    break;

  case 229: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1118 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7972 "bison-chapel.cpp"
    break;

  case 230: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1120 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7978 "bison-chapel.cpp"
    break;

  case 231: /* if_stmt: TIF ifvar block_stmt  */
#line 1121 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7984 "bison-chapel.cpp"
    break;

  case 232: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1122 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7990 "bison-chapel.cpp"
    break;

  case 233: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1123 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7996 "bison-chapel.cpp"
    break;

  case 234: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1125 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8003 "bison-chapel.cpp"
    break;

  case 235: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1127 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8010 "bison-chapel.cpp"
    break;

  case 236: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1129 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8017 "bison-chapel.cpp"
    break;

  case 237: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1131 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8024 "bison-chapel.cpp"
    break;

  case 238: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1136 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 8030 "bison-chapel.cpp"
    break;

  case 239: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1137 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 8036 "bison-chapel.cpp"
    break;

  case 240: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1142 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 8042 "bison-chapel.cpp"
    break;

  case 241: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1144 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8051 "bison-chapel.cpp"
    break;

  case 242: /* ifc_formal_ls: ifc_formal  */
#line 1151 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8057 "bison-chapel.cpp"
    break;

  case 243: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1152 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8063 "bison-chapel.cpp"
    break;

  case 244: /* ifc_formal: ident_def  */
#line 1157 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8069 "bison-chapel.cpp"
    break;

  case 245: /* implements_type_ident: TIDENT  */
#line 1162 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 8075 "bison-chapel.cpp"
    break;

  case 246: /* implements_type_ident: TBOOL  */
#line 1163 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 8081 "bison-chapel.cpp"
    break;

  case 247: /* implements_type_ident: TINT  */
#line 1164 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 8087 "bison-chapel.cpp"
    break;

  case 248: /* implements_type_ident: TUINT  */
#line 1165 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 8093 "bison-chapel.cpp"
    break;

  case 249: /* implements_type_ident: TREAL  */
#line 1166 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 8099 "bison-chapel.cpp"
    break;

  case 250: /* implements_type_ident: TIMAG  */
#line 1167 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 8105 "bison-chapel.cpp"
    break;

  case 251: /* implements_type_ident: TCOMPLEX  */
#line 1168 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 8111 "bison-chapel.cpp"
    break;

  case 252: /* implements_type_ident: TBYTES  */
#line 1169 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 8117 "bison-chapel.cpp"
    break;

  case 253: /* implements_type_ident: TSTRING  */
#line 1170 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 8123 "bison-chapel.cpp"
    break;

  case 254: /* implements_type_ident: TLOCALE  */
#line 1171 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 8129 "bison-chapel.cpp"
    break;

  case 255: /* implements_type_ident: TNOTHING  */
#line 1172 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 8135 "bison-chapel.cpp"
    break;

  case 256: /* implements_type_ident: TVOID  */
#line 1173 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 8141 "bison-chapel.cpp"
    break;

  case 257: /* implements_type_ident: implements_type_error_ident  */
#line 1175 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8148 "bison-chapel.cpp"
    break;

  case 258: /* implements_type_error_ident: TNONE  */
#line 1181 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 8154 "bison-chapel.cpp"
    break;

  case 259: /* implements_type_error_ident: TTHIS  */
#line 1182 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 8160 "bison-chapel.cpp"
    break;

  case 260: /* implements_type_error_ident: TFALSE  */
#line 1183 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 8166 "bison-chapel.cpp"
    break;

  case 261: /* implements_type_error_ident: TTRUE  */
#line 1184 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 8172 "bison-chapel.cpp"
    break;

  case 262: /* implements_type_error_ident: TDOMAIN  */
#line 1193 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 8178 "bison-chapel.cpp"
    break;

  case 263: /* implements_type_error_ident: TINDEX  */
#line 1194 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 8184 "bison-chapel.cpp"
    break;

  case 264: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1199 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8190 "bison-chapel.cpp"
    break;

  case 265: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1201 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8197 "bison-chapel.cpp"
    break;

  case 266: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1204 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8204 "bison-chapel.cpp"
    break;

  case 267: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1210 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8210 "bison-chapel.cpp"
    break;

  case 268: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1212 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8217 "bison-chapel.cpp"
    break;

  case 269: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1215 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8224 "bison-chapel.cpp"
    break;

  case 270: /* defer_stmt: TDEFER stmt  */
#line 1220 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8230 "bison-chapel.cpp"
    break;

  case 271: /* try_stmt: TTRY expr TSEMI  */
#line 1223 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 8236 "bison-chapel.cpp"
    break;

  case 272: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1224 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 8242 "bison-chapel.cpp"
    break;

  case 273: /* try_stmt: TTRY assignment_stmt  */
#line 1225 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 8248 "bison-chapel.cpp"
    break;

  case 274: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1226 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 8254 "bison-chapel.cpp"
    break;

  case 275: /* try_stmt: TTRY block_stmt catch_stmt_ls  */
#line 1227 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8260 "bison-chapel.cpp"
    break;

  case 276: /* try_stmt: TTRYBANG block_stmt catch_stmt_ls  */
#line 1228 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8266 "bison-chapel.cpp"
    break;

  case 277: /* catch_stmt_ls: %empty  */
#line 1232 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8272 "bison-chapel.cpp"
    break;

  case 278: /* catch_stmt_ls: catch_stmt_ls catch_stmt  */
#line 1233 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8278 "bison-chapel.cpp"
    break;

  case 279: /* catch_stmt: TCATCH block_stmt  */
#line 1237 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8284 "bison-chapel.cpp"
    break;

  case 280: /* catch_stmt: TCATCH catch_expr block_stmt  */
#line 1238 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8290 "bison-chapel.cpp"
    break;

  case 281: /* catch_stmt: TCATCH TLP catch_expr TRP block_stmt  */
#line 1239 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8296 "bison-chapel.cpp"
    break;

  case 282: /* catch_expr: ident_def  */
#line 1243 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8302 "bison-chapel.cpp"
    break;

  case 283: /* catch_expr: ident_def TCOLON expr  */
#line 1244 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8308 "bison-chapel.cpp"
    break;

  case 284: /* throw_stmt: TTHROW expr TSEMI  */
#line 1248 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8314 "bison-chapel.cpp"
    break;

  case 285: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1252 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8320 "bison-chapel.cpp"
    break;

  case 286: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1254 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8326 "bison-chapel.cpp"
    break;

  case 287: /* when_stmt_ls: %empty  */
#line 1258 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8332 "bison-chapel.cpp"
    break;

  case 288: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1259 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8338 "bison-chapel.cpp"
    break;

  case 289: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1264 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8344 "bison-chapel.cpp"
    break;

  case 290: /* when_stmt: TOTHERWISE stmt  */
#line 1266 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8350 "bison-chapel.cpp"
    break;

  case 291: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1268 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8356 "bison-chapel.cpp"
    break;

  case 292: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1275 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8370 "bison-chapel.cpp"
    break;

  case 293: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR error TRCBR  */
#line 1285 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8384 "bison-chapel.cpp"
    break;

  case 294: /* class_tag: TCLASS  */
#line 1298 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8394 "bison-chapel.cpp"
    break;

  case 295: /* class_tag: TRECORD  */
#line 1303 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8404 "bison-chapel.cpp"
    break;

  case 296: /* class_tag: TUNION  */
#line 1308 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8414 "bison-chapel.cpp"
    break;

  case 297: /* opt_inherit: %empty  */
#line 1316 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8420 "bison-chapel.cpp"
    break;

  case 298: /* opt_inherit: TCOLON expr_ls  */
#line 1317 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8426 "bison-chapel.cpp"
    break;

  case 299: /* class_level_stmt_ls: %empty  */
#line 1322 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8432 "bison-chapel.cpp"
    break;

  case 300: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1324 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8438 "bison-chapel.cpp"
    break;

  case 301: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1326 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8444 "bison-chapel.cpp"
    break;

  case 302: /* enum_decl_stmt: enum_header ident_def TLCBR enum_ls TRCBR  */
#line 1331 "chapel.ypp"
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
#line 8464 "bison-chapel.cpp"
    break;

  case 303: /* enum_decl_stmt: enum_header ident_def TLCBR error TRCBR  */
#line 1347 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8472 "bison-chapel.cpp"
    break;

  case 304: /* enum_header: TENUM  */
#line 1354 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8482 "bison-chapel.cpp"
    break;

  case 305: /* enum_ls: deprecated_enum_item  */
#line 1363 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8494 "bison-chapel.cpp"
    break;

  case 306: /* enum_ls: enum_ls TCOMMA  */
#line 1371 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8502 "bison-chapel.cpp"
    break;

  case 307: /* enum_ls: enum_ls TCOMMA deprecated_enum_item  */
#line 1375 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8510 "bison-chapel.cpp"
    break;

  case 309: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1383 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr), (yyvsp[-1].pch)); }
#line 8516 "bison-chapel.cpp"
    break;

  case 310: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1385 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr)); }
#line 8522 "bison-chapel.cpp"
    break;

  case 311: /* enum_item: ident_def  */
#line 1389 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8528 "bison-chapel.cpp"
    break;

  case 312: /* enum_item: ident_def TASSIGN expr  */
#line 1390 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8534 "bison-chapel.cpp"
    break;

  case 313: /* $@7: %empty  */
#line 1395 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8543 "bison-chapel.cpp"
    break;

  case 314: /* $@8: %empty  */
#line 1400 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8552 "bison-chapel.cpp"
    break;

  case 315: /* lambda_decl_expr: TLAMBDA $@7 req_formal_ls $@8 opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1405 "chapel.ypp"
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
#line 8574 "bison-chapel.cpp"
    break;

  case 316: /* linkage_spec: %empty  */
#line 1427 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8582 "bison-chapel.cpp"
    break;

  case 317: /* linkage_spec: TINLINE  */
#line 1430 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8591 "bison-chapel.cpp"
    break;

  case 318: /* linkage_spec: TOVERRIDE  */
#line 1434 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8600 "bison-chapel.cpp"
    break;

  case 319: /* $@9: %empty  */
#line 1442 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8614 "bison-chapel.cpp"
    break;

  case 320: /* $@10: %empty  */
#line 1452 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8625 "bison-chapel.cpp"
    break;

  case 321: /* fn_decl_stmt: linkage_spec proc_iter_or_op $@9 fn_decl_stmt_inner $@10 opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where opt_function_body_stmt  */
#line 1459 "chapel.ypp"
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
#line 8659 "bison-chapel.cpp"
    break;

  case 322: /* fn_decl_stmt_inner: opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1492 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8667 "bison-chapel.cpp"
    break;

  case 323: /* fn_decl_stmt_inner: opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1496 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8676 "bison-chapel.cpp"
    break;

  case 324: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1501 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8684 "bison-chapel.cpp"
    break;

  case 325: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1505 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8693 "bison-chapel.cpp"
    break;

  case 326: /* fn_decl_stmt_inner: opt_this_intent_tag error opt_formal_ls  */
#line 1510 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8701 "bison-chapel.cpp"
    break;

  case 328: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1517 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8707 "bison-chapel.cpp"
    break;

  case 329: /* fn_ident: ident_fn_def  */
#line 1521 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8713 "bison-chapel.cpp"
    break;

  case 330: /* fn_ident: TBAND  */
#line 1522 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8719 "bison-chapel.cpp"
    break;

  case 331: /* fn_ident: TBOR  */
#line 1523 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8725 "bison-chapel.cpp"
    break;

  case 332: /* fn_ident: TBXOR  */
#line 1524 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8731 "bison-chapel.cpp"
    break;

  case 333: /* fn_ident: TBNOT  */
#line 1525 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8737 "bison-chapel.cpp"
    break;

  case 334: /* fn_ident: TEQUAL  */
#line 1526 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8743 "bison-chapel.cpp"
    break;

  case 335: /* fn_ident: TNOTEQUAL  */
#line 1527 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8749 "bison-chapel.cpp"
    break;

  case 336: /* fn_ident: TLESSEQUAL  */
#line 1528 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8755 "bison-chapel.cpp"
    break;

  case 337: /* fn_ident: TGREATEREQUAL  */
#line 1529 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8761 "bison-chapel.cpp"
    break;

  case 338: /* fn_ident: TLESS  */
#line 1530 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8767 "bison-chapel.cpp"
    break;

  case 339: /* fn_ident: TGREATER  */
#line 1531 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8773 "bison-chapel.cpp"
    break;

  case 340: /* fn_ident: TPLUS  */
#line 1532 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8779 "bison-chapel.cpp"
    break;

  case 341: /* fn_ident: TMINUS  */
#line 1533 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8785 "bison-chapel.cpp"
    break;

  case 342: /* fn_ident: TSTAR  */
#line 1534 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8791 "bison-chapel.cpp"
    break;

  case 343: /* fn_ident: TDIVIDE  */
#line 1535 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8797 "bison-chapel.cpp"
    break;

  case 344: /* fn_ident: TSHIFTLEFT  */
#line 1536 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8803 "bison-chapel.cpp"
    break;

  case 345: /* fn_ident: TSHIFTRIGHT  */
#line 1537 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8809 "bison-chapel.cpp"
    break;

  case 346: /* fn_ident: TMOD  */
#line 1538 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8815 "bison-chapel.cpp"
    break;

  case 347: /* fn_ident: TEXP  */
#line 1539 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8821 "bison-chapel.cpp"
    break;

  case 348: /* fn_ident: TBANG  */
#line 1540 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8827 "bison-chapel.cpp"
    break;

  case 349: /* fn_ident: TBY  */
#line 1541 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8833 "bison-chapel.cpp"
    break;

  case 350: /* fn_ident: THASH  */
#line 1542 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8839 "bison-chapel.cpp"
    break;

  case 351: /* fn_ident: TALIGN  */
#line 1543 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8845 "bison-chapel.cpp"
    break;

  case 352: /* fn_ident: TSWAP  */
#line 1544 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8851 "bison-chapel.cpp"
    break;

  case 353: /* fn_ident: TIO  */
#line 1545 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8857 "bison-chapel.cpp"
    break;

  case 354: /* fn_ident: TINITEQUALS  */
#line 1546 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8863 "bison-chapel.cpp"
    break;

  case 355: /* fn_ident: TCOLON  */
#line 1547 "chapel.ypp"
                 { (yyval.pch) = ":"; }
#line 8869 "bison-chapel.cpp"
    break;

  case 356: /* fn_ident: ident_def TBANG  */
#line 1548 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8875 "bison-chapel.cpp"
    break;

  case 357: /* assignop_ident: TASSIGN  */
#line 1552 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8881 "bison-chapel.cpp"
    break;

  case 358: /* assignop_ident: TASSIGNPLUS  */
#line 1553 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8887 "bison-chapel.cpp"
    break;

  case 359: /* assignop_ident: TASSIGNMINUS  */
#line 1554 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8893 "bison-chapel.cpp"
    break;

  case 360: /* assignop_ident: TASSIGNMULTIPLY  */
#line 1555 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8899 "bison-chapel.cpp"
    break;

  case 361: /* assignop_ident: TASSIGNDIVIDE  */
#line 1556 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8905 "bison-chapel.cpp"
    break;

  case 362: /* assignop_ident: TASSIGNMOD  */
#line 1557 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8911 "bison-chapel.cpp"
    break;

  case 363: /* assignop_ident: TASSIGNEXP  */
#line 1558 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8917 "bison-chapel.cpp"
    break;

  case 364: /* assignop_ident: TASSIGNBAND  */
#line 1559 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8923 "bison-chapel.cpp"
    break;

  case 365: /* assignop_ident: TASSIGNBOR  */
#line 1560 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8929 "bison-chapel.cpp"
    break;

  case 366: /* assignop_ident: TASSIGNBXOR  */
#line 1561 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8935 "bison-chapel.cpp"
    break;

  case 367: /* assignop_ident: TASSIGNSR  */
#line 1562 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8941 "bison-chapel.cpp"
    break;

  case 368: /* assignop_ident: TASSIGNSL  */
#line 1563 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 8947 "bison-chapel.cpp"
    break;

  case 369: /* opt_formal_ls: %empty  */
#line 1567 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8953 "bison-chapel.cpp"
    break;

  case 370: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1568 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8959 "bison-chapel.cpp"
    break;

  case 371: /* req_formal_ls: TLP formal_ls TRP  */
#line 1572 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8965 "bison-chapel.cpp"
    break;

  case 372: /* formal_ls_inner: formal  */
#line 1576 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8971 "bison-chapel.cpp"
    break;

  case 373: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 1577 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8977 "bison-chapel.cpp"
    break;

  case 374: /* formal_ls: %empty  */
#line 1581 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8983 "bison-chapel.cpp"
    break;

  case 375: /* formal_ls: formal_ls_inner  */
#line 1582 "chapel.ypp"
                           { (yyval.pfnsymbol) = (yyvsp[0].pfnsymbol); }
#line 8989 "bison-chapel.cpp"
    break;

  case 376: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1587 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8995 "bison-chapel.cpp"
    break;

  case 377: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1589 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 9001 "bison-chapel.cpp"
    break;

  case 378: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1591 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 9007 "bison-chapel.cpp"
    break;

  case 379: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1593 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 9013 "bison-chapel.cpp"
    break;

  case 380: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1595 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9019 "bison-chapel.cpp"
    break;

  case 381: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1597 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 9025 "bison-chapel.cpp"
    break;

  case 382: /* opt_intent_tag: %empty  */
#line 1601 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 9031 "bison-chapel.cpp"
    break;

  case 383: /* opt_intent_tag: required_intent_tag  */
#line 1602 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 9037 "bison-chapel.cpp"
    break;

  case 384: /* required_intent_tag: TIN  */
#line 1606 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 9043 "bison-chapel.cpp"
    break;

  case 385: /* required_intent_tag: TINOUT  */
#line 1607 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 9049 "bison-chapel.cpp"
    break;

  case 386: /* required_intent_tag: TOUT  */
#line 1608 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 9055 "bison-chapel.cpp"
    break;

  case 387: /* required_intent_tag: TCONST  */
#line 1609 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 9061 "bison-chapel.cpp"
    break;

  case 388: /* required_intent_tag: TCONST TIN  */
#line 1610 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 9067 "bison-chapel.cpp"
    break;

  case 389: /* required_intent_tag: TCONST TREF  */
#line 1611 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 9073 "bison-chapel.cpp"
    break;

  case 390: /* required_intent_tag: TPARAM  */
#line 1612 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 9079 "bison-chapel.cpp"
    break;

  case 391: /* required_intent_tag: TREF  */
#line 1613 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 9085 "bison-chapel.cpp"
    break;

  case 392: /* required_intent_tag: TTYPE  */
#line 1614 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 9091 "bison-chapel.cpp"
    break;

  case 393: /* opt_this_intent_tag: %empty  */
#line 1618 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 9097 "bison-chapel.cpp"
    break;

  case 394: /* opt_this_intent_tag: TPARAM  */
#line 1619 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 9103 "bison-chapel.cpp"
    break;

  case 395: /* opt_this_intent_tag: TREF  */
#line 1620 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 9109 "bison-chapel.cpp"
    break;

  case 396: /* opt_this_intent_tag: TCONST TREF  */
#line 1621 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 9115 "bison-chapel.cpp"
    break;

  case 397: /* opt_this_intent_tag: TCONST  */
#line 1622 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 9121 "bison-chapel.cpp"
    break;

  case 398: /* opt_this_intent_tag: TTYPE  */
#line 1623 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 9127 "bison-chapel.cpp"
    break;

  case 399: /* proc_iter_or_op: TPROC  */
#line 1627 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 9133 "bison-chapel.cpp"
    break;

  case 400: /* proc_iter_or_op: TITER  */
#line 1628 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 9139 "bison-chapel.cpp"
    break;

  case 401: /* proc_iter_or_op: TOPERATOR  */
#line 1629 "chapel.ypp"
            { (yyval.procIterOp) = ProcIterOp_OP; }
#line 9145 "bison-chapel.cpp"
    break;

  case 402: /* opt_ret_tag: %empty  */
#line 1633 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9151 "bison-chapel.cpp"
    break;

  case 403: /* opt_ret_tag: TCONST  */
#line 1634 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9157 "bison-chapel.cpp"
    break;

  case 404: /* opt_ret_tag: TCONST TREF  */
#line 1635 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 9163 "bison-chapel.cpp"
    break;

  case 405: /* opt_ret_tag: TREF  */
#line 1636 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 9169 "bison-chapel.cpp"
    break;

  case 406: /* opt_ret_tag: TPARAM  */
#line 1637 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 9175 "bison-chapel.cpp"
    break;

  case 407: /* opt_ret_tag: TTYPE  */
#line 1638 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 9181 "bison-chapel.cpp"
    break;

  case 408: /* opt_throws_error: %empty  */
#line 1642 "chapel.ypp"
          { (yyval.b) = false; }
#line 9187 "bison-chapel.cpp"
    break;

  case 409: /* opt_throws_error: TTHROWS  */
#line 1643 "chapel.ypp"
          { (yyval.b) = true;  }
#line 9193 "bison-chapel.cpp"
    break;

  case 410: /* opt_function_body_stmt: TSEMI  */
#line 1646 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 9199 "bison-chapel.cpp"
    break;

  case 413: /* function_body_stmt: return_stmt  */
#line 1652 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9205 "bison-chapel.cpp"
    break;

  case 414: /* query_expr: TQUERIEDIDENT  */
#line 1656 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9211 "bison-chapel.cpp"
    break;

  case 415: /* var_arg_expr: TDOTDOTDOT  */
#line 1660 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9217 "bison-chapel.cpp"
    break;

  case 416: /* var_arg_expr: TDOTDOTDOT expr  */
#line 1661 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9223 "bison-chapel.cpp"
    break;

  case 417: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 1662 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9233 "bison-chapel.cpp"
    break;

  case 418: /* opt_lifetime_where: %empty  */
#line 1670 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9239 "bison-chapel.cpp"
    break;

  case 419: /* opt_lifetime_where: TWHERE expr  */
#line 1672 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9245 "bison-chapel.cpp"
    break;

  case 420: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 1674 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9251 "bison-chapel.cpp"
    break;

  case 421: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 1676 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9257 "bison-chapel.cpp"
    break;

  case 422: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 1678 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9263 "bison-chapel.cpp"
    break;

  case 423: /* lifetime_components_expr: lifetime_expr  */
#line 1683 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9269 "bison-chapel.cpp"
    break;

  case 424: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 1685 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9275 "bison-chapel.cpp"
    break;

  case 425: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 1689 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9281 "bison-chapel.cpp"
    break;

  case 426: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 1690 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9287 "bison-chapel.cpp"
    break;

  case 427: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 1691 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9293 "bison-chapel.cpp"
    break;

  case 428: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 1692 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9299 "bison-chapel.cpp"
    break;

  case 429: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 1693 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9305 "bison-chapel.cpp"
    break;

  case 430: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 1694 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9311 "bison-chapel.cpp"
    break;

  case 431: /* lifetime_expr: TRETURN lifetime_ident  */
#line 1695 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9317 "bison-chapel.cpp"
    break;

  case 432: /* lifetime_ident: TIDENT  */
#line 1700 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9323 "bison-chapel.cpp"
    break;

  case 433: /* lifetime_ident: TTHIS  */
#line 1702 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9329 "bison-chapel.cpp"
    break;

  case 434: /* type_alias_decl_stmt: TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1707 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9335 "bison-chapel.cpp"
    break;

  case 435: /* type_alias_decl_stmt: TCONFIG TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1709 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9341 "bison-chapel.cpp"
    break;

  case 436: /* type_alias_decl_stmt: TEXTERN TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1711 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9347 "bison-chapel.cpp"
    break;

  case 437: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 1716 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9364 "bison-chapel.cpp"
    break;

  case 438: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 1729 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9382 "bison-chapel.cpp"
    break;

  case 439: /* opt_init_type: %empty  */
#line 1745 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9388 "bison-chapel.cpp"
    break;

  case 440: /* opt_init_type: TASSIGN type_level_expr  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9394 "bison-chapel.cpp"
    break;

  case 441: /* opt_init_type: TASSIGN array_type  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9400 "bison-chapel.cpp"
    break;

  case 442: /* var_decl_type: TPARAM  */
#line 1753 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9406 "bison-chapel.cpp"
    break;

  case 443: /* var_decl_type: TCONST  */
#line 1754 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9412 "bison-chapel.cpp"
    break;

  case 444: /* var_decl_type: TREF  */
#line 1755 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9418 "bison-chapel.cpp"
    break;

  case 445: /* var_decl_type: TCONST TREF  */
#line 1756 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9424 "bison-chapel.cpp"
    break;

  case 446: /* var_decl_type: TVAR  */
#line 1757 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9430 "bison-chapel.cpp"
    break;

  case 447: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1762 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9440 "bison-chapel.cpp"
    break;

  case 448: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1768 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9449 "bison-chapel.cpp"
    break;

  case 450: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 1777 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9458 "bison-chapel.cpp"
    break;

  case 451: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 1785 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9464 "bison-chapel.cpp"
    break;

  case 452: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 1787 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9470 "bison-chapel.cpp"
    break;

  case 453: /* tuple_var_decl_component: TUNDERSCORE  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9476 "bison-chapel.cpp"
    break;

  case 454: /* tuple_var_decl_component: ident_def  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9482 "bison-chapel.cpp"
    break;

  case 455: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 1796 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9488 "bison-chapel.cpp"
    break;

  case 456: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 1801 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9494 "bison-chapel.cpp"
    break;

  case 457: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 1803 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9500 "bison-chapel.cpp"
    break;

  case 458: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 1805 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9506 "bison-chapel.cpp"
    break;

  case 459: /* opt_init_expr: %empty  */
#line 1811 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9512 "bison-chapel.cpp"
    break;

  case 460: /* opt_init_expr: TASSIGN TNOINIT  */
#line 1812 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9518 "bison-chapel.cpp"
    break;

  case 461: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 1813 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9524 "bison-chapel.cpp"
    break;

  case 462: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 1819 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9530 "bison-chapel.cpp"
    break;

  case 463: /* ret_array_type: TLSBR TRSBR  */
#line 1821 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9536 "bison-chapel.cpp"
    break;

  case 464: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9544 "bison-chapel.cpp"
    break;

  case 465: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 1827 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9552 "bison-chapel.cpp"
    break;

  case 466: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 1831 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9558 "bison-chapel.cpp"
    break;

  case 467: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 1833 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9566 "bison-chapel.cpp"
    break;

  case 468: /* ret_array_type: TLSBR error TRSBR  */
#line 1837 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9574 "bison-chapel.cpp"
    break;

  case 469: /* opt_ret_type: %empty  */
#line 1844 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9580 "bison-chapel.cpp"
    break;

  case 470: /* opt_ret_type: TCOLON type_level_expr  */
#line 1845 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9586 "bison-chapel.cpp"
    break;

  case 471: /* opt_ret_type: TCOLON ret_array_type  */
#line 1846 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9592 "bison-chapel.cpp"
    break;

  case 472: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 1847 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9598 "bison-chapel.cpp"
    break;

  case 473: /* opt_ret_type: error  */
#line 1848 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9604 "bison-chapel.cpp"
    break;

  case 474: /* opt_type: %empty  */
#line 1853 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9610 "bison-chapel.cpp"
    break;

  case 475: /* opt_type: TCOLON type_level_expr  */
#line 1854 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9616 "bison-chapel.cpp"
    break;

  case 476: /* opt_type: TCOLON array_type  */
#line 1855 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9622 "bison-chapel.cpp"
    break;

  case 477: /* opt_type: TCOLON reserved_type_ident_use  */
#line 1856 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9628 "bison-chapel.cpp"
    break;

  case 478: /* opt_type: error  */
#line 1857 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9634 "bison-chapel.cpp"
    break;

  case 479: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1878 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9642 "bison-chapel.cpp"
    break;

  case 480: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 1882 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9650 "bison-chapel.cpp"
    break;

  case 481: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 1886 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9662 "bison-chapel.cpp"
    break;

  case 482: /* array_type: TLSBR error TRSBR  */
#line 1894 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9670 "bison-chapel.cpp"
    break;

  case 483: /* opt_formal_array_elt_type: %empty  */
#line 1900 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9676 "bison-chapel.cpp"
    break;

  case 484: /* opt_formal_array_elt_type: type_level_expr  */
#line 1901 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9682 "bison-chapel.cpp"
    break;

  case 485: /* opt_formal_array_elt_type: query_expr  */
#line 1902 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9688 "bison-chapel.cpp"
    break;

  case 486: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 1907 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9694 "bison-chapel.cpp"
    break;

  case 487: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9700 "bison-chapel.cpp"
    break;

  case 488: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 1915 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9706 "bison-chapel.cpp"
    break;

  case 489: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 1917 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9712 "bison-chapel.cpp"
    break;

  case 490: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 1919 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9718 "bison-chapel.cpp"
    break;

  case 491: /* opt_formal_type: %empty  */
#line 1923 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9724 "bison-chapel.cpp"
    break;

  case 492: /* opt_formal_type: TCOLON type_level_expr  */
#line 1924 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9730 "bison-chapel.cpp"
    break;

  case 493: /* opt_formal_type: TCOLON query_expr  */
#line 1925 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9736 "bison-chapel.cpp"
    break;

  case 494: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 1926 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9742 "bison-chapel.cpp"
    break;

  case 495: /* opt_formal_type: TCOLON formal_array_type  */
#line 1927 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9748 "bison-chapel.cpp"
    break;

  case 496: /* expr_ls: expr  */
#line 1933 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9754 "bison-chapel.cpp"
    break;

  case 497: /* expr_ls: query_expr  */
#line 1934 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9760 "bison-chapel.cpp"
    break;

  case 498: /* expr_ls: expr_ls TCOMMA expr  */
#line 1935 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9766 "bison-chapel.cpp"
    break;

  case 499: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 1936 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9772 "bison-chapel.cpp"
    break;

  case 500: /* simple_expr_ls: expr  */
#line 1940 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9778 "bison-chapel.cpp"
    break;

  case 501: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 1941 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9784 "bison-chapel.cpp"
    break;

  case 502: /* tuple_component: TUNDERSCORE  */
#line 1945 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9790 "bison-chapel.cpp"
    break;

  case 503: /* tuple_component: opt_try_expr  */
#line 1946 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9796 "bison-chapel.cpp"
    break;

  case 504: /* tuple_component: query_expr  */
#line 1947 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9802 "bison-chapel.cpp"
    break;

  case 505: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 1951 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9808 "bison-chapel.cpp"
    break;

  case 506: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 1952 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9814 "bison-chapel.cpp"
    break;

  case 507: /* opt_actual_ls: %empty  */
#line 1956 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9820 "bison-chapel.cpp"
    break;

  case 509: /* actual_ls: actual_expr  */
#line 1961 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9826 "bison-chapel.cpp"
    break;

  case 510: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 1962 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9832 "bison-chapel.cpp"
    break;

  case 511: /* actual_expr: ident_use TASSIGN query_expr  */
#line 1966 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9838 "bison-chapel.cpp"
    break;

  case 512: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 1967 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9844 "bison-chapel.cpp"
    break;

  case 513: /* actual_expr: query_expr  */
#line 1968 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9850 "bison-chapel.cpp"
    break;

  case 514: /* actual_expr: opt_try_expr  */
#line 1969 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9856 "bison-chapel.cpp"
    break;

  case 515: /* ident_expr: ident_use  */
#line 1973 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9862 "bison-chapel.cpp"
    break;

  case 516: /* ident_expr: scalar_type  */
#line 1974 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9868 "bison-chapel.cpp"
    break;

  case 517: /* type_level_expr: sub_type_level_expr  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9874 "bison-chapel.cpp"
    break;

  case 518: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 1988 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9880 "bison-chapel.cpp"
    break;

  case 519: /* type_level_expr: TQUESTION  */
#line 1990 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9886 "bison-chapel.cpp"
    break;

  case 525: /* sub_type_level_expr: TSINGLE expr  */
#line 2001 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9892 "bison-chapel.cpp"
    break;

  case 526: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2003 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9898 "bison-chapel.cpp"
    break;

  case 527: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2005 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9904 "bison-chapel.cpp"
    break;

  case 528: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2007 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9910 "bison-chapel.cpp"
    break;

  case 529: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2009 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pexpr)); }
#line 9916 "bison-chapel.cpp"
    break;

  case 530: /* sub_type_level_expr: TATOMIC expr  */
#line 2011 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9922 "bison-chapel.cpp"
    break;

  case 531: /* sub_type_level_expr: TSYNC expr  */
#line 2013 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9928 "bison-chapel.cpp"
    break;

  case 532: /* sub_type_level_expr: TOWNED  */
#line 2016 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9934 "bison-chapel.cpp"
    break;

  case 533: /* sub_type_level_expr: TOWNED expr  */
#line 2018 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9940 "bison-chapel.cpp"
    break;

  case 534: /* sub_type_level_expr: TUNMANAGED  */
#line 2020 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9946 "bison-chapel.cpp"
    break;

  case 535: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2022 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9952 "bison-chapel.cpp"
    break;

  case 536: /* sub_type_level_expr: TSHARED  */
#line 2024 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9958 "bison-chapel.cpp"
    break;

  case 537: /* sub_type_level_expr: TSHARED expr  */
#line 2026 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9964 "bison-chapel.cpp"
    break;

  case 538: /* sub_type_level_expr: TBORROWED  */
#line 2028 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9970 "bison-chapel.cpp"
    break;

  case 539: /* sub_type_level_expr: TBORROWED expr  */
#line 2030 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9976 "bison-chapel.cpp"
    break;

  case 540: /* sub_type_level_expr: TCLASS  */
#line 2033 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9982 "bison-chapel.cpp"
    break;

  case 541: /* sub_type_level_expr: TRECORD  */
#line 2035 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9988 "bison-chapel.cpp"
    break;

  case 542: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2040 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9994 "bison-chapel.cpp"
    break;

  case 543: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2042 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10000 "bison-chapel.cpp"
    break;

  case 544: /* for_expr: TFOR expr TDO expr  */
#line 2044 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10006 "bison-chapel.cpp"
    break;

  case 545: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2046 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10012 "bison-chapel.cpp"
    break;

  case 546: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2048 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10018 "bison-chapel.cpp"
    break;

  case 547: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2050 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10024 "bison-chapel.cpp"
    break;

  case 548: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2052 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10030 "bison-chapel.cpp"
    break;

  case 549: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2054 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10036 "bison-chapel.cpp"
    break;

  case 550: /* for_expr: TFORALL expr TDO expr  */
#line 2056 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10042 "bison-chapel.cpp"
    break;

  case 551: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2058 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10048 "bison-chapel.cpp"
    break;

  case 552: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2060 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10054 "bison-chapel.cpp"
    break;

  case 553: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2062 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10060 "bison-chapel.cpp"
    break;

  case 554: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2064 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 10071 "bison-chapel.cpp"
    break;

  case 555: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2071 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 10081 "bison-chapel.cpp"
    break;

  case 556: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2077 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10091 "bison-chapel.cpp"
    break;

  case 557: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2083 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10101 "bison-chapel.cpp"
    break;

  case 558: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2089 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10111 "bison-chapel.cpp"
    break;

  case 559: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2098 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10117 "bison-chapel.cpp"
    break;

  case 560: /* nil_expr: TNIL  */
#line 2107 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 10123 "bison-chapel.cpp"
    break;

  case 568: /* stmt_level_expr: io_expr TIO expr  */
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10129 "bison-chapel.cpp"
    break;

  case 569: /* opt_task_intent_ls: %empty  */
#line 2127 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 10135 "bison-chapel.cpp"
    break;

  case 571: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2132 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10141 "bison-chapel.cpp"
    break;

  case 572: /* task_intent_ls: intent_expr  */
#line 2136 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10147 "bison-chapel.cpp"
    break;

  case 573: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2137 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10153 "bison-chapel.cpp"
    break;

  case 574: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2141 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10159 "bison-chapel.cpp"
    break;

  case 575: /* forall_intent_ls: intent_expr  */
#line 2145 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10165 "bison-chapel.cpp"
    break;

  case 576: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2146 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10171 "bison-chapel.cpp"
    break;

  case 577: /* intent_expr: shadow_var_prefix ident_expr opt_type opt_init_expr  */
#line 2151 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10179 "bison-chapel.cpp"
    break;

  case 578: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2155 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10187 "bison-chapel.cpp"
    break;

  case 579: /* intent_expr: expr TREDUCE ident_expr  */
#line 2159 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10195 "bison-chapel.cpp"
    break;

  case 580: /* shadow_var_prefix: TCONST  */
#line 2165 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10201 "bison-chapel.cpp"
    break;

  case 581: /* shadow_var_prefix: TIN  */
#line 2166 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10207 "bison-chapel.cpp"
    break;

  case 582: /* shadow_var_prefix: TCONST TIN  */
#line 2167 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10213 "bison-chapel.cpp"
    break;

  case 583: /* shadow_var_prefix: TREF  */
#line 2168 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10219 "bison-chapel.cpp"
    break;

  case 584: /* shadow_var_prefix: TCONST TREF  */
#line 2169 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10225 "bison-chapel.cpp"
    break;

  case 585: /* shadow_var_prefix: TVAR  */
#line 2170 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10231 "bison-chapel.cpp"
    break;

  case 587: /* io_expr: io_expr TIO expr  */
#line 2176 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10237 "bison-chapel.cpp"
    break;

  case 588: /* new_maybe_decorated: TNEW  */
#line 2181 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10243 "bison-chapel.cpp"
    break;

  case 589: /* new_maybe_decorated: TNEW TOWNED  */
#line 2183 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10251 "bison-chapel.cpp"
    break;

  case 590: /* new_maybe_decorated: TNEW TSHARED  */
#line 2187 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10259 "bison-chapel.cpp"
    break;

  case 591: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2191 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10267 "bison-chapel.cpp"
    break;

  case 592: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2195 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10275 "bison-chapel.cpp"
    break;

  case 593: /* new_expr: new_maybe_decorated expr  */
#line 2203 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10282 "bison-chapel.cpp"
    break;

  case 594: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2208 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10292 "bison-chapel.cpp"
    break;

  case 595: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2214 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10302 "bison-chapel.cpp"
    break;

  case 596: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2220 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10313 "bison-chapel.cpp"
    break;

  case 597: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2227 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10324 "bison-chapel.cpp"
    break;

  case 598: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2237 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10330 "bison-chapel.cpp"
    break;

  case 608: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2254 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10336 "bison-chapel.cpp"
    break;

  case 609: /* expr: expr TCOLON expr  */
#line 2256 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10342 "bison-chapel.cpp"
    break;

  case 610: /* expr: expr TDOTDOT expr  */
#line 2258 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10348 "bison-chapel.cpp"
    break;

  case 611: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2260 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10354 "bison-chapel.cpp"
    break;

  case 612: /* expr: expr TDOTDOT  */
#line 2275 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10360 "bison-chapel.cpp"
    break;

  case 613: /* expr: TDOTDOT expr  */
#line 2277 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10366 "bison-chapel.cpp"
    break;

  case 614: /* expr: TDOTDOTOPENHIGH expr  */
#line 2279 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10372 "bison-chapel.cpp"
    break;

  case 615: /* expr: TDOTDOT  */
#line 2281 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10378 "bison-chapel.cpp"
    break;

  case 616: /* opt_expr: %empty  */
#line 2285 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10384 "bison-chapel.cpp"
    break;

  case 617: /* opt_expr: expr  */
#line 2286 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10390 "bison-chapel.cpp"
    break;

  case 618: /* opt_try_expr: TTRY expr  */
#line 2290 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10396 "bison-chapel.cpp"
    break;

  case 619: /* opt_try_expr: TTRYBANG expr  */
#line 2291 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10402 "bison-chapel.cpp"
    break;

  case 620: /* opt_try_expr: expr  */
#line 2292 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10408 "bison-chapel.cpp"
    break;

  case 625: /* call_base_expr: lhs_expr  */
#line 2308 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10414 "bison-chapel.cpp"
    break;

  case 626: /* call_base_expr: expr TBANG  */
#line 2309 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10420 "bison-chapel.cpp"
    break;

  case 627: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2310 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10426 "bison-chapel.cpp"
    break;

  case 628: /* call_base_expr: lambda_decl_expr  */
#line 2311 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10432 "bison-chapel.cpp"
    break;

  case 630: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2316 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10438 "bison-chapel.cpp"
    break;

  case 631: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2317 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10444 "bison-chapel.cpp"
    break;

  case 632: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2318 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10450 "bison-chapel.cpp"
    break;

  case 633: /* dot_expr: expr TDOT ident_use  */
#line 2322 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10456 "bison-chapel.cpp"
    break;

  case 634: /* dot_expr: expr TDOT TTYPE  */
#line 2323 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10462 "bison-chapel.cpp"
    break;

  case 635: /* dot_expr: expr TDOT TDOMAIN  */
#line 2324 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10468 "bison-chapel.cpp"
    break;

  case 636: /* dot_expr: expr TDOT TLOCALE  */
#line 2325 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10474 "bison-chapel.cpp"
    break;

  case 637: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2326 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10480 "bison-chapel.cpp"
    break;

  case 638: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2334 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10486 "bison-chapel.cpp"
    break;

  case 639: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2335 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10492 "bison-chapel.cpp"
    break;

  case 640: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2336 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10498 "bison-chapel.cpp"
    break;

  case 641: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2337 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10504 "bison-chapel.cpp"
    break;

  case 642: /* bool_literal: TFALSE  */
#line 2341 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10510 "bison-chapel.cpp"
    break;

  case 643: /* bool_literal: TTRUE  */
#line 2342 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10516 "bison-chapel.cpp"
    break;

  case 644: /* str_bytes_literal: STRINGLITERAL  */
#line 2346 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10522 "bison-chapel.cpp"
    break;

  case 645: /* str_bytes_literal: BYTESLITERAL  */
#line 2347 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10528 "bison-chapel.cpp"
    break;

  case 648: /* literal_expr: INTLITERAL  */
#line 2353 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10534 "bison-chapel.cpp"
    break;

  case 649: /* literal_expr: REALLITERAL  */
#line 2354 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10540 "bison-chapel.cpp"
    break;

  case 650: /* literal_expr: IMAGLITERAL  */
#line 2355 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10546 "bison-chapel.cpp"
    break;

  case 651: /* literal_expr: CSTRINGLITERAL  */
#line 2356 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10552 "bison-chapel.cpp"
    break;

  case 652: /* literal_expr: TNONE  */
#line 2357 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10558 "bison-chapel.cpp"
    break;

  case 653: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2358 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10565 "bison-chapel.cpp"
    break;

  case 654: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2360 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10572 "bison-chapel.cpp"
    break;

  case 655: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2362 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10578 "bison-chapel.cpp"
    break;

  case 656: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2363 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10584 "bison-chapel.cpp"
    break;

  case 657: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2365 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10592 "bison-chapel.cpp"
    break;

  case 658: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2369 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10600 "bison-chapel.cpp"
    break;

  case 659: /* assoc_expr_ls: expr TALIAS expr  */
#line 2376 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10606 "bison-chapel.cpp"
    break;

  case 660: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2377 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10612 "bison-chapel.cpp"
    break;

  case 661: /* binary_op_expr: expr TPLUS expr  */
#line 2381 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10618 "bison-chapel.cpp"
    break;

  case 662: /* binary_op_expr: expr TMINUS expr  */
#line 2382 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10624 "bison-chapel.cpp"
    break;

  case 663: /* binary_op_expr: expr TSTAR expr  */
#line 2383 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10630 "bison-chapel.cpp"
    break;

  case 664: /* binary_op_expr: expr TDIVIDE expr  */
#line 2384 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10636 "bison-chapel.cpp"
    break;

  case 665: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2385 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10642 "bison-chapel.cpp"
    break;

  case 666: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2386 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10648 "bison-chapel.cpp"
    break;

  case 667: /* binary_op_expr: expr TMOD expr  */
#line 2387 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10654 "bison-chapel.cpp"
    break;

  case 668: /* binary_op_expr: expr TEQUAL expr  */
#line 2388 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10660 "bison-chapel.cpp"
    break;

  case 669: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2389 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10666 "bison-chapel.cpp"
    break;

  case 670: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2390 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10672 "bison-chapel.cpp"
    break;

  case 671: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2391 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10678 "bison-chapel.cpp"
    break;

  case 672: /* binary_op_expr: expr TLESS expr  */
#line 2392 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10684 "bison-chapel.cpp"
    break;

  case 673: /* binary_op_expr: expr TGREATER expr  */
#line 2393 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10690 "bison-chapel.cpp"
    break;

  case 674: /* binary_op_expr: expr TBAND expr  */
#line 2394 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10696 "bison-chapel.cpp"
    break;

  case 675: /* binary_op_expr: expr TBOR expr  */
#line 2395 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10702 "bison-chapel.cpp"
    break;

  case 676: /* binary_op_expr: expr TBXOR expr  */
#line 2396 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10708 "bison-chapel.cpp"
    break;

  case 677: /* binary_op_expr: expr TAND expr  */
#line 2397 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10714 "bison-chapel.cpp"
    break;

  case 678: /* binary_op_expr: expr TOR expr  */
#line 2398 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10720 "bison-chapel.cpp"
    break;

  case 679: /* binary_op_expr: expr TEXP expr  */
#line 2399 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10726 "bison-chapel.cpp"
    break;

  case 680: /* binary_op_expr: expr TBY expr  */
#line 2400 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10732 "bison-chapel.cpp"
    break;

  case 681: /* binary_op_expr: expr TALIGN expr  */
#line 2401 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10738 "bison-chapel.cpp"
    break;

  case 682: /* binary_op_expr: expr THASH expr  */
#line 2402 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10744 "bison-chapel.cpp"
    break;

  case 683: /* binary_op_expr: expr TDMAPPED expr  */
#line 2403 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10751 "bison-chapel.cpp"
    break;

  case 684: /* unary_op_expr: TPLUS expr  */
#line 2408 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10757 "bison-chapel.cpp"
    break;

  case 685: /* unary_op_expr: TMINUS expr  */
#line 2409 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10763 "bison-chapel.cpp"
    break;

  case 686: /* unary_op_expr: TMINUSMINUS expr  */
#line 2410 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10769 "bison-chapel.cpp"
    break;

  case 687: /* unary_op_expr: TPLUSPLUS expr  */
#line 2411 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10775 "bison-chapel.cpp"
    break;

  case 688: /* unary_op_expr: TBANG expr  */
#line 2412 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10781 "bison-chapel.cpp"
    break;

  case 689: /* unary_op_expr: expr TBANG  */
#line 2413 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10787 "bison-chapel.cpp"
    break;

  case 690: /* unary_op_expr: TBNOT expr  */
#line 2414 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10793 "bison-chapel.cpp"
    break;

  case 691: /* reduce_expr: expr TREDUCE expr  */
#line 2418 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10799 "bison-chapel.cpp"
    break;

  case 692: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2419 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10805 "bison-chapel.cpp"
    break;

  case 693: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2420 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10811 "bison-chapel.cpp"
    break;

  case 694: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2421 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10817 "bison-chapel.cpp"
    break;

  case 695: /* scan_expr: expr TSCAN expr  */
#line 2425 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10823 "bison-chapel.cpp"
    break;

  case 696: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2426 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10829 "bison-chapel.cpp"
    break;

  case 697: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2427 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10835 "bison-chapel.cpp"
    break;

  case 698: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2428 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10841 "bison-chapel.cpp"
    break;

  case 699: /* reduce_scan_op_expr: TPLUS  */
#line 2433 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10847 "bison-chapel.cpp"
    break;

  case 700: /* reduce_scan_op_expr: TSTAR  */
#line 2434 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10853 "bison-chapel.cpp"
    break;

  case 701: /* reduce_scan_op_expr: TAND  */
#line 2435 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10859 "bison-chapel.cpp"
    break;

  case 702: /* reduce_scan_op_expr: TOR  */
#line 2436 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10865 "bison-chapel.cpp"
    break;

  case 703: /* reduce_scan_op_expr: TBAND  */
#line 2437 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10871 "bison-chapel.cpp"
    break;

  case 704: /* reduce_scan_op_expr: TBOR  */
#line 2438 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10877 "bison-chapel.cpp"
    break;

  case 705: /* reduce_scan_op_expr: TBXOR  */
#line 2439 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10883 "bison-chapel.cpp"
    break;


#line 10887 "bison-chapel.cpp"

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
