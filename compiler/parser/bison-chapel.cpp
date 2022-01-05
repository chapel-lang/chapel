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
  YYSYMBOL_manager_expr = 217,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 218,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 219,              /* manage_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 220, /* deprecated_class_level_stmt  */
  YYSYMBOL_class_level_stmt = 221,         /* class_level_stmt  */
  YYSYMBOL_222_2 = 222,                    /* @2  */
  YYSYMBOL_private_decl = 223,             /* private_decl  */
  YYSYMBOL_forwarding_stmt = 224,          /* forwarding_stmt  */
  YYSYMBOL_extern_export_decl_stmt = 225,  /* extern_export_decl_stmt  */
  YYSYMBOL_226_3 = 226,                    /* $@3  */
  YYSYMBOL_227_4 = 227,                    /* $@4  */
  YYSYMBOL_228_5 = 228,                    /* $@5  */
  YYSYMBOL_229_6 = 229,                    /* $@6  */
  YYSYMBOL_230_7 = 230,                    /* $@7  */
  YYSYMBOL_231_8 = 231,                    /* $@8  */
  YYSYMBOL_extern_block_stmt = 232,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 233,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 234,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 235,                  /* if_stmt  */
  YYSYMBOL_ifvar = 236,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 237,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 238,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 239,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 240,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 241, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 242,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 243,           /* ifc_constraint  */
  YYSYMBOL_defer_stmt = 244,               /* defer_stmt  */
  YYSYMBOL_try_stmt = 245,                 /* try_stmt  */
  YYSYMBOL_catch_stmt_ls = 246,            /* catch_stmt_ls  */
  YYSYMBOL_catch_stmt = 247,               /* catch_stmt  */
  YYSYMBOL_catch_expr = 248,               /* catch_expr  */
  YYSYMBOL_throw_stmt = 249,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 250,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 251,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 252,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 253,          /* class_decl_stmt  */
  YYSYMBOL_class_tag = 254,                /* class_tag  */
  YYSYMBOL_opt_inherit = 255,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 256,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 257,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 258,              /* enum_header  */
  YYSYMBOL_enum_ls = 259,                  /* enum_ls  */
  YYSYMBOL_deprecated_enum_item = 260,     /* deprecated_enum_item  */
  YYSYMBOL_enum_item = 261,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 262,         /* lambda_decl_expr  */
  YYSYMBOL_263_9 = 263,                    /* $@9  */
  YYSYMBOL_264_10 = 264,                   /* $@10  */
  YYSYMBOL_linkage_spec = 265,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt = 266,             /* fn_decl_stmt  */
  YYSYMBOL_267_11 = 267,                   /* $@11  */
  YYSYMBOL_268_12 = 268,                   /* $@12  */
  YYSYMBOL_fn_decl_stmt_inner = 269,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_receiver_expr = 270,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 271,                 /* fn_ident  */
  YYSYMBOL_op_ident = 272,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 273,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 274,              /* all_op_name  */
  YYSYMBOL_opt_formal_ls = 275,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 276,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 277,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 278,                /* formal_ls  */
  YYSYMBOL_formal = 279,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 280,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 281,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 282,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 283,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 284,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 285,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 286,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 287,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 288,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 289,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 290,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 291, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 292,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 293,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 294,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_inner = 295, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 296,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 297,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 298,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 299,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 300,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 301, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 302, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 303,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 304,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 305,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 306,                 /* opt_type  */
  YYSYMBOL_array_type = 307,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 308, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 309,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 310,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 311,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 312,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 313,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 314,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 315,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 316,                /* actual_ls  */
  YYSYMBOL_actual_expr = 317,              /* actual_expr  */
  YYSYMBOL_ident_expr = 318,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 319,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 320,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 321,                 /* for_expr  */
  YYSYMBOL_cond_expr = 322,                /* cond_expr  */
  YYSYMBOL_nil_expr = 323,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 324,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 325,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 326,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 327,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 328,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 329,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 330,              /* intent_expr  */
  YYSYMBOL_shadow_var_prefix = 331,        /* shadow_var_prefix  */
  YYSYMBOL_io_expr = 332,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 333,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 334,                 /* new_expr  */
  YYSYMBOL_let_expr = 335,                 /* let_expr  */
  YYSYMBOL_expr = 336,                     /* expr  */
  YYSYMBOL_opt_expr = 337,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 338,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 339,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 340,           /* call_base_expr  */
  YYSYMBOL_call_expr = 341,                /* call_expr  */
  YYSYMBOL_dot_expr = 342,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 343,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 344,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 345,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 346,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 347,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 348,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 349,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 350,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 351,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 352       /* reduce_scan_op_expr  */
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

#line 527 "bison-chapel.cpp"

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
#define YYLAST   21614

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  172
/* YYNRULES -- Number of rules.  */
#define YYNRULES  716
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1295

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
     689,   693,   697,   706,   711,   716,   721,   725,   729,   737,
     742,   746,   747,   748,   752,   753,   754,   755,   756,   757,
     758,   762,   767,   768,   774,   775,   779,   780,   784,   788,
     790,   792,   794,   796,   798,   805,   806,   810,   811,   812,
     813,   814,   815,   818,   819,   820,   821,   822,   823,   835,
     836,   847,   848,   849,   850,   851,   852,   853,   854,   855,
     856,   857,   858,   859,   860,   861,   862,   863,   864,   865,
     869,   870,   871,   872,   873,   874,   875,   876,   877,   878,
     879,   880,   887,   888,   889,   890,   894,   895,   899,   900,
     904,   905,   906,   910,   911,   915,   919,   920,   922,   927,
     928,   929,   939,   939,   944,   945,   946,   947,   948,   949,
     950,   954,   955,   956,   957,   962,   961,   978,   977,   995,
     994,  1011,  1010,  1028,  1027,  1043,  1042,  1058,  1062,  1067,
    1075,  1086,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,
    1111,  1112,  1113,  1114,  1115,  1116,  1122,  1128,  1134,  1140,
    1147,  1154,  1158,  1165,  1169,  1170,  1171,  1172,  1174,  1175,
    1176,  1177,  1179,  1181,  1183,  1185,  1190,  1191,  1195,  1197,
    1205,  1206,  1211,  1216,  1217,  1218,  1219,  1220,  1221,  1222,
    1223,  1224,  1225,  1226,  1227,  1228,  1235,  1236,  1237,  1238,
    1247,  1248,  1252,  1254,  1257,  1263,  1265,  1268,  1274,  1277,
    1278,  1279,  1280,  1281,  1282,  1286,  1287,  1291,  1292,  1293,
    1297,  1298,  1302,  1305,  1307,  1312,  1313,  1317,  1319,  1321,
    1328,  1338,  1352,  1357,  1362,  1370,  1371,  1376,  1377,  1379,
    1384,  1400,  1407,  1416,  1424,  1428,  1435,  1436,  1438,  1443,
    1444,  1449,  1454,  1448,  1481,  1484,  1488,  1496,  1506,  1495,
    1545,  1549,  1554,  1558,  1563,  1570,  1571,  1575,  1576,  1577,
    1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,
    1601,  1602,  1603,  1604,  1605,  1606,  1610,  1611,  1612,  1613,
    1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,  1625,  1626,
    1630,  1631,  1635,  1639,  1640,  1644,  1645,  1649,  1651,  1653,
    1655,  1657,  1659,  1664,  1665,  1669,  1670,  1671,  1672,  1673,
    1674,  1675,  1676,  1677,  1681,  1682,  1683,  1684,  1685,  1686,
    1690,  1691,  1692,  1696,  1697,  1698,  1699,  1700,  1701,  1705,
    1706,  1709,  1710,  1714,  1715,  1719,  1723,  1724,  1725,  1733,
    1734,  1736,  1738,  1740,  1745,  1747,  1752,  1753,  1754,  1755,
    1756,  1757,  1758,  1762,  1764,  1769,  1771,  1773,  1778,  1791,
    1808,  1809,  1811,  1816,  1817,  1818,  1819,  1820,  1824,  1830,
    1838,  1839,  1847,  1849,  1854,  1856,  1858,  1863,  1865,  1867,
    1874,  1875,  1876,  1881,  1883,  1885,  1889,  1893,  1895,  1899,
    1907,  1908,  1909,  1910,  1911,  1916,  1917,  1918,  1919,  1920,
    1940,  1944,  1948,  1956,  1963,  1964,  1965,  1969,  1971,  1977,
    1979,  1981,  1986,  1987,  1988,  1989,  1990,  1996,  1997,  1998,
    1999,  2003,  2004,  2008,  2009,  2010,  2014,  2015,  2019,  2020,
    2024,  2025,  2029,  2030,  2031,  2032,  2036,  2037,  2048,  2050,
    2052,  2058,  2059,  2060,  2061,  2062,  2063,  2065,  2067,  2069,
    2071,  2073,  2075,  2078,  2080,  2082,  2084,  2086,  2088,  2090,
    2092,  2095,  2097,  2102,  2104,  2106,  2108,  2110,  2112,  2114,
    2116,  2118,  2120,  2122,  2124,  2126,  2133,  2139,  2145,  2151,
    2160,  2170,  2178,  2179,  2180,  2181,  2182,  2183,  2184,  2185,
    2190,  2191,  2195,  2199,  2200,  2204,  2208,  2209,  2213,  2217,
    2221,  2228,  2229,  2230,  2231,  2232,  2233,  2237,  2238,  2243,
    2245,  2249,  2253,  2257,  2265,  2270,  2276,  2282,  2289,  2299,
    2307,  2308,  2309,  2310,  2311,  2312,  2313,  2314,  2315,  2316,
    2318,  2320,  2322,  2337,  2339,  2341,  2343,  2348,  2349,  2353,
    2354,  2355,  2359,  2360,  2361,  2362,  2371,  2372,  2373,  2374,
    2375,  2379,  2380,  2381,  2385,  2386,  2387,  2388,  2389,  2397,
    2398,  2399,  2400,  2404,  2405,  2409,  2410,  2414,  2415,  2416,
    2417,  2418,  2419,  2420,  2421,  2423,  2425,  2426,  2427,  2431,
    2439,  2440,  2444,  2445,  2446,  2447,  2448,  2449,  2450,  2451,
    2452,  2453,  2454,  2455,  2456,  2457,  2458,  2459,  2460,  2461,
    2462,  2463,  2464,  2465,  2466,  2471,  2472,  2473,  2474,  2475,
    2476,  2477,  2481,  2482,  2483,  2484,  2488,  2489,  2490,  2491,
    2496,  2497,  2498,  2499,  2500,  2501,  2502
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
  "use_renames_ls", "opt_only_ls", "use_access_control", "use_stmt",
  "import_stmt", "import_expr", "import_ls", "require_stmt",
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
  "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident", "op_ident",
  "assignop_ident", "all_op_name", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "var_arg_expr", "opt_lifetime_where",
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

#define YYPACT_NINF (-1143)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-717)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1143,   121,  3884, -1143,    67,   293, -1143, -1143, -1143, -1143,
   -1143, -1143,  5284,   240,   375,   353, 15458,   366, 21063,   240,
   12250,   382,   408,   364,   375,  5284, 12250,  1400,  5284,   320,
   21150, 12423,  8426,   445,  9120, 10685, 10685,  7555,  9293,   448,
   -1143,   381, -1143,   469, 21237, 21237, 21237, -1143,  3236, 10858,
     528, 12250, 12250,    35, -1143,   577,   596, 12250, -1143, 15458,
   -1143, 12250,   451,   509,   352,  3371,   628, 21324, -1143, 11033,
    8599, 12250, 10858, 15458, 12250,   590,   635,   522,  5284,   648,
   12250,   654, 12596, 12596, 21237,   666, -1143, 15458, -1143,   673,
    9293, 12250, -1143, 12250, -1143, 12250, -1143, -1143, 14973, 12250,
   -1143, 12250, -1143, -1143, -1143,  4234,  7730,  9468, 12250, -1143,
    5109, -1143, -1143, -1143,   562, -1143,   652, -1143, -1143,   216,
   -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143,
   -1143, -1143, -1143, -1143, -1143, -1143,   688, -1143, -1143, -1143,
   -1143, -1143, -1143, -1143, -1143, 21237, -1143, 21237,    -4,   195,
   -1143, -1143,  3236, -1143,   580, -1143,   583, -1143, -1143,   585,
     588,   592, 12250,   594,   595, 20377,  3678,   189,   597,   598,
   -1143, -1143,   341, -1143, -1143, -1143, -1143, -1143,   221, -1143,
   -1143, 20377,   579,  5284, -1143, -1143,   602, 12250, -1143, -1143,
   12250, 12250, 12250, 21237, -1143, 12250, 11033, 11033,   701,   468,
   -1143, -1143, -1143, -1143,   406,   491, -1143, -1143,   601, 17157,
   21237,  3236, -1143,   609, -1143,   156, 20377,  2183, -1143, -1143,
    9641,   363, 17643, -1143, -1143,   639,  8774,   692, 21411, 20377,
     593,    20, -1143, 21498, 21237, -1143,   593, 21237,   607,    17,
   16728,    32, 16688,    17, 16768,   204, -1143, 16920, 21237, 21237,
     125, 15928,   185,  8774, -1143, -1143, -1143, -1143, -1143, -1143,
   -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143,
   -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143,   610, -1143,
     510,  5284,   612,  2531,    71,     8, -1143,  5284, -1143, -1143,
   17197, -1143,    69, 17706,   793, -1143,   618,   620, -1143, 17197,
     406,   793, -1143,  8774, 16715, -1143, -1143, -1143,   297, 20377,
   12250, 12250, -1143, 20377,   629, 17813, -1143, 17197,   406, 20377,
     621,  8774, -1143, 20377, 17858, -1143, -1143, 17898,  2540, -1143,
   -1143, 18050,   672,   633,   406,    17, 17197, 18090,   443,   443,
    1106,   793,   793,   326, -1143, -1143,  4409,   126, -1143, 12250,
   -1143,   119,   130, -1143,   -30,    87, 18136,   -32,  1106,   792,
   -1143,  4584, -1143,   735, 12250, 12250, 21237,   658,   634, -1143,
   -1143, -1143, -1143,   356,   526, -1143, 12250,   657, 12250, 12250,
   12250, 10685, 10685, 12250,   533, 12250, 12250, 12250, 12250, 12250,
     553, 14973, 12250, 12250, 12250, 12250, 12250, 12250, 12250, 12250,
   12250, 12250, 12250, 12250, 12250, 12250, 12250, 12250,   737, -1143,
   -1143, -1143, -1143, -1143,  9814,  9814, -1143, -1143, -1143, -1143,
    9814, -1143, -1143,  9814,  9814,  8774,  8774, 10685, 10685,  8253,
   -1143, -1143, 17237, 17389, 18242,   636,    59, 21237,  4759, -1143,
   10685,    17,   642,   387, -1143, 12250, -1143, -1143, 12250,   686,
   -1143,   641,   667, -1143, -1143, -1143, 21237, -1143,  3236, -1143,
   -1143, 21237,   650, 21237, -1143,  3236,   770, 11033, -1143,  5459,
   10685, -1143,   647, -1143,    17,  5634, 10685, -1143,    17, -1143,
   10685, -1143,  7382,  7382, -1143,   695,   696,  5284,   791,  5284,
   -1143,   794, 12250, -1143, -1143,   652,   655,  8774, 21237, -1143,
   -1143,   237, -1143, -1143,  2531, -1143,   681,   662, -1143, 12769,
     713, 12250,  3236, -1143, -1143, 12250, -1143, 20788, 12250, 12250,
   -1143,   668, -1143, 11033, -1143, 20377, 20377, -1143,    30, -1143,
    8774,   671, -1143,   818, -1143,   818, -1143, 12942,   699, -1143,
   -1143, -1143, -1143, -1143, -1143, -1143,  9989, -1143, 18288,  7905,
   -1143,  8080, -1143,  5284,   675, 10685, 10164,  4059,   676, 12250,
   11206, -1143, -1143,   431, -1143,  4934, 21237, -1143,   392, 18328,
     394, 16960,   304, 11033,   683, 20701,   405, -1143, 18480, 20533,
   20533,   -23, -1143,   -23, -1143,   -23,  1838,  1264,   188,  1235,
     406,   443, -1143,   674, -1143, -1143, -1143, -1143, -1143,  1106,
    2664,   -23,  1631,  1631, 20533,  1631,  1631,   754,   443,  2664,
   18442,   754,   793,   793,   443,  1106,   690,   694,   698,   706,
     709,   711,   684,   703, -1143,   -23, -1143,   -23,    72, -1143,
   -1143, -1143,   134, -1143,  1645, 20453,   269, 13115, 10685, 13288,
   10685, 12250,  8774, 10685, 15740,   707,   240, 18525, -1143, -1143,
   -1143, 20377, 18565,  8774, -1143,  8774, 21237,   658,   395, 21237,
   21237,   658, -1143,   658,   403, 12250,   142,  9293, 20377,    55,
   17429,  8253, -1143,  9293, 20377,    36, 17005, -1143,    17, 17197,
   -1143, -1143, -1143, 12250,   504, 12250,   517,   524, -1143, -1143,
   -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143, 12250, -1143,
   -1143, 10337, -1143, -1143,   546, -1143,   416, -1143, -1143, -1143,
   18722,   734,   718, 12250, 12250,   852,  5284,   853, 18762,  5284,
   17469,   823, -1143,   177, -1143,   273, -1143,   203, -1143, -1143,
   -1143, -1143, -1143, -1143,   640,   745,   717, -1143,  3470, -1143,
     555,   719,  2531,    71,    26,    80, 12250, 12250,  7209, -1143,
   -1143,   691,  8947, -1143, 20377, -1143, -1143, -1143, 21237, 18802,
   18954, -1143, -1143, 20377,   722,    24,   723, -1143,  2935, -1143,
   -1143,   434, 21237, -1143, -1143, -1143, -1143, -1143, -1143, -1143,
    5284,   147, 17621, -1143, -1143, 20377,  5284, 20377, -1143, 18995,
   -1143, -1143, -1143, 12250, -1143,   101,  3631, 12250, -1143, 11379,
    7382,  7382, -1143,  8774,   757,  1281,   729, 20889,   779,   254,
   -1143, -1143,   815, -1143, -1143, -1143, -1143, 14805,   736, -1143,
   -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143,  8253,
   -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143,
   -1143, -1143,    97, 10685, 10685, 12250,   874, 19036, 12250,   875,
   19188,   274,   740, 19228,  8774,    17,    17, -1143, -1143, -1143,
   -1143,   658,   741, -1143,   658,   658,   742,   747, -1143, 17197,
   -1143, 16004,  5809, -1143,  5984, -1143,   277, -1143, 16080,  6159,
   -1143,    17,  6334, -1143,    17, -1143, -1143,  7382, -1143, 12250,
   -1143, 20377, 20377,  5284, -1143,  5284, 12250, -1143,  5284,   884,
   21237,   759, 21237,   601, -1143, -1143, 21237,   924, -1143,  2531,
     780,   839, -1143, -1143, -1143,   105, -1143, -1143,   713,   755,
      82, -1143, -1143, -1143,   760,   762, -1143,  6509, 11033, -1143,
   -1143, -1143, 21237, -1143,   790,   601, -1143, -1143,  6684,   761,
    6859,   768, -1143, 12250, -1143, -1143, -1143, -1143, -1143, -1143,
   -1143, -1143, -1143, -1143,  7382, -1143, 19273,    54, 17661,   446,
     772,   278,   773,   687, -1143, 21237, -1143, 12250, 20976, -1143,
   -1143,   555,   774,   423,   799,   800,   801,   824,   810,   819,
     820,   829,   825,   830,   831,   427,   844,   837,   838, 12250,
   -1143,   849,   850,   843,   774, -1143,   774, -1143, -1143, -1143,
     713,   288,   321, 19380, 13461, 13634, 19456, 13807, 13980, -1143,
   14153, 14326,   362, -1143, -1143,   827, -1143,   836,   842, -1143,
   -1143, -1143,  5284,  9293, 20377,  9293, 20377,  8253, -1143,  5284,
    9293, 20377, -1143,  9293, 20377, -1143, -1143, 19541, 20377, -1143,
   -1143, 20377,   959,  5284,   854, -1143, -1143, -1143,   780, -1143,
     828, 11554,   276, -1143,   225, -1143, -1143, 10685, 15599,  8774,
    8774,  5284, -1143,    78,   834, 12250, -1143,  9293, -1143, 20377,
    5284,  9293, -1143, 20377,  5284, 20377,   260, 11727,  7382,  7382,
    7382,  7382, -1143, -1143,   855, -1143,  1564, -1143, 16715, -1143,
    3047, -1143, -1143, -1143, 20377, -1143,   108,   237, -1143, 19617,
   -1143, 15845, -1143, -1143, -1143, 12250, 12250, 12250, 12250, 12250,
   12250, 12250, 12250, -1143, -1143,  1798, -1143, -1143,  2352,  3576,
   18762, 16156, 16232, -1143, 18762, 16308, 16384, 12250,  5284, -1143,
   -1143,   276,   780, 10512, -1143, -1143, -1143,   192, 11033, -1143,
   -1143,    52, 12250,   -28, 19693, -1143,   862,   856,   857,   639,
   -1143,   601, 20377, 16460, -1143, 16536, -1143, -1143, -1143, 20377,
     459,   858,   473,   863, -1143, 17380, -1143, -1143, -1143, 14499,
     917,   861, -1143,   898,   899,   774,   774, 19769, 19845, 19921,
   19997, 20073, 20149, 18061, -1143, 19192, 20396, -1143, -1143,  5284,
    5284,  5284,  5284, 20377, -1143, -1143, -1143,   276, 11902,    91,
   -1143, 20377, -1143,   115, -1143,    -6, -1143,   715, 20225, -1143,
   -1143, -1143, 14326,   876,   877, -1143,  5284,  5284, -1143, -1143,
   -1143, -1143, -1143,  7034, -1143, -1143,   169, -1143,   225, -1143,
   -1143, -1143, 12250, 12250, 12250, 12250, 12250, 12250, -1143, -1143,
   -1143, 18762, 18762, 18762, 18762, -1143, -1143, -1143, -1143, -1143,
     439, 10685, 15145, -1143, 12250,    52,   115,   115,   115,   115,
     115,   115,    52,   891, -1143, -1143, 18762, 18762,   867, 14672,
     109,   -19, 20301, -1143, -1143, 20377, -1143, -1143, -1143, -1143,
   -1143, -1143, -1143,   897, -1143, -1143,   335, 15317, -1143, -1143,
   -1143, 12077, -1143,   404, -1143
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   119,   659,   660,   661,   655,
     656,   662,     0,   580,   105,   140,   549,   147,   551,   580,
       0,   146,     0,   454,   105,     0,     0,   324,     0,   270,
     141,   627,   627,   653,     0,     0,     0,     0,     0,   145,
      60,   271,   325,   142,     0,     0,     0,   321,     0,     0,
     149,     0,     0,   599,   571,   663,   150,     0,   326,   543,
     453,     0,     0,     0,   172,   324,   144,   552,   455,     0,
       0,     0,     0,   547,     0,     0,   148,     0,     0,   120,
       0,   654,     0,     0,     0,   143,   304,   545,   457,   151,
       0,     0,   712,     0,   714,     0,   715,   716,   626,     0,
     713,   710,   530,   169,   711,     0,     0,     0,     0,     4,
       0,     5,     9,    43,     0,    46,    55,    10,    11,     0,
      12,    13,    14,    15,   526,   527,    25,    26,    47,   170,
     179,   180,    16,    20,    17,    19,     0,   265,    18,   618,
      22,    23,    24,    21,   178,     0,   176,     0,   615,     0,
     174,   177,     0,   175,   632,   611,   528,   612,   533,   531,
       0,     0,     0,   616,   617,     0,   532,     0,   633,   634,
     635,   657,   658,   610,   535,   534,   613,   614,     0,    42,
      28,   541,     0,     0,   581,   106,     0,     0,   551,   141,
       0,     0,     0,     0,   552,     0,     0,     0,     0,   615,
     632,   531,   616,   617,   550,   532,   633,   634,     0,   580,
       0,     0,   456,     0,   278,     0,   511,   324,   302,   312,
     627,   172,   324,   303,    45,     0,   518,   655,   552,   628,
     324,   655,   201,   552,     0,   189,   324,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
       0,     0,    57,   518,   113,   121,   133,   127,   126,   135,
     116,   125,   136,   122,   137,   114,   138,   131,   124,   132,
     130,   128,   129,   115,   117,   123,   134,   139,     0,   118,
       0,     0,     0,     0,     0,     0,   460,     0,   157,    36,
       0,   163,     0,   162,   697,   603,   600,   601,   602,     0,
     544,   698,     7,   518,   324,   171,   425,   508,     0,   507,
       0,     0,   158,   631,     0,     0,    39,     0,   548,   536,
       0,   518,    40,   542,     0,   285,   281,     0,   532,   285,
     282,     0,   450,     0,   546,     0,     0,     0,   699,   701,
     624,   696,   695,     0,    62,    65,     0,     0,   513,     0,
     515,     0,     0,   514,     0,     0,   507,     0,   625,     0,
       6,     0,    56,     0,     0,     0,     0,   305,     0,   411,
     412,   410,   327,     0,   529,    27,     0,   604,     0,     0,
       0,     0,     0,     0,   700,     0,     0,     0,     0,     0,
       0,   623,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,   373,
     374,   375,   370,   372,     0,     0,   368,   371,   369,   367,
       0,   377,   376,     0,     0,   518,   518,     0,     0,     0,
      29,    30,     0,     0,     0,     0,     0,     0,     0,    31,
       0,     0,     0,     0,    32,     0,    33,    44,     0,   526,
     524,     0,   519,   520,   525,   195,     0,   198,     0,   187,
     191,     0,     0,     0,   197,     0,     0,     0,   211,     0,
       0,   210,     0,   219,     0,     0,     0,   217,     0,   224,
       0,   223,     0,    79,   181,     0,     0,     0,   239,     0,
     366,   235,     0,    59,    58,    55,     0,     0,     0,   249,
      34,   393,   322,   464,     0,   465,   467,     0,   489,     0,
     470,     0,     0,   156,    35,     0,   165,     0,     0,     0,
      37,     0,   173,     0,    98,   629,   630,   159,     0,    38,
       0,     0,   292,   283,   279,   284,   280,     0,   448,   445,
     204,   203,    41,    64,    63,    66,     0,   664,     0,     0,
     649,     0,   651,     0,     0,     0,     0,     0,     0,     0,
       0,   668,     8,     0,    49,     0,     0,    96,     0,    92,
       0,    73,   276,     0,     0,     0,   404,   459,   579,   692,
     691,   694,   703,   702,   707,   706,   688,   685,   686,   687,
     620,   675,   119,     0,   646,   647,   120,   645,   644,   621,
     679,   690,   684,   682,   693,   683,   681,   673,   678,   680,
     689,   672,   676,   677,   674,   622,     0,     0,     0,     0,
       0,     0,     0,     0,   705,   704,   709,   708,   591,   592,
     594,   596,     0,   583,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   666,   276,   580,   580,   207,   446,
     458,   512,     0,     0,   538,     0,     0,   305,     0,     0,
       0,   305,   447,   305,     0,     0,     0,     0,   555,     0,
       0,     0,   220,     0,   561,     0,     0,   218,     0,     0,
     361,   359,   364,   358,   340,   343,   341,   342,   365,   353,
     344,   357,   349,   347,   360,   363,   348,   346,   351,   356,
     345,   350,   354,   355,   352,   362,     0,   378,   379,    68,
      67,    80,     0,     0,     0,   238,     0,   234,     0,     0,
       0,     0,   537,     0,   252,     0,   250,   398,   395,   396,
     397,   401,   402,   403,   393,   386,     0,   383,     0,   394,
     413,     0,   468,     0,   154,   155,   153,   152,     0,   488,
     487,   611,     0,   462,   609,   461,   164,   161,     0,     0,
       0,   643,   510,   509,     0,     0,     0,   539,     0,   286,
     452,   611,     0,   665,   619,   650,   516,   652,   517,   231,
       0,     0,     0,   667,   229,   565,     0,   670,   669,     0,
      51,    50,    48,     0,    91,     0,     0,     0,    84,     0,
       0,    79,   273,     0,   306,     0,     0,     0,   319,     0,
     313,   316,   408,   405,   406,   409,   328,     0,     0,   104,
     102,   103,   101,   100,    99,   641,   642,   593,   595,     0,
     582,   140,   147,   146,   145,   142,   149,   150,   144,   148,
     143,   151,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   202,   522,   523,
     521,   305,     0,   200,   305,   305,     0,     0,   199,     0,
     233,     0,     0,   209,     0,   208,     0,   586,     0,     0,
     215,     0,     0,   213,     0,   222,   221,     0,   182,     0,
     183,   247,   246,     0,   241,     0,     0,   237,     0,   243,
       0,   275,     0,     0,   399,   400,     0,   393,   382,     0,
     502,   414,   417,   416,   418,     0,   466,   469,   470,     0,
       0,   471,   472,   160,     0,     0,   294,     0,     0,   293,
     296,   540,     0,   287,   290,     0,   449,   232,     0,     0,
       0,     0,   230,     0,    97,    94,   358,   340,   343,   341,
     342,   351,   350,   352,     0,    93,    76,    75,    74,     0,
       0,     0,     0,   324,   311,     0,   318,     0,   314,   310,
     407,   413,   380,   107,   121,   127,   126,   110,   125,   122,
     137,   108,   138,   124,   128,   109,   111,   123,   139,     0,
     337,     0,   112,     0,   380,   339,   380,   335,   648,   584,
     470,   632,   632,     0,     0,     0,     0,     0,     0,   275,
       0,     0,     0,   206,   205,     0,   307,     0,     0,   307,
     307,   212,     0,     0,   554,     0,   553,     0,   585,     0,
       0,   560,   216,     0,   559,   214,    71,    70,    69,   240,
     236,   570,   242,     0,     0,   272,   251,   248,   502,   384,
       0,     0,   470,   415,   429,   463,   493,     0,   666,   518,
     518,     0,   298,     0,     0,     0,   288,     0,   227,   567,
       0,     0,   225,   566,     0,   671,     0,     0,     0,    79,
       0,    79,    85,    88,   277,   301,   324,   172,   324,   300,
     324,   308,   166,   317,   320,   315,     0,   393,   334,     0,
     338,     0,   330,   331,   588,     0,     0,     0,     0,     0,
       0,     0,     0,   277,   307,   324,   307,   307,   324,   324,
     558,     0,     0,   587,   564,     0,     0,     0,     0,   245,
      61,   470,   502,     0,   505,   504,   506,   611,   426,   389,
     387,     0,     0,     0,     0,   491,   611,     0,     0,   299,
     297,     0,   291,     0,   228,     0,   226,    95,    78,    77,
       0,     0,     0,     0,   274,   324,   168,   309,   484,     0,
     419,     0,   336,   107,   109,   380,   380,     0,     0,     0,
       0,     0,     0,   324,   194,   324,   324,   186,   190,     0,
       0,     0,     0,    72,   244,   390,   388,   470,   494,     0,
     428,   427,   443,     0,   444,   431,   434,     0,   430,   423,
     424,   323,     0,   605,   606,   289,     0,     0,    87,    90,
      86,    89,   167,     0,   483,   482,   611,   420,   429,   381,
     332,   333,     0,     0,     0,     0,     0,     0,   196,   188,
     192,   557,   556,   563,   562,   392,   391,   496,   497,   499,
     611,     0,   666,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   611,   607,   608,   569,   568,     0,   474,
       0,     0,     0,   498,   500,   433,   435,   436,   439,   440,
     441,   437,   438,   432,   479,   477,   611,   666,   421,   329,
     422,   494,   478,   611,   501
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1143, -1143, -1143,     5,    11,  2808, -1143,     2, -1143, -1143,
   -1143,   549, -1143, -1143, -1143,   407,   689,  -480, -1143,  -773,
   -1143, -1143, -1143,   258, -1143, -1143,   586,  1030, -1143,  2653,
    -100,  -805, -1143,  -988,   287, -1095,   542, -1143, -1143,   -27,
    -927, -1143,   -64, -1143, -1143, -1143, -1143, -1143, -1143, -1143,
   -1143, -1143, -1143,   138, -1143,   968, -1143, -1143,   158,  1236,
   -1143, -1143, -1143, -1143, -1143,   733, -1143,   139, -1143, -1143,
   -1143, -1143, -1143, -1143,  -560,  -675, -1143, -1143, -1143,   104,
    -772,  1258, -1143, -1143, -1143,   424, -1143, -1143, -1143, -1143,
     -22,  -790,  -151,  -722,  -964, -1143, -1143,   -24,   167,   344,
   -1143, -1143, -1143,   110, -1143, -1143,  -191,   -63,  -937,  -144,
    -175,  -167,  -582, -1143,  -200, -1143,    -1,  1052,  -134,   589,
   -1143,  -488,  -885,  -987, -1143,  -705,  -534, -1142, -1114,  -972,
     -15, -1143,   166, -1143,  -236,  -456,  -484,   744,  -457, -1143,
   -1143, -1143,  1470, -1143,   -10, -1143, -1143,  -231, -1143,  -620,
   -1143, -1143, -1143,  1836,  2001,   -12,  1060,    58,  1024, -1143,
    2302,  2507, -1143, -1143, -1143, -1143, -1143, -1143, -1143, -1143,
   -1143,  -424
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   345,  1090,   717,   112,   113,   114,   115,
     116,   363,   495,   117,   252,   118,   346,   711,   570,   712,
     119,   120,   121,   567,   568,   122,   123,   186,   990,   284,
     124,   279,   125,   749,   289,   126,   291,   292,   127,  1091,
     128,   304,   129,   130,   131,   461,   659,   463,   660,   456,
     656,   132,   133,   852,   134,   250,   135,   725,   726,   198,
     137,   138,   139,   140,   141,   533,   769,   935,   142,   143,
     765,   930,   144,   145,   574,   963,   146,   147,   809,   810,
     811,   199,   282,   740,   149,   150,   576,   971,   816,   993,
     994,   707,   708,   709,  1098,   502,   735,   736,   737,   738,
     739,   817,   372,   915,  1228,  1289,  1211,   450,  1139,  1143,
    1205,  1206,  1207,   151,   333,   538,   152,   153,   285,   286,
     506,   507,   753,  1225,  1170,   510,   750,  1248,  1136,  1052,
     347,   215,   351,   352,   451,   452,   453,   200,   155,   156,
     157,   158,   201,   160,   183,   184,   632,   474,   876,   633,
     634,   161,   162,   202,   203,   165,   236,   454,   205,   167,
     206,   207,   170,   171,   172,   173,   357,   174,   175,   176,
     177,   178
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     181,   305,   706,   770,   204,   636,   307,   109,   209,   208,
     442,   478,   992,   110,   216,   424,   741,   496,   373,   229,
     229,   211,   240,   242,   244,   247,   251,   995,   960,   224,
    1102,   764,  1103,  1055,   462,   966,  1092,   290,   918,   293,
     294,   723,   307,   350,   307,   299,   766,   300,  1210,   301,
     287,   877,   751,   295,   308,  1202,   511,   309,   313,   315,
     317,   318,   319,  1134,    70,   287,   323,   521,   324,   879,
     327,   331,   508,    70,   955,   334,  1131,   443,   336,   337,
     771,   338,  -270,   339,  1249,   531,   340,   341,   872,   342,
     472,  1078,   355,   309,   313,   356,   358,   862,   508,   927,
     492,   866,   287,   867,   592,  -295,   508,   643,   459,  1168,
    1273,   287,  1254,   296,   560,  1104,   110,   384,  1202,  -485,
     827,     3,   388,   554,   558,   390,   449,  1079,   314,   393,
    1057,   297,   460,   307,   307,   555,  -271,  1000,  1274,  1251,
    1255,   928,   438,   561,  1203,   553,  1288,  -295,   298,  1294,
     377,   438,   472,   449,   512,  1204,   472,   643,   305,  1166,
    1197,  -576,   828,  1092,   353,  1036,  -485,  1140,  -639,  -480,
    -639,   860,   239,   241,   243,   181,  1210,   424,   432,   433,
     434,  1224,   436,   323,   309,   356,   851,   438,  1092,   622,
     623,  1092,  1092,  1093,  1195,   929,  -485,  -485,   226,   441,
    -480,  -295,   438,   449,   596,   556,   438,   472,   229,   999,
    1054,  1230,  1231,  -480,   313,   515,   509,  -485,  1204,   447,
     380,   449,  -485,  -485,   523,   438,  -480,   487,   556,   458,
    -481,    23,   179,   556,   644,   465,  -485,   556,  1222,   438,
     522,   313,   509,  -485,   211,   354,  1196,   636,   438,   369,
     509,   904,   253,  1169,   917,   556,  1092,  1058,  1092,  1092,
    1245,  -481,   557,   364,   727,   549,  1252,   472,   493,   370,
    -485,   494,   546,  -480,  -481,  -485,   551,   381,  -480,   371,
     829,   382,  1285,    60,  1287,   728,  1141,  -481,   523,   729,
     598,   313,   550,   905,    68,   438,   992,   547,   525,   526,
    1292,  1015,   445,   552,  1017,  1018,  1161,   830,  1163,   313,
     427,   995,  1246,   730,   428,   870,   731,  -503,    62,    88,
     959,   446,   938,   655,   658,   449,   449,   732,   384,   385,
     365,   664,   387,   388,  -481,   389,   390,   548,  -503,  -481,
     393,  1115,  -503,  1142,  1118,  1119,   733,   961,   400,  -253,
     901,   545,   569,   571,   404,   405,   406,   110,   844,   316,
     182,   425,   428,   426,   578,  -503,   579,   580,   581,   583,
     585,   586,   110,   587,   588,   589,   590,   591,   185,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   614,   615,  -473,   449,  1012,   -83,
     968,   752,   313,   313,   307,   636,   887,  1123,   313,  -254,
    -385,   313,   313,   313,   313,   625,   627,   635,   -54,   902,
     655,  1050,  -260,  1027,   655,   969,  1138,  -473,   647,   -54,
     449,  1157,   812,   651,  -590,    23,   652,   -54,  -259,  1183,
    -473,  1185,  1186,   523,   881,   884,   903,  1009,   -54,   110,
    1028,  1084,   666,  -473,   212,   309,   288,   668,   670,   302,
     762,  -590,   524,   674,   676,  -475,   -83,  -589,   679,   802,
     710,   710,   617,   618,  1076,   380,   803,   718,   619,   288,
     720,   620,   621,   762,   813,   313,   350,    60,   350,   325,
     329,   179,   226,   762,  -589,   814,  -475,   543,    68,   754,
    -473,  -268,   512,   293,  -258,  -473,   759,   760,   655,  -475,
     307,   763,   734,  -640,   815,  -640,   758,   210,   313,   582,
     584,   577,  -475,    88,  1145,  -255,   468,   471,   473,   477,
     479,   481,   381,   512,   763,  1113,   382,   313,   793,   313,
     797,   512,   449,   782,   763,   785,   384,   787,   789,   512,
     939,   941,   650,   253,   390,   449,   592,   794,   804,   798,
     863,   309,   887,  -113,  -495,   624,   626,  -115,   868,  -475,
     545,  -119,   936,   593,  -475,  -120,   110,   514,   646,   516,
    -451,   888,   911,   384,  -262,  -495,   520,   594,   388,  -495,
     858,   390,   887,  -714,  1137,   393,   179,  -714,  1160,  -451,
    1162,  1146,   790,   636,   529,   887,  -715,   353,   669,   353,
    -715,  1082,  -495,  -716,   675,   439,   595,  -716,   678,   887,
      23,  1253,   540,   541,  1218,   668,   847,   674,   850,   718,
     313,   853,   785,  -266,   912,  -711,   855,   856,  1220,  -711,
    -639,   313,  -639,   313,    42,   913,   288,   288,   288,   288,
     288,   288,  -263,   869,   457,   871,   596,   488,   491,   635,
     464,   878,   597,  -636,   914,  -636,   996,   727,   326,   330,
      58,   338,    60,   339,  1277,  1278,  1279,  1280,  1281,  1282,
     438,   303,   498,    68,  -257,   307,   341,   499,   728,   342,
     320,  -261,   729,   781,   321,   945,   598,   288,  -638,   288,
    -638,   891,   892,   449,  -267,  -637,   288,  -637,    88,   218,
    -269,   859,  1226,    22,    23,   776,   730,   778,  1086,   731,
     492,   359,  -256,   219,   288,    31,   220,   492,   648,  -264,
     732,    37,   361,   920,   319,   323,   356,   362,    42,  -486,
     313,  1250,   288,   288,   366,  -573,   154,   374,   376,   733,
    -572,   429,  -486,   375,   449,  1263,   154,   437,   448,  -577,
    -578,   672,  -575,  -574,    58,   677,    60,   431,    62,   154,
    1087,   438,   154,  1088,   444,   223,   846,    68,   849,   467,
     455,   569,   497,  -486,   501,   956,   380,   958,   710,   710,
     518,   313,   519,   530,   527,  1250,    84,   537,   539,    86,
     562,   566,    88,   573,   575,   390,   616,   649,   642,  -486,
     922,   653,  1286,   655,   654,   662,  -486,   635,   665,   671,
     713,   714,   154,  1147,  1148,   380,   716,   742,   722,   719,
    1293,   583,   625,  1003,  1250,   743,  1006,  -486,   752,   768,
    1256,   761,   313,   381,   767,   772,   818,   382,   288,   154,
     780,   786,   103,   805,   154,   819,  -486,   825,  1089,   820,
    1024,  -486,  1026,   821,  -486,   307,  1257,  1031,  1258,  1259,
    1034,   822,  1260,  1261,   823,  1037,   824,  1038,   826,   854,
     887,   288,   381,   890,  1041,   288,   382,   893,   895,   900,
     908,   907,   916,   926,   384,   385,   931,   386,   387,   388,
     964,   389,   390,   523,   967,   970,   393,  1004,  1007,   998,
    -490,  1016,  1019,  1063,   400,  1010,   309,  1020,   734,  1043,
     404,   405,   406,  -490,  1045,  1051,  1069,   154,  1073,  1053,
    1056,  1075,  1059,   384,  1060,  1065,  1070,  1083,   388,  -492,
     389,   390,   710,  1074,  1085,   393,  1097,  -140,  -147,  -146,
    1176,   727,  -492,   400,  -490,  1094,   873,   875,  -145,   449,
     449,   406,   880,   883,  -116,   885,   886,  -142,  -149,  -114,
     492,   492,   728,  -150,   492,   492,   729,  1099,  -144,  -148,
    -490,   582,   624,  -492,  -117,  -143,  -151,  -490,  1135,  1100,
    -118,  1101,  1024,  1026,  1128,  1031,  1034,  1114,  1069,  1073,
     730,  1132,   492,   731,   492,    62,  1116,  1151,  -490,  -492,
    1120,  1121,  1117,  1122,   732,   635,  -492,  1124,  1125,  1130,
    1164,  1126,  1227,  1219,   305,   154,   166,  -490,  1221,  1213,
    1214,   154,  -490,   733,  1229,  -490,   166,  -492,  -113,  -115,
    1264,  1265,  1284,  1255,   721,  1144,   785,   313,   313,   166,
     565,   944,   166,  1152,   213,  1153,  -492,   756,   335,  1155,
    1046,  -492,   535,  1167,  -492,  1159,   710,   710,   710,   710,
     307,  1064,  1095,  1171,  1049,  1200,   288,   288,   906,  1175,
    1290,  1096,   288,   288,  1271,   288,   288,  1283,  1276,   246,
     154,   230,     0,  1120,  1177,  1178,  1124,  1179,  1180,  1181,
    1182,   755,   166,     0,     0,   154,   328,   328,   734,     0,
       0,     0,     0,     0,     0,  1193,     0,     0,  1199,     0,
       0,   356,     0,     0,     0,     0,  1201,   899,     0,   166,
    1208,     0,     0,     0,   166,  1247,     0,     0,   380,     0,
       0,     0,  1013,  1014,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1021,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,  1032,     0,
       0,  1035,     0,     0,     0,   933,     0,  1241,  1242,  1243,
    1244,     0,   154,     0,     0,     0,     0,     0,     0,  1247,
       0,     0,     0,     0,     0,   381,     0,     0,     0,   382,
    1073,     0,     0,     0,  1266,  1267,     0,   166,  1270,     0,
       0,   356,     0,   154,     0,     0,     0,     0,     0,   154,
    1241,  1242,  1243,  1244,  1266,  1267,     0,     0,  1247,     0,
       0,   154,     0,   154,     0,     0,     0,     0,   136,  1272,
     785,     0,  1275,     0,     0,     0,   384,   385,   136,   386,
     387,   388,     0,   389,   390,     0,     0,     0,   393,     0,
     148,   136,   288,   288,   136,   399,   400,   380,     0,   403,
     148,     0,   404,   405,   406,   785,   288,     0,   491,  1073,
       0,     0,   962,   148,     0,   491,   148,     0,   288,     0,
       0,   288,     0,     0,     0,     0,   380,   154,     0,     0,
       0,   154,     0,  -307,     0,   166,     0,  -307,  -307,   154,
    1047,   166,  -307,     0,   136,     0,     0,  -307,     0,  -307,
    -307,     0,     0,     0,   381,  -307,     0,     0,   382,     0,
       0,     0,  -307,     0,     0,  -307,   148,     0,     0,     0,
       0,   136,  1066,     0,     0,     0,   136,     0,     0,     0,
    1150,     0,     0,   381,     0,  -307,     0,   382,  -307,     0,
    -307,     0,  -307,   148,  -307,  -307,     0,  -307,   148,  -307,
     166,  -307,     0,     0,     0,   384,   385,     0,   842,     0,
     388,     0,   389,   390,     0,   166,     0,   393,     0,     0,
    -307,     0,     0,  -307,     0,   400,  -307,     0,     0,     0,
       0,   404,   405,   406,   384,     0,     0,     0,   217,   388,
       0,   389,   390,     0,     0,     0,   393,     0,     0,   136,
       0,     0,   218,     0,   400,     0,    22,    23,     0,     0,
     404,   405,   406,     0,     0,     0,   219,     0,    31,   220,
       0,   148,     0,     0,    37,     0,  -307,     0,     0,     0,
       0,    42,  -307,     0,     0,     0,     0,     0,     0,     0,
     154,     0,   166,   154,     0,     0,   -52,     0,     0,     0,
     288,     0,   159,     0,     0,     0,     0,    58,     0,    60,
       0,     0,   159,   221,     0,   -52,   222,     0,   223,     0,
      68,     0,     0,   166,     0,   159,     0,     0,   159,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       0,   166,    86,   166,     0,    88,     0,   136,     0,     0,
       0,     0,     0,   136,   154,     0,     0,     0,   491,   491,
     154,     0,   491,   491,     0,     0,     0,     0,     0,   148,
       0,     0,     0,     0,     0,   148,     0,     0,   159,     0,
    1209,     0,     0,     0,     0,     0,     0,     0,  1215,     0,
     491,   997,   491,     0,     0,   103,     0,     0,     0,     0,
       0,     0,  1165,     0,     0,   159,     0,   166,     0,     0,
     159,   166,   136,     0,     0,     0,   218,  1001,  1002,   166,
      22,    23,     0,     0,     0,     0,     0,   136,     0,     0,
     219,     0,    31,   220,   148,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    42,   154,     0,   154,   148,
       0,     0,     0,   154,     0,     0,   154,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   154,     0,   154,
       0,    58,   154,    60,     0,     0,     0,  1087,   592,     0,
    1088,     0,   223,   159,    68,     0,     0,     0,     0,     0,
       0,     0,   831,   380,     0,   832,     0,     0,     0,     0,
     833,   154,     0,    84,   136,     0,    86,     0,  1209,    88,
       0,   189,   154,     0,   154,     0,     0,     0,     0,     0,
       0,   834,     0,     0,     0,     0,   148,     0,   835,     0,
       0,     0,     0,     0,     0,   136,     0,     0,   836,     0,
       0,   136,     0,     0,     0,     0,   837,     0,     0,     0,
     381,     0,     0,   136,   382,   136,     0,   148,     0,   103,
       0,     0,   838,   148,     0,     0,     0,     0,     0,     0,
     166,     0,     0,   166,   839,   148,     0,   148,   596,     0,
       0,   159,     0,     0,     0,   840,     0,   159,     0,     0,
       0,   841,     0,     0,     0,     0,   154,     0,     0,     0,
       0,   384,   385,   154,   386,   387,   388,     0,   389,   390,
     391,     0,     0,   393,     0,     0,     0,   154,     0,   136,
     399,   400,     0,   136,   403,     0,     0,   404,   405,   406,
       0,   136,     0,     0,   166,   154,     0,     0,   407,     0,
     166,   148,     0,     0,   154,   148,   159,     0,   154,     0,
     218,     0,     0,   148,    22,    23,     0,     0,     0,  1086,
       0,   159,     0,     0,   219,     0,    31,   220,   163,     0,
       0,     0,    37,     0,     0,     0,     0,     0,   163,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,     0,     0,   163,     0,     0,     0,     0,     0,
     380,     0,   154,     0,     0,    58,     0,    60,     0,    62,
       0,  1087,     0,     0,  1088,     0,   223,     0,    68,     0,
       0,     0,     0,     0,     0,     0,   166,     0,   166,     0,
       0,     0,     0,   166,     0,     0,   166,    84,   159,     0,
      86,     0,     0,    88,   163,     0,     0,   166,     0,   166,
       0,     0,   166,     0,     0,     0,     0,   381,     0,     0,
       0,   382,     0,   154,   154,   154,   154,     0,     0,   159,
       0,   163,     0,     0,     0,   159,   163,     0,     0,     0,
       0,   166,   136,     0,     0,   136,     0,   159,     0,   159,
     154,   154,   166,   103,   166,     0,     0,     0,     0,  1184,
       0,     0,     0,     0,   148,     0,     0,   148,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,     0,     0,   397,   398,   399,   400,   401,
       0,   403,     0,   164,   404,   405,   406,     0,     0,     0,
       0,     0,     0,   164,     0,   407,   136,     0,     0,   163,
       0,     0,   136,   159,     0,     0,   164,   159,     0,   164,
       0,     0,     0,     0,     0,   159,     0,     0,   148,     0,
       0,     0,     0,     0,   148,     0,   166,     0,     0,     0,
       0,     0,     0,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   164,
       0,     0,     0,     0,     0,   166,     0,     0,     0,     0,
       0,     0,     0,     0,   166,     0,     0,     0,   166,     0,
       0,     0,     0,     0,     0,     0,   164,     0,   136,     0,
     136,   164,     0,     0,     0,   136,     0,   163,   136,     0,
       0,     0,     0,   163,     0,     0,     0,     0,     0,   136,
     148,   136,   148,     0,   136,     0,     0,   148,     0,     0,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   148,   166,   148,     0,     0,   148,     0,     0,     0,
       0,     0,     0,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   136,     0,   136,     0,     0,     0,
       0,     0,   163,     0,   164,   148,   159,     0,     0,   159,
       0,     0,     0,     0,     0,     0,   148,   163,   148,     0,
       0,     0,     0,     0,     0,   218,     0,     0,     0,    22,
      23,     0,     0,   166,   166,   166,   166,     0,     0,   219,
       0,    31,   220,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
     166,   166,     0,     0,     0,     0,     0,     0,     0,   -52,
     159,     0,     0,     0,     0,     0,   159,     0,   136,     0,
      58,     0,    60,     0,     0,   136,   221,     0,   -52,   222,
       0,   223,     0,    68,   163,     0,     0,     0,     0,   136,
     148,     0,   164,     0,     0,     0,     0,   148,   164,     0,
       0,     0,    84,     0,     0,    86,     0,   136,    88,     0,
       0,   148,     0,     0,   168,   163,   136,     0,     0,     0,
     136,   163,     0,     0,   168,     0,     0,     0,     0,   148,
       0,     0,     0,   163,     0,   163,     0,   168,   148,     0,
     168,     0,   148,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   159,     0,   159,     0,     0,   164,   103,   159,
       0,     0,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   164,   159,   136,   159,     0,     0,   159,     0,
       0,     0,     0,     0,   218,     0,     0,     0,    22,    23,
     168,     0,     0,  1086,     0,     0,   148,     0,   219,   163,
      31,   220,     0,   163,     0,     0,    37,   159,     0,     0,
       0,   163,     0,    42,     0,     0,     0,   168,   159,     0,
     159,     0,   168,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,   136,   136,   136,    58,
       0,    60,     0,    62,     0,  1087,     0,     0,  1088,   164,
     223,     0,    68,     0,     0,     0,     0,   148,   148,   148,
     148,     0,   136,   136,     0,     0,     0,     0,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
     164,     0,     0,     0,   148,   148,   164,     0,     0,     0,
       0,     0,     0,     0,     0,   168,     0,     0,   164,     0,
     164,     0,   159,     0,     0,     0,     0,     0,     0,   159,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,   159,     0,     0,     0,   103,     0,   169,
       0,     0,     0,  1187,     0,     0,     0,     0,     0,     0,
       0,   159,   169,     0,   254,   169,     0,     0,     0,     0,
     159,     0,     0,     0,   159,     0,     0,     0,   255,   256,
       0,   257,   163,     0,   164,   163,   258,     0,   164,     0,
       0,     0,     0,     0,     0,   259,   164,     0,     0,     0,
       0,   260,     0,     0,     0,     0,     0,   261,     0,     0,
       0,   262,     0,   168,   263,   169,     0,     0,     0,   168,
       0,     0,     0,     0,   264,     0,     0,     0,   159,     0,
       0,   265,   266,     0,     0,     0,     0,     0,     0,   267,
       0,     0,   169,     0,     0,     0,   163,   169,   268,     0,
       0,     0,   163,     0,     0,     0,     0,   269,   270,     0,
     271,     0,   272,     0,   273,     0,     0,   274,     0,     0,
       0,   275,   503,     0,   276,     0,     0,   277,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   159,
     159,   159,   159,   168,     0,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
       0,     0,     0,     0,     0,     0,   159,   159,     0,     0,
     169,     0,     0,     0,     0,     0,   380,   278,   280,   281,
       0,     0,     0,   504,     0,     0,     0,     0,   163,   423,
     163,     0,  -636,     0,  -636,   163,     0,   164,   163,     0,
     164,     0,     0,     0,     0,     0,     0,     0,     0,   163,
       0,   163,     0,     0,   163,     0,     0,   332,     0,     0,
     168,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,     0,     0,     0,   382,     0,     0,
       0,     0,     0,   163,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,   163,     0,   163,   168,     0,     0,
       0,   164,     0,     0,     0,     0,     0,   164,   169,   168,
       0,   168,     0,     0,   169,     0,     0,     0,   367,     0,
     368,     0,     0,     0,   384,   385,     0,   386,   387,   388,
     111,   389,   390,   391,     0,     0,   393,   394,   395,     0,
     180,   397,   398,   399,   400,     0,     0,   403,     0,     0,
     404,   405,   406,   214,     0,     0,   225,     0,     0,     0,
       0,   407,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,   169,     0,   168,     0,     0,   163,   168,
       0,     0,     0,   332,     0,   163,     0,   168,   169,     0,
       0,     0,     0,   164,     0,   164,     0,     0,     0,   163,
     164,     0,     0,   164,     0,     0,   322,   332,     0,     0,
     466,     0,     0,     0,   164,     0,   164,   163,     0,   164,
       0,   485,   486,     0,     0,     0,   163,     0,     0,     0,
     163,     0,     0,   111,     0,     0,     0,     0,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   164,     0,
       0,     0,     0,     0,     0,     0,   505,     0,   254,   164,
       0,   164,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,   255,   256,     0,   257,     0,     0,     0,     0,
     258,     0,     0,     0,   163,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,   260,   169,     0,     0,     0,
       0,   261,   169,     0,     0,   262,     0,     0,   263,     0,
       0,   430,     0,     0,   169,     0,   169,     0,   264,     0,
       0,     0,     0,     0,     0,   265,   266,     0,     0,     0,
       0,     0,     0,   267,     0,     0,     0,     0,   168,   572,
       0,   168,   268,   164,     0,   163,   163,   163,   163,     0,
     164,   269,   270,     0,   271,     0,   272,     0,   273,     0,
       0,   274,     0,     0,   164,   275,     0,     0,   276,     0,
       0,   277,   163,   163,     0,     0,     0,     0,     0,     0,
     169,     0,   164,     0,   169,     0,     0,     0,     0,   218,
       0,   164,   169,    22,    23,   164,     0,     0,  1086,     0,
       0,     0,   168,   219,     0,    31,   220,     0,   168,   500,
     645,    37,     0,     0,     0,   513,     0,     0,    42,     0,
       0,     0,     0,     0,     0,   438,     0,   932,     0,   657,
       0,     0,     0,     0,   661,     0,   663,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,   359,   164,
    1087,     0,     0,  1088,     0,   223,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   724,     0,     0,   111,     0,    84,   505,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,   111,
     757,     0,     0,     0,   168,     0,   168,     0,     0,     0,
       0,   168,     0,     0,   168,     0,     0,     0,     0,     0,
     164,   164,   164,   164,     0,   168,     0,   168,     0,     0,
     168,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,     0,     0,     0,     0,   164,   164,   792,
       0,     0,     0,   169,     0,     0,   169,     0,   808,   168,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   254,
     168,     0,   168,     0,     0,     0,   111,     0,     0,     0,
       0,     0,     0,   255,   256,     0,   257,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,   260,   513,     0,     0,
       0,     0,   261,   513,     0,     0,   262,   169,     0,   263,
       0,     0,     0,   169,     0,   715,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,   265,   266,     0,   861,
       0,     0,   864,   865,   267,     0,     0,     0,     0,     0,
       0,     0,     0,   268,   168,     0,     0,     0,     0,     0,
       0,   168,   269,   270,     0,   271,     0,   272,     0,   273,
       0,     0,   274,     0,     0,   168,   275,     0,     0,   276,
       0,     0,   277,     0,     0,     0,     0,     0,     0,     0,
       0,   779,     0,   168,     0,   784,     0,     0,     0,     0,
       0,     0,   168,   111,     0,     0,   168,     0,     0,   169,
       0,   169,     0,     0,     0,     0,   169,     0,     0,   169,
       0,   910,     0,   218,     0,   505,     0,    22,    23,     0,
     169,     0,   169,     0,     0,   169,     0,   219,   283,    31,
     220,   923,     0,     0,     0,    37,     0,     0,   -82,     0,
       0,   934,    42,     0,     0,   332,     0,     0,     0,     0,
     168,     0,     0,     0,   169,     0,     0,   -53,     0,     0,
       0,     0,     0,     0,     0,   169,     0,   169,    58,     0,
      60,     0,     0,     0,     0,     0,   -53,     0,     0,   223,
     808,    68,     0,     0,     0,     0,     0,     0,     0,     0,
     991,     0,     0,   254,     0,     0,     0,     0,     0,     0,
      84,     0,     0,    86,     0,   -82,    88,   255,   256,     0,
     257,   168,   168,   168,   168,   258,     0,     0,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,     0,     0,
     260,     0,     0,     0,     0,     0,   261,     0,   168,   168,
     262,     0,     0,   263,   894,     0,     0,   897,     0,   169,
       0,     0,     0,   264,     0,     0,   169,     0,     0,     0,
     265,   266,     0,     0,     0,     0,     0,     0,   267,     0,
     169,     0,     0,  1044,     0,   724,     0,   268,     0,  1048,
       0,     0,   505,     0,     0,     0,   269,   270,   169,   271,
       0,   272,     0,   273,     0,     0,   274,   169,     0,     0,
     275,   169,     0,   276,     0,   934,   277,     0,   937,     0,
       0,     0,     0,     0,   942,     0,     0,     0,   218,     0,
       0,     0,    22,    23,     0,     0,     0,  1086,     0,     0,
       0,     0,   219,     0,    31,   220,     0,     0,   808,     0,
      37,   808,     0,     0,     0,     0,     0,    42,     0,     0,
       0,     0,     0,     0,   592,   169,     0,     0,     0,     0,
       0,     0,   909,   680,     0,     0,     0,     0,     0,     0,
     681,   593,     0,    58,     0,    60,     0,    62,     0,  1087,
       0,     0,  1088,     0,   223,   594,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,     0,   513,     0,     0,    84,   682,   513,    86,     0,
     513,    88,     0,     0,   595,     0,   169,   169,   169,   169,
       0,  1039,     0,  1040,     0,     0,  1042,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   169,   169,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   596,  1062,     0,     0,     0,     0,
     597,   103,     0,     0,     0,     0,  1068,  1188,  1072,     0,
       0,     0,     0,     0,   991,     0,   490,   409,   410,   411,
     412,   413,     0,     0,   416,   417,   418,   419,     0,   421,
     422,   946,   947,   948,   949,   950,   688,     0,   689,     0,
       0,     0,   690,   691,   692,   693,   694,   695,   696,   697,
     951,   699,   700,     0,   952,     0,     0,   702,   703,   953,
     705,   954,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -597,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
    -636,  1129,  -636,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1149,
       0,     0,     0,     0,     0,     0,     0,     0,  1154,     0,
       0,     0,  1156,     0,    -2,     4,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,  1194,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,    62,    63,    64,  -324,   -52,
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
       0,  -666,     0,    12,    13,    14,    15,    16,  -666,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,  -666,    28,    29,  -666,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -324,   -52,    65,    66,    67,  -666,    68,
      69,    70,  -666,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,  -666,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -666,
    -666,    95,  -666,  -666,  -666,  -666,  -666,  -666,  -666,     0,
    -666,  -666,  -666,  -666,  -666,     0,  -666,  -666,  -666,  -666,
    -666,  -666,  -666,  -666,   103,  -666,  -666,  -666,     0,   105,
    -666,   106,     0,   107,     0,   343,  -666,     5,   306,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,     0,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,    62,    63,    64,  -324,   -52,
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
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
      62,    63,    64,  -324,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
     544,   106,     0,   107,     0,   563,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,     0,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,    62,    63,    64,  -324,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,   564,   106,     0,   107,     0,
     343,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
      62,    63,    64,  -324,   -52,    65,    66,    67,     0,    68,
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
      39,   -81,     0,    40,    41,    42,     0,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,    62,    63,    64,  -324,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,   791,   106,     0,   107,     0,
       4,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
     359,    63,    64,  -324,   -52,    65,    66,    67,     0,    68,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,     0,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -324,   -52,
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
      34,    35,    36,    37,   667,    39,   -81,     0,    40,    41,
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -324,   -52,    65,    66,    67,     0,    68,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,   673,
      39,   -81,     0,    40,    41,    42,     0,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -324,   -52,
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
      34,    35,    36,    37,  1023,    39,   -81,     0,    40,    41,
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -324,   -52,    65,    66,    67,     0,    68,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1025,
      39,   -81,     0,    40,    41,    42,     0,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -324,   -52,
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
      34,    35,    36,    37,  1030,    39,   -81,     0,    40,    41,
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -324,   -52,    65,    66,    67,     0,    68,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1033,
      39,   -81,     0,    40,    41,    42,     0,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -324,   -52,
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
      27,     0,  1061,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -324,   -52,    65,    66,    67,     0,    68,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1067,
      39,   -81,     0,    40,    41,    42,     0,    43,  -324,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -324,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -324,   -52,
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
      34,    35,    36,    37,  1071,    39,   -81,     0,    40,    41,
      42,     0,    43,  -324,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -324,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -324,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
       0,   106,     0,   107,     0,  1268,   108,     5,   306,     6,
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
       0,     0,   104,     0,   196,     0,   106,     0,   197,  1269,
     919,   108,     5,   306,     6,     7,     8,     9,    10,    11,
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
       9,    10,    11,     0,   680,     0,   187,     0,     0,    15,
      16,   681,    17,     0,   188,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   189,     0,
       0,     0,    33,   190,   191,     0,     0,   192,    39,     0,
       0,     0,    41,     0,     0,    43,     0,   682,   193,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     194,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   195,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,   490,   409,   410,
     411,   412,   413,     0,     0,   416,   417,   418,   419,     0,
     421,   422,   683,   684,   685,   686,   687,   688,     0,   689,
       0,    98,     0,   690,   691,   692,   693,   694,   695,   696,
     697,   698,   699,   700,   100,   701,   102,     0,   702,   703,
     704,   705,   196,     0,   106,     0,   197,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   187,
       0,     0,    15,    16,     0,    17,     0,   188,     0,     0,
      21,   245,    23,     0,     0,     0,     0,     0,     0,    29,
       0,   189,     0,     0,     0,    33,   190,   191,     0,     0,
     192,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   193,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,    60,    61,     0,    63,     0,     0,
       0,     0,    66,   194,     0,    68,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   195,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
      88,    89,     0,     0,     0,     0,     0,     0,     0,    92,
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
      77,   195,     0,    79,     0,     0,    81,   310,   311,     0,
      85,   348,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
     349,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     196,     0,   106,     0,   197,     0,     0,   108,     5,   306,
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
       0,    81,   310,   311,     0,    85,   348,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   196,     0,   106,   775,   197,
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
      77,   195,     0,    79,     0,     0,    81,   310,   311,     0,
      85,   348,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     196,     0,   106,   777,   197,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   187,     0,     0,
      15,    16,     0,    17,     0,   188,     0,     0,    21,     0,
     628,     0,     0,     0,     0,     0,     0,    29,     0,   189,
       0,     0,     0,    33,   190,   191,     0,     0,   192,    39,
       0,   629,     0,    41,     0,     0,    43,     0,     0,   193,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   194,     0,   630,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   195,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,   631,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   196,     0,   106,     0,   197,     0,     5,
     108,     6,     7,     8,   231,    10,    11,   232,     0,     0,
     187,     0,     0,    15,    16,     0,    17,     0,   188,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   189,     0,     0,     0,    33,   190,   191,     0,
       0,   192,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   193,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   233,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   195,     0,    79,
       0,     0,    81,     0,     0,   234,    85,     0,   235,    87,
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
     195,     0,    79,     0,     0,    81,   310,   311,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   312,     0,     0,   104,     0,   196,
       0,   106,     0,   197,     0,     0,   108,     5,   306,     6,
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
      81,   310,   311,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   196,     0,   106,     0,   197,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   192,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,   921,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   194,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,     0,    81,   310,   311,     0,    85,     0,     0,
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
      33,   190,   191,     0,     0,   192,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   193,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,   237,
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
       8,     9,    10,    11,     0,     0,     0,   187,     0,     0,
      15,    16,     0,    17,     0,   188,     0,     0,    21,     0,
     248,     0,     0,     0,     0,     0,     0,    29,     0,   189,
       0,     0,     0,    33,   190,   191,     0,     0,   192,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   193,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   194,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   195,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,   249,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   196,     0,   106,     0,   197,     0,     0,
     108,     5,   306,     6,     7,     8,     9,    10,    11,     0,
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
     238,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   196,     0,
     106,     0,   197,     0,     5,   108,     6,     7,     8,   231,
      10,    11,     0,     0,     0,   187,     0,     0,    15,    16,
       0,    17,     0,   188,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,   192,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   193,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   233,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   195,     0,    79,     0,     0,    81,     0,     0,
     234,    85,     0,   235,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   196,     0,   106,     0,   197,     0,     5,   108,     6,
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
      81,   310,   311,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   196,     0,   106,     0,   197,     0,
       0,   108,     5,   306,     6,     7,     8,     9,    10,    11,
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
     773,   106,     0,   197,     0,     0,   108,     5,   306,     6,
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
       0,     0,   104,     0,   196,     0,   106,     0,   197,   783,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   192,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   194,  -710,     0,     0,     0,
    -710,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
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
      75,    76,    77,   195,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   196,     0,   106,     0,   197,  1198,     5,   108,
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
       0,    89,     0,     0,     0,     0,     0,   238,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   196,     0,   106,     0,   197,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   187,     0,     0,    15,    16,     0,    17,     0,
     188,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   287,    29,     0,   189,     0,     0,     0,    33,   190,
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
     100,   101,   102,     0,     0,     0,   104,     0,   105,     0,
     106,     0,   197,     0,     0,   108,     5,   306,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   187,     0,     0,
      15,    16,     0,    17,     0,   188,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   189,
       0,     0,     0,    33,   190,   191,     0,     0,   192,    39,
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
     197,   788,     5,   108,     6,     7,     8,     9,    10,    11,
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
     957,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   196,
       0,   106,     0,   197,     0,     0,   108,     5,   306,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   187,     0,
       0,    15,    16,     0,    17,     0,   188,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,   744,     0,
     189,     0,     0,     0,    33,   190,   191,     0,     0,   192,
      39,     0,     0,     0,   745,     0,     0,    43,     0,     0,
     193,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   194,     0,     0,     0,     0,     0,     0,     0,
      73,   746,    75,    76,    77,   747,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   196,     0,   106,     0,  1133,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   192,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   194,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,     0,    81,     0,     0,     0,    85,  1158,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
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
      75,    76,    77,   195,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   196,     0,   106,     0,  1133,     0,     0,   108,
       5,   306,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,  1112,    39,     0,     0,     0,    41,     0,     0,
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
      33,   190,   191,     0,     0,   192,    39,     0,     0,     0,
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
       8,   227,    10,    11,     0,     0,     0,   187,     0,     0,
      15,    16,     0,    17,     0,   188,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   189,
       0,     0,     0,    33,   190,   191,     0,     0,   192,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   193,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   228,     0,     0,     0,     0,     0,     0,     0,    73,
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
     102,     0,     0,     0,   104,     0,   105,     0,   106,     0,
     197,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   187,     0,     0,    15,    16,     0,    17,
       0,   188,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,   744,     0,   189,     0,     0,     0,    33,
     190,   191,     0,     0,   192,    39,     0,     0,     0,   745,
       0,     0,    43,     0,     0,   193,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   194,     0,     0,
       0,     0,     0,     0,     0,    73,   746,    75,    76,    77,
     747,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   196,
       0,   106,     0,   748,     0,     5,   108,     6,     7,     8,
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
     104,     0,   196,     0,   106,     0,   748,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   187,
       0,     0,    15,    16,     0,    17,     0,   188,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   189,     0,     0,     0,    33,   190,   191,     0,     0,
     845,    39,     0,     0,     0,    41,     0,     0,    43,     0,
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
     191,     0,     0,   848,    39,     0,     0,     0,    41,     0,
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
       0,     0,     0,     0,     0,    29,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,  1106,    39,     0,     0,
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
       0,   196,     0,   106,     0,   197,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   187,     0,
       0,    15,    16,     0,    17,     0,   188,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     189,     0,     0,     0,    33,   190,   191,     0,     0,  1107,
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
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,  1109,    39,     0,     0,     0,    41,     0,     0,
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
      33,   190,   191,     0,     0,  1110,    39,     0,     0,     0,
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
       0,     0,     0,    33,   190,   191,     0,     0,  1111,    39,
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
       0,  1112,    39,     0,     0,     0,    41,     0,     0,    43,
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
       0,     0,     0,   744,     0,   189,     0,     0,     0,    33,
     190,   191,     0,     0,   192,    39,     0,     0,     0,   745,
       0,     0,    43,     0,     0,   193,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   194,     0,     0,
       0,     0,     0,     0,     0,    73,   746,    75,    76,    77,
     747,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   196,
       0,   106,     0,  1223,     0,     5,   108,     6,     7,     8,
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
       0,     0,     0,     0,     0,     0,   972,     0,   973,     0,
       0,     0,    93,    94,    95,    96,    97,   680,     0,     0,
       0,    98,   974,   256,   681,   975,     0,     0,     0,     0,
     976,    99,     0,     0,   100,   101,   102,     0,     0,   259,
     104,   189,     0,     0,   106,   977,  1223,     0,     0,   108,
       0,   978,     0,     0,     0,   262,     0,     0,   979,     0,
     682,     0,     0,     0,     0,     0,     0,     0,   980,     0,
       0,     0,     0,     0,     0,   981,   982,     0,     0,     0,
       0,     0,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,   983,     0,     0,     0,     0,     0,     0,     0,
       0,   269,   270,     0,   984,     0,   272,     0,   985,     0,
       0,   986,     0,     0,     0,   987,     0,     0,   276,     0,
       0,   988,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   409,   410,   411,   412,   413,     0,     0,   416,   417,
     418,   419,     0,   421,   422,   946,   947,   948,   949,   950,
     688,     0,   689,     0,     0,     0,   690,   691,   692,   693,
     694,   695,   696,   697,   951,   699,   700,     0,   952,     0,
       0,   702,   703,   953,   705,     0,     5,   989,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   187,     0,     0,
      15,    16,     0,    17,     0,   188,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   189,
       0,     0,     0,    33,   190,   191,     0,     0,   192,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   193,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   194,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   195,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,     0,   101,   102,     0,     0,
       0,   104,     0,   196,     0,   106,     0,   197,     5,   306,
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
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
    -494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,  -494,     0,     0,     0,  -494,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,     0,     0,     0,   196,     0,   106,  -494,  1133,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   192,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,  -476,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   194,     0,     0,     0,  -476,
       0,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,  -476,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,  -476,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   187,     0,     0,    15,    16,     0,    17,     0,
     188,   102,  -476,    21,     0,     0,     0,  -476,     0,   106,
       0,  1223,    29,     0,   189,     0,     0,     0,    33,   190,
     191,     0,     0,   192,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   193,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   194,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   195,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
      95,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   187,     0,     0,    15,    16,     0,    17,
       0,   188,   102,     0,    21,     0,     0,     0,   196,     0,
     106,     0,   197,    29,     0,   189,     0,     0,     0,    33,
     190,   191,     0,     0,   192,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   193,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   194,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     195,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   187,     0,     0,    15,    16,     0,
      17,     0,   188,   102,     0,    21,     0,     0,     0,   196,
       0,   106,     0,   748,    29,     0,   189,     0,     0,     0,
      33,   190,   191,     0,     0,   192,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   193,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   194,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   195,     0,    79,     0,     0,    81,     0,  1173,     0,
      85,     0,     0,    87,     0,     0,    89,   680,     0,     0,
       0,     0,   255,   256,   681,   257,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,    95,     0,     0,   977,     0,     0,     0,     0,
       0,   261,     0,     0,     0,   262,     0,     0,   263,     0,
     682,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,   106,     0,   197,   981,   266,     0,     0,     0,
       0,     0,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,     0,     0,     0,     0,
     378,   269,   270,     0,   271,     0,   272,   379,  1174,     0,
       0,   986,     0,     0,     0,   275,     0,     0,   276,     0,
     380,   277,     0,     0,     0,     0,     0,     0,     0,     0,
     490,   409,   410,   411,   412,   413,     0,     0,   416,   417,
     418,   419,     0,   421,   422,   946,   947,   948,   949,   950,
     688,     0,   689,     0,     0,     0,   690,   691,   692,   693,
     694,   695,   696,   697,   951,   699,   700,     0,   952,     0,
       0,   702,   703,   953,   705,     0,   378,   381,     0,     0,
       0,   382,     0,   379,     0,     0,     0,     0,     0,     0,
     489,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,   490,   409,   410,   411,   412,   413,     0,
       0,   416,   417,   418,   419,     0,   421,   422,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,   381,   404,   405,   406,   382,   438,   379,
       0,     0,     0,     0,     0,   407,  1022,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   383,   490,
     409,   410,   411,   412,   413,     0,     0,   416,   417,   418,
     419,     0,   421,   422,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   396,
       0,   397,   398,   399,   400,   401,   402,   403,   378,   381,
     404,   405,   406,   382,   438,   379,     0,     0,     0,     0,
       0,   407,  1029,     0,     0,     0,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   383,   490,   409,   410,   411,   412,
     413,     0,     0,   416,   417,   418,   419,     0,   421,   422,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   378,   381,   404,   405,   406,   382,
     438,   379,     0,     0,     0,     0,     0,   407,  1189,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     383,   490,   409,   410,   411,   412,   413,     0,     0,   416,
     417,   418,   419,     0,   421,   422,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   401,   402,   403,
     378,   381,   404,   405,   406,   382,   438,   379,     0,     0,
       0,     0,     0,   407,  1190,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,   490,   409,   410,
     411,   412,   413,     0,     0,   416,   417,   418,   419,     0,
     421,   422,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,   378,   381,   404,   405,
     406,   382,   438,   379,     0,     0,     0,     0,     0,   407,
    1191,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,   490,   409,   410,   411,   412,   413,     0,
       0,   416,   417,   418,   419,     0,   421,   422,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,   381,   404,   405,   406,   382,   438,   379,
       0,     0,     0,     0,     0,   407,  1192,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   383,   490,
     409,   410,   411,   412,   413,     0,     0,   416,   417,   418,
     419,     0,   421,   422,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   396,
       0,   397,   398,   399,   400,   401,   402,   403,   378,   381,
     404,   405,   406,   382,   438,   379,     0,     0,     0,     0,
       0,   407,  1216,     0,     0,     0,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   383,   490,   409,   410,   411,   412,
     413,     0,     0,   416,   417,   418,   419,     0,   421,   422,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,     0,   381,   404,   405,   406,   382,
     438,     0,     0,     0,     0,     0,     0,   407,  1217,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     383,   490,   409,   410,   411,   412,   413,     0,     0,   416,
     417,   418,   419,     0,   421,   422,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   401,   402,   403,
     378,     0,   404,   405,   406,     0,   438,   379,     0,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,     0,
     380,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   476,   218,     0,     0,
     378,    22,    23,     0,     0,     0,     0,   379,     0,     0,
       0,   219,     0,    31,   220,     0,     0,     0,     0,    37,
     380,   469,     0,     0,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   470,   381,     0,     0,
     378,   382,     0,     0,     0,     0,     0,   379,     0,     0,
       0,     0,    58,     0,    60,     0,     0,     0,     0,     0,
     380,   287,     0,   223,     0,    68,     0,     0,   472,     0,
       0,     0,   383,     0,     0,     0,   480,   381,     0,     0,
       0,   382,     0,     0,    84,     0,     0,    86,   384,   385,
      88,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   383,     0,   404,   405,   406,   381,   438,     0,
       0,   382,     0,     0,     0,   407,     0,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   383,     0,   404,   405,   406,     0,   438,     0,
       0,     0,     0,     0,     0,   407,     0,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,     0,   404,   405,   406,     0,   438,   379,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   799,     0,     0,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,   483,     0,     0,     0,   800,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
       0,     0,     0,   382,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,   379,     0,     0,     0,     0,     0,
       0,     0,     0,   801,     0,     0,     0,   380,   882,     0,
       0,     0,     0,     0,   383,     0,     0,     0,     0,   381,
       0,     0,     0,   382,     0,     0,     0,     0,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   383,   484,   404,   405,   406,     0,
       0,     0,     0,     0,   381,     0,     0,   407,   382,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,     0,   472,   404,   405,   406,   383,
       0,     0,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,     0,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,   378,
       0,   404,   405,   406,     0,   438,   379,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   440,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,   379,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   380,
     287,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   381,     0,     0,   378,
     382,     0,     0,     0,     0,     0,   379,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   380,
     637,     0,     0,     0,     0,     0,     0,   182,     0,     0,
       0,   383,     0,     0,     0,   638,   381,     0,     0,     0,
     382,     0,     0,     0,     0,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   383,     0,   404,   405,   406,   381,     0,     0,     0,
     382,     0,     0,     0,   407,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   383,     0,   404,   405,   406,     0,   438,     0,     0,
       0,     0,     0,     0,   407,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   378,   218,   404,   405,   406,    22,    23,   379,     0,
       0,     0,     0,     0,   407,     0,   219,     0,    31,   220,
       0,   380,   639,     0,    37,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,     0,   640,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,   379,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,   380,   874,  1087,     0,     0,  1088,     0,   223,     0,
      68,     0,     0,     0,     0,     0,     0,     0,   381,     0,
       0,   378,   382,     0,     0,     0,     0,     0,   379,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,     0,     0,     0,     0,   381,     0,
       0,     0,   382,     0,     0,     0,     0,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   103,   397,   398,   399,   400,
     401,   402,   403,   383,     0,   404,   405,   406,   381,     0,
       0,     0,   382,     0,     0,     0,   407,     0,     0,   384,
     385,   898,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   383,     0,   404,   405,   406,     0,   438,
       0,     0,     0,     0,     0,     0,   407,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   378,     0,   404,   405,   406,     0,   438,
     379,     0,     0,     0,     0,     0,   407,     0,     0,     0,
       0,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   218,     0,     0,     0,    22,
      23,     0,     0,   378,     0,     0,     0,     0,     0,   219,
     379,    31,   220,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,   380,    42,     0,     0,     0,  1080,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -53,
     381,     0,     0,     0,   382,     0,     0,     0,   378,   517,
      58,     0,    60,     0,     0,   379,     0,     0,   -53,     0,
       0,   223,     0,    68,  1081,     0,     0,     0,   380,     0,
       0,   472,     0,     0,     0,   383,     0,     0,     0,     0,
     381,     0,    84,     0,   382,    86,     0,     0,    88,     0,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   383,     0,   404,   405,   406,
       0,     0,     0,     0,     0,   381,   940,     0,   407,   382,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   378,     0,   404,   405,   406,
     383,     0,   379,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,   380,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   401,   402,   403,
     378,     0,   404,   405,   406,     0,     0,   379,     0,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,     0,     0,     0,   382,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,     0,     0,
       0,   382,     0,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,     0,     0,   404,
     405,   406,   383,   528,     0,     0,     0,   381,     0,     0,
     407,   382,     0,     0,     0,     0,     0,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   383,   532,   404,   405,   406,     0,     0,     0,
       0,     0,     0,     0,     0,   407,     0,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,   534,   404,   405,   406,     0,     0,   379,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,   380,   218,     0,     0,     0,    22,    23,     0,
       0,     0,  1086,     0,     0,     0,     0,   219,     0,    31,
     220,     0,   378,     0,     0,    37,     0,     0,     0,   379,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,   381,
      60,     0,    62,   382,  1087,     0,     0,  1088,   378,   223,
       0,    68,     0,     0,     0,   379,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   380,     0,
      84,     0,     0,    86,   383,     0,    88,     0,     0,   381,
       0,     0,     0,   382,     0,     0,     0,     0,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   383,   536,   404,   405,   406,     0,
       0,     0,     0,     0,     0,   381,   103,   407,     0,   382,
     384,   385,  1238,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   378,   542,   404,   405,   406,   559,
     383,   379,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,   380,     0,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   401,   402,   403,
     378,     0,   404,   405,   406,     0,     0,   379,     0,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,     0,     0,     0,   382,     0,     0,     0,     0,
     378,   795,     0,     0,   641,     0,     0,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,   383,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,     0,     0,
       0,   382,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,     0,     0,   404,   405,
     406,     0,   383,     0,     0,     0,     0,   381,     0,   407,
       0,   382,     0,     0,     0,     0,     0,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   383,     0,   404,   405,   406,     0,     0,     0,
       0,   774,     0,     0,     0,   407,     0,     0,   384,   385,
       0,   386,   387,   388,   380,   389,   796,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,     0,   404,   405,   406,     0,     0,   379,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,     0,     0,     0,   382,     0,   378,     0,     0,
       0,     0,     0,     0,   379,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,   383,     0,     0,   381,
       0,     0,     0,   382,     0,     0,     0,   378,     0,     0,
       0,     0,   384,   385,   379,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   380,     0,   397,
     398,   399,   400,   401,   383,   403,     0,     0,   404,   405,
     406,     0,     0,     0,   381,     0,     0,     0,   382,   407,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,  -598,   397,   398,   399,
     400,   401,   402,   403,     0,   182,   404,   405,   406,   383,
       0,     0,     0,     0,   381,     0,     0,   407,   382,     0,
       0,     0,     0,     0,     0,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,   383,
       0,   404,   405,   406,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,     0,
     857,   404,   405,   406,   378,   889,     0,     0,     0,     0,
       0,   379,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,   379,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   380,     0,     0,   896,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,     0,     0,   378,   382,     0,     0,     0,     0,
       0,   379,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,     0,     0,     0,
       0,   381,     0,     0,     0,   382,     0,     0,     0,     0,
       0,     0,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,   383,     0,   404,   405,
     406,   381,     0,     0,     0,   382,     0,     0,     0,   407,
       0,     0,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,   383,     0,   404,   405,
     406,     0,     0,     0,     0,     0,     0,     0,     0,   407,
       0,     0,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,   378,     0,   404,   405,
     406,     0,     0,   379,     0,   924,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,   379,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,     0,     0,     0,   382,   378,     0,
       0,     0,     0,     0,     0,   379,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   380,  1005,
       0,     0,     0,     0,     0,     0,     0,     0,   383,     0,
       0,     0,     0,     0,   381,     0,     0,     0,   382,     0,
       0,     0,     0,     0,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   396,
       0,   397,   398,   399,   400,   401,   402,   403,   943,   383,
     404,   405,   406,     0,     0,   381,     0,   925,     0,   382,
       0,   407,     0,     0,     0,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,     0,
     383,   404,   405,   406,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   401,   402,   403,
     378,     0,   404,   405,   406,     0,     0,   379,     0,     0,
       0,     0,     0,   407,   218,     0,     0,     0,    22,    23,
     380,  1008,     0,  1086,     0,     0,     0,     0,   219,     0,
      31,   220,     0,     0,     0,     0,    37,     0,     0,     0,
     378,     0,     0,    42,     0,     0,     0,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,  1087,     0,   381,  1088,     0,
     223,   382,    68,     0,     0,   378,  1077,     0,     0,     0,
       0,     0,   379,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,     0,    86,   380,     0,    88,     0,     0,
       0,     0,   383,     0,     0,     0,     0,   381,     0,     0,
       0,   382,     0,     0,     0,     0,     0,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   383,     0,   404,   405,   406,   103,     0,     0,
       0,     0,   381,  1239,     0,   407,   382,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,     0,   404,   405,   406,   383,     0,   379,
       0,     0,     0,  1011,     0,   407,     0,     0,     0,     0,
       0,     0,   380,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,     0,     0,   404,
     405,   406,     0,     0,     0,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,   381,
       0,     0,     0,   382,     0,   379,     0,     0,     0,     0,
       0,     0,  1105,     0,     0,     0,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   383,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,     0,   381,   404,   405,   406,   382,
       0,     0,     0,   378,  1127,     0,     0,   407,  1108,     0,
     379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,     0,     0,     0,     0,     0,     0,
     383,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   401,   402,   403,
       0,     0,   404,   405,   406,     0,     0,     0,     0,   378,
     381,     0,     0,   407,   382,     0,   379,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   383,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   378,   381,   404,   405,   406,
     382,     0,   379,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   383,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   378,   381,   404,   405,   406,   382,     0,   379,     0,
    1172,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,   378,   381,   404,
     405,   406,   382,     0,   379,     0,     0,     0,  1212,     0,
     407,  1232,     0,     0,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   378,   381,   404,   405,   406,   382,     0,
     379,     0,     0,     0,     0,     0,   407,  1233,     0,     0,
       0,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   383,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,   378,
     381,   404,   405,   406,   382,     0,   379,     0,     0,     0,
       0,     0,   407,  1234,     0,     0,     0,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   383,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   378,   381,   404,   405,   406,
     382,     0,   379,     0,     0,     0,     0,     0,   407,  1235,
       0,     0,     0,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   383,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   378,   381,   404,   405,   406,   382,     0,   379,     0,
       0,     0,     0,     0,   407,  1236,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,   378,   381,   404,
     405,   406,   382,     0,   379,     0,     0,     0,     0,     0,
     407,  1237,     0,     0,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1262,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   378,   381,   404,   405,   406,   382,     0,
     379,     0,     0,     0,     0,     0,   407,     0,     0,     0,
       0,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   383,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,   378,
     381,   404,   405,   406,   382,     0,   379,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,   218,     0,
       0,     0,    22,    23,     0,   383,     0,  1086,     0,     0,
       0,     0,   219,     0,    31,   220,     0,     0,     0,     0,
      37,   384,   385,     0,   386,   387,   388,    42,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   378,   381,   404,   405,   406,
     382,     0,   379,    58,     0,    60,  1291,    62,   407,  1087,
       0,     0,  1088,     0,   223,   380,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   383,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,     0,   843,   404,   405,   406,   382,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,   103,     0,     0,     0,   380,     0,  1240,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,     0,     0,   404,
     405,   406,   381,     0,     0,     0,   382,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,     0,     0,
     397,   398,   399,   400,   401,   402,   403,     0,     0,   404,
     405,   406,   806,     0,   254,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,   255,   256,
       0,   257,     0,     0,     0,     0,   258,     0,     0,     0,
       0,     0,   807,     0,     0,   259,     0,     0,     0,     0,
       0,   260,     0,     0,     0,     0,     0,   261,     0,     0,
       0,   262,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,     0,     0,     0,
       0,   265,   266,     0,     0,     0,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
       0,   254,     0,     0,     0,     0,     0,   269,   270,     0,
     271,     0,   272,     0,   273,   255,   256,   274,   257,     0,
       0,   275,     0,   258,   276,    23,     0,   277,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,   260,     0,
       0,     0,     0,     0,   261,     0,     0,     0,   262,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,   265,   266,
       0,     0,     0,     0,     0,     0,   267,    60,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,    68,     0,
       0,     0,     0,     0,   269,   270,     0,   271,     0,   272,
       0,   273,   254,     0,   274,     0,     0,   965,   275,     0,
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
       0,   258,   276,     0,     0,   277,     0,   807,     0,     0,
     259,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,     0,   261,     0,     0,     0,   262,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,   265,   266,     0,     0,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,     0,  -302,     0,     0,     0,
       0,     0,   269,   270,     0,   271,     0,   272,     0,   273,
    -302,  -302,   274,  -302,     0,     0,   275,     0,  -302,   276,
       0,     0,   277,     0,     0,     0,     0,  -302,     0,     0,
       0,     0,     0,  -302,     0,     0,     0,     0,     0,  -302,
       0,     0,     0,  -302,     0,     0,  -302,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -302,     0,     0,     0,
       0,     0,     0,  -302,  -302,     0,     0,     0,     0,     0,
       0,  -302,     0,     0,     0,     0,     0,     0,     0,     0,
    -302,     0,     0,  -312,     0,     0,     0,     0,     0,  -302,
    -302,     0,  -302,     0,  -302,     0,  -302,  -312,  -312,  -302,
    -312,     0,     0,  -302,     0,  -312,  -302,     0,     0,  -302,
       0,     0,     0,     0,  -312,     0,     0,     0,     0,     0,
    -312,     0,     0,     0,     0,     0,  -312,     0,     0,     0,
    -312,     0,     0,  -312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -312,     0,     0,     0,     0,     0,     0,
    -312,  -312,     0,     0,     0,     0,     0,     0,  -312,     0,
       0,     0,     0,     0,     0,     0,     0,  -312,     0,     0,
     254,     0,     0,     0,     0,     0,  -312,  -312,     0,  -312,
       0,  -312,     0,  -312,   255,   256,  -312,   257,     0,     0,
    -312,     0,   258,  -312,     0,     0,  -312,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,     0,   261,     0,     0,     0,   262,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,   265,   266,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,     0,  -303,     0,     0,
       0,     0,     0,   269,   270,     0,   271,     0,   272,     0,
     273,  -303,  -303,   274,  -303,     0,     0,   275,     0,  -303,
     276,     0,     0,   277,     0,     0,     0,     0,  -303,     0,
       0,     0,     0,     0,  -303,     0,     0,     0,     0,     0,
    -303,     0,     0,     0,  -303,     0,     0,  -303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -303,     0,     0,
       0,     0,     0,     0,  -303,  -303,     0,     0,     0,     0,
       0,     0,  -303,     0,     0,     0,     0,     0,     0,     0,
       0,  -303,     0,     0,  -193,     0,     0,     0,     0,     0,
    -303,  -303,     0,  -303,     0,  -303,     0,  -303,  -193,  -193,
    -303,  -193,     0,     0,  -303,     0,  -193,  -303,     0,     0,
    -303,     0,     0,     0,     0,  -193,     0,     0,     0,     0,
       0,  -193,     0,     0,     0,     0,     0,  -193,     0,     0,
       0,  -193,     0,     0,  -193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -193,     0,     0,     0,     0,     0,
       0,  -193,  -193,     0,     0,     0,     0,     0,     0,  -193,
       0,     0,     0,     0,     0,     0,     0,     0,  -193,     0,
       0,  -185,     0,     0,     0,     0,     0,  -193,  -193,     0,
    -193,     0,  -193,     0,  -193,  -185,  -185,  -193,  -185,     0,
       0,  -193,     0,  -185,  -193,     0,     0,  -193,     0,     0,
       0,     0,  -185,     0,     0,     0,     0,     0,  -185,     0,
       0,     0,     0,     0,  -185,     0,     0,     0,  -185,     0,
       0,  -185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -185,     0,     0,     0,     0,     0,     0,  -185,  -185,
       0,     0,     0,     0,     0,     0,  -185,     0,     0,     0,
       0,     0,     0,     0,     0,  -185,     0,     0,     0,     0,
       0,     0,     0,     0,  -185,  -185,     0,  -185,     0,  -185,
       0,  -185,     0,     0,  -185,     0,     0,     0,  -185,     0,
       0,  -185,     0,     0,  -185
};

static const yytype_int16 yycheck[] =
{
      12,    65,   482,   537,    16,   429,    69,     2,    20,    19,
     210,   242,   817,     2,    26,   166,   504,   253,   152,    31,
      32,    22,    34,    35,    36,    37,    38,   817,   801,    27,
     994,     1,   996,   918,   234,   807,   963,    49,   743,    51,
      52,   497,   105,   106,   107,    57,   530,    59,  1143,    61,
      33,   671,   509,    18,    69,     3,    48,    69,    70,    71,
      72,    73,    74,  1051,    92,    33,    78,   303,    80,    33,
      82,    83,     1,    92,   796,    87,  1048,   211,    90,    91,
     537,    93,    56,    95,  1198,   321,    98,    99,    33,   101,
     120,    37,   107,   105,   106,   107,   108,   657,     1,    75,
     251,   661,    33,   663,     3,    75,     1,    48,    88,     1,
    1252,    33,   118,    78,   146,  1000,   105,   140,     3,    48,
      48,     0,   145,   354,   355,   148,   226,    73,    70,   152,
      48,    96,   112,   196,   197,    48,    56,   842,  1252,    48,
     146,   117,   170,   175,    92,   175,   165,   117,   113,  1291,
     162,   170,   120,   253,   146,   103,   120,    48,   222,  1086,
    1132,   165,    90,  1090,   106,   887,    61,  1052,   172,    61,
     174,   655,    34,    35,    36,   187,  1271,   328,   190,   191,
     192,  1169,   197,   195,   196,   197,   642,   170,  1115,   425,
     426,  1118,  1119,   965,  1131,   171,   125,    92,   172,   209,
      92,   171,   170,   303,   103,   146,   170,   120,   220,   829,
     915,  1175,  1176,   105,   226,   146,   145,   146,   103,   217,
      32,   321,   125,   118,   146,   170,   118,   102,   146,   230,
      61,    27,   165,   146,   175,   236,   165,   146,  1165,   170,
     304,   253,   145,   146,   245,   107,  1131,   671,   170,    54,
     145,    48,   172,   145,   742,   146,  1183,   175,  1185,  1186,
    1197,    92,   175,    47,    27,   146,   175,   120,    83,    74,
     173,    86,   146,   165,   105,   170,   146,    89,   170,    84,
     146,    93,  1269,    79,   175,    48,    61,   118,   146,    52,
     390,   303,   173,    90,    90,   170,  1101,   171,   310,   311,
    1287,   861,   146,   173,   864,   865,  1079,   173,  1081,   321,
      89,  1101,  1197,    76,    93,   173,    79,   125,    81,   115,
     800,   165,   175,   146,   458,   425,   426,    90,   140,   141,
     114,   465,   144,   145,   165,   147,   148,   349,   146,   170,
     152,  1016,   150,   118,  1019,  1020,   109,   803,   160,    56,
     173,   346,   364,   365,   166,   167,   168,   346,    89,    72,
     120,   172,    93,   174,   376,   173,   378,   379,   380,   381,
     382,   383,   361,   385,   386,   387,   388,   389,     3,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,    61,   497,   854,    47,
     146,   125,   414,   415,   467,   829,   146,  1027,   420,    56,
     173,   423,   424,   425,   426,   427,   428,   429,    66,   146,
     146,   909,    56,   146,   146,   171,   150,    92,   440,    66,
     530,   171,    27,   445,   146,    27,   448,    85,    56,  1114,
     105,  1116,  1117,   146,   675,   676,   173,   173,    85,   438,
     173,   173,   467,   118,    90,   467,    49,   469,   470,     8,
     523,   173,   165,   475,   476,    61,   114,   146,   480,   165,
     482,   483,   414,   415,   954,    32,   172,   489,   420,    72,
     492,   423,   424,   546,    79,   497,   549,    79,   551,    82,
      83,   165,   172,   556,   173,    90,    92,   171,    90,   511,
     165,    56,   146,   515,    56,   170,   518,   519,   146,   105,
     573,   523,   501,   172,   109,   174,   517,   109,   530,   381,
     382,   165,   118,   115,  1058,    56,   239,   240,   241,   242,
     243,   244,    89,   146,   546,   173,    93,   549,   146,   551,
     146,   146,   642,   555,   556,   557,   140,   559,   560,   146,
     781,   782,   165,   172,   148,   655,     3,   165,   573,   165,
     165,   573,   146,   140,   125,   427,   428,   140,   165,   165,
     565,   148,   772,    20,   170,   148,   565,   290,   440,   292,
     146,   165,    27,   140,    56,   146,   299,    34,   145,   150,
     653,   148,   146,    89,  1051,   152,   165,    93,  1078,   165,
    1080,  1058,   171,  1027,   317,   146,    89,   549,   470,   551,
      93,   165,   173,    89,   476,   208,    63,    93,   480,   146,
      27,  1203,   335,   336,   165,   637,   638,   639,   640,   641,
     642,   643,   644,    56,    79,    89,   646,   647,   165,    93,
     172,   653,   174,   655,    51,    90,   239,   240,   241,   242,
     243,   244,    56,   665,   230,   667,   103,   250,   251,   671,
     236,   673,   109,   172,   109,   174,   817,    27,    82,    83,
      77,   683,    79,   685,  1256,  1257,  1258,  1259,  1260,  1261,
     170,   172,   172,    90,    56,   748,   698,   280,    48,   701,
     100,    56,    52,   555,   172,   795,   796,   290,   172,   292,
     174,   713,   714,   803,    56,   172,   299,   174,   115,    22,
      56,   653,  1169,    26,    27,   549,    76,   551,    31,    79,
     871,    81,    56,    36,   317,    38,    39,   878,   441,    56,
      90,    44,   170,   748,   746,   747,   748,    85,    51,    48,
     752,  1198,   335,   336,    56,   165,     2,   164,   156,   109,
     165,   172,    61,   165,   854,  1212,    12,    56,   119,   165,
     165,   474,   165,   165,    77,   478,    79,   165,    81,    25,
      83,   170,    28,    86,   165,    88,   638,    90,   640,   172,
      88,   793,   172,    92,   172,   797,    32,   799,   800,   801,
     172,   803,   172,   172,   165,  1252,   109,   125,   165,   112,
       8,    66,   115,   145,   170,   148,    69,   165,   172,   118,
     752,   125,  1269,   146,   173,   165,   125,   829,    48,   172,
     125,   125,    78,  1059,  1060,    32,    35,   146,   173,    35,
    1287,   843,   844,   845,  1291,   173,   848,   146,   125,    21,
     125,   173,   854,    89,   173,   146,   172,    93,   441,   105,
     175,   175,   165,   170,   110,   165,   165,   173,   171,   165,
     872,   170,   874,   165,   173,   928,   151,   879,   153,   154,
     882,   165,   157,   158,   165,   887,   165,   889,   175,   172,
     146,   474,    89,   165,   896,   478,    93,    35,    35,    66,
     173,   146,   173,   171,   140,   141,   173,   143,   144,   145,
     171,   147,   148,   146,   125,    90,   152,    33,    33,   173,
      48,   170,   170,   928,   160,   175,   928,   170,   907,    35,
     166,   167,   168,    61,   165,   145,   938,   183,   940,    90,
     175,   943,   172,   140,   172,   145,   175,   165,   145,    48,
     147,   148,   954,   175,   171,   152,   172,   148,   148,   148,
    1101,    27,    61,   160,    92,   967,   669,   670,   148,  1059,
    1060,   168,   675,   676,   140,   678,   679,   148,   148,   140,
    1121,  1122,    48,   148,  1125,  1126,    52,   989,   148,   148,
     118,   843,   844,    92,   140,   148,   148,   125,  1051,   140,
     140,   148,  1004,  1005,    35,  1007,  1008,   170,  1010,  1011,
      76,   173,  1153,    79,  1155,    81,   170,   173,   146,   118,
    1022,  1023,   170,  1025,    90,  1027,   125,  1029,  1030,   165,
     165,  1033,   105,   165,  1088,   281,     2,   165,   165,   173,
     173,   287,   170,   109,   173,   173,    12,   146,   140,   140,
     164,   164,   175,   146,   495,  1057,  1058,  1059,  1060,    25,
     361,   793,    28,  1065,    24,  1067,   165,   515,    90,  1071,
     902,   170,   329,  1090,   173,  1077,  1078,  1079,  1080,  1081,
    1133,   932,   968,  1097,   907,  1138,   669,   670,   734,  1101,
    1271,   971,   675,   676,  1228,   678,   679,  1262,  1255,    37,
     346,    31,    -1,  1105,  1106,  1107,  1108,  1109,  1110,  1111,
    1112,   512,    78,    -1,    -1,   361,    82,    83,  1097,    -1,
      -1,    -1,    -1,    -1,    -1,  1127,    -1,    -1,  1133,    -1,
      -1,  1133,    -1,    -1,    -1,    -1,  1138,   720,    -1,   105,
    1142,    -1,    -1,    -1,   110,  1198,    -1,    -1,    32,    -1,
      -1,    -1,   855,   856,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   869,    -1,    -1,    -1,
    1223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   881,    -1,
      -1,   884,    -1,    -1,    -1,   768,    -1,  1189,  1190,  1191,
    1192,    -1,   438,    -1,    -1,    -1,    -1,    -1,    -1,  1252,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
    1212,    -1,    -1,    -1,  1216,  1217,    -1,   183,  1223,    -1,
      -1,  1223,    -1,   469,    -1,    -1,    -1,    -1,    -1,   475,
    1232,  1233,  1234,  1235,  1236,  1237,    -1,    -1,  1291,    -1,
      -1,   487,    -1,   489,    -1,    -1,    -1,    -1,     2,  1251,
    1252,    -1,  1254,    -1,    -1,    -1,   140,   141,    12,   143,
     144,   145,    -1,   147,   148,    -1,    -1,    -1,   152,    -1,
       2,    25,   855,   856,    28,   159,   160,    32,    -1,   163,
      12,    -1,   166,   167,   168,  1287,   869,    -1,   871,  1291,
      -1,    -1,     1,    25,    -1,   878,    28,    -1,   881,    -1,
      -1,   884,    -1,    -1,    -1,    -1,    32,   553,    -1,    -1,
      -1,   557,    -1,    22,    -1,   281,    -1,    26,    27,   565,
     903,   287,    31,    -1,    78,    -1,    -1,    36,    -1,    38,
      39,    -1,    -1,    -1,    89,    44,    -1,    -1,    93,    -1,
      -1,    -1,    51,    -1,    -1,    54,    78,    -1,    -1,    -1,
      -1,   105,   935,    -1,    -1,    -1,   110,    -1,    -1,    -1,
    1063,    -1,    -1,    89,    -1,    74,    -1,    93,    77,    -1,
      79,    -1,    81,   105,    83,    84,    -1,    86,   110,    88,
     346,    90,    -1,    -1,    -1,   140,   141,    -1,   634,    -1,
     145,    -1,   147,   148,    -1,   361,    -1,   152,    -1,    -1,
     109,    -1,    -1,   112,    -1,   160,   115,    -1,    -1,    -1,
      -1,   166,   167,   168,   140,    -1,    -1,    -1,     8,   145,
      -1,   147,   148,    -1,    -1,    -1,   152,    -1,    -1,   183,
      -1,    -1,    22,    -1,   160,    -1,    26,    27,    -1,    -1,
     166,   167,   168,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,   183,    -1,    -1,    44,    -1,   165,    -1,    -1,    -1,
      -1,    51,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     716,    -1,   438,   719,    -1,    -1,    66,    -1,    -1,    -1,
    1063,    -1,     2,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    -1,    12,    83,    -1,    85,    86,    -1,    88,    -1,
      90,    -1,    -1,   469,    -1,    25,    -1,    -1,    28,   475,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,   487,   112,   489,    -1,   115,    -1,   281,    -1,    -1,
      -1,    -1,    -1,   287,   780,    -1,    -1,    -1,  1121,  1122,
     786,    -1,  1125,  1126,    -1,    -1,    -1,    -1,    -1,   281,
      -1,    -1,    -1,    -1,    -1,   287,    -1,    -1,    78,    -1,
    1143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1151,    -1,
    1153,   817,  1155,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    -1,   105,    -1,   553,    -1,    -1,
     110,   557,   346,    -1,    -1,    -1,    22,   843,   844,   565,
      26,    27,    -1,    -1,    -1,    -1,    -1,   361,    -1,    -1,
      36,    -1,    38,    39,   346,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,   872,    -1,   874,   361,
      -1,    -1,    -1,   879,    -1,    -1,   882,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   893,    -1,   895,
      -1,    77,   898,    79,    -1,    -1,    -1,    83,     3,    -1,
      86,    -1,    88,   183,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    32,    -1,    20,    -1,    -1,    -1,    -1,
      25,   927,    -1,   109,   438,    -1,   112,    -1,  1271,   115,
      -1,    36,   938,    -1,   940,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,   438,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,   469,    -1,    -1,    63,    -1,
      -1,   475,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      89,    -1,    -1,   487,    93,   489,    -1,   469,    -1,   165,
      -1,    -1,    87,   475,    -1,    -1,    -1,    -1,    -1,    -1,
     716,    -1,    -1,   719,    99,   487,    -1,   489,   103,    -1,
      -1,   281,    -1,    -1,    -1,   110,    -1,   287,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,  1022,    -1,    -1,    -1,
      -1,   140,   141,  1029,   143,   144,   145,    -1,   147,   148,
     149,    -1,    -1,   152,    -1,    -1,    -1,  1043,    -1,   553,
     159,   160,    -1,   557,   163,    -1,    -1,   166,   167,   168,
      -1,   565,    -1,    -1,   780,  1061,    -1,    -1,   177,    -1,
     786,   553,    -1,    -1,  1070,   557,   346,    -1,  1074,    -1,
      22,    -1,    -1,   565,    26,    27,    -1,    -1,    -1,    31,
      -1,   361,    -1,    -1,    36,    -1,    38,    39,     2,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    12,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      32,    -1,  1128,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    86,    -1,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   872,    -1,   874,    -1,
      -1,    -1,    -1,   879,    -1,    -1,   882,   109,   438,    -1,
     112,    -1,    -1,   115,    78,    -1,    -1,   893,    -1,   895,
      -1,    -1,   898,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,  1189,  1190,  1191,  1192,    -1,    -1,   469,
      -1,   105,    -1,    -1,    -1,   475,   110,    -1,    -1,    -1,
      -1,   927,   716,    -1,    -1,   719,    -1,   487,    -1,   489,
    1216,  1217,   938,   165,   940,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,   716,    -1,    -1,   719,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,   160,   161,
      -1,   163,    -1,     2,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,   177,   780,    -1,    -1,   183,
      -1,    -1,   786,   553,    -1,    -1,    25,   557,    -1,    28,
      -1,    -1,    -1,    -1,    -1,   565,    -1,    -1,   780,    -1,
      -1,    -1,    -1,    -1,   786,    -1,  1022,    -1,    -1,    -1,
      -1,    -1,    -1,  1029,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1043,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,  1061,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1070,    -1,    -1,    -1,  1074,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,   872,    -1,
     874,   110,    -1,    -1,    -1,   879,    -1,   281,   882,    -1,
      -1,    -1,    -1,   287,    -1,    -1,    -1,    -1,    -1,   893,
     872,   895,   874,    -1,   898,    -1,    -1,   879,    -1,    -1,
     882,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   893,  1128,   895,    -1,    -1,   898,    -1,    -1,    -1,
      -1,    -1,    -1,   927,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   938,    -1,   940,    -1,    -1,    -1,
      -1,    -1,   346,    -1,   183,   927,   716,    -1,    -1,   719,
      -1,    -1,    -1,    -1,    -1,    -1,   938,   361,   940,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,  1189,  1190,  1191,  1192,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
    1216,  1217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
     780,    -1,    -1,    -1,    -1,    -1,   786,    -1,  1022,    -1,
      77,    -1,    79,    -1,    -1,  1029,    83,    -1,    85,    86,
      -1,    88,    -1,    90,   438,    -1,    -1,    -1,    -1,  1043,
    1022,    -1,   281,    -1,    -1,    -1,    -1,  1029,   287,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,  1061,   115,    -1,
      -1,  1043,    -1,    -1,     2,   469,  1070,    -1,    -1,    -1,
    1074,   475,    -1,    -1,    12,    -1,    -1,    -1,    -1,  1061,
      -1,    -1,    -1,   487,    -1,   489,    -1,    25,  1070,    -1,
      28,    -1,  1074,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   872,    -1,   874,    -1,    -1,   346,   165,   879,
      -1,    -1,   882,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   361,   893,  1128,   895,    -1,    -1,   898,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      78,    -1,    -1,    31,    -1,    -1,  1128,    -1,    36,   553,
      38,    39,    -1,   557,    -1,    -1,    44,   927,    -1,    -1,
      -1,   565,    -1,    51,    -1,    -1,    -1,   105,   938,    -1,
     940,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1189,  1190,  1191,  1192,    77,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    86,   438,
      88,    -1,    90,    -1,    -1,    -1,    -1,  1189,  1190,  1191,
    1192,    -1,  1216,  1217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
     469,    -1,    -1,    -1,  1216,  1217,   475,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,   487,    -1,
     489,    -1,  1022,    -1,    -1,    -1,    -1,    -1,    -1,  1029,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,  1043,    -1,    -1,    -1,   165,    -1,    12,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1061,    25,    -1,     3,    28,    -1,    -1,    -1,    -1,
    1070,    -1,    -1,    -1,  1074,    -1,    -1,    -1,    17,    18,
      -1,    20,   716,    -1,   553,   719,    25,    -1,   557,    -1,
      -1,    -1,    -1,    -1,    -1,    34,   565,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,   281,    53,    78,    -1,    -1,    -1,   287,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,  1128,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,   105,    -1,    -1,    -1,   780,   110,    87,    -1,
      -1,    -1,   786,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,   111,    -1,   113,    -1,    -1,   116,   346,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1189,
    1190,  1191,  1192,   361,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,    -1,    -1,    -1,    -1,  1216,  1217,    -1,    -1,
     183,    -1,    -1,    -1,    -1,    -1,    32,    44,    45,    46,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,   872,   169,
     874,    -1,   172,    -1,   174,   879,    -1,   716,   882,    -1,
     719,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   893,
      -1,   895,    -1,    -1,   898,    -1,    -1,    84,    -1,    -1,
     438,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   927,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   469,    -1,    -1,   938,    -1,   940,   475,    -1,    -1,
      -1,   780,    -1,    -1,    -1,    -1,    -1,   786,   281,   487,
      -1,   489,    -1,    -1,   287,    -1,    -1,    -1,   145,    -1,
     147,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
       2,   147,   148,   149,    -1,    -1,   152,   153,   154,    -1,
      12,   157,   158,   159,   160,    -1,    -1,   163,    -1,    -1,
     166,   167,   168,    25,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,   193,    -1,    -1,    -1,
      -1,    -1,    -1,   346,    -1,   553,    -1,    -1,  1022,   557,
      -1,    -1,    -1,   210,    -1,  1029,    -1,   565,   361,    -1,
      -1,    -1,    -1,   872,    -1,   874,    -1,    -1,    -1,  1043,
     879,    -1,    -1,   882,    -1,    -1,    78,   234,    -1,    -1,
     237,    -1,    -1,    -1,   893,    -1,   895,  1061,    -1,   898,
      -1,   248,   249,    -1,    -1,    -1,  1070,    -1,    -1,    -1,
    1074,    -1,    -1,   105,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   927,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,     3,   938,
      -1,   940,    -1,    -1,    -1,   438,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,  1128,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,   469,    -1,    -1,    -1,
      -1,    46,   475,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,   183,    -1,    -1,   487,    -1,   489,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,   716,   366,
      -1,   719,    87,  1022,    -1,  1189,  1190,  1191,  1192,    -1,
    1029,    96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,  1043,   110,    -1,    -1,   113,    -1,
      -1,   116,  1216,  1217,    -1,    -1,    -1,    -1,    -1,    -1,
     553,    -1,  1061,    -1,   557,    -1,    -1,    -1,    -1,    22,
      -1,  1070,   565,    26,    27,  1074,    -1,    -1,    31,    -1,
      -1,    -1,   780,    36,    -1,    38,    39,    -1,   786,   281,
     437,    44,    -1,    -1,    -1,   287,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,    -1,   456,
      -1,    -1,    -1,    -1,   461,    -1,   463,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,  1128,
      83,    -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   498,    -1,    -1,   346,    -1,   109,   504,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,   361,
     517,    -1,    -1,    -1,   872,    -1,   874,    -1,    -1,    -1,
      -1,   879,    -1,    -1,   882,    -1,    -1,    -1,    -1,    -1,
    1189,  1190,  1191,  1192,    -1,   893,    -1,   895,    -1,    -1,
     898,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,  1216,  1217,   566,
      -1,    -1,    -1,   716,    -1,    -1,   719,    -1,   575,   927,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
     938,    -1,   940,    -1,    -1,    -1,   438,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,   469,    -1,    -1,
      -1,    -1,    46,   475,    -1,    -1,    50,   780,    -1,    53,
      -1,    -1,    -1,   786,    -1,   487,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,   656,
      -1,    -1,   659,   660,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,  1022,    -1,    -1,    -1,    -1,    -1,
      -1,  1029,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,  1043,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   553,    -1,  1061,    -1,   557,    -1,    -1,    -1,    -1,
      -1,    -1,  1070,   565,    -1,    -1,  1074,    -1,    -1,   872,
      -1,   874,    -1,    -1,    -1,    -1,   879,    -1,    -1,   882,
      -1,   738,    -1,    22,    -1,   742,    -1,    26,    27,    -1,
     893,    -1,   895,    -1,    -1,   898,    -1,    36,   172,    38,
      39,   758,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,   768,    51,    -1,    -1,   772,    -1,    -1,    -1,    -1,
    1128,    -1,    -1,    -1,   927,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   938,    -1,   940,    77,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    88,
     807,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     817,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,   114,   115,    17,    18,    -1,
      20,  1189,  1190,  1191,  1192,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,  1216,  1217,
      50,    -1,    -1,    53,   716,    -1,    -1,   719,    -1,  1022,
      -1,    -1,    -1,    63,    -1,    -1,  1029,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
    1043,    -1,    -1,   900,    -1,   902,    -1,    87,    -1,   906,
      -1,    -1,   909,    -1,    -1,    -1,    96,    97,  1061,    99,
      -1,   101,    -1,   103,    -1,    -1,   106,  1070,    -1,    -1,
     110,  1074,    -1,   113,    -1,   932,   116,    -1,   780,    -1,
      -1,    -1,    -1,    -1,   786,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,   965,    -1,
      44,   968,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,     3,  1128,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    86,    -1,    88,    34,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     872,    -1,   874,    -1,    -1,   109,    55,   879,   112,    -1,
     882,   115,    -1,    -1,    63,    -1,  1189,  1190,  1191,  1192,
      -1,   893,    -1,   895,    -1,    -1,   898,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1216,  1217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   927,    -1,    -1,    -1,    -1,
     109,   165,    -1,    -1,    -1,    -1,   938,   171,   940,    -1,
      -1,    -1,    -1,    -1,  1101,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
      -1,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,    -1,    -1,   166,   167,   168,
     169,   170,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
     172,  1043,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1061,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1070,    -1,
      -1,    -1,  1074,    -1,     0,     1,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,  1128,    53,    54,    -1,
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
      -1,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,    -1,   170,
     171,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
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
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
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
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,
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
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,
      18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   166,   167,
     168,   169,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    90,    -1,    -1,    -1,    -1,
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
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,
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
      -1,   106,   107,   108,    -1,   110,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,   174,
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
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,   173,   174,    -1,     3,   177,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,   110,    -1,   112,   113,
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
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
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
      63,    -1,    65,    -1,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,
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
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,
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
      27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
     122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     109,   110,    -1,   112,   113,    -1,    -1,   116,    -1,    -1,
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
     106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
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
     171,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
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
     168,    -1,   170,    -1,   172,    -1,   174,   175,     3,   177,
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
      -1,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
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
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,   175,     3,   177,     5,     6,     7,     8,     9,    10,
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
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,   111,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    12,    -1,    -1,
      -1,   149,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    34,
     168,    36,    -1,    -1,   172,    40,   174,    -1,    -1,   177,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,   163,    -1,
      -1,   166,   167,   168,   169,    -1,     3,   172,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,    -1,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,     3,     4,
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
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    61,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,   105,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,   164,   165,    25,    -1,    -1,    -1,   170,    -1,   172,
      -1,   174,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,   164,    -1,    25,    -1,    -1,    -1,   170,    -1,
     172,    -1,   174,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,   164,    -1,    25,    -1,    -1,    -1,   170,
      -1,   172,    -1,   174,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,     3,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    12,    -1,    -1,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,   142,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,   172,    -1,   174,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    96,    97,    -1,    99,    -1,   101,    19,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      32,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,   163,    -1,
      -1,   166,   167,   168,   169,    -1,    12,    89,    -1,    -1,
      -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
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
     160,   161,   162,   163,    -1,    89,   166,   167,   168,    93,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    22,    -1,    -1,
      12,    26,    27,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      32,    33,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    88,    -1,    90,    -1,    -1,   120,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    48,    89,    -1,    -1,
      -1,    93,    -1,    -1,   109,    -1,    -1,   112,   140,   141,
     115,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,   170,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    73,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,   120,   166,   167,   168,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,
      93,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    48,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    22,   166,   167,   168,    26,    27,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    36,    -1,    38,    39,
      -1,    32,    33,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    32,    33,    83,    -1,    -1,    86,    -1,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   165,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,   102,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    -1,   166,   167,   168,    -1,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    36,
      19,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    32,    51,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    13,
      77,    -1,    79,    -1,    -1,    19,    -1,    -1,    85,    -1,
      -1,    88,    -1,    90,    73,    -1,    -1,    -1,    32,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      89,    -1,   109,    -1,    93,   112,    -1,    -1,   115,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    89,   175,    -1,   177,    93,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
     124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   124,   170,    -1,    -1,    -1,    89,    -1,    -1,
     177,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,   165,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    12,    -1,    -1,    44,    -1,    -1,    -1,    19,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    89,
      79,    -1,    81,    93,    83,    -1,    -1,    86,    12,    88,
      -1,    90,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
     109,    -1,    -1,   112,   124,    -1,   115,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    89,   165,   177,    -1,    93,
     140,   141,   171,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,   165,   166,   167,   168,   123,
     124,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,   102,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,   177,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    32,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   140,   141,    19,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,    32,    -1,   157,
     158,   159,   160,   161,   124,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,   177,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   120,   166,   167,   168,   124,
      -1,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,   167,   168,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,    -1,    19,    -1,   173,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   123,   124,
     166,   167,   168,    -1,    -1,    89,    -1,   173,    -1,    93,
      -1,   177,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
     124,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    22,    -1,    -1,    -1,    26,    27,
      32,    33,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      12,    -1,    -1,    51,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    83,    -1,    89,    86,    -1,
      88,    93,    90,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,   112,    32,    -1,   115,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,   165,    -1,    -1,
      -1,    -1,    89,   171,    -1,   177,    93,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,   124,    -1,    19,
      -1,    -1,    -1,   175,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    89,
      -1,    -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    89,   166,   167,   168,    93,
      -1,    -1,    -1,    12,    13,    -1,    -1,   177,   102,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    12,
      89,    -1,    -1,   177,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
     173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,   175,    -1,
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
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,
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
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,   124,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,   140,   141,    -1,   143,   144,   145,    51,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    77,    -1,    79,   175,    81,   177,    83,
      -1,    -1,    86,    -1,    88,    32,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,    89,   166,   167,   168,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    32,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,    -1,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    17,    18,   106,    20,    -1,
      -1,   110,    -1,    25,   113,    27,    -1,   116,    -1,    -1,
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
     185,   186,   187,   188,   189,   190,   191,   194,   196,   201,
     202,   203,   206,   207,   211,   213,   216,   219,   221,   223,
     224,   225,   232,   233,   235,   237,   240,   241,   242,   243,
     244,   245,   249,   250,   253,   254,   257,   258,   262,   265,
     266,   294,   297,   298,   318,   319,   320,   321,   322,   323,
     324,   332,   333,   334,   335,   336,   339,   340,   341,   342,
     343,   344,   345,   346,   348,   349,   350,   351,   352,   165,
     186,   336,   120,   325,   326,     3,   208,    14,    22,    36,
      41,    42,    45,    56,    88,   101,   170,   174,   240,   262,
     318,   323,   334,   335,   336,   339,   341,   342,   325,   336,
     109,   297,    90,   208,   186,   312,   336,     8,    22,    36,
      39,    83,    86,    88,   188,   186,   172,     8,    88,   336,
     337,     8,    11,    88,   109,   112,   337,    79,   122,   234,
     336,   234,   336,   234,   336,    26,   298,   336,    27,   115,
     236,   336,   195,   172,     3,    17,    18,    20,    25,    34,
      40,    46,    50,    53,    63,    70,    71,    78,    87,    96,
      97,    99,   101,   103,   106,   110,   113,   116,   210,   212,
     210,   210,   263,   172,   210,   299,   300,    33,   196,   215,
     336,   217,   218,   336,   336,    18,    78,    96,   113,   336,
     336,   336,     8,   172,   222,   223,     4,   288,   311,   336,
     107,   108,   165,   336,   338,   336,   215,   336,   336,   336,
     100,   172,   186,   336,   336,   196,   207,   336,   339,   196,
     207,   336,   210,   295,   336,   236,   336,   336,   336,   336,
     336,   336,   336,     1,   171,   184,   197,   311,   111,   150,
     288,   313,   314,   338,   234,   311,   336,   347,   336,    81,
     186,   170,    85,   192,    47,   114,    56,   210,   210,    54,
      74,    84,   283,   299,   164,   165,   156,   336,    12,    19,
      32,    89,    93,   124,   140,   141,   143,   144,   145,   147,
     148,   149,   151,   152,   153,   154,   155,   157,   158,   159,
     160,   161,   162,   163,   166,   167,   168,   177,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   169,   273,   172,   174,    89,    93,   172,
     186,   165,   336,   336,   336,   210,   311,    56,   170,   196,
      48,   325,   295,   299,   165,   146,   165,   188,   119,   211,
     288,   315,   316,   317,   338,    88,   230,   266,   297,    88,
     112,   226,   295,   228,   266,   297,   210,   172,   215,    33,
      48,   215,   120,   215,   328,    33,    48,   215,   328,   215,
      48,   215,    37,    73,   165,   210,   210,   102,   196,   102,
     125,   196,   273,    83,    86,   193,   315,   172,   172,   196,
     186,   172,   276,   111,   172,   210,   301,   302,     1,   145,
     306,    48,   146,   186,   215,   146,   215,    13,   172,   172,
     215,   315,   223,   146,   165,   336,   336,   165,   170,   215,
     172,   315,   165,   246,   165,   246,   165,   125,   296,   165,
     215,   215,   165,   171,   171,   184,   146,   171,   336,   146,
     173,   146,   173,   175,   328,    48,   146,   175,   328,   123,
     146,   175,     8,     1,   171,   197,    66,   204,   205,   336,
     199,   336,   210,   145,   255,   170,   267,   165,   336,   336,
     336,   336,   234,   336,   234,   336,   336,   336,   336,   336,
     336,   336,     3,    20,    34,    63,   103,   109,   211,   336,
     336,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   336,   336,    69,   338,   338,   338,
     338,   338,   315,   315,   234,   336,   234,   336,    27,    48,
      90,   115,   327,   330,   331,   336,   352,    33,    48,    33,
      48,   102,   172,    48,   175,   210,   234,   336,   215,   165,
     165,   336,   336,   125,   173,   146,   231,   210,   299,   227,
     229,   210,   165,   210,   299,    48,   311,    45,   336,   234,
     336,   172,   215,    45,   336,   234,   336,   215,   234,   336,
      12,    19,    55,   140,   141,   142,   143,   144,   145,   147,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   163,   166,   167,   168,   169,   198,   272,   273,   274,
     336,   198,   200,   125,   125,   186,    35,   186,   336,    35,
     336,   192,   173,   316,   210,   238,   239,    27,    48,    52,
      76,    79,    90,   109,   185,   277,   278,   279,   280,   281,
     264,   302,   146,   173,    34,    50,    97,   101,   174,   214,
     307,   319,   125,   303,   336,   300,   217,   210,   297,   336,
     336,   173,   288,   336,     1,   251,   317,   173,    21,   247,
     307,   319,   146,   171,   173,   173,   313,   173,   313,   186,
     175,   234,   336,   175,   186,   336,   175,   336,   175,   336,
     171,   171,   210,   146,   165,    13,   148,   146,   165,    13,
      37,    73,   165,   172,   311,   170,     1,    31,   210,   259,
     260,   261,    27,    79,    90,   109,   269,   282,   172,   165,
     165,   165,   165,   165,   165,   173,   175,    48,    90,   146,
     173,    17,    20,    25,    46,    53,    63,    71,    87,    99,
     110,   116,   318,    89,    89,    45,   234,   336,    45,   234,
     336,   316,   234,   336,   172,   325,   325,   165,   288,   338,
     317,   210,   255,   165,   210,   210,   255,   255,   165,   336,
     173,   336,    33,   215,    33,   215,   329,   330,   336,    33,
     215,   328,    33,   215,   328,   215,   215,   146,   165,    13,
     165,   336,   336,    35,   186,    35,    35,   186,   102,   196,
      66,   173,   146,   173,    48,    90,   280,   146,   173,   172,
     210,    27,    79,    90,   109,   284,   173,   302,   306,     1,
     311,    69,   338,   210,   173,   173,   171,    75,   117,   171,
     252,   173,   172,   196,   210,   248,   295,   186,   175,   328,
     175,   328,   186,   123,   204,   211,   140,   141,   142,   143,
     144,   159,   163,   168,   170,   274,   336,   111,   336,   198,
     200,   316,     1,   256,   171,     8,   261,   125,   146,   171,
      90,   268,     1,     3,    17,    20,    25,    40,    46,    53,
      63,    70,    71,    87,    99,   103,   106,   110,   116,   172,
     209,   210,   212,   270,   271,   272,   273,   318,   173,   330,
     306,   318,   318,   336,    33,    33,   336,    33,    33,   173,
     175,   175,   316,   215,   215,   255,   170,   255,   255,   170,
     170,   215,   102,    45,   336,    45,   336,   146,   173,   102,
      45,   336,   215,    45,   336,   215,   274,   336,   336,   186,
     186,   336,   186,    35,   210,   165,   239,   196,   210,   279,
     302,   145,   310,    90,   306,   303,   175,    48,   175,   172,
     172,    33,   186,   311,   248,   145,   196,    45,   186,   336,
     175,    45,   186,   336,   175,   336,   198,    13,    37,    73,
      37,    73,   165,   165,   173,   171,    31,    83,    86,   171,
     185,   220,   221,   261,   336,   260,   284,   172,   275,   336,
     140,   148,   275,   275,   303,   102,    45,    45,   102,    45,
      45,    45,    45,   173,   170,   256,   170,   170,   256,   256,
     336,   336,   336,   330,   336,   336,   336,    13,    35,   186,
     165,   310,   173,   174,   214,   288,   309,   319,   150,   289,
     303,    61,   118,   290,   336,   307,   319,   315,   315,   186,
     215,   173,   336,   336,   186,   336,   186,   171,   111,   336,
     198,   200,   198,   200,   165,     8,   221,   220,     1,   145,
     305,   278,   173,     3,   103,   271,   273,   336,   336,   336,
     336,   336,   336,   256,   171,   256,   256,   171,   171,   102,
     102,   102,   102,   336,   186,   289,   303,   310,   175,   311,
     288,   336,     3,    92,   103,   291,   292,   293,   336,   196,
     216,   287,   175,   173,   173,   196,   102,   102,   165,   165,
     165,   165,   221,   174,   214,   304,   319,   105,   285,   173,
     275,   275,   102,   102,   102,   102,   102,   102,   171,   171,
     171,   336,   336,   336,   336,   289,   303,   288,   308,   309,
     319,    48,   175,   293,   118,   146,   125,   151,   153,   154,
     157,   158,    61,   319,   164,   164,   336,   336,     1,   175,
     311,   290,   336,   308,   309,   336,   292,   293,   293,   293,
     293,   293,   293,   291,   175,   304,   319,   175,   165,   286,
     287,   175,   304,   319,   308
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
     198,   198,   198,   199,   199,   199,   199,   199,   199,   200,
     200,   201,   201,   201,   202,   202,   202,   202,   202,   202,
     202,   203,   204,   204,   204,   204,   205,   205,   206,   207,
     207,   207,   207,   207,   207,   208,   208,   209,   209,   209,
     209,   209,   209,   210,   210,   210,   210,   210,   210,   211,
     211,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   214,   214,   214,   214,   215,   215,   216,   216,
     217,   217,   217,   218,   218,   219,   220,   220,   220,   221,
     221,   221,   222,   221,   223,   223,   223,   223,   223,   223,
     223,   224,   224,   224,   224,   226,   225,   227,   225,   228,
     225,   229,   225,   230,   225,   231,   225,   225,   225,   225,
     225,   232,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   234,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   236,   236,   237,   237,
     238,   238,   239,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   241,   241,   241,   241,
     241,   241,   242,   242,   242,   243,   243,   243,   244,   245,
     245,   245,   245,   245,   245,   246,   246,   247,   247,   247,
     248,   248,   249,   250,   250,   251,   251,   252,   252,   252,
     253,   253,   254,   254,   254,   255,   255,   256,   256,   256,
     257,   257,   258,   259,   259,   259,   260,   260,   260,   261,
     261,   263,   264,   262,   265,   265,   265,   267,   268,   266,
     269,   269,   269,   269,   269,   270,   270,   271,   271,   271,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   274,   274,
     275,   275,   276,   277,   277,   278,   278,   279,   279,   279,
     279,   279,   279,   280,   280,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   282,   282,   282,   282,   282,   282,
     283,   283,   283,   284,   284,   284,   284,   284,   284,   285,
     285,   286,   286,   287,   287,   288,   289,   289,   289,   290,
     290,   290,   290,   290,   291,   291,   292,   292,   292,   292,
     292,   292,   292,   293,   293,   294,   294,   294,   295,   295,
     296,   296,   296,   297,   297,   297,   297,   297,   298,   298,
     299,   299,   300,   300,   301,   301,   301,   302,   302,   302,
     303,   303,   303,   304,   304,   304,   304,   304,   304,   304,
     305,   305,   305,   305,   305,   306,   306,   306,   306,   306,
     307,   307,   307,   307,   308,   308,   308,   309,   309,   309,
     309,   309,   310,   310,   310,   310,   310,   311,   311,   311,
     311,   312,   312,   313,   313,   313,   314,   314,   315,   315,
     316,   316,   317,   317,   317,   317,   318,   318,   319,   319,
     319,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     322,   323,   324,   324,   324,   324,   324,   324,   324,   324,
     325,   325,   326,   327,   327,   328,   329,   329,   330,   330,
     330,   331,   331,   331,   331,   331,   331,   332,   332,   333,
     333,   333,   333,   333,   334,   334,   334,   334,   334,   335,
     336,   336,   336,   336,   336,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   336,   336,   336,   337,   337,   338,
     338,   338,   339,   339,   339,   339,   340,   340,   340,   340,
     340,   341,   341,   341,   342,   342,   342,   342,   342,   343,
     343,   343,   343,   344,   344,   345,   345,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     347,   347,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   349,   349,   349,   349,   349,
     349,   349,   350,   350,   350,   350,   351,   351,   351,   351,
     352,   352,   352,   352,   352,   352,   352
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
       0,     7,     2,     3,     3,     1,     2,     1,     1,     3,
       3,     3,     5,     1,     3,     3,     3,     5,     5,     0,
       1,     0,     1,     1,     4,     6,     8,     8,     6,     8,
       8,     4,     1,     3,     3,     5,     1,     3,     3,     4,
       4,     4,     4,     4,     4,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     3,
       4,     3,     1,     1,     3,     3,     1,     3,     2,     1,
       1,     2,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     5,     5,     2,     0,     8,     0,     9,     0,
       8,     0,     9,     0,     8,     0,     9,     3,     3,     5,
       5,     2,     5,     3,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     3,
       4,     5,     5,     3,     3,     6,     7,     6,     7,     4,
       5,     4,     5,     4,     4,     3,     6,     5,     4,     3,
       6,     5,     6,     5,     8,     7,     4,     4,     6,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     4,     7,     5,     3,     6,     2,     3,
       3,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     3,
       6,     6,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     5,     1,     1,     2,     3,     1,     3,     2,     1,
       3,     0,     0,     8,     0,     1,     1,     0,     0,    10,
       3,     3,     5,     5,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     3,     1,     3,     0,     1,     4,     5,     4,
       5,     6,     6,     0,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     1,     0,     1,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     0,
       2,     2,     4,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     3,     4,     4,     2,     4,
       0,     2,     2,     1,     1,     1,     2,     1,     4,     3,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     3,     2,     4,     3,     3,     4,     3,
       0,     2,     2,     2,     1,     0,     2,     2,     2,     1,
       4,     4,     6,     3,     0,     1,     1,     3,     4,     3,
       4,     6,     0,     2,     2,     2,     2,     1,     1,     3,
       3,     1,     3,     1,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     3,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     6,     6,     4,     9,     9,     7,     6,
       6,     4,     9,     9,     7,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     4,     1,     3,     4,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     1,     3,     1,
       2,     2,     2,     2,     2,     8,     8,     9,     9,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     3,     3,     2,     2,     2,     1,     0,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     5,     3,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1
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
#line 518 "chapel.ypp"
                                       { yyblock = (yyval.pblockstmt); }
#line 6706 "bison-chapel.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 523 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6712 "bison-chapel.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 524 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6718 "bison-chapel.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 531 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6724 "bison-chapel.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 536 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6730 "bison-chapel.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 537 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6736 "bison-chapel.cpp"
    break;

  case 27: /* stmt: stmt_level_expr TSEMI  */
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6742 "bison-chapel.cpp"
    break;

  case 28: /* stmt: TATOMIC stmt  */
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6748 "bison-chapel.cpp"
    break;

  case 29: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 562 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6754 "bison-chapel.cpp"
    break;

  case 30: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 563 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6760 "bison-chapel.cpp"
    break;

  case 31: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 564 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6766 "bison-chapel.cpp"
    break;

  case 32: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 565 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6772 "bison-chapel.cpp"
    break;

  case 33: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 566 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6778 "bison-chapel.cpp"
    break;

  case 34: /* stmt: TLABEL ident_def stmt  */
#line 567 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6784 "bison-chapel.cpp"
    break;

  case 35: /* stmt: TLOCAL expr do_stmt  */
#line 568 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6790 "bison-chapel.cpp"
    break;

  case 36: /* stmt: TLOCAL do_stmt  */
#line 569 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6796 "bison-chapel.cpp"
    break;

  case 37: /* stmt: TON expr do_stmt  */
#line 570 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6802 "bison-chapel.cpp"
    break;

  case 38: /* stmt: TSERIAL expr do_stmt  */
#line 571 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6808 "bison-chapel.cpp"
    break;

  case 39: /* stmt: TSERIAL do_stmt  */
#line 572 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6814 "bison-chapel.cpp"
    break;

  case 40: /* stmt: TSYNC stmt  */
#line 573 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6820 "bison-chapel.cpp"
    break;

  case 41: /* stmt: TYIELD expr TSEMI  */
#line 574 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6826 "bison-chapel.cpp"
    break;

  case 42: /* stmt: error TSEMI  */
#line 575 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6832 "bison-chapel.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 581 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 6838 "bison-chapel.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 583 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 6844 "bison-chapel.cpp"
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
#line 6858 "bison-chapel.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 606 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6867 "bison-chapel.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 611 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6876 "bison-chapel.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 616 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6885 "bison-chapel.cpp"
    break;

  case 52: /* access_control: %empty  */
#line 623 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6891 "bison-chapel.cpp"
    break;

  case 53: /* access_control: TPUBLIC  */
#line 624 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6897 "bison-chapel.cpp"
    break;

  case 54: /* access_control: TPRIVATE  */
#line 625 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6903 "bison-chapel.cpp"
    break;

  case 55: /* opt_prototype: %empty  */
#line 629 "chapel.ypp"
             { (yyval.b) = false; }
#line 6909 "bison-chapel.cpp"
    break;

  case 56: /* opt_prototype: TPROTOTYPE  */
#line 630 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6915 "bison-chapel.cpp"
    break;

  case 57: /* include_access_control: %empty  */
#line 634 "chapel.ypp"
           { (yyval.b) = false; }
#line 6921 "bison-chapel.cpp"
    break;

  case 58: /* include_access_control: TPUBLIC  */
#line 635 "chapel.ypp"
           { (yyval.b) = false; }
#line 6927 "bison-chapel.cpp"
    break;

  case 59: /* include_access_control: TPRIVATE  */
#line 636 "chapel.ypp"
           { (yyval.b) = true; }
#line 6933 "bison-chapel.cpp"
    break;

  case 60: /* $@1: %empty  */
#line 641 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6942 "bison-chapel.cpp"
    break;

  case 61: /* include_module_stmt: TINCLUDE $@1 include_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 646 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6950 "bison-chapel.cpp"
    break;

  case 62: /* block_stmt: TLCBR TRCBR  */
#line 661 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6956 "bison-chapel.cpp"
    break;

  case 63: /* block_stmt: TLCBR stmt_ls TRCBR  */
#line 662 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6962 "bison-chapel.cpp"
    break;

  case 64: /* block_stmt: TLCBR error TRCBR  */
#line 663 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6968 "bison-chapel.cpp"
    break;

  case 65: /* stmt_ls: toplevel_stmt  */
#line 668 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6974 "bison-chapel.cpp"
    break;

  case 66: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 669 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6980 "bison-chapel.cpp"
    break;

  case 67: /* renames_ls: expr  */
#line 674 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6990 "bison-chapel.cpp"
    break;

  case 68: /* renames_ls: all_op_name  */
#line 679 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7000 "bison-chapel.cpp"
    break;

  case 69: /* renames_ls: expr TAS expr  */
#line 684 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7010 "bison-chapel.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA expr  */
#line 689 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 7019 "bison-chapel.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 693 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 7028 "bison-chapel.cpp"
    break;

  case 72: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 697 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7037 "bison-chapel.cpp"
    break;

  case 73: /* use_renames_ls: expr  */
#line 706 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 7047 "bison-chapel.cpp"
    break;

  case 74: /* use_renames_ls: expr TAS expr  */
#line 711 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7057 "bison-chapel.cpp"
    break;

  case 75: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 716 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 7067 "bison-chapel.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 721 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 7076 "bison-chapel.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 725 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7085 "bison-chapel.cpp"
    break;

  case 78: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 729 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7094 "bison-chapel.cpp"
    break;

  case 79: /* opt_only_ls: %empty  */
#line 737 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7104 "bison-chapel.cpp"
    break;

  case 81: /* use_access_control: %empty  */
#line 746 "chapel.ypp"
           { (yyval.b) = true; }
#line 7110 "bison-chapel.cpp"
    break;

  case 82: /* use_access_control: TPUBLIC  */
#line 747 "chapel.ypp"
           { (yyval.b) = false; }
#line 7116 "bison-chapel.cpp"
    break;

  case 83: /* use_access_control: TPRIVATE  */
#line 748 "chapel.ypp"
           { (yyval.b) = true; }
#line 7122 "bison-chapel.cpp"
    break;

  case 84: /* use_stmt: use_access_control TUSE use_renames_ls TSEMI  */
#line 752 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7128 "bison-chapel.cpp"
    break;

  case 85: /* use_stmt: use_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 753 "chapel.ypp"
                                                           { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7134 "bison-chapel.cpp"
    break;

  case 86: /* use_stmt: use_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 754 "chapel.ypp"
                                                                    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7140 "bison-chapel.cpp"
    break;

  case 87: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 755 "chapel.ypp"
                                                                           { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7146 "bison-chapel.cpp"
    break;

  case 88: /* use_stmt: use_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 756 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7152 "bison-chapel.cpp"
    break;

  case 89: /* use_stmt: use_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 757 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7158 "bison-chapel.cpp"
    break;

  case 90: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 758 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7164 "bison-chapel.cpp"
    break;

  case 91: /* import_stmt: use_access_control TIMPORT import_ls TSEMI  */
#line 762 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7171 "bison-chapel.cpp"
    break;

  case 92: /* import_expr: expr  */
#line 767 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7177 "bison-chapel.cpp"
    break;

  case 93: /* import_expr: expr TDOT all_op_name  */
#line 768 "chapel.ypp"
                        { std::vector<PotentialRename*>* renames = new std::vector<PotentialRename*>();
                          PotentialRename* nameInMod = new PotentialRename();
                          nameInMod->tag = PotentialRename::SINGLE;
                          nameInMod->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                          renames->push_back(nameInMod);
                          (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), renames); }
#line 7188 "bison-chapel.cpp"
    break;

  case 94: /* import_expr: expr TAS ident_use  */
#line 774 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7194 "bison-chapel.cpp"
    break;

  case 95: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 775 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7200 "bison-chapel.cpp"
    break;

  case 96: /* import_ls: import_expr  */
#line 779 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7206 "bison-chapel.cpp"
    break;

  case 97: /* import_ls: import_ls TCOMMA import_expr  */
#line 780 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7212 "bison-chapel.cpp"
    break;

  case 98: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 784 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7218 "bison-chapel.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 789 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7224 "bison-chapel.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 791 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7230 "bison-chapel.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 793 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7236 "bison-chapel.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 795 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7242 "bison-chapel.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 797 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7248 "bison-chapel.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 799 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7254 "bison-chapel.cpp"
    break;

  case 105: /* opt_label_ident: %empty  */
#line 805 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7260 "bison-chapel.cpp"
    break;

  case 106: /* opt_label_ident: TIDENT  */
#line 806 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7266 "bison-chapel.cpp"
    break;

  case 107: /* ident_fn_def: TIDENT  */
#line 810 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7272 "bison-chapel.cpp"
    break;

  case 108: /* ident_fn_def: TNONE  */
#line 811 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7278 "bison-chapel.cpp"
    break;

  case 109: /* ident_fn_def: TTHIS  */
#line 812 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7284 "bison-chapel.cpp"
    break;

  case 110: /* ident_fn_def: TFALSE  */
#line 813 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7290 "bison-chapel.cpp"
    break;

  case 111: /* ident_fn_def: TTRUE  */
#line 814 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7296 "bison-chapel.cpp"
    break;

  case 112: /* ident_fn_def: internal_type_ident_def  */
#line 815 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7302 "bison-chapel.cpp"
    break;

  case 113: /* ident_def: TIDENT  */
#line 818 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7308 "bison-chapel.cpp"
    break;

  case 114: /* ident_def: TNONE  */
#line 819 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7314 "bison-chapel.cpp"
    break;

  case 115: /* ident_def: TTHIS  */
#line 820 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7320 "bison-chapel.cpp"
    break;

  case 116: /* ident_def: TFALSE  */
#line 821 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7326 "bison-chapel.cpp"
    break;

  case 117: /* ident_def: TTRUE  */
#line 822 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7332 "bison-chapel.cpp"
    break;

  case 118: /* ident_def: internal_type_ident_def  */
#line 823 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7338 "bison-chapel.cpp"
    break;

  case 119: /* ident_use: TIDENT  */
#line 835 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7344 "bison-chapel.cpp"
    break;

  case 120: /* ident_use: TTHIS  */
#line 836 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7350 "bison-chapel.cpp"
    break;

  case 121: /* internal_type_ident_def: TBOOL  */
#line 847 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7356 "bison-chapel.cpp"
    break;

  case 122: /* internal_type_ident_def: TINT  */
#line 848 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7362 "bison-chapel.cpp"
    break;

  case 123: /* internal_type_ident_def: TUINT  */
#line 849 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7368 "bison-chapel.cpp"
    break;

  case 124: /* internal_type_ident_def: TREAL  */
#line 850 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7374 "bison-chapel.cpp"
    break;

  case 125: /* internal_type_ident_def: TIMAG  */
#line 851 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7380 "bison-chapel.cpp"
    break;

  case 126: /* internal_type_ident_def: TCOMPLEX  */
#line 852 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7386 "bison-chapel.cpp"
    break;

  case 127: /* internal_type_ident_def: TBYTES  */
#line 853 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7392 "bison-chapel.cpp"
    break;

  case 128: /* internal_type_ident_def: TSTRING  */
#line 854 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7398 "bison-chapel.cpp"
    break;

  case 129: /* internal_type_ident_def: TSYNC  */
#line 855 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7404 "bison-chapel.cpp"
    break;

  case 130: /* internal_type_ident_def: TSINGLE  */
#line 856 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7410 "bison-chapel.cpp"
    break;

  case 131: /* internal_type_ident_def: TOWNED  */
#line 857 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7416 "bison-chapel.cpp"
    break;

  case 132: /* internal_type_ident_def: TSHARED  */
#line 858 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7422 "bison-chapel.cpp"
    break;

  case 133: /* internal_type_ident_def: TBORROWED  */
#line 859 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7428 "bison-chapel.cpp"
    break;

  case 134: /* internal_type_ident_def: TUNMANAGED  */
#line 860 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7434 "bison-chapel.cpp"
    break;

  case 135: /* internal_type_ident_def: TDOMAIN  */
#line 861 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7440 "bison-chapel.cpp"
    break;

  case 136: /* internal_type_ident_def: TINDEX  */
#line 862 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7446 "bison-chapel.cpp"
    break;

  case 137: /* internal_type_ident_def: TLOCALE  */
#line 863 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7452 "bison-chapel.cpp"
    break;

  case 138: /* internal_type_ident_def: TNOTHING  */
#line 864 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7458 "bison-chapel.cpp"
    break;

  case 139: /* internal_type_ident_def: TVOID  */
#line 865 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7464 "bison-chapel.cpp"
    break;

  case 140: /* scalar_type: TBOOL  */
#line 869 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7470 "bison-chapel.cpp"
    break;

  case 141: /* scalar_type: TENUM  */
#line 870 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7476 "bison-chapel.cpp"
    break;

  case 142: /* scalar_type: TINT  */
#line 871 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7482 "bison-chapel.cpp"
    break;

  case 143: /* scalar_type: TUINT  */
#line 872 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7488 "bison-chapel.cpp"
    break;

  case 144: /* scalar_type: TREAL  */
#line 873 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7494 "bison-chapel.cpp"
    break;

  case 145: /* scalar_type: TIMAG  */
#line 874 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7500 "bison-chapel.cpp"
    break;

  case 146: /* scalar_type: TCOMPLEX  */
#line 875 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7506 "bison-chapel.cpp"
    break;

  case 147: /* scalar_type: TBYTES  */
#line 876 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7512 "bison-chapel.cpp"
    break;

  case 148: /* scalar_type: TSTRING  */
#line 877 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7518 "bison-chapel.cpp"
    break;

  case 149: /* scalar_type: TLOCALE  */
#line 878 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7524 "bison-chapel.cpp"
    break;

  case 150: /* scalar_type: TNOTHING  */
#line 879 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7530 "bison-chapel.cpp"
    break;

  case 151: /* scalar_type: TVOID  */
#line 880 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7536 "bison-chapel.cpp"
    break;

  case 152: /* reserved_type_ident_use: TSYNC  */
#line 887 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7542 "bison-chapel.cpp"
    break;

  case 153: /* reserved_type_ident_use: TSINGLE  */
#line 888 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7548 "bison-chapel.cpp"
    break;

  case 154: /* reserved_type_ident_use: TDOMAIN  */
#line 889 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7554 "bison-chapel.cpp"
    break;

  case 155: /* reserved_type_ident_use: TINDEX  */
#line 890 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7560 "bison-chapel.cpp"
    break;

  case 156: /* do_stmt: TDO stmt  */
#line 894 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7566 "bison-chapel.cpp"
    break;

  case 157: /* do_stmt: block_stmt  */
#line 895 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7572 "bison-chapel.cpp"
    break;

  case 158: /* return_stmt: TRETURN TSEMI  */
#line 899 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7578 "bison-chapel.cpp"
    break;

  case 159: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 900 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7584 "bison-chapel.cpp"
    break;

  case 160: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 904 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-3].pexpr), (yyvsp[-1].pflagset), (yyvsp[0].pch)); }
#line 7590 "bison-chapel.cpp"
    break;

  case 161: /* manager_expr: expr TAS ident_def  */
#line 905 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-2].pexpr), NULL, (yyvsp[0].pch)); }
#line 7596 "bison-chapel.cpp"
    break;

  case 162: /* manager_expr: expr  */
#line 906 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[0].pexpr), NULL, NULL); }
#line 7602 "bison-chapel.cpp"
    break;

  case 163: /* manager_expr_ls: manager_expr  */
#line 910 "chapel.ypp"
                                        { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7608 "bison-chapel.cpp"
    break;

  case 164: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 911 "chapel.ypp"
                                        { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7614 "bison-chapel.cpp"
    break;

  case 165: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 915 "chapel.ypp"
                                  { (yyval.pblockstmt) = buildManageStmt((yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7620 "bison-chapel.cpp"
    break;

  case 167: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 921 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 7626 "bison-chapel.cpp"
    break;

  case 168: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 923 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 7632 "bison-chapel.cpp"
    break;

  case 169: /* class_level_stmt: TSEMI  */
#line 927 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7638 "bison-chapel.cpp"
    break;

  case 171: /* class_level_stmt: TPUBLIC private_decl  */
#line 929 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7644 "bison-chapel.cpp"
    break;

  case 172: /* @2: %empty  */
#line 939 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7650 "bison-chapel.cpp"
    break;

  case 173: /* class_level_stmt: TPRIVATE @2 private_decl  */
#line 940 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7656 "bison-chapel.cpp"
    break;

  case 181: /* forwarding_stmt: TFORWARDING expr TSEMI  */
#line 954 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7662 "bison-chapel.cpp"
    break;

  case 182: /* forwarding_stmt: TFORWARDING expr TEXCEPT renames_ls TSEMI  */
#line 955 "chapel.ypp"
                                             { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7668 "bison-chapel.cpp"
    break;

  case 183: /* forwarding_stmt: TFORWARDING expr TONLY opt_only_ls TSEMI  */
#line 956 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7674 "bison-chapel.cpp"
    break;

  case 184: /* forwarding_stmt: TFORWARDING var_decl_stmt  */
#line 957 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7680 "bison-chapel.cpp"
    break;

  case 185: /* $@3: %empty  */
#line 962 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7689 "bison-chapel.cpp"
    break;

  case 186: /* extern_export_decl_stmt: TEXTERN TRECORD $@3 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 967 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7703 "bison-chapel.cpp"
    break;

  case 187: /* $@4: %empty  */
#line 978 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7712 "bison-chapel.cpp"
    break;

  case 188: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TRECORD $@4 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 983 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7727 "bison-chapel.cpp"
    break;

  case 189: /* $@5: %empty  */
#line 995 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7736 "bison-chapel.cpp"
    break;

  case 190: /* extern_export_decl_stmt: TEXTERN TUNION $@5 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1000 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7750 "bison-chapel.cpp"
    break;

  case 191: /* $@6: %empty  */
#line 1011 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7759 "bison-chapel.cpp"
    break;

  case 192: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TUNION $@6 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1016 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7774 "bison-chapel.cpp"
    break;

  case 193: /* $@7: %empty  */
#line 1028 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7783 "bison-chapel.cpp"
    break;

  case 194: /* extern_export_decl_stmt: TEXPORT TRECORD $@7 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1033 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7797 "bison-chapel.cpp"
    break;

  case 195: /* $@8: %empty  */
#line 1043 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7806 "bison-chapel.cpp"
    break;

  case 196: /* extern_export_decl_stmt: TEXPORT STRINGLITERAL TRECORD $@8 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1048 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7820 "bison-chapel.cpp"
    break;

  case 197: /* extern_export_decl_stmt: TEXTERN opt_expr fn_decl_stmt  */
#line 1059 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7828 "bison-chapel.cpp"
    break;

  case 198: /* extern_export_decl_stmt: TEXPORT opt_expr fn_decl_stmt  */
#line 1063 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7836 "bison-chapel.cpp"
    break;

  case 199: /* extern_export_decl_stmt: TEXTERN opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1068 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7848 "bison-chapel.cpp"
    break;

  case 200: /* extern_export_decl_stmt: TEXPORT opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1076 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7860 "bison-chapel.cpp"
    break;

  case 201: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1087 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7868 "bison-chapel.cpp"
    break;

  case 202: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1093 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7874 "bison-chapel.cpp"
    break;

  case 203: /* loop_stmt: TWHILE expr do_stmt  */
#line 1094 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7880 "bison-chapel.cpp"
    break;

  case 204: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1095 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7886 "bison-chapel.cpp"
    break;

  case 205: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1096 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7892 "bison-chapel.cpp"
    break;

  case 206: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1097 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7898 "bison-chapel.cpp"
    break;

  case 207: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1098 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7904 "bison-chapel.cpp"
    break;

  case 208: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1099 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7910 "bison-chapel.cpp"
    break;

  case 209: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1100 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7916 "bison-chapel.cpp"
    break;

  case 210: /* loop_stmt: TFOR expr do_stmt  */
#line 1101 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7922 "bison-chapel.cpp"
    break;

  case 211: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1102 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7928 "bison-chapel.cpp"
    break;

  case 212: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1103 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7934 "bison-chapel.cpp"
    break;

  case 213: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1104 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7940 "bison-chapel.cpp"
    break;

  case 214: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1105 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7946 "bison-chapel.cpp"
    break;

  case 215: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1106 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7952 "bison-chapel.cpp"
    break;

  case 216: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1107 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7958 "bison-chapel.cpp"
    break;

  case 217: /* loop_stmt: TFORALL expr do_stmt  */
#line 1108 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7964 "bison-chapel.cpp"
    break;

  case 218: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1109 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7970 "bison-chapel.cpp"
    break;

  case 219: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1110 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7976 "bison-chapel.cpp"
    break;

  case 220: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1111 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7982 "bison-chapel.cpp"
    break;

  case 221: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1112 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7988 "bison-chapel.cpp"
    break;

  case 222: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1113 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7994 "bison-chapel.cpp"
    break;

  case 223: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1114 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 8000 "bison-chapel.cpp"
    break;

  case 224: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1115 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 8006 "bison-chapel.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1117 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8016 "bison-chapel.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1123 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8026 "bison-chapel.cpp"
    break;

  case 227: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1129 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8036 "bison-chapel.cpp"
    break;

  case 228: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1135 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8046 "bison-chapel.cpp"
    break;

  case 229: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1141 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8057 "bison-chapel.cpp"
    break;

  case 230: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1148 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8068 "bison-chapel.cpp"
    break;

  case 231: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1155 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8076 "bison-chapel.cpp"
    break;

  case 232: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1159 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8084 "bison-chapel.cpp"
    break;

  case 233: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1165 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 8090 "bison-chapel.cpp"
    break;

  case 234: /* if_stmt: TIF expr TTHEN stmt  */
#line 1169 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8096 "bison-chapel.cpp"
    break;

  case 235: /* if_stmt: TIF expr block_stmt  */
#line 1170 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8102 "bison-chapel.cpp"
    break;

  case 236: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1171 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8108 "bison-chapel.cpp"
    break;

  case 237: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1172 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8114 "bison-chapel.cpp"
    break;

  case 238: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1174 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8120 "bison-chapel.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar block_stmt  */
#line 1175 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8126 "bison-chapel.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1176 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8132 "bison-chapel.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1177 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8138 "bison-chapel.cpp"
    break;

  case 242: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1179 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8145 "bison-chapel.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1181 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8152 "bison-chapel.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1183 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8159 "bison-chapel.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1185 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8166 "bison-chapel.cpp"
    break;

  case 246: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1190 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 8172 "bison-chapel.cpp"
    break;

  case 247: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1191 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 8178 "bison-chapel.cpp"
    break;

  case 248: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1196 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 8184 "bison-chapel.cpp"
    break;

  case 249: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1198 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8193 "bison-chapel.cpp"
    break;

  case 250: /* ifc_formal_ls: ifc_formal  */
#line 1205 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8199 "bison-chapel.cpp"
    break;

  case 251: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1206 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8205 "bison-chapel.cpp"
    break;

  case 252: /* ifc_formal: ident_def  */
#line 1211 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8211 "bison-chapel.cpp"
    break;

  case 253: /* implements_type_ident: TIDENT  */
#line 1216 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 8217 "bison-chapel.cpp"
    break;

  case 254: /* implements_type_ident: TBOOL  */
#line 1217 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 8223 "bison-chapel.cpp"
    break;

  case 255: /* implements_type_ident: TINT  */
#line 1218 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 8229 "bison-chapel.cpp"
    break;

  case 256: /* implements_type_ident: TUINT  */
#line 1219 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 8235 "bison-chapel.cpp"
    break;

  case 257: /* implements_type_ident: TREAL  */
#line 1220 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 8241 "bison-chapel.cpp"
    break;

  case 258: /* implements_type_ident: TIMAG  */
#line 1221 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 8247 "bison-chapel.cpp"
    break;

  case 259: /* implements_type_ident: TCOMPLEX  */
#line 1222 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 8253 "bison-chapel.cpp"
    break;

  case 260: /* implements_type_ident: TBYTES  */
#line 1223 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 8259 "bison-chapel.cpp"
    break;

  case 261: /* implements_type_ident: TSTRING  */
#line 1224 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 8265 "bison-chapel.cpp"
    break;

  case 262: /* implements_type_ident: TLOCALE  */
#line 1225 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 8271 "bison-chapel.cpp"
    break;

  case 263: /* implements_type_ident: TNOTHING  */
#line 1226 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 8277 "bison-chapel.cpp"
    break;

  case 264: /* implements_type_ident: TVOID  */
#line 1227 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 8283 "bison-chapel.cpp"
    break;

  case 265: /* implements_type_ident: implements_type_error_ident  */
#line 1229 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8290 "bison-chapel.cpp"
    break;

  case 266: /* implements_type_error_ident: TNONE  */
#line 1235 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 8296 "bison-chapel.cpp"
    break;

  case 267: /* implements_type_error_ident: TTHIS  */
#line 1236 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 8302 "bison-chapel.cpp"
    break;

  case 268: /* implements_type_error_ident: TFALSE  */
#line 1237 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 8308 "bison-chapel.cpp"
    break;

  case 269: /* implements_type_error_ident: TTRUE  */
#line 1238 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 8314 "bison-chapel.cpp"
    break;

  case 270: /* implements_type_error_ident: TDOMAIN  */
#line 1247 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 8320 "bison-chapel.cpp"
    break;

  case 271: /* implements_type_error_ident: TINDEX  */
#line 1248 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 8326 "bison-chapel.cpp"
    break;

  case 272: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1253 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8332 "bison-chapel.cpp"
    break;

  case 273: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1255 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8339 "bison-chapel.cpp"
    break;

  case 274: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1258 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8346 "bison-chapel.cpp"
    break;

  case 275: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1264 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8352 "bison-chapel.cpp"
    break;

  case 276: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1266 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8359 "bison-chapel.cpp"
    break;

  case 277: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1269 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8366 "bison-chapel.cpp"
    break;

  case 278: /* defer_stmt: TDEFER stmt  */
#line 1274 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8372 "bison-chapel.cpp"
    break;

  case 279: /* try_stmt: TTRY expr TSEMI  */
#line 1277 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 8378 "bison-chapel.cpp"
    break;

  case 280: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1278 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 8384 "bison-chapel.cpp"
    break;

  case 281: /* try_stmt: TTRY assignment_stmt  */
#line 1279 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 8390 "bison-chapel.cpp"
    break;

  case 282: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1280 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 8396 "bison-chapel.cpp"
    break;

  case 283: /* try_stmt: TTRY block_stmt catch_stmt_ls  */
#line 1281 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8402 "bison-chapel.cpp"
    break;

  case 284: /* try_stmt: TTRYBANG block_stmt catch_stmt_ls  */
#line 1282 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8408 "bison-chapel.cpp"
    break;

  case 285: /* catch_stmt_ls: %empty  */
#line 1286 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8414 "bison-chapel.cpp"
    break;

  case 286: /* catch_stmt_ls: catch_stmt_ls catch_stmt  */
#line 1287 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8420 "bison-chapel.cpp"
    break;

  case 287: /* catch_stmt: TCATCH block_stmt  */
#line 1291 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8426 "bison-chapel.cpp"
    break;

  case 288: /* catch_stmt: TCATCH catch_expr block_stmt  */
#line 1292 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8432 "bison-chapel.cpp"
    break;

  case 289: /* catch_stmt: TCATCH TLP catch_expr TRP block_stmt  */
#line 1293 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8438 "bison-chapel.cpp"
    break;

  case 290: /* catch_expr: ident_def  */
#line 1297 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8444 "bison-chapel.cpp"
    break;

  case 291: /* catch_expr: ident_def TCOLON expr  */
#line 1298 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8450 "bison-chapel.cpp"
    break;

  case 292: /* throw_stmt: TTHROW expr TSEMI  */
#line 1302 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8456 "bison-chapel.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1306 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8462 "bison-chapel.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1308 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8468 "bison-chapel.cpp"
    break;

  case 295: /* when_stmt_ls: %empty  */
#line 1312 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8474 "bison-chapel.cpp"
    break;

  case 296: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1313 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8480 "bison-chapel.cpp"
    break;

  case 297: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1318 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8486 "bison-chapel.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE stmt  */
#line 1320 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8492 "bison-chapel.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1322 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8498 "bison-chapel.cpp"
    break;

  case 300: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1329 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8512 "bison-chapel.cpp"
    break;

  case 301: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR error TRCBR  */
#line 1339 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8526 "bison-chapel.cpp"
    break;

  case 302: /* class_tag: TCLASS  */
#line 1352 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8536 "bison-chapel.cpp"
    break;

  case 303: /* class_tag: TRECORD  */
#line 1357 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8546 "bison-chapel.cpp"
    break;

  case 304: /* class_tag: TUNION  */
#line 1362 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8556 "bison-chapel.cpp"
    break;

  case 305: /* opt_inherit: %empty  */
#line 1370 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8562 "bison-chapel.cpp"
    break;

  case 306: /* opt_inherit: TCOLON expr_ls  */
#line 1371 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8568 "bison-chapel.cpp"
    break;

  case 307: /* class_level_stmt_ls: %empty  */
#line 1376 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8574 "bison-chapel.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1378 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8580 "bison-chapel.cpp"
    break;

  case 309: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1380 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8586 "bison-chapel.cpp"
    break;

  case 310: /* enum_decl_stmt: enum_header ident_def TLCBR enum_ls TRCBR  */
#line 1385 "chapel.ypp"
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
#line 8606 "bison-chapel.cpp"
    break;

  case 311: /* enum_decl_stmt: enum_header ident_def TLCBR error TRCBR  */
#line 1401 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8614 "bison-chapel.cpp"
    break;

  case 312: /* enum_header: TENUM  */
#line 1408 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8624 "bison-chapel.cpp"
    break;

  case 313: /* enum_ls: deprecated_enum_item  */
#line 1417 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8636 "bison-chapel.cpp"
    break;

  case 314: /* enum_ls: enum_ls TCOMMA  */
#line 1425 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8644 "bison-chapel.cpp"
    break;

  case 315: /* enum_ls: enum_ls TCOMMA deprecated_enum_item  */
#line 1429 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8652 "bison-chapel.cpp"
    break;

  case 317: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1437 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr), (yyvsp[-1].pch)); }
#line 8658 "bison-chapel.cpp"
    break;

  case 318: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1439 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr)); }
#line 8664 "bison-chapel.cpp"
    break;

  case 319: /* enum_item: ident_def  */
#line 1443 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8670 "bison-chapel.cpp"
    break;

  case 320: /* enum_item: ident_def TASSIGN expr  */
#line 1444 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8676 "bison-chapel.cpp"
    break;

  case 321: /* $@9: %empty  */
#line 1449 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8685 "bison-chapel.cpp"
    break;

  case 322: /* $@10: %empty  */
#line 1454 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8694 "bison-chapel.cpp"
    break;

  case 323: /* lambda_decl_expr: TLAMBDA $@9 req_formal_ls $@10 opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1459 "chapel.ypp"
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
#line 8716 "bison-chapel.cpp"
    break;

  case 324: /* linkage_spec: %empty  */
#line 1481 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8724 "bison-chapel.cpp"
    break;

  case 325: /* linkage_spec: TINLINE  */
#line 1484 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8733 "bison-chapel.cpp"
    break;

  case 326: /* linkage_spec: TOVERRIDE  */
#line 1488 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8742 "bison-chapel.cpp"
    break;

  case 327: /* $@11: %empty  */
#line 1496 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8756 "bison-chapel.cpp"
    break;

  case 328: /* $@12: %empty  */
#line 1506 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8767 "bison-chapel.cpp"
    break;

  case 329: /* fn_decl_stmt: linkage_spec proc_iter_or_op $@11 fn_decl_stmt_inner $@12 opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where opt_function_body_stmt  */
#line 1513 "chapel.ypp"
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
#line 8801 "bison-chapel.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1546 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8809 "bison-chapel.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1550 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8818 "bison-chapel.cpp"
    break;

  case 332: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1555 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8826 "bison-chapel.cpp"
    break;

  case 333: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1559 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8835 "bison-chapel.cpp"
    break;

  case 334: /* fn_decl_stmt_inner: opt_this_intent_tag error opt_formal_ls  */
#line 1564 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8843 "bison-chapel.cpp"
    break;

  case 336: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1571 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8849 "bison-chapel.cpp"
    break;

  case 337: /* fn_ident: ident_fn_def  */
#line 1575 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8855 "bison-chapel.cpp"
    break;

  case 338: /* fn_ident: ident_def TBANG  */
#line 1576 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8861 "bison-chapel.cpp"
    break;

  case 339: /* fn_ident: op_ident  */
#line 1577 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8867 "bison-chapel.cpp"
    break;

  case 340: /* op_ident: TBAND  */
#line 1581 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8873 "bison-chapel.cpp"
    break;

  case 341: /* op_ident: TBOR  */
#line 1582 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8879 "bison-chapel.cpp"
    break;

  case 342: /* op_ident: TBXOR  */
#line 1583 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8885 "bison-chapel.cpp"
    break;

  case 343: /* op_ident: TBNOT  */
#line 1584 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8891 "bison-chapel.cpp"
    break;

  case 344: /* op_ident: TEQUAL  */
#line 1585 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8897 "bison-chapel.cpp"
    break;

  case 345: /* op_ident: TNOTEQUAL  */
#line 1586 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8903 "bison-chapel.cpp"
    break;

  case 346: /* op_ident: TLESSEQUAL  */
#line 1587 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8909 "bison-chapel.cpp"
    break;

  case 347: /* op_ident: TGREATEREQUAL  */
#line 1588 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8915 "bison-chapel.cpp"
    break;

  case 348: /* op_ident: TLESS  */
#line 1589 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8921 "bison-chapel.cpp"
    break;

  case 349: /* op_ident: TGREATER  */
#line 1590 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8927 "bison-chapel.cpp"
    break;

  case 350: /* op_ident: TPLUS  */
#line 1591 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8933 "bison-chapel.cpp"
    break;

  case 351: /* op_ident: TMINUS  */
#line 1592 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8939 "bison-chapel.cpp"
    break;

  case 352: /* op_ident: TSTAR  */
#line 1593 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8945 "bison-chapel.cpp"
    break;

  case 353: /* op_ident: TDIVIDE  */
#line 1594 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8951 "bison-chapel.cpp"
    break;

  case 354: /* op_ident: TSHIFTLEFT  */
#line 1595 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8957 "bison-chapel.cpp"
    break;

  case 355: /* op_ident: TSHIFTRIGHT  */
#line 1596 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8963 "bison-chapel.cpp"
    break;

  case 356: /* op_ident: TMOD  */
#line 1597 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8969 "bison-chapel.cpp"
    break;

  case 357: /* op_ident: TEXP  */
#line 1598 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8975 "bison-chapel.cpp"
    break;

  case 358: /* op_ident: TBANG  */
#line 1599 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8981 "bison-chapel.cpp"
    break;

  case 359: /* op_ident: TBY  */
#line 1600 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8987 "bison-chapel.cpp"
    break;

  case 360: /* op_ident: THASH  */
#line 1601 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8993 "bison-chapel.cpp"
    break;

  case 361: /* op_ident: TALIGN  */
#line 1602 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8999 "bison-chapel.cpp"
    break;

  case 362: /* op_ident: TSWAP  */
#line 1603 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 9005 "bison-chapel.cpp"
    break;

  case 363: /* op_ident: TIO  */
#line 1604 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 9011 "bison-chapel.cpp"
    break;

  case 364: /* op_ident: TINITEQUALS  */
#line 1605 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 9017 "bison-chapel.cpp"
    break;

  case 365: /* op_ident: TCOLON  */
#line 1606 "chapel.ypp"
                 { (yyval.pch) = ":"; }
#line 9023 "bison-chapel.cpp"
    break;

  case 366: /* assignop_ident: TASSIGN  */
#line 1610 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 9029 "bison-chapel.cpp"
    break;

  case 367: /* assignop_ident: TASSIGNPLUS  */
#line 1611 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 9035 "bison-chapel.cpp"
    break;

  case 368: /* assignop_ident: TASSIGNMINUS  */
#line 1612 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 9041 "bison-chapel.cpp"
    break;

  case 369: /* assignop_ident: TASSIGNMULTIPLY  */
#line 1613 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 9047 "bison-chapel.cpp"
    break;

  case 370: /* assignop_ident: TASSIGNDIVIDE  */
#line 1614 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 9053 "bison-chapel.cpp"
    break;

  case 371: /* assignop_ident: TASSIGNMOD  */
#line 1615 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 9059 "bison-chapel.cpp"
    break;

  case 372: /* assignop_ident: TASSIGNEXP  */
#line 1616 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 9065 "bison-chapel.cpp"
    break;

  case 373: /* assignop_ident: TASSIGNBAND  */
#line 1617 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 9071 "bison-chapel.cpp"
    break;

  case 374: /* assignop_ident: TASSIGNBOR  */
#line 1618 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 9077 "bison-chapel.cpp"
    break;

  case 375: /* assignop_ident: TASSIGNBXOR  */
#line 1619 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 9083 "bison-chapel.cpp"
    break;

  case 376: /* assignop_ident: TASSIGNSR  */
#line 1620 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 9089 "bison-chapel.cpp"
    break;

  case 377: /* assignop_ident: TASSIGNSL  */
#line 1621 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 9095 "bison-chapel.cpp"
    break;

  case 378: /* all_op_name: op_ident  */
#line 1625 "chapel.ypp"
                 { (yyval.pch) = (yyvsp[0].pch); }
#line 9101 "bison-chapel.cpp"
    break;

  case 379: /* all_op_name: assignop_ident  */
#line 1626 "chapel.ypp"
                 { (yyval.pch) = (yyvsp[0].pch); }
#line 9107 "bison-chapel.cpp"
    break;

  case 380: /* opt_formal_ls: %empty  */
#line 1630 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 9113 "bison-chapel.cpp"
    break;

  case 381: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1631 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9119 "bison-chapel.cpp"
    break;

  case 382: /* req_formal_ls: TLP formal_ls TRP  */
#line 1635 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9125 "bison-chapel.cpp"
    break;

  case 383: /* formal_ls_inner: formal  */
#line 1639 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 9131 "bison-chapel.cpp"
    break;

  case 384: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 1640 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 9137 "bison-chapel.cpp"
    break;

  case 385: /* formal_ls: %empty  */
#line 1644 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 9143 "bison-chapel.cpp"
    break;

  case 386: /* formal_ls: formal_ls_inner  */
#line 1645 "chapel.ypp"
                           { (yyval.pfnsymbol) = (yyvsp[0].pfnsymbol); }
#line 9149 "bison-chapel.cpp"
    break;

  case 387: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1650 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 9155 "bison-chapel.cpp"
    break;

  case 388: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1652 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 9161 "bison-chapel.cpp"
    break;

  case 389: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1654 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 9167 "bison-chapel.cpp"
    break;

  case 390: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1656 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 9173 "bison-chapel.cpp"
    break;

  case 391: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1658 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9179 "bison-chapel.cpp"
    break;

  case 392: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1660 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 9185 "bison-chapel.cpp"
    break;

  case 393: /* opt_intent_tag: %empty  */
#line 1664 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 9191 "bison-chapel.cpp"
    break;

  case 394: /* opt_intent_tag: required_intent_tag  */
#line 1665 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 9197 "bison-chapel.cpp"
    break;

  case 395: /* required_intent_tag: TIN  */
#line 1669 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 9203 "bison-chapel.cpp"
    break;

  case 396: /* required_intent_tag: TINOUT  */
#line 1670 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 9209 "bison-chapel.cpp"
    break;

  case 397: /* required_intent_tag: TOUT  */
#line 1671 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 9215 "bison-chapel.cpp"
    break;

  case 398: /* required_intent_tag: TCONST  */
#line 1672 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 9221 "bison-chapel.cpp"
    break;

  case 399: /* required_intent_tag: TCONST TIN  */
#line 1673 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 9227 "bison-chapel.cpp"
    break;

  case 400: /* required_intent_tag: TCONST TREF  */
#line 1674 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 9233 "bison-chapel.cpp"
    break;

  case 401: /* required_intent_tag: TPARAM  */
#line 1675 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 9239 "bison-chapel.cpp"
    break;

  case 402: /* required_intent_tag: TREF  */
#line 1676 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 9245 "bison-chapel.cpp"
    break;

  case 403: /* required_intent_tag: TTYPE  */
#line 1677 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 9251 "bison-chapel.cpp"
    break;

  case 404: /* opt_this_intent_tag: %empty  */
#line 1681 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 9257 "bison-chapel.cpp"
    break;

  case 405: /* opt_this_intent_tag: TPARAM  */
#line 1682 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 9263 "bison-chapel.cpp"
    break;

  case 406: /* opt_this_intent_tag: TREF  */
#line 1683 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 9269 "bison-chapel.cpp"
    break;

  case 407: /* opt_this_intent_tag: TCONST TREF  */
#line 1684 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 9275 "bison-chapel.cpp"
    break;

  case 408: /* opt_this_intent_tag: TCONST  */
#line 1685 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 9281 "bison-chapel.cpp"
    break;

  case 409: /* opt_this_intent_tag: TTYPE  */
#line 1686 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 9287 "bison-chapel.cpp"
    break;

  case 410: /* proc_iter_or_op: TPROC  */
#line 1690 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 9293 "bison-chapel.cpp"
    break;

  case 411: /* proc_iter_or_op: TITER  */
#line 1691 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 9299 "bison-chapel.cpp"
    break;

  case 412: /* proc_iter_or_op: TOPERATOR  */
#line 1692 "chapel.ypp"
            { (yyval.procIterOp) = ProcIterOp_OP; }
#line 9305 "bison-chapel.cpp"
    break;

  case 413: /* opt_ret_tag: %empty  */
#line 1696 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9311 "bison-chapel.cpp"
    break;

  case 414: /* opt_ret_tag: TCONST  */
#line 1697 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9317 "bison-chapel.cpp"
    break;

  case 415: /* opt_ret_tag: TCONST TREF  */
#line 1698 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 9323 "bison-chapel.cpp"
    break;

  case 416: /* opt_ret_tag: TREF  */
#line 1699 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 9329 "bison-chapel.cpp"
    break;

  case 417: /* opt_ret_tag: TPARAM  */
#line 1700 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 9335 "bison-chapel.cpp"
    break;

  case 418: /* opt_ret_tag: TTYPE  */
#line 1701 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 9341 "bison-chapel.cpp"
    break;

  case 419: /* opt_throws_error: %empty  */
#line 1705 "chapel.ypp"
          { (yyval.b) = false; }
#line 9347 "bison-chapel.cpp"
    break;

  case 420: /* opt_throws_error: TTHROWS  */
#line 1706 "chapel.ypp"
          { (yyval.b) = true;  }
#line 9353 "bison-chapel.cpp"
    break;

  case 421: /* opt_function_body_stmt: TSEMI  */
#line 1709 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 9359 "bison-chapel.cpp"
    break;

  case 424: /* function_body_stmt: return_stmt  */
#line 1715 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9365 "bison-chapel.cpp"
    break;

  case 425: /* query_expr: TQUERIEDIDENT  */
#line 1719 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9371 "bison-chapel.cpp"
    break;

  case 426: /* var_arg_expr: TDOTDOTDOT  */
#line 1723 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9377 "bison-chapel.cpp"
    break;

  case 427: /* var_arg_expr: TDOTDOTDOT expr  */
#line 1724 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9383 "bison-chapel.cpp"
    break;

  case 428: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 1725 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9393 "bison-chapel.cpp"
    break;

  case 429: /* opt_lifetime_where: %empty  */
#line 1733 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9399 "bison-chapel.cpp"
    break;

  case 430: /* opt_lifetime_where: TWHERE expr  */
#line 1735 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9405 "bison-chapel.cpp"
    break;

  case 431: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 1737 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9411 "bison-chapel.cpp"
    break;

  case 432: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 1739 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9417 "bison-chapel.cpp"
    break;

  case 433: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 1741 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9423 "bison-chapel.cpp"
    break;

  case 434: /* lifetime_components_expr: lifetime_expr  */
#line 1746 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9429 "bison-chapel.cpp"
    break;

  case 435: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 1748 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9435 "bison-chapel.cpp"
    break;

  case 436: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 1752 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9441 "bison-chapel.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 1753 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9447 "bison-chapel.cpp"
    break;

  case 438: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 1754 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9453 "bison-chapel.cpp"
    break;

  case 439: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 1755 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9459 "bison-chapel.cpp"
    break;

  case 440: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 1756 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9465 "bison-chapel.cpp"
    break;

  case 441: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 1757 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9471 "bison-chapel.cpp"
    break;

  case 442: /* lifetime_expr: TRETURN lifetime_ident  */
#line 1758 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9477 "bison-chapel.cpp"
    break;

  case 443: /* lifetime_ident: TIDENT  */
#line 1763 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9483 "bison-chapel.cpp"
    break;

  case 444: /* lifetime_ident: TTHIS  */
#line 1765 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9489 "bison-chapel.cpp"
    break;

  case 445: /* type_alias_decl_stmt: TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1770 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9495 "bison-chapel.cpp"
    break;

  case 446: /* type_alias_decl_stmt: TCONFIG TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1772 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9501 "bison-chapel.cpp"
    break;

  case 447: /* type_alias_decl_stmt: TEXTERN TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1774 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9507 "bison-chapel.cpp"
    break;

  case 448: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 1779 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9524 "bison-chapel.cpp"
    break;

  case 449: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 1792 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9542 "bison-chapel.cpp"
    break;

  case 450: /* opt_init_type: %empty  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9548 "bison-chapel.cpp"
    break;

  case 451: /* opt_init_type: TASSIGN type_level_expr  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9554 "bison-chapel.cpp"
    break;

  case 452: /* opt_init_type: TASSIGN array_type  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9560 "bison-chapel.cpp"
    break;

  case 453: /* var_decl_type: TPARAM  */
#line 1816 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9566 "bison-chapel.cpp"
    break;

  case 454: /* var_decl_type: TCONST  */
#line 1817 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9572 "bison-chapel.cpp"
    break;

  case 455: /* var_decl_type: TREF  */
#line 1818 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9578 "bison-chapel.cpp"
    break;

  case 456: /* var_decl_type: TCONST TREF  */
#line 1819 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9584 "bison-chapel.cpp"
    break;

  case 457: /* var_decl_type: TVAR  */
#line 1820 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9590 "bison-chapel.cpp"
    break;

  case 458: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1825 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9600 "bison-chapel.cpp"
    break;

  case 459: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1831 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9609 "bison-chapel.cpp"
    break;

  case 461: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 1840 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9618 "bison-chapel.cpp"
    break;

  case 462: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 1848 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9624 "bison-chapel.cpp"
    break;

  case 463: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 1850 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9630 "bison-chapel.cpp"
    break;

  case 464: /* tuple_var_decl_component: TUNDERSCORE  */
#line 1855 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9636 "bison-chapel.cpp"
    break;

  case 465: /* tuple_var_decl_component: ident_def  */
#line 1857 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9642 "bison-chapel.cpp"
    break;

  case 466: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 1859 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9648 "bison-chapel.cpp"
    break;

  case 467: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 1864 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9654 "bison-chapel.cpp"
    break;

  case 468: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 1866 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9660 "bison-chapel.cpp"
    break;

  case 469: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 1868 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9666 "bison-chapel.cpp"
    break;

  case 470: /* opt_init_expr: %empty  */
#line 1874 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9672 "bison-chapel.cpp"
    break;

  case 471: /* opt_init_expr: TASSIGN TNOINIT  */
#line 1875 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9678 "bison-chapel.cpp"
    break;

  case 472: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 1876 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9684 "bison-chapel.cpp"
    break;

  case 473: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 1882 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9690 "bison-chapel.cpp"
    break;

  case 474: /* ret_array_type: TLSBR TRSBR  */
#line 1884 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9696 "bison-chapel.cpp"
    break;

  case 475: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9704 "bison-chapel.cpp"
    break;

  case 476: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 1890 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9712 "bison-chapel.cpp"
    break;

  case 477: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 1894 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9718 "bison-chapel.cpp"
    break;

  case 478: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 1896 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9726 "bison-chapel.cpp"
    break;

  case 479: /* ret_array_type: TLSBR error TRSBR  */
#line 1900 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9734 "bison-chapel.cpp"
    break;

  case 480: /* opt_ret_type: %empty  */
#line 1907 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9740 "bison-chapel.cpp"
    break;

  case 481: /* opt_ret_type: TCOLON type_level_expr  */
#line 1908 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9746 "bison-chapel.cpp"
    break;

  case 482: /* opt_ret_type: TCOLON ret_array_type  */
#line 1909 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9752 "bison-chapel.cpp"
    break;

  case 483: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 1910 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9758 "bison-chapel.cpp"
    break;

  case 484: /* opt_ret_type: error  */
#line 1911 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9764 "bison-chapel.cpp"
    break;

  case 485: /* opt_type: %empty  */
#line 1916 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9770 "bison-chapel.cpp"
    break;

  case 486: /* opt_type: TCOLON type_level_expr  */
#line 1917 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9776 "bison-chapel.cpp"
    break;

  case 487: /* opt_type: TCOLON array_type  */
#line 1918 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9782 "bison-chapel.cpp"
    break;

  case 488: /* opt_type: TCOLON reserved_type_ident_use  */
#line 1919 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9788 "bison-chapel.cpp"
    break;

  case 489: /* opt_type: error  */
#line 1920 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9794 "bison-chapel.cpp"
    break;

  case 490: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1941 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9802 "bison-chapel.cpp"
    break;

  case 491: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 1945 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9810 "bison-chapel.cpp"
    break;

  case 492: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 1949 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9822 "bison-chapel.cpp"
    break;

  case 493: /* array_type: TLSBR error TRSBR  */
#line 1957 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9830 "bison-chapel.cpp"
    break;

  case 494: /* opt_formal_array_elt_type: %empty  */
#line 1963 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9836 "bison-chapel.cpp"
    break;

  case 495: /* opt_formal_array_elt_type: type_level_expr  */
#line 1964 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9842 "bison-chapel.cpp"
    break;

  case 496: /* opt_formal_array_elt_type: query_expr  */
#line 1965 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9848 "bison-chapel.cpp"
    break;

  case 497: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9854 "bison-chapel.cpp"
    break;

  case 498: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9860 "bison-chapel.cpp"
    break;

  case 499: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9866 "bison-chapel.cpp"
    break;

  case 500: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9872 "bison-chapel.cpp"
    break;

  case 501: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9878 "bison-chapel.cpp"
    break;

  case 502: /* opt_formal_type: %empty  */
#line 1986 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9884 "bison-chapel.cpp"
    break;

  case 503: /* opt_formal_type: TCOLON type_level_expr  */
#line 1987 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9890 "bison-chapel.cpp"
    break;

  case 504: /* opt_formal_type: TCOLON query_expr  */
#line 1988 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9896 "bison-chapel.cpp"
    break;

  case 505: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 1989 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9902 "bison-chapel.cpp"
    break;

  case 506: /* opt_formal_type: TCOLON formal_array_type  */
#line 1990 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9908 "bison-chapel.cpp"
    break;

  case 507: /* expr_ls: expr  */
#line 1996 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9914 "bison-chapel.cpp"
    break;

  case 508: /* expr_ls: query_expr  */
#line 1997 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9920 "bison-chapel.cpp"
    break;

  case 509: /* expr_ls: expr_ls TCOMMA expr  */
#line 1998 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9926 "bison-chapel.cpp"
    break;

  case 510: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 1999 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9932 "bison-chapel.cpp"
    break;

  case 511: /* simple_expr_ls: expr  */
#line 2003 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9938 "bison-chapel.cpp"
    break;

  case 512: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2004 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9944 "bison-chapel.cpp"
    break;

  case 513: /* tuple_component: TUNDERSCORE  */
#line 2008 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9950 "bison-chapel.cpp"
    break;

  case 514: /* tuple_component: opt_try_expr  */
#line 2009 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9956 "bison-chapel.cpp"
    break;

  case 515: /* tuple_component: query_expr  */
#line 2010 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9962 "bison-chapel.cpp"
    break;

  case 516: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2014 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9968 "bison-chapel.cpp"
    break;

  case 517: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2015 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9974 "bison-chapel.cpp"
    break;

  case 518: /* opt_actual_ls: %empty  */
#line 2019 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9980 "bison-chapel.cpp"
    break;

  case 520: /* actual_ls: actual_expr  */
#line 2024 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9986 "bison-chapel.cpp"
    break;

  case 521: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2025 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9992 "bison-chapel.cpp"
    break;

  case 522: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2029 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9998 "bison-chapel.cpp"
    break;

  case 523: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2030 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 10004 "bison-chapel.cpp"
    break;

  case 524: /* actual_expr: query_expr  */
#line 2031 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10010 "bison-chapel.cpp"
    break;

  case 525: /* actual_expr: opt_try_expr  */
#line 2032 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10016 "bison-chapel.cpp"
    break;

  case 526: /* ident_expr: ident_use  */
#line 2036 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 10022 "bison-chapel.cpp"
    break;

  case 527: /* ident_expr: scalar_type  */
#line 2037 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10028 "bison-chapel.cpp"
    break;

  case 528: /* type_level_expr: sub_type_level_expr  */
#line 2049 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10034 "bison-chapel.cpp"
    break;

  case 529: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2051 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 10040 "bison-chapel.cpp"
    break;

  case 530: /* type_level_expr: TQUESTION  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 10046 "bison-chapel.cpp"
    break;

  case 536: /* sub_type_level_expr: TSINGLE expr  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 10052 "bison-chapel.cpp"
    break;

  case 537: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 10058 "bison-chapel.cpp"
    break;

  case 538: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 10064 "bison-chapel.cpp"
    break;

  case 539: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 10070 "bison-chapel.cpp"
    break;

  case 540: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2072 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pexpr)); }
#line 10076 "bison-chapel.cpp"
    break;

  case 541: /* sub_type_level_expr: TATOMIC expr  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 10082 "bison-chapel.cpp"
    break;

  case 542: /* sub_type_level_expr: TSYNC expr  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 10088 "bison-chapel.cpp"
    break;

  case 543: /* sub_type_level_expr: TOWNED  */
#line 2079 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 10094 "bison-chapel.cpp"
    break;

  case 544: /* sub_type_level_expr: TOWNED expr  */
#line 2081 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 10100 "bison-chapel.cpp"
    break;

  case 545: /* sub_type_level_expr: TUNMANAGED  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 10106 "bison-chapel.cpp"
    break;

  case 546: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2085 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10112 "bison-chapel.cpp"
    break;

  case 547: /* sub_type_level_expr: TSHARED  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 10118 "bison-chapel.cpp"
    break;

  case 548: /* sub_type_level_expr: TSHARED expr  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 10124 "bison-chapel.cpp"
    break;

  case 549: /* sub_type_level_expr: TBORROWED  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 10130 "bison-chapel.cpp"
    break;

  case 550: /* sub_type_level_expr: TBORROWED expr  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10136 "bison-chapel.cpp"
    break;

  case 551: /* sub_type_level_expr: TCLASS  */
#line 2096 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 10142 "bison-chapel.cpp"
    break;

  case 552: /* sub_type_level_expr: TRECORD  */
#line 2098 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 10148 "bison-chapel.cpp"
    break;

  case 553: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2103 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10154 "bison-chapel.cpp"
    break;

  case 554: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2105 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10160 "bison-chapel.cpp"
    break;

  case 555: /* for_expr: TFOR expr TDO expr  */
#line 2107 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10166 "bison-chapel.cpp"
    break;

  case 556: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2109 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10172 "bison-chapel.cpp"
    break;

  case 557: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10178 "bison-chapel.cpp"
    break;

  case 558: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2113 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10184 "bison-chapel.cpp"
    break;

  case 559: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2115 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10190 "bison-chapel.cpp"
    break;

  case 560: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2117 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10196 "bison-chapel.cpp"
    break;

  case 561: /* for_expr: TFORALL expr TDO expr  */
#line 2119 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10202 "bison-chapel.cpp"
    break;

  case 562: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10208 "bison-chapel.cpp"
    break;

  case 563: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10214 "bison-chapel.cpp"
    break;

  case 564: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10220 "bison-chapel.cpp"
    break;

  case 565: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2127 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 10231 "bison-chapel.cpp"
    break;

  case 566: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2134 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 10241 "bison-chapel.cpp"
    break;

  case 567: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2140 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10251 "bison-chapel.cpp"
    break;

  case 568: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2146 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10261 "bison-chapel.cpp"
    break;

  case 569: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2152 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10271 "bison-chapel.cpp"
    break;

  case 570: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2161 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10277 "bison-chapel.cpp"
    break;

  case 571: /* nil_expr: TNIL  */
#line 2170 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 10283 "bison-chapel.cpp"
    break;

  case 579: /* stmt_level_expr: io_expr TIO expr  */
#line 2186 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10289 "bison-chapel.cpp"
    break;

  case 580: /* opt_task_intent_ls: %empty  */
#line 2190 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 10295 "bison-chapel.cpp"
    break;

  case 582: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2195 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10301 "bison-chapel.cpp"
    break;

  case 583: /* task_intent_ls: intent_expr  */
#line 2199 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10307 "bison-chapel.cpp"
    break;

  case 584: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2200 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10313 "bison-chapel.cpp"
    break;

  case 585: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2204 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10319 "bison-chapel.cpp"
    break;

  case 586: /* forall_intent_ls: intent_expr  */
#line 2208 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10325 "bison-chapel.cpp"
    break;

  case 587: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2209 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10331 "bison-chapel.cpp"
    break;

  case 588: /* intent_expr: shadow_var_prefix ident_expr opt_type opt_init_expr  */
#line 2214 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10339 "bison-chapel.cpp"
    break;

  case 589: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2218 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10347 "bison-chapel.cpp"
    break;

  case 590: /* intent_expr: expr TREDUCE ident_expr  */
#line 2222 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10355 "bison-chapel.cpp"
    break;

  case 591: /* shadow_var_prefix: TCONST  */
#line 2228 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10361 "bison-chapel.cpp"
    break;

  case 592: /* shadow_var_prefix: TIN  */
#line 2229 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10367 "bison-chapel.cpp"
    break;

  case 593: /* shadow_var_prefix: TCONST TIN  */
#line 2230 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10373 "bison-chapel.cpp"
    break;

  case 594: /* shadow_var_prefix: TREF  */
#line 2231 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10379 "bison-chapel.cpp"
    break;

  case 595: /* shadow_var_prefix: TCONST TREF  */
#line 2232 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10385 "bison-chapel.cpp"
    break;

  case 596: /* shadow_var_prefix: TVAR  */
#line 2233 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10391 "bison-chapel.cpp"
    break;

  case 598: /* io_expr: io_expr TIO expr  */
#line 2239 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10397 "bison-chapel.cpp"
    break;

  case 599: /* new_maybe_decorated: TNEW  */
#line 2244 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10403 "bison-chapel.cpp"
    break;

  case 600: /* new_maybe_decorated: TNEW TOWNED  */
#line 2246 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10411 "bison-chapel.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW TSHARED  */
#line 2250 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10419 "bison-chapel.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2254 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10427 "bison-chapel.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2258 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10435 "bison-chapel.cpp"
    break;

  case 604: /* new_expr: new_maybe_decorated expr  */
#line 2266 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10442 "bison-chapel.cpp"
    break;

  case 605: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2271 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10452 "bison-chapel.cpp"
    break;

  case 606: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2277 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10462 "bison-chapel.cpp"
    break;

  case 607: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2283 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10473 "bison-chapel.cpp"
    break;

  case 608: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2290 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10484 "bison-chapel.cpp"
    break;

  case 609: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2300 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10490 "bison-chapel.cpp"
    break;

  case 619: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2317 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10496 "bison-chapel.cpp"
    break;

  case 620: /* expr: expr TCOLON expr  */
#line 2319 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10502 "bison-chapel.cpp"
    break;

  case 621: /* expr: expr TDOTDOT expr  */
#line 2321 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10508 "bison-chapel.cpp"
    break;

  case 622: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2323 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10514 "bison-chapel.cpp"
    break;

  case 623: /* expr: expr TDOTDOT  */
#line 2338 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10520 "bison-chapel.cpp"
    break;

  case 624: /* expr: TDOTDOT expr  */
#line 2340 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10526 "bison-chapel.cpp"
    break;

  case 625: /* expr: TDOTDOTOPENHIGH expr  */
#line 2342 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10532 "bison-chapel.cpp"
    break;

  case 626: /* expr: TDOTDOT  */
#line 2344 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10538 "bison-chapel.cpp"
    break;

  case 627: /* opt_expr: %empty  */
#line 2348 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10544 "bison-chapel.cpp"
    break;

  case 628: /* opt_expr: expr  */
#line 2349 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10550 "bison-chapel.cpp"
    break;

  case 629: /* opt_try_expr: TTRY expr  */
#line 2353 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10556 "bison-chapel.cpp"
    break;

  case 630: /* opt_try_expr: TTRYBANG expr  */
#line 2354 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10562 "bison-chapel.cpp"
    break;

  case 631: /* opt_try_expr: expr  */
#line 2355 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10568 "bison-chapel.cpp"
    break;

  case 636: /* call_base_expr: lhs_expr  */
#line 2371 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10574 "bison-chapel.cpp"
    break;

  case 637: /* call_base_expr: expr TBANG  */
#line 2372 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10580 "bison-chapel.cpp"
    break;

  case 638: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2373 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10586 "bison-chapel.cpp"
    break;

  case 639: /* call_base_expr: lambda_decl_expr  */
#line 2374 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10592 "bison-chapel.cpp"
    break;

  case 641: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2379 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10598 "bison-chapel.cpp"
    break;

  case 642: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2380 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10604 "bison-chapel.cpp"
    break;

  case 643: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2381 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10610 "bison-chapel.cpp"
    break;

  case 644: /* dot_expr: expr TDOT ident_use  */
#line 2385 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10616 "bison-chapel.cpp"
    break;

  case 645: /* dot_expr: expr TDOT TTYPE  */
#line 2386 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10622 "bison-chapel.cpp"
    break;

  case 646: /* dot_expr: expr TDOT TDOMAIN  */
#line 2387 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10628 "bison-chapel.cpp"
    break;

  case 647: /* dot_expr: expr TDOT TLOCALE  */
#line 2388 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10634 "bison-chapel.cpp"
    break;

  case 648: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2389 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10640 "bison-chapel.cpp"
    break;

  case 649: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2397 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10646 "bison-chapel.cpp"
    break;

  case 650: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2398 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10652 "bison-chapel.cpp"
    break;

  case 651: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2399 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10658 "bison-chapel.cpp"
    break;

  case 652: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2400 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10664 "bison-chapel.cpp"
    break;

  case 653: /* bool_literal: TFALSE  */
#line 2404 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10670 "bison-chapel.cpp"
    break;

  case 654: /* bool_literal: TTRUE  */
#line 2405 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10676 "bison-chapel.cpp"
    break;

  case 655: /* str_bytes_literal: STRINGLITERAL  */
#line 2409 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10682 "bison-chapel.cpp"
    break;

  case 656: /* str_bytes_literal: BYTESLITERAL  */
#line 2410 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10688 "bison-chapel.cpp"
    break;

  case 659: /* literal_expr: INTLITERAL  */
#line 2416 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10694 "bison-chapel.cpp"
    break;

  case 660: /* literal_expr: REALLITERAL  */
#line 2417 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10700 "bison-chapel.cpp"
    break;

  case 661: /* literal_expr: IMAGLITERAL  */
#line 2418 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10706 "bison-chapel.cpp"
    break;

  case 662: /* literal_expr: CSTRINGLITERAL  */
#line 2419 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10712 "bison-chapel.cpp"
    break;

  case 663: /* literal_expr: TNONE  */
#line 2420 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10718 "bison-chapel.cpp"
    break;

  case 664: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2421 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10725 "bison-chapel.cpp"
    break;

  case 665: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2423 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10732 "bison-chapel.cpp"
    break;

  case 666: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2425 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10738 "bison-chapel.cpp"
    break;

  case 667: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2426 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10744 "bison-chapel.cpp"
    break;

  case 668: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2428 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10752 "bison-chapel.cpp"
    break;

  case 669: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2432 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10760 "bison-chapel.cpp"
    break;

  case 670: /* assoc_expr_ls: expr TALIAS expr  */
#line 2439 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10766 "bison-chapel.cpp"
    break;

  case 671: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2440 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10772 "bison-chapel.cpp"
    break;

  case 672: /* binary_op_expr: expr TPLUS expr  */
#line 2444 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10778 "bison-chapel.cpp"
    break;

  case 673: /* binary_op_expr: expr TMINUS expr  */
#line 2445 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10784 "bison-chapel.cpp"
    break;

  case 674: /* binary_op_expr: expr TSTAR expr  */
#line 2446 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10790 "bison-chapel.cpp"
    break;

  case 675: /* binary_op_expr: expr TDIVIDE expr  */
#line 2447 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10796 "bison-chapel.cpp"
    break;

  case 676: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2448 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10802 "bison-chapel.cpp"
    break;

  case 677: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2449 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10808 "bison-chapel.cpp"
    break;

  case 678: /* binary_op_expr: expr TMOD expr  */
#line 2450 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10814 "bison-chapel.cpp"
    break;

  case 679: /* binary_op_expr: expr TEQUAL expr  */
#line 2451 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10820 "bison-chapel.cpp"
    break;

  case 680: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2452 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10826 "bison-chapel.cpp"
    break;

  case 681: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2453 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10832 "bison-chapel.cpp"
    break;

  case 682: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2454 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10838 "bison-chapel.cpp"
    break;

  case 683: /* binary_op_expr: expr TLESS expr  */
#line 2455 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10844 "bison-chapel.cpp"
    break;

  case 684: /* binary_op_expr: expr TGREATER expr  */
#line 2456 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10850 "bison-chapel.cpp"
    break;

  case 685: /* binary_op_expr: expr TBAND expr  */
#line 2457 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10856 "bison-chapel.cpp"
    break;

  case 686: /* binary_op_expr: expr TBOR expr  */
#line 2458 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10862 "bison-chapel.cpp"
    break;

  case 687: /* binary_op_expr: expr TBXOR expr  */
#line 2459 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10868 "bison-chapel.cpp"
    break;

  case 688: /* binary_op_expr: expr TAND expr  */
#line 2460 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10874 "bison-chapel.cpp"
    break;

  case 689: /* binary_op_expr: expr TOR expr  */
#line 2461 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10880 "bison-chapel.cpp"
    break;

  case 690: /* binary_op_expr: expr TEXP expr  */
#line 2462 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10886 "bison-chapel.cpp"
    break;

  case 691: /* binary_op_expr: expr TBY expr  */
#line 2463 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10892 "bison-chapel.cpp"
    break;

  case 692: /* binary_op_expr: expr TALIGN expr  */
#line 2464 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10898 "bison-chapel.cpp"
    break;

  case 693: /* binary_op_expr: expr THASH expr  */
#line 2465 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10904 "bison-chapel.cpp"
    break;

  case 694: /* binary_op_expr: expr TDMAPPED expr  */
#line 2466 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10911 "bison-chapel.cpp"
    break;

  case 695: /* unary_op_expr: TPLUS expr  */
#line 2471 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10917 "bison-chapel.cpp"
    break;

  case 696: /* unary_op_expr: TMINUS expr  */
#line 2472 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10923 "bison-chapel.cpp"
    break;

  case 697: /* unary_op_expr: TMINUSMINUS expr  */
#line 2473 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10929 "bison-chapel.cpp"
    break;

  case 698: /* unary_op_expr: TPLUSPLUS expr  */
#line 2474 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10935 "bison-chapel.cpp"
    break;

  case 699: /* unary_op_expr: TBANG expr  */
#line 2475 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10941 "bison-chapel.cpp"
    break;

  case 700: /* unary_op_expr: expr TBANG  */
#line 2476 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10947 "bison-chapel.cpp"
    break;

  case 701: /* unary_op_expr: TBNOT expr  */
#line 2477 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10953 "bison-chapel.cpp"
    break;

  case 702: /* reduce_expr: expr TREDUCE expr  */
#line 2481 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10959 "bison-chapel.cpp"
    break;

  case 703: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2482 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10965 "bison-chapel.cpp"
    break;

  case 704: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2483 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10971 "bison-chapel.cpp"
    break;

  case 705: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2484 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10977 "bison-chapel.cpp"
    break;

  case 706: /* scan_expr: expr TSCAN expr  */
#line 2488 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10983 "bison-chapel.cpp"
    break;

  case 707: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2489 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10989 "bison-chapel.cpp"
    break;

  case 708: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2490 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10995 "bison-chapel.cpp"
    break;

  case 709: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2491 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 11001 "bison-chapel.cpp"
    break;

  case 710: /* reduce_scan_op_expr: TPLUS  */
#line 2496 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 11007 "bison-chapel.cpp"
    break;

  case 711: /* reduce_scan_op_expr: TSTAR  */
#line 2497 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 11013 "bison-chapel.cpp"
    break;

  case 712: /* reduce_scan_op_expr: TAND  */
#line 2498 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 11019 "bison-chapel.cpp"
    break;

  case 713: /* reduce_scan_op_expr: TOR  */
#line 2499 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 11025 "bison-chapel.cpp"
    break;

  case 714: /* reduce_scan_op_expr: TBAND  */
#line 2500 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 11031 "bison-chapel.cpp"
    break;

  case 715: /* reduce_scan_op_expr: TBOR  */
#line 2501 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 11037 "bison-chapel.cpp"
    break;

  case 716: /* reduce_scan_op_expr: TBXOR  */
#line 2502 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 11043 "bison-chapel.cpp"
    break;


#line 11047 "bison-chapel.cpp"

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
