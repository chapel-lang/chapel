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
#line 219 "chapel.ypp"

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
#define YYLAST   21271

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  174
/* YYNRULES -- Number of rules.  */
#define YYNRULES  715
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1290

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
       0,   523,   523,   528,   529,   535,   536,   541,   542,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   587,   588,   590,   595,   596,   600,   613,
     618,   623,   631,   632,   633,   637,   638,   642,   643,   644,
     649,   648,   669,   670,   671,   676,   677,   682,   687,   692,
     697,   701,   705,   714,   719,   724,   729,   733,   737,   745,
     750,   754,   755,   756,   760,   761,   762,   763,   764,   765,
     766,   770,   775,   776,   782,   783,   787,   788,   792,   796,
     798,   800,   802,   804,   806,   813,   814,   818,   819,   820,
     821,   822,   823,   826,   827,   828,   829,   830,   831,   843,
     844,   855,   856,   857,   858,   859,   860,   861,   862,   863,
     864,   865,   866,   867,   868,   869,   870,   871,   872,   873,
     877,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     887,   888,   895,   896,   897,   898,   902,   903,   907,   908,
     912,   913,   914,   918,   919,   923,   927,   928,   930,   935,
     936,   937,   947,   947,   952,   953,   954,   955,   956,   957,
     958,   962,   963,   964,   965,   970,   969,   986,   985,  1003,
    1002,  1019,  1018,  1036,  1035,  1051,  1050,  1066,  1070,  1075,
    1083,  1094,  1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,
    1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,
    1119,  1120,  1121,  1122,  1123,  1124,  1130,  1136,  1142,  1148,
    1155,  1162,  1166,  1173,  1177,  1178,  1179,  1180,  1182,  1183,
    1184,  1185,  1187,  1189,  1191,  1193,  1198,  1199,  1203,  1205,
    1213,  1214,  1219,  1224,  1225,  1226,  1227,  1228,  1229,  1230,
    1231,  1232,  1233,  1234,  1235,  1236,  1243,  1244,  1245,  1246,
    1255,  1256,  1260,  1262,  1265,  1271,  1273,  1276,  1282,  1285,
    1286,  1289,  1290,  1294,  1295,  1299,  1300,  1301,  1305,  1306,
    1310,  1313,  1315,  1320,  1321,  1325,  1327,  1329,  1336,  1346,
    1360,  1365,  1370,  1378,  1379,  1384,  1385,  1387,  1392,  1408,
    1415,  1424,  1432,  1436,  1443,  1444,  1446,  1451,  1452,  1457,
    1462,  1456,  1489,  1492,  1496,  1504,  1514,  1503,  1553,  1557,
    1562,  1566,  1571,  1578,  1579,  1583,  1584,  1585,  1589,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,
    1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,
    1611,  1612,  1613,  1614,  1618,  1619,  1620,  1621,  1622,  1623,
    1624,  1625,  1626,  1627,  1628,  1629,  1633,  1634,  1638,  1639,
    1643,  1647,  1648,  1652,  1653,  1657,  1659,  1661,  1663,  1665,
    1667,  1677,  1678,  1682,  1683,  1684,  1685,  1686,  1687,  1688,
    1689,  1690,  1694,  1695,  1696,  1697,  1698,  1699,  1703,  1704,
    1705,  1709,  1710,  1711,  1712,  1713,  1714,  1718,  1719,  1722,
    1723,  1727,  1728,  1732,  1736,  1737,  1738,  1746,  1747,  1749,
    1751,  1753,  1758,  1760,  1765,  1766,  1767,  1768,  1769,  1770,
    1771,  1775,  1777,  1782,  1784,  1786,  1788,  1793,  1806,  1823,
    1824,  1826,  1831,  1832,  1833,  1834,  1835,  1839,  1845,  1853,
    1854,  1862,  1864,  1869,  1871,  1873,  1878,  1880,  1882,  1889,
    1890,  1891,  1896,  1898,  1900,  1904,  1908,  1910,  1914,  1922,
    1923,  1924,  1925,  1926,  1931,  1932,  1933,  1934,  1935,  1955,
    1959,  1963,  1971,  1978,  1979,  1980,  1984,  1986,  1992,  1994,
    1996,  2001,  2002,  2003,  2004,  2005,  2011,  2012,  2013,  2014,
    2018,  2019,  2023,  2024,  2025,  2029,  2030,  2034,  2035,  2039,
    2040,  2044,  2045,  2046,  2047,  2051,  2052,  2063,  2065,  2067,
    2073,  2074,  2075,  2076,  2077,  2078,  2080,  2082,  2084,  2086,
    2088,  2090,  2093,  2095,  2097,  2099,  2101,  2103,  2105,  2107,
    2110,  2112,  2117,  2119,  2121,  2123,  2125,  2127,  2129,  2131,
    2133,  2135,  2137,  2139,  2141,  2148,  2154,  2160,  2166,  2175,
    2185,  2193,  2194,  2195,  2196,  2197,  2198,  2199,  2200,  2205,
    2206,  2210,  2214,  2215,  2219,  2223,  2224,  2228,  2232,  2236,
    2243,  2244,  2245,  2246,  2247,  2248,  2252,  2253,  2258,  2260,
    2264,  2268,  2272,  2280,  2285,  2291,  2297,  2304,  2314,  2322,
    2323,  2324,  2325,  2326,  2327,  2328,  2329,  2330,  2331,  2333,
    2335,  2337,  2352,  2354,  2356,  2358,  2363,  2364,  2368,  2369,
    2370,  2374,  2375,  2376,  2377,  2386,  2387,  2388,  2389,  2390,
    2394,  2395,  2396,  2400,  2401,  2402,  2403,  2404,  2412,  2413,
    2414,  2415,  2419,  2420,  2424,  2425,  2429,  2430,  2431,  2432,
    2433,  2434,  2435,  2436,  2438,  2440,  2441,  2442,  2446,  2454,
    2455,  2459,  2460,  2461,  2462,  2463,  2464,  2465,  2466,  2467,
    2468,  2469,  2470,  2471,  2472,  2473,  2474,  2475,  2476,  2477,
    2478,  2479,  2480,  2481,  2486,  2487,  2488,  2489,  2490,  2491,
    2492,  2496,  2497,  2498,  2499,  2503,  2504,  2505,  2506,  2511,
    2512,  2513,  2514,  2515,  2516,  2517
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

#define YYPACT_NINF (-1092)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-716)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1092,   107,  3583, -1092,   -48,    62, -1092, -1092, -1092, -1092,
   -1092, -1092, 12122,    33,   236,   204, 15029,   215, 20720,    33,
   12122,   248,   400,   223,   236,  4983, 12122,  2152,  4983,   181,
   20807, 12295,  8298,   277,  8992, 10557, 10557,  7427,  9165,   292,
   -1092,   261, -1092,   337, 20894, 20894, 20894, -1092,  2915, 10730,
     406, 12122, 12122,   241, -1092,   426,   430, 12122, -1092, 15029,
   -1092, 12122,   319,   305,   532, 17341,   431, 20981, -1092, 10905,
    8471, 12122, 10730, 15029, 12122,   397,   442,   347,  4983,   472,
   12122,   476, -1092, -1092, 20894,   494, -1092, 15029, -1092,   501,
    9165,  9338, -1092, 12122, -1092, 12122, -1092, -1092, 14544, 12122,
   -1092, 12122, -1092, -1092, -1092,  3933,  7602,  9513, 12122, -1092,
    4808, -1092, -1092, -1092, -1092,   412, -1092,   523, -1092, -1092,
      97, -1092, -1092, -1092, -1092, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092, -1092, -1092, -1092, -1092, -1092,   562, -1092, -1092,
   -1092, -1092,  7254, -1092, -1092, -1092, -1092, 20894, -1092, 20894,
     275,    46, -1092, -1092,  2915, -1092,   461, -1092,   468, -1092,
   -1092,   464,   477,   485, 12122,   513,   520, 19843,  1909,   274,
     528,   529, -1092, -1092,   401, -1092, -1092, -1092, -1092, -1092,
     507, -1092, -1092, -1092, 12122, 12122, 12122, 20894, -1092, 12122,
   10905, 10905,   603,   415, -1092, -1092, -1092, -1092, 19843,   447,
   -1092, -1092,   503,  4983, -1092, -1092,   534,   125,   518, 16855,
   20894,  2915, -1092,   540, -1092,   116, 19843, 17078, -1092, -1092,
    9686,    38, 18207, -1092, -1092,   578,  8646,   619, 21068, 19843,
     465,   384, -1092, 21155, 20894, -1092,   465, 20894,   536,    21,
   16426,    11, 16386,    21, 16466,   210, -1092, 16618, 20894, 20894,
     -31, 15626,   435,  8646, -1092, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092, -1092, -1092, -1092, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092, -1092, -1092, -1092, -1092, -1092, -1092,   539, -1092,
     478,  4983,   542,  1498,   147,    87, -1092,  4983, -1092, -1092,
   16895, -1092,    36, 17404,   750, -1092,   544,   547, -1092, 16895,
     125,   750, -1092,  8646,  1262, -1092, -1092, -1092,   190, 19843,
   12122, 12122, -1092, 19843,   557, 17511, -1092, 16895,   125, 19843,
     552,  8646, -1092, 19843, 17556,   587,   560,   125,    21, 16895,
     561,   195,   195,  1927,   750,   750,   -59, -1092, -1092,  4108,
     266, -1092, 12122, -1092,   -54,   106, -1092,   -38,   -10, 17602,
     101,  1927,   723, -1092,  4283, -1092,   666, 12122, 12122, 20894,
   -1092, -1092,   597,   563, -1092, -1092, -1092, -1092,   259,   488,
   -1092, 12122,   601, 12122, 12122, 12122, 10557, 10557, 12122,   510,
   12122, 12122, 12122, 12122, 12122,   449, 14544, 12122, 12122, 12122,
   12122, 12122, 12122, 12122, 12122, 12122, 12122, 12122, 12122, 12122,
   12122, 12122, 12122,   674, -1092, -1092, -1092, -1092, -1092,  9338,
    9338, -1092, -1092, -1092, -1092,  9338, -1092, -1092,  9338,  9338,
    8646,  8646, 10557, 10557, 16935, 17087, 17708,   573,    43, 20894,
    8125, -1092, -1092,  4458, -1092, 10557,    21,   599,   309, -1092,
   12122, -1092, -1092, 12122,   627, -1092,   594,   623, -1092, -1092,
   -1092, 20894, -1092,  2915, -1092, 20894, -1092, 20894,   606, 20894,
   -1092,  2915,   726, 10905, -1092,  5158, 10557, -1092,   612, -1092,
      21,  5333, 10557, -1092,    21, -1092, 10557, -1092,  7081,  7081,
   -1092,   650,   653,  4983,   756,  4983, -1092,   757, 12122, -1092,
   -1092,   523,   624,  8646, 20894, -1092, -1092,   242, -1092, -1092,
    1498, -1092,   654,   626, -1092, 12468,   671, 12122,  2915, -1092,
   -1092, 12122, -1092, 20445, 12122, 12122, -1092,   628, -1092, 10905,
   -1092, 19843, 19843, -1092,    49, -1092,  8646,   629, -1092, 12641,
     660, -1092, -1092, -1092, -1092, -1092, -1092, -1092,  9861, -1092,
   17754,  7777, -1092,  7952, -1092,  4983,   632, 10557, 10036,  3758,
     633, 12122, 11078, -1092, -1092,    96, -1092,  4633, 20894, -1092,
     315, 17794,   329, 16658,   285,   789, 10905,   641, 20358,   498,
   -1092, 17946, 20190, 20190,   524, -1092,   524, -1092,   524,  1100,
     740,  1535,   424,   125,   195, -1092,   642, -1092, -1092, -1092,
   -1092, -1092,  1927,  1290,   524, 14708, 14708, 20190, 14708, 14708,
    1329,   195,  1290, 17908,  1329,   750,   750,   195,  1927,   651,
     652,   657,   658,   661,   662,   646,   640, -1092,   524, -1092,
     524, 12814, 10557, 12987, 10557, 12122,  8646, 10557, 15311,   656,
      37, -1092, -1092, -1092,   124, -1092,  1735, 19919,   531,    33,
   17991, -1092, -1092, -1092, 19843, 18031,  8646, -1092,  8646, 20894,
     597,   339, 20894,   665, 20894,   597, -1092,   597,   343, 12122,
     129,  9165, 19843,    34, 17127,  8125, -1092,  9165, 19843,    23,
   16703, -1092,    21, 16895, -1092, -1092, -1092, 12122,   546, 12122,
     551,   568, -1092, -1092, -1092, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092, 12122, -1092, -1092, 10209, -1092, -1092,   588, -1092,
     388, -1092, -1092, -1092, 18188,   685,   667, 12122, 12122,   799,
    4983,   802, 18228,  4983, 17167,   772, -1092,   161, -1092,   165,
   -1092,   174, -1092, -1092, -1092, -1092, -1092, -1092,  1025,   694,
     668, -1092,  3199, -1092,   543,   669,  1498,   147,    45,    60,
   12122, 12122,  6908, -1092, -1092,   533,  8819, -1092, 19843, -1092,
   -1092, -1092, 20894, 18268, 18420, -1092, -1092, 19843,   673,   160,
     675, -1092, -1092,   413, 20894, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092,  4983,   -15, 17319, -1092, -1092, 19843,  4983, 19843,
   -1092, 18461, -1092, -1092, -1092, 12122, -1092,    73, 15384, 12122,
   -1092, 11251,  7081,  7081, -1092,  8646,  2625, -1092,   701,  2346,
     679, 20546,   727,   270, -1092, -1092,   761, -1092, -1092, -1092,
   -1092,  3402,   680, -1092, -1092, -1092, -1092, -1092, -1092, -1092,
   -1092, 12122,   821, 18502, 12122,   823, 18654,   168,   683, 18694,
    8646, -1092, -1092,  8125, -1092, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092, -1092, -1092, -1092, -1092,    42, 10557, 10557,    21,
      21, -1092, -1092, -1092, -1092,   597,   689, -1092,   597, -1092,
     597,   690,   691, -1092, 16895, -1092, 15702,  5508, -1092,  5683,
   -1092,   176, -1092, 15778,  5858, -1092,    21,  6033, -1092,    21,
   -1092, -1092,  7081, -1092, 12122, -1092, 19843, 19843,  4983, -1092,
    4983, 12122, -1092,  4983,   829, 20894,   702, 20894,   518, -1092,
   -1092, 20894,  1259, -1092,  1498,   721,   779, -1092, -1092, -1092,
      79, -1092, -1092,   671,   699,    50, -1092, -1092, -1092,   704,
     705, -1092,  6208, 10905, -1092, -1092, -1092, -1092, -1092,  6383,
     703,  6558,   707, -1092, 12122, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092, -1092, -1092, -1092,  7081, -1092, 18739,    95, 17359,
     421,   718,   183, 20894, -1092,   739,   518,   715,  3063, -1092,
   20894, -1092, 12122, 20633, -1092, -1092,   543,   717,   134,   743,
     745,   746,   759,   748,   753,   764,   769,   765,   766,   767,
     393,   776,   775,   777, 12122, -1092,   786,   787,   781,   717,
   -1092,   717, -1092, -1092, 18846, 13160, 13333, 18922, 13506, 13679,
   -1092, 13852, 14025,   221, -1092,   671,   227,   246, -1092, -1092,
     734, -1092,   770,   771, -1092, -1092, -1092,  4983,  9165, 19843,
    9165, 19843,  8125, -1092,  4983,  9165, 19843, -1092,  9165, 19843,
   -1092, -1092, 19007, 19843, -1092, -1092, 19843,   899,  4983,   774,
   -1092, -1092, -1092,   721, -1092,   762, 11426,    13, -1092,   148,
   -1092, -1092, 10557, 15170,  8646,  8646,  4983, -1092,    40,  9165,
   -1092, 19843,  4983,  9165, -1092, 19843,  4983, 19843,   271, 11599,
    7081,  7081,  7081,  7081, -1092, -1092,   778,   763, 12122, -1092,
   -1092,  1868, -1092,  1262, -1092,  1696, -1092, -1092, -1092, 19843,
   -1092,    98,   242, -1092, 19083, -1092, 15543, -1092, -1092, 12122,
   12122, 12122, 12122, 12122, 12122, 12122, 12122, -1092, -1092, -1092,
   18658, -1092, -1092, 19862, 20014, 18228, 15854, 15930, -1092, 18228,
   16006, 16082, 12122,  4983, -1092, -1092,    13,   721, 10384, -1092,
   -1092, -1092,   155, 10905, -1092, -1092,   139, 12122,   -20, 19159,
   -1092,   545,   773,   782,   578, -1092, 16158, -1092, 16234, -1092,
   -1092, -1092, 19843,   437,   784,   439,   791, -1092,   518, 19843,
    3351, -1092, -1092, -1092, 14198,   832,   785, -1092,   804,   807,
     717,   717, 19235, 19311, 19387, 19463, 19539, 19615, 20081, -1092,
   20150, 20217, -1092, -1092,  4983,  4983,  4983,  4983, 19843, -1092,
   -1092, -1092,    13, 11774,    61, -1092, 19843, -1092,   131, -1092,
     -21, -1092,   636, 19691, -1092, -1092, -1092, 14025,   788,   793,
    4983,  4983, -1092, -1092, -1092, -1092, -1092, -1092,  6733, -1092,
   -1092,   234, -1092,   148, -1092, -1092, -1092, 12122, 12122, 12122,
   12122, 12122, 12122, -1092, -1092, -1092, 18228, 18228, 18228, 18228,
   -1092, -1092, -1092, -1092, -1092,   338, 10557, 14716, -1092, 12122,
     139,   131,   131,   131,   131,   131,   131,   139,   595, -1092,
   -1092, 18228, 18228,   790, 14371,    80,    -8, 19767, -1092, -1092,
   19843, -1092, -1092, -1092, -1092, -1092, -1092, -1092,   813, -1092,
   -1092,   340, 14888, -1092, -1092, -1092, 11949, -1092,   373, -1092
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   119,   658,   659,   660,   654,
     655,   661,     0,   579,   105,   140,   548,   147,   550,   579,
       0,   146,     0,   453,   105,     0,     0,   322,     0,   270,
     141,   626,   626,   652,     0,     0,     0,     0,     0,   145,
      60,   271,   323,   142,     0,     0,     0,   319,     0,     0,
     149,     0,     0,   598,   570,   662,   150,     0,   324,   542,
     452,     0,     0,     0,   172,   322,   144,   551,   454,     0,
       0,     0,     0,   546,     0,     0,   148,     0,     0,   120,
       0,   653,   279,   280,     0,   143,   302,   544,   456,   151,
       0,     0,   711,     0,   713,     0,   714,   715,   625,     0,
     712,   709,   529,   169,   710,     0,     0,     0,     0,     4,
       0,     5,    12,     9,    43,     0,    46,    55,    10,    11,
       0,    13,    14,    15,    27,   525,   526,    21,    30,    47,
     170,   179,   180,    16,    29,    28,    18,     0,   265,    17,
     617,    19,     0,    20,    32,    31,   178,     0,   176,     0,
     614,     0,   174,   177,     0,   175,   631,   610,   527,   611,
     532,   530,     0,     0,     0,   615,   616,     0,   531,     0,
     632,   633,   634,   656,   657,   609,   534,   533,   612,   613,
       0,    26,   550,   141,     0,     0,     0,     0,   551,     0,
       0,     0,     0,   614,   631,   530,   615,   616,   540,   531,
     632,   633,     0,     0,   580,   106,     0,   549,     0,   579,
       0,     0,   455,     0,   278,     0,   510,   322,   300,   310,
     626,   172,   322,   301,    45,     0,   517,   654,   551,   627,
     322,   654,   201,   551,     0,   189,   322,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
       0,     0,    57,   517,   113,   121,   133,   127,   126,   135,
     116,   125,   136,   122,   137,   114,   138,   131,   124,   132,
     130,   128,   129,   115,   117,   123,   134,   139,     0,   118,
       0,     0,     0,     0,     0,     0,   459,     0,   157,    38,
       0,   163,     0,   162,   696,   602,   599,   600,   601,     0,
     543,   697,     7,   517,   322,   171,   423,   507,     0,   506,
       0,     0,   158,   630,     0,     0,    41,     0,   547,   535,
       0,   517,    42,   541,     0,   449,     0,   545,     0,     0,
       0,   698,   700,   623,   695,   694,     0,    62,    65,     0,
       0,   512,     0,   514,     0,     0,   513,     0,     0,   506,
       0,   624,     0,     6,     0,    56,     0,     0,     0,     0,
     281,   283,   303,     0,   409,   410,   408,   325,     0,   528,
      33,     0,   603,     0,     0,     0,     0,     0,     0,   699,
       0,     0,     0,     0,     0,     0,   622,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   364,   371,   372,   373,   368,   370,     0,
       0,   366,   369,   367,   365,     0,   375,   374,     0,     0,
     517,   517,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,    22,     0,    35,     0,     0,     0,     0,    23,
       0,    36,    44,     0,   525,   523,     0,   518,   519,   524,
     195,     0,   198,     0,   187,     0,   191,     0,     0,     0,
     197,     0,     0,     0,   211,     0,     0,   210,     0,   219,
       0,     0,     0,   217,     0,   224,     0,   223,     0,    79,
     181,     0,     0,     0,   239,     0,   364,   235,     0,    59,
      58,    55,     0,     0,     0,   249,    24,   391,   320,   463,
       0,   464,   466,     0,   488,     0,   469,     0,     0,   156,
      37,     0,   165,     0,     0,     0,    39,     0,   173,     0,
      98,   628,   629,   159,     0,    40,     0,     0,   290,     0,
     447,   443,   204,   203,    25,    64,    63,    66,     0,   663,
       0,     0,   648,     0,   650,     0,     0,     0,     0,     0,
       0,     0,     0,   667,     8,     0,    49,     0,     0,    96,
       0,    92,     0,    73,   276,   282,     0,     0,     0,   402,
     458,   578,   691,   690,   693,   702,   701,   706,   705,   687,
     684,   685,   686,   619,   674,   119,     0,   645,   646,   120,
     644,   643,   620,   678,   689,   683,   681,   692,   682,   680,
     672,   677,   679,   688,   671,   675,   676,   673,   621,     0,
       0,     0,     0,     0,     0,     0,     0,   704,   703,   708,
     707,     0,     0,     0,     0,     0,     0,     0,   665,   276,
     590,   591,   593,   595,     0,   582,     0,     0,     0,   579,
     579,   207,   444,   457,   511,     0,     0,   537,     0,     0,
     303,     0,     0,     0,     0,   303,   445,   303,     0,     0,
       0,     0,   554,     0,     0,     0,   220,     0,   560,     0,
       0,   218,     0,     0,   359,   357,   362,   356,   338,   341,
     339,   340,   363,   351,   342,   355,   347,   345,   358,   361,
     346,   344,   349,   354,   343,   348,   352,   353,   350,   360,
       0,   376,   377,    68,    67,    80,     0,     0,     0,   238,
       0,   234,     0,     0,     0,     0,   536,     0,   252,     0,
     250,   396,   393,   394,   395,   399,   400,   401,   391,   384,
       0,   381,     0,   392,   411,     0,   467,     0,   154,   155,
     153,   152,     0,   487,   486,   610,     0,   461,   608,   460,
     164,   161,     0,     0,     0,   642,   509,   508,     0,     0,
       0,   538,   451,   610,     0,   664,   618,   649,   515,   651,
     516,   231,     0,     0,     0,   666,   229,   564,     0,   669,
     668,     0,    51,    50,    48,     0,    91,     0,     0,     0,
      84,     0,     0,    79,   273,     0,     0,   284,   304,     0,
       0,     0,   317,     0,   311,   314,   406,   403,   404,   407,
     326,     0,     0,   104,   102,   103,   101,   100,    99,   640,
     641,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   592,   594,     0,   581,   140,   147,   146,   145,   142,
     149,   150,   144,   148,   143,   151,     0,     0,     0,     0,
       0,   202,   521,   522,   520,   303,     0,   200,   303,   446,
     303,     0,     0,   199,     0,   233,     0,     0,   209,     0,
     208,     0,   585,     0,     0,   215,     0,     0,   213,     0,
     222,   221,     0,   182,     0,   183,   247,   246,     0,   241,
       0,     0,   237,     0,   243,     0,   275,     0,     0,   397,
     398,     0,   391,   380,     0,   501,   412,   415,   414,   416,
       0,   465,   468,   469,     0,     0,   470,   471,   160,     0,
       0,   292,     0,     0,   291,   294,   539,   448,   232,     0,
       0,     0,     0,   230,     0,    97,    94,   356,   338,   341,
     339,   340,   349,   348,   350,     0,    93,    76,    75,    74,
       0,     0,     0,     0,   285,   288,     0,     0,   322,   309,
       0,   316,     0,   312,   308,   405,   411,   378,   107,   121,
     127,   126,   110,   125,   122,   137,   108,   138,   124,   128,
     109,   111,   123,   139,     0,   335,     0,   112,     0,   378,
     337,   378,   333,   647,     0,     0,     0,     0,     0,     0,
     275,     0,     0,     0,   583,   469,   631,   631,   206,   205,
       0,   305,     0,     0,   305,   305,   212,     0,     0,   553,
       0,   552,     0,   584,     0,     0,   559,   216,     0,   558,
     214,    71,    70,    69,   240,   236,   569,   242,     0,     0,
     272,   251,   248,   501,   382,     0,     0,   469,   413,   427,
     462,   492,     0,   665,   517,   517,     0,   296,     0,     0,
     227,   566,     0,     0,   225,   565,     0,   670,     0,     0,
       0,    79,     0,    79,    85,    88,   277,     0,     0,   286,
     299,   322,   172,   322,   298,   322,   306,   166,   315,   318,
     313,     0,   391,   332,     0,   336,     0,   328,   329,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   587,   305,
     322,   305,   305,   322,   322,   557,     0,     0,   586,   563,
       0,     0,     0,     0,   245,    61,   469,   501,     0,   504,
     503,   505,   610,   424,   387,   385,     0,     0,     0,     0,
     490,   610,     0,     0,   297,   295,     0,   228,     0,   226,
      95,    78,    77,     0,     0,     0,     0,   274,     0,   289,
     322,   168,   307,   483,     0,   417,     0,   334,   107,   109,
     378,   378,     0,     0,     0,     0,     0,     0,   322,   194,
     322,   322,   186,   190,     0,     0,     0,     0,    72,   244,
     388,   386,   469,   493,     0,   426,   425,   441,     0,   442,
     429,   432,     0,   428,   421,   422,   321,     0,   604,   605,
       0,     0,    87,    90,    86,    89,   287,   167,     0,   482,
     481,   610,   418,   427,   379,   330,   331,     0,     0,     0,
       0,     0,     0,   196,   188,   192,   556,   555,   562,   561,
     390,   389,   495,   496,   498,   610,     0,   665,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   610,   606,
     607,   568,   567,     0,   473,     0,     0,     0,   497,   499,
     431,   433,   434,   437,   438,   439,   435,   436,   430,   478,
     476,   610,   665,   419,   327,   420,   493,   477,   610,   500
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1092, -1092, -1092,     5,     3,  2281,   818, -1092,     2, -1092,
   -1092, -1092,   470, -1092, -1092, -1092,   496,   608,  -475, -1092,
    -763, -1092, -1092, -1092,   178, -1092, -1092, -1092,   940, -1092,
    2622,   -25,  -793, -1092,  -943,  1929, -1074,   456, -1092, -1092,
    -116,  -930, -1092,   -64, -1092, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092, -1092, -1092,   251, -1092,   883, -1092, -1092,    78,
    2198, -1092, -1092, -1092, -1092, -1092, -1092, -1092, -1092,    24,
   -1092, -1092, -1092, -1092, -1092, -1092,  -602,  -589, -1092, -1092,
   -1092,    15,  -766,   843, -1092, -1092, -1092,   369, -1092, -1092,
   -1092, -1092,  -120,  -790,  -122,  -737,  -958, -1092, -1092,  -113,
      86,   252, -1092, -1092, -1092,    25, -1092, -1092,  -285,     8,
   -1038,  -231,  -264,  -254,  -497, -1092,  -198, -1092,    -7,   963,
    -137,   493, -1092,  -491,  -872,  -862, -1092,  -705,  -523, -1003,
   -1091,  -980,   -42, -1092,   149, -1092,  -251,  -480,  -484,   643,
    -344, -1092, -1092, -1092,   926, -1092,    -9, -1092, -1092,  -226,
   -1092,  -631, -1092, -1092, -1092,   943,  1404,   -12,   971,    20,
    1636, -1092,  1860,  1998, -1092, -1092, -1092, -1092, -1092, -1092,
   -1092, -1092, -1092,  -419
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   338,  1085,   711,   112,   113,   114,   115,
     116,   117,   356,   491,   118,   252,   119,   339,   705,   562,
     706,   120,   121,   122,   559,   560,   123,   124,   206,   985,
     284,   125,   279,   126,   743,   289,   127,   291,   292,   128,
    1086,   129,   304,   130,   131,   132,   457,   652,   459,   654,
     451,   649,   133,   134,   828,   135,   250,   136,   719,   720,
     192,   138,   139,   140,   141,   142,   143,   565,   797,   956,
     144,   145,   759,   925,   146,   147,   567,   958,   148,   149,
     803,   804,   805,   193,   282,   734,   151,   152,   569,   966,
     810,   988,   989,   701,   702,   703,  1093,   498,   729,   730,
     731,   732,   733,   811,   367,   910,  1223,  1284,  1206,   445,
    1134,  1138,  1200,  1201,  1202,   153,   326,   530,   154,   155,
     285,   286,   502,   503,   747,  1220,  1165,   506,   744,  1243,
    1131,  1047,   340,   215,   344,   345,   446,   447,   448,   194,
     157,   158,   159,   160,   195,   162,   203,   204,   634,   470,
     871,   635,   636,   163,   164,   196,   197,   167,   236,   449,
     199,   169,   200,   201,   172,   173,   174,   175,   350,   176,
     177,   178,   179,   180
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     198,   305,   492,   700,   207,   110,   762,   109,   209,   735,
     208,   638,   437,   717,   216,   211,   474,   368,   987,   229,
     229,   990,   240,   242,   244,   247,   251,   308,  1087,   224,
     951,  1097,   913,  1098,   872,   961,   458,   290,   547,   293,
     294,  1050,   760,   504,   287,   299,   419,   300,   856,   301,
     758,   946,   517,   861,   287,   862,   874,   309,   313,   315,
     317,   318,   319,  1126,  1205,   348,   323,   867,   324,   287,
     527,   483,    70,   287,   438,   327,   585,   307,   329,   313,
     504,   331,   468,   332,    70,   831,   333,   334,  1190,   335,
     314,   627,   541,   309,   313,   349,   351,  1249,  1052,  1163,
     364,  -270,  1244,  1129,   -54,   468,   181,     3,   110,  1246,
     468,   330,   535,   307,   343,   307,  -271,   181,  -253,   542,
     365,   546,   550,   -54,  -293,  1250,   346,   832,   627,   488,
     366,   468,  1070,  1108,  1197,   507,   548,   545,   746,   433,
    -484,  1005,  1197,   468,   357,  1031,   827,  1192,   504,   428,
     433,  1161,   372,   202,  1240,  1087,  1269,  1283,   305,  -479,
     929,   745,   433,  1133,   854,   549,  -293,  -484,  1071,   615,
     616,  -484,   424,   425,   426,  1135,   589,   323,   309,   349,
    1087,   433,   511,  1087,  1087,   763,   519,   505,  -484,   548,
    -479,   433,  1205,   433,  1088,  -484,   548,  -484,   307,   307,
     436,   444,  1004,  -479,   433,  1049,   433,   548,   229,  1136,
     433,   358,  1225,  1226,   313,  -484,  -479,   226,   628,   442,
    -293,  1219,   899,   453,   505,  1053,   548,   375,   444,   461,
    1217,  1198,   253,   508,  1199,   922,  1247,    23,   211,   205,
     518,   313,  1199,  1164,  1268,   912,   638,   552,  1087,  -484,
    1087,  1087,   543,  1010,  1191,  1282,  1012,   653,  1013,   295,
    -254,   181,   440,  -479,   900,   379,  1137,   782,  -479,   721,
     833,  -260,  -484,   385,  -113,   519,   553,   923,   444,   544,
    -502,   441,  -119,  1289,   376,   239,   241,   243,   377,    60,
     722,   313,   505,  -484,   723,  -480,   444,   834,   521,   522,
      68,  -502,   865,   987,  -259,  -502,   990,   648,  1154,   313,
    1156,   897,  -484,   212,   648,   952,   651,   950,   724,   296,
    1241,   725,  1022,    62,   658,    88,  -480,   302,  -502,   648,
     540,   924,   726,  -268,   896,   379,   519,   297,   898,  -480,
     383,  1000,   110,   385,   537,   561,   563,   388,  -258,  1023,
    1003,   727,  -480,   226,   298,   520,  1076,   110,   347,   571,
     591,   572,   573,   574,   576,   578,   579,   648,   580,   581,
     582,   583,   584,  -589,   592,   593,   594,   595,   596,   597,
     598,   599,   600,   601,   602,   603,   604,   605,   606,   607,
     608,  1118,  -588,  -255,  1107,   444,   444,   313,   313,  -480,
    -589,  -472,  1280,   313,  -480,   508,   313,   313,   313,   313,
     618,   620,   538,  1045,   638,  -383,   963,   882,   637,  -588,
    1287,   660,  1110,   640,   570,  1113,  1114,    23,   644,   610,
     611,   645,  -472,   253,  -474,   612,   110,   539,   613,   614,
    -575,   964,  1150,   876,   879,  -472,   420,  -638,   421,  -638,
     794,   309,   585,   662,   664,   508,   375,   795,  -472,   668,
     670,   785,  -262,  -494,   673,  -474,   704,   704,   444,   586,
    1068,   307,   454,   712,   643,   789,   714,   303,  -474,    60,
     786,   313,  -266,   587,  -494,   508,  -263,  -257,  -494,   508,
      68,  -474,    23,   455,   790,   748,   456,   320,  -261,   293,
     728,   444,   753,   754,   857,  -472,   752,   757,   863,   210,
    -472,  -494,   588,   376,   313,    88,    42,   377,   489,   321,
    1178,   490,  1180,  1181,   798,   806,   757,   756,  -267,   313,
    1140,   313,  -269,  -115,   882,   774,   757,   777,  -474,   779,
     781,  -120,    58,  -474,    60,   288,   756,   930,   932,   343,
    -256,   343,   589,   883,   309,    68,   756,  -264,   590,  -450,
     110,   346,   537,   346,   379,   380,   927,   882,   288,   383,
     906,   384,   385,  -639,   307,  -639,   388,   807,  -450,   -83,
      88,  -485,   354,   882,   395,   882,  1074,  -638,   808,  -638,
     399,   400,   401,  -489,  -485,  1153,   422,  1155,   -54,   452,
     423,   444,  1212,   638,  1214,   460,  -489,   809,   355,   662,
     823,   668,   826,   712,   313,   829,   777,   -54,   359,  -635,
     848,  -635,   907,   444,   423,  -485,  -572,   575,   577,  -571,
     849,   850,   369,   908,   313,  -713,   313,  -489,   361,  -713,
    -714,   371,   370,  -491,  -714,   156,   -83,   864,   433,   866,
     494,  -485,   909,   637,   852,   873,  -491,  -715,  -485,   429,
    -637,  -715,  -637,  -489,   379,   331,   853,   332,   156,   383,
    -489,   156,   385,   617,   619,   430,   388,  -710,  -576,  -485,
     334,  -710,  -636,   335,  -636,  -577,   639,  -491,   433,   991,
     768,  -489,   770,  -574,  -573,   886,   887,   443,  -485,   432,
     915,  1248,  1132,  -485,   434,   439,  -485,   450,   463,  1141,
    -489,   493,   529,  -491,   497,  -489,   514,   663,  -489,   515,
    -491,   156,   523,   669,   526,   531,   534,   672,   319,   323,
     349,   554,   558,   568,   313,   288,   288,   288,   288,   288,
     288,  -491,   566,   609,   488,   626,   484,   487,   156,   385,
     307,   488,   646,   156,  1272,  1273,  1274,  1275,  1276,  1277,
    -491,  1251,   936,   591,   642,  -491,   917,   647,  -491,   648,
     444,   656,   375,   561,   659,   707,   495,   947,   708,   949,
     704,   704,   375,   313,   665,   156,   288,  1252,   288,  1253,
    1254,   710,   713,  1255,  1256,   288,   746,   716,   773,   737,
     736,   755,   761,  1142,  1143,   444,   764,   772,   778,   994,
     796,   799,   997,   288,   812,   820,   813,   814,   313,   819,
    1221,   637,   815,   816,   288,   288,   817,   818,   830,   376,
     859,   882,   885,   377,   888,   576,   618,   890,   895,   376,
     902,   903,   911,   377,   921,   150,   156,   519,   926,  1245,
     959,   965,   962,   993,   995,  1019,   998,  1021,  1001,  1011,
    1014,  1015,  1026,  1258,  1038,  1029,  1046,  1040,   150,  1048,
    1032,   150,  1033,   822,  1051,   825,  1054,  1055,  1062,  1036,
     379,  1058,  1066,  1075,  1078,   383,  1080,   384,   385,  1092,
     379,  -140,   388,  -147,  -146,   383,  -145,   384,   385,  -116,
     395,  -142,   388,  1245,  1109,   728,   399,   400,   401,  -114,
     395,   309,  -149,  -150,  -144,  -148,  -117,  1061,   401,  1065,
    1281,   150,  1067,  -143,   156,  -151,  1095,  -118,   161,  1096,
     156,   307,   288,   704,  1123,  1127,  1158,  1222,  1288,  1125,
    1111,  1112,  1245,  1157,  -113,   165,  1208,  -115,   150,  1213,
    1089,   161,  1259,   150,   161,  1209,  1215,  1260,  1224,  1250,
     360,   715,   557,   935,   213,  1279,   288,   750,   165,  1162,
     288,   165,  1094,   328,  1171,  1041,  1170,  1077,  1090,  1166,
     901,  1285,   156,  1019,  1021,   150,  1026,  1029,  1044,  1061,
    1065,  1091,  1266,  1278,   488,   488,  1271,   156,   488,   488,
     246,   749,   230,     0,   161,  1115,  1116,     0,  1117,     0,
     637,     0,  1119,  1120,     0,     0,  1121,     0,     0,   305,
       0,   165,     0,     0,   488,     0,   488,     0,     0,   444,
     444,   161,     0,     0,     0,     0,   161,     0,     0,     0,
    1139,   777,   313,   313,     0,     0,   150,  1146,   165,     0,
       0,  1148,   721,   165,  1130,     0,     0,  1152,   704,   704,
     704,   704,     0,     0,     0,     0,  1159,     0,   161,     0,
       0,     0,     0,   722,     0,     0,   156,   723,     0,     0,
       0,     0,     0,     0,     0,   165,  1194,  1115,  1172,  1173,
    1119,  1174,  1175,  1176,  1177,   728,     0,     0,   575,   617,
       0,   724,     0,     0,   725,     0,   352,     0,   156,     0,
    1188,     0,     0,     0,   156,   726,   349,     0,     0,     0,
       0,  1196,     0,     0,   150,  1203,   156,     0,   156,   161,
     150,     0,   375,     0,   727,     0,   307,     0,     0,     0,
       0,  1195,     0,     0,     0,     0,   165,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   288,
     288,     0,     0,     0,     0,   288,   288,     0,   288,   288,
       0,     0,  1236,  1237,  1238,  1239,  1265,     0,     0,     0,
       0,     0,   150,     0,     0,     0,     0,     0,   156,   376,
       0,     0,   156,   377,     0,  1065,     0,   150,  1261,  1262,
     156,  1242,     0,     0,     0,     0,   349,   161,     0,     0,
     894,     0,     0,   161,     0,  1236,  1237,  1238,  1239,  1261,
    1262,     0,     0,     0,   165,     0,   307,     0,     0,     0,
     165,     0,     0,     0,  1267,   777,     0,  1270,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,  1242,     0,   392,   393,   394,
     395,   396,     0,   398,     0,   161,   399,   400,   401,     0,
     777,     0,     0,     0,  1065,     0,   150,   402,     0,   846,
     161,     0,   165,     0,   218,     0,   721,     0,    22,    23,
       0,     0,   954,     0,  1242,     0,     0,   165,   219,     0,
      31,   220,     0,     0,     0,     0,    37,   722,   150,     0,
       0,   723,     0,    42,   150,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,   150,     0,   150,     0,
       0,     0,     0,     0,     0,   724,     0,     0,   725,    58,
      62,    60,     0,     0,     0,   288,   288,     0,     0,   726,
     223,     0,    68,   156,     0,     0,   156,     0,     0,   161,
     288,   375,   487,     0,     0,     0,     0,     0,   727,   487,
       0,    84,   288,     0,    86,   288,   165,    88,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,   150,     0,
       0,   161,   150,     0,  1042,     0,     0,   161,     0,     0,
     150,     0,     0,     0,     0,     0,   166,     0,   165,   161,
       0,   161,     0,     0,   165,   156,     0,     0,   376,     0,
       0,   156,   377,     0,     0,     0,   165,     0,   165,   166,
     379,   380,   166,   381,   382,   383,     0,   384,   385,   386,
       0,     0,   388,   389,   390,     0,     0,   392,   393,   394,
     395,     0,  1079,   398,   992,     0,   399,   400,   401,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,   379,
     380,   161,   381,   382,   383,   161,   384,   385,     0,     0,
       0,   388,   166,   161,     0,     0,     0,     0,   165,   395,
    1006,  1007,   165,     0,     0,   399,   400,   401,     0,     0,
     165,   254,     0,     0,     0,     0,     0,     0,     0,   166,
     156,     0,   156,     0,   166,   255,   256,   156,   257,     0,
     156,     0,     0,   258,     0,     0,     0,     0,     0,     0,
       0,   156,   259,   156,     0,     0,   156,     0,   260,     0,
       0,     0,     0,     0,   261,     0,   166,     0,   262,     0,
       0,   263,     0,   150,   288,     0,   150,     0,     0,     0,
       0,   264,     0,     0,     0,   156,     0,   375,   265,   266,
       0,     0,   156,     0,   156,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,     0,     0,
       0,     0,     0,     0,   269,   270,     0,   271,     0,   272,
       0,   273,     0,     0,   274,     0,     0,   166,   275,   499,
       0,   276,   487,   487,   277,   150,   487,   487,     0,     0,
       0,   150,     0,     0,   376,     0,     0,     0,   377,     0,
       0,     0,     0,     0,  1204,     0,   161,     0,   168,   161,
       0,     0,   487,     0,   487,     0,     0,     0,     0,     0,
       0,     0,     0,   165,  1216,     0,   165,     0,     0,     0,
     156,   168,     0,     0,   168,     0,     0,   156,     0,     0,
     500,     0,     0,     0,     0,   379,   380,     0,     0,   382,
     383,   156,   384,   385,     0,   166,     0,   388,     0,     0,
       0,   166,     0,     0,     0,   395,     0,     0,   161,   156,
       0,   399,   400,   401,   161,   156,     0,     0,     0,   156,
     150,     0,   150,     0,   168,   165,     0,   150,   218,     0,
     150,   165,    22,    23,     0,     0,     0,  1081,     0,     0,
       0,   150,   219,   150,    31,   220,   150,     0,   585,     0,
      37,   168,     0,   166,     0,     0,   168,    42,     0,     0,
       0,     0,   835,     0,     0,   836,     0,     0,   166,     0,
     837,     0,  1204,     0,     0,   150,   156,     0,     0,     0,
       0,   183,   150,    58,   150,    60,     0,   352,   168,  1082,
       0,   838,  1083,     0,   223,     0,    68,     0,   839,     0,
       0,     0,     0,   161,     0,   161,     0,     0,   840,     0,
     161,     0,     0,   161,     0,    84,   841,     0,    86,     0,
     165,    88,   165,     0,   161,     0,   161,   165,     0,   161,
     165,     0,   842,     0,     0,     0,     0,   156,   156,   156,
     156,   165,     0,   165,   843,     0,   165,   166,   589,   168,
       0,     0,     0,     0,     0,   844,     0,     0,   161,     0,
       0,   845,     0,   156,   156,   161,     0,   161,     0,     0,
     150,   103,   170,     0,     0,   165,     0,   150,     0,   166,
       0,     0,   165,     0,   165,   166,  1160,     0,     0,     0,
       0,   150,     0,     0,     0,   170,     0,   166,   170,   166,
     218,     0,     0,     0,    22,    23,     0,     0,     0,   150,
       0,     0,     0,     0,   219,   150,    31,   220,     0,   150,
       0,     0,    37,     0,     0,     0,     0,   168,     0,    42,
       0,     0,     0,   168,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,     0,
       0,     0,     0,   161,     0,    58,     0,    60,     0,   166,
     161,  1082,     0,   166,  1083,     0,   223,     0,    68,   375,
     165,   166,     0,     0,   161,   170,   150,   165,     0,     0,
     170,     0,     0,     0,     0,   168,     0,    84,     0,     0,
      86,   165,   161,    88,     0,     0,     0,     0,   161,     0,
     168,     0,   161,     0,     0,     0,     0,     0,     0,   165,
     171,   316,   170,     0,     0,   165,     0,     0,     0,   165,
       0,     0,     0,     0,     0,     0,   376,     0,     0,     0,
     377,     0,     0,   171,     0,     0,   171,   150,   150,   150,
     150,     0,     0,   103,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   161,
       0,     0,     0,   150,   150,     0,     0,     0,     0,     0,
       0,     0,     0,   170,     0,  -596,   165,   379,   380,   168,
     381,   382,   383,     0,   384,   385,   171,     0,   418,   388,
       0,  -635,     0,  -635,     0,     0,   394,   395,     0,     0,
     398,     0,     0,   399,   400,   401,     0,     0,     0,     0,
       0,   168,     0,   171,     0,     0,     0,   168,   171,     0,
     161,   161,   161,   161,   166,     0,     0,   166,     0,   168,
       0,   168,     0,     0,     0,     0,     0,   165,   165,   165,
     165,     0,     0,     0,     0,     0,   161,   161,     0,     0,
     171,   170,     0,     0,     0,     0,     0,   170,     0,     0,
       0,     0,     0,   165,   165,     0,     0,     0,     0,     0,
     217,     0,     0,     0,     0,     0,     0,     0,   464,   467,
     469,   473,   475,   477,   218,     0,   166,     0,    22,    23,
       0,   168,   166,     0,     0,   168,     0,     0,   219,     0,
      31,   220,     0,   168,     0,     0,    37,     0,     0,   170,
     137,   171,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,     0,     0,     0,   -52,   510,
       0,   512,     0,   137,     0,     0,   137,     0,   516,    58,
       0,    60,     0,     0,     0,   221,     0,   -52,   222,     0,
     223,     0,    68,     0,     0,     0,   525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   532,   533,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,   166,     0,   166,     0,     0,   137,     0,   166,   171,
       0,   166,     0,   111,     0,   171,     0,     0,     0,     0,
       0,     0,   166,   170,   166,     0,     0,   166,     0,     0,
       0,     0,     0,   137,     0,     0,   214,     0,   137,   225,
       0,     0,     0,     0,     0,     0,     0,   103,     0,     0,
       0,     0,     0,     0,     0,   170,   166,     0,     0,     0,
       0,   170,     0,   166,     0,   166,     0,   171,     0,     0,
       0,     0,     0,   170,     0,   170,   168,   957,     0,   168,
       0,     0,   171,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   641,     0,     0,  -305,     0,
       0,     0,  -305,  -305,     0,     0,     0,  -305,     0,     0,
       0,     0,  -305,     0,  -305,  -305,   111,     0,     0,     0,
    -305,   353,     0,     0,     0,     0,     0,  -305,     0,   666,
    -305,   137,     0,   671,     0,   170,     0,     0,   168,   170,
       0,     0,     0,     0,   168,     0,     0,   170,     0,     0,
    -305,   166,     0,  -305,     0,  -305,     0,  -305,   166,  -305,
    -305,   171,  -305,     0,  -305,     0,  -305,     0,     0,     0,
       0,     0,   166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -305,     0,     0,  -305,     0,
     166,  -305,     0,   171,     0,     0,   166,     0,     0,   171,
     166,     0,     0,     0,     0,     0,     0,     0,     0,   137,
       0,   171,     0,   171,   431,   137,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   168,     0,   168,     0,     0,     0,     0,
     168,  -305,     0,   168,     0,     0,     0,  -305,     0,     0,
       0,     0,     0,     0,   168,     0,   168,   166,     0,   168,
       0,     0,     0,     0,     0,     0,     0,   137,     0,     0,
       0,     0,     0,   171,     0,     0,     0,   171,     0,     0,
       0,     0,   137,     0,     0,   171,     0,     0,   168,     0,
       0,     0,   496,     0,     0,   168,     0,   168,   509,     0,
     170,     0,     0,   170,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   166,
     166,   166,   868,   870,     0,     0,     0,     0,   875,   878,
       0,   880,   881,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   166,     0,     0,     0,     0,
     111,     0,     0,     0,     0,     0,     0,     0,   254,     0,
       0,   137,   170,     0,     0,   111,     0,     0,   170,     0,
       0,     0,   255,   256,     0,   257,     0,     0,     0,     0,
     258,     0,     0,   168,     0,     0,     0,     0,     0,   259,
     168,     0,     0,   137,     0,   260,   278,   280,   281,   137,
       0,   261,     0,     0,   168,   262,     0,     0,   263,     0,
       0,   137,     0,   137,     0,     0,     0,     0,   264,     0,
       0,     0,   168,     0,     0,   265,   266,     0,   168,     0,
       0,     0,   168,   267,     0,     0,   325,     0,   171,     0,
       0,   171,   268,     0,   111,     0,     0,     0,     0,     0,
       0,   269,   270,     0,   271,     0,   272,   170,   273,   170,
       0,   274,     0,     0,   170,   275,     0,   170,   276,     0,
       0,   277,     0,   137,     0,     0,   509,   137,   170,     0,
     170,     0,   509,   170,     0,   137,     0,     0,     0,   168,
       0,     0,     0,     0,   709,     0,     0,     0,     0,   362,
     171,   363,     0,     0,     0,     0,   171,     0,  1008,  1009,
       0,     0,   170,     0,     0,     0,     0,     0,     0,   170,
       0,   170,     0,  1016,     0,   433,     0,   953,     0,     0,
       0,     0,     0,     0,     0,  1027,     0,     0,  1030,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,   168,   168,   168,     0,     0,   771,     0,     0,     0,
     776,     0,   325,     0,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,     0,     0,   168,   168,     0,     0,
       0,     0,     0,     0,     0,     0,   325,     0,     0,   462,
       0,     0,     0,     0,     0,   171,     0,   171,     0,     0,
     481,   482,   171,     0,     0,   171,     0,   170,     0,     0,
       0,     0,     0,     0,   170,     0,   171,     0,   171,     0,
       0,   171,     0,     0,     0,     0,     0,     0,   170,     0,
       0,     0,     0,     0,     0,   501,     0,     0,   137,     0,
       0,   137,     0,     0,     0,     0,   170,     0,   254,     0,
     171,     0,   170,     0,     0,     0,   170,   171,     0,   171,
       0,     0,   255,   256,     0,   257,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   261,     0,     0,     0,   262,     0,     0,   263,     0,
     137,     0,     0,     0,     0,     0,   137,     0,   264,     0,
       0,   564,     0,   170,     0,   265,   266,  1145,     0,     0,
       0,   889,     0,   267,   892,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,     0,     0,     0,     0,
       0,   269,   270,     0,   271,   171,   272,     0,   273,     0,
       0,   274,   171,     0,     0,   275,     0,     0,   276,     0,
       0,   277,     0,     0,     0,     0,   171,     0,     0,     0,
       0,     0,     0,     0,   170,   170,   170,   170,     0,     0,
       0,   629,     0,   928,   171,     0,     0,     0,     0,   933,
     171,     0,     0,     0,   171,   137,     0,   137,     0,     0,
     170,   170,   137,   650,     0,   137,     0,   325,     0,   655,
       0,   657,     0,     0,     0,   218,   137,   283,   137,    22,
      23,   137,     0,     0,  1081,     0,     0,     0,     0,   219,
       0,    31,   220,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,    42,     0,   718,     0,     0,     0,
     137,   171,   501,     0,     0,     0,     0,   137,     0,   137,
       0,     0,     0,     0,     0,   751,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,  1082,     0,   509,  1083,
     509,   223,     0,    68,     0,   509,     0,     0,   509,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1034,
       0,  1035,    84,     0,  1037,    86,     0,     0,    88,     0,
     784,     0,   171,   171,   171,   171,     0,     0,     0,     0,
     802,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   254,  1057,     0,     0,     0,     0,   171,   171,
    1060,     0,  1064,     0,     0,   137,   255,   256,     0,   257,
       0,     0,   137,     0,   258,     0,     0,     0,   103,     0,
       0,     0,     0,   259,  1084,     0,   137,     0,     0,   260,
       0,     0,     0,     0,     0,   261,     0,     0,     0,   262,
       0,     0,   263,     0,   137,     0,     0,     0,     0,     0,
     137,     0,   264,     0,   137,     0,     0,     0,     0,   265,
     266,   855,     0,     0,   858,     0,   860,   267,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,     0,     0,
       0,     0,     0,     0,     0,   269,   270,     0,   271,     0,
     272,     0,   273,     0,     0,   274,     0,     0,     0,   275,
       0,     0,   276,     0,     0,   277,     0,     0,     0,  1124,
       0,   137,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1144,     0,     0,
       0,     0,     0,  1147,     0,     0,     0,  1149,     0,     0,
       0,     0,     0,     0,   905,     0,     0,     0,   501,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   904,     0,   218,   918,     0,     0,    22,    23,     0,
       0,     0,   137,   137,   137,   137,   325,   219,     0,    31,
     220,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,     0,    42,   967,  1189,   968,     0,     0,   137,   137,
       0,     0,     0,     0,   674,     0,     0,     0,   955,   969,
     256,   675,   970,   802,     0,     0,     0,   971,    58,     0,
      60,     0,     0,   986,  1082,     0,   259,  1083,   183,   223,
       0,    68,   972,     0,     0,     0,     0,     0,   973,     0,
       0,     0,   262,     0,     0,   974,     0,   676,     0,     0,
      84,     0,     0,    86,     0,   975,    88,     0,     0,     0,
       0,     0,   976,   977,     0,     0,     0,     0,     0,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,   978,
       0,     0,     0,     0,     0,     0,     0,     0,   269,   270,
       0,   979,     0,   272,     0,   980,     0,     0,   981,     0,
       0,     0,   982,     0,     0,   276,   103,  1039,   983,   718,
       0,     0,     0,  1043,     0,     0,   501,   486,   404,   405,
     406,   407,   408,     0,     0,   411,   412,   413,   414,     0,
     416,   417,   937,   938,   939,   940,   941,   682,     0,   683,
       0,     0,     0,   684,   685,   686,   687,   688,   689,   690,
     691,   942,   693,   694,     0,   943,     0,     0,   696,   697,
     944,   699,     0,     0,   984,   955,     0,     0,     0,     0,
       0,     0,   802,    -2,     4,   802,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   986,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
    -665,     0,    12,    13,    14,    15,    16,  -665,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    -665,    28,    29,  -665,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -322,   -52,    65,    66,    67,  -665,    68,    69,
      70,  -665,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,  -665,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -665,  -665,
      95,  -665,  -665,  -665,  -665,  -665,  -665,  -665,     0,  -665,
    -665,  -665,  -665,  -665,     0,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,   103,  -665,  -665,  -665,     0,   105,  -665,
     106,     0,   107,     0,   336,  -665,     5,   306,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   337,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -322,   -52,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,   536,
     106,     0,   107,     0,   555,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   556,   106,     0,   107,     0,   336,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -322,   -52,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,   337,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   783,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,   352,
      63,    64,  -322,   -52,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,   661,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -322,   -52,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,   667,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1018,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -322,   -52,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1020,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1025,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -322,   -52,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1028,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,  1056,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -322,   -52,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1059,    39,
     -81,     0,    40,    41,    42,     0,    43,  -322,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -52,
      53,    54,     0,    55,    56,    57,     0,  -322,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -322,   -52,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -81,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1063,    39,   -81,     0,    40,    41,    42,
       0,    43,  -322,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -52,    53,    54,     0,    55,    56,
      57,     0,  -322,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -322,   -52,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -81,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,  1263,   108,     5,   306,     6,     7,
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
       0,   104,     0,   190,     0,   106,     0,   191,  1264,   914,
     108,     5,   306,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   183,     0,     0,     0,    33,   184,
     185,     0,     0,   186,    39,     0,     0,     0,    41,     0,
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
      10,    11,     0,   674,     0,    12,     0,     0,    15,    16,
     675,    17,     0,   182,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,   186,    39,     0,     0,
       0,    41,     0,     0,    43,     0,   676,   187,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   188,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   189,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,   486,   404,   405,   406,
     407,   408,     0,     0,   411,   412,   413,   414,     0,   416,
     417,   677,   678,   679,   680,   681,   682,     0,   683,     0,
      98,     0,   684,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   100,   695,   102,     0,   696,   697,   698,
     699,   190,     0,   106,     0,   191,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
       0,    15,    16,     0,    17,     0,   182,    19,    20,    21,
       0,     0,     0,     0,    26,     0,     0,    28,    29,     0,
     183,     0,     0,     0,    33,    34,    35,    36,     0,    38,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     187,     0,     0,    47,    48,     0,    49,    50,    51,    52,
       0,    53,    54,     0,    55,    56,    57,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   188,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,    90,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,   245,    23,     0,     0,     0,     0,     0,
       0,    29,     0,   183,     0,     0,     0,    33,   184,   185,
       0,     0,   186,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   187,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,    60,    61,     0,    63,
       0,     0,     0,     0,    66,   188,     0,    68,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   189,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,    88,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
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
       0,    98,   342,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   190,     0,   106,     0,   191,     0,     0,   108,
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
     767,   191,     0,     0,   108,     5,   306,     6,     7,     8,
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
     104,     0,   190,     0,   106,   769,   191,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,   630,     0,     0,     0,     0,     0,     0,    29,
       0,   183,     0,     0,     0,    33,   184,   185,     0,     0,
     186,    39,     0,   631,     0,    41,     0,     0,    43,     0,
       0,   187,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   188,     0,   632,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   189,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     633,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   190,     0,   106,     0,   191,
       0,     5,   108,     6,     7,     8,   231,    10,    11,   232,
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
      99,     0,     0,   100,   101,   102,   312,     0,     0,   104,
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
       0,     0,    81,   310,   311,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   190,     0,   106,     0,
     191,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,   186,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   187,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,   916,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   188,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     189,     0,    79,     0,     0,    81,   310,   311,     0,    85,
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
       0,     0,    33,   184,   185,     0,     0,   186,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   187,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,   237,    61,     0,    63,     0,     0,     0,     0,    66,
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
      21,     0,   248,     0,     0,     0,     0,     0,     0,    29,
       0,   183,     0,     0,     0,    33,   184,   185,     0,     0,
     186,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   187,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   188,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   189,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     249,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   190,     0,   106,     0,   191,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   183,     0,     0,     0,    33,   184,
     185,     0,     0,   186,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   187,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   188,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   189,
       0,    79,     0,     0,    81,   310,   311,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   190,     0,
     106,     0,   191,     0,     0,   108,     5,   306,     6,     7,
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
       0,     0,     0,     0,     0,   238,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   190,     0,   106,     0,   191,     0,     5,
     108,     6,     7,     8,   231,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   233,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   189,     0,    79,
       0,     0,    81,     0,     0,   234,    85,     0,   235,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   190,     0,   106,     0,
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
       0,   190,   765,   106,     0,   191,     0,     0,   108,     5,
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
     191,   775,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,   186,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   187,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   188,  -709,     0,
       0,     0,  -709,     0,     0,    73,    74,    75,    76,    77,
     189,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   190,
       0,   106,     0,   191,     0,     0,   108,     5,   306,     6,
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
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,   191,  1193,
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
      87,     0,     0,    89,     0,     0,     0,     0,     0,   238,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   190,     0,   106,
       0,   191,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   287,    29,     0,   183,     0,     0,     0,
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
     105,     0,   106,     0,   191,     0,     0,   108,     5,   306,
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
     185,     0,     0,   186,    39,     0,     0,     0,    41,     0,
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
     106,     0,   191,   780,     5,   108,     6,     7,     8,     9,
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
       0,    85,   948,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   190,     0,   106,     0,   191,     0,     0,   108,     5,
     306,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     738,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,   739,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   188,     0,     0,     0,     0,     0,
       0,     0,    73,   740,    75,    76,    77,   741,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   190,     0,   106,     0,
    1128,     0,     5,   108,     6,     7,     8,     9,    10,    11,
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
    1151,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   190,
       0,   106,     0,   191,     0,     0,   108,     5,   306,     6,
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
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,  1128,     0,
       0,   108,     5,   306,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,  1106,    39,     0,     0,     0,    41,
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
       0,     0,    33,   184,   185,     0,     0,   186,    39,     0,
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
       6,     7,     8,   227,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   182,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   183,     0,     0,     0,    33,   184,   185,     0,     0,
     186,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   187,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   228,     0,     0,     0,     0,     0,     0,
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
       0,     0,   738,     0,   183,     0,     0,     0,    33,   184,
     185,     0,     0,   186,    39,     0,     0,     0,   739,     0,
       0,    43,     0,     0,   187,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   188,     0,     0,     0,
       0,     0,     0,     0,    73,   740,    75,    76,    77,   741,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   190,     0,
     106,     0,   742,     0,     5,   108,     6,     7,     8,     9,
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
       0,   190,     0,   106,     0,   742,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     183,     0,     0,     0,    33,   184,   185,     0,     0,   821,
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
       0,     0,   824,    39,     0,     0,     0,    41,     0,     0,
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
      33,   184,   185,     0,     0,  1100,    39,     0,     0,     0,
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
       0,     0,     0,    33,   184,   185,     0,     0,  1101,    39,
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
       0,   104,     0,   190,     0,   106,     0,   191,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,  1103,    39,     0,     0,     0,    41,     0,     0,    43,
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
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,  1104,    39,     0,     0,     0,    41,
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
       0,     0,    33,   184,   185,     0,     0,  1105,    39,     0,
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
    1106,    39,     0,     0,     0,    41,     0,     0,    43,     0,
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
       0,     0,   738,     0,   183,     0,     0,     0,    33,   184,
     185,     0,     0,   186,    39,     0,     0,     0,   739,     0,
       0,    43,     0,     0,   187,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   188,     0,     0,     0,
       0,     0,     0,     0,    73,   740,    75,    76,    77,   741,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   190,     0,
     106,     0,  1218,     0,     5,   108,     6,     7,     8,     9,
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
       0,     0,     0,   106,     0,  1218,     0,     5,   108,     6,
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
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,     0,   101,   102,     0,
       0,     0,   104,     0,   190,     0,   106,     0,   191,     5,
     306,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   182,     0,
     375,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   183,     0,     0,     0,    33,   184,   185,     0,
       0,   186,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   187,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,   376,    63,     0,
       0,   377,     0,    66,   188,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   189,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,  -493,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,    95,     0,
     388,     0,  -493,     0,     0,     0,  -493,   394,   395,     0,
       0,   398,     0,     0,   399,   400,   401,     0,     0,     0,
     102,     0,     0,     0,     0,   402,   190,     0,   106,  -493,
    1128,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   183,     0,     0,     0,    33,   184,
     185,     0,     0,   186,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   187,     0,     0,    47,    48,  -475,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   188,     0,     0,     0,
    -475,     0,     0,     0,    73,    74,    75,    76,    77,   189,
       0,    79,     0,  -475,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,  -475,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   182,   102,  -475,    21,     0,     0,     0,  -475,     0,
     106,     0,  1218,    29,     0,   183,     0,     0,     0,    33,
     184,   185,     0,     0,   186,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   187,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   188,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     189,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,    95,     0,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   182,   102,     0,    21,     0,     0,     0,   190,
       0,   106,     0,   191,    29,     0,   183,     0,     0,     0,
      33,   184,   185,     0,     0,   186,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   187,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   188,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   189,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   182,   102,     0,    21,     0,     0,     0,
     190,     0,   106,     0,   742,    29,     0,   183,     0,     0,
       0,    33,   184,   185,     0,     0,   186,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   187,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,   585,     0,    59,
       0,    61,     0,    63,     0,     0,   674,     0,    66,   188,
       0,     0,     0,   675,   586,     0,     0,    73,    74,    75,
      76,    77,   189,     0,    79,     0,     0,    81,   587,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   676,
       0,     0,     0,     0,     0,     0,     0,   588,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,     0,   191,     0,   589,     0,     0,
       0,     0,     0,   590,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   486,
     404,   405,   406,   407,   408,     0,     0,   411,   412,   413,
     414,     0,   416,   417,   937,   938,   939,   940,   941,   682,
       0,   683,     0,     0,     0,   684,   685,   686,   687,   688,
     689,   690,   691,   942,   693,   694,  1168,   943,     0,     0,
     696,   697,   944,   699,   945,   674,     0,     0,     0,     0,
     255,   256,   675,   257,     0,     0,     0,     0,   258,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,   972,     0,     0,     0,     0,     0,   261,
       0,     0,     0,   262,     0,     0,   263,     0,   676,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,   976,   266,     0,     0,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
     268,     0,     0,     0,     0,     0,     0,     0,   373,   269,
     270,     0,   271,     0,   272,   374,  1169,     0,     0,   981,
       0,     0,     0,   275,     0,     0,   276,     0,   375,   277,
       0,     0,     0,     0,     0,     0,     0,     0,   486,   404,
     405,   406,   407,   408,     0,     0,   411,   412,   413,   414,
       0,   416,   417,   937,   938,   939,   940,   941,   682,     0,
     683,     0,     0,     0,   684,   685,   686,   687,   688,   689,
     690,   691,   942,   693,   694,     0,   943,     0,     0,   696,
     697,   944,   699,     0,   373,   376,     0,     0,     0,   377,
       0,   374,     0,     0,     0,     0,     0,     0,   485,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   486,   404,   405,   406,   407,   408,     0,     0,   411,
     412,   413,   414,     0,   416,   417,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     373,   376,   399,   400,   401,   377,   433,   374,     0,     0,
       0,     0,     0,   402,  1017,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,   486,   404,   405,
     406,   407,   408,     0,     0,   411,   412,   413,   414,     0,
     416,   417,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   373,   376,   399,   400,
     401,   377,   433,   374,     0,     0,     0,     0,     0,   402,
    1024,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   486,   404,   405,   406,   407,   408,     0,
       0,   411,   412,   413,   414,     0,   416,   417,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,   373,   376,   399,   400,   401,   377,   433,   374,
       0,     0,     0,     0,     0,   402,  1184,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,   486,
     404,   405,   406,   407,   408,     0,     0,   411,   412,   413,
     414,     0,   416,   417,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   373,   376,
     399,   400,   401,   377,   433,   374,     0,     0,     0,     0,
       0,   402,  1185,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,   486,   404,   405,   406,   407,
     408,     0,     0,   411,   412,   413,   414,     0,   416,   417,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,   373,   376,   399,   400,   401,   377,
     433,   374,     0,     0,     0,     0,     0,   402,  1186,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   486,   404,   405,   406,   407,   408,     0,     0,   411,
     412,   413,   414,     0,   416,   417,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     373,   376,   399,   400,   401,   377,   433,   374,     0,     0,
       0,     0,     0,   402,  1187,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,   486,   404,   405,
     406,   407,   408,     0,     0,   411,   412,   413,   414,     0,
     416,   417,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   373,   376,   399,   400,
     401,   377,   433,   374,     0,     0,     0,     0,     0,   402,
    1210,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,   486,   404,   405,   406,   407,   408,     0,
       0,   411,   412,   413,   414,     0,   416,   417,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,   376,   399,   400,   401,   377,   433,     0,
       0,     0,     0,     0,     0,   402,  1211,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,   486,
     404,   405,   406,   407,   408,     0,     0,   411,   412,   413,
     414,     0,   416,   417,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   373,     0,
     399,   400,   401,     0,   433,   374,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,   375,   471,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,     0,     0,     0,   373,     0,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,   465,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   466,   376,     0,     0,   373,   377,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   375,   287,
       0,     0,     0,     0,     0,     0,   468,     0,     0,     0,
     378,     0,     0,     0,   476,   376,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     378,     0,   399,   400,   401,   376,   433,     0,     0,   377,
       0,     0,     0,   402,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     378,     0,   399,   400,   401,     0,   433,     0,     0,     0,
       0,     0,     0,   402,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     373,     0,   399,   400,   401,     0,   433,   374,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,   478,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     373,   791,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,   479,     0,     0,     0,   792,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,   373,     0,     0,     0,     0,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,   793,     0,     0,     0,   375,   877,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,   378,   480,   399,   400,   401,     0,     0,     0,
       0,     0,   376,     0,     0,   402,   377,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,   468,   399,   400,   401,   378,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   373,     0,   399,
     400,   401,     0,   433,   374,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   435,     0,     0,     0,   373,     0,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   287,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,     0,   373,   377,     0,
       0,     0,     0,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   621,     0,
       0,     0,     0,     0,     0,   202,     0,     0,     0,   378,
       0,     0,     0,   622,   376,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   378,
       0,   399,   400,   401,   376,     0,     0,     0,   377,     0,
       0,     0,   402,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   378,
       0,   399,   400,   401,     0,   433,     0,     0,     0,     0,
       0,     0,   402,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     218,   399,   400,   401,    22,    23,   374,     0,     0,     0,
       0,     0,   402,     0,   219,     0,    31,   220,     0,   375,
     623,     0,    37,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,     0,   624,     0,     0,     0,   373,
       0,     0,     0,     0,   -52,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,   375,
     869,   221,     0,   -52,   222,     0,   223,     0,    68,     0,
       0,     0,     0,     0,     0,     0,   376,     0,     0,   373,
     377,     0,     0,     0,     0,     0,   374,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,   376,     0,     0,     0,
     377,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,   103,   392,   393,   394,   395,   396,   397,
     398,   378,     0,   399,   400,   401,   376,     0,     0,     0,
     377,     0,     0,     0,   402,     0,     0,   379,   380,   893,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,   378,     0,   399,   400,   401,     0,   433,     0,     0,
       0,     0,     0,     0,   402,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,   373,     0,   399,   400,   401,     0,   433,   374,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   218,     0,     0,     0,    22,    23,     0,
       0,   373,     0,     0,     0,     0,     0,   219,   374,    31,
     220,     0,     0,     0,     0,    37,     0,     0,   -82,     0,
       0,   375,    42,     0,     0,     0,  1072,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -53,   376,     0,
       0,     0,   377,     0,     0,     0,   373,   513,    58,     0,
      60,     0,     0,   374,     0,     0,   -53,     0,     0,   223,
       0,    68,  1073,     0,     0,     0,   375,     0,     0,   468,
       0,     0,     0,   378,     0,     0,     0,     0,   376,     0,
      84,     0,   377,    86,     0,   -82,    88,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,   378,     0,   399,   400,   401,     0,     0,
       0,     0,     0,   376,   931,     0,   402,   377,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,   373,     0,   399,   400,   401,   378,     0,
     374,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,   375,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   373,     0,
     399,   400,   401,     0,     0,   374,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   376,     0,     0,     0,   377,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,     0,   399,   400,   401,
     378,   524,     0,     0,     0,     0,     0,     0,   402,     0,
       0,   376,     0,     0,     0,   377,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
     373,   528,   399,   400,   401,   551,   378,   374,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
     375,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   373,     0,   399,   400,
     401,     0,     0,   374,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,   377,     0,     0,     0,     0,   373,   787,     0,     0,
     625,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   376,     0,     0,     0,   377,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,     0,     0,   399,   400,   401,     0,   378,     0,
       0,     0,     0,   376,     0,   402,     0,   377,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   378,     0,
     399,   400,   401,     0,     0,     0,     0,   766,     0,     0,
       0,   402,     0,     0,   379,   380,     0,   381,   382,   383,
     375,   384,   788,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   373,     0,
     399,   400,   401,     0,     0,   374,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,   377,     0,   373,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,   376,     0,     0,     0,   377,
       0,     0,     0,   373,     0,     0,     0,     0,   379,   380,
     374,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   375,     0,   392,   393,   394,   395,   396,
     378,   398,     0,     0,   399,   400,   401,     0,     0,     0,
     376,     0,     0,     0,   377,   402,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,  -597,   392,   393,   394,   395,   396,   397,   398,
       0,   202,   399,   400,   401,   378,     0,     0,     0,     0,
     376,     0,     0,   402,   377,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   378,     0,   399,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,   851,   399,   400,   401,
     373,   884,     0,     0,     0,     0,     0,   374,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,   218,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
     373,     0,     0,   219,     0,    31,   220,   374,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
     375,     0,     0,   891,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -53,     0,     0,     0,   376,     0,     0,
     373,   377,     0,     0,    58,     0,    60,   374,     0,     0,
       0,     0,   -53,     0,     0,   223,     0,    68,     0,     0,
     375,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,    84,   376,     0,    86,
       0,   377,    88,     0,     0,     0,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,   378,     0,   399,   400,   401,   376,     0,     0,
       0,   377,     0,     0,     0,   402,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,   378,     0,   399,   400,   401,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,   379,   380,
       0,   381,   382,   383,     0,   384,   385,   386,     0,   387,
     388,   389,   390,   391,     0,   392,   393,   394,   395,   396,
     397,   398,   373,     0,   399,   400,   401,     0,     0,   374,
       0,   919,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   373,     0,     0,     0,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   376,
       0,     0,     0,   377,   373,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,   996,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,     0,     0,
     376,     0,     0,     0,   377,     0,     0,     0,     0,     0,
     379,   380,     0,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   397,   398,   934,   378,   399,   400,   401,     0,
       0,   376,     0,   920,     0,   377,     0,   402,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,     0,   378,   399,   400,   401,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,   373,     0,   399,   400,
     401,     0,     0,   374,     0,     0,     0,     0,     0,   402,
     218,     0,     0,     0,    22,    23,   375,   999,     0,  1081,
       0,     0,     0,     0,   219,     0,    31,   220,     0,     0,
       0,     0,    37,     0,     0,     0,   373,     0,     0,    42,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,  1082,     0,   376,  1083,     0,   223,   377,    68,     0,
       0,   373,  1069,     0,     0,     0,     0,     0,   374,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
      86,   375,     0,    88,     0,     0,     0,     0,   378,     0,
       0,     0,     0,   376,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   378,     0,
     399,   400,   401,   103,     0,     0,     0,     0,   376,  1179,
       0,   402,   377,     0,   379,   380,     0,   381,   382,   383,
       0,   384,   385,   386,     0,   387,   388,   389,   390,   391,
       0,   392,   393,   394,   395,   396,   397,   398,   373,     0,
     399,   400,   401,   378,     0,   374,     0,     0,     0,  1002,
       0,   402,     0,     0,     0,     0,     0,     0,   375,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   373,   376,     0,     0,     0,   377,
       0,   374,     0,     0,     0,     0,     0,     0,  1099,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   379,   380,     0,   381,
     382,   383,     0,   384,   385,   386,     0,   387,   388,   389,
     390,   391,     0,   392,   393,   394,   395,   396,   397,   398,
       0,   376,   399,   400,   401,   377,     0,     0,     0,   373,
    1122,     0,     0,   402,  1102,     0,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,   378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   379,   380,     0,   381,   382,   383,     0,   384,
     385,   386,     0,   387,   388,   389,   390,   391,     0,   392,
     393,   394,   395,   396,   397,   398,     0,     0,   399,   400,
     401,     0,     0,     0,     0,   373,   376,     0,     0,   402,
     377,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,   373,   376,   399,   400,   401,   377,     0,   374,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   373,   376,   399,
     400,   401,   377,     0,   374,     0,  1167,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,   373,   376,   399,   400,   401,   377,     0,
     374,     0,     0,     0,  1207,     0,   402,  1227,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   379,   380,     0,   381,   382,
     383,     0,   384,   385,   386,     0,   387,   388,   389,   390,
     391,     0,   392,   393,   394,   395,   396,   397,   398,   373,
     376,   399,   400,   401,   377,     0,   374,     0,     0,     0,
       0,     0,   402,  1228,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   379,   380,     0,   381,   382,   383,     0,   384,   385,
     386,     0,   387,   388,   389,   390,   391,     0,   392,   393,
     394,   395,   396,   397,   398,   373,   376,   399,   400,   401,
     377,     0,   374,     0,     0,     0,     0,     0,   402,  1229,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   379,   380,     0,
     381,   382,   383,     0,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,   373,   376,   399,   400,   401,   377,     0,   374,     0,
       0,     0,     0,     0,   402,  1230,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,   373,   376,   399,
     400,   401,   377,     0,   374,     0,     0,     0,     0,     0,
     402,  1231,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     380,     0,   381,   382,   383,     0,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,   373,   376,   399,   400,   401,   377,     0,
     374,     0,     0,     0,     0,     0,   402,  1232,     0,     0,
       0,     0,     0,   375,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1257,     0,     0,   379,   380,     0,   381,   382,
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
     377,     0,   374,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,   375,     0,     0,     0,     0,
       0,     0,     0,     0,   218,     0,     0,     0,    22,    23,
       0,   378,     0,  1081,     0,     0,     0,     0,   219,     0,
      31,   220,     0,     0,     0,     0,    37,   379,   380,     0,
     381,   382,   383,    42,   384,   385,   386,     0,   387,   388,
     389,   390,   391,     0,   392,   393,   394,   395,   396,   397,
     398,   373,   376,   399,   400,   401,   377,     0,   374,    58,
       0,    60,  1286,    62,   402,  1082,     0,     0,  1083,     0,
     223,   375,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,   379,   380,     0,   381,   382,   383,     0,
     384,   385,   386,     0,   387,   388,   389,   390,   391,     0,
     392,   393,   394,   395,   396,   397,   398,     0,   847,   399,
     400,   401,   377,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,   103,     0,     0,
       0,     0,     0,  1182,     0,     0,   218,     0,     0,     0,
      22,    23,     0,   378,     0,  1081,     0,     0,     0,     0,
     219,     0,    31,   220,     0,     0,     0,     0,    37,   379,
     380,     0,   381,   382,   383,    42,   384,   385,   386,     0,
     387,   388,   389,   390,   391,     0,   392,   393,   394,   395,
     396,   397,   398,     0,     0,   399,   400,   401,     0,     0,
       0,    58,     0,    60,     0,    62,   402,  1082,     0,     0,
    1083,     0,   223,   218,    68,     0,     0,    22,    23,     0,
       0,     0,  1081,     0,     0,     0,     0,   219,     0,    31,
     220,     0,     0,    84,     0,    37,    86,     0,     0,    88,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,  1082,     0,     0,  1083,     0,   223,
       0,    68,   218,     0,     0,     0,    22,    23,     0,   103,
       0,  1081,     0,     0,     0,  1183,   219,     0,    31,   220,
      84,     0,     0,    86,    37,     0,    88,     0,     0,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,  1082,     0,     0,  1083,     0,   223,   218,
      68,     0,     0,    22,    23,     0,   103,     0,  1081,     0,
       0,     0,  1233,   219,     0,    31,   220,     0,     0,    84,
       0,    37,    86,     0,     0,    88,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   376,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
    1082,     0,     0,  1083,     0,   223,     0,    68,     0,     0,
       0,     0,     0,     0,   378,   103,     0,     0,     0,     0,
       0,  1234,     0,     0,     0,     0,    84,     0,     0,    86,
     379,   380,    88,   381,   382,   383,     0,   384,   385,   386,
       0,   387,   388,   389,   390,     0,     0,   392,   393,   394,
     395,   396,   397,   398,     0,     0,   399,   400,   401,   800,
       0,   254,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,   255,   256,     0,   257,     0,
       0,     0,   103,   258,     0,     0,     0,     0,  1235,   801,
       0,     0,   259,     0,     0,     0,     0,     0,   260,     0,
       0,     0,     0,     0,   261,     0,     0,     0,   262,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,   265,   266,
       0,     0,     0,     0,     0,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,   254,     0,
       0,     0,     0,     0,   269,   270,     0,   271,     0,   272,
       0,   273,   255,   256,   274,   257,     0,     0,   275,     0,
     258,   276,    23,     0,   277,     0,     0,     0,     0,   259,
       0,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   261,     0,     0,     0,   262,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,   265,   266,     0,     0,     0,
       0,     0,     0,   267,    60,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,    68,     0,     0,     0,     0,
       0,   269,   270,     0,   271,     0,   272,     0,   273,   254,
       0,   274,     0,     0,   960,   275,     0,     0,   276,     0,
      88,   277,     0,   255,   256,     0,   257,     0,     0,     0,
       0,   258,     0,     0,     0,     0,     0,     0,     0,     0,
     259,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,     0,   261,     0,     0,     0,   262,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,   265,   266,     0,     0,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,     0,   254,     0,     0,     0,
       0,     0,   269,   270,     0,   271,     0,   272,     0,   273,
     255,   256,   274,   257,     0,     0,   275,     0,   258,   276,
       0,     0,   277,     0,   801,     0,     0,   259,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,     0,   261,
       0,     0,     0,   262,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,   265,   266,     0,     0,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
     268,     0,     0,  -300,     0,     0,     0,     0,     0,   269,
     270,     0,   271,     0,   272,     0,   273,  -300,  -300,   274,
    -300,     0,     0,   275,     0,  -300,   276,     0,     0,   277,
       0,     0,     0,     0,  -300,     0,     0,     0,     0,     0,
    -300,     0,     0,     0,     0,     0,  -300,     0,     0,     0,
    -300,     0,     0,  -300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -300,     0,     0,     0,     0,     0,     0,
    -300,  -300,     0,     0,     0,     0,     0,     0,  -300,     0,
       0,     0,     0,     0,     0,     0,     0,  -300,     0,     0,
    -310,     0,     0,     0,     0,     0,  -300,  -300,     0,  -300,
       0,  -300,     0,  -300,  -310,  -310,  -300,  -310,     0,     0,
    -300,     0,  -310,  -300,     0,     0,  -300,     0,     0,     0,
       0,  -310,     0,     0,     0,     0,     0,  -310,     0,     0,
       0,     0,     0,  -310,     0,     0,     0,  -310,     0,     0,
    -310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -310,     0,     0,     0,     0,     0,     0,  -310,  -310,     0,
       0,     0,     0,     0,     0,  -310,     0,     0,     0,     0,
       0,     0,     0,     0,  -310,     0,     0,   254,     0,     0,
       0,     0,     0,  -310,  -310,     0,  -310,     0,  -310,     0,
    -310,   255,   256,  -310,   257,     0,     0,  -310,     0,   258,
    -310,     0,     0,  -310,     0,     0,     0,     0,   259,     0,
       0,     0,     0,     0,   260,     0,     0,     0,     0,     0,
     261,     0,     0,     0,   262,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,   265,   266,     0,     0,     0,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,   268,     0,     0,  -301,     0,     0,     0,     0,     0,
     269,   270,     0,   271,     0,   272,     0,   273,  -301,  -301,
     274,  -301,     0,     0,   275,     0,  -301,   276,     0,     0,
     277,     0,     0,     0,     0,  -301,     0,     0,     0,     0,
       0,  -301,     0,     0,     0,     0,     0,  -301,     0,     0,
       0,  -301,     0,     0,  -301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -301,     0,     0,     0,     0,     0,
       0,  -301,  -301,     0,     0,     0,     0,     0,     0,  -301,
       0,     0,     0,     0,     0,     0,     0,     0,  -301,     0,
       0,  -193,     0,     0,     0,     0,     0,  -301,  -301,     0,
    -301,     0,  -301,     0,  -301,  -193,  -193,  -301,  -193,     0,
       0,  -301,     0,  -193,  -301,     0,     0,  -301,     0,     0,
       0,     0,  -193,     0,     0,     0,     0,     0,  -193,     0,
       0,     0,     0,     0,  -193,     0,     0,     0,  -193,     0,
       0,  -193,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -193,     0,     0,     0,     0,     0,     0,  -193,  -193,
       0,     0,     0,     0,     0,     0,  -193,     0,     0,     0,
       0,     0,     0,     0,     0,  -193,     0,     0,  -185,     0,
       0,     0,     0,     0,  -193,  -193,     0,  -193,     0,  -193,
       0,  -193,  -185,  -185,  -193,  -185,     0,     0,  -193,     0,
    -185,  -193,     0,     0,  -193,     0,     0,     0,     0,  -185,
       0,     0,     0,     0,     0,  -185,     0,     0,     0,     0,
       0,  -185,     0,     0,     0,  -185,     0,     0,  -185,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -185,     0,
       0,     0,     0,     0,     0,  -185,  -185,     0,     0,     0,
       0,     0,     0,  -185,     0,     0,     0,     0,     0,     0,
       0,     0,  -185,     0,     0,     0,     0,     0,     0,     0,
       0,  -185,  -185,     0,  -185,     0,  -185,     0,  -185,     0,
       0,  -185,     0,     0,     0,  -185,     0,     0,  -185,     0,
       0,  -185
};

static const yytype_int16 yycheck[] =
{
      12,    65,   253,   478,    16,     2,   529,     2,    20,   500,
      19,   430,   210,   493,    26,    22,   242,   154,   811,    31,
      32,   811,    34,    35,    36,    37,    38,    69,   958,    27,
     793,   989,   737,   991,   665,   801,   234,    49,    48,    51,
      52,   913,   526,     1,    33,    57,   168,    59,   650,    61,
       1,   788,   303,   655,    33,   657,    33,    69,    70,    71,
      72,    73,    74,  1043,  1138,   107,    78,    33,    80,    33,
     321,   102,    92,    33,   211,    87,     3,    69,    90,    91,
       1,    93,   120,    95,    92,    48,    98,    99,  1126,   101,
      70,    48,   146,   105,   106,   107,   108,   118,    48,     1,
      54,    56,  1193,  1046,    66,   120,   165,     0,   105,    48,
     120,    91,   171,   105,   106,   107,    56,   165,    56,   173,
      74,   347,   348,    85,    75,   146,   106,    90,    48,   251,
      84,   120,    37,  1005,     3,    48,   146,   175,   125,   170,
      61,   846,     3,   120,    47,   882,   626,  1127,     1,   191,
     170,  1081,   164,   120,  1192,  1085,  1247,   165,   222,    61,
     175,   505,   170,   150,   648,   175,   117,   125,    73,   420,
     421,    92,   184,   185,   186,  1047,   103,   189,   190,   191,
    1110,   170,   146,  1113,  1114,   529,   146,   145,   146,   146,
      92,   170,  1266,   170,   960,    48,   146,   118,   190,   191,
     209,   226,   833,   105,   170,   910,   170,   146,   220,    61,
     170,   114,  1170,  1171,   226,   173,   118,   172,   175,   217,
     171,  1164,    48,   230,   145,   175,   146,    32,   253,   236,
    1160,    92,   172,   146,   103,    75,   175,    27,   245,     3,
     304,   253,   103,   145,  1247,   736,   665,   146,  1178,   170,
    1180,  1181,   146,   855,  1126,   175,   858,   455,   860,    18,
      56,   165,   146,   165,    90,   140,   118,   171,   170,    27,
     146,    56,   125,   148,   140,   146,   175,   117,   303,   173,
     125,   165,   148,  1286,    89,    34,    35,    36,    93,    79,
      48,   303,   145,   146,    52,    61,   321,   173,   310,   311,
      90,   146,   173,  1096,    56,   150,  1096,   146,  1071,   321,
    1073,   146,   165,    90,   146,   795,   453,   792,    76,    78,
    1192,    79,   146,    81,   461,   115,    92,     8,   173,   146,
     342,   171,    90,    56,   173,   140,   146,    96,   173,   105,
     145,   173,   339,   148,   339,   357,   358,   152,    56,   173,
     830,   109,   118,   172,   113,   165,   173,   354,   107,   371,
     385,   373,   374,   375,   376,   377,   378,   146,   380,   381,
     382,   383,   384,   146,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,  1022,   146,    56,   173,   420,   421,   409,   410,   165,
     173,    61,  1264,   415,   170,   146,   418,   419,   420,   421,
     422,   423,   146,   904,   833,   173,   146,   146,   430,   173,
    1282,   463,  1011,   435,   165,  1014,  1015,    27,   440,   409,
     410,   443,    92,   172,    61,   415,   433,   171,   418,   419,
     165,   171,   171,   669,   670,   105,   172,   172,   174,   174,
     165,   463,     3,   465,   466,   146,    32,   172,   118,   471,
     472,   146,    56,   125,   476,    92,   478,   479,   493,    20,
     945,   463,    88,   485,   165,   146,   488,   172,   105,    79,
     165,   493,    56,    34,   146,   146,    56,    56,   150,   146,
      90,   118,    27,   109,   165,   507,   112,   100,    56,   511,
     497,   526,   514,   515,   165,   165,   513,   519,   165,   109,
     170,   173,    63,    89,   526,   115,    51,    93,    83,   172,
    1109,    86,  1111,  1112,   566,    27,   538,   519,    56,   541,
    1053,   543,    56,   140,   146,   547,   548,   549,   165,   551,
     552,   148,    77,   170,    79,    49,   538,   773,   774,   541,
      56,   543,   103,   165,   566,    90,   548,    56,   109,   146,
     557,   541,   557,   543,   140,   141,   764,   146,    72,   145,
      27,   147,   148,   172,   566,   174,   152,    79,   165,    47,
     115,    48,   170,   146,   160,   146,   165,   172,    90,   174,
     166,   167,   168,    48,    61,  1070,    89,  1072,    66,   230,
      93,   626,   165,  1022,   165,   236,    61,   109,    85,   621,
     622,   623,   624,   625,   626,   627,   628,    85,    56,   172,
      89,   174,    79,   648,    93,    92,   165,   376,   377,   165,
     639,   640,   164,    90,   646,    89,   648,    92,   142,    93,
      89,   156,   165,    48,    93,     2,   114,   659,   170,   661,
     172,   118,   109,   665,   646,   667,    61,    89,   125,    56,
     172,    93,   174,   118,   140,   677,   646,   679,    25,   145,
     125,    28,   148,   422,   423,   172,   152,    89,   165,   146,
     692,    93,   172,   695,   174,   165,   435,    92,   170,   811,
     541,   146,   543,   165,   165,   707,   708,   119,   165,   165,
     742,  1198,  1046,   170,   208,   165,   173,    88,   172,  1053,
     165,   172,   125,   118,   172,   170,   172,   466,   173,   172,
     125,    78,   165,   472,   172,   165,   165,   476,   740,   741,
     742,     8,    66,   170,   746,   239,   240,   241,   242,   243,
     244,   146,   145,    69,   866,   172,   250,   251,   105,   148,
     742,   873,   125,   110,  1251,  1252,  1253,  1254,  1255,  1256,
     165,   125,   787,   788,   165,   170,   746,   173,   173,   146,
     795,   165,    32,   785,    48,   125,   280,   789,   125,   791,
     792,   793,    32,   795,   172,   142,   290,   151,   292,   153,
     154,    35,    35,   157,   158,   299,   125,   173,   547,   173,
     146,   173,   173,  1054,  1055,   830,   146,   175,   175,   821,
      21,   170,   824,   317,   172,   175,   165,   165,   830,   173,
    1164,   833,   165,   165,   328,   329,   165,   165,   172,    89,
     165,   146,   165,    93,    35,   847,   848,    35,    66,    89,
     146,   173,   173,    93,   171,     2,   203,   146,   173,  1193,
     171,    90,   125,   173,    33,   867,    33,   869,   175,   170,
     170,   170,   874,  1207,    35,   877,   145,   165,    25,    90,
     882,    28,   884,   622,   175,   624,   172,   172,   175,   891,
     140,   923,   175,   165,   145,   145,   171,   147,   148,   172,
     140,   148,   152,   148,   148,   145,   148,   147,   148,   140,
     160,   148,   152,  1247,   170,   902,   166,   167,   168,   140,
     160,   923,   148,   148,   148,   148,   140,   929,   168,   931,
    1264,    78,   934,   148,   281,   148,   140,   140,     2,   148,
     287,   923,   436,   945,    35,   173,   173,   105,  1282,   165,
     170,   170,  1286,   165,   140,     2,   173,   140,   105,   165,
     962,    25,   164,   110,    28,   173,   165,   164,   173,   146,
     142,   491,   354,   785,    24,   175,   470,   511,    25,  1085,
     474,    28,   984,    90,  1096,   897,  1096,   953,   963,  1092,
     728,  1266,   339,   995,   996,   142,   998,   999,   902,  1001,
    1002,   966,  1223,  1257,  1116,  1117,  1250,   354,  1120,  1121,
      37,   508,    31,    -1,    78,  1017,  1018,    -1,  1020,    -1,
    1022,    -1,  1024,  1025,    -1,    -1,  1028,    -1,    -1,  1083,
      -1,    78,    -1,    -1,  1146,    -1,  1148,    -1,    -1,  1054,
    1055,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
    1052,  1053,  1054,  1055,    -1,    -1,   203,  1059,   105,    -1,
      -1,  1063,    27,   110,  1046,    -1,    -1,  1069,  1070,  1071,
    1072,  1073,    -1,    -1,    -1,    -1,  1078,    -1,   142,    -1,
      -1,    -1,    -1,    48,    -1,    -1,   433,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,  1128,  1099,  1100,  1101,
    1102,  1103,  1104,  1105,  1106,  1092,    -1,    -1,   847,   848,
      -1,    76,    -1,    -1,    79,    -1,    81,    -1,   465,    -1,
    1122,    -1,    -1,    -1,   471,    90,  1128,    -1,    -1,    -1,
      -1,  1133,    -1,    -1,   281,  1137,   483,    -1,   485,   203,
     287,    -1,    32,    -1,   109,    -1,  1128,    -1,    -1,    -1,
      -1,  1133,    -1,    -1,    -1,    -1,   203,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   663,
     664,    -1,    -1,    -1,    -1,   669,   670,    -1,   672,   673,
      -1,    -1,  1184,  1185,  1186,  1187,  1218,    -1,    -1,    -1,
      -1,    -1,   339,    -1,    -1,    -1,    -1,    -1,   545,    89,
      -1,    -1,   549,    93,    -1,  1207,    -1,   354,  1210,  1211,
     557,  1193,    -1,    -1,    -1,    -1,  1218,   281,    -1,    -1,
     714,    -1,    -1,   287,    -1,  1227,  1228,  1229,  1230,  1231,
    1232,    -1,    -1,    -1,   281,    -1,  1218,    -1,    -1,    -1,
     287,    -1,    -1,    -1,  1246,  1247,    -1,  1249,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,  1247,    -1,   157,   158,   159,
     160,   161,    -1,   163,    -1,   339,   166,   167,   168,    -1,
    1282,    -1,    -1,    -1,  1286,    -1,   433,   177,    -1,   636,
     354,    -1,   339,    -1,    22,    -1,    27,    -1,    26,    27,
      -1,    -1,   796,    -1,  1286,    -1,    -1,   354,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    48,   465,    -1,
      -1,    52,    -1,    51,   471,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   483,    -1,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,    79,    77,
      81,    79,    -1,    -1,    -1,   849,   850,    -1,    -1,    90,
      88,    -1,    90,   710,    -1,    -1,   713,    -1,    -1,   433,
     864,    32,   866,    -1,    -1,    -1,    -1,    -1,   109,   873,
      -1,   109,   876,    -1,   112,   879,   433,   115,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,   545,    -1,
      -1,   465,   549,    -1,   898,    -1,    -1,   471,    -1,    -1,
     557,    -1,    -1,    -1,    -1,    -1,     2,    -1,   465,   483,
      -1,   485,    -1,    -1,   471,   772,    -1,    -1,    89,    -1,
      -1,   778,    93,    -1,    -1,    -1,   483,    -1,   485,    25,
     140,   141,    28,   143,   144,   145,    -1,   147,   148,   149,
      -1,    -1,   152,   153,   154,    -1,    -1,   157,   158,   159,
     160,    -1,   956,   163,   811,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,   140,
     141,   545,   143,   144,   145,   549,   147,   148,    -1,    -1,
      -1,   152,    78,   557,    -1,    -1,    -1,    -1,   545,   160,
     847,   848,   549,    -1,    -1,   166,   167,   168,    -1,    -1,
     557,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     867,    -1,   869,    -1,   110,    17,    18,   874,    20,    -1,
     877,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   888,    34,   890,    -1,    -1,   893,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,   142,    -1,    50,    -1,
      -1,    53,    -1,   710,  1058,    -1,   713,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,   922,    -1,    32,    70,    71,
      -1,    -1,   929,    -1,   931,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,   203,   110,   111,
      -1,   113,  1116,  1117,   116,   772,  1120,  1121,    -1,    -1,
      -1,   778,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,  1138,    -1,   710,    -1,     2,   713,
      -1,    -1,  1146,    -1,  1148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   710,  1158,    -1,   713,    -1,    -1,    -1,
    1017,    25,    -1,    -1,    28,    -1,    -1,  1024,    -1,    -1,
     172,    -1,    -1,    -1,    -1,   140,   141,    -1,    -1,   144,
     145,  1038,   147,   148,    -1,   281,    -1,   152,    -1,    -1,
      -1,   287,    -1,    -1,    -1,   160,    -1,    -1,   772,  1056,
      -1,   166,   167,   168,   778,  1062,    -1,    -1,    -1,  1066,
     867,    -1,   869,    -1,    78,   772,    -1,   874,    22,    -1,
     877,   778,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   888,    36,   890,    38,    39,   893,    -1,     3,    -1,
      44,   105,    -1,   339,    -1,    -1,   110,    51,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    20,    -1,    -1,   354,    -1,
      25,    -1,  1266,    -1,    -1,   922,  1123,    -1,    -1,    -1,
      -1,    36,   929,    77,   931,    79,    -1,    81,   142,    83,
      -1,    46,    86,    -1,    88,    -1,    90,    -1,    53,    -1,
      -1,    -1,    -1,   867,    -1,   869,    -1,    -1,    63,    -1,
     874,    -1,    -1,   877,    -1,   109,    71,    -1,   112,    -1,
     867,   115,   869,    -1,   888,    -1,   890,   874,    -1,   893,
     877,    -1,    87,    -1,    -1,    -1,    -1,  1184,  1185,  1186,
    1187,   888,    -1,   890,    99,    -1,   893,   433,   103,   203,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,   922,    -1,
      -1,   116,    -1,  1210,  1211,   929,    -1,   931,    -1,    -1,
    1017,   165,     2,    -1,    -1,   922,    -1,  1024,    -1,   465,
      -1,    -1,   929,    -1,   931,   471,     8,    -1,    -1,    -1,
      -1,  1038,    -1,    -1,    -1,    25,    -1,   483,    28,   485,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,  1056,
      -1,    -1,    -1,    -1,    36,  1062,    38,    39,    -1,  1066,
      -1,    -1,    44,    -1,    -1,    -1,    -1,   281,    -1,    51,
      -1,    -1,    -1,   287,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,  1017,    -1,    77,    -1,    79,    -1,   545,
    1024,    83,    -1,   549,    86,    -1,    88,    -1,    90,    32,
    1017,   557,    -1,    -1,  1038,   105,  1123,  1024,    -1,    -1,
     110,    -1,    -1,    -1,    -1,   339,    -1,   109,    -1,    -1,
     112,  1038,  1056,   115,    -1,    -1,    -1,    -1,  1062,    -1,
     354,    -1,  1066,    -1,    -1,    -1,    -1,    -1,    -1,  1056,
       2,    72,   142,    -1,    -1,  1062,    -1,    -1,    -1,  1066,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    25,    -1,    -1,    28,  1184,  1185,  1186,
    1187,    -1,    -1,   165,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,  1123,
      -1,    -1,    -1,  1210,  1211,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,    -1,   156,  1123,   140,   141,   433,
     143,   144,   145,    -1,   147,   148,    78,    -1,   169,   152,
      -1,   172,    -1,   174,    -1,    -1,   159,   160,    -1,    -1,
     163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,   465,    -1,   105,    -1,    -1,    -1,   471,   110,    -1,
    1184,  1185,  1186,  1187,   710,    -1,    -1,   713,    -1,   483,
      -1,   485,    -1,    -1,    -1,    -1,    -1,  1184,  1185,  1186,
    1187,    -1,    -1,    -1,    -1,    -1,  1210,  1211,    -1,    -1,
     142,   281,    -1,    -1,    -1,    -1,    -1,   287,    -1,    -1,
      -1,    -1,    -1,  1210,  1211,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   240,
     241,   242,   243,   244,    22,    -1,   772,    -1,    26,    27,
      -1,   545,   778,    -1,    -1,   549,    -1,    -1,    36,    -1,
      38,    39,    -1,   557,    -1,    -1,    44,    -1,    -1,   339,
       2,   203,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,    -1,    -1,    -1,    66,   290,
      -1,   292,    -1,    25,    -1,    -1,    28,    -1,   299,    77,
      -1,    79,    -1,    -1,    -1,    83,    -1,    85,    86,    -1,
      88,    -1,    90,    -1,    -1,    -1,   317,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   328,   329,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,   867,    -1,   869,    -1,    -1,    78,    -1,   874,   281,
      -1,   877,    -1,     2,    -1,   287,    -1,    -1,    -1,    -1,
      -1,    -1,   888,   433,   890,    -1,    -1,   893,    -1,    -1,
      -1,    -1,    -1,   105,    -1,    -1,    25,    -1,   110,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   465,   922,    -1,    -1,    -1,
      -1,   471,    -1,   929,    -1,   931,    -1,   339,    -1,    -1,
      -1,    -1,    -1,   483,    -1,   485,   710,     1,    -1,   713,
      -1,    -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,   436,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,   105,    -1,    -1,    -1,
      44,   110,    -1,    -1,    -1,    -1,    -1,    51,    -1,   470,
      54,   203,    -1,   474,    -1,   545,    -1,    -1,   772,   549,
      -1,    -1,    -1,    -1,   778,    -1,    -1,   557,    -1,    -1,
      74,  1017,    -1,    77,    -1,    79,    -1,    81,  1024,    83,
      84,   433,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,  1038,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
    1056,   115,    -1,   465,    -1,    -1,  1062,    -1,    -1,   471,
    1066,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   281,
      -1,   483,    -1,   485,   203,   287,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   867,    -1,   869,    -1,    -1,    -1,    -1,
     874,   165,    -1,   877,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   888,    -1,   890,  1123,    -1,   893,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,
      -1,    -1,    -1,   545,    -1,    -1,    -1,   549,    -1,    -1,
      -1,    -1,   354,    -1,    -1,   557,    -1,    -1,   922,    -1,
      -1,    -1,   281,    -1,    -1,   929,    -1,   931,   287,    -1,
     710,    -1,    -1,   713,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1184,  1185,
    1186,  1187,   663,   664,    -1,    -1,    -1,    -1,   669,   670,
      -1,   672,   673,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1210,  1211,    -1,    -1,    -1,    -1,
     339,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,   433,   772,    -1,    -1,   354,    -1,    -1,   778,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,  1017,    -1,    -1,    -1,    -1,    -1,    34,
    1024,    -1,    -1,   465,    -1,    40,    44,    45,    46,   471,
      -1,    46,    -1,    -1,  1038,    50,    -1,    -1,    53,    -1,
      -1,   483,    -1,   485,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,  1056,    -1,    -1,    70,    71,    -1,  1062,    -1,
      -1,    -1,  1066,    78,    -1,    -1,    84,    -1,   710,    -1,
      -1,   713,    87,    -1,   433,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,   867,   103,   869,
      -1,   106,    -1,    -1,   874,   110,    -1,   877,   113,    -1,
      -1,   116,    -1,   545,    -1,    -1,   465,   549,   888,    -1,
     890,    -1,   471,   893,    -1,   557,    -1,    -1,    -1,  1123,
      -1,    -1,    -1,    -1,   483,    -1,    -1,    -1,    -1,   147,
     772,   149,    -1,    -1,    -1,    -1,   778,    -1,   849,   850,
      -1,    -1,   922,    -1,    -1,    -1,    -1,    -1,    -1,   929,
      -1,   931,    -1,   864,    -1,   170,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   876,    -1,    -1,   879,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1184,  1185,  1186,  1187,    -1,    -1,   545,    -1,    -1,    -1,
     549,    -1,   210,    -1,    -1,    -1,    -1,    -1,   557,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1210,  1211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   234,    -1,    -1,   237,
      -1,    -1,    -1,    -1,    -1,   867,    -1,   869,    -1,    -1,
     248,   249,   874,    -1,    -1,   877,    -1,  1017,    -1,    -1,
      -1,    -1,    -1,    -1,  1024,    -1,   888,    -1,   890,    -1,
      -1,   893,    -1,    -1,    -1,    -1,    -1,    -1,  1038,    -1,
      -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,   710,    -1,
      -1,   713,    -1,    -1,    -1,    -1,  1056,    -1,     3,    -1,
     922,    -1,  1062,    -1,    -1,    -1,  1066,   929,    -1,   931,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
     772,    -1,    -1,    -1,    -1,    -1,   778,    -1,    63,    -1,
      -1,   359,    -1,  1123,    -1,    70,    71,  1058,    -1,    -1,
      -1,   710,    -1,    78,   713,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,  1017,   101,    -1,   103,    -1,
      -1,   106,  1024,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,  1038,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1184,  1185,  1186,  1187,    -1,    -1,
      -1,   429,    -1,   772,  1056,    -1,    -1,    -1,    -1,   778,
    1062,    -1,    -1,    -1,  1066,   867,    -1,   869,    -1,    -1,
    1210,  1211,   874,   451,    -1,   877,    -1,   455,    -1,   457,
      -1,   459,    -1,    -1,    -1,    22,   888,   172,   890,    26,
      27,   893,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,   494,    -1,    -1,    -1,
     922,  1123,   500,    -1,    -1,    -1,    -1,   929,    -1,   931,
      -1,    -1,    -1,    -1,    -1,   513,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    83,    -1,   867,    86,
     869,    88,    -1,    90,    -1,   874,    -1,    -1,   877,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   888,
      -1,   890,   109,    -1,   893,   112,    -1,    -1,   115,    -1,
     558,    -1,  1184,  1185,  1186,  1187,    -1,    -1,    -1,    -1,
     568,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   922,    -1,    -1,    -1,    -1,  1210,  1211,
     929,    -1,   931,    -1,    -1,  1017,    17,    18,    -1,    20,
      -1,    -1,  1024,    -1,    25,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    34,   171,    -1,  1038,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,  1056,    -1,    -1,    -1,    -1,    -1,
    1062,    -1,    63,    -1,  1066,    -1,    -1,    -1,    -1,    70,
      71,   649,    -1,    -1,   652,    -1,   654,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,  1038,
      -1,  1123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1056,    -1,    -1,
      -1,    -1,    -1,  1062,    -1,    -1,    -1,  1066,    -1,    -1,
      -1,    -1,    -1,    -1,   732,    -1,    -1,    -1,   736,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    22,   752,    -1,    -1,    26,    27,    -1,
      -1,    -1,  1184,  1185,  1186,  1187,   764,    36,    -1,    38,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,     1,  1123,     3,    -1,    -1,  1210,  1211,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,   796,    17,
      18,    19,    20,   801,    -1,    -1,    -1,    25,    77,    -1,
      79,    -1,    -1,   811,    83,    -1,    34,    86,    36,    88,
      -1,    90,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
     109,    -1,    -1,   112,    -1,    63,   115,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,   165,   895,   116,   897,
      -1,    -1,    -1,   901,    -1,    -1,   904,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,   169,    -1,    -1,   172,   953,    -1,    -1,    -1,    -1,
      -1,    -1,   960,     0,     1,   963,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1096,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
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
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,     1,
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
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,
      19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   166,   167,   168,
     169,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    64,    65,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,   119,    -1,    -1,    -1,    -1,   124,    -1,
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
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
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
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,
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
      -1,    -1,   106,    -1,    -1,   109,   110,    -1,   112,   113,
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
     174,   175,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    93,    -1,    -1,    96,    97,    98,    99,   100,
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
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,
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
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,
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
     172,    -1,   174,   175,     3,   177,     5,     6,     7,     8,
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
      -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
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
      -1,    -1,    -1,   172,    -1,   174,    -1,     3,   177,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,    -1,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      32,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    89,    82,    -1,
      -1,    93,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,   142,    -1,
     152,    -1,   146,    -1,    -1,    -1,   150,   159,   160,    -1,
      -1,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
     164,    -1,    -1,    -1,    -1,   177,   170,    -1,   172,   173,
     174,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    61,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,   105,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,   164,   165,    25,    -1,    -1,    -1,   170,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
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
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,   164,    -1,    25,    -1,    -1,    -1,
     170,    -1,   172,    -1,   174,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,     3,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    12,    -1,    87,    88,
      -1,    -1,    -1,    19,    20,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    34,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,   174,    -1,   103,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,     3,   163,    -1,    -1,
     166,   167,   168,   169,   170,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    96,
      97,    -1,    99,    -1,   101,    19,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    32,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,    -1,    -1,   166,
     167,   168,   169,    -1,    12,    89,    -1,    -1,    -1,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
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
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
     102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,    89,   166,   167,   168,    93,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    48,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,   170,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    73,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,   177,    93,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,   120,   166,   167,   168,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    -1,   166,
     167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    12,    93,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    48,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      22,   166,   167,   168,    26,    27,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    36,    -1,    38,    39,    -1,    32,
      33,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    66,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    32,
      33,    83,    -1,    85,    86,    -1,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,
      93,    -1,    -1,    -1,    -1,    -1,    19,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   165,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,   102,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    36,    19,    38,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    32,    51,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    12,    13,    77,    -1,
      79,    -1,    -1,    19,    -1,    -1,    85,    -1,    -1,    88,
      -1,    90,    73,    -1,    -1,    -1,    32,    -1,    -1,   120,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
     109,    -1,    93,   112,    -1,   114,   115,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    89,   175,    -1,   177,    93,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    -1,   166,   167,   168,   124,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     124,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    89,    -1,    -1,    -1,    93,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,   165,   166,   167,   168,   123,   124,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
     102,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   124,    -1,
      -1,    -1,    -1,    89,    -1,   177,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      32,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   140,   141,
      19,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,    32,    -1,   157,   158,   159,   160,   161,
     124,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,   177,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   120,   166,   167,   168,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,   177,    93,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,   166,   167,   168,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    36,    -1,    38,    39,    19,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    89,    -1,    -1,
      12,    93,    -1,    -1,    77,    -1,    79,    19,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    88,    -1,    90,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,   109,    89,    -1,   112,
      -1,    93,   115,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   123,   124,   166,   167,   168,    -1,
      -1,    89,    -1,   173,    -1,    93,    -1,   177,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,   124,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      22,    -1,    -1,    -1,    26,    27,    32,    33,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    12,    -1,    -1,    51,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    83,    -1,    89,    86,    -1,    88,    93,    90,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    32,    -1,   115,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,   165,    -1,    -1,    -1,    -1,    89,   171,
      -1,   177,    93,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,   124,    -1,    19,    -1,    -1,    -1,   175,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    89,    -1,    -1,    -1,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      -1,    89,   166,   167,   168,    93,    -1,    -1,    -1,    12,
      13,    -1,    -1,   177,   102,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    12,    89,    -1,    -1,   177,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,   173,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,   175,    -1,   177,   102,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,   140,   141,    -1,   143,   144,
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
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,   124,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,   140,   141,    -1,
     143,   144,   145,    51,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    77,
      -1,    79,   175,    81,   177,    83,    -1,    -1,    86,    -1,
      88,    32,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    89,   166,
     167,   168,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,   124,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,   140,
     141,    -1,   143,   144,   145,    51,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,   177,    83,    -1,    -1,
      86,    -1,    88,    22,    90,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    -1,   109,    -1,    44,   112,    -1,    -1,   115,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    86,    -1,    88,
      -1,    90,    22,    -1,    -1,    -1,    26,    27,    -1,   165,
      -1,    31,    -1,    -1,    -1,   171,    36,    -1,    38,    39,
     109,    -1,    -1,   112,    44,    -1,   115,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    86,    -1,    88,    22,
      90,    -1,    -1,    26,    27,    -1,   165,    -1,    31,    -1,
      -1,    -1,   171,    36,    -1,    38,    39,    -1,    -1,   109,
      -1,    44,   112,    -1,    -1,   115,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,
     140,   141,   115,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,     1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,   165,    25,    -1,    -1,    -1,    -1,   171,    31,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    17,    18,   106,    20,    -1,    -1,   110,    -1,
      25,   113,    27,    -1,   116,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,     3,
      -1,   106,    -1,    -1,     8,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      17,    18,   106,    20,    -1,    -1,   110,    -1,    25,   113,
      -1,    -1,   116,    -1,    31,    -1,    -1,    34,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    17,    18,   106,    20,    -1,    -1,   110,    -1,
      25,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116
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
     340,   338,   338,   338,   338,   338,     1,   171,   184,   198,
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
      88,   231,   268,   299,    88,   109,   112,   227,   297,   229,
     268,   299,   211,   172,   216,    33,    48,   216,   120,   216,
     330,    33,    48,   216,   330,   216,    48,   216,    37,    73,
     165,   211,   211,   102,   197,   102,   125,   197,   275,    83,
      86,   194,   317,   172,   172,   197,   186,   172,   278,   111,
     172,   211,   303,   304,     1,   145,   308,    48,   146,   186,
     216,   146,   216,    13,   172,   172,   216,   317,   224,   146,
     165,   338,   338,   165,   170,   216,   172,   317,   165,   125,
     298,   165,   216,   216,   165,   171,   171,   184,   146,   171,
     338,   146,   173,   146,   173,   175,   330,    48,   146,   175,
     330,   123,   146,   175,     8,     1,   171,   198,    66,   205,
     206,   338,   200,   338,   211,   248,   145,   257,   170,   269,
     165,   338,   338,   338,   338,   235,   338,   235,   338,   338,
     338,   338,   338,   338,   338,     3,    20,    34,    63,   103,
     109,   212,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,    69,
     340,   340,   340,   340,   340,   317,   317,   235,   338,   235,
     338,    33,    48,    33,    48,   102,   172,    48,   175,   211,
      27,    48,    90,   115,   329,   332,   333,   338,   354,   235,
     338,   216,   165,   165,   338,   338,   125,   173,   146,   232,
     211,   301,   228,   297,   230,   211,   165,   211,   301,    48,
     313,    45,   338,   235,   338,   172,   216,    45,   338,   235,
     338,   216,   235,   338,    12,    19,    55,   140,   141,   142,
     143,   144,   145,   147,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   163,   166,   167,   168,   169,
     199,   274,   275,   276,   338,   199,   201,   125,   125,   186,
      35,   186,   338,    35,   338,   193,   173,   318,   211,   239,
     240,    27,    48,    52,    76,    79,    90,   109,   185,   279,
     280,   281,   282,   283,   266,   304,   146,   173,    34,    50,
      97,   101,   174,   215,   309,   321,   125,   305,   338,   302,
     218,   211,   299,   338,   338,   173,   290,   338,     1,   253,
     319,   173,   309,   321,   146,   171,   173,   173,   315,   173,
     315,   186,   175,   235,   338,   175,   186,   338,   175,   338,
     175,   338,   171,   171,   211,   146,   165,    13,   148,   146,
     165,    13,    37,    73,   165,   172,    21,   249,   313,   170,
       1,    31,   211,   261,   262,   263,    27,    79,    90,   109,
     271,   284,   172,   165,   165,   165,   165,   165,   165,   173,
     175,    45,   235,   338,    45,   235,   338,   318,   235,   338,
     172,    48,    90,   146,   173,    17,    20,    25,    46,    53,
      63,    71,    87,    99,   110,   116,   320,    89,    89,   327,
     327,   165,   290,   340,   319,   211,   257,   165,   211,   165,
     211,   257,   257,   165,   338,   173,   338,    33,   216,    33,
     216,   331,   332,   338,    33,   216,   330,    33,   216,   330,
     216,   216,   146,   165,    13,   165,   338,   338,    35,   186,
      35,    35,   186,   102,   197,    66,   173,   146,   173,    48,
      90,   282,   146,   173,   172,   211,    27,    79,    90,   109,
     286,   173,   304,   308,     1,   313,    69,   340,   211,   173,
     173,   171,    75,   117,   171,   254,   173,   297,   186,   175,
     330,   175,   330,   186,   123,   205,   212,   140,   141,   142,
     143,   144,   159,   163,   168,   170,   276,   338,   111,   338,
     199,   201,   318,   172,   197,   211,   250,     1,   258,   171,
       8,   263,   125,   146,   171,    90,   270,     1,     3,    17,
      20,    25,    40,    46,    53,    63,    70,    71,    87,    99,
     103,   106,   110,   116,   172,   210,   211,   213,   272,   273,
     274,   275,   320,   173,   338,    33,    33,   338,    33,    33,
     173,   175,   175,   318,   332,   308,   320,   320,   216,   216,
     257,   170,   257,   257,   170,   170,   216,   102,    45,   338,
      45,   338,   146,   173,   102,    45,   338,   216,    45,   338,
     216,   276,   338,   338,   186,   186,   338,   186,    35,   211,
     165,   240,   197,   211,   281,   304,   145,   312,    90,   308,
     305,   175,    48,   175,   172,   172,    33,   186,   313,    45,
     186,   338,   175,    45,   186,   338,   175,   338,   199,    13,
      37,    73,    37,    73,   165,   165,   173,   250,   145,   197,
     171,    31,    83,    86,   171,   185,   221,   222,   263,   338,
     262,   286,   172,   277,   338,   140,   148,   277,   277,   102,
      45,    45,   102,    45,    45,    45,    45,   173,   305,   170,
     258,   170,   170,   258,   258,   338,   338,   338,   332,   338,
     338,   338,    13,    35,   186,   165,   312,   173,   174,   215,
     290,   311,   321,   150,   291,   305,    61,   118,   292,   338,
     309,   321,   317,   317,   186,   216,   338,   186,   338,   186,
     171,   111,   338,   199,   201,   199,   201,   165,   173,   338,
       8,   222,   221,     1,   145,   307,   280,   173,     3,   103,
     273,   275,   338,   338,   338,   338,   338,   338,   258,   171,
     258,   258,   171,   171,   102,   102,   102,   102,   338,   186,
     291,   305,   312,   175,   313,   290,   338,     3,    92,   103,
     293,   294,   295,   338,   197,   217,   289,   175,   173,   173,
     102,   102,   165,   165,   165,   165,   197,   222,   174,   215,
     306,   321,   105,   287,   173,   277,   277,   102,   102,   102,
     102,   102,   102,   171,   171,   171,   338,   338,   338,   338,
     291,   305,   290,   310,   311,   321,    48,   175,   295,   118,
     146,   125,   151,   153,   154,   157,   158,    61,   321,   164,
     164,   338,   338,     1,   175,   313,   292,   338,   310,   311,
     338,   294,   295,   295,   295,   295,   295,   295,   293,   175,
     306,   321,   175,   165,   288,   289,   175,   306,   321,   310
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
     294,   295,   295,   296,   296,   296,   296,   297,   297,   298,
     298,   298,   299,   299,   299,   299,   299,   300,   300,   301,
     301,   302,   302,   303,   303,   303,   304,   304,   304,   305,
     305,   305,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   307,   307,   307,   308,   308,   308,   308,   308,   309,
     309,   309,   309,   310,   310,   310,   311,   311,   311,   311,
     311,   312,   312,   312,   312,   312,   313,   313,   313,   313,
     314,   314,   315,   315,   315,   316,   316,   317,   317,   318,
     318,   319,   319,   319,   319,   320,   320,   321,   321,   321,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   324,
     325,   326,   326,   326,   326,   326,   326,   326,   326,   327,
     327,   328,   329,   329,   330,   331,   331,   332,   332,   332,
     333,   333,   333,   333,   333,   333,   334,   334,   335,   335,
     335,   335,   335,   336,   336,   336,   336,   336,   337,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   339,   339,   340,   340,
     340,   341,   341,   341,   341,   342,   342,   342,   342,   342,
     343,   343,   343,   344,   344,   344,   344,   344,   345,   345,
     345,   345,   346,   346,   347,   347,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   349,
     349,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   351,   351,   351,   351,   351,   351,
     351,   352,   352,   352,   352,   353,   353,   353,   353,   354,
     354,   354,   354,   354,   354,   354
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
       2,     1,     1,     3,     4,     4,     5,     2,     4,     0,
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
#line 523 "chapel.ypp"
                                       { yyblock = (yyval.pblockstmt); }
#line 6644 "bison-chapel.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 528 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6650 "bison-chapel.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 529 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6656 "bison-chapel.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 536 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6662 "bison-chapel.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 541 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6668 "bison-chapel.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 542 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6674 "bison-chapel.cpp"
    break;

  case 22: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6680 "bison-chapel.cpp"
    break;

  case 23: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6686 "bison-chapel.cpp"
    break;

  case 24: /* stmt: TLABEL ident_def stmt  */
#line 562 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6692 "bison-chapel.cpp"
    break;

  case 25: /* stmt: TYIELD opt_try_expr TSEMI  */
#line 563 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6698 "bison-chapel.cpp"
    break;

  case 26: /* stmt: error TSEMI  */
#line 564 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6704 "bison-chapel.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 574 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6710 "bison-chapel.cpp"
    break;

  case 34: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 575 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6716 "bison-chapel.cpp"
    break;

  case 35: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 576 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6722 "bison-chapel.cpp"
    break;

  case 36: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 577 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6728 "bison-chapel.cpp"
    break;

  case 37: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 578 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6734 "bison-chapel.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL do_stmt  */
#line 579 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6740 "bison-chapel.cpp"
    break;

  case 39: /* tryable_stmt: TON expr do_stmt  */
#line 580 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6746 "bison-chapel.cpp"
    break;

  case 40: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 581 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6752 "bison-chapel.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL do_stmt  */
#line 582 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6758 "bison-chapel.cpp"
    break;

  case 42: /* tryable_stmt: TSYNC stmt  */
#line 583 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6764 "bison-chapel.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 589 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 6770 "bison-chapel.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 591 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 6776 "bison-chapel.cpp"
    break;

  case 48: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 601 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6790 "bison-chapel.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 614 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6799 "bison-chapel.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 619 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6808 "bison-chapel.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 624 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6817 "bison-chapel.cpp"
    break;

  case 52: /* access_control: %empty  */
#line 631 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6823 "bison-chapel.cpp"
    break;

  case 53: /* access_control: TPUBLIC  */
#line 632 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6829 "bison-chapel.cpp"
    break;

  case 54: /* access_control: TPRIVATE  */
#line 633 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6835 "bison-chapel.cpp"
    break;

  case 55: /* opt_prototype: %empty  */
#line 637 "chapel.ypp"
             { (yyval.b) = false; }
#line 6841 "bison-chapel.cpp"
    break;

  case 56: /* opt_prototype: TPROTOTYPE  */
#line 638 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6847 "bison-chapel.cpp"
    break;

  case 57: /* include_access_control: %empty  */
#line 642 "chapel.ypp"
           { (yyval.b) = false; }
#line 6853 "bison-chapel.cpp"
    break;

  case 58: /* include_access_control: TPUBLIC  */
#line 643 "chapel.ypp"
           { (yyval.b) = false; }
#line 6859 "bison-chapel.cpp"
    break;

  case 59: /* include_access_control: TPRIVATE  */
#line 644 "chapel.ypp"
           { (yyval.b) = true; }
#line 6865 "bison-chapel.cpp"
    break;

  case 60: /* $@1: %empty  */
#line 649 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6874 "bison-chapel.cpp"
    break;

  case 61: /* include_module_stmt: TINCLUDE $@1 include_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 654 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6882 "bison-chapel.cpp"
    break;

  case 62: /* block_stmt: TLCBR TRCBR  */
#line 669 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6888 "bison-chapel.cpp"
    break;

  case 63: /* block_stmt: TLCBR stmt_ls TRCBR  */
#line 670 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6894 "bison-chapel.cpp"
    break;

  case 64: /* block_stmt: TLCBR error TRCBR  */
#line 671 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6900 "bison-chapel.cpp"
    break;

  case 65: /* stmt_ls: toplevel_stmt  */
#line 676 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6906 "bison-chapel.cpp"
    break;

  case 66: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 677 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6912 "bison-chapel.cpp"
    break;

  case 67: /* renames_ls: expr  */
#line 682 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6922 "bison-chapel.cpp"
    break;

  case 68: /* renames_ls: all_op_name  */
#line 687 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6932 "bison-chapel.cpp"
    break;

  case 69: /* renames_ls: expr TAS expr  */
#line 692 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6942 "bison-chapel.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA expr  */
#line 697 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6951 "bison-chapel.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 701 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6960 "bison-chapel.cpp"
    break;

  case 72: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 705 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6969 "bison-chapel.cpp"
    break;

  case 73: /* use_renames_ls: expr  */
#line 714 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6979 "bison-chapel.cpp"
    break;

  case 74: /* use_renames_ls: expr TAS expr  */
#line 719 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6989 "bison-chapel.cpp"
    break;

  case 75: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 724 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6999 "bison-chapel.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 729 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 7008 "bison-chapel.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 733 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7017 "bison-chapel.cpp"
    break;

  case 78: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 737 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7026 "bison-chapel.cpp"
    break;

  case 79: /* opt_only_ls: %empty  */
#line 745 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7036 "bison-chapel.cpp"
    break;

  case 81: /* use_access_control: %empty  */
#line 754 "chapel.ypp"
           { (yyval.b) = true; }
#line 7042 "bison-chapel.cpp"
    break;

  case 82: /* use_access_control: TPUBLIC  */
#line 755 "chapel.ypp"
           { (yyval.b) = false; }
#line 7048 "bison-chapel.cpp"
    break;

  case 83: /* use_access_control: TPRIVATE  */
#line 756 "chapel.ypp"
           { (yyval.b) = true; }
#line 7054 "bison-chapel.cpp"
    break;

  case 84: /* use_stmt: use_access_control TUSE use_renames_ls TSEMI  */
#line 760 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7060 "bison-chapel.cpp"
    break;

  case 85: /* use_stmt: use_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 761 "chapel.ypp"
                                                           { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7066 "bison-chapel.cpp"
    break;

  case 86: /* use_stmt: use_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 762 "chapel.ypp"
                                                                    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7072 "bison-chapel.cpp"
    break;

  case 87: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 763 "chapel.ypp"
                                                                           { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7078 "bison-chapel.cpp"
    break;

  case 88: /* use_stmt: use_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 764 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7084 "bison-chapel.cpp"
    break;

  case 89: /* use_stmt: use_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 765 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7090 "bison-chapel.cpp"
    break;

  case 90: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 766 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7096 "bison-chapel.cpp"
    break;

  case 91: /* import_stmt: use_access_control TIMPORT import_ls TSEMI  */
#line 770 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7103 "bison-chapel.cpp"
    break;

  case 92: /* import_expr: expr  */
#line 775 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7109 "bison-chapel.cpp"
    break;

  case 93: /* import_expr: expr TDOT all_op_name  */
#line 776 "chapel.ypp"
                        { std::vector<PotentialRename*>* renames = new std::vector<PotentialRename*>();
                          PotentialRename* nameInMod = new PotentialRename();
                          nameInMod->tag = PotentialRename::SINGLE;
                          nameInMod->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                          renames->push_back(nameInMod);
                          (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), renames); }
#line 7120 "bison-chapel.cpp"
    break;

  case 94: /* import_expr: expr TAS ident_use  */
#line 782 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7126 "bison-chapel.cpp"
    break;

  case 95: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 783 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7132 "bison-chapel.cpp"
    break;

  case 96: /* import_ls: import_expr  */
#line 787 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7138 "bison-chapel.cpp"
    break;

  case 97: /* import_ls: import_ls TCOMMA import_expr  */
#line 788 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7144 "bison-chapel.cpp"
    break;

  case 98: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 792 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7150 "bison-chapel.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 797 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7156 "bison-chapel.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 799 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7162 "bison-chapel.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 801 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7168 "bison-chapel.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 803 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7174 "bison-chapel.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 805 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7180 "bison-chapel.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 807 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7186 "bison-chapel.cpp"
    break;

  case 105: /* opt_label_ident: %empty  */
#line 813 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7192 "bison-chapel.cpp"
    break;

  case 106: /* opt_label_ident: TIDENT  */
#line 814 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7198 "bison-chapel.cpp"
    break;

  case 107: /* ident_fn_def: TIDENT  */
#line 818 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7204 "bison-chapel.cpp"
    break;

  case 108: /* ident_fn_def: TNONE  */
#line 819 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7210 "bison-chapel.cpp"
    break;

  case 109: /* ident_fn_def: TTHIS  */
#line 820 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7216 "bison-chapel.cpp"
    break;

  case 110: /* ident_fn_def: TFALSE  */
#line 821 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7222 "bison-chapel.cpp"
    break;

  case 111: /* ident_fn_def: TTRUE  */
#line 822 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7228 "bison-chapel.cpp"
    break;

  case 112: /* ident_fn_def: internal_type_ident_def  */
#line 823 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7234 "bison-chapel.cpp"
    break;

  case 113: /* ident_def: TIDENT  */
#line 826 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7240 "bison-chapel.cpp"
    break;

  case 114: /* ident_def: TNONE  */
#line 827 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7246 "bison-chapel.cpp"
    break;

  case 115: /* ident_def: TTHIS  */
#line 828 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7252 "bison-chapel.cpp"
    break;

  case 116: /* ident_def: TFALSE  */
#line 829 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7258 "bison-chapel.cpp"
    break;

  case 117: /* ident_def: TTRUE  */
#line 830 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7264 "bison-chapel.cpp"
    break;

  case 118: /* ident_def: internal_type_ident_def  */
#line 831 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7270 "bison-chapel.cpp"
    break;

  case 119: /* ident_use: TIDENT  */
#line 843 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7276 "bison-chapel.cpp"
    break;

  case 120: /* ident_use: TTHIS  */
#line 844 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7282 "bison-chapel.cpp"
    break;

  case 121: /* internal_type_ident_def: TBOOL  */
#line 855 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7288 "bison-chapel.cpp"
    break;

  case 122: /* internal_type_ident_def: TINT  */
#line 856 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7294 "bison-chapel.cpp"
    break;

  case 123: /* internal_type_ident_def: TUINT  */
#line 857 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7300 "bison-chapel.cpp"
    break;

  case 124: /* internal_type_ident_def: TREAL  */
#line 858 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7306 "bison-chapel.cpp"
    break;

  case 125: /* internal_type_ident_def: TIMAG  */
#line 859 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7312 "bison-chapel.cpp"
    break;

  case 126: /* internal_type_ident_def: TCOMPLEX  */
#line 860 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7318 "bison-chapel.cpp"
    break;

  case 127: /* internal_type_ident_def: TBYTES  */
#line 861 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7324 "bison-chapel.cpp"
    break;

  case 128: /* internal_type_ident_def: TSTRING  */
#line 862 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7330 "bison-chapel.cpp"
    break;

  case 129: /* internal_type_ident_def: TSYNC  */
#line 863 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7336 "bison-chapel.cpp"
    break;

  case 130: /* internal_type_ident_def: TSINGLE  */
#line 864 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7342 "bison-chapel.cpp"
    break;

  case 131: /* internal_type_ident_def: TOWNED  */
#line 865 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7348 "bison-chapel.cpp"
    break;

  case 132: /* internal_type_ident_def: TSHARED  */
#line 866 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7354 "bison-chapel.cpp"
    break;

  case 133: /* internal_type_ident_def: TBORROWED  */
#line 867 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7360 "bison-chapel.cpp"
    break;

  case 134: /* internal_type_ident_def: TUNMANAGED  */
#line 868 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7366 "bison-chapel.cpp"
    break;

  case 135: /* internal_type_ident_def: TDOMAIN  */
#line 869 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7372 "bison-chapel.cpp"
    break;

  case 136: /* internal_type_ident_def: TINDEX  */
#line 870 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7378 "bison-chapel.cpp"
    break;

  case 137: /* internal_type_ident_def: TLOCALE  */
#line 871 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7384 "bison-chapel.cpp"
    break;

  case 138: /* internal_type_ident_def: TNOTHING  */
#line 872 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7390 "bison-chapel.cpp"
    break;

  case 139: /* internal_type_ident_def: TVOID  */
#line 873 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7396 "bison-chapel.cpp"
    break;

  case 140: /* scalar_type: TBOOL  */
#line 877 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7402 "bison-chapel.cpp"
    break;

  case 141: /* scalar_type: TENUM  */
#line 878 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7408 "bison-chapel.cpp"
    break;

  case 142: /* scalar_type: TINT  */
#line 879 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7414 "bison-chapel.cpp"
    break;

  case 143: /* scalar_type: TUINT  */
#line 880 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7420 "bison-chapel.cpp"
    break;

  case 144: /* scalar_type: TREAL  */
#line 881 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7426 "bison-chapel.cpp"
    break;

  case 145: /* scalar_type: TIMAG  */
#line 882 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7432 "bison-chapel.cpp"
    break;

  case 146: /* scalar_type: TCOMPLEX  */
#line 883 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7438 "bison-chapel.cpp"
    break;

  case 147: /* scalar_type: TBYTES  */
#line 884 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7444 "bison-chapel.cpp"
    break;

  case 148: /* scalar_type: TSTRING  */
#line 885 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7450 "bison-chapel.cpp"
    break;

  case 149: /* scalar_type: TLOCALE  */
#line 886 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7456 "bison-chapel.cpp"
    break;

  case 150: /* scalar_type: TNOTHING  */
#line 887 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7462 "bison-chapel.cpp"
    break;

  case 151: /* scalar_type: TVOID  */
#line 888 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7468 "bison-chapel.cpp"
    break;

  case 152: /* reserved_type_ident_use: TSYNC  */
#line 895 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7474 "bison-chapel.cpp"
    break;

  case 153: /* reserved_type_ident_use: TSINGLE  */
#line 896 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7480 "bison-chapel.cpp"
    break;

  case 154: /* reserved_type_ident_use: TDOMAIN  */
#line 897 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7486 "bison-chapel.cpp"
    break;

  case 155: /* reserved_type_ident_use: TINDEX  */
#line 898 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7492 "bison-chapel.cpp"
    break;

  case 156: /* do_stmt: TDO stmt  */
#line 902 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7498 "bison-chapel.cpp"
    break;

  case 157: /* do_stmt: block_stmt  */
#line 903 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7504 "bison-chapel.cpp"
    break;

  case 158: /* return_stmt: TRETURN TSEMI  */
#line 907 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7510 "bison-chapel.cpp"
    break;

  case 159: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 908 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7516 "bison-chapel.cpp"
    break;

  case 160: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 912 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-3].pexpr), (yyvsp[-1].pflagset), (yyvsp[0].pch)); }
#line 7522 "bison-chapel.cpp"
    break;

  case 161: /* manager_expr: expr TAS ident_def  */
#line 913 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-2].pexpr), NULL, (yyvsp[0].pch)); }
#line 7528 "bison-chapel.cpp"
    break;

  case 162: /* manager_expr: expr  */
#line 914 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[0].pexpr), NULL, NULL); }
#line 7534 "bison-chapel.cpp"
    break;

  case 163: /* manager_expr_ls: manager_expr  */
#line 918 "chapel.ypp"
                                        { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7540 "bison-chapel.cpp"
    break;

  case 164: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 919 "chapel.ypp"
                                        { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7546 "bison-chapel.cpp"
    break;

  case 165: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 923 "chapel.ypp"
                                  { (yyval.pblockstmt) = buildManageStmt((yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7552 "bison-chapel.cpp"
    break;

  case 167: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 929 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 7558 "bison-chapel.cpp"
    break;

  case 168: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 931 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 7564 "bison-chapel.cpp"
    break;

  case 169: /* class_level_stmt: TSEMI  */
#line 935 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7570 "bison-chapel.cpp"
    break;

  case 171: /* class_level_stmt: TPUBLIC private_decl  */
#line 937 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7576 "bison-chapel.cpp"
    break;

  case 172: /* @2: %empty  */
#line 947 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7582 "bison-chapel.cpp"
    break;

  case 173: /* class_level_stmt: TPRIVATE @2 private_decl  */
#line 948 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7588 "bison-chapel.cpp"
    break;

  case 181: /* forwarding_stmt: TFORWARDING expr TSEMI  */
#line 962 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7594 "bison-chapel.cpp"
    break;

  case 182: /* forwarding_stmt: TFORWARDING expr TEXCEPT renames_ls TSEMI  */
#line 963 "chapel.ypp"
                                             { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7600 "bison-chapel.cpp"
    break;

  case 183: /* forwarding_stmt: TFORWARDING expr TONLY opt_only_ls TSEMI  */
#line 964 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7606 "bison-chapel.cpp"
    break;

  case 184: /* forwarding_stmt: TFORWARDING var_decl_stmt  */
#line 965 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7612 "bison-chapel.cpp"
    break;

  case 185: /* $@3: %empty  */
#line 970 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7621 "bison-chapel.cpp"
    break;

  case 186: /* extern_export_decl_stmt: TEXTERN TRECORD $@3 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 975 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7635 "bison-chapel.cpp"
    break;

  case 187: /* $@4: %empty  */
#line 986 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7644 "bison-chapel.cpp"
    break;

  case 188: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TRECORD $@4 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 991 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7659 "bison-chapel.cpp"
    break;

  case 189: /* $@5: %empty  */
#line 1003 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7668 "bison-chapel.cpp"
    break;

  case 190: /* extern_export_decl_stmt: TEXTERN TUNION $@5 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1008 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7682 "bison-chapel.cpp"
    break;

  case 191: /* $@6: %empty  */
#line 1019 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7691 "bison-chapel.cpp"
    break;

  case 192: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TUNION $@6 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1024 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7706 "bison-chapel.cpp"
    break;

  case 193: /* $@7: %empty  */
#line 1036 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7715 "bison-chapel.cpp"
    break;

  case 194: /* extern_export_decl_stmt: TEXPORT TRECORD $@7 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1041 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7729 "bison-chapel.cpp"
    break;

  case 195: /* $@8: %empty  */
#line 1051 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7738 "bison-chapel.cpp"
    break;

  case 196: /* extern_export_decl_stmt: TEXPORT STRINGLITERAL TRECORD $@8 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1056 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7752 "bison-chapel.cpp"
    break;

  case 197: /* extern_export_decl_stmt: TEXTERN opt_expr fn_decl_stmt  */
#line 1067 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7760 "bison-chapel.cpp"
    break;

  case 198: /* extern_export_decl_stmt: TEXPORT opt_expr fn_decl_stmt  */
#line 1071 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7768 "bison-chapel.cpp"
    break;

  case 199: /* extern_export_decl_stmt: TEXTERN opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1076 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7780 "bison-chapel.cpp"
    break;

  case 200: /* extern_export_decl_stmt: TEXPORT opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1084 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7792 "bison-chapel.cpp"
    break;

  case 201: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1095 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7800 "bison-chapel.cpp"
    break;

  case 202: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1101 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7806 "bison-chapel.cpp"
    break;

  case 203: /* loop_stmt: TWHILE expr do_stmt  */
#line 1102 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7812 "bison-chapel.cpp"
    break;

  case 204: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1103 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7818 "bison-chapel.cpp"
    break;

  case 205: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1104 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7824 "bison-chapel.cpp"
    break;

  case 206: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1105 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7830 "bison-chapel.cpp"
    break;

  case 207: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1106 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7836 "bison-chapel.cpp"
    break;

  case 208: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1107 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7842 "bison-chapel.cpp"
    break;

  case 209: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1108 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7848 "bison-chapel.cpp"
    break;

  case 210: /* loop_stmt: TFOR expr do_stmt  */
#line 1109 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7854 "bison-chapel.cpp"
    break;

  case 211: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1110 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7860 "bison-chapel.cpp"
    break;

  case 212: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1111 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7866 "bison-chapel.cpp"
    break;

  case 213: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1112 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7872 "bison-chapel.cpp"
    break;

  case 214: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1113 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7878 "bison-chapel.cpp"
    break;

  case 215: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1114 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7884 "bison-chapel.cpp"
    break;

  case 216: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1115 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7890 "bison-chapel.cpp"
    break;

  case 217: /* loop_stmt: TFORALL expr do_stmt  */
#line 1116 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7896 "bison-chapel.cpp"
    break;

  case 218: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1117 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7902 "bison-chapel.cpp"
    break;

  case 219: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1118 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7908 "bison-chapel.cpp"
    break;

  case 220: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1119 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7914 "bison-chapel.cpp"
    break;

  case 221: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1120 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7920 "bison-chapel.cpp"
    break;

  case 222: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1121 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7926 "bison-chapel.cpp"
    break;

  case 223: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1122 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7932 "bison-chapel.cpp"
    break;

  case 224: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1123 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7938 "bison-chapel.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1125 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7948 "bison-chapel.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1131 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7958 "bison-chapel.cpp"
    break;

  case 227: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1137 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7968 "bison-chapel.cpp"
    break;

  case 228: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1143 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7978 "bison-chapel.cpp"
    break;

  case 229: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1149 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7989 "bison-chapel.cpp"
    break;

  case 230: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1156 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 8000 "bison-chapel.cpp"
    break;

  case 231: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1163 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8008 "bison-chapel.cpp"
    break;

  case 232: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1167 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 8016 "bison-chapel.cpp"
    break;

  case 233: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1173 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 8022 "bison-chapel.cpp"
    break;

  case 234: /* if_stmt: TIF expr TTHEN stmt  */
#line 1177 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8028 "bison-chapel.cpp"
    break;

  case 235: /* if_stmt: TIF expr block_stmt  */
#line 1178 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8034 "bison-chapel.cpp"
    break;

  case 236: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1179 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8040 "bison-chapel.cpp"
    break;

  case 237: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1180 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8046 "bison-chapel.cpp"
    break;

  case 238: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1182 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8052 "bison-chapel.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar block_stmt  */
#line 1183 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8058 "bison-chapel.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1184 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8064 "bison-chapel.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1185 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8070 "bison-chapel.cpp"
    break;

  case 242: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1187 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8077 "bison-chapel.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1189 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8084 "bison-chapel.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1191 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8091 "bison-chapel.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1193 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8098 "bison-chapel.cpp"
    break;

  case 246: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1198 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 8104 "bison-chapel.cpp"
    break;

  case 247: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1199 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 8110 "bison-chapel.cpp"
    break;

  case 248: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1204 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 8116 "bison-chapel.cpp"
    break;

  case 249: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1206 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8125 "bison-chapel.cpp"
    break;

  case 250: /* ifc_formal_ls: ifc_formal  */
#line 1213 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8131 "bison-chapel.cpp"
    break;

  case 251: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1214 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8137 "bison-chapel.cpp"
    break;

  case 252: /* ifc_formal: ident_def  */
#line 1219 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8143 "bison-chapel.cpp"
    break;

  case 253: /* implements_type_ident: TIDENT  */
#line 1224 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 8149 "bison-chapel.cpp"
    break;

  case 254: /* implements_type_ident: TBOOL  */
#line 1225 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 8155 "bison-chapel.cpp"
    break;

  case 255: /* implements_type_ident: TINT  */
#line 1226 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 8161 "bison-chapel.cpp"
    break;

  case 256: /* implements_type_ident: TUINT  */
#line 1227 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 8167 "bison-chapel.cpp"
    break;

  case 257: /* implements_type_ident: TREAL  */
#line 1228 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 8173 "bison-chapel.cpp"
    break;

  case 258: /* implements_type_ident: TIMAG  */
#line 1229 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 8179 "bison-chapel.cpp"
    break;

  case 259: /* implements_type_ident: TCOMPLEX  */
#line 1230 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 8185 "bison-chapel.cpp"
    break;

  case 260: /* implements_type_ident: TBYTES  */
#line 1231 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 8191 "bison-chapel.cpp"
    break;

  case 261: /* implements_type_ident: TSTRING  */
#line 1232 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 8197 "bison-chapel.cpp"
    break;

  case 262: /* implements_type_ident: TLOCALE  */
#line 1233 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 8203 "bison-chapel.cpp"
    break;

  case 263: /* implements_type_ident: TNOTHING  */
#line 1234 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 8209 "bison-chapel.cpp"
    break;

  case 264: /* implements_type_ident: TVOID  */
#line 1235 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 8215 "bison-chapel.cpp"
    break;

  case 265: /* implements_type_ident: implements_type_error_ident  */
#line 1237 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8222 "bison-chapel.cpp"
    break;

  case 266: /* implements_type_error_ident: TNONE  */
#line 1243 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 8228 "bison-chapel.cpp"
    break;

  case 267: /* implements_type_error_ident: TTHIS  */
#line 1244 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 8234 "bison-chapel.cpp"
    break;

  case 268: /* implements_type_error_ident: TFALSE  */
#line 1245 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 8240 "bison-chapel.cpp"
    break;

  case 269: /* implements_type_error_ident: TTRUE  */
#line 1246 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 8246 "bison-chapel.cpp"
    break;

  case 270: /* implements_type_error_ident: TDOMAIN  */
#line 1255 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 8252 "bison-chapel.cpp"
    break;

  case 271: /* implements_type_error_ident: TINDEX  */
#line 1256 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 8258 "bison-chapel.cpp"
    break;

  case 272: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1261 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8264 "bison-chapel.cpp"
    break;

  case 273: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1263 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8271 "bison-chapel.cpp"
    break;

  case 274: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1266 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8278 "bison-chapel.cpp"
    break;

  case 275: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1272 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8284 "bison-chapel.cpp"
    break;

  case 276: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1274 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8291 "bison-chapel.cpp"
    break;

  case 277: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1277 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8298 "bison-chapel.cpp"
    break;

  case 278: /* defer_stmt: TDEFER stmt  */
#line 1282 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8304 "bison-chapel.cpp"
    break;

  case 279: /* try_token: TTRY  */
#line 1285 "chapel.ypp"
           { (yyval.b) = false; }
#line 8310 "bison-chapel.cpp"
    break;

  case 280: /* try_token: TTRYBANG  */
#line 1286 "chapel.ypp"
           { (yyval.b) = true; }
#line 8316 "bison-chapel.cpp"
    break;

  case 281: /* try_stmt: try_token tryable_stmt  */
#line 1289 "chapel.ypp"
                         { (yyval.pblockstmt) = TryStmt::build((yyvsp[-1].b), (yyvsp[0].pblockstmt)); }
#line 8322 "bison-chapel.cpp"
    break;

  case 282: /* try_stmt: try_token block_stmt catch_stmt_ls  */
#line 1290 "chapel.ypp"
                                       { (yyval.pblockstmt) = TryStmt::build((yyvsp[-2].b), (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8328 "bison-chapel.cpp"
    break;

  case 283: /* catch_stmt_ls: %empty  */
#line 1294 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8334 "bison-chapel.cpp"
    break;

  case 284: /* catch_stmt_ls: catch_stmt_ls catch_stmt  */
#line 1295 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8340 "bison-chapel.cpp"
    break;

  case 285: /* catch_stmt: TCATCH block_stmt  */
#line 1299 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8346 "bison-chapel.cpp"
    break;

  case 286: /* catch_stmt: TCATCH catch_expr block_stmt  */
#line 1300 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8352 "bison-chapel.cpp"
    break;

  case 287: /* catch_stmt: TCATCH TLP catch_expr TRP block_stmt  */
#line 1301 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8358 "bison-chapel.cpp"
    break;

  case 288: /* catch_expr: ident_def  */
#line 1305 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8364 "bison-chapel.cpp"
    break;

  case 289: /* catch_expr: ident_def TCOLON expr  */
#line 1306 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8370 "bison-chapel.cpp"
    break;

  case 290: /* throw_stmt: TTHROW expr TSEMI  */
#line 1310 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8376 "bison-chapel.cpp"
    break;

  case 291: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1314 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8382 "bison-chapel.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1316 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8388 "bison-chapel.cpp"
    break;

  case 293: /* when_stmt_ls: %empty  */
#line 1320 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8394 "bison-chapel.cpp"
    break;

  case 294: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1321 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8400 "bison-chapel.cpp"
    break;

  case 295: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1326 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8406 "bison-chapel.cpp"
    break;

  case 296: /* when_stmt: TOTHERWISE stmt  */
#line 1328 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8412 "bison-chapel.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1330 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8418 "bison-chapel.cpp"
    break;

  case 298: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1337 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8432 "bison-chapel.cpp"
    break;

  case 299: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR error TRCBR  */
#line 1347 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8446 "bison-chapel.cpp"
    break;

  case 300: /* class_tag: TCLASS  */
#line 1360 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8456 "bison-chapel.cpp"
    break;

  case 301: /* class_tag: TRECORD  */
#line 1365 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8466 "bison-chapel.cpp"
    break;

  case 302: /* class_tag: TUNION  */
#line 1370 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8476 "bison-chapel.cpp"
    break;

  case 303: /* opt_inherit: %empty  */
#line 1378 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8482 "bison-chapel.cpp"
    break;

  case 304: /* opt_inherit: TCOLON expr_ls  */
#line 1379 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8488 "bison-chapel.cpp"
    break;

  case 305: /* class_level_stmt_ls: %empty  */
#line 1384 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8494 "bison-chapel.cpp"
    break;

  case 306: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1386 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8500 "bison-chapel.cpp"
    break;

  case 307: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1388 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8506 "bison-chapel.cpp"
    break;

  case 308: /* enum_decl_stmt: enum_header ident_def TLCBR enum_ls TRCBR  */
#line 1393 "chapel.ypp"
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
#line 8526 "bison-chapel.cpp"
    break;

  case 309: /* enum_decl_stmt: enum_header ident_def TLCBR error TRCBR  */
#line 1409 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8534 "bison-chapel.cpp"
    break;

  case 310: /* enum_header: TENUM  */
#line 1416 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8544 "bison-chapel.cpp"
    break;

  case 311: /* enum_ls: deprecated_enum_item  */
#line 1425 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8556 "bison-chapel.cpp"
    break;

  case 312: /* enum_ls: enum_ls TCOMMA  */
#line 1433 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8564 "bison-chapel.cpp"
    break;

  case 313: /* enum_ls: enum_ls TCOMMA deprecated_enum_item  */
#line 1437 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8572 "bison-chapel.cpp"
    break;

  case 315: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1445 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr), (yyvsp[-1].pch)); }
#line 8578 "bison-chapel.cpp"
    break;

  case 316: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1447 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr)); }
#line 8584 "bison-chapel.cpp"
    break;

  case 317: /* enum_item: ident_def  */
#line 1451 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8590 "bison-chapel.cpp"
    break;

  case 318: /* enum_item: ident_def TASSIGN expr  */
#line 1452 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8596 "bison-chapel.cpp"
    break;

  case 319: /* $@9: %empty  */
#line 1457 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8605 "bison-chapel.cpp"
    break;

  case 320: /* $@10: %empty  */
#line 1462 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8614 "bison-chapel.cpp"
    break;

  case 321: /* lambda_decl_expr: TLAMBDA $@9 req_formal_ls $@10 opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1467 "chapel.ypp"
    {
      (yyvsp[-5].pfnsymbol)->retTag = (yyvsp[-3].retTag);
      if ((yyvsp[-3].retTag) == RET_REF || (yyvsp[-3].retTag) == RET_CONST_REF)
        USR_FATAL((yyvsp[-5].pfnsymbol), "'ref' return types are not allowed in lambdas");
      if ((yyvsp[-3].retTag) == RET_PARAM)
        USR_FATAL((yyvsp[-5].pfnsymbol), "'param' return types are not allowed in lambdas");
      if ((yyvsp[-3].retTag) == RET_TYPE)
        USR_FATAL((yyvsp[-5].pfnsymbol), "'type' return types are not allowed in lambdas");
      if ((yyvsp[-2].pexpr))
        (yyvsp[-5].pfnsymbol)->retExprType = new BlockStmt((yyvsp[-2].pexpr), BLOCK_SCOPELESS);
      if ((yyvsp[-1].lifetimeAndWhere).where)
        (yyvsp[-5].pfnsymbol)->where = new BlockStmt((yyvsp[-1].lifetimeAndWhere).where);
      if ((yyvsp[-1].lifetimeAndWhere).lifetime)
        (yyvsp[-5].pfnsymbol)->lifetimeConstraints = new BlockStmt((yyvsp[-1].lifetimeAndWhere).lifetime);
      (yyvsp[-5].pfnsymbol)->insertAtTail((yyvsp[0].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[-5].pfnsymbol)));
    }
#line 8636 "bison-chapel.cpp"
    break;

  case 322: /* linkage_spec: %empty  */
#line 1489 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8644 "bison-chapel.cpp"
    break;

  case 323: /* linkage_spec: TINLINE  */
#line 1492 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8653 "bison-chapel.cpp"
    break;

  case 324: /* linkage_spec: TOVERRIDE  */
#line 1496 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8662 "bison-chapel.cpp"
    break;

  case 325: /* $@11: %empty  */
#line 1504 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8676 "bison-chapel.cpp"
    break;

  case 326: /* $@12: %empty  */
#line 1514 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8687 "bison-chapel.cpp"
    break;

  case 327: /* fn_decl_stmt: linkage_spec proc_iter_or_op $@11 fn_decl_stmt_inner $@12 opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where opt_function_body_stmt  */
#line 1521 "chapel.ypp"
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
#line 8721 "bison-chapel.cpp"
    break;

  case 328: /* fn_decl_stmt_inner: opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1554 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8729 "bison-chapel.cpp"
    break;

  case 329: /* fn_decl_stmt_inner: opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1558 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8738 "bison-chapel.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1563 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8746 "bison-chapel.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1567 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8755 "bison-chapel.cpp"
    break;

  case 332: /* fn_decl_stmt_inner: opt_this_intent_tag error opt_formal_ls  */
#line 1572 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8763 "bison-chapel.cpp"
    break;

  case 334: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1579 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8769 "bison-chapel.cpp"
    break;

  case 335: /* fn_ident: ident_fn_def  */
#line 1583 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8775 "bison-chapel.cpp"
    break;

  case 336: /* fn_ident: ident_def TBANG  */
#line 1584 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8781 "bison-chapel.cpp"
    break;

  case 337: /* fn_ident: op_ident  */
#line 1585 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8787 "bison-chapel.cpp"
    break;

  case 338: /* op_ident: TBAND  */
#line 1589 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8793 "bison-chapel.cpp"
    break;

  case 339: /* op_ident: TBOR  */
#line 1590 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8799 "bison-chapel.cpp"
    break;

  case 340: /* op_ident: TBXOR  */
#line 1591 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8805 "bison-chapel.cpp"
    break;

  case 341: /* op_ident: TBNOT  */
#line 1592 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8811 "bison-chapel.cpp"
    break;

  case 342: /* op_ident: TEQUAL  */
#line 1593 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8817 "bison-chapel.cpp"
    break;

  case 343: /* op_ident: TNOTEQUAL  */
#line 1594 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8823 "bison-chapel.cpp"
    break;

  case 344: /* op_ident: TLESSEQUAL  */
#line 1595 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8829 "bison-chapel.cpp"
    break;

  case 345: /* op_ident: TGREATEREQUAL  */
#line 1596 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8835 "bison-chapel.cpp"
    break;

  case 346: /* op_ident: TLESS  */
#line 1597 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8841 "bison-chapel.cpp"
    break;

  case 347: /* op_ident: TGREATER  */
#line 1598 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8847 "bison-chapel.cpp"
    break;

  case 348: /* op_ident: TPLUS  */
#line 1599 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8853 "bison-chapel.cpp"
    break;

  case 349: /* op_ident: TMINUS  */
#line 1600 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8859 "bison-chapel.cpp"
    break;

  case 350: /* op_ident: TSTAR  */
#line 1601 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8865 "bison-chapel.cpp"
    break;

  case 351: /* op_ident: TDIVIDE  */
#line 1602 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8871 "bison-chapel.cpp"
    break;

  case 352: /* op_ident: TSHIFTLEFT  */
#line 1603 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8877 "bison-chapel.cpp"
    break;

  case 353: /* op_ident: TSHIFTRIGHT  */
#line 1604 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8883 "bison-chapel.cpp"
    break;

  case 354: /* op_ident: TMOD  */
#line 1605 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8889 "bison-chapel.cpp"
    break;

  case 355: /* op_ident: TEXP  */
#line 1606 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8895 "bison-chapel.cpp"
    break;

  case 356: /* op_ident: TBANG  */
#line 1607 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8901 "bison-chapel.cpp"
    break;

  case 357: /* op_ident: TBY  */
#line 1608 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8907 "bison-chapel.cpp"
    break;

  case 358: /* op_ident: THASH  */
#line 1609 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8913 "bison-chapel.cpp"
    break;

  case 359: /* op_ident: TALIGN  */
#line 1610 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8919 "bison-chapel.cpp"
    break;

  case 360: /* op_ident: TSWAP  */
#line 1611 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8925 "bison-chapel.cpp"
    break;

  case 361: /* op_ident: TIO  */
#line 1612 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8931 "bison-chapel.cpp"
    break;

  case 362: /* op_ident: TINITEQUALS  */
#line 1613 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8937 "bison-chapel.cpp"
    break;

  case 363: /* op_ident: TCOLON  */
#line 1614 "chapel.ypp"
                 { (yyval.pch) = ":"; }
#line 8943 "bison-chapel.cpp"
    break;

  case 364: /* assignop_ident: TASSIGN  */
#line 1618 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8949 "bison-chapel.cpp"
    break;

  case 365: /* assignop_ident: TASSIGNPLUS  */
#line 1619 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8955 "bison-chapel.cpp"
    break;

  case 366: /* assignop_ident: TASSIGNMINUS  */
#line 1620 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8961 "bison-chapel.cpp"
    break;

  case 367: /* assignop_ident: TASSIGNMULTIPLY  */
#line 1621 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8967 "bison-chapel.cpp"
    break;

  case 368: /* assignop_ident: TASSIGNDIVIDE  */
#line 1622 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8973 "bison-chapel.cpp"
    break;

  case 369: /* assignop_ident: TASSIGNMOD  */
#line 1623 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8979 "bison-chapel.cpp"
    break;

  case 370: /* assignop_ident: TASSIGNEXP  */
#line 1624 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8985 "bison-chapel.cpp"
    break;

  case 371: /* assignop_ident: TASSIGNBAND  */
#line 1625 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8991 "bison-chapel.cpp"
    break;

  case 372: /* assignop_ident: TASSIGNBOR  */
#line 1626 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8997 "bison-chapel.cpp"
    break;

  case 373: /* assignop_ident: TASSIGNBXOR  */
#line 1627 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 9003 "bison-chapel.cpp"
    break;

  case 374: /* assignop_ident: TASSIGNSR  */
#line 1628 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 9009 "bison-chapel.cpp"
    break;

  case 375: /* assignop_ident: TASSIGNSL  */
#line 1629 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 9015 "bison-chapel.cpp"
    break;

  case 376: /* all_op_name: op_ident  */
#line 1633 "chapel.ypp"
                 { (yyval.pch) = (yyvsp[0].pch); }
#line 9021 "bison-chapel.cpp"
    break;

  case 377: /* all_op_name: assignop_ident  */
#line 1634 "chapel.ypp"
                 { (yyval.pch) = (yyvsp[0].pch); }
#line 9027 "bison-chapel.cpp"
    break;

  case 378: /* opt_formal_ls: %empty  */
#line 1638 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 9033 "bison-chapel.cpp"
    break;

  case 379: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1639 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9039 "bison-chapel.cpp"
    break;

  case 380: /* req_formal_ls: TLP formal_ls TRP  */
#line 1643 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9045 "bison-chapel.cpp"
    break;

  case 381: /* formal_ls_inner: formal  */
#line 1647 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 9051 "bison-chapel.cpp"
    break;

  case 382: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 1648 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 9057 "bison-chapel.cpp"
    break;

  case 383: /* formal_ls: %empty  */
#line 1652 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 9063 "bison-chapel.cpp"
    break;

  case 384: /* formal_ls: formal_ls_inner  */
#line 1653 "chapel.ypp"
                           { (yyval.pfnsymbol) = (yyvsp[0].pfnsymbol); }
#line 9069 "bison-chapel.cpp"
    break;

  case 385: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1658 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 9075 "bison-chapel.cpp"
    break;

  case 386: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1660 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 9081 "bison-chapel.cpp"
    break;

  case 387: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1662 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 9087 "bison-chapel.cpp"
    break;

  case 388: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1664 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 9093 "bison-chapel.cpp"
    break;

  case 389: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1666 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9099 "bison-chapel.cpp"
    break;

  case 390: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1668 "chapel.ypp"
    {
      astlocMarker mark(chplLineno, yyfilename);
      UnresolvedSymExpr* dummy = new UnresolvedSymExpr("varargs");
      USR_FATAL(dummy, "variable-length argument may not be grouped "
                       "in a tuple");
    }
#line 9110 "bison-chapel.cpp"
    break;

  case 391: /* opt_intent_tag: %empty  */
#line 1677 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 9116 "bison-chapel.cpp"
    break;

  case 392: /* opt_intent_tag: required_intent_tag  */
#line 1678 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 9122 "bison-chapel.cpp"
    break;

  case 393: /* required_intent_tag: TIN  */
#line 1682 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 9128 "bison-chapel.cpp"
    break;

  case 394: /* required_intent_tag: TINOUT  */
#line 1683 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 9134 "bison-chapel.cpp"
    break;

  case 395: /* required_intent_tag: TOUT  */
#line 1684 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 9140 "bison-chapel.cpp"
    break;

  case 396: /* required_intent_tag: TCONST  */
#line 1685 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 9146 "bison-chapel.cpp"
    break;

  case 397: /* required_intent_tag: TCONST TIN  */
#line 1686 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 9152 "bison-chapel.cpp"
    break;

  case 398: /* required_intent_tag: TCONST TREF  */
#line 1687 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 9158 "bison-chapel.cpp"
    break;

  case 399: /* required_intent_tag: TPARAM  */
#line 1688 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 9164 "bison-chapel.cpp"
    break;

  case 400: /* required_intent_tag: TREF  */
#line 1689 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 9170 "bison-chapel.cpp"
    break;

  case 401: /* required_intent_tag: TTYPE  */
#line 1690 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 9176 "bison-chapel.cpp"
    break;

  case 402: /* opt_this_intent_tag: %empty  */
#line 1694 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 9182 "bison-chapel.cpp"
    break;

  case 403: /* opt_this_intent_tag: TPARAM  */
#line 1695 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 9188 "bison-chapel.cpp"
    break;

  case 404: /* opt_this_intent_tag: TREF  */
#line 1696 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 9194 "bison-chapel.cpp"
    break;

  case 405: /* opt_this_intent_tag: TCONST TREF  */
#line 1697 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 9200 "bison-chapel.cpp"
    break;

  case 406: /* opt_this_intent_tag: TCONST  */
#line 1698 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 9206 "bison-chapel.cpp"
    break;

  case 407: /* opt_this_intent_tag: TTYPE  */
#line 1699 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 9212 "bison-chapel.cpp"
    break;

  case 408: /* proc_iter_or_op: TPROC  */
#line 1703 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 9218 "bison-chapel.cpp"
    break;

  case 409: /* proc_iter_or_op: TITER  */
#line 1704 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 9224 "bison-chapel.cpp"
    break;

  case 410: /* proc_iter_or_op: TOPERATOR  */
#line 1705 "chapel.ypp"
            { (yyval.procIterOp) = ProcIterOp_OP; }
#line 9230 "bison-chapel.cpp"
    break;

  case 411: /* opt_ret_tag: %empty  */
#line 1709 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9236 "bison-chapel.cpp"
    break;

  case 412: /* opt_ret_tag: TCONST  */
#line 1710 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9242 "bison-chapel.cpp"
    break;

  case 413: /* opt_ret_tag: TCONST TREF  */
#line 1711 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 9248 "bison-chapel.cpp"
    break;

  case 414: /* opt_ret_tag: TREF  */
#line 1712 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 9254 "bison-chapel.cpp"
    break;

  case 415: /* opt_ret_tag: TPARAM  */
#line 1713 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 9260 "bison-chapel.cpp"
    break;

  case 416: /* opt_ret_tag: TTYPE  */
#line 1714 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 9266 "bison-chapel.cpp"
    break;

  case 417: /* opt_throws_error: %empty  */
#line 1718 "chapel.ypp"
          { (yyval.b) = false; }
#line 9272 "bison-chapel.cpp"
    break;

  case 418: /* opt_throws_error: TTHROWS  */
#line 1719 "chapel.ypp"
          { (yyval.b) = true;  }
#line 9278 "bison-chapel.cpp"
    break;

  case 419: /* opt_function_body_stmt: TSEMI  */
#line 1722 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 9284 "bison-chapel.cpp"
    break;

  case 422: /* function_body_stmt: return_stmt  */
#line 1728 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9290 "bison-chapel.cpp"
    break;

  case 423: /* query_expr: TQUERIEDIDENT  */
#line 1732 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9296 "bison-chapel.cpp"
    break;

  case 424: /* var_arg_expr: TDOTDOTDOT  */
#line 1736 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9302 "bison-chapel.cpp"
    break;

  case 425: /* var_arg_expr: TDOTDOTDOT expr  */
#line 1737 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9308 "bison-chapel.cpp"
    break;

  case 426: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 1738 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9318 "bison-chapel.cpp"
    break;

  case 427: /* opt_lifetime_where: %empty  */
#line 1746 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9324 "bison-chapel.cpp"
    break;

  case 428: /* opt_lifetime_where: TWHERE expr  */
#line 1748 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9330 "bison-chapel.cpp"
    break;

  case 429: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 1750 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9336 "bison-chapel.cpp"
    break;

  case 430: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 1752 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9342 "bison-chapel.cpp"
    break;

  case 431: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 1754 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9348 "bison-chapel.cpp"
    break;

  case 432: /* lifetime_components_expr: lifetime_expr  */
#line 1759 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9354 "bison-chapel.cpp"
    break;

  case 433: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 1761 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9360 "bison-chapel.cpp"
    break;

  case 434: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 1765 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9366 "bison-chapel.cpp"
    break;

  case 435: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 1766 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9372 "bison-chapel.cpp"
    break;

  case 436: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 1767 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9378 "bison-chapel.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 1768 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9384 "bison-chapel.cpp"
    break;

  case 438: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 1769 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9390 "bison-chapel.cpp"
    break;

  case 439: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 1770 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9396 "bison-chapel.cpp"
    break;

  case 440: /* lifetime_expr: TRETURN lifetime_ident  */
#line 1771 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9402 "bison-chapel.cpp"
    break;

  case 441: /* lifetime_ident: TIDENT  */
#line 1776 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9408 "bison-chapel.cpp"
    break;

  case 442: /* lifetime_ident: TTHIS  */
#line 1778 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9414 "bison-chapel.cpp"
    break;

  case 443: /* type_alias_decl_stmt: TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1783 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9420 "bison-chapel.cpp"
    break;

  case 444: /* type_alias_decl_stmt: TCONFIG TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1785 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9426 "bison-chapel.cpp"
    break;

  case 445: /* type_alias_decl_stmt: TEXTERN TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1787 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9432 "bison-chapel.cpp"
    break;

  case 446: /* type_alias_decl_stmt: TEXTERN STRINGLITERAL TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1789 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt), (yyvsp[-3].pch)); }
#line 9438 "bison-chapel.cpp"
    break;

  case 447: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 1794 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9455 "bison-chapel.cpp"
    break;

  case 448: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 1807 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9473 "bison-chapel.cpp"
    break;

  case 449: /* opt_init_type: %empty  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9479 "bison-chapel.cpp"
    break;

  case 450: /* opt_init_type: TASSIGN type_level_expr  */
#line 1825 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9485 "bison-chapel.cpp"
    break;

  case 451: /* opt_init_type: TASSIGN array_type  */
#line 1827 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9491 "bison-chapel.cpp"
    break;

  case 452: /* var_decl_type: TPARAM  */
#line 1831 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9497 "bison-chapel.cpp"
    break;

  case 453: /* var_decl_type: TCONST  */
#line 1832 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9503 "bison-chapel.cpp"
    break;

  case 454: /* var_decl_type: TREF  */
#line 1833 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9509 "bison-chapel.cpp"
    break;

  case 455: /* var_decl_type: TCONST TREF  */
#line 1834 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9515 "bison-chapel.cpp"
    break;

  case 456: /* var_decl_type: TVAR  */
#line 1835 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9521 "bison-chapel.cpp"
    break;

  case 457: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1840 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9531 "bison-chapel.cpp"
    break;

  case 458: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1846 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9540 "bison-chapel.cpp"
    break;

  case 460: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 1855 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9549 "bison-chapel.cpp"
    break;

  case 461: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 1863 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9555 "bison-chapel.cpp"
    break;

  case 462: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 1865 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9561 "bison-chapel.cpp"
    break;

  case 463: /* tuple_var_decl_component: TUNDERSCORE  */
#line 1870 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9567 "bison-chapel.cpp"
    break;

  case 464: /* tuple_var_decl_component: ident_def  */
#line 1872 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9573 "bison-chapel.cpp"
    break;

  case 465: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 1874 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9579 "bison-chapel.cpp"
    break;

  case 466: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 1879 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9585 "bison-chapel.cpp"
    break;

  case 467: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 1881 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9591 "bison-chapel.cpp"
    break;

  case 468: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 1883 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9597 "bison-chapel.cpp"
    break;

  case 469: /* opt_init_expr: %empty  */
#line 1889 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9603 "bison-chapel.cpp"
    break;

  case 470: /* opt_init_expr: TASSIGN TNOINIT  */
#line 1890 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9609 "bison-chapel.cpp"
    break;

  case 471: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 1891 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9615 "bison-chapel.cpp"
    break;

  case 472: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 1897 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9621 "bison-chapel.cpp"
    break;

  case 473: /* ret_array_type: TLSBR TRSBR  */
#line 1899 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9627 "bison-chapel.cpp"
    break;

  case 474: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1901 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9635 "bison-chapel.cpp"
    break;

  case 475: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 1905 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9643 "bison-chapel.cpp"
    break;

  case 476: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 1909 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9649 "bison-chapel.cpp"
    break;

  case 477: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 1911 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9657 "bison-chapel.cpp"
    break;

  case 478: /* ret_array_type: TLSBR error TRSBR  */
#line 1915 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9665 "bison-chapel.cpp"
    break;

  case 479: /* opt_ret_type: %empty  */
#line 1922 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9671 "bison-chapel.cpp"
    break;

  case 480: /* opt_ret_type: TCOLON type_level_expr  */
#line 1923 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9677 "bison-chapel.cpp"
    break;

  case 481: /* opt_ret_type: TCOLON ret_array_type  */
#line 1924 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9683 "bison-chapel.cpp"
    break;

  case 482: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 1925 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9689 "bison-chapel.cpp"
    break;

  case 483: /* opt_ret_type: error  */
#line 1926 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9695 "bison-chapel.cpp"
    break;

  case 484: /* opt_type: %empty  */
#line 1931 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9701 "bison-chapel.cpp"
    break;

  case 485: /* opt_type: TCOLON type_level_expr  */
#line 1932 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9707 "bison-chapel.cpp"
    break;

  case 486: /* opt_type: TCOLON array_type  */
#line 1933 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9713 "bison-chapel.cpp"
    break;

  case 487: /* opt_type: TCOLON reserved_type_ident_use  */
#line 1934 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9719 "bison-chapel.cpp"
    break;

  case 488: /* opt_type: error  */
#line 1935 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9725 "bison-chapel.cpp"
    break;

  case 489: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1956 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9733 "bison-chapel.cpp"
    break;

  case 490: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 1960 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9741 "bison-chapel.cpp"
    break;

  case 491: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 1964 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9753 "bison-chapel.cpp"
    break;

  case 492: /* array_type: TLSBR error TRSBR  */
#line 1972 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9761 "bison-chapel.cpp"
    break;

  case 493: /* opt_formal_array_elt_type: %empty  */
#line 1978 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9767 "bison-chapel.cpp"
    break;

  case 494: /* opt_formal_array_elt_type: type_level_expr  */
#line 1979 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9773 "bison-chapel.cpp"
    break;

  case 495: /* opt_formal_array_elt_type: query_expr  */
#line 1980 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9779 "bison-chapel.cpp"
    break;

  case 496: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9785 "bison-chapel.cpp"
    break;

  case 497: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9791 "bison-chapel.cpp"
    break;

  case 498: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 1993 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9797 "bison-chapel.cpp"
    break;

  case 499: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 1995 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9803 "bison-chapel.cpp"
    break;

  case 500: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 1997 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9809 "bison-chapel.cpp"
    break;

  case 501: /* opt_formal_type: %empty  */
#line 2001 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9815 "bison-chapel.cpp"
    break;

  case 502: /* opt_formal_type: TCOLON type_level_expr  */
#line 2002 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9821 "bison-chapel.cpp"
    break;

  case 503: /* opt_formal_type: TCOLON query_expr  */
#line 2003 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9827 "bison-chapel.cpp"
    break;

  case 504: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2004 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9833 "bison-chapel.cpp"
    break;

  case 505: /* opt_formal_type: TCOLON formal_array_type  */
#line 2005 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9839 "bison-chapel.cpp"
    break;

  case 506: /* expr_ls: expr  */
#line 2011 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9845 "bison-chapel.cpp"
    break;

  case 507: /* expr_ls: query_expr  */
#line 2012 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9851 "bison-chapel.cpp"
    break;

  case 508: /* expr_ls: expr_ls TCOMMA expr  */
#line 2013 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9857 "bison-chapel.cpp"
    break;

  case 509: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2014 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9863 "bison-chapel.cpp"
    break;

  case 510: /* simple_expr_ls: expr  */
#line 2018 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9869 "bison-chapel.cpp"
    break;

  case 511: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2019 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9875 "bison-chapel.cpp"
    break;

  case 512: /* tuple_component: TUNDERSCORE  */
#line 2023 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9881 "bison-chapel.cpp"
    break;

  case 513: /* tuple_component: opt_try_expr  */
#line 2024 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9887 "bison-chapel.cpp"
    break;

  case 514: /* tuple_component: query_expr  */
#line 2025 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9893 "bison-chapel.cpp"
    break;

  case 515: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2029 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9899 "bison-chapel.cpp"
    break;

  case 516: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2030 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9905 "bison-chapel.cpp"
    break;

  case 517: /* opt_actual_ls: %empty  */
#line 2034 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9911 "bison-chapel.cpp"
    break;

  case 519: /* actual_ls: actual_expr  */
#line 2039 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9917 "bison-chapel.cpp"
    break;

  case 520: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2040 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9923 "bison-chapel.cpp"
    break;

  case 521: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2044 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9929 "bison-chapel.cpp"
    break;

  case 522: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2045 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9935 "bison-chapel.cpp"
    break;

  case 523: /* actual_expr: query_expr  */
#line 2046 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9941 "bison-chapel.cpp"
    break;

  case 524: /* actual_expr: opt_try_expr  */
#line 2047 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9947 "bison-chapel.cpp"
    break;

  case 525: /* ident_expr: ident_use  */
#line 2051 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9953 "bison-chapel.cpp"
    break;

  case 526: /* ident_expr: scalar_type  */
#line 2052 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9959 "bison-chapel.cpp"
    break;

  case 527: /* type_level_expr: sub_type_level_expr  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9965 "bison-chapel.cpp"
    break;

  case 528: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9971 "bison-chapel.cpp"
    break;

  case 529: /* type_level_expr: TQUESTION  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9977 "bison-chapel.cpp"
    break;

  case 535: /* sub_type_level_expr: TSINGLE expr  */
#line 2079 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9983 "bison-chapel.cpp"
    break;

  case 536: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2081 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9989 "bison-chapel.cpp"
    break;

  case 537: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9995 "bison-chapel.cpp"
    break;

  case 538: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2085 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 10001 "bison-chapel.cpp"
    break;

  case 539: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeTypeForParentDomain", (yyvsp[-1].pexpr)); }
#line 10007 "bison-chapel.cpp"
    break;

  case 540: /* sub_type_level_expr: TATOMIC expr  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 10013 "bison-chapel.cpp"
    break;

  case 541: /* sub_type_level_expr: TSYNC expr  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 10019 "bison-chapel.cpp"
    break;

  case 542: /* sub_type_level_expr: TOWNED  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 10025 "bison-chapel.cpp"
    break;

  case 543: /* sub_type_level_expr: TOWNED expr  */
#line 2096 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 10031 "bison-chapel.cpp"
    break;

  case 544: /* sub_type_level_expr: TUNMANAGED  */
#line 2098 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 10037 "bison-chapel.cpp"
    break;

  case 545: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10043 "bison-chapel.cpp"
    break;

  case 546: /* sub_type_level_expr: TSHARED  */
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 10049 "bison-chapel.cpp"
    break;

  case 547: /* sub_type_level_expr: TSHARED expr  */
#line 2104 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 10055 "bison-chapel.cpp"
    break;

  case 548: /* sub_type_level_expr: TBORROWED  */
#line 2106 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 10061 "bison-chapel.cpp"
    break;

  case 549: /* sub_type_level_expr: TBORROWED expr  */
#line 2108 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10067 "bison-chapel.cpp"
    break;

  case 550: /* sub_type_level_expr: TCLASS  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 10073 "bison-chapel.cpp"
    break;

  case 551: /* sub_type_level_expr: TRECORD  */
#line 2113 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 10079 "bison-chapel.cpp"
    break;

  case 552: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2118 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10085 "bison-chapel.cpp"
    break;

  case 553: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2120 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10091 "bison-chapel.cpp"
    break;

  case 554: /* for_expr: TFOR expr TDO expr  */
#line 2122 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10097 "bison-chapel.cpp"
    break;

  case 555: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2124 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10103 "bison-chapel.cpp"
    break;

  case 556: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2126 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10109 "bison-chapel.cpp"
    break;

  case 557: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2128 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10115 "bison-chapel.cpp"
    break;

  case 558: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2130 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10121 "bison-chapel.cpp"
    break;

  case 559: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2132 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10127 "bison-chapel.cpp"
    break;

  case 560: /* for_expr: TFORALL expr TDO expr  */
#line 2134 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10133 "bison-chapel.cpp"
    break;

  case 561: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2136 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10139 "bison-chapel.cpp"
    break;

  case 562: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2138 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10145 "bison-chapel.cpp"
    break;

  case 563: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2140 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10151 "bison-chapel.cpp"
    break;

  case 564: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2142 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 10162 "bison-chapel.cpp"
    break;

  case 565: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2149 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 10172 "bison-chapel.cpp"
    break;

  case 566: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2155 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10182 "bison-chapel.cpp"
    break;

  case 567: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2161 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10192 "bison-chapel.cpp"
    break;

  case 568: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2167 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10202 "bison-chapel.cpp"
    break;

  case 569: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2176 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10208 "bison-chapel.cpp"
    break;

  case 570: /* nil_expr: TNIL  */
#line 2185 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 10214 "bison-chapel.cpp"
    break;

  case 578: /* stmt_level_expr: io_expr TIO expr  */
#line 2201 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10220 "bison-chapel.cpp"
    break;

  case 579: /* opt_task_intent_ls: %empty  */
#line 2205 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 10226 "bison-chapel.cpp"
    break;

  case 581: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2210 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10232 "bison-chapel.cpp"
    break;

  case 582: /* task_intent_ls: intent_expr  */
#line 2214 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10238 "bison-chapel.cpp"
    break;

  case 583: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2215 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10244 "bison-chapel.cpp"
    break;

  case 584: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2219 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10250 "bison-chapel.cpp"
    break;

  case 585: /* forall_intent_ls: intent_expr  */
#line 2223 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10256 "bison-chapel.cpp"
    break;

  case 586: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2224 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10262 "bison-chapel.cpp"
    break;

  case 587: /* intent_expr: shadow_var_prefix ident_expr opt_type opt_init_expr  */
#line 2229 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10270 "bison-chapel.cpp"
    break;

  case 588: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2233 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10278 "bison-chapel.cpp"
    break;

  case 589: /* intent_expr: expr TREDUCE ident_expr  */
#line 2237 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10286 "bison-chapel.cpp"
    break;

  case 590: /* shadow_var_prefix: TCONST  */
#line 2243 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10292 "bison-chapel.cpp"
    break;

  case 591: /* shadow_var_prefix: TIN  */
#line 2244 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10298 "bison-chapel.cpp"
    break;

  case 592: /* shadow_var_prefix: TCONST TIN  */
#line 2245 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10304 "bison-chapel.cpp"
    break;

  case 593: /* shadow_var_prefix: TREF  */
#line 2246 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10310 "bison-chapel.cpp"
    break;

  case 594: /* shadow_var_prefix: TCONST TREF  */
#line 2247 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10316 "bison-chapel.cpp"
    break;

  case 595: /* shadow_var_prefix: TVAR  */
#line 2248 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10322 "bison-chapel.cpp"
    break;

  case 597: /* io_expr: io_expr TIO expr  */
#line 2254 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10328 "bison-chapel.cpp"
    break;

  case 598: /* new_maybe_decorated: TNEW  */
#line 2259 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10334 "bison-chapel.cpp"
    break;

  case 599: /* new_maybe_decorated: TNEW TOWNED  */
#line 2261 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10342 "bison-chapel.cpp"
    break;

  case 600: /* new_maybe_decorated: TNEW TSHARED  */
#line 2265 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10350 "bison-chapel.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2269 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10358 "bison-chapel.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2273 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10366 "bison-chapel.cpp"
    break;

  case 603: /* new_expr: new_maybe_decorated expr  */
#line 2281 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10373 "bison-chapel.cpp"
    break;

  case 604: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2286 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10383 "bison-chapel.cpp"
    break;

  case 605: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2292 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10393 "bison-chapel.cpp"
    break;

  case 606: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2298 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10404 "bison-chapel.cpp"
    break;

  case 607: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2305 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10415 "bison-chapel.cpp"
    break;

  case 608: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2315 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10421 "bison-chapel.cpp"
    break;

  case 618: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2332 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10427 "bison-chapel.cpp"
    break;

  case 619: /* expr: expr TCOLON expr  */
#line 2334 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10433 "bison-chapel.cpp"
    break;

  case 620: /* expr: expr TDOTDOT expr  */
#line 2336 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10439 "bison-chapel.cpp"
    break;

  case 621: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2338 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10445 "bison-chapel.cpp"
    break;

  case 622: /* expr: expr TDOTDOT  */
#line 2353 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10451 "bison-chapel.cpp"
    break;

  case 623: /* expr: TDOTDOT expr  */
#line 2355 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10457 "bison-chapel.cpp"
    break;

  case 624: /* expr: TDOTDOTOPENHIGH expr  */
#line 2357 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10463 "bison-chapel.cpp"
    break;

  case 625: /* expr: TDOTDOT  */
#line 2359 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10469 "bison-chapel.cpp"
    break;

  case 626: /* opt_expr: %empty  */
#line 2363 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10475 "bison-chapel.cpp"
    break;

  case 627: /* opt_expr: expr  */
#line 2364 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10481 "bison-chapel.cpp"
    break;

  case 628: /* opt_try_expr: TTRY expr  */
#line 2368 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10487 "bison-chapel.cpp"
    break;

  case 629: /* opt_try_expr: TTRYBANG expr  */
#line 2369 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10493 "bison-chapel.cpp"
    break;

  case 630: /* opt_try_expr: expr  */
#line 2370 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10499 "bison-chapel.cpp"
    break;

  case 635: /* call_base_expr: lhs_expr  */
#line 2386 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10505 "bison-chapel.cpp"
    break;

  case 636: /* call_base_expr: expr TBANG  */
#line 2387 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10511 "bison-chapel.cpp"
    break;

  case 637: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2388 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10517 "bison-chapel.cpp"
    break;

  case 638: /* call_base_expr: lambda_decl_expr  */
#line 2389 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10523 "bison-chapel.cpp"
    break;

  case 640: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2394 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10529 "bison-chapel.cpp"
    break;

  case 641: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2395 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10535 "bison-chapel.cpp"
    break;

  case 642: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2396 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10541 "bison-chapel.cpp"
    break;

  case 643: /* dot_expr: expr TDOT ident_use  */
#line 2400 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10547 "bison-chapel.cpp"
    break;

  case 644: /* dot_expr: expr TDOT TTYPE  */
#line 2401 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10553 "bison-chapel.cpp"
    break;

  case 645: /* dot_expr: expr TDOT TDOMAIN  */
#line 2402 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10559 "bison-chapel.cpp"
    break;

  case 646: /* dot_expr: expr TDOT TLOCALE  */
#line 2403 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10565 "bison-chapel.cpp"
    break;

  case 647: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2404 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10571 "bison-chapel.cpp"
    break;

  case 648: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2412 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10577 "bison-chapel.cpp"
    break;

  case 649: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2413 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10583 "bison-chapel.cpp"
    break;

  case 650: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2414 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10589 "bison-chapel.cpp"
    break;

  case 651: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2415 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10595 "bison-chapel.cpp"
    break;

  case 652: /* bool_literal: TFALSE  */
#line 2419 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10601 "bison-chapel.cpp"
    break;

  case 653: /* bool_literal: TTRUE  */
#line 2420 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10607 "bison-chapel.cpp"
    break;

  case 654: /* str_bytes_literal: STRINGLITERAL  */
#line 2424 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10613 "bison-chapel.cpp"
    break;

  case 655: /* str_bytes_literal: BYTESLITERAL  */
#line 2425 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10619 "bison-chapel.cpp"
    break;

  case 658: /* literal_expr: INTLITERAL  */
#line 2431 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10625 "bison-chapel.cpp"
    break;

  case 659: /* literal_expr: REALLITERAL  */
#line 2432 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10631 "bison-chapel.cpp"
    break;

  case 660: /* literal_expr: IMAGLITERAL  */
#line 2433 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10637 "bison-chapel.cpp"
    break;

  case 661: /* literal_expr: CSTRINGLITERAL  */
#line 2434 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10643 "bison-chapel.cpp"
    break;

  case 662: /* literal_expr: TNONE  */
#line 2435 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10649 "bison-chapel.cpp"
    break;

  case 663: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2436 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10656 "bison-chapel.cpp"
    break;

  case 664: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2438 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10663 "bison-chapel.cpp"
    break;

  case 665: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2440 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10669 "bison-chapel.cpp"
    break;

  case 666: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2441 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10675 "bison-chapel.cpp"
    break;

  case 667: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2443 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10683 "bison-chapel.cpp"
    break;

  case 668: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2447 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10691 "bison-chapel.cpp"
    break;

  case 669: /* assoc_expr_ls: expr TALIAS expr  */
#line 2454 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10697 "bison-chapel.cpp"
    break;

  case 670: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2455 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10703 "bison-chapel.cpp"
    break;

  case 671: /* binary_op_expr: expr TPLUS expr  */
#line 2459 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10709 "bison-chapel.cpp"
    break;

  case 672: /* binary_op_expr: expr TMINUS expr  */
#line 2460 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10715 "bison-chapel.cpp"
    break;

  case 673: /* binary_op_expr: expr TSTAR expr  */
#line 2461 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10721 "bison-chapel.cpp"
    break;

  case 674: /* binary_op_expr: expr TDIVIDE expr  */
#line 2462 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10727 "bison-chapel.cpp"
    break;

  case 675: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2463 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10733 "bison-chapel.cpp"
    break;

  case 676: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2464 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10739 "bison-chapel.cpp"
    break;

  case 677: /* binary_op_expr: expr TMOD expr  */
#line 2465 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10745 "bison-chapel.cpp"
    break;

  case 678: /* binary_op_expr: expr TEQUAL expr  */
#line 2466 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10751 "bison-chapel.cpp"
    break;

  case 679: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2467 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10757 "bison-chapel.cpp"
    break;

  case 680: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2468 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10763 "bison-chapel.cpp"
    break;

  case 681: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2469 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10769 "bison-chapel.cpp"
    break;

  case 682: /* binary_op_expr: expr TLESS expr  */
#line 2470 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10775 "bison-chapel.cpp"
    break;

  case 683: /* binary_op_expr: expr TGREATER expr  */
#line 2471 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10781 "bison-chapel.cpp"
    break;

  case 684: /* binary_op_expr: expr TBAND expr  */
#line 2472 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10787 "bison-chapel.cpp"
    break;

  case 685: /* binary_op_expr: expr TBOR expr  */
#line 2473 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10793 "bison-chapel.cpp"
    break;

  case 686: /* binary_op_expr: expr TBXOR expr  */
#line 2474 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10799 "bison-chapel.cpp"
    break;

  case 687: /* binary_op_expr: expr TAND expr  */
#line 2475 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10805 "bison-chapel.cpp"
    break;

  case 688: /* binary_op_expr: expr TOR expr  */
#line 2476 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10811 "bison-chapel.cpp"
    break;

  case 689: /* binary_op_expr: expr TEXP expr  */
#line 2477 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10817 "bison-chapel.cpp"
    break;

  case 690: /* binary_op_expr: expr TBY expr  */
#line 2478 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10823 "bison-chapel.cpp"
    break;

  case 691: /* binary_op_expr: expr TALIGN expr  */
#line 2479 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10829 "bison-chapel.cpp"
    break;

  case 692: /* binary_op_expr: expr THASH expr  */
#line 2480 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10835 "bison-chapel.cpp"
    break;

  case 693: /* binary_op_expr: expr TDMAPPED expr  */
#line 2481 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10842 "bison-chapel.cpp"
    break;

  case 694: /* unary_op_expr: TPLUS expr  */
#line 2486 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10848 "bison-chapel.cpp"
    break;

  case 695: /* unary_op_expr: TMINUS expr  */
#line 2487 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10854 "bison-chapel.cpp"
    break;

  case 696: /* unary_op_expr: TMINUSMINUS expr  */
#line 2488 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10860 "bison-chapel.cpp"
    break;

  case 697: /* unary_op_expr: TPLUSPLUS expr  */
#line 2489 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10866 "bison-chapel.cpp"
    break;

  case 698: /* unary_op_expr: TBANG expr  */
#line 2490 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10872 "bison-chapel.cpp"
    break;

  case 699: /* unary_op_expr: expr TBANG  */
#line 2491 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10878 "bison-chapel.cpp"
    break;

  case 700: /* unary_op_expr: TBNOT expr  */
#line 2492 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10884 "bison-chapel.cpp"
    break;

  case 701: /* reduce_expr: expr TREDUCE expr  */
#line 2496 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10890 "bison-chapel.cpp"
    break;

  case 702: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2497 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10896 "bison-chapel.cpp"
    break;

  case 703: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2498 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10902 "bison-chapel.cpp"
    break;

  case 704: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2499 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10908 "bison-chapel.cpp"
    break;

  case 705: /* scan_expr: expr TSCAN expr  */
#line 2503 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10914 "bison-chapel.cpp"
    break;

  case 706: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2504 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10920 "bison-chapel.cpp"
    break;

  case 707: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2505 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10926 "bison-chapel.cpp"
    break;

  case 708: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2506 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10932 "bison-chapel.cpp"
    break;

  case 709: /* reduce_scan_op_expr: TPLUS  */
#line 2511 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10938 "bison-chapel.cpp"
    break;

  case 710: /* reduce_scan_op_expr: TSTAR  */
#line 2512 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10944 "bison-chapel.cpp"
    break;

  case 711: /* reduce_scan_op_expr: TAND  */
#line 2513 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10950 "bison-chapel.cpp"
    break;

  case 712: /* reduce_scan_op_expr: TOR  */
#line 2514 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10956 "bison-chapel.cpp"
    break;

  case 713: /* reduce_scan_op_expr: TBAND  */
#line 2515 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10962 "bison-chapel.cpp"
    break;

  case 714: /* reduce_scan_op_expr: TBOR  */
#line 2516 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10968 "bison-chapel.cpp"
    break;

  case 715: /* reduce_scan_op_expr: TBXOR  */
#line 2517 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10974 "bison-chapel.cpp"
    break;


#line 10978 "bison-chapel.cpp"

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
