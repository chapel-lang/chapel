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
  YYSYMBOL_tryable_stmt = 187,             /* tryable_stmt  */
  YYSYMBOL_deprecated_decl_stmt = 188,     /* deprecated_decl_stmt  */
  YYSYMBOL_deprecated_decl_base = 189,     /* deprecated_decl_base  */
  YYSYMBOL_module_decl_start = 190,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 191,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 192,           /* access_control  */
  YYSYMBOL_opt_prototype = 193,            /* opt_prototype  */
  YYSYMBOL_include_access_control = 194,   /* include_access_control  */
  YYSYMBOL_include_module_stmt = 195,      /* include_module_stmt  */
  YYSYMBOL_196_1 = 196,                    /* $@1  */
  YYSYMBOL_block_stmt = 197,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 198,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 199,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 200,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 201,              /* opt_only_ls  */
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
  YYSYMBOL_try_token = 246,                /* try_token  */
  YYSYMBOL_try_stmt = 247,                 /* try_stmt  */
  YYSYMBOL_catch_stmt_ls = 248,            /* catch_stmt_ls  */
  YYSYMBOL_catch_stmt = 249,               /* catch_stmt  */
  YYSYMBOL_catch_expr = 250,               /* catch_expr  */
  YYSYMBOL_throw_stmt = 251,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 252,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 253,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 254,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 255,          /* class_decl_stmt  */
  YYSYMBOL_class_tag = 256,                /* class_tag  */
  YYSYMBOL_opt_inherit = 257,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 258,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 259,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 260,              /* enum_header  */
  YYSYMBOL_enum_ls = 261,                  /* enum_ls  */
  YYSYMBOL_deprecated_enum_item = 262,     /* deprecated_enum_item  */
  YYSYMBOL_enum_item = 263,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 264,         /* lambda_decl_expr  */
  YYSYMBOL_265_9 = 265,                    /* $@9  */
  YYSYMBOL_266_10 = 266,                   /* $@10  */
  YYSYMBOL_linkage_spec = 267,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt = 268,             /* fn_decl_stmt  */
  YYSYMBOL_269_11 = 269,                   /* $@11  */
  YYSYMBOL_270_12 = 270,                   /* $@12  */
  YYSYMBOL_fn_decl_stmt_inner = 271,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_receiver_expr = 272,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 273,                 /* fn_ident  */
  YYSYMBOL_op_ident = 274,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 275,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 276,              /* all_op_name  */
  YYSYMBOL_opt_formal_ls = 277,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 278,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 279,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 280,                /* formal_ls  */
  YYSYMBOL_formal = 281,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 282,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 283,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 284,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 285,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 286,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 287,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 288,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 289,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 290,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 291,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 292,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 293, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 294,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 295,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 296,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_inner = 297, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 298,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 299,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 300,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 301,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 302,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 303, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 304, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 305,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 306,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 307,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 308,                 /* opt_type  */
  YYSYMBOL_array_type = 309,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 310, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 311,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 312,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 313,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 314,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 315,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 316,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 317,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 318,                /* actual_ls  */
  YYSYMBOL_actual_expr = 319,              /* actual_expr  */
  YYSYMBOL_ident_expr = 320,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 321,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 322,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 323,                 /* for_expr  */
  YYSYMBOL_cond_expr = 324,                /* cond_expr  */
  YYSYMBOL_nil_expr = 325,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 326,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 327,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 328,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 329,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 330,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 331,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 332,              /* intent_expr  */
  YYSYMBOL_shadow_var_prefix = 333,        /* shadow_var_prefix  */
  YYSYMBOL_io_expr = 334,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 335,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 336,                 /* new_expr  */
  YYSYMBOL_let_expr = 337,                 /* let_expr  */
  YYSYMBOL_expr = 338,                     /* expr  */
  YYSYMBOL_opt_expr = 339,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 340,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 341,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 342,           /* call_base_expr  */
  YYSYMBOL_call_expr = 343,                /* call_expr  */
  YYSYMBOL_dot_expr = 344,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 345,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 346,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 347,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 348,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 349,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 350,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 351,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 352,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 353,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 354       /* reduce_scan_op_expr  */
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

#line 529 "bison-chapel.cpp"

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
#define YYLAST   21208

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  174
/* YYNRULES -- Number of rules.  */
#define YYNRULES  714
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1287

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
     553,   554,   555,   556,   557,   558,   559,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   582,   583,   585,   590,   591,   595,   608,
     613,   618,   626,   627,   628,   632,   633,   637,   638,   639,
     644,   643,   664,   665,   666,   671,   672,   677,   682,   687,
     692,   696,   700,   709,   714,   719,   724,   728,   732,   740,
     745,   749,   750,   751,   755,   756,   757,   758,   759,   760,
     761,   765,   770,   771,   777,   778,   782,   783,   787,   791,
     793,   795,   797,   799,   801,   808,   809,   813,   814,   815,
     816,   817,   818,   821,   822,   823,   824,   825,   826,   838,
     839,   850,   851,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,   866,   867,   868,
     872,   873,   874,   875,   876,   877,   878,   879,   880,   881,
     882,   883,   890,   891,   892,   893,   897,   898,   902,   903,
     907,   908,   909,   913,   914,   918,   922,   923,   925,   930,
     931,   932,   942,   942,   947,   948,   949,   950,   951,   952,
     953,   957,   958,   959,   960,   965,   964,   981,   980,   998,
     997,  1014,  1013,  1031,  1030,  1046,  1045,  1061,  1065,  1070,
    1078,  1089,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,
    1114,  1115,  1116,  1117,  1118,  1119,  1125,  1131,  1137,  1143,
    1150,  1157,  1161,  1168,  1172,  1173,  1174,  1175,  1177,  1178,
    1179,  1180,  1182,  1184,  1186,  1188,  1193,  1194,  1198,  1200,
    1208,  1209,  1214,  1219,  1220,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1238,  1239,  1240,  1241,
    1250,  1251,  1255,  1257,  1260,  1266,  1268,  1271,  1277,  1280,
    1281,  1284,  1285,  1289,  1290,  1294,  1295,  1296,  1300,  1301,
    1305,  1308,  1310,  1315,  1316,  1320,  1322,  1324,  1331,  1341,
    1355,  1360,  1365,  1373,  1374,  1379,  1380,  1382,  1387,  1403,
    1410,  1419,  1427,  1431,  1438,  1439,  1441,  1446,  1447,  1452,
    1457,  1451,  1484,  1487,  1491,  1499,  1509,  1498,  1548,  1552,
    1557,  1561,  1566,  1573,  1574,  1578,  1579,  1580,  1584,  1585,
    1586,  1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,
    1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1605,
    1606,  1607,  1608,  1609,  1613,  1614,  1615,  1616,  1617,  1618,
    1619,  1620,  1621,  1622,  1623,  1624,  1628,  1629,  1633,  1634,
    1638,  1642,  1643,  1647,  1648,  1652,  1654,  1656,  1658,  1660,
    1662,  1667,  1668,  1672,  1673,  1674,  1675,  1676,  1677,  1678,
    1679,  1680,  1684,  1685,  1686,  1687,  1688,  1689,  1693,  1694,
    1695,  1699,  1700,  1701,  1702,  1703,  1704,  1708,  1709,  1712,
    1713,  1717,  1718,  1722,  1726,  1727,  1728,  1736,  1737,  1739,
    1741,  1743,  1748,  1750,  1755,  1756,  1757,  1758,  1759,  1760,
    1761,  1765,  1767,  1772,  1774,  1776,  1781,  1794,  1811,  1812,
    1814,  1819,  1820,  1821,  1822,  1823,  1827,  1833,  1841,  1842,
    1850,  1852,  1857,  1859,  1861,  1866,  1868,  1870,  1877,  1878,
    1879,  1884,  1886,  1888,  1892,  1896,  1898,  1902,  1910,  1911,
    1912,  1913,  1914,  1919,  1920,  1921,  1922,  1923,  1943,  1947,
    1951,  1959,  1966,  1967,  1968,  1972,  1974,  1980,  1982,  1984,
    1989,  1990,  1991,  1992,  1993,  1999,  2000,  2001,  2002,  2006,
    2007,  2011,  2012,  2013,  2017,  2018,  2022,  2023,  2027,  2028,
    2032,  2033,  2034,  2035,  2039,  2040,  2051,  2053,  2055,  2061,
    2062,  2063,  2064,  2065,  2066,  2068,  2070,  2072,  2074,  2076,
    2078,  2081,  2083,  2085,  2087,  2089,  2091,  2093,  2095,  2098,
    2100,  2105,  2107,  2109,  2111,  2113,  2115,  2117,  2119,  2121,
    2123,  2125,  2127,  2129,  2136,  2142,  2148,  2154,  2163,  2173,
    2181,  2182,  2183,  2184,  2185,  2186,  2187,  2188,  2193,  2194,
    2198,  2202,  2203,  2207,  2211,  2212,  2216,  2220,  2224,  2231,
    2232,  2233,  2234,  2235,  2236,  2240,  2241,  2246,  2248,  2252,
    2256,  2260,  2268,  2273,  2279,  2285,  2292,  2302,  2310,  2311,
    2312,  2313,  2314,  2315,  2316,  2317,  2318,  2319,  2321,  2323,
    2325,  2340,  2342,  2344,  2346,  2351,  2352,  2356,  2357,  2358,
    2362,  2363,  2364,  2365,  2374,  2375,  2376,  2377,  2378,  2382,
    2383,  2384,  2388,  2389,  2390,  2391,  2392,  2400,  2401,  2402,
    2403,  2407,  2408,  2412,  2413,  2417,  2418,  2419,  2420,  2421,
    2422,  2423,  2424,  2426,  2428,  2429,  2430,  2434,  2442,  2443,
    2447,  2448,  2449,  2450,  2451,  2452,  2453,  2454,  2455,  2456,
    2457,  2458,  2459,  2460,  2461,  2462,  2463,  2464,  2465,  2466,
    2467,  2468,  2469,  2474,  2475,  2476,  2477,  2478,  2479,  2480,
    2484,  2485,  2486,  2487,  2491,  2492,  2493,  2494,  2499,  2500,
    2501,  2502,  2503,  2504,  2505
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
  "toplevel_stmt", "pragma_ls", "stmt", "tryable_stmt",
  "deprecated_decl_stmt", "deprecated_decl_base", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype",
  "include_access_control", "include_module_stmt", "$@1", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls",
  "use_access_control", "use_stmt", "import_stmt", "import_expr",
  "import_ls", "require_stmt", "assignment_stmt", "opt_label_ident",
  "ident_fn_def", "ident_def", "ident_use", "internal_type_ident_def",
  "scalar_type", "reserved_type_ident_use", "do_stmt", "return_stmt",
  "manager_expr", "manager_expr_ls", "manage_stmt",
  "deprecated_class_level_stmt", "class_level_stmt", "@2", "private_decl",
  "forwarding_stmt", "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_token",
  "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt",
  "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header", "enum_ls", "deprecated_enum_item", "enum_item",
  "lambda_decl_expr", "$@9", "$@10", "linkage_spec", "fn_decl_stmt",
  "$@11", "$@12", "fn_decl_stmt_inner", "fn_decl_receiver_expr",
  "fn_ident", "op_ident", "assignop_ident", "all_op_name", "opt_formal_ls",
  "req_formal_ls", "formal_ls_inner", "formal_ls", "formal",
  "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_iter_or_op", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "var_arg_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_stmt", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "opt_ret_type", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "sub_type_level_expr", "for_expr",
  "cond_expr", "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "shadow_var_prefix", "io_expr",
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

#define YYPACT_NINF (-1153)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-715)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1153,    76,  3821, -1153,   -53,    61, -1153, -1153, -1153, -1153,
   -1153, -1153, 12187,    75,   206,   207, 15222,   218, 20657,    75,
   12187,   244,   241,   263,   206,  5221, 12187,  2237,  5221,    57,
   20744, 12360,  8363,   302,  9057, 10622, 10622,  3634,  9230,   385,
   -1153,   274, -1153,   430, 20831, 20831, 20831, -1153,  3130, 10795,
     461, 12187, 12187,   336, -1153,   467,   468, 12187, -1153, 15222,
   -1153, 12187,   488,   369,   220,  2579,   499, 20918, -1153, 10970,
    8536, 12187, 10795, 15222, 12187,   464,   525,   425,  5221,   542,
   12187,   550, -1153, -1153, 20831,   569, -1153, 15222, -1153,   570,
    9230, 12187, -1153, 12187, -1153, 12187, -1153, -1153, 14737, 12187,
   -1153, 12187, -1153, -1153, -1153,  4171,  7667,  9405, 12187, -1153,
    5046, -1153, -1153, -1153, -1153,   437, -1153,   552, -1153, -1153,
      50, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153,
   -1153, -1153, -1153, -1153, -1153, -1153, -1153,   582, -1153, -1153,
   -1153, -1153,  7492, -1153, -1153, -1153, -1153, 20831, -1153, 20831,
     429,   227, -1153, -1153,  3130, -1153,   493, -1153,   500, -1153,
   -1153,   521,   536,   548, 12187,   543,   549, 20051,  2845,   221,
     556,   558, -1153, -1153,   313, -1153, -1153, -1153, -1153, -1153,
     459, -1153, -1153, -1153, 12187, 12187, 12187, 20831, -1153, 12187,
   10970, 10970,   593,   476, -1153, -1153, -1153, -1153, 20051,   485,
   -1153, -1153,   541,  5221, -1153, -1153,   560,   -34,   545, 16951,
   20831,  3130, -1153,   564, -1153,   138, 20051,  2728, -1153, -1153,
    9578,   130,  2624, -1153, -1153,   597,  8711,   646, 21005, 20051,
     540,   209, -1153, 21092, 20831, -1153,   540, 20831,   566,    32,
   16665,    78,  1472,    32, 16705,   322, -1153, 15596, 20831, 20831,
     -29, 15905,   513,  8711, -1153, -1153, -1153, -1153, -1153, -1153,
   -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153,
   -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153,   574, -1153,
     497,  5221,   575,  1890,   114,    29, -1153,  5221, -1153, -1153,
   17054, -1153,    87, 17572,   484, -1153,   580,   581, -1153, 17054,
     -34,   484, -1153,  8711,  1626, -1153, -1153, -1153,   259, 20051,
   12187, 12187, -1153, 20051,   592, 17487, -1153, 17054,   -34, 20051,
     595,  8711, -1153, 20051, 17724,   617,   600,   -34,    32, 17054,
   17764,   184,   184,   677,   484,   484,   124, -1153, -1153,  4346,
     -55, -1153, 12187, -1153,   110,   123, -1153,    67,    19, 17810,
     -54,   677,   761, -1153,  4521, -1153,   708, 12187, 12187, 20831,
   -1153, -1153,   627,   610, -1153, -1153, -1153, -1153,   325,   505,
   -1153, 12187,   636, 12187, 12187, 12187, 10622, 10622, 12187,   518,
   12187, 12187, 12187, 12187, 12187,   442, 14737, 12187, 12187, 12187,
   12187, 12187, 12187, 12187, 12187, 12187, 12187, 12187, 12187, 12187,
   12187, 12187, 12187,   719, -1153, -1153, -1153, -1153, -1153,  9751,
    9751, -1153, -1153, -1153, -1153,  9751, -1153, -1153,  9751,  9751,
    8711,  8711, 10622, 10622, 17103, 17143, 17916,   619,    37, 20831,
    8190, -1153, -1153,  4696, -1153, 10622,    32,   624,   342, -1153,
   12187, -1153, -1153, 12187,   667, -1153,   621,   651, -1153, -1153,
   -1153, 20831, -1153,  3130, -1153, -1153, 20831,   633, 20831, -1153,
    3130,   753, 10970, -1153,  5396, 10622, -1153,   630, -1153,    32,
    5571, 10622, -1153,    32, -1153, 10622, -1153,  7319,  7319, -1153,
     678,   681,  5221,   774,  5221, -1153,   776, 12187, -1153, -1153,
     552,   639,  8711, 20831, -1153, -1153,   403, -1153, -1153,  1890,
   -1153,   668,   640, -1153, 12533,   701, 12187,  3130, -1153, -1153,
   12187, -1153, 20382, 12187, 12187, -1153,   654, -1153, 10970, -1153,
   20051, 20051, -1153,    49, -1153,  8711,   662, -1153, 12706,   692,
   -1153, -1153, -1153, -1153, -1153, -1153, -1153,  9926, -1153, 17962,
    7842, -1153,  8017, -1153,  5221,   666, 10622, 10101,  3996,   671,
   12187, 11143, -1153, -1153,   139, -1153,  4871, 20831, -1153,   346,
   18002,   364, 16857,   282,   821, 10970,   679, 20295,   258, -1153,
   18154,   988,   988,   551, -1153,   551, -1153,   551,  1906,   494,
    2010,   848,   -34,   184, -1153,   675, -1153, -1153, -1153, -1153,
   -1153,   677, 16603,   551,   596,   596,   988,   596,   596,  1299,
     184, 16603,  1687,  1299,   484,   484,   184,   677,   683,   685,
     686,   690,   693,   695,   684,   688, -1153,   551, -1153,   551,
   12879, 10622, 13052, 10622, 12187,  8711, 10622, 15504,   689,    55,
   -1153, -1153, -1153,   166, -1153,  1453, 20127,   465,    75, 18199,
   -1153, -1153, -1153, 20051, 18239,  8711, -1153,  8711, 20831,   627,
     368, 20831, 20831,   627, -1153,   627,   372, 12187,   168,  9230,
   20051,    47, 17295,  8190, -1153,  9230, 20051,    80, 16902, -1153,
      32, 17054, -1153, -1153, -1153, 12187,   527, 12187,   534,   577,
   -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153,
   12187, -1153, -1153, 10274, -1153, -1153,   591, -1153,   375, -1153,
   -1153, -1153, 18396,   718,   705, 12187, 12187,   831,  5221,   836,
   18436,  5221, 17335,   808, -1153,   182, -1153,   256, -1153,   180,
   -1153, -1153, -1153, -1153, -1153, -1153,   706,   729,   704, -1153,
   15609, -1153,   399,   709,  1890,   114,    -1,    34, 12187, 12187,
    7146, -1153, -1153,   658,  8884, -1153, 20051, -1153, -1153, -1153,
   20831, 18476, 18628, -1153, -1153, 20051,   707,    13,   710, -1153,
   -1153,   398, 20831, -1153, -1153, -1153, -1153, -1153, -1153, -1153,
    5221,    83, 17380, -1153, -1153, 20051,  5221, 20051, -1153, 18669,
   -1153, -1153, -1153, 12187, -1153,    39, 15663, 12187, -1153, 11316,
    7319,  7319, -1153,  8711,  2441, -1153,   733,  1323,   713, 20483,
     756,   -21, -1153, -1153,   801, -1153, -1153, -1153, -1153, 14569,
     722, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153, 12187,
     859, 18710, 12187,   863, 18862,   287,   724, 18902,  8711, -1153,
   -1153,  8190, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153,
   -1153, -1153, -1153, -1153,    45, 10622, 10622,    32,    32, -1153,
   -1153, -1153, -1153,   627,   727, -1153,   627,   627,   730,   737,
   -1153, 17054, -1153, 15981,  5746, -1153,  5921, -1153,   288, -1153,
   16057,  6096, -1153,    32,  6271, -1153,    32, -1153, -1153,  7319,
   -1153, 12187, -1153, 20051, 20051,  5221, -1153,  5221, 12187, -1153,
    5221,   868, 20831,   744, 20831,   545, -1153, -1153, 20831,   846,
   -1153,  1890,   765,   823, -1153, -1153, -1153,    43, -1153, -1153,
     701,   736,    89, -1153, -1153, -1153,   746,   749, -1153,  6446,
   10970, -1153, -1153, -1153, -1153, -1153,  6621,   748,  6796,   751,
   -1153, 12187, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153,
   -1153, -1153,  7319, -1153, 18947,    63, 17532,   400,   759,   294,
   20831, -1153,   783,   545,   758,  1706, -1153, 20831, -1153, 12187,
   20570, -1153, -1153,   399,   760,   203,   786,   787,   791,   804,
     797,   798,   800,   809,   802,   803,   810,   252,   812,   811,
     814, 12187, -1153,   816,   817,   815,   760, -1153,   760, -1153,
   -1153, 19054, 13225, 13398, 19130, 13571, 13744, -1153, 13917, 14090,
     296, -1153,   701,   310,   324, -1153, -1153,   796, -1153,   805,
     806, -1153, -1153, -1153,  5221,  9230, 20051,  9230, 20051,  8190,
   -1153,  5221,  9230, 20051, -1153,  9230, 20051, -1153, -1153, 19215,
   20051, -1153, -1153, 20051,   896,  5221,   807, -1153, -1153, -1153,
     765, -1153,   794, 11491,   157, -1153,    38, -1153, -1153, 10622,
   15363,  8711,  8711,  5221, -1153,   101,  9230, -1153, 20051,  5221,
    9230, -1153, 20051,  5221, 20051,   167, 11664,  7319,  7319,  7319,
    7319, -1153, -1153,   813,   795, 12187, -1153, -1153,  1343, -1153,
    1626, -1153, 18171, -1153, -1153, -1153, 20051, -1153,    62,   403,
   -1153, 19291, -1153, 15822, -1153, -1153, 12187, 12187, 12187, 12187,
   12187, 12187, 12187, 12187, -1153, -1153, -1153,  3302, -1153, -1153,
    3517, 17299, 18436, 16133, 16209, -1153, 18436, 16285, 16361, 12187,
    5221, -1153, -1153,   157,   765, 10449, -1153, -1153, -1153,   271,
   10970, -1153, -1153,   173, 12187,    15, 19367, -1153,   747,   818,
     824,   597, -1153, 16437, -1153, 16513, -1153, -1153, -1153, 20051,
     401,   820,   440,   829, -1153,   545, 20051, 16865, -1153, -1153,
   -1153, 14263,   866,   825, -1153,   834,   837,   760,   760, 19443,
   19519, 19595, 19671, 19747, 19823, 17735, -1153, 18866, 20070, -1153,
   -1153,  5221,  5221,  5221,  5221, 20051, -1153, -1153, -1153,   157,
   11839,   103, -1153, 20051, -1153,    98, -1153,    11, -1153,   431,
   19899, -1153, -1153, -1153, 14090,   840,   842,  5221,  5221, -1153,
   -1153, -1153, -1153, -1153, -1153,  6971, -1153, -1153,   170, -1153,
      38, -1153, -1153, -1153, 12187, 12187, 12187, 12187, 12187, 12187,
   -1153, -1153, -1153, 18436, 18436, 18436, 18436, -1153, -1153, -1153,
   -1153, -1153,   318, 10622, 14909, -1153, 12187,   173,    98,    98,
      98,    98,    98,    98,   173,   951, -1153, -1153, 18436, 18436,
     826, 14436,   105,    54, 19975, -1153, -1153, 20051, -1153, -1153,
   -1153, -1153, -1153, -1153, -1153,   833, -1153, -1153,   255, 15081,
   -1153, -1153, -1153, 12014, -1153,   330, -1153
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   119,   657,   658,   659,   653,
     654,   660,     0,   578,   105,   140,   547,   147,   549,   578,
       0,   146,     0,   452,   105,     0,     0,   322,     0,   270,
     141,   625,   625,   651,     0,     0,     0,     0,     0,   145,
      60,   271,   323,   142,     0,     0,     0,   319,     0,     0,
     149,     0,     0,   597,   569,   661,   150,     0,   324,   541,
     451,     0,     0,     0,   172,   322,   144,   550,   453,     0,
       0,     0,     0,   545,     0,     0,   148,     0,     0,   120,
       0,   652,   279,   280,     0,   143,   302,   543,   455,   151,
       0,     0,   710,     0,   712,     0,   713,   714,   624,     0,
     711,   708,   528,   169,   709,     0,     0,     0,     0,     4,
       0,     5,    12,     9,    43,     0,    46,    55,    10,    11,
       0,    13,    14,    15,    27,   524,   525,    21,    30,    47,
     170,   179,   180,    16,    29,    28,    18,     0,   265,    17,
     616,    19,     0,    20,    32,    31,   178,     0,   176,     0,
     613,     0,   174,   177,     0,   175,   630,   609,   526,   610,
     531,   529,     0,     0,     0,   614,   615,     0,   530,     0,
     631,   632,   633,   655,   656,   608,   533,   532,   611,   612,
       0,    26,   549,   141,     0,     0,     0,     0,   550,     0,
       0,     0,     0,   613,   630,   529,   614,   615,   539,   530,
     631,   632,     0,     0,   579,   106,     0,   548,     0,   578,
       0,     0,   454,     0,   278,     0,   509,   322,   300,   310,
     625,   172,   322,   301,    45,     0,   516,   653,   550,   626,
     322,   653,   201,   550,     0,   189,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
       0,     0,    57,   516,   113,   121,   133,   127,   126,   135,
     116,   125,   136,   122,   137,   114,   138,   131,   124,   132,
     130,   128,   129,   115,   117,   123,   134,   139,     0,   118,
       0,     0,     0,     0,     0,     0,   458,     0,   157,    38,
       0,   163,     0,   162,   695,   601,   598,   599,   600,     0,
     542,   696,     7,   516,   322,   171,   423,   506,     0,   505,
       0,     0,   158,   629,     0,     0,    41,     0,   546,   534,
       0,   516,    42,   540,     0,   448,     0,   544,     0,     0,
       0,   697,   699,   622,   694,   693,     0,    62,    65,     0,
       0,   511,     0,   513,     0,     0,   512,     0,     0,   505,
       0,   623,     0,     6,     0,    56,     0,     0,     0,     0,
     281,   283,   303,     0,   409,   410,   408,   325,     0,   527,
      33,     0,   602,     0,     0,     0,     0,     0,     0,   698,
       0,     0,     0,     0,     0,     0,   621,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   364,   371,   372,   373,   368,   370,     0,
       0,   366,   369,   367,   365,     0,   375,   374,     0,     0,
     516,   516,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,    22,     0,    35,     0,     0,     0,     0,    23,
       0,    36,    44,     0,   524,   522,     0,   517,   518,   523,
     195,     0,   198,     0,   187,   191,     0,     0,     0,   197,
       0,     0,     0,   211,     0,     0,   210,     0,   219,     0,
       0,     0,   217,     0,   224,     0,   223,     0,    79,   181,
       0,     0,     0,   239,     0,   364,   235,     0,    59,    58,
      55,     0,     0,     0,   249,    24,   391,   320,   462,     0,
     463,   465,     0,   487,     0,   468,     0,     0,   156,    37,
       0,   165,     0,     0,     0,    39,     0,   173,     0,    98,
     627,   628,   159,     0,    40,     0,     0,   290,     0,   446,
     443,   204,   203,    25,    64,    63,    66,     0,   662,     0,
       0,   647,     0,   649,     0,     0,     0,     0,     0,     0,
       0,     0,   666,     8,     0,    49,     0,     0,    96,     0,
      92,     0,    73,   276,   282,     0,     0,     0,   402,   457,
     577,   690,   689,   692,   701,   700,   705,   704,   686,   683,
     684,   685,   618,   673,   119,     0,   644,   645,   120,   643,
     642,   619,   677,   688,   682,   680,   691,   681,   679,   671,
     676,   678,   687,   670,   674,   675,   672,   620,     0,     0,
       0,     0,     0,     0,     0,     0,   703,   702,   707,   706,
       0,     0,     0,     0,     0,     0,     0,   664,   276,   589,
     590,   592,   594,     0,   581,     0,     0,     0,   578,   578,
     207,   444,   456,   510,     0,     0,   536,     0,     0,   303,
       0,     0,     0,   303,   445,   303,     0,     0,     0,     0,
     553,     0,     0,     0,   220,     0,   559,     0,     0,   218,
       0,     0,   359,   357,   362,   356,   338,   341,   339,   340,
     363,   351,   342,   355,   347,   345,   358,   361,   346,   344,
     349,   354,   343,   348,   352,   353,   350,   360,     0,   376,
     377,    68,    67,    80,     0,     0,     0,   238,     0,   234,
       0,     0,     0,     0,   535,     0,   252,     0,   250,   396,
     393,   394,   395,   399,   400,   401,   391,   384,     0,   381,
       0,   392,   411,     0,   466,     0,   154,   155,   153,   152,
       0,   486,   485,   609,     0,   460,   607,   459,   164,   161,
       0,     0,     0,   641,   508,   507,     0,     0,     0,   537,
     450,   609,     0,   663,   617,   648,   514,   650,   515,   231,
       0,     0,     0,   665,   229,   563,     0,   668,   667,     0,
      51,    50,    48,     0,    91,     0,     0,     0,    84,     0,
       0,    79,   273,     0,     0,   284,   304,     0,     0,     0,
     317,     0,   311,   314,   406,   403,   404,   407,   326,     0,
       0,   104,   102,   103,   101,   100,    99,   639,   640,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
     593,     0,   580,   140,   147,   146,   145,   142,   149,   150,
     144,   148,   143,   151,     0,     0,     0,     0,     0,   202,
     520,   521,   519,   303,     0,   200,   303,   303,     0,     0,
     199,     0,   233,     0,     0,   209,     0,   208,     0,   584,
       0,     0,   215,     0,     0,   213,     0,   222,   221,     0,
     182,     0,   183,   247,   246,     0,   241,     0,     0,   237,
       0,   243,     0,   275,     0,     0,   397,   398,     0,   391,
     380,     0,   500,   412,   415,   414,   416,     0,   464,   467,
     468,     0,     0,   469,   470,   160,     0,     0,   292,     0,
       0,   291,   294,   538,   447,   232,     0,     0,     0,     0,
     230,     0,    97,    94,   356,   338,   341,   339,   340,   349,
     348,   350,     0,    93,    76,    75,    74,     0,     0,     0,
       0,   285,   288,     0,     0,   322,   309,     0,   316,     0,
     312,   308,   405,   411,   378,   107,   121,   127,   126,   110,
     125,   122,   137,   108,   138,   124,   128,   109,   111,   123,
     139,     0,   335,     0,   112,     0,   378,   337,   378,   333,
     646,     0,     0,     0,     0,     0,     0,   275,     0,     0,
       0,   582,   468,   630,   630,   206,   205,     0,   305,     0,
       0,   305,   305,   212,     0,     0,   552,     0,   551,     0,
     583,     0,     0,   558,   216,     0,   557,   214,    71,    70,
      69,   240,   236,   568,   242,     0,     0,   272,   251,   248,
     500,   382,     0,     0,   468,   413,   427,   461,   491,     0,
     664,   516,   516,     0,   296,     0,     0,   227,   565,     0,
       0,   225,   564,     0,   669,     0,     0,     0,    79,     0,
      79,    85,    88,   277,     0,     0,   286,   299,   322,   172,
     322,   298,   322,   306,   166,   315,   318,   313,     0,   391,
     332,     0,   336,     0,   328,   329,     0,     0,     0,     0,
       0,     0,     0,     0,   277,   586,   305,   322,   305,   305,
     322,   322,   556,     0,     0,   585,   562,     0,     0,     0,
       0,   245,    61,   468,   500,     0,   503,   502,   504,   609,
     424,   387,   385,     0,     0,     0,     0,   489,   609,     0,
       0,   297,   295,     0,   228,     0,   226,    95,    78,    77,
       0,     0,     0,     0,   274,     0,   289,   322,   168,   307,
     482,     0,   417,     0,   334,   107,   109,   378,   378,     0,
       0,     0,     0,     0,     0,   322,   194,   322,   322,   186,
     190,     0,     0,     0,     0,    72,   244,   388,   386,   468,
     492,     0,   426,   425,   441,     0,   442,   429,   432,     0,
     428,   421,   422,   321,     0,   603,   604,     0,     0,    87,
      90,    86,    89,   287,   167,     0,   481,   480,   609,   418,
     427,   379,   330,   331,     0,     0,     0,     0,     0,     0,
     196,   188,   192,   555,   554,   561,   560,   390,   389,   494,
     495,   497,   609,     0,   664,   440,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   609,   605,   606,   567,   566,
       0,   472,     0,     0,     0,   496,   498,   431,   433,   434,
     437,   438,   439,   435,   436,   430,   477,   475,   609,   664,
     419,   327,   420,   492,   476,   609,   499
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1153, -1153, -1153,     5,     3,  2575,   869, -1153,     4, -1153,
   -1153, -1153,   492, -1153, -1153, -1153,   432,   663,  -465, -1153,
    -762, -1153, -1153, -1153,   235, -1153, -1153, -1153,   995, -1153,
    2863,   -98,  -799, -1153,  -972,  2636, -1082,   512, -1153, -1153,
     -58,  -885, -1153,   -59, -1153, -1153, -1153, -1153, -1153, -1153,
   -1153, -1153, -1153, -1153,    97, -1153,   933, -1153, -1153,   131,
    2368, -1153, -1153, -1153, -1153, -1153, -1153, -1153, -1153,    77,
   -1153, -1153, -1153, -1153, -1153, -1153,  -619,  -693, -1153, -1153,
   -1153,    66,  -758,  1047, -1153, -1153, -1153,   273, -1153, -1153,
   -1153, -1153,   -65,  -791,  -153,  -753,  -914, -1153, -1153,   -60,
     133,   304, -1153, -1153, -1153,    70, -1153, -1153,  -228,    53,
   -1041,  -186,  -218,  -206,  -363, -1153,  -182, -1153,    10,  1005,
    -127,   538, -1153,  -482,  -862, -1152, -1153,  -697,  -519, -1139,
   -1134,  -986,    -5, -1153,   165, -1153,  -252,  -476,  -504,   828,
    -493, -1153, -1153, -1153,  1247, -1153,   -17, -1153, -1153,  -229,
   -1153,  -620, -1153, -1153, -1153,  1318,  1665,   -12,  1015,   160,
    1878, -1153,  2128,  2338, -1153, -1153, -1153, -1153, -1153, -1153,
   -1153, -1153, -1153,  -427
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   338,  1082,   709,   112,   113,   114,   115,
     116,   117,   356,   490,   118,   252,   119,   339,   703,   561,
     704,   120,   121,   122,   558,   559,   123,   124,   206,   982,
     284,   125,   279,   126,   741,   289,   127,   291,   292,   128,
    1083,   129,   304,   130,   131,   132,   456,   651,   458,   652,
     451,   648,   133,   134,   826,   135,   250,   136,   717,   718,
     192,   138,   139,   140,   141,   142,   143,   564,   795,   953,
     144,   145,   757,   922,   146,   147,   566,   955,   148,   149,
     801,   802,   803,   193,   282,   732,   151,   152,   568,   963,
     808,   985,   986,   699,   700,   701,  1090,   497,   727,   728,
     729,   730,   731,   809,   367,   907,  1220,  1281,  1203,   445,
    1131,  1135,  1197,  1198,  1199,   153,   326,   529,   154,   155,
     285,   286,   501,   502,   745,  1217,  1162,   505,   742,  1240,
    1128,  1044,   340,   215,   344,   345,   446,   447,   448,   194,
     157,   158,   159,   160,   195,   162,   203,   204,   633,   469,
     868,   634,   635,   163,   164,   196,   197,   167,   236,   449,
     199,   169,   200,   201,   172,   173,   174,   175,   350,   176,
     177,   178,   179,   180
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     198,   491,   208,   637,   207,   110,   305,   109,   209,   760,
     984,   743,   698,   473,   216,   419,   715,   733,   987,   229,
     229,   758,   240,   242,   244,   247,   251,   368,   437,   948,
     854,   224,   211,   943,   858,   761,   859,   290,   910,   293,
     294,   958,   584,   869,   503,   299,   503,   300,  1047,   301,
     756,   516,   457,  1202,  1123,  -270,  1241,   309,   313,   315,
     317,   318,   319,  1160,   308,   287,   323,   546,   324,   526,
    1084,  1126,  1094,   482,  1095,   327,     3,   506,   329,   330,
     864,   331,  1187,   332,   438,   626,   333,   334,   919,   335,
    -271,   537,   551,   309,   313,   349,   351,   357,   487,  1133,
    1067,  1194,   348,   829,  -483,  1265,   379,    70,   110,  1277,
    1266,   287,   181,   871,   385,   503,   538,  -253,   545,   549,
     287,   552,   307,  -478,  -293,   960,  1028,  1284,   444,  1246,
     920,   239,   241,   243,   287,  -483,  1068,  1049,  1189,   467,
    1105,   433,   588,   852,  1286,   830,    70,  1002,  1237,   825,
     961,  1243,   372,   626,  -478,   444,  1134,  1247,   307,   343,
     307,  -483,  -483,   305,   358,   547,  -293,  -478,   614,   615,
    -483,   226,   424,   425,   426,   507,  1194,   323,   309,   349,
    -478,  1202,  1132,   547,   921,   433,   428,   467,   504,  1216,
     504,  -483,   436,  1158,   548,   202,   -54,  1084,   467,  1085,
     467,  1196,   433,   467,   347,   444,   253,  1161,   229,   205,
    1046,  1001,   627,  -483,   313,   -54,   375,   433,  -483,  1280,
    -293,   442,  1084,   444,   433,  1084,  1084,  -478,   896,   226,
     314,  -479,  -478,   510,  1007,   547,   637,  1009,  1010,  -483,
     453,   313,   544,   307,   307,   517,   460,   518,   433,   547,
     433,   547,   909,  1222,  1223,   211,   540,   433,   926,   504,
    -483,  1188,  -479,  -254,  1050,  1195,   346,   -83,    23,   542,
     897,   433,  1214,   376,  -260,  -479,  1196,   377,  1244,  -483,
    1279,   364,   744,   541,   440,   804,   -54,   590,  -479,   181,
    1084,   313,  1084,  1084,   984,   534,   543,   454,   520,   521,
    -259,   365,   987,   441,   181,   -54,  1151,  1130,  1153,   313,
     780,   366,   831,   879,   518,  1107,  -471,   949,  1110,  1111,
      60,   455,   444,   444,   379,   947,   650,  1238,   647,   383,
     539,    68,   385,   656,   -83,  -479,   388,   805,  1147,   832,
    -479,   862,   110,  -113,   536,   560,   562,  -471,   806,    23,
     210,  -119,  1000,   212,   295,   893,    88,   110,  -268,   570,
    -471,   571,   572,   573,   575,   577,   578,   807,   579,   580,
     581,   582,   583,  -471,   591,   592,   593,   594,   595,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,  -473,  -115,   420,   444,   421,  -501,   313,   313,  1115,
    -120,    60,   894,   313,   637,   518,   313,   313,   313,   313,
     617,   619,    68,  1175,   296,  1177,  1178,  -501,   636,  1042,
    -471,  -501,  -473,   639,   519,  -471,   903,   444,   643,   895,
     719,   644,   297,   647,  1019,  -473,   110,    88,   873,   876,
     647,  -258,   647,  -493,  -501,   584,   253,   792,  -473,   298,
     309,   720,   660,   662,   793,   721,  -588,   658,   666,   668,
     997,  1020,   585,   671,  -493,   702,   702,  1073,  -493,  1104,
    -587,   507,   710,   574,   576,   712,   586,  1065,   904,   722,
     313,   288,   723,  -588,    62,  -638,  -255,  -638,   507,   905,
     569,  -493,   783,   724,   746,  -473,   302,  -587,   293,   726,
    -473,   751,   752,   452,   288,   587,   755,   642,   906,   459,
     787,   784,   725,   313,   507,   307,   375,  -262,   507,   616,
     618,   879,   750,  -266,  -263,   755,   375,   444,   313,   788,
     313,  1137,   638,   855,   772,   755,   775,   860,   777,   779,
     880,   303,   927,   929,  -449,   588,   879,   879,   422,   444,
    1129,   589,   423,   309,   846,  -257,  1248,  1138,   423,   110,
     796,   536,   661,  -449,   320,  1071,  1209,    23,   667,   609,
     610,   754,   670,   376,   361,   611,  -383,   377,   612,   613,
     924,  -261,  1249,   376,  1250,  1251,   879,   377,  1252,  1253,
     754,    42,   637,   343,  -574,   343,   488,   321,  -267,   489,
     754,  -637,  1150,  -637,  1152,  1211,  -269,   354,   660,   821,
     666,   824,   710,   313,   827,   775,  -712,    58,   307,    60,
    -712,   847,   848,  -713,   379,  -256,  -264,  -713,   375,   383,
      68,   384,   385,   313,   379,   313,   388,   355,   359,   383,
     434,   384,   385,   771,   395,   861,   388,   863,  -637,   429,
    -637,   636,   401,   870,   395,    88,   988,  -634,  -571,  -634,
     399,   400,   401,   331,   369,   332,  -714,   433,  1218,   493,
    -714,   288,   288,   288,   288,   288,   288,  -636,   334,  -636,
    -709,   335,   483,   486,  -709,   376,  -570,   933,   590,   377,
    -635,   379,  -635,   883,   884,   444,   383,  1242,   850,   385,
     346,   370,   346,   388,   371,   766,  -484,   768,  -575,   375,
     487,  1255,   494,   430,  -576,   433,   443,   487,   820,  -484,
     823,  -573,   288,  -572,   288,   432,   319,   323,   349,   439,
     444,   288,   313,   719,   450,   912,   379,   380,   462,   381,
     382,   383,   528,   384,   385,   386,   492,   496,   388,   288,
    -484,  1242,   513,   514,   720,   394,   395,   522,   721,   398,
     288,   288,   399,   400,   401,   530,   376,   525,  1278,   553,
     377,   560,   565,   402,   557,   944,  -484,   946,   702,   702,
     567,   313,   722,  -484,   385,   723,  1285,   352,   608,   641,
    1242,   625,   645,   307,   646,  -488,   724,   647,   654,  1139,
    1140,   657,   663,   705,  -484,   851,   706,   991,  -488,   708,
     994,   711,   714,   735,   734,   725,   313,   379,   380,   636,
     381,   382,   383,  -484,   384,   385,   744,   753,  -484,   388,
     156,  -484,  1245,   575,   617,   759,   394,   395,   762,  -488,
     398,   770,   794,   399,   400,   401,   776,   810,   811,   797,
     812,   813,  1016,   156,  1018,   814,   156,   817,   815,  1023,
     816,   828,  1026,   818,   879,  -488,   885,  1029,   288,  1030,
     882,   887,  -488,   719,   892,   899,  1033,   900,   918,   518,
     375,   959,   908,   923,   956,  1269,  1270,  1271,  1272,  1273,
    1274,   962,   992,  -488,   720,   990,   995,  1008,   721,   998,
    1011,   288,   726,  1035,   914,   288,   156,  1012,   309,  1037,
    1043,  1048,  -488,  1045,  1058,  1055,  1062,  -488,  1051,  1064,
    -488,  1052,   722,  1059,  1072,   723,  1063,    62,  1075,  1077,
     702,  1120,  1089,   156,  -140,  -147,   724,   376,   156,  -146,
    1168,   377,   574,   616,  -116,  -145,  -142,  1086,  -149,  -114,
    -150,  -144,  -117,   444,   444,   725,  1092,  -118,  -148,  -143,
     487,   487,  -151,  1093,   487,   487,  1106,  1124,  1155,  1091,
     156,  1219,  1122,   307,  -113,  1108,  1109,  -115,  1154,  1247,
    1016,  1018,   713,  1023,  1026,  1210,  1058,  1062,   379,   380,
     487,  1205,   487,   383,  1212,   384,   385,  1206,  1221,  -490,
     388,  1276,  1112,  1113,  1256,  1114,  1257,   636,   395,  1116,
    1117,   360,  -490,  1118,   399,   400,   401,   556,   932,   213,
     375,   305,   748,   328,  1159,  1038,  1087,  1074,  1167,  1163,
     898,   156,  1041,  1088,  1263,  1282,  1275,  1136,   775,   313,
     313,  1268,   246,  -490,  1143,   747,   230,     0,  1145,   150,
       0,     0,     0,     0,  1149,   702,   702,   702,   702,     0,
       0,     0,     0,  1156,     0,     0,     0,     0,     0,  -490,
       0,     0,   150,     0,     0,   150,  -490,   376,     0,     0,
       0,   377,     0,     0,  1112,  1169,  1170,  1116,  1171,  1172,
    1173,  1174,   726,   288,   288,     0,  1127,  -490,     0,   288,
     288,     0,   288,   288,     0,     0,     0,  1185,     0,   156,
       0,     0,   378,   349,     0,   156,  -490,     0,  1193,     0,
    1191,  -490,  1200,     0,  -490,   150,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,     0,   891,   392,   393,   394,   395,   396,
     397,   398,   150,     0,   399,   400,   401,   150,     0,     0,
       0,     0,     0,     0,     0,   402,     0,   156,     0,  1233,
    1234,  1235,  1236,     0,     0,     0,     0,     0,   307,     0,
       0,     0,   156,  1192,     0,     0,     0,     0,     0,   150,
       0,     0,  1062,     0,     0,  1258,  1259,     0,     0,     0,
       0,     0,     0,   349,     0,     0,     0,     0,     0,     0,
    1262,     0,  1233,  1234,  1235,  1236,  1258,  1259,     0,     0,
       0,     0,     0,     0,     0,     0,   951,     0,     0,     0,
       0,  1264,   775,     0,  1267,     0,     0,     0,     0,     0,
       0,     0,     0,  1239,     0,     0,     0,     0,     0,   161,
     150,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   156,     0,     0,     0,     0,     0,   775,   307,     0,
       0,  1062,   161,     0,     0,   161,     0,     0,     0,   288,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   156,   288,     0,   486,     0,  1239,   156,     0,
       0,     0,   486,     0,     0,   288,     0,     0,   288,     0,
     156,     0,   156,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,     0,   954,   161,     0,  1039,   150,     0,
       0,   375,     0,     0,   150,     0,  1239,     0,     0,     0,
       0,     0,     0,   165,     0,  -305,   165,     0,     0,  -305,
    -305,  1157,   161,     0,  -305,     0,     0,   161,     0,  -305,
       0,  -305,  -305,     0,     0,   218,     0,  -305,     0,    22,
      23,     0,   156,     0,  -305,     0,   156,  -305,     0,   219,
       0,    31,   220,     0,   156,  1076,   150,    37,   376,   161,
       0,     0,   377,     0,    42,     0,   165,  -305,     0,     0,
    -305,   150,  -305,     0,  -305,     0,  -305,  -305,     0,  -305,
       0,  -305,     0,  -305,     0,     0,     0,     0,     0,     0,
      58,     0,    60,   165,     0,     0,  1079,     0,   165,  1080,
       0,   223,  -305,    68,     0,  -305,     0,     0,  -305,   379,
     380,     0,   381,   382,   383,     0,   384,   385,     0,     0,
     161,   388,    84,     0,     0,    86,   584,     0,    88,   395,
     165,     0,     0,   844,     0,   399,   400,   401,     0,     0,
     833,     0,     0,   834,     0,     0,     0,     0,   835,     0,
     150,     0,     0,     0,   373,     0,     0,   288,  -305,   183,
       0,   374,     0,     0,  -305,     0,     0,     0,     0,   836,
       0,     0,     0,     0,   375,   470,   837,     0,   103,     0,
       0,   150,     0,     0,     0,     0,   838,   150,     0,     0,
     471,   165,     0,     0,   839,     0,     0,     0,   161,   150,
       0,   150,     0,     0,   161,     0,   156,     0,     0,   156,
     840,     0,     0,     0,     0,   486,   486,     0,     0,   486,
     486,     0,   841,     0,     0,     0,   588,     0,     0,     0,
       0,   376,     0,   842,     0,   377,     0,  1201,     0,   843,
       0,     0,     0,     0,     0,   486,     0,   486,     0,     0,
       0,     0,     0,     0,     0,     0,   161,  1213,     0,     0,
       0,   150,   467,     0,     0,   150,   378,     0,   156,   165,
       0,   161,     0,   150,   156,   165,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,   989,   399,   400,
     401,     0,   433,     0,     0,     0,     0,     0,   218,   402,
       0,     0,    22,    23,     0,     0,     0,   165,     0,     0,
       0,     0,   219,     0,    31,   220,     0,   166,     0,     0,
      37,     0,   165,  1003,  1004,     0,     0,    42,     0,     0,
     161,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,     0,   156,   166,   156,  1201,     0,     0,     0,   156,
       0,     0,   156,    58,     0,    60,     0,     0,     0,     0,
       0,   161,     0,   156,   223,   156,    68,   161,   156,   375,
       0,     0,     0,     0,     0,     0,     0,     0,   218,   161,
       0,   161,    22,    23,     0,    84,     0,  1078,    86,     0,
       0,    88,   219,   166,    31,   220,     0,   156,     0,     0,
      37,   165,     0,     0,   156,   150,   156,    42,   150,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,     0,     0,     0,     0,   166,   376,     0,     0,     0,
     377,     0,   165,    58,     0,    60,     0,    62,   165,  1079,
       0,   161,  1080,     0,   223,   161,    68,     0,     0,     0,
     165,     0,   165,   161,     0,     0,     0,   166,     0,     0,
       0,   378,     0,     0,     0,    84,     0,   150,    86,     0,
       0,    88,     0,   150,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   156,     0,   392,   393,   394,   395,   396,   156,
     398,     0,     0,   399,   400,   401,     0,     0,     0,     0,
       0,     0,   165,   156,   402,     0,   165,     0,   166,     0,
       0,   103,     0,     0,   165,     0,     0,  1081,     0,     0,
     168,   156,     0,     0,     0,     0,     0,   156,     0,     0,
       0,   156,     0,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   168,     0,     0,   168,   255,   256,     0,
     257,   150,     0,   150,     0,   258,     0,     0,   150,     0,
       0,   150,     0,     0,   259,     0,     0,     0,     0,     0,
     260,     0,   150,     0,   150,     0,   261,   150,   375,     0,
     262,     0,     0,   263,     0,     0,   166,     0,   156,     0,
       0,     0,   166,   264,     0,   161,   168,     0,   161,     0,
     265,   266,     0,     0,     0,     0,   150,     0,   267,     0,
       0,     0,     0,   150,     0,   150,     0,   268,     0,     0,
       0,     0,     0,   168,     0,     0,   269,   270,   168,   271,
       0,   272,     0,   273,     0,   376,   274,     0,     0,   377,
     275,   498,     0,   276,   166,     0,   277,     0,     0,   156,
     156,   156,   156,     0,     0,     0,     0,   161,     0,   166,
     168,     0,     0,   161,     0,     0,   165,     0,     0,   165,
       0,     0,     0,     0,     0,   156,   156,     0,     0,     0,
       0,     0,   375,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   150,   499,   392,   393,   394,   395,   396,   150,   398,
       0,     0,   399,   400,   401,     0,     0,     0,     0,     0,
       0,   168,   150,   402,     0,     0,     0,     0,   165,     0,
       0,     0,     0,     0,   165,     0,     0,     0,   166,   376,
     150,     0,     0,   377,     0,     0,   150,     0,     0,     0,
     150,   161,     0,   161,     0,     0,     0,     0,   161,     0,
       0,   161,     0,     0,     0,     0,     0,     0,     0,   166,
     170,     0,   161,     0,   161,   166,     0,   161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   166,     0,   166,
     379,   380,     0,   170,   382,   383,   170,   384,   385,   168,
       0,     0,   388,     0,     0,   168,   161,   150,     0,     0,
     395,     0,     0,   161,     0,   161,   399,   400,   401,     0,
       0,     0,   165,     0,   165,     0,     0,     0,     0,   165,
       0,     0,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   165,     0,   165,   170,     0,   165,   166,
       0,     0,     0,   166,     0,     0,     0,   168,     0,     0,
       0,   166,     0,     0,     0,     0,     0,     0,   150,   150,
     150,   150,   168,   170,     0,     0,     0,   165,   170,     0,
       0,     0,     0,     0,   165,   217,   165,     0,     0,     0,
       0,     0,     0,     0,   150,   150,     0,     0,     0,   218,
       0,   161,     0,    22,    23,     0,     0,     0,   161,     0,
     170,     0,     0,   219,     0,    31,   220,     0,     0,     0,
       0,    37,   161,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     161,     0,     0,   -52,     0,     0,   161,     0,     0,     0,
     161,   168,     0,     0,    58,     0,    60,     0,     0,     0,
     221,     0,   -52,   222,     0,   223,     0,    68,     0,     0,
       0,   170,   165,     0,     0,     0,     0,     0,     0,   165,
     171,     0,   168,     0,     0,     0,    84,     0,   168,    86,
       0,     0,    88,   165,     0,     0,     0,     0,     0,     0,
     168,     0,   168,   171,     0,     0,   171,   161,     0,     0,
     137,   165,     0,   166,     0,     0,   166,   165,     0,     0,
       0,   165,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   137,     0,     0,   137,     0,     0,     0,
       0,     0,   103,     0,     0,     0,     0,     0,     0,   170,
       0,     0,     0,     0,     0,   170,   171,     0,     0,     0,
       0,     0,   168,     0,     0,     0,   168,     0,   161,   161,
     161,   161,     0,     0,   168,   166,     0,     0,   165,     0,
       0,   166,     0,   171,   254,     0,   137,     0,   171,     0,
       0,     0,     0,     0,   161,   161,     0,     0,   255,   256,
       0,   257,     0,     0,     0,     0,   258,   170,     0,     0,
       0,     0,     0,   137,     0,   259,     0,     0,   137,     0,
     171,   260,   170,     0,     0,     0,     0,   261,     0,     0,
       0,   262,     0,     0,   263,     0,     0,     0,     0,   165,
     165,   165,   165,     0,   264,     0,     0,     0,     0,     0,
       0,   265,   266,     0,     0,     0,     0,     0,     0,   267,
       0,     0,     0,     0,     0,   165,   165,     0,   268,   166,
       0,   166,     0,     0,     0,     0,   166,   269,   270,   166,
     271,   171,   272,     0,   273,     0,     0,   274,     0,     0,
     166,   275,   166,     0,   276,   166,     0,   277,     0,     0,
       0,   170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   137,     0,     0,     0,     0,     0,   111,     0,     0,
       0,     0,     0,     0,   166,     0,   168,     0,     0,   168,
       0,   166,   170,   166,     0,     0,     0,     0,   170,     0,
     214,   218,     0,   225,     0,    22,    23,     0,     0,     0,
     170,   433,   170,   950,     0,   219,     0,    31,   220,   171,
       0,     0,     0,    37,     0,   171,   -82,     0,     0,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -53,   218,     0,   168,   137,
      22,    23,     0,   322,   168,   137,    58,     0,    60,     0,
     219,     0,    31,   220,   -53,     0,     0,   223,    37,    68,
       0,     0,   170,     0,     0,    42,   170,   171,     0,   166,
     111,     0,     0,     0,   170,   353,   166,     0,    84,     0,
     -53,    86,   171,   -82,    88,     0,     0,     0,     0,     0,
     166,    58,     0,    60,     0,     0,     0,   137,   316,   -53,
       0,     0,   223,     0,    68,     0,     0,     0,   166,     0,
       0,     0,   137,     0,   166,     0,     0,     0,   166,     0,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,   168,     0,   168,     0,     0,     0,     0,   168,
     218,     0,   168,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,   168,   219,   168,    31,   220,   168,     0,
       0,   171,    37,     0,     0,     0,     0,     0,   431,    42,
       0,     0,     0,     0,     0,   166,     0,     0,     0,     0,
       0,     0,     0,     0,   -52,     0,     0,   168,     0,     0,
       0,   137,   171,     0,   168,    58,   168,    60,   171,     0,
       0,   221,     0,   -52,   222,     0,   223,     0,    68,     0,
     171,     0,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,     0,     0,     0,   170,    84,   137,   170,
      86,     0,     0,    88,     0,     0,   166,   166,   166,   166,
     137,     0,   137,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   508,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,   166,     0,   463,   466,   468,   472,   474,
     476,     0,   171,     0,     0,     0,   171,     0,     0,     0,
       0,     0,   168,   103,   171,     0,     0,     0,   170,   168,
       0,     0,     0,     0,   170,     0,     0,   278,   280,   281,
       0,     0,   137,   168,   111,     0,   137,     0,     0,     0,
       0,     0,     0,     0,   137,     0,   509,     0,   511,   111,
       0,   168,     0,     0,     0,   515,     0,   168,     0,     0,
       0,   168,     0,     0,     0,     0,     0,   325,     0,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   531,   532,     0,     0,     0,     0,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,     0,     0,     0,     0,     0,
       0,     0,   170,     0,   170,     0,     0,     0,   168,   170,
       0,  -595,   170,     0,     0,     0,     0,     0,   111,     0,
     362,     0,   363,   170,   418,   170,     0,  -634,   170,  -634,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   508,
       0,     0,     0,     0,     0,   508,   171,   170,     0,   171,
     427,     0,     0,     0,   170,     0,   170,   707,     0,   168,
     168,   168,   168,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   640,   325,     0,     0,   137,     0,     0,   137,
       0,     0,     0,     0,     0,   168,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   325,     0,     0,
     461,     0,     0,     0,     0,   664,     0,     0,   171,   669,
       0,   480,   481,     0,   171,     0,     0,     0,     0,   769,
       0,     0,     0,   774,     0,     0,     0,     0,     0,     0,
       0,   111,     0,   254,     0,     0,     0,     0,   137,     0,
       0,     0,   170,     0,   137,     0,   500,   255,   256,   170,
     257,     0,     0,     0,     0,   258,     0,     0,     0,     0,
       0,     0,     0,   170,   259,     0,     0,     0,     0,     0,
     260,     0,     0,     0,     0,     0,   261,     0,     0,     0,
     262,   170,     0,   263,     0,     0,     0,   170,     0,     0,
       0,   170,     0,   264,     0,     0,     0,     0,     0,     0,
     265,   266,   171,     0,   171,     0,     0,     0,   267,   171,
       0,     0,   171,     0,     0,     0,     0,   268,     0,     0,
       0,     0,   563,   171,     0,   171,   269,   270,   171,   271,
       0,   272,   137,   273,   137,     0,   274,     0,     0,   137,
     275,     0,   137,   276,     0,     0,   277,     0,   170,     0,
       0,     0,     0,   137,     0,   137,     0,   171,   137,     0,
       0,     0,     0,     0,   171,     0,   171,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   886,     0,     0,   889,   137,     0,     0,
       0,     0,   628,     0,   137,     0,   137,   865,   867,     0,
       0,     0,   283,   872,   875,     0,   877,   878,     0,   170,
     170,   170,   170,     0,   649,     0,     0,     0,     0,   653,
       0,   655,     0,     0,   218,     0,     0,     0,    22,    23,
       0,     0,     0,  1078,     0,   170,   170,     0,   219,     0,
      31,   220,     0,     0,     0,   925,    37,     0,     0,     0,
       0,   930,   171,    42,     0,     0,   716,     0,     0,   171,
       0,     0,   500,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,   749,     0,     0,     0,    58,
       0,    60,   137,    62,     0,  1079,     0,     0,  1080,   137,
     223,   171,    68,     0,     0,     0,     0,   171,     0,     0,
       0,   171,     0,   137,     0,     0,     0,     0,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
     782,   137,     0,     0,     0,     0,     0,   137,     0,     0,
     800,   137,     0,     0,     0,     0,     0,     0,     0,   508,
       0,   508,     0,     0,     0,     0,   508,     0,     0,   508,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
    1031,     0,  1032,     0,     0,  1034,     0,   103,     0,     0,
       0,     0,     0,  1176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1005,  1006,     0,     0,     0,   137,     0,
       0,     0,     0,     0,  1054,     0,     0,  1013,     0,     0,
       0,  1057,     0,  1061,     0,     0,     0,     0,     0,  1024,
       0,   853,  1027,     0,   856,   857,     0,     0,     0,   171,
     171,   171,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   218,
       0,     0,     0,    22,    23,   171,   171,     0,  1078,   137,
     137,   137,   137,   219,     0,    31,   220,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,   137,   137,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   902,    58,     0,    60,   500,    62,     0,
    1079,     0,     0,  1080,     0,   223,     0,    68,     0,     0,
    1121,     0,     0,   915,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,    84,     0,  1141,    86,
       0,     0,    88,     0,  1144,     0,     0,     5,  1146,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   182,   952,     0,    21,
     245,    23,   800,     0,     0,     0,     0,     0,    29,     0,
     183,     0,   983,     0,    33,   184,   185,     0,     0,   186,
      39,     0,   103,     0,    41,     0,     0,    43,  1179,     0,
     187,  1142,     0,    47,    48,  1186,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,    60,    61,     0,    63,     0,     0,     0,
       0,    66,   188,     0,    68,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   189,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,    88,
      89,     0,     0,     0,     0,  1036,     0,   716,    92,     0,
       0,  1040,     0,     0,   500,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,   191,     0,
       0,   108,     0,   952,     0,     0,     0,     0,     0,     0,
     800,    -2,     4,   800,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   983,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,  -664,     0,
      12,    13,    14,    15,    16,  -664,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,  -664,    28,
      29,  -664,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -322,   -52,    65,    66,    67,  -664,    68,    69,    70,  -664,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
    -664,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -664,  -664,    95,  -664,
    -664,  -664,  -664,  -664,  -664,  -664,     0,  -664,  -664,  -664,
    -664,  -664,     0,  -664,  -664,  -664,  -664,  -664,  -664,  -664,
    -664,   103,  -664,  -664,  -664,     0,   105,  -664,   106,     0,
     107,     0,   336,  -664,     5,   306,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,   337,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,    62,    63,    64,
    -322,   -52,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,   535,   106,     0,
     107,     0,   554,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,   555,   106,     0,   107,     0,   336,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,    62,    63,    64,
    -322,   -52,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,   337,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,   781,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,   352,    63,    64,
    -322,   -52,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,   659,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -322,   -52,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,   665,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1015,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -322,   -52,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1017,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1022,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -322,   -52,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1025,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,  1053,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -322,   -52,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1056,    39,   -81,     0,
      40,    41,    42,     0,    43,  -322,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -52,    53,    54,
       0,    55,    56,    57,     0,  -322,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -322,   -52,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -81,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1060,    39,   -81,     0,    40,    41,    42,     0,    43,
    -322,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -52,    53,    54,     0,    55,    56,    57,     0,
    -322,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -322,   -52,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -81,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,  1260,   108,     5,   306,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,   186,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   187,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   188,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   189,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   190,     0,   106,     0,   191,  1261,   911,   108,     5,
     306,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   188,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   189,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   190,     0,   106,     0,
     191,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,   672,     0,    12,     0,     0,    15,    16,   673,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,   186,    39,     0,     0,     0,    41,
       0,     0,    43,     0,   674,   187,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   188,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     189,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,   485,   404,   405,   406,   407,   408,
       0,     0,   411,   412,   413,   414,     0,   416,   417,   675,
     676,   677,   678,   679,   680,     0,   681,     0,    98,     0,
     682,   683,   684,   685,   686,   687,   688,   689,   690,   691,
     692,   100,   693,   102,     0,   694,   695,   696,   697,   190,
       0,   106,     0,   191,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,     0,    15,
      16,     0,    17,     0,   182,    19,    20,    21,     0,     0,
       0,     0,    26,     0,     0,    28,    29,     0,   183,     0,
       0,     0,    33,    34,    35,    36,     0,    38,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   187,     0,
       0,    47,    48,     0,    49,    50,    51,    52,     0,    53,
      54,     0,    55,    56,    57,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     188,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,    90,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   105,     0,   106,     0,   107,     0,     0,   108,
       5,   306,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   183,     0,     0,     0,    33,   184,   185,
       0,     0,   186,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   187,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   188,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   189,     0,
      79,     0,     0,    81,   310,   311,     0,    85,   341,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,   342,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   190,     0,   106,
       0,   191,     0,     0,   108,     5,   306,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   183,     0,
       0,     0,    33,   184,   185,     0,     0,   186,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   187,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     188,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   189,     0,    79,     0,     0,    81,   310,
     311,     0,    85,   341,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   190,     0,   106,   765,   191,     0,     0,   108,
       5,   306,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   183,     0,     0,     0,    33,   184,   185,
       0,     0,   186,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   187,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   188,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   189,     0,
      79,     0,     0,    81,   310,   311,     0,    85,   341,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   190,     0,   106,
     767,   191,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,   629,     0,     0,
       0,     0,     0,     0,    29,     0,   183,     0,     0,     0,
      33,   184,   185,     0,     0,   186,    39,     0,   630,     0,
      41,     0,     0,    43,     0,     0,   187,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   188,     0,
     631,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   189,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,   632,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     190,     0,   106,     0,   191,     0,     5,   108,     6,     7,
       8,   231,    10,    11,   232,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   183,
       0,     0,     0,    33,   184,   185,     0,     0,   186,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   187,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   233,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   189,     0,    79,     0,     0,    81,
       0,     0,   234,    85,     0,   235,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   190,     0,   106,     0,   191,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   188,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   189,     0,    79,
       0,     0,    81,   310,   311,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   312,     0,     0,   104,     0,   190,     0,   106,     0,
     191,     0,     0,   108,     5,   306,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,   186,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   187,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   188,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   189,     0,    79,     0,     0,    81,   310,   311,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   190,     0,   106,     0,   191,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     183,     0,     0,     0,    33,   184,   185,     0,     0,   186,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     187,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,   913,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   188,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   189,     0,    79,     0,     0,
      81,   310,   311,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,   191,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   183,     0,     0,     0,    33,   184,   185,
       0,     0,   186,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   187,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,   237,    61,     0,    63,
       0,     0,     0,     0,    66,   188,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   189,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,   238,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   190,     0,   106,
       0,   191,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,   248,     0,     0,
       0,     0,     0,     0,    29,     0,   183,     0,     0,     0,
      33,   184,   185,     0,     0,   186,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   187,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   188,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   189,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,   249,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     190,     0,   106,     0,   191,     0,     0,   108,     5,   306,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   183,     0,     0,     0,    33,   184,   185,     0,     0,
     186,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   187,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   188,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   189,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,   238,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   190,     0,   106,     0,   191,
       0,     5,   108,     6,     7,     8,   231,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   183,     0,     0,     0,    33,   184,
     185,     0,     0,   186,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   187,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   233,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   189,
       0,    79,     0,     0,    81,     0,     0,   234,    85,     0,
     235,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   190,     0,
     106,     0,   191,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,   186,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   187,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   188,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   189,     0,    79,     0,     0,    81,   310,   311,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   190,     0,   106,     0,   191,     0,     0,   108,     5,
     306,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   188,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   189,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   190,   763,   106,     0,
     191,     0,     0,   108,     5,   306,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,   186,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   187,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   188,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   189,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   190,     0,   106,     0,   191,   773,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     183,     0,     0,     0,    33,   184,   185,     0,     0,   186,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     187,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   188,  -708,     0,     0,     0,  -708,     0,     0,
      73,    74,    75,    76,    77,   189,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,   191,     0,
       0,   108,     5,   306,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,   186,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   187,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   188,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     189,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   190,
       0,   106,     0,   191,  1190,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   183,     0,
       0,     0,    33,   184,   185,     0,     0,   186,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   187,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     188,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   189,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,   238,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   190,     0,   106,     0,   191,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,   287,    29,
       0,   183,     0,     0,     0,    33,   184,   185,     0,     0,
     186,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   187,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   188,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   189,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   105,     0,   106,     0,   191,
       0,     0,   108,     5,   306,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   183,     0,     0,     0,
      33,   184,   185,     0,     0,   186,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   187,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   188,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   189,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     190,     0,   106,     0,   191,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   183,
       0,     0,     0,    33,   184,   185,     0,     0,   186,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   187,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   188,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   189,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   190,     0,   106,     0,   191,   778,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   188,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   189,     0,    79,
       0,     0,    81,     0,     0,     0,    85,   945,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   190,     0,   106,     0,
     191,     0,     0,   108,     5,   306,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   736,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,   186,    39,     0,     0,
       0,   737,     0,     0,    43,     0,     0,   187,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   188,
       0,     0,     0,     0,     0,     0,     0,    73,   738,    75,
      76,    77,   739,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   190,     0,   106,     0,  1125,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     183,     0,     0,     0,    33,   184,   185,     0,     0,   186,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     187,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   188,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   189,     0,    79,     0,     0,
      81,     0,     0,     0,    85,  1148,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,   191,     0,
       0,   108,     5,   306,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,   186,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   187,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   188,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     189,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   190,
       0,   106,     0,  1125,     0,     0,   108,     5,   306,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     183,     0,     0,     0,    33,   184,   185,     0,     0,  1103,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     187,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   188,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   189,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,   191,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   183,     0,     0,     0,    33,   184,   185,
       0,     0,   186,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   187,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   188,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   189,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   190,     0,   106,
       0,   191,     0,     5,   108,     6,     7,     8,   227,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   183,     0,     0,     0,
      33,   184,   185,     0,     0,   186,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   187,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   228,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   189,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     190,     0,   106,     0,   191,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,   736,     0,   183,
       0,     0,     0,    33,   184,   185,     0,     0,   186,    39,
       0,     0,     0,   737,     0,     0,    43,     0,     0,   187,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   188,     0,     0,     0,     0,     0,     0,     0,    73,
     738,    75,    76,    77,   739,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   190,     0,   106,     0,   740,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   188,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   189,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   190,     0,   106,     0,
     740,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,   819,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   187,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   188,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     189,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   190,
       0,   106,     0,   191,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   183,     0,
       0,     0,    33,   184,   185,     0,     0,   822,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   187,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     188,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   189,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   190,     0,   106,     0,   191,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   183,     0,     0,     0,    33,   184,   185,     0,     0,
    1097,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   187,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   188,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   189,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   190,     0,   106,     0,   191,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   183,     0,     0,     0,    33,   184,
     185,     0,     0,  1098,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   187,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   188,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   189,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   190,     0,
     106,     0,   191,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,  1100,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   187,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   188,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   189,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   190,     0,   106,     0,   191,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     183,     0,     0,     0,    33,   184,   185,     0,     0,  1101,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     187,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   188,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   189,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,   191,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   183,     0,     0,     0,    33,   184,   185,
       0,     0,  1102,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   187,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   188,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   189,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   190,     0,   106,
       0,   191,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   183,     0,     0,     0,
      33,   184,   185,     0,     0,  1103,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   187,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   188,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   189,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     190,     0,   106,     0,   191,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,   736,     0,   183,
       0,     0,     0,    33,   184,   185,     0,     0,   186,    39,
       0,     0,     0,   737,     0,     0,    43,     0,     0,   187,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   188,     0,     0,     0,     0,     0,     0,     0,    73,
     738,    75,    76,    77,   739,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   190,     0,   106,     0,  1215,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   188,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   189,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     964,     0,   965,     0,     0,     0,    93,    94,    95,    96,
      97,   672,     0,     0,     0,    98,   966,   256,   673,   967,
       0,     0,     0,     0,   968,    99,     0,     0,   100,   101,
     102,     0,     0,   259,   104,   183,     0,     0,   106,   969,
    1215,     0,     0,   108,     0,   970,     0,     0,     0,   262,
       0,     0,   971,     0,   674,     0,     0,     0,     0,     0,
       0,     0,   972,     0,     0,     0,     0,     0,     0,   973,
     974,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,   975,     0,     0,     0,
       0,     0,     0,     0,     0,   269,   270,     0,   976,     0,
     272,     0,   977,     0,     0,   978,     0,     0,     0,   979,
       0,     0,   276,     0,     0,   980,     0,     0,     0,     0,
       0,     0,     0,     0,   485,   404,   405,   406,   407,   408,
       0,     0,   411,   412,   413,   414,     0,   416,   417,   934,
     935,   936,   937,   938,   680,     0,   681,     0,     0,     0,
     682,   683,   684,   685,   686,   687,   688,   689,   939,   691,
     692,     0,   940,     0,     0,   694,   695,   941,   697,     0,
       5,   981,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   183,     0,     0,     0,    33,   184,   185,
       0,     0,   186,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   187,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   188,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   189,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
     101,   102,     0,     0,     0,   104,     0,   190,     0,   106,
       0,   191,     5,   306,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,   186,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   187,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   188,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     189,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,  -492,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,  -492,     0,     0,     0,  -492,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,     0,     0,     0,   190,
       0,   106,  -492,  1125,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,   186,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   187,     0,     0,
      47,    48,  -474,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   188,
       0,     0,     0,  -474,     0,     0,     0,    73,    74,    75,
      76,    77,   189,     0,    79,     0,  -474,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,  -474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   182,   102,  -474,    21,     0,     0,
       0,  -474,     0,   106,     0,  1215,    29,     0,   183,     0,
       0,     0,    33,   184,   185,     0,     0,   186,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   187,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     188,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   189,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,    95,     0,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   182,   102,     0,    21,     0,
       0,     0,   190,     0,   106,     0,   191,    29,     0,   183,
       0,     0,     0,    33,   184,   185,     0,     0,   186,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   187,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   188,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   189,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   182,   102,     0,    21,
       0,     0,     0,   190,     0,   106,     0,   740,    29,     0,
     183,     0,     0,     0,    33,   184,   185,     0,     0,   186,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     187,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   188,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   189,     0,    79,   373,     0,
      81,     0,   254,     0,    85,   374,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,   255,   256,   375,   257,
       0,     0,     0,   477,   258,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,    95,     0,     0,   260,
       0,     0,     0,     0,     0,   261,     0,     0,     0,   262,
       0,     0,   263,     0,     0,     0,   584,     0,     0,   478,
       0,     0,   264,     0,     0,   672,   106,     0,   191,   265,
     266,     0,   673,   585,     0,   376,     0,   267,     0,   377,
       0,     0,     0,     0,     0,     0,   268,   586,     0,     0,
       0,     0,     0,     0,     0,   269,   270,     0,   271,     0,
     272,     0,   273,     0,     0,   274,     0,     0,   674,   275,
     378,     0,   276,     0,     0,   277,   587,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,   479,   399,   400,   401,     0,   588,     0,     0,     0,
       0,     0,   589,   402,     0,     0,     0,     0,     0,     0,
       0,   901,     0,     0,     0,     0,     0,     0,   485,   404,
     405,   406,   407,   408,     0,     0,   411,   412,   413,   414,
       0,   416,   417,   934,   935,   936,   937,   938,   680,     0,
     681,     0,     0,     0,   682,   683,   684,   685,   686,   687,
     688,   689,   939,   691,   692,  1165,   940,     0,     0,   694,
     695,   941,   697,   942,   672,     0,     0,     0,     0,   255,
     256,   673,   257,     0,     0,     0,     0,   258,     0,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,   969,     0,     0,     0,     0,     0,   261,     0,
       0,     0,   262,     0,     0,   263,     0,   674,     0,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,   973,   266,     0,     0,     0,     0,     0,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,     0,     0,     0,     0,     0,     0,   373,   269,   270,
       0,   271,     0,   272,   374,  1166,     0,     0,   978,     0,
       0,     0,   275,     0,     0,   276,     0,   375,   277,     0,
       0,     0,     0,     0,     0,     0,     0,   485,   404,   405,
     406,   407,   408,     0,     0,   411,   412,   413,   414,     0,
     416,   417,   934,   935,   936,   937,   938,   680,     0,   681,
       0,     0,     0,   682,   683,   684,   685,   686,   687,   688,
     689,   939,   691,   692,     0,   940,     0,     0,   694,   695,
     941,   697,     0,   373,   376,     0,     0,     0,   377,     0,
     374,     0,     0,     0,     0,     0,     0,   484,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     485,   404,   405,   406,   407,   408,     0,     0,   411,   412,
     413,   414,     0,   416,   417,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     376,   399,   400,   401,   377,   433,   374,     0,     0,     0,
       0,     0,   402,  1014,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,   485,   404,   405,   406,
     407,   408,     0,     0,   411,   412,   413,   414,     0,   416,
     417,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   373,   376,   399,   400,   401,
     377,   433,   374,     0,     0,     0,     0,     0,   402,  1021,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   485,   404,   405,   406,   407,   408,     0,     0,
     411,   412,   413,   414,     0,   416,   417,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,   373,   376,   399,   400,   401,   377,   433,   374,     0,
       0,     0,     0,     0,   402,  1181,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,   485,   404,
     405,   406,   407,   408,     0,     0,   411,   412,   413,   414,
       0,   416,   417,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   373,   376,   399,
     400,   401,   377,   433,   374,     0,     0,     0,     0,     0,
     402,  1182,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,   485,   404,   405,   406,   407,   408,
       0,     0,   411,   412,   413,   414,     0,   416,   417,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,   373,   376,   399,   400,   401,   377,   433,
     374,     0,     0,     0,     0,     0,   402,  1183,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
     485,   404,   405,   406,   407,   408,     0,     0,   411,   412,
     413,   414,     0,   416,   417,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     376,   399,   400,   401,   377,   433,   374,     0,     0,     0,
       0,     0,   402,  1184,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,   485,   404,   405,   406,
     407,   408,     0,     0,   411,   412,   413,   414,     0,   416,
     417,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   373,   376,   399,   400,   401,
     377,   433,   374,     0,     0,     0,     0,     0,   402,  1207,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,   485,   404,   405,   406,   407,   408,     0,     0,
     411,   412,   413,   414,     0,   416,   417,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,   376,   399,   400,   401,   377,   433,     0,     0,
       0,     0,     0,     0,   402,  1208,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,   378,   485,   404,
     405,   406,   407,   408,     0,     0,   411,   412,   413,   414,
       0,   416,   417,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   373,     0,   399,
     400,   401,     0,   433,   374,     0,     0,     0,     0,     0,
     402,     0,   376,     0,     0,     0,   377,   375,   464,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   287,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,   475,   376,   388,   389,   390,   377,     0,
     392,   393,   394,   395,     0,     0,   398,     0,     0,   399,
     400,   401,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,   376,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   378,
       0,   399,   400,   401,     0,   433,     0,     0,     0,     0,
       0,     0,   402,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     789,   399,   400,   401,     0,   433,   374,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,   218,     0,   375,
       0,    22,    23,     0,   790,     0,     0,     0,     0,     0,
       0,   219,     0,    31,   220,     0,     0,     0,     0,    37,
       0,     0,     0,     0,   373,     0,    42,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
     791,     0,     0,     0,   375,   874,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,   376,     0,  1079,     0,
     377,  1080,     0,   223,     0,    68,     0,     0,     0,     0,
       0,     0,     0,   373,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,   378,     0,   375,     0,     0,     0,     0,     0,     0,
       0,   376,     0,     0,     0,   377,     0,   379,   380,   435,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,   467,   399,   400,   401,   378,     0,     0,     0,
     103,     0,     0,     0,   402,     0,     0,     0,     0,     0,
     376,     0,   379,   380,   377,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   373,     0,   399,   400,
     401,   202,   433,   374,     0,   378,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,   375,   287,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   373,     0,   399,   400,   401,
       0,     0,   374,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,   375,   620,     0,     0,     0,
       0,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,   621,     0,     0,     0,   373,     0,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,   622,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   623,   376,     0,   379,   380,   377,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,     0,
     399,   400,   401,     0,   433,     0,     0,   378,     0,     0,
       0,   402,   376,     0,     0,     0,   377,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   378,     0,   399,
     400,   401,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   373,     0,   399,
     400,   401,     0,     0,   374,     0,     0,     0,     0,     0,
     402,   218,     0,     0,     0,    22,    23,   375,   866,     0,
    1078,     0,     0,     0,     0,   219,     0,    31,   220,     0,
       0,     0,     0,    37,     0,     0,     0,   373,     0,     0,
      42,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,  1079,     0,   376,  1080,     0,   223,   377,    68,
       0,     0,   373,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
       0,    86,   375,     0,    88,     0,     0,     0,     0,   378,
       0,     0,     0,     0,   376,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,   379,   380,   890,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   378,
       0,   399,   400,   401,   103,   433,     0,     0,     0,   376,
    1180,     0,   402,   377,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     467,   399,   400,   401,   378,   433,   374,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,   375,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,   373,     0,   399,   400,   401,     0,
       0,   374,     0,     0,     0,   928,     0,   402,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,  1069,
       0,     0,     0,     0,     0,     0,   376,     0,     0,     0,
     377,     0,     0,     0,   373,   512,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,  1070,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   376,     0,     0,     0,   377,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,   378,   523,     0,     0,
       0,   376,     0,     0,   402,   377,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   378,     0,   399,   400,
     401,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   373,     0,   399,   400,
     401,     0,     0,   374,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,   375,   218,     0,     0,
       0,    22,    23,     0,     0,     0,  1078,     0,     0,     0,
       0,   219,     0,    31,   220,     0,   373,     0,     0,    37,
       0,     0,     0,   374,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,   376,    60,     0,    62,   377,  1079,     0,
       0,  1080,   373,   223,     0,    68,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,    84,     0,     0,    86,   378,     0,
      88,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   378,   527,
     399,   400,   401,     0,     0,     0,     0,     0,     0,   376,
     103,   402,     0,   377,   379,   380,  1230,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   373,   533,
     399,   400,   401,   550,   378,   374,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,   375,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,   373,     0,   399,   400,   401,     0,
       0,   374,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,     0,     0,     0,   377,
       0,     0,     0,     0,   373,   785,     0,     0,   624,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   376,     0,     0,     0,   377,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,     0,   399,   400,   401,     0,   378,     0,     0,     0,
       0,   376,     0,   402,     0,   377,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   378,     0,   399,   400,
     401,     0,     0,     0,     0,   764,     0,     0,     0,   402,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     786,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   373,     0,   399,   400,
     401,     0,     0,   374,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,   218,     0,     0,     0,    22,    23,     0,
       0,     0,  1078,     0,     0,     0,     0,   219,     0,    31,
     220,   373,     0,     0,     0,    37,     0,     0,   374,     0,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   376,     0,     0,     0,   377,    58,     0,
      60,   373,   352,     0,  1079,     0,     0,  1080,   374,   223,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,   378,     0,
      84,     0,     0,    86,     0,     0,    88,     0,   376,     0,
       0,     0,   377,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
    -596,   392,   393,   394,   395,   396,   397,   398,     0,   202,
     399,   400,   401,   378,     0,     0,     0,     0,   376,     0,
       0,   402,   377,     0,     0,     0,   103,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,   378,     0,   399,   400,   401,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,   849,   399,   400,   401,   373,   881,
       0,     0,     0,     0,     0,   374,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,     0,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
       0,   888,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,     0,     0,   373,   377,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,   376,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     378,     0,   399,   400,   401,   376,     0,     0,     0,   377,
       0,     0,     0,   402,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     378,     0,   399,   400,   401,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     373,     0,   399,   400,   401,     0,     0,   374,     0,   916,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   373,     0,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,   377,   373,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,   993,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,   376,     0,
       0,     0,   377,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,   931,   378,   399,   400,   401,     0,     0,   376,
       0,   917,     0,   377,     0,   402,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,   378,   399,   400,   401,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,   373,     0,   399,   400,   401,     0,
       0,   374,     0,     0,     0,     0,     0,   402,   218,     0,
       0,     0,    22,    23,   375,   996,     0,  1078,     0,     0,
       0,     0,   219,     0,    31,   220,     0,     0,     0,     0,
      37,     0,     0,     0,   373,     0,     0,    42,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,  1079,
       0,   376,  1080,     0,   223,   377,    68,     0,     0,   373,
    1066,     0,     0,     0,     0,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,     0,    86,   375,
       0,    88,     0,     0,     0,     0,   378,     0,     0,     0,
       0,   376,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   378,     0,   399,   400,
     401,   103,     0,     0,     0,     0,   376,  1231,     0,   402,
     377,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   373,     0,   399,   400,
     401,   378,     0,   374,     0,     0,     0,   999,     0,   402,
       0,     0,     0,     0,     0,     0,   375,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   373,   376,     0,     0,     0,   377,     0,   374,
       0,     0,     0,     0,     0,     0,  1096,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,     0,   376,
     399,   400,   401,   377,     0,     0,     0,   373,  1119,     0,
       0,   402,  1099,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,     0,
       0,     0,     0,   373,   376,     0,     0,   402,   377,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     376,   399,   400,   401,   377,     0,   374,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   373,   376,   399,   400,   401,
     377,     0,   374,     0,  1164,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,   373,   376,   399,   400,   401,   377,     0,   374,     0,
       0,     0,  1204,     0,   402,  1224,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   373,   376,   399,
     400,   401,   377,     0,   374,     0,     0,     0,     0,     0,
     402,  1225,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,   373,   376,   399,   400,   401,   377,     0,
     374,     0,     0,     0,     0,     0,   402,  1226,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     376,   399,   400,   401,   377,     0,   374,     0,     0,     0,
       0,     0,   402,  1227,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   373,   376,   399,   400,   401,
     377,     0,   374,     0,     0,     0,     0,     0,   402,  1228,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,   373,   376,   399,   400,   401,   377,     0,   374,     0,
       0,     0,     0,     0,   402,  1229,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1254,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   373,   376,   399,
     400,   401,   377,     0,   374,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,   373,   376,   399,   400,   401,   377,     0,
     374,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,   218,     0,     0,     0,    22,    23,     0,   378,
       0,  1078,     0,     0,     0,     0,   219,     0,    31,   220,
       0,     0,     0,     0,    37,   379,   380,     0,   381,   382,
     383,    42,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     376,   399,   400,   401,   377,     0,   374,    58,     0,    60,
    1283,    62,   402,  1079,     0,     0,  1080,     0,   223,   375,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,   845,   399,   400,   401,
     377,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,   103,     0,     0,     0,     0,
       0,  1232,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,     0,     0,   399,   400,   401,   798,     0,   254,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,   255,   256,     0,   257,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,   799,     0,     0,   259,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   261,     0,     0,     0,   262,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,   265,   266,     0,     0,     0,
       0,     0,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,   254,     0,     0,     0,     0,
       0,   269,   270,     0,   271,     0,   272,     0,   273,   255,
     256,   274,   257,     0,     0,   275,     0,   258,   276,    23,
       0,   277,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,   260,     0,     0,     0,     0,     0,   261,     0,
       0,     0,   262,     0,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,   265,   266,     0,     0,     0,     0,     0,     0,
     267,    60,     0,     0,     0,     0,     0,     0,     0,   268,
       0,     0,    68,     0,     0,     0,     0,     0,   269,   270,
       0,   271,     0,   272,     0,   273,   254,     0,   274,     0,
       0,   957,   275,     0,     0,   276,     0,    88,   277,     0,
     255,   256,     0,   257,     0,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,     0,   261,
       0,     0,     0,   262,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,   265,   266,     0,     0,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
     268,     0,     0,   254,     0,     0,     0,     0,     0,   269,
     270,     0,   271,     0,   272,     0,   273,   255,   256,   274,
     257,     0,     0,   275,     0,   258,   276,     0,     0,   277,
       0,   799,     0,     0,   259,     0,     0,     0,     0,     0,
     260,     0,     0,     0,     0,     0,   261,     0,     0,     0,
     262,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
     265,   266,     0,     0,     0,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,   268,     0,     0,
    -300,     0,     0,     0,     0,     0,   269,   270,     0,   271,
       0,   272,     0,   273,  -300,  -300,   274,  -300,     0,     0,
     275,     0,  -300,   276,     0,     0,   277,     0,     0,     0,
       0,  -300,     0,     0,     0,     0,     0,  -300,     0,     0,
       0,     0,     0,  -300,     0,     0,     0,  -300,     0,     0,
    -300,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -300,     0,     0,     0,     0,     0,     0,  -300,  -300,     0,
       0,     0,     0,     0,     0,  -300,     0,     0,     0,     0,
       0,     0,     0,     0,  -300,     0,     0,  -310,     0,     0,
       0,     0,     0,  -300,  -300,     0,  -300,     0,  -300,     0,
    -300,  -310,  -310,  -300,  -310,     0,     0,  -300,     0,  -310,
    -300,     0,     0,  -300,     0,     0,     0,     0,  -310,     0,
       0,     0,     0,     0,  -310,     0,     0,     0,     0,     0,
    -310,     0,     0,     0,  -310,     0,     0,  -310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -310,     0,     0,
       0,     0,     0,     0,  -310,  -310,     0,     0,     0,     0,
       0,     0,  -310,     0,     0,     0,     0,     0,     0,     0,
       0,  -310,     0,     0,   254,     0,     0,     0,     0,     0,
    -310,  -310,     0,  -310,     0,  -310,     0,  -310,   255,   256,
    -310,   257,     0,     0,  -310,     0,   258,  -310,     0,     0,
    -310,     0,     0,     0,     0,   259,     0,     0,     0,     0,
       0,   260,     0,     0,     0,     0,     0,   261,     0,     0,
       0,   262,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,     0,     0,     0,
       0,   265,   266,     0,     0,     0,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
       0,  -301,     0,     0,     0,     0,     0,   269,   270,     0,
     271,     0,   272,     0,   273,  -301,  -301,   274,  -301,     0,
       0,   275,     0,  -301,   276,     0,     0,   277,     0,     0,
       0,     0,  -301,     0,     0,     0,     0,     0,  -301,     0,
       0,     0,     0,     0,  -301,     0,     0,     0,  -301,     0,
       0,  -301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -301,     0,     0,     0,     0,     0,     0,  -301,  -301,
       0,     0,     0,     0,     0,     0,  -301,     0,     0,     0,
       0,     0,     0,     0,     0,  -301,     0,     0,  -193,     0,
       0,     0,     0,     0,  -301,  -301,     0,  -301,     0,  -301,
       0,  -301,  -193,  -193,  -301,  -193,     0,     0,  -301,     0,
    -193,  -301,     0,     0,  -301,     0,     0,     0,     0,  -193,
       0,     0,     0,     0,     0,  -193,     0,     0,     0,     0,
       0,  -193,     0,     0,     0,  -193,     0,     0,  -193,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -193,     0,
       0,     0,     0,     0,     0,  -193,  -193,     0,     0,     0,
       0,     0,     0,  -193,     0,     0,     0,     0,     0,     0,
       0,     0,  -193,     0,     0,  -185,     0,     0,     0,     0,
       0,  -193,  -193,     0,  -193,     0,  -193,     0,  -193,  -185,
    -185,  -193,  -185,     0,     0,  -193,     0,  -185,  -193,     0,
       0,  -193,     0,     0,     0,     0,  -185,     0,     0,     0,
       0,     0,  -185,     0,     0,     0,     0,     0,  -185,     0,
       0,     0,  -185,     0,     0,  -185,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -185,     0,     0,     0,     0,
       0,     0,  -185,  -185,     0,     0,     0,     0,     0,     0,
    -185,     0,     0,     0,     0,     0,     0,     0,     0,  -185,
       0,     0,     0,     0,     0,     0,     0,     0,  -185,  -185,
       0,  -185,     0,  -185,     0,  -185,     0,     0,  -185,     0,
       0,     0,  -185,     0,     0,  -185,     0,     0,  -185
};

static const yytype_int16 yycheck[] =
{
      12,   253,    19,   430,    16,     2,    65,     2,    20,   528,
     809,   504,   477,   242,    26,   168,   492,   499,   809,    31,
      32,   525,    34,    35,    36,    37,    38,   154,   210,   791,
     649,    27,    22,   786,   653,   528,   655,    49,   735,    51,
      52,   799,     3,   663,     1,    57,     1,    59,   910,    61,
       1,   303,   234,  1135,  1040,    56,  1190,    69,    70,    71,
      72,    73,    74,     1,    69,    33,    78,    48,    80,   321,
     955,  1043,   986,   102,   988,    87,     0,    48,    90,    91,
      33,    93,  1123,    95,   211,    48,    98,    99,    75,   101,
      56,   146,   146,   105,   106,   107,   108,    47,   251,    61,
      37,     3,   107,    48,    61,  1244,   140,    92,   105,  1261,
    1244,    33,   165,    33,   148,     1,   171,    56,   347,   348,
      33,   175,    69,    61,    75,   146,   879,  1279,   226,   118,
     117,    34,    35,    36,    33,    92,    73,    48,  1124,   120,
    1002,   170,   103,   647,  1283,    90,    92,   844,  1189,   625,
     171,    48,   164,    48,    92,   253,   118,   146,   105,   106,
     107,   118,    48,   222,   114,   146,   117,   105,   420,   421,
     125,   172,   184,   185,   186,   146,     3,   189,   190,   191,
     118,  1263,  1044,   146,   171,   170,   191,   120,   145,  1161,
     145,   146,   209,  1078,   175,   120,    66,  1082,   120,   957,
     120,   103,   170,   120,   107,   303,   172,   145,   220,     3,
     907,   831,   175,   170,   226,    85,    32,   170,   173,   165,
     171,   217,  1107,   321,   170,  1110,  1111,   165,    48,   172,
      70,    61,   170,   146,   853,   146,   663,   856,   857,   125,
     230,   253,   175,   190,   191,   304,   236,   146,   170,   146,
     170,   146,   734,  1167,  1168,   245,   146,   170,   175,   145,
     146,  1123,    92,    56,   175,    92,   106,    47,    27,   146,
      90,   170,  1157,    89,    56,   105,   103,    93,   175,   165,
     175,    54,   125,   173,   146,    27,    66,   385,   118,   165,
    1175,   303,  1177,  1178,  1093,   171,   173,    88,   310,   311,
      56,    74,  1093,   165,   165,    85,  1068,   150,  1070,   321,
     171,    84,   146,   146,   146,  1008,    61,   793,  1011,  1012,
      79,   112,   420,   421,   140,   790,   453,  1189,   146,   145,
     342,    90,   148,   460,   114,   165,   152,    79,   171,   173,
     170,   173,   339,   140,   339,   357,   358,    92,    90,    27,
     109,   148,   828,    90,    18,   173,   115,   354,    56,   371,
     105,   373,   374,   375,   376,   377,   378,   109,   380,   381,
     382,   383,   384,   118,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,    61,   140,   172,   492,   174,   125,   409,   410,  1019,
     148,    79,   146,   415,   831,   146,   418,   419,   420,   421,
     422,   423,    90,  1106,    78,  1108,  1109,   146,   430,   901,
     165,   150,    92,   435,   165,   170,    27,   525,   440,   173,
      27,   443,    96,   146,   146,   105,   433,   115,   667,   668,
     146,    56,   146,   125,   173,     3,   172,   165,   118,   113,
     462,    48,   464,   465,   172,    52,   146,   462,   470,   471,
     173,   173,    20,   475,   146,   477,   478,   173,   150,   173,
     146,   146,   484,   376,   377,   487,    34,   942,    79,    76,
     492,    49,    79,   173,    81,   172,    56,   174,   146,    90,
     165,   173,   146,    90,   506,   165,     8,   173,   510,   496,
     170,   513,   514,   230,    72,    63,   518,   165,   109,   236,
     146,   165,   109,   525,   146,   462,    32,    56,   146,   422,
     423,   146,   512,    56,    56,   537,    32,   625,   540,   165,
     542,  1050,   435,   165,   546,   547,   548,   165,   550,   551,
     165,   172,   771,   772,   146,   103,   146,   146,    89,   647,
    1043,   109,    93,   565,    89,    56,   125,  1050,    93,   556,
     565,   556,   465,   165,   100,   165,   165,    27,   471,   409,
     410,   518,   475,    89,   142,   415,   173,    93,   418,   419,
     762,    56,   151,    89,   153,   154,   146,    93,   157,   158,
     537,    51,  1019,   540,   165,   542,    83,   172,    56,    86,
     547,   172,  1067,   174,  1069,   165,    56,   170,   620,   621,
     622,   623,   624,   625,   626,   627,    89,    77,   565,    79,
      93,   638,   639,    89,   140,    56,    56,    93,    32,   145,
      90,   147,   148,   645,   140,   647,   152,    85,    56,   145,
     208,   147,   148,   546,   160,   657,   152,   659,   172,    56,
     174,   663,   168,   665,   160,   115,   809,   172,   165,   174,
     166,   167,   168,   675,   164,   677,    89,   170,  1161,   172,
      93,   239,   240,   241,   242,   243,   244,   172,   690,   174,
      89,   693,   250,   251,    93,    89,   165,   785,   786,    93,
     172,   140,   174,   705,   706,   793,   145,  1190,   645,   148,
     540,   165,   542,   152,   156,   540,    48,   542,   165,    32,
     863,  1204,   280,   172,   165,   170,   119,   870,   621,    61,
     623,   165,   290,   165,   292,   165,   738,   739,   740,   165,
     828,   299,   744,    27,    88,   740,   140,   141,   172,   143,
     144,   145,   125,   147,   148,   149,   172,   172,   152,   317,
      92,  1244,   172,   172,    48,   159,   160,   165,    52,   163,
     328,   329,   166,   167,   168,   165,    89,   172,  1261,     8,
      93,   783,   145,   177,    66,   787,   118,   789,   790,   791,
     170,   793,    76,   125,   148,    79,  1279,    81,    69,   165,
    1283,   172,   125,   740,   173,    48,    90,   146,   165,  1051,
    1052,    48,   172,   125,   146,   645,   125,   819,    61,    35,
     822,    35,   173,   173,   146,   109,   828,   140,   141,   831,
     143,   144,   145,   165,   147,   148,   125,   173,   170,   152,
       2,   173,  1195,   845,   846,   173,   159,   160,   146,    92,
     163,   175,    21,   166,   167,   168,   175,   172,   165,   170,
     165,   165,   864,    25,   866,   165,    28,   173,   165,   871,
     165,   172,   874,   175,   146,   118,    35,   879,   436,   881,
     165,    35,   125,    27,    66,   146,   888,   173,   171,   146,
      32,   125,   173,   173,   171,  1248,  1249,  1250,  1251,  1252,
    1253,    90,    33,   146,    48,   173,    33,   170,    52,   175,
     170,   469,   899,    35,   744,   473,    78,   170,   920,   165,
     145,   175,   165,    90,   926,   920,   928,   170,   172,   931,
     173,   172,    76,   175,   165,    79,   175,    81,   145,   171,
     942,    35,   172,   105,   148,   148,    90,    89,   110,   148,
    1093,    93,   845,   846,   140,   148,   148,   959,   148,   140,
     148,   148,   140,  1051,  1052,   109,   140,   140,   148,   148,
    1113,  1114,   148,   148,  1117,  1118,   170,   173,   173,   981,
     142,   105,   165,   920,   140,   170,   170,   140,   165,   146,
     992,   993,   490,   995,   996,   165,   998,   999,   140,   141,
    1143,   173,  1145,   145,   165,   147,   148,   173,   173,    48,
     152,   175,  1014,  1015,   164,  1017,   164,  1019,   160,  1021,
    1022,   142,    61,  1025,   166,   167,   168,   354,   783,    24,
      32,  1080,   510,    90,  1082,   894,   960,   950,  1093,  1089,
     726,   203,   899,   963,  1220,  1263,  1254,  1049,  1050,  1051,
    1052,  1247,    37,    92,  1056,   507,    31,    -1,  1060,     2,
      -1,    -1,    -1,    -1,  1066,  1067,  1068,  1069,  1070,    -1,
      -1,    -1,    -1,  1075,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    25,    -1,    -1,    28,   125,    89,    -1,    -1,
      -1,    93,    -1,    -1,  1096,  1097,  1098,  1099,  1100,  1101,
    1102,  1103,  1089,   661,   662,    -1,  1043,   146,    -1,   667,
     668,    -1,   670,   671,    -1,    -1,    -1,  1119,    -1,   281,
      -1,    -1,   124,  1125,    -1,   287,   165,    -1,  1130,    -1,
    1125,   170,  1134,    -1,   173,    78,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,   712,   157,   158,   159,   160,   161,
     162,   163,   105,    -1,   166,   167,   168,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,   339,    -1,  1181,
    1182,  1183,  1184,    -1,    -1,    -1,    -1,    -1,  1125,    -1,
      -1,    -1,   354,  1130,    -1,    -1,    -1,    -1,    -1,   142,
      -1,    -1,  1204,    -1,    -1,  1207,  1208,    -1,    -1,    -1,
      -1,    -1,    -1,  1215,    -1,    -1,    -1,    -1,    -1,    -1,
    1215,    -1,  1224,  1225,  1226,  1227,  1228,  1229,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   794,    -1,    -1,    -1,
      -1,  1243,  1244,    -1,  1246,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1190,    -1,    -1,    -1,    -1,    -1,     2,
     203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   433,    -1,    -1,    -1,    -1,    -1,  1279,  1215,    -1,
      -1,  1283,    25,    -1,    -1,    28,    -1,    -1,    -1,   847,
     848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   464,   861,    -1,   863,    -1,  1244,   470,    -1,
      -1,    -1,   870,    -1,    -1,   873,    -1,    -1,   876,    -1,
     482,    -1,   484,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,     1,    78,    -1,   895,   281,    -1,
      -1,    32,    -1,    -1,   287,    -1,  1283,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    22,    28,    -1,    -1,    26,
      27,     8,   105,    -1,    31,    -1,    -1,   110,    -1,    36,
      -1,    38,    39,    -1,    -1,    22,    -1,    44,    -1,    26,
      27,    -1,   544,    -1,    51,    -1,   548,    54,    -1,    36,
      -1,    38,    39,    -1,   556,   953,   339,    44,    89,   142,
      -1,    -1,    93,    -1,    51,    -1,    78,    74,    -1,    -1,
      77,   354,    79,    -1,    81,    -1,    83,    84,    -1,    86,
      -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,   105,    -1,    -1,    83,    -1,   110,    86,
      -1,    88,   109,    90,    -1,   112,    -1,    -1,   115,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,    -1,    -1,
     203,   152,   109,    -1,    -1,   112,     3,    -1,   115,   160,
     142,    -1,    -1,   635,    -1,   166,   167,   168,    -1,    -1,
      17,    -1,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
     433,    -1,    -1,    -1,    12,    -1,    -1,  1055,   165,    36,
      -1,    19,    -1,    -1,   171,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    32,    33,    53,    -1,   165,    -1,
      -1,   464,    -1,    -1,    -1,    -1,    63,   470,    -1,    -1,
      48,   203,    -1,    -1,    71,    -1,    -1,    -1,   281,   482,
      -1,   484,    -1,    -1,   287,    -1,   708,    -1,    -1,   711,
      87,    -1,    -1,    -1,    -1,  1113,  1114,    -1,    -1,  1117,
    1118,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    89,    -1,   110,    -1,    93,    -1,  1135,    -1,   116,
      -1,    -1,    -1,    -1,    -1,  1143,    -1,  1145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   339,  1155,    -1,    -1,
      -1,   544,   120,    -1,    -1,   548,   124,    -1,   770,   281,
      -1,   354,    -1,   556,   776,   287,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,   809,   166,   167,
     168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    22,   177,
      -1,    -1,    26,    27,    -1,    -1,    -1,   339,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,     2,    -1,    -1,
      44,    -1,   354,   845,   846,    -1,    -1,    51,    -1,    -1,
     433,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,   864,    28,   866,  1263,    -1,    -1,    -1,   871,
      -1,    -1,   874,    77,    -1,    79,    -1,    -1,    -1,    -1,
      -1,   464,    -1,   885,    88,   887,    90,   470,   890,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,   482,
      -1,   484,    26,    27,    -1,   109,    -1,    31,   112,    -1,
      -1,   115,    36,    78,    38,    39,    -1,   919,    -1,    -1,
      44,   433,    -1,    -1,   926,   708,   928,    51,   711,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,   110,    89,    -1,    -1,    -1,
      93,    -1,   464,    77,    -1,    79,    -1,    81,   470,    83,
      -1,   544,    86,    -1,    88,   548,    90,    -1,    -1,    -1,
     482,    -1,   484,   556,    -1,    -1,    -1,   142,    -1,    -1,
      -1,   124,    -1,    -1,    -1,   109,    -1,   770,   112,    -1,
      -1,   115,    -1,   776,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,  1014,    -1,   157,   158,   159,   160,   161,  1021,
     163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,   544,  1035,   177,    -1,   548,    -1,   203,    -1,
      -1,   165,    -1,    -1,   556,    -1,    -1,   171,    -1,    -1,
       2,  1053,    -1,    -1,    -1,    -1,    -1,  1059,    -1,    -1,
      -1,  1063,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    17,    18,    -1,
      20,   864,    -1,   866,    -1,    25,    -1,    -1,   871,    -1,
      -1,   874,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,   885,    -1,   887,    -1,    46,   890,    32,    -1,
      50,    -1,    -1,    53,    -1,    -1,   281,    -1,  1120,    -1,
      -1,    -1,   287,    63,    -1,   708,    78,    -1,   711,    -1,
      70,    71,    -1,    -1,    -1,    -1,   919,    -1,    78,    -1,
      -1,    -1,    -1,   926,    -1,   928,    -1,    87,    -1,    -1,
      -1,    -1,    -1,   105,    -1,    -1,    96,    97,   110,    99,
      -1,   101,    -1,   103,    -1,    89,   106,    -1,    -1,    93,
     110,   111,    -1,   113,   339,    -1,   116,    -1,    -1,  1181,
    1182,  1183,  1184,    -1,    -1,    -1,    -1,   770,    -1,   354,
     142,    -1,    -1,   776,    -1,    -1,   708,    -1,    -1,   711,
      -1,    -1,    -1,    -1,    -1,  1207,  1208,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,  1014,   172,   157,   158,   159,   160,   161,  1021,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,   203,  1035,   177,    -1,    -1,    -1,    -1,   770,    -1,
      -1,    -1,    -1,    -1,   776,    -1,    -1,    -1,   433,    89,
    1053,    -1,    -1,    93,    -1,    -1,  1059,    -1,    -1,    -1,
    1063,   864,    -1,   866,    -1,    -1,    -1,    -1,   871,    -1,
      -1,   874,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   464,
       2,    -1,   885,    -1,   887,   470,    -1,   890,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   482,    -1,   484,
     140,   141,    -1,    25,   144,   145,    28,   147,   148,   281,
      -1,    -1,   152,    -1,    -1,   287,   919,  1120,    -1,    -1,
     160,    -1,    -1,   926,    -1,   928,   166,   167,   168,    -1,
      -1,    -1,   864,    -1,   866,    -1,    -1,    -1,    -1,   871,
      -1,    -1,   874,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   885,    -1,   887,    78,    -1,   890,   544,
      -1,    -1,    -1,   548,    -1,    -1,    -1,   339,    -1,    -1,
      -1,   556,    -1,    -1,    -1,    -1,    -1,    -1,  1181,  1182,
    1183,  1184,   354,   105,    -1,    -1,    -1,   919,   110,    -1,
      -1,    -1,    -1,    -1,   926,     8,   928,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1207,  1208,    -1,    -1,    -1,    22,
      -1,  1014,    -1,    26,    27,    -1,    -1,    -1,  1021,    -1,
     142,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,  1035,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1053,    -1,    -1,    66,    -1,    -1,  1059,    -1,    -1,    -1,
    1063,   433,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,
      83,    -1,    85,    86,    -1,    88,    -1,    90,    -1,    -1,
      -1,   203,  1014,    -1,    -1,    -1,    -1,    -1,    -1,  1021,
       2,    -1,   464,    -1,    -1,    -1,   109,    -1,   470,   112,
      -1,    -1,   115,  1035,    -1,    -1,    -1,    -1,    -1,    -1,
     482,    -1,   484,    25,    -1,    -1,    28,  1120,    -1,    -1,
       2,  1053,    -1,   708,    -1,    -1,   711,  1059,    -1,    -1,
      -1,  1063,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,   281,
      -1,    -1,    -1,    -1,    -1,   287,    78,    -1,    -1,    -1,
      -1,    -1,   544,    -1,    -1,    -1,   548,    -1,  1181,  1182,
    1183,  1184,    -1,    -1,   556,   770,    -1,    -1,  1120,    -1,
      -1,   776,    -1,   105,     3,    -1,    78,    -1,   110,    -1,
      -1,    -1,    -1,    -1,  1207,  1208,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,   339,    -1,    -1,
      -1,    -1,    -1,   105,    -1,    34,    -1,    -1,   110,    -1,
     142,    40,   354,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,  1181,
    1182,  1183,  1184,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,  1207,  1208,    -1,    87,   864,
      -1,   866,    -1,    -1,    -1,    -1,   871,    96,    97,   874,
      99,   203,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
     885,   110,   887,    -1,   113,   890,    -1,   116,    -1,    -1,
      -1,   433,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    -1,    -1,    -1,   919,    -1,   708,    -1,    -1,   711,
      -1,   926,   464,   928,    -1,    -1,    -1,    -1,   470,    -1,
      25,    22,    -1,    28,    -1,    26,    27,    -1,    -1,    -1,
     482,   170,   484,   172,    -1,    36,    -1,    38,    39,   281,
      -1,    -1,    -1,    44,    -1,   287,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    22,    -1,   770,   281,
      26,    27,    -1,    78,   776,   287,    77,    -1,    79,    -1,
      36,    -1,    38,    39,    85,    -1,    -1,    88,    44,    90,
      -1,    -1,   544,    -1,    -1,    51,   548,   339,    -1,  1014,
     105,    -1,    -1,    -1,   556,   110,  1021,    -1,   109,    -1,
      66,   112,   354,   114,   115,    -1,    -1,    -1,    -1,    -1,
    1035,    77,    -1,    79,    -1,    -1,    -1,   339,    72,    85,
      -1,    -1,    88,    -1,    90,    -1,    -1,    -1,  1053,    -1,
      -1,    -1,   354,    -1,  1059,    -1,    -1,    -1,  1063,    -1,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,   864,    -1,   866,    -1,    -1,    -1,    -1,   871,
      22,    -1,   874,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   885,    36,   887,    38,    39,   890,    -1,
      -1,   433,    44,    -1,    -1,    -1,    -1,    -1,   203,    51,
      -1,    -1,    -1,    -1,    -1,  1120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,   919,    -1,    -1,
      -1,   433,   464,    -1,   926,    77,   928,    79,   470,    -1,
      -1,    83,    -1,    85,    86,    -1,    88,    -1,    90,    -1,
     482,    -1,   484,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   464,    -1,    -1,    -1,   708,   109,   470,   711,
     112,    -1,    -1,   115,    -1,    -1,  1181,  1182,  1183,  1184,
     482,    -1,   484,    -1,    -1,    -1,   281,    -1,    -1,    -1,
      -1,    -1,   287,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1207,  1208,    -1,   239,   240,   241,   242,   243,
     244,    -1,   544,    -1,    -1,    -1,   548,    -1,    -1,    -1,
      -1,    -1,  1014,   165,   556,    -1,    -1,    -1,   770,  1021,
      -1,    -1,    -1,    -1,   776,    -1,    -1,    44,    45,    46,
      -1,    -1,   544,  1035,   339,    -1,   548,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   556,    -1,   290,    -1,   292,   354,
      -1,  1053,    -1,    -1,    -1,   299,    -1,  1059,    -1,    -1,
      -1,  1063,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   328,   329,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   864,    -1,   866,    -1,    -1,    -1,  1120,   871,
      -1,   156,   874,    -1,    -1,    -1,    -1,    -1,   433,    -1,
     147,    -1,   149,   885,   169,   887,    -1,   172,   890,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   464,
      -1,    -1,    -1,    -1,    -1,   470,   708,   919,    -1,   711,
     187,    -1,    -1,    -1,   926,    -1,   928,   482,    -1,  1181,
    1182,  1183,  1184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   436,   210,    -1,    -1,   708,    -1,    -1,   711,
      -1,    -1,    -1,    -1,    -1,  1207,  1208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   234,    -1,    -1,
     237,    -1,    -1,    -1,    -1,   469,    -1,    -1,   770,   473,
      -1,   248,   249,    -1,   776,    -1,    -1,    -1,    -1,   544,
      -1,    -1,    -1,   548,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   556,    -1,     3,    -1,    -1,    -1,    -1,   770,    -1,
      -1,    -1,  1014,    -1,   776,    -1,   283,    17,    18,  1021,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1035,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,  1053,    -1,    53,    -1,    -1,    -1,  1059,    -1,    -1,
      -1,  1063,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,   864,    -1,   866,    -1,    -1,    -1,    78,   871,
      -1,    -1,   874,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,   359,   885,    -1,   887,    96,    97,   890,    99,
      -1,   101,   864,   103,   866,    -1,   106,    -1,    -1,   871,
     110,    -1,   874,   113,    -1,    -1,   116,    -1,  1120,    -1,
      -1,    -1,    -1,   885,    -1,   887,    -1,   919,   890,    -1,
      -1,    -1,    -1,    -1,   926,    -1,   928,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   708,    -1,    -1,   711,   919,    -1,    -1,
      -1,    -1,   429,    -1,   926,    -1,   928,   661,   662,    -1,
      -1,    -1,   172,   667,   668,    -1,   670,   671,    -1,  1181,
    1182,  1183,  1184,    -1,   451,    -1,    -1,    -1,    -1,   456,
      -1,   458,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    31,    -1,  1207,  1208,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,   770,    44,    -1,    -1,    -1,
      -1,   776,  1014,    51,    -1,    -1,   493,    -1,    -1,  1021,
      -1,    -1,   499,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1035,    -1,   512,    -1,    -1,    -1,    77,
      -1,    79,  1014,    81,    -1,    83,    -1,    -1,    86,  1021,
      88,  1053,    90,    -1,    -1,    -1,    -1,  1059,    -1,    -1,
      -1,  1063,    -1,  1035,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
     557,  1053,    -1,    -1,    -1,    -1,    -1,  1059,    -1,    -1,
     567,  1063,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   864,
      -1,   866,    -1,    -1,    -1,    -1,   871,    -1,    -1,   874,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1120,    -1,
     885,    -1,   887,    -1,    -1,   890,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   847,   848,    -1,    -1,    -1,  1120,    -1,
      -1,    -1,    -1,    -1,   919,    -1,    -1,   861,    -1,    -1,
      -1,   926,    -1,   928,    -1,    -1,    -1,    -1,    -1,   873,
      -1,   648,   876,    -1,   651,   652,    -1,    -1,    -1,  1181,
    1182,  1183,  1184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,  1207,  1208,    -1,    31,  1181,
    1182,  1183,  1184,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,  1207,  1208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   730,    77,    -1,    79,   734,    81,    -1,
      83,    -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,
    1035,    -1,    -1,   750,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   762,   109,    -1,  1053,   112,
      -1,    -1,   115,    -1,  1059,    -1,    -1,     3,  1063,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   794,    -1,    25,
      26,    27,   799,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,   809,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,   165,    -1,    50,    -1,    -1,    53,   171,    -1,
      56,  1055,    -1,    59,    60,  1120,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,   892,    -1,   894,   124,    -1,
      -1,   898,    -1,    -1,   901,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,    -1,   950,    -1,    -1,    -1,    -1,    -1,    -1,
     957,     0,     1,   960,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1093,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
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
      -1,   170,    -1,   172,    -1,   174,   175,     1,   177,     3,
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
      -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,   147,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   166,   167,   168,   169,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    -1,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    33,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    64,    65,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,   119,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
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
     103,    -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
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
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,   177,
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
     173,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    48,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     3,   177,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    14,    -1,    -1,
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
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
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
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
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
      -1,    67,    68,    69,    70,    71,    -1,    -1,    -1,    -1,
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
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
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
     164,    -1,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
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
      -1,   170,    -1,   172,    -1,   174,   175,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    93,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
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
      -1,   172,    -1,   174,   175,     3,   177,     5,     6,     7,
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
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
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
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,     3,
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
     106,    -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,
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
       1,    -1,     3,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    12,    -1,    -1,    -1,   149,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    25,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    34,   168,    36,    -1,    -1,   172,    40,
     174,    -1,    -1,   177,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   163,    -1,    -1,   166,   167,   168,   169,    -1,
       3,   172,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,     3,     4,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    61,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,   105,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,   164,   165,    25,    -1,    -1,
      -1,   170,    -1,   172,    -1,   174,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,    -1,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,   164,    -1,    25,    -1,
      -1,    -1,   170,    -1,   172,    -1,   174,    34,    -1,    36,
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
      -1,    -1,    -1,    -1,    -1,   142,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   164,    -1,    25,
      -1,    -1,    -1,   170,    -1,   172,    -1,   174,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    12,    -1,
     106,    -1,     3,    -1,   110,    19,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    17,    18,    32,    20,
      -1,    -1,    -1,    37,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,   142,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,     3,    -1,    -1,    73,
      -1,    -1,    63,    -1,    -1,    12,   172,    -1,   174,    70,
      71,    -1,    19,    20,    -1,    89,    -1,    78,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    55,   110,
     124,    -1,   113,    -1,    -1,   116,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,   167,   168,    -1,   103,    -1,    -1,    -1,
      -1,    -1,   109,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,     3,   163,    -1,    -1,   166,
     167,   168,   169,   170,    12,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    96,    97,
      -1,    99,    -1,   101,    19,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    32,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,   169,    -1,    12,    89,    -1,    -1,    -1,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,    89,   166,   167,   168,    93,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    -1,   166,
     167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    89,    -1,    -1,    -1,    93,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    48,    89,   152,   153,   154,    93,    -1,
     157,   158,   159,   160,    -1,    -1,   163,    -1,    -1,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      13,   166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    22,    -1,    32,
      -1,    26,    27,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    12,    -1,    51,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    89,    -1,    83,    -1,
      93,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,   124,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,   140,   141,    48,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,   120,   166,   167,   168,   124,    -1,    -1,    -1,
     165,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      89,    -1,   140,   141,    93,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,   120,   170,    19,    -1,   124,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    89,    -1,   140,   141,    93,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,    -1,   170,    -1,    -1,   124,    -1,    -1,
      -1,   177,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    -1,   166,
     167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    22,    -1,    -1,    -1,    26,    27,    32,    33,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    12,    -1,    -1,
      51,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    83,    -1,    89,    86,    -1,    88,    93,    90,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,   112,    32,    -1,   115,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   102,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,   165,   170,    -1,    -1,    -1,    89,
     171,    -1,   177,    93,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
     120,   166,   167,   168,   124,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,   175,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    73,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,   124,   170,    -1,    -1,
      -1,    89,    -1,    -1,   177,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    12,    -1,    -1,    44,
      -1,    -1,    -1,    19,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    89,    79,    -1,    81,    93,    83,    -1,
      -1,    86,    12,    88,    -1,    90,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,   109,    -1,    -1,   112,   124,    -1,
     115,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,   165,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    89,
     165,   177,    -1,    93,   140,   141,   171,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,   165,
     166,   167,   168,   123,   124,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    12,    13,    -1,    -1,   102,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,   177,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    12,    -1,    -1,    -1,    44,    -1,    -1,    19,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    77,    -1,
      79,    12,    81,    -1,    83,    -1,    -1,    86,    19,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    89,    -1,
      -1,    -1,    93,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   120,
     166,   167,   168,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,   177,    93,    -1,    -1,    -1,   165,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,   167,   168,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,   173,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   123,   124,   166,   167,   168,    -1,    -1,    89,
      -1,   173,    -1,    93,    -1,   177,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,   124,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    22,    -1,
      -1,    -1,    26,    27,    32,    33,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    12,    -1,    -1,    51,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
      -1,    89,    86,    -1,    88,    93,    90,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    32,
      -1,   115,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,   165,    -1,    -1,    -1,    -1,    89,   171,    -1,   177,
      93,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,   124,    -1,    19,    -1,    -1,    -1,   175,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    89,    -1,    -1,    -1,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    89,
     166,   167,   168,    93,    -1,    -1,    -1,    12,    13,    -1,
      -1,   177,   102,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    12,    89,    -1,    -1,   177,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,   173,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,   175,    -1,   177,   102,    -1,    -1,    -1,    -1,
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
      61,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,   124,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,   140,   141,    -1,   143,   144,
     145,    51,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    77,    -1,    79,
     175,    81,   177,    83,    -1,    -1,    86,    -1,    88,    32,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,    89,   166,   167,   168,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,     1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    17,
      18,   106,    20,    -1,    -1,   110,    -1,    25,   113,    27,
      -1,   116,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,     3,    -1,   106,    -1,
      -1,     8,   110,    -1,    -1,   113,    -1,   115,   116,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    17,    18,   106,
      20,    -1,    -1,   110,    -1,    25,   113,    -1,    -1,   116,
      -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    17,    18,   106,    20,    -1,    -1,   110,    -1,
      25,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    17,
      18,   106,    20,    -1,    -1,   110,    -1,    25,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
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
     185,   186,   187,   188,   189,   190,   191,   192,   195,   197,
     202,   203,   204,   207,   208,   212,   214,   217,   220,   222,
     224,   225,   226,   233,   234,   236,   238,   241,   242,   243,
     244,   245,   246,   247,   251,   252,   255,   256,   259,   260,
     264,   267,   268,   296,   299,   300,   320,   321,   322,   323,
     324,   325,   326,   334,   335,   336,   337,   338,   341,   342,
     343,   344,   345,   346,   347,   348,   350,   351,   352,   353,
     354,   165,    22,    36,    41,    42,    45,    56,    88,   101,
     170,   174,   241,   264,   320,   325,   336,   337,   338,   341,
     343,   344,   120,   327,   328,     3,   209,   338,   327,   338,
     109,   299,    90,   209,   186,   314,   338,     8,    22,    36,
      39,    83,    86,    88,   189,   186,   172,     8,    88,   338,
     339,     8,    11,    88,   109,   112,   339,    79,   122,   235,
     338,   235,   338,   235,   338,    26,   300,   338,    27,   115,
     237,   338,   196,   172,     3,    17,    18,    20,    25,    34,
      40,    46,    50,    53,    63,    70,    71,    78,    87,    96,
      97,    99,   101,   103,   106,   110,   113,   116,   211,   213,
     211,   211,   265,   172,   211,   301,   302,    33,   197,   216,
     338,   218,   219,   338,   338,    18,    78,    96,   113,   338,
     338,   338,     8,   172,   223,   224,     4,   290,   313,   338,
     107,   108,   165,   338,   340,   338,   216,   338,   338,   338,
     100,   172,   186,   338,   338,   211,   297,   338,   237,   338,
     338,   338,   338,   338,   338,   338,     1,   171,   184,   198,
     313,   111,   150,   290,   315,   316,   340,   235,   313,   338,
     349,   338,    81,   186,   170,    85,   193,    47,   114,    56,
     187,   197,   211,   211,    54,    74,    84,   285,   301,   164,
     165,   156,   338,    12,    19,    32,    89,    93,   124,   140,
     141,   143,   144,   145,   147,   148,   149,   151,   152,   153,
     154,   155,   157,   158,   159,   160,   161,   162,   163,   166,
     167,   168,   177,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   169,   275,
     172,   174,    89,    93,   338,   338,   338,   211,   313,    56,
     172,   186,   165,   170,   197,    48,   327,   297,   301,   165,
     146,   165,   189,   119,   212,   290,   317,   318,   319,   340,
      88,   231,   268,   299,    88,   112,   227,   297,   229,   268,
     299,   211,   172,   216,    33,    48,   216,   120,   216,   330,
      33,    48,   216,   330,   216,    48,   216,    37,    73,   165,
     211,   211,   102,   197,   102,   125,   197,   275,    83,    86,
     194,   317,   172,   172,   197,   186,   172,   278,   111,   172,
     211,   303,   304,     1,   145,   308,    48,   146,   186,   216,
     146,   216,    13,   172,   172,   216,   317,   224,   146,   165,
     338,   338,   165,   170,   216,   172,   317,   165,   125,   298,
     165,   216,   216,   165,   171,   171,   184,   146,   171,   338,
     146,   173,   146,   173,   175,   330,    48,   146,   175,   330,
     123,   146,   175,     8,     1,   171,   198,    66,   205,   206,
     338,   200,   338,   211,   248,   145,   257,   170,   269,   165,
     338,   338,   338,   338,   235,   338,   235,   338,   338,   338,
     338,   338,   338,   338,     3,    20,    34,    63,   103,   109,
     212,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,    69,   340,
     340,   340,   340,   340,   317,   317,   235,   338,   235,   338,
      33,    48,    33,    48,   102,   172,    48,   175,   211,    27,
      48,    90,   115,   329,   332,   333,   338,   354,   235,   338,
     216,   165,   165,   338,   338,   125,   173,   146,   232,   211,
     301,   228,   230,   211,   165,   211,   301,    48,   313,    45,
     338,   235,   338,   172,   216,    45,   338,   235,   338,   216,
     235,   338,    12,    19,    55,   140,   141,   142,   143,   144,
     145,   147,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   163,   166,   167,   168,   169,   199,   274,
     275,   276,   338,   199,   201,   125,   125,   186,    35,   186,
     338,    35,   338,   193,   173,   318,   211,   239,   240,    27,
      48,    52,    76,    79,    90,   109,   185,   279,   280,   281,
     282,   283,   266,   304,   146,   173,    34,    50,    97,   101,
     174,   215,   309,   321,   125,   305,   338,   302,   218,   211,
     299,   338,   338,   173,   290,   338,     1,   253,   319,   173,
     309,   321,   146,   171,   173,   173,   315,   173,   315,   186,
     175,   235,   338,   175,   186,   338,   175,   338,   175,   338,
     171,   171,   211,   146,   165,    13,   148,   146,   165,    13,
      37,    73,   165,   172,    21,   249,   313,   170,     1,    31,
     211,   261,   262,   263,    27,    79,    90,   109,   271,   284,
     172,   165,   165,   165,   165,   165,   165,   173,   175,    45,
     235,   338,    45,   235,   338,   318,   235,   338,   172,    48,
      90,   146,   173,    17,    20,    25,    46,    53,    63,    71,
      87,    99,   110,   116,   320,    89,    89,   327,   327,   165,
     290,   340,   319,   211,   257,   165,   211,   211,   257,   257,
     165,   338,   173,   338,    33,   216,    33,   216,   331,   332,
     338,    33,   216,   330,    33,   216,   330,   216,   216,   146,
     165,    13,   165,   338,   338,    35,   186,    35,    35,   186,
     102,   197,    66,   173,   146,   173,    48,    90,   282,   146,
     173,   172,   211,    27,    79,    90,   109,   286,   173,   304,
     308,     1,   313,    69,   340,   211,   173,   173,   171,    75,
     117,   171,   254,   173,   297,   186,   175,   330,   175,   330,
     186,   123,   205,   212,   140,   141,   142,   143,   144,   159,
     163,   168,   170,   276,   338,   111,   338,   199,   201,   318,
     172,   197,   211,   250,     1,   258,   171,     8,   263,   125,
     146,   171,    90,   270,     1,     3,    17,    20,    25,    40,
      46,    53,    63,    70,    71,    87,    99,   103,   106,   110,
     116,   172,   210,   211,   213,   272,   273,   274,   275,   320,
     173,   338,    33,    33,   338,    33,    33,   173,   175,   175,
     318,   332,   308,   320,   320,   216,   216,   257,   170,   257,
     257,   170,   170,   216,   102,    45,   338,    45,   338,   146,
     173,   102,    45,   338,   216,    45,   338,   216,   276,   338,
     338,   186,   186,   338,   186,    35,   211,   165,   240,   197,
     211,   281,   304,   145,   312,    90,   308,   305,   175,    48,
     175,   172,   172,    33,   186,   313,    45,   186,   338,   175,
      45,   186,   338,   175,   338,   199,    13,    37,    73,    37,
      73,   165,   165,   173,   250,   145,   197,   171,    31,    83,
      86,   171,   185,   221,   222,   263,   338,   262,   286,   172,
     277,   338,   140,   148,   277,   277,   102,    45,    45,   102,
      45,    45,    45,    45,   173,   305,   170,   258,   170,   170,
     258,   258,   338,   338,   338,   332,   338,   338,   338,    13,
      35,   186,   165,   312,   173,   174,   215,   290,   311,   321,
     150,   291,   305,    61,   118,   292,   338,   309,   321,   317,
     317,   186,   216,   338,   186,   338,   186,   171,   111,   338,
     199,   201,   199,   201,   165,   173,   338,     8,   222,   221,
       1,   145,   307,   280,   173,     3,   103,   273,   275,   338,
     338,   338,   338,   338,   338,   258,   171,   258,   258,   171,
     171,   102,   102,   102,   102,   338,   186,   291,   305,   312,
     175,   313,   290,   338,     3,    92,   103,   293,   294,   295,
     338,   197,   217,   289,   175,   173,   173,   102,   102,   165,
     165,   165,   165,   197,   222,   174,   215,   306,   321,   105,
     287,   173,   277,   277,   102,   102,   102,   102,   102,   102,
     171,   171,   171,   338,   338,   338,   338,   291,   305,   290,
     310,   311,   321,    48,   175,   295,   118,   146,   125,   151,
     153,   154,   157,   158,    61,   321,   164,   164,   338,   338,
       1,   175,   313,   292,   338,   310,   311,   338,   294,   295,
     295,   295,   295,   295,   295,   293,   175,   306,   321,   175,
     165,   288,   289,   175,   306,   321,   310
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   188,   188,   188,   189,   189,   190,   191,
     191,   191,   192,   192,   192,   193,   193,   194,   194,   194,
     196,   195,   197,   197,   197,   198,   198,   199,   199,   199,
     199,   199,   199,   200,   200,   200,   200,   200,   200,   201,
     201,   202,   202,   202,   203,   203,   203,   203,   203,   203,
     203,   204,   205,   205,   205,   205,   206,   206,   207,   208,
     208,   208,   208,   208,   208,   209,   209,   210,   210,   210,
     210,   210,   210,   211,   211,   211,   211,   211,   211,   212,
     212,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   215,   215,   215,   215,   216,   216,   217,   217,
     218,   218,   218,   219,   219,   220,   221,   221,   221,   222,
     222,   222,   223,   222,   224,   224,   224,   224,   224,   224,
     224,   225,   225,   225,   225,   227,   226,   228,   226,   229,
     226,   230,   226,   231,   226,   232,   226,   226,   226,   226,
     226,   233,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   235,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   237,   237,   238,   238,
     239,   239,   240,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   242,   242,   242,   242,
     242,   242,   243,   243,   243,   244,   244,   244,   245,   246,
     246,   247,   247,   248,   248,   249,   249,   249,   250,   250,
     251,   252,   252,   253,   253,   254,   254,   254,   255,   255,
     256,   256,   256,   257,   257,   258,   258,   258,   259,   259,
     260,   261,   261,   261,   262,   262,   262,   263,   263,   265,
     266,   264,   267,   267,   267,   269,   270,   268,   271,   271,
     271,   271,   271,   272,   272,   273,   273,   273,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   276,   276,   277,   277,
     278,   279,   279,   280,   280,   281,   281,   281,   281,   281,
     281,   282,   282,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   284,   284,   284,   284,   284,   284,   285,   285,
     285,   286,   286,   286,   286,   286,   286,   287,   287,   288,
     288,   289,   289,   290,   291,   291,   291,   292,   292,   292,
     292,   292,   293,   293,   294,   294,   294,   294,   294,   294,
     294,   295,   295,   296,   296,   296,   297,   297,   298,   298,
     298,   299,   299,   299,   299,   299,   300,   300,   301,   301,
     302,   302,   303,   303,   303,   304,   304,   304,   305,   305,
     305,   306,   306,   306,   306,   306,   306,   306,   307,   307,
     307,   307,   307,   308,   308,   308,   308,   308,   309,   309,
     309,   309,   310,   310,   310,   311,   311,   311,   311,   311,
     312,   312,   312,   312,   312,   313,   313,   313,   313,   314,
     314,   315,   315,   315,   316,   316,   317,   317,   318,   318,
     319,   319,   319,   319,   320,   320,   321,   321,   321,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   324,   325,
     326,   326,   326,   326,   326,   326,   326,   326,   327,   327,
     328,   329,   329,   330,   331,   331,   332,   332,   332,   333,
     333,   333,   333,   333,   333,   334,   334,   335,   335,   335,
     335,   335,   336,   336,   336,   336,   336,   337,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   339,   339,   340,   340,   340,
     341,   341,   341,   341,   342,   342,   342,   342,   342,   343,
     343,   343,   344,   344,   344,   344,   344,   345,   345,   345,
     345,   346,   346,   347,   347,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   349,   349,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   351,   351,   351,   351,   351,   351,   351,
     352,   352,   352,   352,   353,   353,   353,   353,   354,   354,
     354,   354,   354,   354,   354
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     2,     1,     1,     1,
       1,     1,     1,     2,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     1,     3,     2,     1,     1,     4,     3,
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
       1,     1,     6,     4,     7,     5,     3,     6,     2,     1,
       1,     2,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     3,     6,     6,
       1,     1,     1,     0,     2,     0,     2,     3,     5,     5,
       1,     1,     2,     3,     1,     3,     2,     1,     3,     0,
       0,     8,     0,     1,     1,     0,     0,    10,     3,     3,
       5,     5,     3,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     3,
       3,     1,     3,     0,     1,     4,     5,     4,     5,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     2,     2,
       4,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     4,     4,     2,     4,     0,     2,
       2,     1,     1,     1,     2,     1,     4,     3,     1,     3,
       3,     5,     1,     1,     3,     1,     2,     3,     0,     2,
       2,     3,     2,     4,     3,     3,     4,     3,     0,     2,
       2,     2,     1,     0,     2,     2,     2,     1,     4,     4,
       6,     3,     0,     1,     1,     3,     4,     3,     4,     6,
       0,     2,     2,     2,     2,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     1,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     3,     1,     2,     2,
       2,     2,     2,     8,     8,     9,     9,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       3,     2,     2,     2,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     5,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

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
#line 6630 "bison-chapel.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 523 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6636 "bison-chapel.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 524 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6642 "bison-chapel.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 531 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6648 "bison-chapel.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 536 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6654 "bison-chapel.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 537 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6660 "bison-chapel.cpp"
    break;

  case 22: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 555 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6666 "bison-chapel.cpp"
    break;

  case 23: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 556 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6672 "bison-chapel.cpp"
    break;

  case 24: /* stmt: TLABEL ident_def stmt  */
#line 557 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6678 "bison-chapel.cpp"
    break;

  case 25: /* stmt: TYIELD expr TSEMI  */
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6684 "bison-chapel.cpp"
    break;

  case 26: /* stmt: error TSEMI  */
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6690 "bison-chapel.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 569 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6696 "bison-chapel.cpp"
    break;

  case 34: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 570 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6702 "bison-chapel.cpp"
    break;

  case 35: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 571 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6708 "bison-chapel.cpp"
    break;

  case 36: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 572 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6714 "bison-chapel.cpp"
    break;

  case 37: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 573 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6720 "bison-chapel.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL do_stmt  */
#line 574 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6726 "bison-chapel.cpp"
    break;

  case 39: /* tryable_stmt: TON expr do_stmt  */
#line 575 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6732 "bison-chapel.cpp"
    break;

  case 40: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 576 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6738 "bison-chapel.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL do_stmt  */
#line 577 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6744 "bison-chapel.cpp"
    break;

  case 42: /* tryable_stmt: TSYNC stmt  */
#line 578 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6750 "bison-chapel.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 584 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 6756 "bison-chapel.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 586 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 6762 "bison-chapel.cpp"
    break;

  case 48: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 596 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6776 "bison-chapel.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 609 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6785 "bison-chapel.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 614 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6794 "bison-chapel.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 619 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6803 "bison-chapel.cpp"
    break;

  case 52: /* access_control: %empty  */
#line 626 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6809 "bison-chapel.cpp"
    break;

  case 53: /* access_control: TPUBLIC  */
#line 627 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6815 "bison-chapel.cpp"
    break;

  case 54: /* access_control: TPRIVATE  */
#line 628 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6821 "bison-chapel.cpp"
    break;

  case 55: /* opt_prototype: %empty  */
#line 632 "chapel.ypp"
             { (yyval.b) = false; }
#line 6827 "bison-chapel.cpp"
    break;

  case 56: /* opt_prototype: TPROTOTYPE  */
#line 633 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6833 "bison-chapel.cpp"
    break;

  case 57: /* include_access_control: %empty  */
#line 637 "chapel.ypp"
           { (yyval.b) = false; }
#line 6839 "bison-chapel.cpp"
    break;

  case 58: /* include_access_control: TPUBLIC  */
#line 638 "chapel.ypp"
           { (yyval.b) = false; }
#line 6845 "bison-chapel.cpp"
    break;

  case 59: /* include_access_control: TPRIVATE  */
#line 639 "chapel.ypp"
           { (yyval.b) = true; }
#line 6851 "bison-chapel.cpp"
    break;

  case 60: /* $@1: %empty  */
#line 644 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6860 "bison-chapel.cpp"
    break;

  case 61: /* include_module_stmt: TINCLUDE $@1 include_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 649 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6868 "bison-chapel.cpp"
    break;

  case 62: /* block_stmt: TLCBR TRCBR  */
#line 664 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6874 "bison-chapel.cpp"
    break;

  case 63: /* block_stmt: TLCBR stmt_ls TRCBR  */
#line 665 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6880 "bison-chapel.cpp"
    break;

  case 64: /* block_stmt: TLCBR error TRCBR  */
#line 666 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6886 "bison-chapel.cpp"
    break;

  case 65: /* stmt_ls: toplevel_stmt  */
#line 671 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6892 "bison-chapel.cpp"
    break;

  case 66: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 672 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6898 "bison-chapel.cpp"
    break;

  case 67: /* renames_ls: expr  */
#line 677 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6908 "bison-chapel.cpp"
    break;

  case 68: /* renames_ls: all_op_name  */
#line 682 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6918 "bison-chapel.cpp"
    break;

  case 69: /* renames_ls: expr TAS expr  */
#line 687 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6928 "bison-chapel.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA expr  */
#line 692 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6937 "bison-chapel.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 696 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6946 "bison-chapel.cpp"
    break;

  case 72: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 700 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6955 "bison-chapel.cpp"
    break;

  case 73: /* use_renames_ls: expr  */
#line 709 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6965 "bison-chapel.cpp"
    break;

  case 74: /* use_renames_ls: expr TAS expr  */
#line 714 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6975 "bison-chapel.cpp"
    break;

  case 75: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 719 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6985 "bison-chapel.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 724 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6994 "bison-chapel.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 728 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7003 "bison-chapel.cpp"
    break;

  case 78: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 732 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7012 "bison-chapel.cpp"
    break;

  case 79: /* opt_only_ls: %empty  */
#line 740 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7022 "bison-chapel.cpp"
    break;

  case 81: /* use_access_control: %empty  */
#line 749 "chapel.ypp"
           { (yyval.b) = true; }
#line 7028 "bison-chapel.cpp"
    break;

  case 82: /* use_access_control: TPUBLIC  */
#line 750 "chapel.ypp"
           { (yyval.b) = false; }
#line 7034 "bison-chapel.cpp"
    break;

  case 83: /* use_access_control: TPRIVATE  */
#line 751 "chapel.ypp"
           { (yyval.b) = true; }
#line 7040 "bison-chapel.cpp"
    break;

  case 84: /* use_stmt: use_access_control TUSE use_renames_ls TSEMI  */
#line 755 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7046 "bison-chapel.cpp"
    break;

  case 85: /* use_stmt: use_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 756 "chapel.ypp"
                                                           { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7052 "bison-chapel.cpp"
    break;

  case 86: /* use_stmt: use_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 757 "chapel.ypp"
                                                                    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7058 "bison-chapel.cpp"
    break;

  case 87: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 758 "chapel.ypp"
                                                                           { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7064 "bison-chapel.cpp"
    break;

  case 88: /* use_stmt: use_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 759 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7070 "bison-chapel.cpp"
    break;

  case 89: /* use_stmt: use_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 760 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7076 "bison-chapel.cpp"
    break;

  case 90: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 761 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7082 "bison-chapel.cpp"
    break;

  case 91: /* import_stmt: use_access_control TIMPORT import_ls TSEMI  */
#line 765 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7089 "bison-chapel.cpp"
    break;

  case 92: /* import_expr: expr  */
#line 770 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7095 "bison-chapel.cpp"
    break;

  case 93: /* import_expr: expr TDOT all_op_name  */
#line 771 "chapel.ypp"
                        { std::vector<PotentialRename*>* renames = new std::vector<PotentialRename*>();
                          PotentialRename* nameInMod = new PotentialRename();
                          nameInMod->tag = PotentialRename::SINGLE;
                          nameInMod->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                          renames->push_back(nameInMod);
                          (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), renames); }
#line 7106 "bison-chapel.cpp"
    break;

  case 94: /* import_expr: expr TAS ident_use  */
#line 777 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7112 "bison-chapel.cpp"
    break;

  case 95: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 778 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7118 "bison-chapel.cpp"
    break;

  case 96: /* import_ls: import_expr  */
#line 782 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7124 "bison-chapel.cpp"
    break;

  case 97: /* import_ls: import_ls TCOMMA import_expr  */
#line 783 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7130 "bison-chapel.cpp"
    break;

  case 98: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 787 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7136 "bison-chapel.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 792 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7142 "bison-chapel.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 794 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7148 "bison-chapel.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 796 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7154 "bison-chapel.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 798 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7160 "bison-chapel.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 800 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7166 "bison-chapel.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 802 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7172 "bison-chapel.cpp"
    break;

  case 105: /* opt_label_ident: %empty  */
#line 808 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7178 "bison-chapel.cpp"
    break;

  case 106: /* opt_label_ident: TIDENT  */
#line 809 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7184 "bison-chapel.cpp"
    break;

  case 107: /* ident_fn_def: TIDENT  */
#line 813 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7190 "bison-chapel.cpp"
    break;

  case 108: /* ident_fn_def: TNONE  */
#line 814 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7196 "bison-chapel.cpp"
    break;

  case 109: /* ident_fn_def: TTHIS  */
#line 815 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7202 "bison-chapel.cpp"
    break;

  case 110: /* ident_fn_def: TFALSE  */
#line 816 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7208 "bison-chapel.cpp"
    break;

  case 111: /* ident_fn_def: TTRUE  */
#line 817 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7214 "bison-chapel.cpp"
    break;

  case 112: /* ident_fn_def: internal_type_ident_def  */
#line 818 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7220 "bison-chapel.cpp"
    break;

  case 113: /* ident_def: TIDENT  */
#line 821 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7226 "bison-chapel.cpp"
    break;

  case 114: /* ident_def: TNONE  */
#line 822 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7232 "bison-chapel.cpp"
    break;

  case 115: /* ident_def: TTHIS  */
#line 823 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7238 "bison-chapel.cpp"
    break;

  case 116: /* ident_def: TFALSE  */
#line 824 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7244 "bison-chapel.cpp"
    break;

  case 117: /* ident_def: TTRUE  */
#line 825 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7250 "bison-chapel.cpp"
    break;

  case 118: /* ident_def: internal_type_ident_def  */
#line 826 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7256 "bison-chapel.cpp"
    break;

  case 119: /* ident_use: TIDENT  */
#line 838 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7262 "bison-chapel.cpp"
    break;

  case 120: /* ident_use: TTHIS  */
#line 839 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7268 "bison-chapel.cpp"
    break;

  case 121: /* internal_type_ident_def: TBOOL  */
#line 850 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7274 "bison-chapel.cpp"
    break;

  case 122: /* internal_type_ident_def: TINT  */
#line 851 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7280 "bison-chapel.cpp"
    break;

  case 123: /* internal_type_ident_def: TUINT  */
#line 852 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7286 "bison-chapel.cpp"
    break;

  case 124: /* internal_type_ident_def: TREAL  */
#line 853 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7292 "bison-chapel.cpp"
    break;

  case 125: /* internal_type_ident_def: TIMAG  */
#line 854 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7298 "bison-chapel.cpp"
    break;

  case 126: /* internal_type_ident_def: TCOMPLEX  */
#line 855 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7304 "bison-chapel.cpp"
    break;

  case 127: /* internal_type_ident_def: TBYTES  */
#line 856 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7310 "bison-chapel.cpp"
    break;

  case 128: /* internal_type_ident_def: TSTRING  */
#line 857 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7316 "bison-chapel.cpp"
    break;

  case 129: /* internal_type_ident_def: TSYNC  */
#line 858 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7322 "bison-chapel.cpp"
    break;

  case 130: /* internal_type_ident_def: TSINGLE  */
#line 859 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7328 "bison-chapel.cpp"
    break;

  case 131: /* internal_type_ident_def: TOWNED  */
#line 860 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7334 "bison-chapel.cpp"
    break;

  case 132: /* internal_type_ident_def: TSHARED  */
#line 861 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7340 "bison-chapel.cpp"
    break;

  case 133: /* internal_type_ident_def: TBORROWED  */
#line 862 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7346 "bison-chapel.cpp"
    break;

  case 134: /* internal_type_ident_def: TUNMANAGED  */
#line 863 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7352 "bison-chapel.cpp"
    break;

  case 135: /* internal_type_ident_def: TDOMAIN  */
#line 864 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7358 "bison-chapel.cpp"
    break;

  case 136: /* internal_type_ident_def: TINDEX  */
#line 865 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7364 "bison-chapel.cpp"
    break;

  case 137: /* internal_type_ident_def: TLOCALE  */
#line 866 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7370 "bison-chapel.cpp"
    break;

  case 138: /* internal_type_ident_def: TNOTHING  */
#line 867 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7376 "bison-chapel.cpp"
    break;

  case 139: /* internal_type_ident_def: TVOID  */
#line 868 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7382 "bison-chapel.cpp"
    break;

  case 140: /* scalar_type: TBOOL  */
#line 872 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7388 "bison-chapel.cpp"
    break;

  case 141: /* scalar_type: TENUM  */
#line 873 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7394 "bison-chapel.cpp"
    break;

  case 142: /* scalar_type: TINT  */
#line 874 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7400 "bison-chapel.cpp"
    break;

  case 143: /* scalar_type: TUINT  */
#line 875 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7406 "bison-chapel.cpp"
    break;

  case 144: /* scalar_type: TREAL  */
#line 876 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7412 "bison-chapel.cpp"
    break;

  case 145: /* scalar_type: TIMAG  */
#line 877 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7418 "bison-chapel.cpp"
    break;

  case 146: /* scalar_type: TCOMPLEX  */
#line 878 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7424 "bison-chapel.cpp"
    break;

  case 147: /* scalar_type: TBYTES  */
#line 879 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7430 "bison-chapel.cpp"
    break;

  case 148: /* scalar_type: TSTRING  */
#line 880 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7436 "bison-chapel.cpp"
    break;

  case 149: /* scalar_type: TLOCALE  */
#line 881 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7442 "bison-chapel.cpp"
    break;

  case 150: /* scalar_type: TNOTHING  */
#line 882 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7448 "bison-chapel.cpp"
    break;

  case 151: /* scalar_type: TVOID  */
#line 883 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7454 "bison-chapel.cpp"
    break;

  case 152: /* reserved_type_ident_use: TSYNC  */
#line 890 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7460 "bison-chapel.cpp"
    break;

  case 153: /* reserved_type_ident_use: TSINGLE  */
#line 891 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7466 "bison-chapel.cpp"
    break;

  case 154: /* reserved_type_ident_use: TDOMAIN  */
#line 892 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7472 "bison-chapel.cpp"
    break;

  case 155: /* reserved_type_ident_use: TINDEX  */
#line 893 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7478 "bison-chapel.cpp"
    break;

  case 156: /* do_stmt: TDO stmt  */
#line 897 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7484 "bison-chapel.cpp"
    break;

  case 157: /* do_stmt: block_stmt  */
#line 898 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7490 "bison-chapel.cpp"
    break;

  case 158: /* return_stmt: TRETURN TSEMI  */
#line 902 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7496 "bison-chapel.cpp"
    break;

  case 159: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 903 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7502 "bison-chapel.cpp"
    break;

  case 160: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 907 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-3].pexpr), (yyvsp[-1].pflagset), (yyvsp[0].pch)); }
#line 7508 "bison-chapel.cpp"
    break;

  case 161: /* manager_expr: expr TAS ident_def  */
#line 908 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-2].pexpr), NULL, (yyvsp[0].pch)); }
#line 7514 "bison-chapel.cpp"
    break;

  case 162: /* manager_expr: expr  */
#line 909 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[0].pexpr), NULL, NULL); }
#line 7520 "bison-chapel.cpp"
    break;

  case 163: /* manager_expr_ls: manager_expr  */
#line 913 "chapel.ypp"
                                        { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7526 "bison-chapel.cpp"
    break;

  case 164: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 914 "chapel.ypp"
                                        { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7532 "bison-chapel.cpp"
    break;

  case 165: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 918 "chapel.ypp"
                                  { (yyval.pblockstmt) = buildManageStmt((yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7538 "bison-chapel.cpp"
    break;

  case 167: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 924 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 7544 "bison-chapel.cpp"
    break;

  case 168: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 926 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 7550 "bison-chapel.cpp"
    break;

  case 169: /* class_level_stmt: TSEMI  */
#line 930 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7556 "bison-chapel.cpp"
    break;

  case 171: /* class_level_stmt: TPUBLIC private_decl  */
#line 932 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7562 "bison-chapel.cpp"
    break;

  case 172: /* @2: %empty  */
#line 942 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7568 "bison-chapel.cpp"
    break;

  case 173: /* class_level_stmt: TPRIVATE @2 private_decl  */
#line 943 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7574 "bison-chapel.cpp"
    break;

  case 181: /* forwarding_stmt: TFORWARDING expr TSEMI  */
#line 957 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7580 "bison-chapel.cpp"
    break;

  case 182: /* forwarding_stmt: TFORWARDING expr TEXCEPT renames_ls TSEMI  */
#line 958 "chapel.ypp"
                                             { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7586 "bison-chapel.cpp"
    break;

  case 183: /* forwarding_stmt: TFORWARDING expr TONLY opt_only_ls TSEMI  */
#line 959 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7592 "bison-chapel.cpp"
    break;

  case 184: /* forwarding_stmt: TFORWARDING var_decl_stmt  */
#line 960 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7598 "bison-chapel.cpp"
    break;

  case 185: /* $@3: %empty  */
#line 965 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7607 "bison-chapel.cpp"
    break;

  case 186: /* extern_export_decl_stmt: TEXTERN TRECORD $@3 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 970 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7621 "bison-chapel.cpp"
    break;

  case 187: /* $@4: %empty  */
#line 981 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7630 "bison-chapel.cpp"
    break;

  case 188: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TRECORD $@4 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 986 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7645 "bison-chapel.cpp"
    break;

  case 189: /* $@5: %empty  */
#line 998 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7654 "bison-chapel.cpp"
    break;

  case 190: /* extern_export_decl_stmt: TEXTERN TUNION $@5 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1003 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7668 "bison-chapel.cpp"
    break;

  case 191: /* $@6: %empty  */
#line 1014 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7677 "bison-chapel.cpp"
    break;

  case 192: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TUNION $@6 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1019 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7692 "bison-chapel.cpp"
    break;

  case 193: /* $@7: %empty  */
#line 1031 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7701 "bison-chapel.cpp"
    break;

  case 194: /* extern_export_decl_stmt: TEXPORT TRECORD $@7 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1036 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7715 "bison-chapel.cpp"
    break;

  case 195: /* $@8: %empty  */
#line 1046 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7724 "bison-chapel.cpp"
    break;

  case 196: /* extern_export_decl_stmt: TEXPORT STRINGLITERAL TRECORD $@8 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1051 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7738 "bison-chapel.cpp"
    break;

  case 197: /* extern_export_decl_stmt: TEXTERN opt_expr fn_decl_stmt  */
#line 1062 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7746 "bison-chapel.cpp"
    break;

  case 198: /* extern_export_decl_stmt: TEXPORT opt_expr fn_decl_stmt  */
#line 1066 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7754 "bison-chapel.cpp"
    break;

  case 199: /* extern_export_decl_stmt: TEXTERN opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1071 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7766 "bison-chapel.cpp"
    break;

  case 200: /* extern_export_decl_stmt: TEXPORT opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1079 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7778 "bison-chapel.cpp"
    break;

  case 201: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1090 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7786 "bison-chapel.cpp"
    break;

  case 202: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1096 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7792 "bison-chapel.cpp"
    break;

  case 203: /* loop_stmt: TWHILE expr do_stmt  */
#line 1097 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7798 "bison-chapel.cpp"
    break;

  case 204: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1098 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7804 "bison-chapel.cpp"
    break;

  case 205: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1099 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7810 "bison-chapel.cpp"
    break;

  case 206: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1100 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7816 "bison-chapel.cpp"
    break;

  case 207: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1101 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7822 "bison-chapel.cpp"
    break;

  case 208: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1102 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7828 "bison-chapel.cpp"
    break;

  case 209: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1103 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7834 "bison-chapel.cpp"
    break;

  case 210: /* loop_stmt: TFOR expr do_stmt  */
#line 1104 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7840 "bison-chapel.cpp"
    break;

  case 211: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1105 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7846 "bison-chapel.cpp"
    break;

  case 212: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1106 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7852 "bison-chapel.cpp"
    break;

  case 213: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1107 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7858 "bison-chapel.cpp"
    break;

  case 214: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1108 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7864 "bison-chapel.cpp"
    break;

  case 215: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1109 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7870 "bison-chapel.cpp"
    break;

  case 216: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1110 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7876 "bison-chapel.cpp"
    break;

  case 217: /* loop_stmt: TFORALL expr do_stmt  */
#line 1111 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7882 "bison-chapel.cpp"
    break;

  case 218: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1112 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7888 "bison-chapel.cpp"
    break;

  case 219: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1113 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7894 "bison-chapel.cpp"
    break;

  case 220: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1114 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7900 "bison-chapel.cpp"
    break;

  case 221: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1115 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7906 "bison-chapel.cpp"
    break;

  case 222: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1116 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7912 "bison-chapel.cpp"
    break;

  case 223: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1117 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7918 "bison-chapel.cpp"
    break;

  case 224: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1118 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7924 "bison-chapel.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1120 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7934 "bison-chapel.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1126 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7944 "bison-chapel.cpp"
    break;

  case 227: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1132 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7954 "bison-chapel.cpp"
    break;

  case 228: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1138 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7964 "bison-chapel.cpp"
    break;

  case 229: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1144 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7975 "bison-chapel.cpp"
    break;

  case 230: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1151 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7986 "bison-chapel.cpp"
    break;

  case 231: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1158 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7994 "bison-chapel.cpp"
    break;

  case 232: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1162 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8002 "bison-chapel.cpp"
    break;

  case 233: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1168 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 8008 "bison-chapel.cpp"
    break;

  case 234: /* if_stmt: TIF expr TTHEN stmt  */
#line 1172 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8014 "bison-chapel.cpp"
    break;

  case 235: /* if_stmt: TIF expr block_stmt  */
#line 1173 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8020 "bison-chapel.cpp"
    break;

  case 236: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1174 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8026 "bison-chapel.cpp"
    break;

  case 237: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1175 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8032 "bison-chapel.cpp"
    break;

  case 238: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1177 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8038 "bison-chapel.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar block_stmt  */
#line 1178 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8044 "bison-chapel.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1179 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8050 "bison-chapel.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1180 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8056 "bison-chapel.cpp"
    break;

  case 242: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1182 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8063 "bison-chapel.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1184 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8070 "bison-chapel.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1186 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8077 "bison-chapel.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1188 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8084 "bison-chapel.cpp"
    break;

  case 246: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1193 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 8090 "bison-chapel.cpp"
    break;

  case 247: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1194 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 8096 "bison-chapel.cpp"
    break;

  case 248: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1199 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 8102 "bison-chapel.cpp"
    break;

  case 249: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1201 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8111 "bison-chapel.cpp"
    break;

  case 250: /* ifc_formal_ls: ifc_formal  */
#line 1208 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8117 "bison-chapel.cpp"
    break;

  case 251: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1209 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8123 "bison-chapel.cpp"
    break;

  case 252: /* ifc_formal: ident_def  */
#line 1214 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8129 "bison-chapel.cpp"
    break;

  case 253: /* implements_type_ident: TIDENT  */
#line 1219 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 8135 "bison-chapel.cpp"
    break;

  case 254: /* implements_type_ident: TBOOL  */
#line 1220 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 8141 "bison-chapel.cpp"
    break;

  case 255: /* implements_type_ident: TINT  */
#line 1221 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 8147 "bison-chapel.cpp"
    break;

  case 256: /* implements_type_ident: TUINT  */
#line 1222 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 8153 "bison-chapel.cpp"
    break;

  case 257: /* implements_type_ident: TREAL  */
#line 1223 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 8159 "bison-chapel.cpp"
    break;

  case 258: /* implements_type_ident: TIMAG  */
#line 1224 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 8165 "bison-chapel.cpp"
    break;

  case 259: /* implements_type_ident: TCOMPLEX  */
#line 1225 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 8171 "bison-chapel.cpp"
    break;

  case 260: /* implements_type_ident: TBYTES  */
#line 1226 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 8177 "bison-chapel.cpp"
    break;

  case 261: /* implements_type_ident: TSTRING  */
#line 1227 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 8183 "bison-chapel.cpp"
    break;

  case 262: /* implements_type_ident: TLOCALE  */
#line 1228 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 8189 "bison-chapel.cpp"
    break;

  case 263: /* implements_type_ident: TNOTHING  */
#line 1229 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 8195 "bison-chapel.cpp"
    break;

  case 264: /* implements_type_ident: TVOID  */
#line 1230 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 8201 "bison-chapel.cpp"
    break;

  case 265: /* implements_type_ident: implements_type_error_ident  */
#line 1232 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8208 "bison-chapel.cpp"
    break;

  case 266: /* implements_type_error_ident: TNONE  */
#line 1238 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 8214 "bison-chapel.cpp"
    break;

  case 267: /* implements_type_error_ident: TTHIS  */
#line 1239 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 8220 "bison-chapel.cpp"
    break;

  case 268: /* implements_type_error_ident: TFALSE  */
#line 1240 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 8226 "bison-chapel.cpp"
    break;

  case 269: /* implements_type_error_ident: TTRUE  */
#line 1241 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 8232 "bison-chapel.cpp"
    break;

  case 270: /* implements_type_error_ident: TDOMAIN  */
#line 1250 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 8238 "bison-chapel.cpp"
    break;

  case 271: /* implements_type_error_ident: TINDEX  */
#line 1251 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 8244 "bison-chapel.cpp"
    break;

  case 272: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1256 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8250 "bison-chapel.cpp"
    break;

  case 273: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1258 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8257 "bison-chapel.cpp"
    break;

  case 274: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1261 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8264 "bison-chapel.cpp"
    break;

  case 275: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1267 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8270 "bison-chapel.cpp"
    break;

  case 276: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1269 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8277 "bison-chapel.cpp"
    break;

  case 277: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1272 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8284 "bison-chapel.cpp"
    break;

  case 278: /* defer_stmt: TDEFER stmt  */
#line 1277 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8290 "bison-chapel.cpp"
    break;

  case 279: /* try_token: TTRY  */
#line 1280 "chapel.ypp"
           { (yyval.b) = false; }
#line 8296 "bison-chapel.cpp"
    break;

  case 280: /* try_token: TTRYBANG  */
#line 1281 "chapel.ypp"
           { (yyval.b) = true; }
#line 8302 "bison-chapel.cpp"
    break;

  case 281: /* try_stmt: try_token tryable_stmt  */
#line 1284 "chapel.ypp"
                         { (yyval.pblockstmt) = TryStmt::build((yyvsp[-1].b), (yyvsp[0].pblockstmt)); }
#line 8308 "bison-chapel.cpp"
    break;

  case 282: /* try_stmt: try_token block_stmt catch_stmt_ls  */
#line 1285 "chapel.ypp"
                                       { (yyval.pblockstmt) = TryStmt::build((yyvsp[-2].b), (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8314 "bison-chapel.cpp"
    break;

  case 283: /* catch_stmt_ls: %empty  */
#line 1289 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8320 "bison-chapel.cpp"
    break;

  case 284: /* catch_stmt_ls: catch_stmt_ls catch_stmt  */
#line 1290 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8326 "bison-chapel.cpp"
    break;

  case 285: /* catch_stmt: TCATCH block_stmt  */
#line 1294 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8332 "bison-chapel.cpp"
    break;

  case 286: /* catch_stmt: TCATCH catch_expr block_stmt  */
#line 1295 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8338 "bison-chapel.cpp"
    break;

  case 287: /* catch_stmt: TCATCH TLP catch_expr TRP block_stmt  */
#line 1296 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8344 "bison-chapel.cpp"
    break;

  case 288: /* catch_expr: ident_def  */
#line 1300 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8350 "bison-chapel.cpp"
    break;

  case 289: /* catch_expr: ident_def TCOLON expr  */
#line 1301 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8356 "bison-chapel.cpp"
    break;

  case 290: /* throw_stmt: TTHROW expr TSEMI  */
#line 1305 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8362 "bison-chapel.cpp"
    break;

  case 291: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1309 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8368 "bison-chapel.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1311 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8374 "bison-chapel.cpp"
    break;

  case 293: /* when_stmt_ls: %empty  */
#line 1315 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8380 "bison-chapel.cpp"
    break;

  case 294: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1316 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8386 "bison-chapel.cpp"
    break;

  case 295: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1321 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8392 "bison-chapel.cpp"
    break;

  case 296: /* when_stmt: TOTHERWISE stmt  */
#line 1323 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8398 "bison-chapel.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1325 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8404 "bison-chapel.cpp"
    break;

  case 298: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1332 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8418 "bison-chapel.cpp"
    break;

  case 299: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR error TRCBR  */
#line 1342 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8432 "bison-chapel.cpp"
    break;

  case 300: /* class_tag: TCLASS  */
#line 1355 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8442 "bison-chapel.cpp"
    break;

  case 301: /* class_tag: TRECORD  */
#line 1360 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8452 "bison-chapel.cpp"
    break;

  case 302: /* class_tag: TUNION  */
#line 1365 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8462 "bison-chapel.cpp"
    break;

  case 303: /* opt_inherit: %empty  */
#line 1373 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8468 "bison-chapel.cpp"
    break;

  case 304: /* opt_inherit: TCOLON expr_ls  */
#line 1374 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8474 "bison-chapel.cpp"
    break;

  case 305: /* class_level_stmt_ls: %empty  */
#line 1379 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8480 "bison-chapel.cpp"
    break;

  case 306: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1381 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8486 "bison-chapel.cpp"
    break;

  case 307: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1383 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8492 "bison-chapel.cpp"
    break;

  case 308: /* enum_decl_stmt: enum_header ident_def TLCBR enum_ls TRCBR  */
#line 1388 "chapel.ypp"
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
#line 8512 "bison-chapel.cpp"
    break;

  case 309: /* enum_decl_stmt: enum_header ident_def TLCBR error TRCBR  */
#line 1404 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8520 "bison-chapel.cpp"
    break;

  case 310: /* enum_header: TENUM  */
#line 1411 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8530 "bison-chapel.cpp"
    break;

  case 311: /* enum_ls: deprecated_enum_item  */
#line 1420 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8542 "bison-chapel.cpp"
    break;

  case 312: /* enum_ls: enum_ls TCOMMA  */
#line 1428 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8550 "bison-chapel.cpp"
    break;

  case 313: /* enum_ls: enum_ls TCOMMA deprecated_enum_item  */
#line 1432 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8558 "bison-chapel.cpp"
    break;

  case 315: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1440 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr), (yyvsp[-1].pch)); }
#line 8564 "bison-chapel.cpp"
    break;

  case 316: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1442 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr)); }
#line 8570 "bison-chapel.cpp"
    break;

  case 317: /* enum_item: ident_def  */
#line 1446 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8576 "bison-chapel.cpp"
    break;

  case 318: /* enum_item: ident_def TASSIGN expr  */
#line 1447 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8582 "bison-chapel.cpp"
    break;

  case 319: /* $@9: %empty  */
#line 1452 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8591 "bison-chapel.cpp"
    break;

  case 320: /* $@10: %empty  */
#line 1457 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8600 "bison-chapel.cpp"
    break;

  case 321: /* lambda_decl_expr: TLAMBDA $@9 req_formal_ls $@10 opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1462 "chapel.ypp"
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
#line 8622 "bison-chapel.cpp"
    break;

  case 322: /* linkage_spec: %empty  */
#line 1484 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8630 "bison-chapel.cpp"
    break;

  case 323: /* linkage_spec: TINLINE  */
#line 1487 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8639 "bison-chapel.cpp"
    break;

  case 324: /* linkage_spec: TOVERRIDE  */
#line 1491 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8648 "bison-chapel.cpp"
    break;

  case 325: /* $@11: %empty  */
#line 1499 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8662 "bison-chapel.cpp"
    break;

  case 326: /* $@12: %empty  */
#line 1509 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8673 "bison-chapel.cpp"
    break;

  case 327: /* fn_decl_stmt: linkage_spec proc_iter_or_op $@11 fn_decl_stmt_inner $@12 opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where opt_function_body_stmt  */
#line 1516 "chapel.ypp"
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
#line 8707 "bison-chapel.cpp"
    break;

  case 328: /* fn_decl_stmt_inner: opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1549 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8715 "bison-chapel.cpp"
    break;

  case 329: /* fn_decl_stmt_inner: opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1553 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8724 "bison-chapel.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1558 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8732 "bison-chapel.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1562 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8741 "bison-chapel.cpp"
    break;

  case 332: /* fn_decl_stmt_inner: opt_this_intent_tag error opt_formal_ls  */
#line 1567 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8749 "bison-chapel.cpp"
    break;

  case 334: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1574 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8755 "bison-chapel.cpp"
    break;

  case 335: /* fn_ident: ident_fn_def  */
#line 1578 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8761 "bison-chapel.cpp"
    break;

  case 336: /* fn_ident: ident_def TBANG  */
#line 1579 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8767 "bison-chapel.cpp"
    break;

  case 337: /* fn_ident: op_ident  */
#line 1580 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8773 "bison-chapel.cpp"
    break;

  case 338: /* op_ident: TBAND  */
#line 1584 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8779 "bison-chapel.cpp"
    break;

  case 339: /* op_ident: TBOR  */
#line 1585 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8785 "bison-chapel.cpp"
    break;

  case 340: /* op_ident: TBXOR  */
#line 1586 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8791 "bison-chapel.cpp"
    break;

  case 341: /* op_ident: TBNOT  */
#line 1587 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8797 "bison-chapel.cpp"
    break;

  case 342: /* op_ident: TEQUAL  */
#line 1588 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8803 "bison-chapel.cpp"
    break;

  case 343: /* op_ident: TNOTEQUAL  */
#line 1589 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8809 "bison-chapel.cpp"
    break;

  case 344: /* op_ident: TLESSEQUAL  */
#line 1590 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8815 "bison-chapel.cpp"
    break;

  case 345: /* op_ident: TGREATEREQUAL  */
#line 1591 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8821 "bison-chapel.cpp"
    break;

  case 346: /* op_ident: TLESS  */
#line 1592 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8827 "bison-chapel.cpp"
    break;

  case 347: /* op_ident: TGREATER  */
#line 1593 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8833 "bison-chapel.cpp"
    break;

  case 348: /* op_ident: TPLUS  */
#line 1594 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8839 "bison-chapel.cpp"
    break;

  case 349: /* op_ident: TMINUS  */
#line 1595 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8845 "bison-chapel.cpp"
    break;

  case 350: /* op_ident: TSTAR  */
#line 1596 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8851 "bison-chapel.cpp"
    break;

  case 351: /* op_ident: TDIVIDE  */
#line 1597 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8857 "bison-chapel.cpp"
    break;

  case 352: /* op_ident: TSHIFTLEFT  */
#line 1598 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8863 "bison-chapel.cpp"
    break;

  case 353: /* op_ident: TSHIFTRIGHT  */
#line 1599 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8869 "bison-chapel.cpp"
    break;

  case 354: /* op_ident: TMOD  */
#line 1600 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8875 "bison-chapel.cpp"
    break;

  case 355: /* op_ident: TEXP  */
#line 1601 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8881 "bison-chapel.cpp"
    break;

  case 356: /* op_ident: TBANG  */
#line 1602 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8887 "bison-chapel.cpp"
    break;

  case 357: /* op_ident: TBY  */
#line 1603 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8893 "bison-chapel.cpp"
    break;

  case 358: /* op_ident: THASH  */
#line 1604 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8899 "bison-chapel.cpp"
    break;

  case 359: /* op_ident: TALIGN  */
#line 1605 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8905 "bison-chapel.cpp"
    break;

  case 360: /* op_ident: TSWAP  */
#line 1606 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8911 "bison-chapel.cpp"
    break;

  case 361: /* op_ident: TIO  */
#line 1607 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8917 "bison-chapel.cpp"
    break;

  case 362: /* op_ident: TINITEQUALS  */
#line 1608 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8923 "bison-chapel.cpp"
    break;

  case 363: /* op_ident: TCOLON  */
#line 1609 "chapel.ypp"
                 { (yyval.pch) = ":"; }
#line 8929 "bison-chapel.cpp"
    break;

  case 364: /* assignop_ident: TASSIGN  */
#line 1613 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8935 "bison-chapel.cpp"
    break;

  case 365: /* assignop_ident: TASSIGNPLUS  */
#line 1614 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8941 "bison-chapel.cpp"
    break;

  case 366: /* assignop_ident: TASSIGNMINUS  */
#line 1615 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8947 "bison-chapel.cpp"
    break;

  case 367: /* assignop_ident: TASSIGNMULTIPLY  */
#line 1616 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8953 "bison-chapel.cpp"
    break;

  case 368: /* assignop_ident: TASSIGNDIVIDE  */
#line 1617 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8959 "bison-chapel.cpp"
    break;

  case 369: /* assignop_ident: TASSIGNMOD  */
#line 1618 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8965 "bison-chapel.cpp"
    break;

  case 370: /* assignop_ident: TASSIGNEXP  */
#line 1619 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8971 "bison-chapel.cpp"
    break;

  case 371: /* assignop_ident: TASSIGNBAND  */
#line 1620 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8977 "bison-chapel.cpp"
    break;

  case 372: /* assignop_ident: TASSIGNBOR  */
#line 1621 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8983 "bison-chapel.cpp"
    break;

  case 373: /* assignop_ident: TASSIGNBXOR  */
#line 1622 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8989 "bison-chapel.cpp"
    break;

  case 374: /* assignop_ident: TASSIGNSR  */
#line 1623 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8995 "bison-chapel.cpp"
    break;

  case 375: /* assignop_ident: TASSIGNSL  */
#line 1624 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 9001 "bison-chapel.cpp"
    break;

  case 376: /* all_op_name: op_ident  */
#line 1628 "chapel.ypp"
                 { (yyval.pch) = (yyvsp[0].pch); }
#line 9007 "bison-chapel.cpp"
    break;

  case 377: /* all_op_name: assignop_ident  */
#line 1629 "chapel.ypp"
                 { (yyval.pch) = (yyvsp[0].pch); }
#line 9013 "bison-chapel.cpp"
    break;

  case 378: /* opt_formal_ls: %empty  */
#line 1633 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 9019 "bison-chapel.cpp"
    break;

  case 379: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1634 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9025 "bison-chapel.cpp"
    break;

  case 380: /* req_formal_ls: TLP formal_ls TRP  */
#line 1638 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9031 "bison-chapel.cpp"
    break;

  case 381: /* formal_ls_inner: formal  */
#line 1642 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 9037 "bison-chapel.cpp"
    break;

  case 382: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 1643 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 9043 "bison-chapel.cpp"
    break;

  case 383: /* formal_ls: %empty  */
#line 1647 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 9049 "bison-chapel.cpp"
    break;

  case 384: /* formal_ls: formal_ls_inner  */
#line 1648 "chapel.ypp"
                           { (yyval.pfnsymbol) = (yyvsp[0].pfnsymbol); }
#line 9055 "bison-chapel.cpp"
    break;

  case 385: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1653 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 9061 "bison-chapel.cpp"
    break;

  case 386: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1655 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 9067 "bison-chapel.cpp"
    break;

  case 387: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1657 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 9073 "bison-chapel.cpp"
    break;

  case 388: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1659 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 9079 "bison-chapel.cpp"
    break;

  case 389: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1661 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9085 "bison-chapel.cpp"
    break;

  case 390: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1663 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 9091 "bison-chapel.cpp"
    break;

  case 391: /* opt_intent_tag: %empty  */
#line 1667 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 9097 "bison-chapel.cpp"
    break;

  case 392: /* opt_intent_tag: required_intent_tag  */
#line 1668 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 9103 "bison-chapel.cpp"
    break;

  case 393: /* required_intent_tag: TIN  */
#line 1672 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 9109 "bison-chapel.cpp"
    break;

  case 394: /* required_intent_tag: TINOUT  */
#line 1673 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 9115 "bison-chapel.cpp"
    break;

  case 395: /* required_intent_tag: TOUT  */
#line 1674 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 9121 "bison-chapel.cpp"
    break;

  case 396: /* required_intent_tag: TCONST  */
#line 1675 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 9127 "bison-chapel.cpp"
    break;

  case 397: /* required_intent_tag: TCONST TIN  */
#line 1676 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 9133 "bison-chapel.cpp"
    break;

  case 398: /* required_intent_tag: TCONST TREF  */
#line 1677 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 9139 "bison-chapel.cpp"
    break;

  case 399: /* required_intent_tag: TPARAM  */
#line 1678 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 9145 "bison-chapel.cpp"
    break;

  case 400: /* required_intent_tag: TREF  */
#line 1679 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 9151 "bison-chapel.cpp"
    break;

  case 401: /* required_intent_tag: TTYPE  */
#line 1680 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 9157 "bison-chapel.cpp"
    break;

  case 402: /* opt_this_intent_tag: %empty  */
#line 1684 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 9163 "bison-chapel.cpp"
    break;

  case 403: /* opt_this_intent_tag: TPARAM  */
#line 1685 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 9169 "bison-chapel.cpp"
    break;

  case 404: /* opt_this_intent_tag: TREF  */
#line 1686 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 9175 "bison-chapel.cpp"
    break;

  case 405: /* opt_this_intent_tag: TCONST TREF  */
#line 1687 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 9181 "bison-chapel.cpp"
    break;

  case 406: /* opt_this_intent_tag: TCONST  */
#line 1688 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 9187 "bison-chapel.cpp"
    break;

  case 407: /* opt_this_intent_tag: TTYPE  */
#line 1689 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 9193 "bison-chapel.cpp"
    break;

  case 408: /* proc_iter_or_op: TPROC  */
#line 1693 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 9199 "bison-chapel.cpp"
    break;

  case 409: /* proc_iter_or_op: TITER  */
#line 1694 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 9205 "bison-chapel.cpp"
    break;

  case 410: /* proc_iter_or_op: TOPERATOR  */
#line 1695 "chapel.ypp"
            { (yyval.procIterOp) = ProcIterOp_OP; }
#line 9211 "bison-chapel.cpp"
    break;

  case 411: /* opt_ret_tag: %empty  */
#line 1699 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9217 "bison-chapel.cpp"
    break;

  case 412: /* opt_ret_tag: TCONST  */
#line 1700 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9223 "bison-chapel.cpp"
    break;

  case 413: /* opt_ret_tag: TCONST TREF  */
#line 1701 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 9229 "bison-chapel.cpp"
    break;

  case 414: /* opt_ret_tag: TREF  */
#line 1702 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 9235 "bison-chapel.cpp"
    break;

  case 415: /* opt_ret_tag: TPARAM  */
#line 1703 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 9241 "bison-chapel.cpp"
    break;

  case 416: /* opt_ret_tag: TTYPE  */
#line 1704 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 9247 "bison-chapel.cpp"
    break;

  case 417: /* opt_throws_error: %empty  */
#line 1708 "chapel.ypp"
          { (yyval.b) = false; }
#line 9253 "bison-chapel.cpp"
    break;

  case 418: /* opt_throws_error: TTHROWS  */
#line 1709 "chapel.ypp"
          { (yyval.b) = true;  }
#line 9259 "bison-chapel.cpp"
    break;

  case 419: /* opt_function_body_stmt: TSEMI  */
#line 1712 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 9265 "bison-chapel.cpp"
    break;

  case 422: /* function_body_stmt: return_stmt  */
#line 1718 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9271 "bison-chapel.cpp"
    break;

  case 423: /* query_expr: TQUERIEDIDENT  */
#line 1722 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9277 "bison-chapel.cpp"
    break;

  case 424: /* var_arg_expr: TDOTDOTDOT  */
#line 1726 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9283 "bison-chapel.cpp"
    break;

  case 425: /* var_arg_expr: TDOTDOTDOT expr  */
#line 1727 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9289 "bison-chapel.cpp"
    break;

  case 426: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 1728 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9299 "bison-chapel.cpp"
    break;

  case 427: /* opt_lifetime_where: %empty  */
#line 1736 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9305 "bison-chapel.cpp"
    break;

  case 428: /* opt_lifetime_where: TWHERE expr  */
#line 1738 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9311 "bison-chapel.cpp"
    break;

  case 429: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 1740 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9317 "bison-chapel.cpp"
    break;

  case 430: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 1742 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9323 "bison-chapel.cpp"
    break;

  case 431: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 1744 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9329 "bison-chapel.cpp"
    break;

  case 432: /* lifetime_components_expr: lifetime_expr  */
#line 1749 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9335 "bison-chapel.cpp"
    break;

  case 433: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 1751 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9341 "bison-chapel.cpp"
    break;

  case 434: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 1755 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9347 "bison-chapel.cpp"
    break;

  case 435: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 1756 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9353 "bison-chapel.cpp"
    break;

  case 436: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 1757 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9359 "bison-chapel.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 1758 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9365 "bison-chapel.cpp"
    break;

  case 438: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 1759 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9371 "bison-chapel.cpp"
    break;

  case 439: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 1760 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9377 "bison-chapel.cpp"
    break;

  case 440: /* lifetime_expr: TRETURN lifetime_ident  */
#line 1761 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9383 "bison-chapel.cpp"
    break;

  case 441: /* lifetime_ident: TIDENT  */
#line 1766 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9389 "bison-chapel.cpp"
    break;

  case 442: /* lifetime_ident: TTHIS  */
#line 1768 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9395 "bison-chapel.cpp"
    break;

  case 443: /* type_alias_decl_stmt: TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1773 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9401 "bison-chapel.cpp"
    break;

  case 444: /* type_alias_decl_stmt: TCONFIG TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1775 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9407 "bison-chapel.cpp"
    break;

  case 445: /* type_alias_decl_stmt: TEXTERN TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1777 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9413 "bison-chapel.cpp"
    break;

  case 446: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 1782 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9430 "bison-chapel.cpp"
    break;

  case 447: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 1795 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9448 "bison-chapel.cpp"
    break;

  case 448: /* opt_init_type: %empty  */
#line 1811 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9454 "bison-chapel.cpp"
    break;

  case 449: /* opt_init_type: TASSIGN type_level_expr  */
#line 1813 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9460 "bison-chapel.cpp"
    break;

  case 450: /* opt_init_type: TASSIGN array_type  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9466 "bison-chapel.cpp"
    break;

  case 451: /* var_decl_type: TPARAM  */
#line 1819 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9472 "bison-chapel.cpp"
    break;

  case 452: /* var_decl_type: TCONST  */
#line 1820 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9478 "bison-chapel.cpp"
    break;

  case 453: /* var_decl_type: TREF  */
#line 1821 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9484 "bison-chapel.cpp"
    break;

  case 454: /* var_decl_type: TCONST TREF  */
#line 1822 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9490 "bison-chapel.cpp"
    break;

  case 455: /* var_decl_type: TVAR  */
#line 1823 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9496 "bison-chapel.cpp"
    break;

  case 456: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1828 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9506 "bison-chapel.cpp"
    break;

  case 457: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1834 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9515 "bison-chapel.cpp"
    break;

  case 459: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 1843 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9524 "bison-chapel.cpp"
    break;

  case 460: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 1851 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9530 "bison-chapel.cpp"
    break;

  case 461: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 1853 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9536 "bison-chapel.cpp"
    break;

  case 462: /* tuple_var_decl_component: TUNDERSCORE  */
#line 1858 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9542 "bison-chapel.cpp"
    break;

  case 463: /* tuple_var_decl_component: ident_def  */
#line 1860 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9548 "bison-chapel.cpp"
    break;

  case 464: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 1862 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9554 "bison-chapel.cpp"
    break;

  case 465: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 1867 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9560 "bison-chapel.cpp"
    break;

  case 466: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 1869 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9566 "bison-chapel.cpp"
    break;

  case 467: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 1871 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9572 "bison-chapel.cpp"
    break;

  case 468: /* opt_init_expr: %empty  */
#line 1877 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9578 "bison-chapel.cpp"
    break;

  case 469: /* opt_init_expr: TASSIGN TNOINIT  */
#line 1878 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9584 "bison-chapel.cpp"
    break;

  case 470: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 1879 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9590 "bison-chapel.cpp"
    break;

  case 471: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 1885 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9596 "bison-chapel.cpp"
    break;

  case 472: /* ret_array_type: TLSBR TRSBR  */
#line 1887 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9602 "bison-chapel.cpp"
    break;

  case 473: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1889 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9610 "bison-chapel.cpp"
    break;

  case 474: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 1893 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9618 "bison-chapel.cpp"
    break;

  case 475: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9624 "bison-chapel.cpp"
    break;

  case 476: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 1899 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9632 "bison-chapel.cpp"
    break;

  case 477: /* ret_array_type: TLSBR error TRSBR  */
#line 1903 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9640 "bison-chapel.cpp"
    break;

  case 478: /* opt_ret_type: %empty  */
#line 1910 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9646 "bison-chapel.cpp"
    break;

  case 479: /* opt_ret_type: TCOLON type_level_expr  */
#line 1911 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9652 "bison-chapel.cpp"
    break;

  case 480: /* opt_ret_type: TCOLON ret_array_type  */
#line 1912 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9658 "bison-chapel.cpp"
    break;

  case 481: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 1913 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9664 "bison-chapel.cpp"
    break;

  case 482: /* opt_ret_type: error  */
#line 1914 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9670 "bison-chapel.cpp"
    break;

  case 483: /* opt_type: %empty  */
#line 1919 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9676 "bison-chapel.cpp"
    break;

  case 484: /* opt_type: TCOLON type_level_expr  */
#line 1920 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9682 "bison-chapel.cpp"
    break;

  case 485: /* opt_type: TCOLON array_type  */
#line 1921 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9688 "bison-chapel.cpp"
    break;

  case 486: /* opt_type: TCOLON reserved_type_ident_use  */
#line 1922 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9694 "bison-chapel.cpp"
    break;

  case 487: /* opt_type: error  */
#line 1923 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9700 "bison-chapel.cpp"
    break;

  case 488: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1944 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9708 "bison-chapel.cpp"
    break;

  case 489: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 1948 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9716 "bison-chapel.cpp"
    break;

  case 490: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 1952 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9728 "bison-chapel.cpp"
    break;

  case 491: /* array_type: TLSBR error TRSBR  */
#line 1960 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9736 "bison-chapel.cpp"
    break;

  case 492: /* opt_formal_array_elt_type: %empty  */
#line 1966 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9742 "bison-chapel.cpp"
    break;

  case 493: /* opt_formal_array_elt_type: type_level_expr  */
#line 1967 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9748 "bison-chapel.cpp"
    break;

  case 494: /* opt_formal_array_elt_type: query_expr  */
#line 1968 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9754 "bison-chapel.cpp"
    break;

  case 495: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 1973 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9760 "bison-chapel.cpp"
    break;

  case 496: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 1975 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9766 "bison-chapel.cpp"
    break;

  case 497: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 1981 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9772 "bison-chapel.cpp"
    break;

  case 498: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 1983 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9778 "bison-chapel.cpp"
    break;

  case 499: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9784 "bison-chapel.cpp"
    break;

  case 500: /* opt_formal_type: %empty  */
#line 1989 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9790 "bison-chapel.cpp"
    break;

  case 501: /* opt_formal_type: TCOLON type_level_expr  */
#line 1990 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9796 "bison-chapel.cpp"
    break;

  case 502: /* opt_formal_type: TCOLON query_expr  */
#line 1991 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9802 "bison-chapel.cpp"
    break;

  case 503: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 1992 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9808 "bison-chapel.cpp"
    break;

  case 504: /* opt_formal_type: TCOLON formal_array_type  */
#line 1993 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9814 "bison-chapel.cpp"
    break;

  case 505: /* expr_ls: expr  */
#line 1999 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9820 "bison-chapel.cpp"
    break;

  case 506: /* expr_ls: query_expr  */
#line 2000 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9826 "bison-chapel.cpp"
    break;

  case 507: /* expr_ls: expr_ls TCOMMA expr  */
#line 2001 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9832 "bison-chapel.cpp"
    break;

  case 508: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2002 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9838 "bison-chapel.cpp"
    break;

  case 509: /* simple_expr_ls: expr  */
#line 2006 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9844 "bison-chapel.cpp"
    break;

  case 510: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2007 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9850 "bison-chapel.cpp"
    break;

  case 511: /* tuple_component: TUNDERSCORE  */
#line 2011 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9856 "bison-chapel.cpp"
    break;

  case 512: /* tuple_component: opt_try_expr  */
#line 2012 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9862 "bison-chapel.cpp"
    break;

  case 513: /* tuple_component: query_expr  */
#line 2013 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9868 "bison-chapel.cpp"
    break;

  case 514: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2017 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9874 "bison-chapel.cpp"
    break;

  case 515: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2018 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9880 "bison-chapel.cpp"
    break;

  case 516: /* opt_actual_ls: %empty  */
#line 2022 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9886 "bison-chapel.cpp"
    break;

  case 518: /* actual_ls: actual_expr  */
#line 2027 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9892 "bison-chapel.cpp"
    break;

  case 519: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2028 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9898 "bison-chapel.cpp"
    break;

  case 520: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2032 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9904 "bison-chapel.cpp"
    break;

  case 521: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2033 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9910 "bison-chapel.cpp"
    break;

  case 522: /* actual_expr: query_expr  */
#line 2034 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9916 "bison-chapel.cpp"
    break;

  case 523: /* actual_expr: opt_try_expr  */
#line 2035 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9922 "bison-chapel.cpp"
    break;

  case 524: /* ident_expr: ident_use  */
#line 2039 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9928 "bison-chapel.cpp"
    break;

  case 525: /* ident_expr: scalar_type  */
#line 2040 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9934 "bison-chapel.cpp"
    break;

  case 526: /* type_level_expr: sub_type_level_expr  */
#line 2052 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9940 "bison-chapel.cpp"
    break;

  case 527: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2054 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9946 "bison-chapel.cpp"
    break;

  case 528: /* type_level_expr: TQUESTION  */
#line 2056 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9952 "bison-chapel.cpp"
    break;

  case 534: /* sub_type_level_expr: TSINGLE expr  */
#line 2067 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9958 "bison-chapel.cpp"
    break;

  case 535: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2069 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9964 "bison-chapel.cpp"
    break;

  case 536: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2071 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9970 "bison-chapel.cpp"
    break;

  case 537: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2073 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9976 "bison-chapel.cpp"
    break;

  case 538: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeTypeForParentDomain", (yyvsp[-1].pexpr)); }
#line 9982 "bison-chapel.cpp"
    break;

  case 539: /* sub_type_level_expr: TATOMIC expr  */
#line 2077 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9988 "bison-chapel.cpp"
    break;

  case 540: /* sub_type_level_expr: TSYNC expr  */
#line 2079 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9994 "bison-chapel.cpp"
    break;

  case 541: /* sub_type_level_expr: TOWNED  */
#line 2082 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 10000 "bison-chapel.cpp"
    break;

  case 542: /* sub_type_level_expr: TOWNED expr  */
#line 2084 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 10006 "bison-chapel.cpp"
    break;

  case 543: /* sub_type_level_expr: TUNMANAGED  */
#line 2086 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 10012 "bison-chapel.cpp"
    break;

  case 544: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2088 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10018 "bison-chapel.cpp"
    break;

  case 545: /* sub_type_level_expr: TSHARED  */
#line 2090 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 10024 "bison-chapel.cpp"
    break;

  case 546: /* sub_type_level_expr: TSHARED expr  */
#line 2092 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 10030 "bison-chapel.cpp"
    break;

  case 547: /* sub_type_level_expr: TBORROWED  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 10036 "bison-chapel.cpp"
    break;

  case 548: /* sub_type_level_expr: TBORROWED expr  */
#line 2096 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10042 "bison-chapel.cpp"
    break;

  case 549: /* sub_type_level_expr: TCLASS  */
#line 2099 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 10048 "bison-chapel.cpp"
    break;

  case 550: /* sub_type_level_expr: TRECORD  */
#line 2101 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 10054 "bison-chapel.cpp"
    break;

  case 551: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2106 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10060 "bison-chapel.cpp"
    break;

  case 552: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2108 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10066 "bison-chapel.cpp"
    break;

  case 553: /* for_expr: TFOR expr TDO expr  */
#line 2110 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10072 "bison-chapel.cpp"
    break;

  case 554: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2112 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10078 "bison-chapel.cpp"
    break;

  case 555: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2114 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10084 "bison-chapel.cpp"
    break;

  case 556: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2116 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10090 "bison-chapel.cpp"
    break;

  case 557: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2118 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10096 "bison-chapel.cpp"
    break;

  case 558: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2120 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10102 "bison-chapel.cpp"
    break;

  case 559: /* for_expr: TFORALL expr TDO expr  */
#line 2122 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10108 "bison-chapel.cpp"
    break;

  case 560: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2124 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10114 "bison-chapel.cpp"
    break;

  case 561: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2126 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10120 "bison-chapel.cpp"
    break;

  case 562: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2128 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10126 "bison-chapel.cpp"
    break;

  case 563: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2130 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 10137 "bison-chapel.cpp"
    break;

  case 564: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2137 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 10147 "bison-chapel.cpp"
    break;

  case 565: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2143 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10157 "bison-chapel.cpp"
    break;

  case 566: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2149 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10167 "bison-chapel.cpp"
    break;

  case 567: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2155 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10177 "bison-chapel.cpp"
    break;

  case 568: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2164 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10183 "bison-chapel.cpp"
    break;

  case 569: /* nil_expr: TNIL  */
#line 2173 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 10189 "bison-chapel.cpp"
    break;

  case 577: /* stmt_level_expr: io_expr TIO expr  */
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10195 "bison-chapel.cpp"
    break;

  case 578: /* opt_task_intent_ls: %empty  */
#line 2193 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 10201 "bison-chapel.cpp"
    break;

  case 580: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2198 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10207 "bison-chapel.cpp"
    break;

  case 581: /* task_intent_ls: intent_expr  */
#line 2202 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10213 "bison-chapel.cpp"
    break;

  case 582: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2203 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10219 "bison-chapel.cpp"
    break;

  case 583: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2207 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10225 "bison-chapel.cpp"
    break;

  case 584: /* forall_intent_ls: intent_expr  */
#line 2211 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10231 "bison-chapel.cpp"
    break;

  case 585: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2212 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10237 "bison-chapel.cpp"
    break;

  case 586: /* intent_expr: shadow_var_prefix ident_expr opt_type opt_init_expr  */
#line 2217 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10245 "bison-chapel.cpp"
    break;

  case 587: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2221 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10253 "bison-chapel.cpp"
    break;

  case 588: /* intent_expr: expr TREDUCE ident_expr  */
#line 2225 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10261 "bison-chapel.cpp"
    break;

  case 589: /* shadow_var_prefix: TCONST  */
#line 2231 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10267 "bison-chapel.cpp"
    break;

  case 590: /* shadow_var_prefix: TIN  */
#line 2232 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10273 "bison-chapel.cpp"
    break;

  case 591: /* shadow_var_prefix: TCONST TIN  */
#line 2233 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10279 "bison-chapel.cpp"
    break;

  case 592: /* shadow_var_prefix: TREF  */
#line 2234 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10285 "bison-chapel.cpp"
    break;

  case 593: /* shadow_var_prefix: TCONST TREF  */
#line 2235 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10291 "bison-chapel.cpp"
    break;

  case 594: /* shadow_var_prefix: TVAR  */
#line 2236 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10297 "bison-chapel.cpp"
    break;

  case 596: /* io_expr: io_expr TIO expr  */
#line 2242 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10303 "bison-chapel.cpp"
    break;

  case 597: /* new_maybe_decorated: TNEW  */
#line 2247 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10309 "bison-chapel.cpp"
    break;

  case 598: /* new_maybe_decorated: TNEW TOWNED  */
#line 2249 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10317 "bison-chapel.cpp"
    break;

  case 599: /* new_maybe_decorated: TNEW TSHARED  */
#line 2253 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10325 "bison-chapel.cpp"
    break;

  case 600: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2257 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10333 "bison-chapel.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2261 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10341 "bison-chapel.cpp"
    break;

  case 602: /* new_expr: new_maybe_decorated expr  */
#line 2269 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10348 "bison-chapel.cpp"
    break;

  case 603: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2274 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10358 "bison-chapel.cpp"
    break;

  case 604: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2280 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10368 "bison-chapel.cpp"
    break;

  case 605: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2286 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10379 "bison-chapel.cpp"
    break;

  case 606: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2293 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10390 "bison-chapel.cpp"
    break;

  case 607: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2303 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10396 "bison-chapel.cpp"
    break;

  case 617: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2320 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10402 "bison-chapel.cpp"
    break;

  case 618: /* expr: expr TCOLON expr  */
#line 2322 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10408 "bison-chapel.cpp"
    break;

  case 619: /* expr: expr TDOTDOT expr  */
#line 2324 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10414 "bison-chapel.cpp"
    break;

  case 620: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2326 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10420 "bison-chapel.cpp"
    break;

  case 621: /* expr: expr TDOTDOT  */
#line 2341 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10426 "bison-chapel.cpp"
    break;

  case 622: /* expr: TDOTDOT expr  */
#line 2343 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10432 "bison-chapel.cpp"
    break;

  case 623: /* expr: TDOTDOTOPENHIGH expr  */
#line 2345 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10438 "bison-chapel.cpp"
    break;

  case 624: /* expr: TDOTDOT  */
#line 2347 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10444 "bison-chapel.cpp"
    break;

  case 625: /* opt_expr: %empty  */
#line 2351 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10450 "bison-chapel.cpp"
    break;

  case 626: /* opt_expr: expr  */
#line 2352 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10456 "bison-chapel.cpp"
    break;

  case 627: /* opt_try_expr: TTRY expr  */
#line 2356 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10462 "bison-chapel.cpp"
    break;

  case 628: /* opt_try_expr: TTRYBANG expr  */
#line 2357 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10468 "bison-chapel.cpp"
    break;

  case 629: /* opt_try_expr: expr  */
#line 2358 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10474 "bison-chapel.cpp"
    break;

  case 634: /* call_base_expr: lhs_expr  */
#line 2374 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10480 "bison-chapel.cpp"
    break;

  case 635: /* call_base_expr: expr TBANG  */
#line 2375 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10486 "bison-chapel.cpp"
    break;

  case 636: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2376 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10492 "bison-chapel.cpp"
    break;

  case 637: /* call_base_expr: lambda_decl_expr  */
#line 2377 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10498 "bison-chapel.cpp"
    break;

  case 639: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2382 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10504 "bison-chapel.cpp"
    break;

  case 640: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2383 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10510 "bison-chapel.cpp"
    break;

  case 641: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2384 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10516 "bison-chapel.cpp"
    break;

  case 642: /* dot_expr: expr TDOT ident_use  */
#line 2388 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10522 "bison-chapel.cpp"
    break;

  case 643: /* dot_expr: expr TDOT TTYPE  */
#line 2389 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10528 "bison-chapel.cpp"
    break;

  case 644: /* dot_expr: expr TDOT TDOMAIN  */
#line 2390 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10534 "bison-chapel.cpp"
    break;

  case 645: /* dot_expr: expr TDOT TLOCALE  */
#line 2391 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10540 "bison-chapel.cpp"
    break;

  case 646: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2392 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10546 "bison-chapel.cpp"
    break;

  case 647: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2400 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10552 "bison-chapel.cpp"
    break;

  case 648: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2401 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10558 "bison-chapel.cpp"
    break;

  case 649: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2402 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10564 "bison-chapel.cpp"
    break;

  case 650: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2403 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10570 "bison-chapel.cpp"
    break;

  case 651: /* bool_literal: TFALSE  */
#line 2407 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10576 "bison-chapel.cpp"
    break;

  case 652: /* bool_literal: TTRUE  */
#line 2408 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10582 "bison-chapel.cpp"
    break;

  case 653: /* str_bytes_literal: STRINGLITERAL  */
#line 2412 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10588 "bison-chapel.cpp"
    break;

  case 654: /* str_bytes_literal: BYTESLITERAL  */
#line 2413 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10594 "bison-chapel.cpp"
    break;

  case 657: /* literal_expr: INTLITERAL  */
#line 2419 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10600 "bison-chapel.cpp"
    break;

  case 658: /* literal_expr: REALLITERAL  */
#line 2420 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10606 "bison-chapel.cpp"
    break;

  case 659: /* literal_expr: IMAGLITERAL  */
#line 2421 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10612 "bison-chapel.cpp"
    break;

  case 660: /* literal_expr: CSTRINGLITERAL  */
#line 2422 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10618 "bison-chapel.cpp"
    break;

  case 661: /* literal_expr: TNONE  */
#line 2423 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10624 "bison-chapel.cpp"
    break;

  case 662: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2424 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10631 "bison-chapel.cpp"
    break;

  case 663: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2426 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10638 "bison-chapel.cpp"
    break;

  case 664: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2428 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10644 "bison-chapel.cpp"
    break;

  case 665: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2429 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10650 "bison-chapel.cpp"
    break;

  case 666: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2431 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10658 "bison-chapel.cpp"
    break;

  case 667: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2435 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10666 "bison-chapel.cpp"
    break;

  case 668: /* assoc_expr_ls: expr TALIAS expr  */
#line 2442 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10672 "bison-chapel.cpp"
    break;

  case 669: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2443 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10678 "bison-chapel.cpp"
    break;

  case 670: /* binary_op_expr: expr TPLUS expr  */
#line 2447 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10684 "bison-chapel.cpp"
    break;

  case 671: /* binary_op_expr: expr TMINUS expr  */
#line 2448 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10690 "bison-chapel.cpp"
    break;

  case 672: /* binary_op_expr: expr TSTAR expr  */
#line 2449 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10696 "bison-chapel.cpp"
    break;

  case 673: /* binary_op_expr: expr TDIVIDE expr  */
#line 2450 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10702 "bison-chapel.cpp"
    break;

  case 674: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2451 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10708 "bison-chapel.cpp"
    break;

  case 675: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2452 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10714 "bison-chapel.cpp"
    break;

  case 676: /* binary_op_expr: expr TMOD expr  */
#line 2453 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10720 "bison-chapel.cpp"
    break;

  case 677: /* binary_op_expr: expr TEQUAL expr  */
#line 2454 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10726 "bison-chapel.cpp"
    break;

  case 678: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2455 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10732 "bison-chapel.cpp"
    break;

  case 679: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2456 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10738 "bison-chapel.cpp"
    break;

  case 680: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2457 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10744 "bison-chapel.cpp"
    break;

  case 681: /* binary_op_expr: expr TLESS expr  */
#line 2458 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10750 "bison-chapel.cpp"
    break;

  case 682: /* binary_op_expr: expr TGREATER expr  */
#line 2459 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10756 "bison-chapel.cpp"
    break;

  case 683: /* binary_op_expr: expr TBAND expr  */
#line 2460 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10762 "bison-chapel.cpp"
    break;

  case 684: /* binary_op_expr: expr TBOR expr  */
#line 2461 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10768 "bison-chapel.cpp"
    break;

  case 685: /* binary_op_expr: expr TBXOR expr  */
#line 2462 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10774 "bison-chapel.cpp"
    break;

  case 686: /* binary_op_expr: expr TAND expr  */
#line 2463 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10780 "bison-chapel.cpp"
    break;

  case 687: /* binary_op_expr: expr TOR expr  */
#line 2464 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10786 "bison-chapel.cpp"
    break;

  case 688: /* binary_op_expr: expr TEXP expr  */
#line 2465 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10792 "bison-chapel.cpp"
    break;

  case 689: /* binary_op_expr: expr TBY expr  */
#line 2466 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10798 "bison-chapel.cpp"
    break;

  case 690: /* binary_op_expr: expr TALIGN expr  */
#line 2467 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10804 "bison-chapel.cpp"
    break;

  case 691: /* binary_op_expr: expr THASH expr  */
#line 2468 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10810 "bison-chapel.cpp"
    break;

  case 692: /* binary_op_expr: expr TDMAPPED expr  */
#line 2469 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10817 "bison-chapel.cpp"
    break;

  case 693: /* unary_op_expr: TPLUS expr  */
#line 2474 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10823 "bison-chapel.cpp"
    break;

  case 694: /* unary_op_expr: TMINUS expr  */
#line 2475 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10829 "bison-chapel.cpp"
    break;

  case 695: /* unary_op_expr: TMINUSMINUS expr  */
#line 2476 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10835 "bison-chapel.cpp"
    break;

  case 696: /* unary_op_expr: TPLUSPLUS expr  */
#line 2477 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10841 "bison-chapel.cpp"
    break;

  case 697: /* unary_op_expr: TBANG expr  */
#line 2478 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10847 "bison-chapel.cpp"
    break;

  case 698: /* unary_op_expr: expr TBANG  */
#line 2479 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10853 "bison-chapel.cpp"
    break;

  case 699: /* unary_op_expr: TBNOT expr  */
#line 2480 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10859 "bison-chapel.cpp"
    break;

  case 700: /* reduce_expr: expr TREDUCE expr  */
#line 2484 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10865 "bison-chapel.cpp"
    break;

  case 701: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2485 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10871 "bison-chapel.cpp"
    break;

  case 702: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2486 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10877 "bison-chapel.cpp"
    break;

  case 703: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2487 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10883 "bison-chapel.cpp"
    break;

  case 704: /* scan_expr: expr TSCAN expr  */
#line 2491 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10889 "bison-chapel.cpp"
    break;

  case 705: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2492 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10895 "bison-chapel.cpp"
    break;

  case 706: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2493 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10901 "bison-chapel.cpp"
    break;

  case 707: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2494 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10907 "bison-chapel.cpp"
    break;

  case 708: /* reduce_scan_op_expr: TPLUS  */
#line 2499 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10913 "bison-chapel.cpp"
    break;

  case 709: /* reduce_scan_op_expr: TSTAR  */
#line 2500 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10919 "bison-chapel.cpp"
    break;

  case 710: /* reduce_scan_op_expr: TAND  */
#line 2501 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10925 "bison-chapel.cpp"
    break;

  case 711: /* reduce_scan_op_expr: TOR  */
#line 2502 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10931 "bison-chapel.cpp"
    break;

  case 712: /* reduce_scan_op_expr: TBAND  */
#line 2503 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10937 "bison-chapel.cpp"
    break;

  case 713: /* reduce_scan_op_expr: TBOR  */
#line 2504 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10943 "bison-chapel.cpp"
    break;

  case 714: /* reduce_scan_op_expr: TBXOR  */
#line 2505 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10949 "bison-chapel.cpp"
    break;


#line 10953 "bison-chapel.cpp"

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
