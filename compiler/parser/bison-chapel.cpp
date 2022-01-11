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
#define YYLAST   21112

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  174
/* YYNRULES -- Number of rules.  */
#define YYNRULES  715
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1289

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
     553,   554,   555,   556,   557,   558,   559,   560,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   583,   584,   586,   591,   592,   596,
     609,   614,   619,   627,   628,   629,   633,   634,   638,   639,
     640,   645,   644,   665,   666,   667,   672,   673,   678,   683,
     688,   693,   697,   701,   710,   715,   720,   725,   729,   733,
     741,   746,   750,   751,   752,   756,   757,   758,   759,   760,
     761,   762,   766,   771,   772,   778,   779,   783,   784,   788,
     792,   794,   796,   798,   800,   802,   809,   810,   814,   815,
     816,   817,   818,   819,   822,   823,   824,   825,   826,   827,
     839,   840,   851,   852,   853,   854,   855,   856,   857,   858,
     859,   860,   861,   862,   863,   864,   865,   866,   867,   868,
     869,   873,   874,   875,   876,   877,   878,   879,   880,   881,
     882,   883,   884,   891,   892,   893,   894,   898,   899,   903,
     904,   908,   909,   910,   914,   915,   919,   923,   924,   926,
     931,   932,   933,   943,   943,   948,   949,   950,   951,   952,
     953,   954,   958,   959,   960,   961,   966,   965,   982,   981,
     999,   998,  1015,  1014,  1032,  1031,  1047,  1046,  1062,  1066,
    1071,  1079,  1090,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,
    1114,  1115,  1116,  1117,  1118,  1119,  1120,  1126,  1132,  1138,
    1144,  1151,  1158,  1162,  1169,  1173,  1174,  1175,  1176,  1178,
    1179,  1180,  1181,  1183,  1185,  1187,  1189,  1194,  1195,  1199,
    1201,  1209,  1210,  1215,  1220,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1229,  1230,  1231,  1232,  1239,  1240,  1241,
    1242,  1251,  1252,  1256,  1258,  1261,  1267,  1269,  1272,  1278,
    1281,  1282,  1285,  1286,  1290,  1291,  1295,  1296,  1297,  1301,
    1302,  1306,  1309,  1311,  1316,  1317,  1321,  1323,  1325,  1332,
    1342,  1356,  1361,  1366,  1374,  1375,  1380,  1381,  1383,  1388,
    1404,  1411,  1420,  1428,  1432,  1439,  1440,  1442,  1447,  1448,
    1453,  1458,  1452,  1485,  1488,  1492,  1500,  1510,  1499,  1549,
    1553,  1558,  1562,  1567,  1574,  1575,  1579,  1580,  1581,  1585,
    1586,  1587,  1588,  1589,  1590,  1591,  1592,  1593,  1594,  1595,
    1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,  1604,  1605,
    1606,  1607,  1608,  1609,  1610,  1614,  1615,  1616,  1617,  1618,
    1619,  1620,  1621,  1622,  1623,  1624,  1625,  1629,  1630,  1634,
    1635,  1639,  1643,  1644,  1648,  1649,  1653,  1655,  1657,  1659,
    1661,  1663,  1668,  1669,  1673,  1674,  1675,  1676,  1677,  1678,
    1679,  1680,  1681,  1685,  1686,  1687,  1688,  1689,  1690,  1694,
    1695,  1696,  1700,  1701,  1702,  1703,  1704,  1705,  1709,  1710,
    1713,  1714,  1718,  1719,  1723,  1727,  1728,  1729,  1737,  1738,
    1740,  1742,  1744,  1749,  1751,  1756,  1757,  1758,  1759,  1760,
    1761,  1762,  1766,  1768,  1773,  1775,  1777,  1782,  1795,  1812,
    1813,  1815,  1820,  1821,  1822,  1823,  1824,  1828,  1834,  1842,
    1843,  1851,  1853,  1858,  1860,  1862,  1867,  1869,  1871,  1878,
    1879,  1880,  1885,  1887,  1889,  1893,  1897,  1899,  1903,  1911,
    1912,  1913,  1914,  1915,  1920,  1921,  1922,  1923,  1924,  1944,
    1948,  1952,  1960,  1967,  1968,  1969,  1973,  1975,  1981,  1983,
    1985,  1990,  1991,  1992,  1993,  1994,  2000,  2001,  2002,  2003,
    2007,  2008,  2012,  2013,  2014,  2018,  2019,  2023,  2024,  2028,
    2029,  2033,  2034,  2035,  2036,  2040,  2041,  2052,  2054,  2056,
    2062,  2063,  2064,  2065,  2066,  2067,  2069,  2071,  2073,  2075,
    2077,  2079,  2082,  2084,  2086,  2088,  2090,  2092,  2094,  2096,
    2099,  2101,  2106,  2108,  2110,  2112,  2114,  2116,  2118,  2120,
    2122,  2124,  2126,  2128,  2130,  2137,  2143,  2149,  2155,  2164,
    2174,  2182,  2183,  2184,  2185,  2186,  2187,  2188,  2189,  2194,
    2195,  2199,  2203,  2204,  2208,  2212,  2213,  2217,  2221,  2225,
    2232,  2233,  2234,  2235,  2236,  2237,  2241,  2242,  2247,  2249,
    2253,  2257,  2261,  2269,  2274,  2280,  2286,  2293,  2303,  2311,
    2312,  2313,  2314,  2315,  2316,  2317,  2318,  2319,  2320,  2322,
    2324,  2326,  2341,  2343,  2345,  2347,  2352,  2353,  2357,  2358,
    2359,  2363,  2364,  2365,  2366,  2375,  2376,  2377,  2378,  2379,
    2383,  2384,  2385,  2389,  2390,  2391,  2392,  2393,  2401,  2402,
    2403,  2404,  2408,  2409,  2413,  2414,  2418,  2419,  2420,  2421,
    2422,  2423,  2424,  2425,  2427,  2429,  2430,  2431,  2435,  2443,
    2444,  2448,  2449,  2450,  2451,  2452,  2453,  2454,  2455,  2456,
    2457,  2458,  2459,  2460,  2461,  2462,  2463,  2464,  2465,  2466,
    2467,  2468,  2469,  2470,  2475,  2476,  2477,  2478,  2479,  2480,
    2481,  2485,  2486,  2487,  2488,  2492,  2493,  2494,  2495,  2500,
    2501,  2502,  2503,  2504,  2505,  2506
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

#define YYPACT_NINF (-1142)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-716)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1142,   153,  3771, -1142,    50,   178, -1142, -1142, -1142, -1142,
   -1142, -1142,  5173,   144,   284,   241, 15347,   286, 19928,   144,
   12312,   371,   342,   266,   284,  5173, 12312,  1509,  5173,   234,
   20648, 12485,  8488,   387,  9182, 10747, 10747,  7617,  9355,   442,
   -1142,   267, -1142,   450, 20735, 20735, 20735, -1142,  2525, 10920,
     464, 12312, 12312,   208, -1142,   496,   518, 12312, -1142, 15347,
   -1142, 12312,   579,   423,   350, 17229,   537, 20822, -1142, 11095,
    8661, 12312, 10920, 15347, 12312,   507,   561,   446,  5173,   575,
   12312,   576, -1142, -1142, 20735,   588, -1142, 15347, -1142,   592,
    9355, 12312, -1142, 12312, -1142, 12312, -1142, -1142, 14862, 12312,
   -1142, 12312, -1142, -1142, -1142,  4123,  7792,  9530, 12312, -1142,
    4998, -1142, -1142, -1142, -1142,   353, -1142,   566, -1142, -1142,
      26, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142, -1142, -1142, -1142,   601, -1142, -1142,
   -1142, -1142,  7444, -1142, -1142, -1142, -1142, 20735, -1142, 20735,
     264,   402, -1142, -1142,  2525, -1142,   494, -1142,   497, -1142,
   -1142,   499,   501,   513, 12312,   505,   506, 19998,  3577,   171,
     512,   516, -1142, -1142,   333, -1142, -1142, -1142, -1142, -1142,
      17, -1142, -1142, 19998,   510,  5173, -1142, -1142,   519, 12312,
   -1142, -1142, 12312, 12312, 12312, 20735, -1142, 12312, 11095, 11095,
     630,   420, -1142, -1142, -1142, -1142,   204,   467, -1142, -1142,
     517, 17006, 20735,  2525, -1142,   523, -1142,   -42, 19998,  1393,
   -1142, -1142,  9703,    47, 17492, -1142, -1142,   570,  8836,   603,
   20909, 19998,   239,    42, -1142, 20996, 20735, -1142,   239, 20735,
     520,    23, 16577,    18,  3255,    23, 16617,   238, -1142, 16769,
   20735, 20735,   -59, 15817,   224,  8836, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
     526, -1142,   223,  5173,   527,  1759,   107,    16, -1142,  5173,
   -1142, -1142, 17046, -1142,    20, 17555,  1197, -1142,   528,   529,
   -1142, 17046,   204,  1197, -1142,  8836,  2657, -1142, -1142, -1142,
     190, 19998, 12312, 12312, -1142, 19998,   525, 17662, -1142, 17046,
     204, 19998,   530,  8836, -1142, 19998, 17707,   568,   538,   204,
      23, 17046, 17747,   394,   394, 15026,  1197,  1197,   175, -1142,
   -1142,  4298,   -49, -1142, 12312, -1142,   136,   189, -1142,   -29,
      19, 17899,   -46, 15026,   705, -1142,  4473, -1142,   651, 12312,
   12312, 20735, -1142, -1142,   573,   552, -1142, -1142, -1142, -1142,
     277,   471, -1142, 12312,   571, 12312, 12312, 12312, 10747, 10747,
   12312,   480, 12312, 12312, 12312, 12312, 12312,   221, 14862, 12312,
   12312, 12312, 12312, 12312, 12312, 12312, 12312, 12312, 12312, 12312,
   12312, 12312, 12312, 12312, 12312,   654, -1142, -1142, -1142, -1142,
   -1142,  9876,  9876, -1142, -1142, -1142, -1142,  9876, -1142, -1142,
    9876,  9876,  8836,  8836, 10747, 10747,  8315, -1142, -1142, 17086,
   17238, 17939,   553,    76, 20735,  4648, -1142, 10747,    23,   567,
     344, -1142, 12312, -1142, -1142, 12312,   606, -1142,   560,   590,
   -1142, -1142, -1142, 20735, -1142,  2525, -1142, -1142, 20735,   572,
   20735, -1142,  2525,   690, 11095, -1142,  5348, 10747, -1142,   569,
   -1142,    23,  5523, 10747, -1142,    23, -1142, 10747, -1142,  7271,
    7271, -1142,   614,   615,  5173,   707,  5173, -1142,   709, 12312,
   -1142, -1142,   566,   574,  8836, 20735, -1142, -1142,   116, -1142,
   -1142,  1759, -1142,   600,   577, -1142, 12658,   624, 12312,  2525,
   -1142, -1142, 12312, -1142, 20547, 12312, 12312, -1142,   581, -1142,
   11095, -1142, 19998, 19998, -1142,    40, -1142,  8836,   585, -1142,
   12831,   613, -1142, -1142, -1142, -1142, -1142, -1142, -1142, 10051,
   -1142, 17979,  7967, -1142,  8142, -1142,  5173,   586, 10747, 10226,
    3948,   587, 12312, 11268, -1142, -1142,   389, -1142,  4823, 20735,
   -1142,   368, 18131,   382, 16809,   131,   731, 11095,   593, 20460,
     369, -1142, 18171, 20190, 20190,   458, -1142,   458, -1142,   458,
   20288,  1097,  2019,  1860,   204,   394, -1142,   598, -1142, -1142,
   -1142, -1142, -1142, 15026, 16515,   458,  2337,  2337, 20190,  2337,
    2337,  1570,   394, 16515, 20235,  1570,  1197,  1197,   394, 15026,
     609,   611,   620,   623,   626,   627,   605,   621, -1142,   458,
   -1142,   458,    69, -1142, -1142, -1142,   255, -1142,  2171, 20150,
     488, 13004, 10747, 13177, 10747, 12312,  8836, 10747, 15629,   625,
     144, 18216, -1142, -1142, -1142, 19998, 18323,  8836, -1142,  8836,
   20735,   573,   397, 20735, 20735,   573, -1142,   573,   405, 12312,
     258,  9355, 19998,    49, 17278,  8315, -1142,  9355, 19998,    21,
   16854, -1142,    23, 17046, -1142, -1142, -1142, 12312,   515, 12312,
     540,   541, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, 12312, -1142, -1142, 10399, -1142, -1142,   549, -1142,
     410, -1142, -1142, -1142, 18368,   619,   634, 12312, 12312,   760,
    5173,   767, 18408,  5173, 17318,   737, -1142,   272, -1142,   274,
   -1142,   247, -1142, -1142, -1142, -1142, -1142, -1142,   708,   658,
     632, -1142,  2912, -1142,   390,   633,  1759,   107,    45,    51,
   12312, 12312,  7098, -1142, -1142,   602,  9009, -1142, 19998, -1142,
   -1142, -1142, 20735, 18560, 18600, -1142, -1142, 19998,   636,   160,
     635, -1142, -1142,   417, 20735, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142,  5173,   -28, 17470, -1142, -1142, 19998,  5173, 19998,
   -1142, 18646, -1142, -1142, -1142, 12312, -1142,   109,  3520, 12312,
   -1142, 11441,  7271,  7271, -1142,  8836,  1865, -1142,   663,  1362,
     639, 19464,   687,   127, -1142, -1142,   723, -1142, -1142, -1142,
   -1142, 14694,   643, -1142, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142,  8315, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142, -1142, -1142,    84, 10747, 10747, 12312,
     785, 18752, 12312,   787, 18798,   300,   646, 18838,  8836,    23,
      23, -1142, -1142, -1142, -1142,   573,   652, -1142,   573,   573,
     653,   659, -1142, 17046, -1142, 15893,  5698, -1142,  5873, -1142,
     338, -1142, 15969,  6048, -1142,    23,  6223, -1142,    23, -1142,
   -1142,  7271, -1142, 12312, -1142, 19998, 19998,  5173, -1142,  5173,
   12312, -1142,  5173,   793, 20735,   666, 20735,   517, -1142, -1142,
   20735,   852, -1142,  1759,   688,   742, -1142, -1142, -1142,   113,
   -1142, -1142,   624,   660,   100, -1142, -1142, -1142,   662,   667,
   -1142,  6398, 11095, -1142, -1142, -1142, -1142, -1142,  6573,   668,
    6748,   669, -1142, 12312, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142,  7271, -1142, 18990,   232, 17510,   418,
     673,   345, 20735, -1142,   696,   517,   671,  2264, -1142, 20735,
   -1142, 12312, 19696, -1142, -1142,   390,   675,   341,   697,   700,
     701,   710,   704,   711,   712,   713,   714,   715,   718,   354,
     717,   722,   724, 12312, -1142,   733,   734,   727,   675, -1142,
     675, -1142, -1142, -1142,   624,   346,   349, 19030, 13350, 13523,
   19070, 13696, 13869, -1142, 14042, 14215,   351, -1142, -1142,   685,
   -1142,   706,   716, -1142, -1142, -1142,  5173,  9355, 19998,  9355,
   19998,  8315, -1142,  5173,  9355, 19998, -1142,  9355, 19998, -1142,
   -1142, 19222, 19998, -1142, -1142, 19998,   823,  5173,   719, -1142,
   -1142, -1142,   688, -1142,   720, 11616,   299, -1142,    15, -1142,
   -1142, 10747, 15488,  8836,  8836,  5173, -1142,    32,  9355, -1142,
   19998,  5173,  9355, -1142, 19998,  5173, 19998,   187, 11789,  7271,
    7271,  7271,  7271, -1142, -1142,   729,   725, 12312, -1142, -1142,
     869, -1142,  2657, -1142, 20383, -1142, -1142, -1142, 19998, -1142,
     115,   116, -1142, 19262, -1142, 15734, -1142, -1142, -1142, 12312,
   12312, 12312, 12312, 12312, 12312, 12312, 12312, -1142, -1142,  2626,
   -1142, -1142,  3083,  3436, 18408, 16045, 16121, -1142, 18408, 16197,
   16273, 12312,  5173, -1142, -1142,   299,   688, 10574, -1142, -1142,
   -1142,   177, 11095, -1142, -1142,   164, 12312,   -20, 19302, -1142,
     937,   726,   730,   570, -1142, 16349, -1142, 16425, -1142, -1142,
   -1142, 19998,   421,   732,   426,   744, -1142,   517, 19998,  1295,
   -1142, -1142, -1142, 14388,   775,   738, -1142,   745,   747,   675,
     675, 19454, 19494, 19534, 19686, 19726, 19766, 18135, -1142, 18571,
   20154, -1142, -1142,  5173,  5173,  5173,  5173, 19998, -1142, -1142,
   -1142,   299, 11964,   101, -1142, 19998, -1142,   176, -1142,   -15,
   -1142,   521, 19918, -1142, -1142, -1142, 14215,   728,   748,  5173,
    5173, -1142, -1142, -1142, -1142, -1142, -1142,  6923, -1142, -1142,
     233, -1142,    15, -1142, -1142, -1142, 12312, 12312, 12312, 12312,
   12312, 12312, -1142, -1142, -1142, 18408, 18408, 18408, 18408, -1142,
   -1142, -1142, -1142, -1142,   312, 10747, 15034, -1142, 12312,   164,
     176,   176,   176,   176,   176,   176,   164,  1019, -1142, -1142,
   18408, 18408,   740, 14561,   103,   169, 19958, -1142, -1142, 19998,
   -1142, -1142, -1142, -1142, -1142, -1142, -1142,   735, -1142, -1142,
     361, 15206, -1142, -1142, -1142, 12139, -1142,   383, -1142
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   120,   658,   659,   660,   654,
     655,   661,     0,   579,   106,   141,   548,   148,   550,   579,
       0,   147,     0,   453,   106,     0,     0,   323,     0,   271,
     142,   626,   626,   652,     0,     0,     0,     0,     0,   146,
      61,   272,   324,   143,     0,     0,     0,   320,     0,     0,
     150,     0,     0,   598,   570,   662,   151,     0,   325,   542,
     452,     0,     0,     0,   173,   323,   145,   551,   454,     0,
       0,     0,     0,   546,     0,     0,   149,     0,     0,   121,
       0,   653,   280,   281,     0,   144,   303,   544,   456,   152,
       0,     0,   711,     0,   713,     0,   714,   715,   625,     0,
     712,   709,   529,   170,   710,     0,     0,     0,     0,     4,
       0,     5,    12,     9,    44,     0,    47,    56,    10,    11,
       0,    13,    14,    15,    28,   525,   526,    21,    31,    48,
     171,   180,   181,    16,    30,    29,    18,     0,   266,    17,
     617,    19,     0,    20,    33,    32,   179,     0,   177,     0,
     614,     0,   175,   178,     0,   176,   631,   610,   527,   611,
     532,   530,     0,     0,     0,   615,   616,     0,   531,     0,
     632,   633,   634,   656,   657,   609,   534,   533,   612,   613,
       0,    27,    22,   540,     0,     0,   580,   107,     0,     0,
     550,   142,     0,     0,     0,     0,   551,     0,     0,     0,
       0,   614,   631,   530,   615,   616,   549,   531,   632,   633,
       0,   579,     0,     0,   455,     0,   279,     0,   510,   323,
     301,   311,   626,   173,   323,   302,    46,     0,   517,   654,
     551,   627,   323,   654,   202,   551,     0,   190,   323,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   185,     0,
       0,     0,     0,     0,    58,   517,   114,   122,   134,   128,
     127,   136,   117,   126,   137,   123,   138,   115,   139,   132,
     125,   133,   131,   129,   130,   116,   118,   124,   135,   140,
       0,   119,     0,     0,     0,     0,     0,     0,   459,     0,
     158,    39,     0,   164,     0,   163,   696,   602,   599,   600,
     601,     0,   543,   697,     7,   517,   323,   172,   424,   507,
       0,   506,     0,     0,   159,   630,     0,     0,    42,     0,
     547,   535,     0,   517,    43,   541,     0,   449,     0,   545,
       0,     0,     0,   698,   700,   623,   695,   694,     0,    63,
      66,     0,     0,   512,     0,   514,     0,     0,   513,     0,
       0,   506,     0,   624,     0,     6,     0,    57,     0,     0,
       0,     0,   282,   284,   304,     0,   410,   411,   409,   326,
       0,   528,    34,     0,   603,     0,     0,     0,     0,     0,
       0,   699,     0,     0,     0,     0,     0,     0,   622,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,   372,   373,   374,   369,
     371,     0,     0,   367,   370,   368,   366,     0,   376,   375,
       0,     0,   517,   517,     0,     0,     0,    35,    23,     0,
       0,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,    24,     0,    37,    45,     0,   525,   523,     0,   518,
     519,   524,   196,     0,   199,     0,   188,   192,     0,     0,
       0,   198,     0,     0,     0,   212,     0,     0,   211,     0,
     220,     0,     0,     0,   218,     0,   225,     0,   224,     0,
      80,   182,     0,     0,     0,   240,     0,   365,   236,     0,
      60,    59,    56,     0,     0,     0,   250,    25,   392,   321,
     463,     0,   464,   466,     0,   488,     0,   469,     0,     0,
     157,    38,     0,   166,     0,     0,     0,    40,     0,   174,
       0,    99,   628,   629,   160,     0,    41,     0,     0,   291,
       0,   447,   444,   205,   204,    26,    65,    64,    67,     0,
     663,     0,     0,   648,     0,   650,     0,     0,     0,     0,
       0,     0,     0,     0,   667,     8,     0,    50,     0,     0,
      97,     0,    93,     0,    74,   277,   283,     0,     0,     0,
     403,   458,   578,   691,   690,   693,   702,   701,   706,   705,
     687,   684,   685,   686,   619,   674,   120,     0,   645,   646,
     121,   644,   643,   620,   678,   689,   683,   681,   692,   682,
     680,   672,   677,   679,   688,   671,   675,   676,   673,   621,
       0,     0,     0,     0,     0,     0,     0,     0,   704,   703,
     708,   707,   590,   591,   593,   595,     0,   582,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   665,   277,
     579,   579,   208,   445,   457,   511,     0,     0,   537,     0,
       0,   304,     0,     0,     0,   304,   446,   304,     0,     0,
       0,     0,   554,     0,     0,     0,   221,     0,   560,     0,
       0,   219,     0,     0,   360,   358,   363,   357,   339,   342,
     340,   341,   364,   352,   343,   356,   348,   346,   359,   362,
     347,   345,   350,   355,   344,   349,   353,   354,   351,   361,
       0,   377,   378,    69,    68,    81,     0,     0,     0,   239,
       0,   235,     0,     0,     0,     0,   536,     0,   253,     0,
     251,   397,   394,   395,   396,   400,   401,   402,   392,   385,
       0,   382,     0,   393,   412,     0,   467,     0,   155,   156,
     154,   153,     0,   487,   486,   610,     0,   461,   608,   460,
     165,   162,     0,     0,     0,   642,   509,   508,     0,     0,
       0,   538,   451,   610,     0,   664,   618,   649,   515,   651,
     516,   232,     0,     0,     0,   666,   230,   564,     0,   669,
     668,     0,    52,    51,    49,     0,    92,     0,     0,     0,
      85,     0,     0,    80,   274,     0,     0,   285,   305,     0,
       0,     0,   318,     0,   312,   315,   407,   404,   405,   408,
     327,     0,     0,   105,   103,   104,   102,   101,   100,   640,
     641,   592,   594,     0,   581,   141,   148,   147,   146,   143,
     150,   151,   145,   149,   144,   152,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   203,   521,   522,   520,   304,     0,   201,   304,   304,
       0,     0,   200,     0,   234,     0,     0,   210,     0,   209,
       0,   585,     0,     0,   216,     0,     0,   214,     0,   223,
     222,     0,   183,     0,   184,   248,   247,     0,   242,     0,
       0,   238,     0,   244,     0,   276,     0,     0,   398,   399,
       0,   392,   381,     0,   501,   413,   416,   415,   417,     0,
     465,   468,   469,     0,     0,   470,   471,   161,     0,     0,
     293,     0,     0,   292,   295,   539,   448,   233,     0,     0,
       0,     0,   231,     0,    98,    95,   357,   339,   342,   340,
     341,   350,   349,   351,     0,    94,    77,    76,    75,     0,
       0,     0,     0,   286,   289,     0,     0,   323,   310,     0,
     317,     0,   313,   309,   406,   412,   379,   108,   122,   128,
     127,   111,   126,   123,   138,   109,   139,   125,   129,   110,
     112,   124,   140,     0,   336,     0,   113,     0,   379,   338,
     379,   334,   647,   583,   469,   631,   631,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,   207,   206,     0,
     306,     0,     0,   306,   306,   213,     0,     0,   553,     0,
     552,     0,   584,     0,     0,   559,   217,     0,   558,   215,
      72,    71,    70,   241,   237,   569,   243,     0,     0,   273,
     252,   249,   501,   383,     0,     0,   469,   414,   428,   462,
     492,     0,   665,   517,   517,     0,   297,     0,     0,   228,
     566,     0,     0,   226,   565,     0,   670,     0,     0,     0,
      80,     0,    80,    86,    89,   278,     0,     0,   287,   300,
     323,   173,   323,   299,   323,   307,   167,   316,   319,   314,
       0,   392,   333,     0,   337,     0,   329,   330,   587,     0,
       0,     0,     0,     0,     0,     0,     0,   278,   306,   323,
     306,   306,   323,   323,   557,     0,     0,   586,   563,     0,
       0,     0,     0,   246,    62,   469,   501,     0,   504,   503,
     505,   610,   425,   388,   386,     0,     0,     0,     0,   490,
     610,     0,     0,   298,   296,     0,   229,     0,   227,    96,
      79,    78,     0,     0,     0,     0,   275,     0,   290,   323,
     169,   308,   483,     0,   418,     0,   335,   108,   110,   379,
     379,     0,     0,     0,     0,     0,     0,   323,   195,   323,
     323,   187,   191,     0,     0,     0,     0,    73,   245,   389,
     387,   469,   493,     0,   427,   426,   442,     0,   443,   430,
     433,     0,   429,   422,   423,   322,     0,   604,   605,     0,
       0,    88,    91,    87,    90,   288,   168,     0,   482,   481,
     610,   419,   428,   380,   331,   332,     0,     0,     0,     0,
       0,     0,   197,   189,   193,   556,   555,   562,   561,   391,
     390,   495,   496,   498,   610,     0,   665,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   610,   606,   607,
     568,   567,     0,   473,     0,     0,     0,   497,   499,   432,
     434,   435,   438,   439,   440,   436,   437,   431,   478,   476,
     610,   665,   420,   328,   421,   493,   477,   610,   500
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1142, -1142, -1142,     8,    78,  2505,   746, -1142,   -11, -1142,
   -1142, -1142,   414, -1142, -1142, -1142,   463,   533,  -472, -1142,
    -757, -1142, -1142, -1142,    97, -1142, -1142, -1142,   878, -1142,
    2798,  -186,  -783, -1142,  -990,  2895, -1093,   407, -1142, -1142,
    -161,  -909, -1142,   -64, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142, -1142,    92, -1142,   834, -1142, -1142,    31,
    2434, -1142, -1142, -1142, -1142, -1142, -1142, -1142, -1142,   -27,
   -1142, -1142, -1142, -1142, -1142, -1142,  -567,  -751, -1142, -1142,
   -1142,   -26,  -770,   741, -1142, -1142, -1142,   352, -1142, -1142,
   -1142, -1142,  -160,  -781,  -151,  -725,  -956, -1142, -1142,  -154,
      38,   212, -1142, -1142, -1142,   -24, -1142, -1142,  -322,    29,
   -1007,  -277,  -309,  -299,  -639, -1142,  -203, -1142,     7,   916,
    -136,   447, -1142,  -488,  -885,  -982, -1142,  -691,  -527, -1141,
   -1121,  -968,   -57, -1142,   114, -1142,  -253,  -473,  -428,   812,
    -495, -1142, -1142, -1142,  1210, -1142,   -13, -1142, -1142,  -229,
   -1142,  -627, -1142, -1142, -1142,  1281,  1352,   -12,   927,   168,
    1894, -1142,  1965,  2036, -1142, -1142, -1142, -1142, -1142, -1142,
   -1142, -1142, -1142,  -421
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   340,  1084,   711,   112,   113,   114,   115,
     116,   117,   358,   492,   118,   254,   119,   341,   705,   563,
     706,   120,   121,   122,   560,   561,   123,   124,   188,   984,
     286,   125,   281,   126,   743,   291,   127,   293,   294,   128,
    1085,   129,   306,   130,   131,   132,   458,   653,   460,   654,
     453,   650,   133,   134,   846,   135,   252,   136,   719,   720,
     200,   138,   139,   140,   141,   142,   143,   566,   797,   955,
     144,   145,   759,   924,   146,   147,   568,   957,   148,   149,
     803,   804,   805,   201,   284,   734,   151,   152,   570,   965,
     810,   987,   988,   701,   702,   703,  1092,   499,   729,   730,
     731,   732,   733,   811,   369,   909,  1222,  1283,  1205,   447,
    1133,  1137,  1199,  1200,  1201,   153,   328,   531,   154,   155,
     287,   288,   503,   504,   747,  1219,  1164,   507,   744,  1242,
    1130,  1046,   342,   217,   346,   347,   448,   449,   450,   202,
     157,   158,   159,   160,   203,   162,   185,   186,   626,   471,
     870,   627,   628,   163,   164,   204,   205,   167,   238,   451,
     207,   169,   208,   209,   172,   173,   174,   175,   352,   176,
     177,   178,   179,   180
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     183,   307,   493,   762,   206,   630,   210,   700,   211,   439,
     109,   745,   310,   735,   218,   475,   226,   421,   370,   231,
     231,   717,   242,   244,   246,   249,   253,  1049,   986,   213,
     989,   960,  1096,   459,  1097,   763,   950,   292,   871,   295,
     296,   758,   446,   484,  1204,   301,   912,   302,  1086,   303,
     350,   289,   518,   289,   873,  1128,   289,   311,   315,   317,
     319,   320,   321,   945,   508,   289,   325,   548,   326,   446,
     528,  1243,    70,   359,  1125,   329,  1135,   440,   331,   332,
     110,   333,   866,   334,   856,   505,   335,   336,   860,   337,
     861,   469,   469,   311,   315,   351,   353,   539,   309,   760,
     553,  -271,   489,  1248,   442,  1267,   424,  -272,   505,  1098,
     425,   435,   586,   -55,   505,  -294,  1162,   821,  1189,   446,
     547,   551,   540,   443,   637,  1268,   241,   243,   245,   554,
     456,  1249,   -55,  1136,   309,   345,   309,   446,   469,   469,
     360,   469,   433,   721,  1288,   994,   546,   928,  1051,  1245,
     435,   637,   374,     3,   457,  -484,  1030,  -294,  1191,   822,
     307,  1134,   509,   845,   722,   549,   512,  1196,   723,   616,
     617,  1160,  1204,  1218,  -484,  1086,  -479,   183,   520,  1196,
     429,   430,   431,   110,  1239,   325,   311,   351,   435,  1087,
     435,   435,   724,   435,   550,   725,   993,    62,   438,   349,
    1086,   592,   435,  1086,  1086,  -484,   726,  -479,   444,  -484,
     231,  -294,   590,  1224,  1225,   181,   315,   228,  1048,   435,
    -479,   854,   549,   255,   586,   727,   297,   309,   309,   506,
    -484,  -484,  -484,  -479,  -254,   921,   446,   446,   316,   455,
    1190,   587,   519,   315,   630,   462,   549,   549,   911,   549,
    1216,   638,   506,  -484,   213,   588,  1197,  -484,   506,  1109,
    1163,    70,  1112,  1113,   184,    23,    23,  1198,  1086,  1069,
    1086,  1086,  -484,   962,   348,  1052,  1246,   922,  1281,  1198,
    -479,  1279,   542,  -484,   589,  -479,   298,   187,  1009,  -384,
      42,  1011,  1012,   315,  -480,   898,   794,  -255,   963,  1286,
     522,   523,  -502,   795,   299,  1070,  1240,   490,   446,   543,
     491,   315,   986,  1153,   989,  1155,    58,    60,    60,   652,
     949,   300,   951,  -502,   590,  -480,   658,  -502,    68,    68,
     591,   923,   541,   881,  1282,   544,   520,   899,  -480,   435,
     181,   446,  -261,   422,   381,   423,   536,   562,   564,   538,
    -502,  -480,   387,    88,    88,   521,   214,  1177,  1149,  1179,
    1180,   572,   545,   573,   574,   575,   577,   579,   580,    23,
     581,   582,   583,   584,   585,  1006,   593,   594,   595,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   435,  1117,   495,   806,   -84,  -480,   315,
     315,   823,   630,  -480,   520,   315,   228,   660,   315,   315,
     315,   315,   619,   621,   629,  1044,   -55,   905,   649,   110,
     896,    60,  -472,   509,   746,   641,   377,  -260,   824,  -575,
     645,   864,    68,   646,   110,   -55,  -638,  -494,  -638,   255,
     875,   878,   571,  -269,  -474,   895,   649,   897,   807,  1132,
     446,   212,   311,  -472,   662,   664,   366,    88,  -494,   808,
     668,   670,  -494,   446,   -84,   673,  -472,   704,   704,   906,
     576,   578,  1067,  1003,   712,  -474,   367,   714,   809,  -472,
     907,  -114,   315,   378,  1021,  -494,   368,   379,  -474,  -120,
     509,   649,  -589,   309,  -116,  -588,   748,   649,  -259,   908,
     295,  -474,  -121,   753,   754,  -639,  -256,  -639,   757,   644,
     798,  1022,   290,   110,   785,   315,   618,   620,  1075,  -589,
    -263,   752,  -588,   356,  1107,  1139,  -472,   757,   789,   640,
     315,  -472,   315,   786,   381,   290,   774,   757,   777,   385,
     779,   781,   387,   509,   929,   931,   390,   790,  -474,   756,
    1131,   509,  -267,  -474,   181,   311,   881,  1140,  1247,   663,
     782,   926,   857,  -450,   881,   669,   538,   881,   756,   672,
     862,   345,   881,   345,  -264,   882,   728,   838,   756,   611,
     612,   425,  -450,  1073,   454,   613,  1211,   304,   614,   615,
     461,  1213,  -638,  -258,  -638,   305,   309,  1152,   381,  1154,
     630,   935,   592,   385,  -713,   363,   387,   322,  -713,   446,
     390,  1271,  1272,  1273,  1274,  1275,  1276,  -262,   323,   662,
     841,   668,   844,   712,   315,   847,   777,   849,   850,  -714,
    -715,  -268,  -270,  -714,  -715,   315,   110,   315,  -710,  -635,
     773,  -635,  -710,  -637,  -257,  -637,  1250,   863,  -265,   865,
    -485,   357,  -636,   629,  -636,   872,   768,   361,   770,  -572,
     990,   371,   446,  -485,  -571,   333,   372,   334,  1220,   373,
    -576,  -577,  1251,   436,  1252,  1253,   852,  -574,  1254,  1255,
     336,  -573,   426,   337,   428,   914,   434,   435,   441,   445,
     524,   452,   464,   530,  -485,   885,   886,  1244,   494,   498,
     515,   516,   527,   532,   290,   290,   290,   290,   290,   290,
     348,  1257,   348,   555,   489,   485,   488,   559,   567,   387,
    -485,   489,   569,   610,   840,   636,   843,  -485,   321,   325,
     351,   647,   643,   648,   315,   721,   649,   656,   659,   707,
     708,   665,   710,   150,   713,   496,   736,   716,  -485,   746,
     737,  1244,   796,   150,   755,   290,   722,   290,   761,   764,
     723,   772,   778,   799,   290,   881,   150,  -485,  1280,   150,
     812,   309,  -485,   562,   813,  -485,   814,   946,   819,   948,
     704,   704,   290,   315,   724,   815,  1287,   725,   816,   354,
    1244,   817,   818,   290,   290,   887,   820,   848,   726,   884,
    1141,  1142,   889,   894,   901,   902,   910,   920,   925,   520,
     958,   629,   961,   964,   156,   853,   992,   727,   998,   150,
    1001,  1004,  1010,  1013,   156,   577,   619,   997,  1037,  1014,
    1000,  1039,  1047,  1045,  1053,  1050,   315,   156,  1074,  1054,
     156,  1077,  1079,  1061,  1065,  -141,   150,  1091,  -148,  -147,
    -117,   150,  -146,  -115,  1018,  1108,  1020,  -118,  1122,  -143,
    -150,  1025,  -151,  -145,  1028,  1057,  -149,   446,   446,  1031,
    -144,  1032,  -152,  1094,  -119,  1095,  1110,  1159,  1035,   721,
    1221,  1249,   934,   150,  1124,  -114,  1111,  -116,   362,   558,
     156,   220,  1258,  1126,  1156,    22,    23,  1212,  1157,  1207,
     722,   290,   215,  1208,   723,   221,   715,    31,   222,  1214,
     311,  1223,  1259,    37,   916,  1278,  1060,   156,  1064,   750,
      42,  1066,   156,  1161,   330,  1076,   150,  1040,   724,   576,
     618,   725,   704,    62,   290,  1169,  1089,  1165,   290,  1043,
     900,  1090,   726,  1284,  1170,  1265,    58,  1277,    60,  1088,
    1270,   309,  1081,   248,   156,  1082,   749,   225,   232,    68,
       0,   727,     0,     0,   489,   489,     0,     0,   489,   489,
       0,  1093,     0,     0,     0,     0,     0,     0,    84,   728,
       0,    86,     0,     0,    88,  -489,  1018,  1020,     0,  1025,
    1028,     0,  1060,  1064,   489,     0,   489,   156,  -489,     0,
       0,     0,     0,     0,  1114,  1115,     0,  1116,     0,   629,
       0,  1118,  1119,     0,     0,  1120,     0,     0,   307,     0,
       0,     0,     0,     0,   150,     0,     0,     0,     0,  -489,
     150,     0,     0,     0,   103,     0,     0,     0,     0,  1138,
     777,   315,   315,     0,     0,     0,  1145,     0,     0,     0,
    1147,     0,     0,     0,     0,  -489,  1151,   704,   704,   704,
     704,     0,  -489,     0,     0,  1158,     0,  -491,     0,     0,
    1193,     0,     0,     0,  1129,     0,     0,     0,     0,     0,
    -491,     0,   150,  -489,     0,     0,     0,  1114,  1171,  1172,
    1118,  1173,  1174,  1175,  1176,   156,     0,   150,     0,     0,
       0,   156,  -489,     0,     0,     0,     0,  -489,     0,  1187,
    -489,  -491,     0,     0,     0,   351,     0,     0,     0,     0,
    1195,     0,     0,     0,  1202,     0,   290,   290,     0,   377,
       0,     0,   290,   290,     0,   290,   290,  -491,     0,     0,
       0,     0,     0,     0,  -491,     0,     0,     0,     0,     0,
       0,     0,     0,   156,     0,     0,   309,     0,     0,     0,
    1264,  1194,     0,     0,     0,  -491,     0,     0,   156,   728,
       0,  1235,  1236,  1237,  1238,     0,   150,   893,     0,     0,
       0,     0,     0,     0,  -491,     0,   378,     0,     0,  -491,
     379,     0,  -491,     0,  1064,     0,     0,  1260,  1261,     0,
       0,     0,     0,     0,     0,   351,     0,   150,     0,     0,
       0,     0,   161,   150,  1235,  1236,  1237,  1238,  1260,  1261,
       0,  1241,   161,     0,     0,   150,     0,   150,     0,   377,
       0,     0,     0,  1266,   777,   161,  1269,   381,   161,     0,
       0,     0,   385,     0,   386,   387,   309,   156,     0,   390,
       0,     0,     0,     0,     0,     0,     0,   397,     0,   953,
       0,     0,     0,   401,   402,   403,     0,     0,     0,   777,
       0,     0,     0,  1064,     0,  1241,     0,     0,   156,     0,
       0,     0,     0,   165,   156,     0,   378,   150,   161,     0,
     379,   150,     0,   165,     0,     0,   156,     0,   156,   150,
       0,     0,     0,     0,     0,     0,   165,     0,     0,   165,
       0,     0,   290,   290,  1241,   161,     0,   220,     0,     0,
     161,    22,    23,     0,     0,     0,   290,     0,   488,     0,
       0,   221,     0,    31,   222,   488,     0,   381,   290,    37,
       0,   290,   385,     0,   386,   387,    42,     0,     0,   390,
       0,     0,   161,     0,   166,     0,     0,   397,   156,   165,
    1041,     0,   156,   956,   166,   403,     0,     0,     0,     0,
     156,     0,    58,     0,    60,     0,     0,   166,  1081,     0,
     166,  1082,     0,   225,  -306,    68,   165,     0,  -306,  -306,
       0,   165,     0,  -306,     0,   161,     0,     0,  -306,     0,
    -306,  -306,     0,     0,    84,     0,  -306,    86,     0,     0,
      88,     0,     0,  -306,     0,   220,  -306,     0,  1078,    22,
      23,     0,     0,   165,     0,     0,     0,     0,     0,   221,
     166,    31,   222,     0,     0,     0,  -306,    37,     0,  -306,
     836,  -306,     0,  -306,    42,  -306,  -306,     0,  -306,     0,
    -306,   150,  -306,     0,   150,     0,     0,   166,     0,   -53,
     103,     0,   166,     0,     0,     0,   165,     0,     0,     0,
      58,  -306,    60,     0,  -306,     0,   223,  -306,   -53,   224,
       0,   225,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   161,   166,     0,     0,     0,     0,   161,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,   150,     0,     0,     0,   219,     0,   150,
     290,     0,   156,     0,     0,   156,     0,  -306,     0,     0,
       0,   220,     0,  -306,     0,    22,    23,   166,     0,     0,
       0,     0,     0,     0,     0,   221,     0,    31,   222,     0,
       0,   161,     0,    37,     0,     0,     0,     0,   103,     0,
      42,     0,     0,     0,   165,     0,   161,     0,     0,     0,
     165,     0,     0,     0,     0,   -53,     0,     0,   488,   488,
       0,     0,   488,   488,   156,     0,    58,     0,    60,     0,
     156,     0,   223,     0,   -53,   224,     0,   225,     0,    68,
    1203,     0,   377,     0,     0,     0,     0,   150,   488,   150,
     488,     0,     0,     0,   150,     0,     0,   150,    84,     0,
    1215,    86,   165,   991,    88,     0,     0,     0,   150,     0,
     150,     0,     0,   150,     0,   166,     0,   165,     0,     0,
       0,   166,     0,     0,     0,   161,     0,     0,     0,   995,
     996,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,   150,   379,     0,     0,     0,     0,     0,   150,
       0,   150,     0,     0,   103,     0,   161,     0,   156,     0,
     156,     0,   161,     0,     0,   156,     0,     0,   156,     0,
       0,     0,     0,   166,   161,     0,   161,     0,     0,   156,
       0,   156,     0,     0,   156,     0,     0,     0,   166,     0,
     381,   382,     0,   383,   384,   385,   165,   386,   387,     0,
       0,     0,   390,     0,     0,     0,     0,     0,  1203,     0,
     397,     0,     0,   156,     0,     0,   401,   402,   403,     0,
     156,     0,   156,     0,     0,     0,     0,   165,     0,     0,
       0,     0,     0,   165,     0,     0,   161,   150,     0,     0,
     161,     0,   256,     0,   150,   165,     0,   165,   161,     0,
       0,     0,     0,     0,     0,     0,   257,   258,   150,   259,
       0,     0,     0,     0,   260,     0,     0,   166,     0,     0,
       0,     0,     0,   261,     0,     0,   150,     0,     0,   262,
       0,     0,   150,     0,     0,   263,   150,     0,     0,   264,
       0,     0,   265,     0,     0,     0,     0,     0,   166,     0,
       0,     0,   266,     0,   166,     0,     0,   165,   156,   267,
     268,   165,     0,     0,     0,   156,   166,   269,   166,   165,
       0,     0,     0,     0,     0,     0,   270,     0,     0,   156,
       0,     0,     0,     0,     0,   271,   272,     0,   273,     0,
     274,     0,   275,   150,     0,   276,     0,   156,   256,   277,
     500,     0,   278,   156,     0,   279,     0,   156,     0,     0,
       0,     0,   257,   258,     0,   259,     0,     0,     0,     0,
     260,     0,   377,     0,     0,     0,   168,     0,   166,   261,
       0,     0,   166,     0,     0,   262,   168,     0,     0,     0,
     166,   263,     0,     0,     0,   264,     0,     0,   265,   168,
     161,     0,   168,   161,   150,   150,   150,   150,   266,     0,
       0,   501,     0,     0,   156,   267,   268,     0,     0,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,   378,
     150,   150,   270,   379,     0,     0,     0,     0,     0,     0,
       0,   271,   272,     0,   273,     0,   274,   170,   275,     0,
       0,   276,   168,     0,     0,   277,     0,   170,   278,     0,
       0,   279,   161,     0,     0,     0,     0,     0,   161,     0,
     170,   165,     0,   170,   165,   156,   156,   156,   156,   168,
     381,   382,     0,     0,   168,   385,     0,   386,   387,     0,
       0,     0,   390,     0,     0,     0,     0,     0,     0,     0,
     397,   156,   156,     0,     0,     0,   401,   402,   403,     0,
       0,     0,     0,     0,     0,   435,   168,   952,   171,     0,
       0,     0,     0,   170,     0,     0,     0,     0,   171,     0,
       0,   377,     0,   165,     0,     0,     0,     0,     0,   165,
       0,   171,   166,     0,   171,   166,     0,     0,     0,     0,
     170,     0,     0,     0,     0,   170,   161,     0,   161,   168,
       0,     0,     0,   161,     0,     0,   161,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   161,     0,   161,
       0,     0,   161,     0,     0,     0,     0,   170,   378,     0,
       0,     0,   379,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,     0,     0,     0,     0,     0,
     166,   161,     0,     0,     0,     0,     0,     0,   161,     0,
     161,   171,     0,     0,     0,     0,   171,   165,     0,   165,
     170,     0,     0,     0,   165,     0,     0,   165,     0,   381,
     382,     0,     0,   384,   385,     0,   386,   387,   165,     0,
     165,   390,     0,   165,   586,     0,     0,   168,   171,   397,
       0,     0,     0,   168,     0,   401,   402,   403,   825,     0,
       0,   826,     0,     0,     0,     0,   827,     0,     0,     0,
       0,     0,   165,     0,     0,     0,     0,   191,     0,   165,
       0,   165,     0,     0,     0,     0,     0,   828,   166,     0,
     166,   171,     0,     0,   829,   166,   161,     0,   166,     0,
       0,     0,     0,   161,   830,   168,     0,     0,     0,   166,
       0,   166,   831,     0,   166,     0,     0,   161,   170,     0,
     168,     0,     0,     0,   170,     0,     0,     0,   832,     0,
       0,     0,     0,     0,     0,   161,     0,     0,     0,     0,
     833,   161,     0,   166,   590,   161,     0,     0,     0,     0,
     166,   834,   166,     0,     0,     0,   220,   835,     0,     0,
      22,    23,     0,     0,     0,  1080,     0,   165,     0,     0,
     221,     0,    31,   222,   165,     0,   170,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,   165,   171,
       0,   170,     0,     0,     0,   171,     0,     0,     0,   168,
       0,     0,   161,     0,     0,     0,   165,     0,     0,     0,
       0,    58,   165,    60,     0,    62,   165,  1081,     0,     0,
    1082,     0,   225,     0,    68,     0,     0,     0,     0,     0,
     168,     0,     0,     0,     0,     0,   168,     0,   166,   377,
       0,     0,     0,    84,     0,   166,    86,   171,   168,    88,
     168,     0,     0,     0,     0,     0,     0,     0,     0,   166,
       0,     0,   171,   161,   161,   161,   161,     0,     0,     0,
     170,     0,     0,   165,     0,     0,     0,   166,     0,     0,
       0,     0,     0,   166,     0,     0,     0,   166,     0,   161,
     161,     0,     0,     0,     0,     0,   378,     0,     0,   103,
     379,   170,     0,     0,     0,  1083,   137,   170,     0,     0,
     168,     0,     0,     0,   168,     0,   137,     0,     0,   170,
       0,   170,   168,     0,     0,     0,     0,     0,     0,   137,
       0,     0,   137,     0,   165,   165,   165,   165,     0,     0,
       0,   171,     0,     0,   166,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,     0,   390,
     165,   165,     0,     0,     0,     0,   396,   397,     0,     0,
     400,     0,   171,   401,   402,   403,     0,   111,   171,     0,
       0,   170,   137,     0,   404,   170,     0,   182,     0,     0,
     171,     0,   171,   170,     0,     0,     0,     0,   256,     0,
     216,     0,     0,   227,     0,   166,   166,   166,   166,   137,
       0,     0,   257,   258,   137,   259,     0,     0,     0,     0,
     260,     0,     0,     0,     0,     0,     0,     0,     0,   261,
       0,   166,   166,     0,     0,   262,     0,     0,     0,     0,
       0,   263,     0,     0,     0,   264,     0,     0,   265,     0,
       0,     0,   171,   324,     0,     0,   171,     0,   266,     0,
       0,     0,     0,     0,   171,   267,   268,     0,     0,     0,
       0,     0,     0,   269,   168,     0,     0,   168,     0,     0,
     111,     0,   270,     0,     0,   355,     0,     0,     0,   137,
       0,   271,   272,     0,   273,     0,   274,     0,   275,     0,
       0,   276,     0,     0,     0,   277,     0,     0,   278,     0,
       0,   279,     0,     0,     0,     0,     0,     0,   220,     0,
       0,     0,    22,    23,     0,     0,     0,  1080,     0,     0,
       0,     0,   221,     0,    31,   222,   168,     0,     0,     0,
      37,     0,   168,     0,     0,   170,     0,    42,   170,   220,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
     427,     0,     0,   221,     0,    31,   222,   285,     0,     0,
       0,    37,     0,    58,     0,    60,     0,    62,    42,  1081,
       0,     0,  1082,     0,   225,     0,    68,   137,     0,     0,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    84,    60,   170,    86,     0,
       0,    88,     0,   170,     0,   225,   171,    68,     0,   171,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     168,     0,   168,     0,     0,     0,    84,   168,     0,    86,
     168,     0,    88,     0,     0,   137,     0,     0,     0,     0,
       0,   168,     0,   168,     0,     0,   168,     0,   497,     0,
     137,   103,     0,     0,   510,     0,     0,  1178,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,     0,     0,     0,   171,   168,     0,     0,     0,     0,
       0,     0,   168,     0,   168,     0,     0,     0,     0,     0,
       0,   170,     0,   170,     0,     0,     0,     0,   170,     0,
       0,   170,   280,   282,   283,     0,   111,     0,     0,     0,
       0,     0,   170,     0,   170,     0,     0,   170,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,     0,   137,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   327,     0,     0,     0,   170,     0,     0,     0,
       0,     0,     0,   170,     0,   170,     0,     0,     0,     0,
     137,     0,   171,     0,   171,     0,   137,     0,     0,   171,
     168,     0,   171,     0,     0,   256,     0,   168,   137,     0,
     137,     0,     0,   171,     0,   171,     0,     0,   171,   257,
     258,   168,   259,     0,     0,     0,     0,   260,     0,     0,
     111,     0,     0,     0,     0,   364,   261,   365,     0,   168,
       0,     0,   262,     0,     0,   168,     0,   171,   263,   168,
       0,     0,   264,     0,   171,   265,   171,   318,     0,     0,
       0,   510,     0,     0,     0,   266,     0,   510,     0,     0,
     137,   170,   267,   268,   137,     0,     0,     0,   170,   709,
     269,     0,   137,   432,     0,     0,     0,     0,     0,   270,
       0,     0,   170,     0,     0,     0,     0,     0,   271,   272,
     327,   273,     0,   274,     0,   275,   168,     0,   276,     0,
     170,     0,   277,     0,     0,   278,   170,     0,   279,     0,
     170,     0,     0,     0,   327,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,   483,
       0,   771,   171,     0,     0,   776,     0,     0,     0,   171,
       0,     0,     0,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,     0,     0,   168,   168,   168,
     168,     0,     0,   502,   903,     0,     0,   170,     0,     0,
       0,   171,     0,     0,     0,     0,     0,   171,     0,     0,
       0,   171,     0,   168,   168,   220,     0,     0,     0,    22,
      23,     0,     0,     0,  1080,     0,     0,     0,     0,   221,
       0,    31,   222,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,    42,     0,   465,   468,   470,   474,
     476,   478,     0,     0,   137,     0,     0,   137,   170,   170,
     170,   170,     0,     0,     0,     0,     0,     0,   171,   565,
      58,     0,    60,     0,    62,     0,  1081,     0,     0,  1082,
       0,   225,     0,    68,   170,   170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   511,     0,   513,
       0,     0,    84,     0,     0,    86,   517,     0,    88,     0,
       0,     0,     0,     0,     0,     0,   137,     0,     0,     0,
       0,     0,   137,     0,   526,   888,     0,     0,   891,   171,
     171,   171,   171,     0,     0,   533,   534,     0,     0,     0,
       0,     0,   639,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   171,     0,   103,     0,
       0,   651,     0,     0,  1181,     0,   655,     0,   657,     0,
       0,     0,     0,     0,     0,     0,     0,   375,     0,     0,
       0,     0,     0,     0,   376,     0,     0,   927,     0,     0,
       0,     0,     0,   932,     0,     0,     0,   377,   472,     0,
       0,     0,     0,   718,     0,     0,     0,     0,     0,   502,
     137,     0,   137,   473,     0,     0,     0,   137,     0,     0,
     137,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,   137,     0,   137,     0,     0,   137,     0,     0,     0,
       0,     0,     0,   642,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,     0,     0,     0,   379,     0,
       0,     0,     0,     0,     0,   137,     0,   784,     0,     0,
       0,     0,   137,     0,   137,     0,   666,   802,     0,     0,
     671,   510,     0,   510,     0,   469,     0,     0,   510,   380,
       0,   510,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1033,     0,  1034,   381,   382,  1036,   383,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,   392,
     393,     0,   394,   395,   396,   397,   398,   399,   400,     0,
       0,   401,   402,   403,     0,   435,  1056,     0,     0,     0,
       0,     0,   404,  1059,     0,  1063,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   855,     0,
     137,   858,   859,     0,     0,     0,     0,   137,   220,     0,
       0,     0,    22,    23,     0,     0,     0,  1080,     0,     0,
       0,   137,   221,     0,    31,   222,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    42,     0,   137,
       0,     0,     0,     0,     0,   137,     0,     0,     0,   137,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,  1081,
       0,     0,  1082,   586,   225,     0,    68,     0,     0,     0,
     904,     0,   674,     0,   502,     0,     0,     0,     0,   675,
     587,     0,  1123,     0,     0,    84,     0,     0,    86,     0,
     917,    88,     0,     0,   588,     0,   137,     0,   867,   869,
    1143,     0,   327,     0,   874,   877,  1146,   879,   880,     0,
    1148,     0,     0,     0,     0,   676,     0,     0,     0,     0,
       0,     0,     0,   589,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   954,     0,     0,     0,     0,   802,
       0,   103,     0,     0,     0,     0,     0,  1182,     0,   985,
       0,     0,     0,     0,     0,     0,     0,   137,   137,   137,
     137,     0,     0,   590,     0,     0,     0,  1188,     0,   591,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   137,   137,   487,   406,   407,   408,   409,
     410,     0,     0,   413,   414,   415,   416,     0,   418,   419,
     936,   937,   938,   939,   940,   682,     0,   683,     0,     0,
       0,   684,   685,   686,   687,   688,   689,   690,   691,   941,
     693,   694,     0,   942,     0,     0,   696,   697,   943,   699,
     944,     0,  1038,     0,   718,     0,     0,     0,  1042,     0,
       0,   502,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -596,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1007,  1008,   420,     0,     0,  -635,
     954,  -635,     0,     0,     0,     0,     0,   802,  1015,     0,
     802,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1026,    -2,     4,  1029,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -82,     0,
      40,    41,    42,     0,    43,  -323,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -53,    53,    54,
       0,    55,    56,    57,     0,  -323,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -323,   -53,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -82,    88,    89,     0,     0,
      90,     0,    91,   985,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     0,   108,     4,
       0,     5,  1144,     6,     7,     8,     9,    10,    11,     0,
    -665,     0,    12,    13,    14,    15,    16,  -665,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    -665,    28,    29,  -665,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,  -665,    68,    69,
      70,  -665,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,  -665,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -665,  -665,
      95,  -665,  -665,  -665,  -665,  -665,  -665,  -665,     0,  -665,
    -665,  -665,  -665,  -665,     0,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,   103,  -665,  -665,  -665,     0,   105,  -665,
     106,     0,   107,     0,   338,  -665,     5,   308,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   339,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,   537,
     106,     0,   107,     0,   556,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   557,   106,     0,   107,     0,   338,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,   339,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
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
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,   354,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
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
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
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
      35,    36,    37,   661,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
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
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
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
      35,    36,    37,  1017,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1019,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
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
      35,    36,    37,  1024,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1027,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,  1055,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1058,    39,
     -82,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -53,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -53,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -82,    88,    89,
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
      35,    36,    37,  1062,    39,   -82,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -53,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -53,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -82,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,  1262,   108,     5,   308,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   189,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,   194,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,  1263,   913,
     108,     5,   308,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,   674,     0,   189,     0,     0,    15,    16,
     675,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,   676,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,   487,   406,   407,   408,
     409,   410,     0,     0,   413,   414,   415,   416,     0,   418,
     419,   677,   678,   679,   680,   681,   682,     0,   683,     0,
      98,     0,   684,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   100,   695,   102,     0,   696,   697,   698,
     699,   198,     0,   106,     0,   199,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,    13,
       0,    15,    16,     0,    17,     0,   190,    19,    20,    21,
       0,     0,     0,     0,    26,     0,     0,    28,    29,     0,
     191,     0,     0,     0,    33,    34,    35,    36,     0,    38,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,    49,    50,    51,    52,
       0,    53,    54,     0,    55,    56,    57,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,    90,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,   247,    23,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,    60,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,    68,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,    88,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     0,   108,     5,   308,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,   312,
     313,     0,    85,   343,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,   344,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     0,   108,
       5,   308,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,   312,   313,     0,    85,   343,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
     767,   199,     0,     0,   108,     5,   308,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,   312,
     313,     0,    85,   343,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,   769,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,   622,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,   623,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,   624,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     625,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,   233,    10,    11,   234,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   235,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,   236,    85,     0,
     237,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,   312,   313,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   314,     0,     0,   104,
       0,   198,     0,   106,     0,   199,     0,     0,   108,     5,
     308,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,   312,   313,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,   915,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,   312,   313,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,   239,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,   240,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,   250,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     251,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     0,   108,     5,   308,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   189,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,   240,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   199,     0,     5,   108,     6,     7,
       8,   233,    10,    11,     0,     0,     0,   189,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,   194,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   235,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,   236,    85,     0,   237,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,   312,   313,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     0,   108,     5,   308,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,   765,   106,     0,   199,     0,     0,   108,     5,
     308,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,   775,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,  -709,     0,
       0,     0,  -709,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     0,   108,     5,   308,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   198,     0,   106,     0,   199,  1192,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,   240,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   189,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   289,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     105,     0,   106,     0,   199,     0,     0,   108,     5,   308,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,   780,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,   947,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     0,   199,     0,     0,   108,     5,
     308,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     738,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,   739,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,   740,    75,    76,    77,   741,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
    1127,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
    1150,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     0,   108,     5,   308,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   198,     0,   106,     0,  1127,     0,
       0,   108,     5,   308,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,  1106,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,   229,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   230,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   738,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,   739,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,   740,    75,    76,    77,   741,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   742,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     0,   742,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   839,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   198,     0,   106,     0,   199,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   842,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   189,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,  1100,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   199,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   189,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,  1101,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,  1103,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,  1104,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,  1105,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   189,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
    1106,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   738,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,   739,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,   740,    75,    76,    77,   741,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,  1217,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   189,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   966,     0,   967,     0,     0,
       0,    93,    94,    95,    96,    97,   674,     0,     0,     0,
      98,   968,   258,   675,   969,     0,     0,     0,     0,   970,
      99,     0,     0,   100,   101,   102,     0,     0,   261,   104,
     191,     0,     0,   106,   971,  1217,     0,     0,   108,     0,
     972,     0,     0,     0,   264,     0,     0,   973,     0,   676,
       0,     0,     0,     0,     0,     0,     0,   974,     0,     0,
       0,     0,     0,     0,   975,   976,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   977,     0,     0,     0,     0,     0,     0,     0,     0,
     271,   272,     0,   978,     0,   274,     0,   979,     0,     0,
     980,     0,     0,     0,   981,     0,     0,   278,     0,     0,
     982,     0,     0,     0,     0,     0,     0,     0,     0,   487,
     406,   407,   408,   409,   410,     0,     0,   413,   414,   415,
     416,     0,   418,   419,   936,   937,   938,   939,   940,   682,
       0,   683,     0,     0,     0,   684,   685,   686,   687,   688,
     689,   690,   691,   941,   693,   694,     0,   942,     0,     0,
     696,   697,   943,   699,     0,     5,   983,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   189,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,     0,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     5,   308,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   189,     0,
       0,    15,    16,     0,    17,     0,   190,     0,   377,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,   378,    63,     0,     0,   379,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,  -493,
       0,     0,     0,     0,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,     0,    95,     0,   390,     0,
    -493,     0,     0,     0,  -493,   396,   397,     0,     0,   400,
       0,     0,   401,   402,   403,     0,     0,     0,   102,     0,
       0,     0,     0,     0,   198,     0,   106,  -493,  1127,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     189,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,  -475,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,  -475,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,  -475,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,  -475,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   189,     0,     0,    15,    16,     0,    17,     0,   190,
     102,  -475,    21,     0,     0,     0,  -475,     0,   106,     0,
    1217,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,    95,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   189,     0,     0,    15,    16,     0,    17,     0,
     190,   102,     0,    21,     0,     0,     0,   198,     0,   106,
       0,   199,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   189,     0,     0,    15,    16,     0,    17,
       0,   190,   102,     0,    21,     0,     0,     0,   198,     0,
     106,     0,   742,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,  1167,     0,    85,
       0,     0,    87,     0,     0,    89,   674,     0,     0,     0,
       0,   257,   258,   675,   259,     0,     0,     0,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,   261,     0,
       0,    95,     0,     0,   971,     0,     0,     0,     0,     0,
     263,     0,     0,     0,   264,     0,     0,   265,     0,   676,
       0,     0,     0,     0,     0,     0,     0,   266,     0,     0,
       0,   106,     0,   199,   975,   268,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,   375,
     271,   272,     0,   273,     0,   274,   376,  1168,     0,     0,
     980,     0,     0,     0,   277,     0,     0,   278,     0,   377,
     279,     0,     0,     0,     0,     0,     0,     0,     0,   487,
     406,   407,   408,   409,   410,     0,     0,   413,   414,   415,
     416,     0,   418,   419,   936,   937,   938,   939,   940,   682,
       0,   683,     0,     0,     0,   684,   685,   686,   687,   688,
     689,   690,   691,   941,   693,   694,     0,   942,     0,     0,
     696,   697,   943,   699,     0,   375,   378,     0,     0,     0,
     379,     0,   376,     0,     0,     0,     0,     0,     0,   486,
       0,     0,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,   487,   406,   407,   408,   409,   410,     0,     0,
     413,   414,   415,   416,     0,   418,   419,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   375,   378,   401,   402,   403,   379,   435,   376,     0,
       0,     0,     0,     0,   404,  1016,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   380,   487,   406,
     407,   408,   409,   410,     0,     0,   413,   414,   415,   416,
       0,   418,   419,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,   392,   393,     0,
     394,   395,   396,   397,   398,   399,   400,   375,   378,   401,
     402,   403,   379,   435,   376,     0,     0,     0,     0,     0,
     404,  1023,     0,     0,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,   487,   406,   407,   408,   409,   410,
       0,     0,   413,   414,   415,   416,     0,   418,   419,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   375,   378,   401,   402,   403,   379,   435,
     376,     0,     0,     0,     0,     0,   404,  1183,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   380,
     487,   406,   407,   408,   409,   410,     0,     0,   413,   414,
     415,   416,     0,   418,   419,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,   392,
     393,     0,   394,   395,   396,   397,   398,   399,   400,   375,
     378,   401,   402,   403,   379,   435,   376,     0,     0,     0,
       0,     0,   404,  1184,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   380,   487,   406,   407,   408,
     409,   410,     0,     0,   413,   414,   415,   416,     0,   418,
     419,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,   392,   393,     0,   394,   395,
     396,   397,   398,   399,   400,   375,   378,   401,   402,   403,
     379,   435,   376,     0,     0,     0,     0,     0,   404,  1185,
       0,     0,     0,     0,     0,   377,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,   487,   406,   407,   408,   409,   410,     0,     0,
     413,   414,   415,   416,     0,   418,   419,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   375,   378,   401,   402,   403,   379,   435,   376,     0,
       0,     0,     0,     0,   404,  1186,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   380,   487,   406,
     407,   408,   409,   410,     0,     0,   413,   414,   415,   416,
       0,   418,   419,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,   392,   393,     0,
     394,   395,   396,   397,   398,   399,   400,   375,   378,   401,
     402,   403,   379,   435,   376,     0,     0,     0,     0,     0,
     404,  1209,     0,     0,     0,     0,     0,   377,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,   487,   406,   407,   408,   409,   410,
       0,     0,   413,   414,   415,   416,     0,   418,   419,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,     0,   378,   401,   402,   403,   379,   435,
       0,     0,     0,     0,     0,     0,   404,  1210,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   377,     0,   380,
     487,   406,   407,   408,   409,   410,     0,     0,   413,   414,
     415,   416,     0,   418,   419,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,   392,
     393,     0,   394,   395,   396,   397,   398,   399,   400,   375,
       0,   401,   402,   403,     0,   435,   376,     0,     0,     0,
       0,     0,   404,     0,   378,     0,     0,     0,   379,   377,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   467,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     289,     0,     0,     0,     0,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,   477,   378,   390,   391,   392,
     379,     0,   394,   395,   396,   397,     0,     0,   400,     0,
       0,   401,   402,   403,     0,     0,     0,     0,     0,     0,
       0,     0,   404,     0,     0,     0,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,   378,     0,     0,     0,
     379,     0,     0,     0,     0,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   380,     0,   401,   402,   403,     0,   435,     0,     0,
       0,     0,     0,     0,   404,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   375,     0,   401,   402,   403,     0,   435,   376,     0,
       0,     0,     0,     0,   404,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,   791,     0,     0,     0,     0,     0,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,   480,     0,     0,     0,   792,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,     0,   379,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,   376,     0,     0,     0,     0,     0,     0,
       0,     0,   793,     0,     0,     0,   377,   876,     0,     0,
       0,     0,     0,   380,     0,     0,     0,     0,   378,     0,
       0,     0,   379,     0,     0,     0,     0,     0,     0,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   380,   481,   401,   402,   403,     0,     0,
       0,     0,     0,   378,     0,     0,   404,   379,     0,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,     0,   469,   401,   402,   403,   380,     0,
       0,     0,     0,     0,     0,     0,   404,     0,     0,     0,
       0,     0,     0,     0,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,   392,   393,
       0,   394,   395,   396,   397,   398,   399,   400,   375,     0,
     401,   402,   403,     0,   435,   376,     0,     0,     0,     0,
       0,   404,     0,     0,     0,     0,     0,     0,   377,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,   375,     0,
       0,     0,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   289,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,   375,   379,
       0,     0,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   631,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
     380,     0,     0,     0,   632,   378,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     380,     0,   401,   402,   403,   378,     0,     0,     0,   379,
       0,     0,     0,   404,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     380,     0,   401,   402,   403,     0,   435,     0,     0,     0,
       0,     0,     0,   404,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     375,   220,   401,   402,   403,    22,    23,   376,     0,     0,
       0,     0,     0,   404,     0,   221,     0,    31,   222,     0,
     377,   633,     0,    37,     0,     0,   -83,     0,     0,     0,
      42,     0,     0,     0,     0,     0,   634,     0,     0,     0,
     375,     0,     0,     0,     0,   -54,     0,   376,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
     377,   868,     0,     0,   -54,     0,     0,   225,     0,    68,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
     375,   379,     0,     0,     0,     0,     0,   376,    84,     0,
       0,    86,     0,   -83,    88,     0,     0,     0,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,   378,     0,     0,
       0,   379,     0,     0,     0,     0,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,   378,     0,     0,
       0,   379,     0,     0,     0,   404,     0,     0,   381,   382,
     892,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,     0,   435,     0,
       0,     0,     0,     0,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   375,     0,   401,   402,   403,     0,   435,   376,
       0,     0,     0,     0,     0,   404,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   220,     0,     0,     0,    22,    23,
       0,     0,   375,     0,     0,     0,     0,     0,   221,   376,
      31,   222,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,   377,    42,     0,     0,     0,  1071,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -54,   378,
       0,     0,     0,   379,     0,     0,     0,   375,   514,    58,
       0,    60,     0,     0,   376,     0,     0,   -54,     0,     0,
     225,     0,    68,  1072,     0,     0,     0,   377,     0,     0,
     469,     0,     0,     0,   380,     0,     0,     0,     0,   378,
       0,    84,     0,   379,    86,     0,     0,    88,     0,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,     0,
       0,     0,     0,     0,   378,   930,     0,   404,   379,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   375,     0,   401,   402,   403,   380,
       0,   376,     0,     0,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,   377,   381,   382,     0,   383,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,   392,
     393,     0,   394,   395,   396,   397,   398,   399,   400,   375,
       0,   401,   402,   403,     0,     0,   376,     0,     0,     0,
       0,     0,   404,     0,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,   379,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,   376,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
       0,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,     0,     0,
     379,     0,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,     0,     0,   401,   402,
     403,   380,   525,     0,     0,     0,   378,     0,     0,   404,
     379,     0,     0,     0,     0,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   380,   529,   401,   402,   403,     0,     0,     0,     0,
       0,     0,     0,     0,   404,     0,     0,   381,   382,     0,
     383,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,   392,   393,     0,   394,   395,   396,   397,   398,   399,
     400,   375,   535,   401,   402,   403,     0,     0,   376,     0,
       0,     0,     0,     0,   404,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
       0,   375,   379,     0,     0,     0,     0,     0,   376,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   377,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   552,   380,     0,     0,     0,     0,   378,     0,
       0,     0,   379,     0,     0,     0,     0,     0,     0,   381,
     382,   635,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   380,     0,   401,   402,   403,   378,     0,
       0,     0,   379,     0,     0,     0,   404,     0,     0,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   380,     0,   401,   402,   403,     0,     0,
       0,     0,     0,     0,     0,     0,   404,     0,     0,   381,
     382,     0,   383,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,   392,   393,     0,   394,   395,   396,   397,
     398,   399,   400,   375,   787,   401,   402,   403,     0,     0,
     376,     0,   766,     0,     0,     0,   404,   220,     0,     0,
       0,    22,    23,   377,     0,     0,  1080,     0,     0,     0,
       0,   221,     0,    31,   222,     0,     0,     0,     0,    37,
       0,     0,     0,   375,     0,     0,    42,     0,     0,     0,
     376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   377,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,  1081,     0,
     378,  1082,     0,   225,   379,    68,     0,     0,   375,     0,
       0,     0,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,    86,   377,     0,
      88,     0,     0,     0,     0,   380,     0,     0,     0,     0,
     378,     0,     0,     0,   379,     0,     0,     0,     0,     0,
       0,   381,   382,     0,   383,   384,   385,     0,   386,   788,
     388,     0,   389,   390,   391,   392,   393,     0,   394,   395,
     396,   397,   398,   399,   400,   380,     0,   401,   402,   403,
     103,     0,     0,     0,     0,   378,  1232,     0,   404,   379,
       0,   381,   382,     0,   383,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,   392,   393,  -597,   394,   395,
     396,   397,   398,   399,   400,   375,   184,   401,   402,   403,
     380,     0,   376,     0,     0,     0,     0,     0,   404,     0,
       0,     0,     0,     0,     0,   377,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     375,   883,   401,   402,   403,     0,     0,   376,     0,     0,
       0,     0,     0,   404,     0,     0,     0,     0,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,   379,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     377,     0,     0,   890,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,   379,     0,   381,   382,     0,   383,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,   392,   393,     0,
     394,   395,   396,   397,   398,   399,   400,     0,   851,   401,
     402,   403,   380,     0,     0,     0,     0,   378,     0,     0,
     404,   379,     0,     0,     0,     0,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   375,     0,   401,   402,   403,     0,     0,   376,
       0,     0,     0,     0,     0,   404,     0,     0,     0,     0,
       0,     0,   377,   220,     0,     0,     0,    22,    23,     0,
       0,     0,  1080,     0,     0,     0,     0,   221,     0,    31,
     222,     0,   375,     0,     0,    37,     0,     0,     0,   376,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,   378,
      60,     0,    62,   379,  1081,     0,     0,  1082,   375,   225,
       0,    68,     0,     0,     0,   376,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   377,     0,
      84,     0,     0,    86,   380,     0,    88,     0,     0,   378,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,     0,
       0,     0,     0,   918,     0,   378,   103,   404,     0,   379,
     381,   382,  1233,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   375,     0,   401,   402,   403,   933,
     380,   376,     0,   919,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,   377,   999,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     375,     0,   401,   402,   403,     0,     0,   376,     0,     0,
       0,     0,     0,   404,     0,     0,     0,     0,     0,     0,
     377,  1002,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,     0,   379,     0,     0,     0,     0,
     375,     0,     0,     0,     0,     0,     0,   376,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     377,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   378,     0,     0,
       0,   379,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,     0,     0,   401,   402,
     403,     0,   380,     0,     0,     0,     0,   378,     0,   404,
       0,   379,     0,     0,     0,     0,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,     0,     0,     0,
       0,     0,     0,     0,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   375,  1068,   401,   402,   403,     0,     0,   376,
       0,     0,     0,  1005,     0,   404,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   375,     0,     0,     0,     0,     0,     0,   376,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,   375,   379,     0,     0,     0,     0,     0,   376,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,   378,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
     381,   382,  1099,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,   378,
       0,     0,     0,   379,     0,     0,     0,   404,     0,     0,
     381,   382,  1102,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,     0,
       0,     0,     0,     0,     0,     0,     0,   404,     0,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   375,  1121,   401,   402,   403,     0,
       0,   376,     0,     0,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   378,     0,     0,   375,   379,     0,     0,     0,     0,
       0,   376,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,   378,     0,     0,     0,   379,     0,     0,     0,     0,
       0,     0,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,   380,     0,   401,   402,
     403,   378,     0,     0,     0,   379,     0,     0,     0,   404,
       0,     0,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,   380,     0,   401,   402,
     403,     0,     0,     0,     0,  1166,     0,     0,     0,   404,
       0,     0,   381,   382,     0,   383,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,   392,   393,     0,   394,
     395,   396,   397,   398,   399,   400,   375,   256,   401,   402,
     403,     0,   959,   376,     0,     0,     0,  1206,     0,   404,
       0,   257,   258,     0,   259,     0,   377,     0,     0,   260,
       0,     0,     0,     0,     0,     0,     0,     0,   261,     0,
       0,     0,     0,     0,   262,     0,   375,     0,     0,     0,
     263,     0,     0,   376,   264,     0,     0,   265,     0,     0,
       0,     0,     0,     0,     0,     0,   377,   266,     0,     0,
       0,     0,     0,     0,   267,   268,     0,     0,     0,     0,
       0,     0,   269,   378,     0,     0,   375,   379,     0,     0,
       0,   270,     0,   376,     0,     0,  1226,     0,     0,     0,
     271,   272,     0,   273,     0,   274,   377,   275,     0,     0,
     276,     0,     0,     0,   277,     0,     0,   278,   380,     0,
     279,     0,     0,   378,     0,     0,     0,   379,     0,     0,
       0,     0,     0,     0,   381,   382,  1227,   383,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,   392,   393,
       0,   394,   395,   396,   397,   398,   399,   400,   380,     0,
     401,   402,   403,   378,     0,     0,     0,   379,     0,     0,
       0,   404,     0,     0,   381,   382,  1228,   383,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,   392,   393,
       0,   394,   395,   396,   397,   398,   399,   400,   380,     0,
     401,   402,   403,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,     0,   381,   382,     0,   383,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,   392,   393,
       0,   394,   395,   396,   397,   398,   399,   400,   375,   256,
     401,   402,   403,     0,     0,   376,     0,     0,     0,     0,
       0,   404,     0,   257,   258,     0,   259,     0,   377,     0,
       0,   260,     0,     0,     0,     0,     0,   801,     0,     0,
     261,     0,     0,     0,     0,     0,   262,     0,   375,     0,
       0,     0,   263,     0,     0,   376,   264,     0,     0,   265,
       0,     0,     0,     0,     0,     0,     0,     0,   377,   266,
       0,     0,     0,     0,     0,     0,   267,   268,     0,     0,
       0,     0,     0,     0,   269,   378,     0,     0,   375,   379,
       0,     0,     0,   270,     0,   376,     0,     0,  1229,     0,
       0,     0,   271,   272,     0,   273,     0,   274,   377,   275,
       0,     0,   276,     0,     0,     0,   277,     0,     0,   278,
     380,     0,   279,     0,     0,   378,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,   381,   382,  1230,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     380,     0,   401,   402,   403,   378,     0,     0,     0,   379,
       0,     0,     0,   404,     0,     0,   381,   382,  1231,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     380,     0,   401,   402,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,     0,   381,   382,     0,   383,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
     392,   393,     0,   394,   395,   396,   397,   398,   399,   400,
     375,  -301,   401,   402,   403,     0,     0,   376,     0,     0,
       0,     0,     0,   404,     0,  -301,  -301,     0,  -301,     0,
     377,     0,     0,  -301,     0,     0,     0,     0,     0,     0,
       0,     0,  -301,     0,     0,     0,     0,     0,  -301,     0,
     375,     0,     0,     0,  -301,     0,     0,   376,  -301,  1256,
       0,  -301,     0,     0,     0,     0,     0,     0,     0,     0,
     377,  -301,     0,     0,     0,     0,     0,     0,  -301,  -301,
       0,     0,     0,     0,     0,     0,  -301,   378,     0,     0,
     375,   379,     0,     0,     0,  -301,     0,   376,     0,     0,
       0,     0,     0,     0,  -301,  -301,     0,  -301,     0,  -301,
     377,  -301,     0,     0,  -301,     0,     0,     0,  -301,     0,
       0,  -301,   380,     0,  -301,     0,     0,   378,     0,     0,
       0,   379,     0,     0,     0,     0,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,   378,     0,     0,
       0,   379,     0,     0,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   380,     0,   401,   402,   403,     0,     0,     0,
       0,     0,     0,  1285,     0,   404,     0,     0,   381,   382,
       0,   383,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,   392,   393,     0,   394,   395,   396,   397,   398,
     399,   400,   375,     0,   401,   402,   403,     0,     0,   376,
       0,     0,     0,     0,     0,   404,   220,     0,     0,     0,
      22,    23,   377,     0,     0,  1080,     0,     0,     0,     0,
     221,     0,    31,   222,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,  1081,     0,   837,
    1082,     0,   225,   379,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,     0,    86,   377,     0,    88,
       0,     0,     0,     0,   380,     0,     0,     0,     0,   378,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,   393,     0,   394,   395,   396,
     397,   398,   399,   400,   380,     0,   401,   402,   403,   103,
     377,     0,     0,     0,   378,  1234,     0,   404,   379,     0,
     381,   382,     0,   383,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,   392,     0,     0,   394,   395,   396,
     397,   398,   399,   400,     0,     0,   401,   402,   403,   380,
       0,     0,     0,     0,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,     0,   381,   382,   378,   383,   384,
     385,   379,   386,   387,   388,     0,   389,   390,   391,   392,
       0,     0,   394,   395,   396,   397,   398,     0,   400,     0,
       0,   401,   402,   403,     0,   220,     0,     0,     0,    22,
      23,     0,   404,     0,  1080,     0,     0,     0,     0,   221,
       0,    31,   222,     0,     0,     0,     0,    37,   381,   382,
       0,   383,   384,   385,    42,   386,   387,   388,     0,   389,
     390,   391,   392,     0,     0,   394,   395,   396,   397,   398,
       0,   400,     0,     0,   401,   402,   403,     0,     0,     0,
      58,   800,    60,   256,   354,   404,  1081,     0,     0,  1082,
       0,   225,     0,    68,     0,     0,     0,   257,   258,     0,
     259,     0,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   801,    84,     0,   261,    86,     0,     0,    88,     0,
     262,     0,     0,     0,     0,     0,   263,     0,     0,     0,
     264,     0,     0,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   266,     0,     0,     0,     0,     0,     0,
     267,   268,     0,     0,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   103,     0,
     256,     0,     0,     0,     0,     0,   271,   272,     0,   273,
       0,   274,     0,   275,   257,   258,   276,   259,     0,     0,
     277,     0,   260,   278,    23,     0,   279,     0,     0,     0,
       0,   261,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,     0,   263,     0,     0,     0,   264,     0,     0,
     265,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     266,     0,     0,     0,     0,     0,     0,   267,   268,     0,
       0,     0,     0,     0,     0,   269,    60,     0,     0,     0,
       0,     0,     0,     0,   270,     0,     0,    68,     0,     0,
       0,     0,     0,   271,   272,     0,   273,     0,   274,     0,
     275,  -311,     0,   276,     0,     0,     0,   277,     0,     0,
     278,     0,    88,   279,     0,  -311,  -311,     0,  -311,     0,
       0,     0,     0,  -311,     0,     0,     0,     0,     0,     0,
       0,     0,  -311,     0,     0,     0,     0,     0,  -311,     0,
       0,     0,     0,     0,  -311,     0,     0,     0,  -311,     0,
       0,  -311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -311,     0,     0,     0,     0,     0,     0,  -311,  -311,
       0,     0,     0,     0,     0,     0,  -311,     0,     0,     0,
       0,     0,     0,     0,     0,  -311,     0,     0,   256,     0,
       0,     0,     0,     0,  -311,  -311,     0,  -311,     0,  -311,
       0,  -311,   257,   258,  -311,   259,     0,     0,  -311,     0,
     260,  -311,     0,     0,  -311,     0,     0,     0,     0,   261,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,   263,     0,     0,     0,   264,     0,     0,   265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   266,     0,
       0,     0,     0,     0,     0,   267,   268,     0,     0,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,  -302,     0,     0,     0,     0,
       0,   271,   272,     0,   273,     0,   274,     0,   275,  -302,
    -302,   276,  -302,     0,     0,   277,     0,  -302,   278,     0,
       0,   279,     0,     0,     0,     0,  -302,     0,     0,     0,
       0,     0,  -302,     0,     0,     0,     0,     0,  -302,     0,
       0,     0,  -302,     0,     0,  -302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -302,     0,     0,     0,     0,
       0,     0,  -302,  -302,     0,     0,     0,     0,     0,     0,
    -302,     0,     0,     0,     0,     0,     0,     0,     0,  -302,
       0,     0,  -194,     0,     0,     0,     0,     0,  -302,  -302,
       0,  -302,     0,  -302,     0,  -302,  -194,  -194,  -302,  -194,
       0,     0,  -302,     0,  -194,  -302,     0,     0,  -302,     0,
       0,     0,     0,  -194,     0,     0,     0,     0,     0,  -194,
       0,     0,     0,     0,     0,  -194,     0,     0,     0,  -194,
       0,     0,  -194,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -194,     0,     0,     0,     0,     0,     0,  -194,
    -194,     0,     0,     0,     0,     0,     0,  -194,     0,     0,
       0,     0,     0,     0,     0,     0,  -194,     0,     0,  -186,
       0,     0,     0,     0,     0,  -194,  -194,     0,  -194,     0,
    -194,     0,  -194,  -186,  -186,  -194,  -186,     0,     0,  -194,
       0,  -186,  -194,     0,     0,  -194,     0,     0,     0,     0,
    -186,     0,     0,     0,     0,     0,  -186,     0,     0,     0,
       0,     0,  -186,     0,     0,     0,  -186,     0,     0,  -186,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -186,
       0,     0,     0,     0,     0,     0,  -186,  -186,     0,     0,
       0,     0,     0,     0,  -186,     0,     0,     0,     0,     0,
       0,     0,     0,  -186,     0,     0,     0,     0,     0,     0,
       0,     0,  -186,  -186,     0,  -186,     0,  -186,     0,  -186,
       0,     0,  -186,     0,     0,     0,  -186,     0,     0,  -186,
       0,     0,  -186
};

static const yytype_int16 yycheck[] =
{
      12,    65,   255,   530,    16,   426,    19,   479,    20,   212,
       2,   506,    69,   501,    26,   244,    27,   168,   154,    31,
      32,   494,    34,    35,    36,    37,    38,   912,   811,    22,
     811,   801,   988,   236,   990,   530,   793,    49,   665,    51,
      52,     1,   228,   102,  1137,    57,   737,    59,   957,    61,
     107,    33,   305,    33,    33,  1045,    33,    69,    70,    71,
      72,    73,    74,   788,    48,    33,    78,    48,    80,   255,
     323,  1192,    92,    47,  1042,    87,    61,   213,    90,    91,
       2,    93,    33,    95,   651,     1,    98,    99,   655,   101,
     657,   120,   120,   105,   106,   107,   108,   146,    69,   527,
     146,    56,   253,   118,   146,  1246,    89,    56,     1,   994,
      93,   170,     3,    66,     1,    75,     1,    48,  1125,   305,
     349,   350,   171,   165,    48,  1246,    34,    35,    36,   175,
      88,   146,    85,   118,   105,   106,   107,   323,   120,   120,
     114,   120,   199,    27,  1285,   836,   175,   175,    48,    48,
     170,    48,   164,     0,   112,    48,   881,   117,  1126,    90,
     224,  1046,   146,   636,    48,   146,   146,     3,    52,   422,
     423,  1080,  1265,  1163,    61,  1084,    61,   189,   146,     3,
     192,   193,   194,   105,  1191,   197,   198,   199,   170,   959,
     170,   170,    76,   170,   175,    79,   823,    81,   211,   107,
    1109,   387,   170,  1112,  1113,    92,    90,    92,   219,   125,
     222,   171,   103,  1169,  1170,   165,   228,   172,   909,   170,
     105,   649,   146,   172,     3,   109,    18,   198,   199,   145,
     146,   118,   125,   118,    56,    75,   422,   423,    70,   232,
    1125,    20,   306,   255,   665,   238,   146,   146,   736,   146,
    1159,   175,   145,   146,   247,    34,    92,   173,   145,  1010,
     145,    92,  1013,  1014,   120,    27,    27,   103,  1177,    37,
    1179,  1180,   165,   146,   106,   175,   175,   117,   175,   103,
     165,  1263,   146,   170,    63,   170,    78,     3,   855,   173,
      51,   858,   859,   305,    61,    48,   165,    56,   171,  1281,
     312,   313,   125,   172,    96,    73,  1191,    83,   494,   173,
      86,   323,  1095,  1070,  1095,  1072,    77,    79,    79,   455,
     792,   113,   795,   146,   103,    92,   462,   150,    90,    90,
     109,   171,   344,   146,   165,   146,   146,    90,   105,   170,
     165,   527,    56,   172,   140,   174,   171,   359,   360,   341,
     173,   118,   148,   115,   115,   165,    90,  1108,   171,  1110,
    1111,   373,   173,   375,   376,   377,   378,   379,   380,    27,
     382,   383,   384,   385,   386,   848,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   170,  1021,   172,    27,    47,   165,   411,
     412,   146,   823,   170,   146,   417,   172,   464,   420,   421,
     422,   423,   424,   425,   426,   903,    66,    27,   146,   341,
     146,    79,    61,   146,   125,   437,    32,    56,   173,   165,
     442,   173,    90,   445,   356,    85,   172,   125,   174,   172,
     669,   670,   165,    56,    61,   173,   146,   173,    79,   150,
     636,   109,   464,    92,   466,   467,    54,   115,   146,    90,
     472,   473,   150,   649,   114,   477,   105,   479,   480,    79,
     378,   379,   944,   173,   486,    92,    74,   489,   109,   118,
      90,   140,   494,    89,   146,   173,    84,    93,   105,   148,
     146,   146,   146,   464,   140,   146,   508,   146,    56,   109,
     512,   118,   148,   515,   516,   172,    56,   174,   520,   165,
     567,   173,    49,   435,   146,   527,   424,   425,   173,   173,
      56,   514,   173,   170,   173,  1052,   165,   539,   146,   437,
     542,   170,   544,   165,   140,    72,   548,   549,   550,   145,
     552,   553,   148,   146,   773,   774,   152,   165,   165,   520,
    1045,   146,    56,   170,   165,   567,   146,  1052,  1197,   467,
     171,   764,   165,   146,   146,   473,   558,   146,   539,   477,
     165,   542,   146,   544,    56,   165,   498,    89,   549,   411,
     412,    93,   165,   165,   232,   417,   165,     8,   420,   421,
     238,   165,   172,    56,   174,   172,   567,  1069,   140,  1071,
    1021,   787,   788,   145,    89,   142,   148,   100,    93,   795,
     152,  1250,  1251,  1252,  1253,  1254,  1255,    56,   172,   631,
     632,   633,   634,   635,   636,   637,   638,   640,   641,    89,
      89,    56,    56,    93,    93,   647,   558,   649,    89,   172,
     548,   174,    93,   172,    56,   174,   125,   659,    56,   661,
      48,    85,   172,   665,   174,   667,   542,    56,   544,   165,
     811,   164,   848,    61,   165,   677,   165,   679,  1163,   156,
     165,   165,   151,   210,   153,   154,   647,   165,   157,   158,
     692,   165,   172,   695,   165,   742,    56,   170,   165,   119,
     165,    88,   172,   125,    92,   707,   708,  1192,   172,   172,
     172,   172,   172,   165,   241,   242,   243,   244,   245,   246,
     542,  1206,   544,     8,   865,   252,   253,    66,   145,   148,
     118,   872,   170,    69,   632,   172,   634,   125,   740,   741,
     742,   125,   165,   173,   746,    27,   146,   165,    48,   125,
     125,   172,    35,     2,    35,   282,   146,   173,   146,   125,
     173,  1246,    21,    12,   173,   292,    48,   294,   173,   146,
      52,   175,   175,   170,   301,   146,    25,   165,  1263,    28,
     172,   742,   170,   785,   165,   173,   165,   789,   173,   791,
     792,   793,   319,   795,    76,   165,  1281,    79,   165,    81,
    1285,   165,   165,   330,   331,    35,   175,   172,    90,   165,
    1053,  1054,    35,    66,   146,   173,   173,   171,   173,   146,
     171,   823,   125,    90,     2,   647,   173,   109,    33,    78,
      33,   175,   170,   170,    12,   837,   838,   839,    35,   170,
     842,   165,    90,   145,   172,   175,   848,    25,   165,   172,
      28,   145,   171,   175,   175,   148,   105,   172,   148,   148,
     140,   110,   148,   140,   866,   170,   868,   140,    35,   148,
     148,   873,   148,   148,   876,   922,   148,  1053,  1054,   881,
     148,   883,   148,   140,   140,   148,   170,     8,   890,    27,
     105,   146,   785,   142,   165,   140,   170,   140,   142,   356,
      78,    22,   164,   173,   165,    26,    27,   165,   173,   173,
      48,   438,    24,   173,    52,    36,   492,    38,    39,   165,
     922,   173,   164,    44,   746,   175,   928,   105,   930,   512,
      51,   933,   110,  1084,    90,   952,   185,   896,    76,   837,
     838,    79,   944,    81,   471,  1095,   962,  1091,   475,   901,
     728,   965,    90,  1265,  1095,  1222,    77,  1256,    79,   961,
    1249,   922,    83,    37,   142,    86,   509,    88,    31,    90,
      -1,   109,    -1,    -1,  1115,  1116,    -1,    -1,  1119,  1120,
      -1,   983,    -1,    -1,    -1,    -1,    -1,    -1,   109,   901,
      -1,   112,    -1,    -1,   115,    48,   998,   999,    -1,  1001,
    1002,    -1,  1004,  1005,  1145,    -1,  1147,   185,    61,    -1,
      -1,    -1,    -1,    -1,  1016,  1017,    -1,  1019,    -1,  1021,
      -1,  1023,  1024,    -1,    -1,  1027,    -1,    -1,  1082,    -1,
      -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    92,
     289,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,  1051,
    1052,  1053,  1054,    -1,    -1,    -1,  1058,    -1,    -1,    -1,
    1062,    -1,    -1,    -1,    -1,   118,  1068,  1069,  1070,  1071,
    1072,    -1,   125,    -1,    -1,  1077,    -1,    48,    -1,    -1,
    1127,    -1,    -1,    -1,  1045,    -1,    -1,    -1,    -1,    -1,
      61,    -1,   341,   146,    -1,    -1,    -1,  1099,  1100,  1101,
    1102,  1103,  1104,  1105,  1106,   283,    -1,   356,    -1,    -1,
      -1,   289,   165,    -1,    -1,    -1,    -1,   170,    -1,  1121,
     173,    92,    -1,    -1,    -1,  1127,    -1,    -1,    -1,    -1,
    1132,    -1,    -1,    -1,  1136,    -1,   663,   664,    -1,    32,
      -1,    -1,   669,   670,    -1,   672,   673,   118,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   341,    -1,    -1,  1127,    -1,    -1,    -1,
    1217,  1132,    -1,    -1,    -1,   146,    -1,    -1,   356,  1091,
      -1,  1183,  1184,  1185,  1186,    -1,   435,   714,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    89,    -1,    -1,   170,
      93,    -1,   173,    -1,  1206,    -1,    -1,  1209,  1210,    -1,
      -1,    -1,    -1,    -1,    -1,  1217,    -1,   466,    -1,    -1,
      -1,    -1,     2,   472,  1226,  1227,  1228,  1229,  1230,  1231,
      -1,  1192,    12,    -1,    -1,   484,    -1,   486,    -1,    32,
      -1,    -1,    -1,  1245,  1246,    25,  1248,   140,    28,    -1,
      -1,    -1,   145,    -1,   147,   148,  1217,   435,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   796,
      -1,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,  1281,
      -1,    -1,    -1,  1285,    -1,  1246,    -1,    -1,   466,    -1,
      -1,    -1,    -1,     2,   472,    -1,    89,   546,    78,    -1,
      93,   550,    -1,    12,    -1,    -1,   484,    -1,   486,   558,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,
      -1,    -1,   849,   850,  1285,   105,    -1,    22,    -1,    -1,
     110,    26,    27,    -1,    -1,    -1,   863,    -1,   865,    -1,
      -1,    36,    -1,    38,    39,   872,    -1,   140,   875,    44,
      -1,   878,   145,    -1,   147,   148,    51,    -1,    -1,   152,
      -1,    -1,   142,    -1,     2,    -1,    -1,   160,   546,    78,
     897,    -1,   550,     1,    12,   168,    -1,    -1,    -1,    -1,
     558,    -1,    77,    -1,    79,    -1,    -1,    25,    83,    -1,
      28,    86,    -1,    88,    22,    90,   105,    -1,    26,    27,
      -1,   110,    -1,    31,    -1,   185,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,   109,    -1,    44,   112,    -1,    -1,
     115,    -1,    -1,    51,    -1,    22,    54,    -1,   955,    26,
      27,    -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    36,
      78,    38,    39,    -1,    -1,    -1,    74,    44,    -1,    77,
     628,    79,    -1,    81,    51,    83,    84,    -1,    86,    -1,
      88,   710,    90,    -1,   713,    -1,    -1,   105,    -1,    66,
     165,    -1,   110,    -1,    -1,    -1,   185,    -1,    -1,    -1,
      77,   109,    79,    -1,   112,    -1,    83,   115,    85,    86,
      -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   283,   142,    -1,    -1,    -1,    -1,   289,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,   772,    -1,    -1,    -1,     8,    -1,   778,
    1057,    -1,   710,    -1,    -1,   713,    -1,   165,    -1,    -1,
      -1,    22,    -1,   171,    -1,    26,    27,   185,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      -1,   341,    -1,    44,    -1,    -1,    -1,    -1,   165,    -1,
      51,    -1,    -1,    -1,   283,    -1,   356,    -1,    -1,    -1,
     289,    -1,    -1,    -1,    -1,    66,    -1,    -1,  1115,  1116,
      -1,    -1,  1119,  1120,   772,    -1,    77,    -1,    79,    -1,
     778,    -1,    83,    -1,    85,    86,    -1,    88,    -1,    90,
    1137,    -1,    32,    -1,    -1,    -1,    -1,   866,  1145,   868,
    1147,    -1,    -1,    -1,   873,    -1,    -1,   876,   109,    -1,
    1157,   112,   341,   811,   115,    -1,    -1,    -1,   887,    -1,
     889,    -1,    -1,   892,    -1,   283,    -1,   356,    -1,    -1,
      -1,   289,    -1,    -1,    -1,   435,    -1,    -1,    -1,   837,
     838,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,   921,    93,    -1,    -1,    -1,    -1,    -1,   928,
      -1,   930,    -1,    -1,   165,    -1,   466,    -1,   866,    -1,
     868,    -1,   472,    -1,    -1,   873,    -1,    -1,   876,    -1,
      -1,    -1,    -1,   341,   484,    -1,   486,    -1,    -1,   887,
      -1,   889,    -1,    -1,   892,    -1,    -1,    -1,   356,    -1,
     140,   141,    -1,   143,   144,   145,   435,   147,   148,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,  1265,    -1,
     160,    -1,    -1,   921,    -1,    -1,   166,   167,   168,    -1,
     928,    -1,   930,    -1,    -1,    -1,    -1,   466,    -1,    -1,
      -1,    -1,    -1,   472,    -1,    -1,   546,  1016,    -1,    -1,
     550,    -1,     3,    -1,  1023,   484,    -1,   486,   558,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,  1037,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,   435,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,  1055,    -1,    -1,    40,
      -1,    -1,  1061,    -1,    -1,    46,  1065,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,   466,    -1,
      -1,    -1,    63,    -1,   472,    -1,    -1,   546,  1016,    70,
      71,   550,    -1,    -1,    -1,  1023,   484,    78,   486,   558,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,  1037,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,  1122,    -1,   106,    -1,  1055,     3,   110,
     111,    -1,   113,  1061,    -1,   116,    -1,  1065,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    32,    -1,    -1,    -1,     2,    -1,   546,    34,
      -1,    -1,   550,    -1,    -1,    40,    12,    -1,    -1,    -1,
     558,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    25,
     710,    -1,    28,   713,  1183,  1184,  1185,  1186,    63,    -1,
      -1,   172,    -1,    -1,  1122,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    89,
    1209,  1210,    87,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,     2,   103,    -1,
      -1,   106,    78,    -1,    -1,   110,    -1,    12,   113,    -1,
      -1,   116,   772,    -1,    -1,    -1,    -1,    -1,   778,    -1,
      25,   710,    -1,    28,   713,  1183,  1184,  1185,  1186,   105,
     140,   141,    -1,    -1,   110,   145,    -1,   147,   148,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,  1209,  1210,    -1,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   170,   142,   172,     2,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    32,    -1,   772,    -1,    -1,    -1,    -1,    -1,   778,
      -1,    25,   710,    -1,    28,   713,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,   110,   866,    -1,   868,   185,
      -1,    -1,    -1,   873,    -1,    -1,   876,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   887,    -1,   889,
      -1,    -1,   892,    -1,    -1,    -1,    -1,   142,    89,    -1,
      -1,    -1,    93,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   772,    -1,    -1,    -1,    -1,    -1,
     778,   921,    -1,    -1,    -1,    -1,    -1,    -1,   928,    -1,
     930,   105,    -1,    -1,    -1,    -1,   110,   866,    -1,   868,
     185,    -1,    -1,    -1,   873,    -1,    -1,   876,    -1,   140,
     141,    -1,    -1,   144,   145,    -1,   147,   148,   887,    -1,
     889,   152,    -1,   892,     3,    -1,    -1,   283,   142,   160,
      -1,    -1,    -1,   289,    -1,   166,   167,   168,    17,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,   921,    -1,    -1,    -1,    -1,    36,    -1,   928,
      -1,   930,    -1,    -1,    -1,    -1,    -1,    46,   866,    -1,
     868,   185,    -1,    -1,    53,   873,  1016,    -1,   876,    -1,
      -1,    -1,    -1,  1023,    63,   341,    -1,    -1,    -1,   887,
      -1,   889,    71,    -1,   892,    -1,    -1,  1037,   283,    -1,
     356,    -1,    -1,    -1,   289,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,  1055,    -1,    -1,    -1,    -1,
      99,  1061,    -1,   921,   103,  1065,    -1,    -1,    -1,    -1,
     928,   110,   930,    -1,    -1,    -1,    22,   116,    -1,    -1,
      26,    27,    -1,    -1,    -1,    31,    -1,  1016,    -1,    -1,
      36,    -1,    38,    39,  1023,    -1,   341,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,  1037,   283,
      -1,   356,    -1,    -1,    -1,   289,    -1,    -1,    -1,   435,
      -1,    -1,  1122,    -1,    -1,    -1,  1055,    -1,    -1,    -1,
      -1,    77,  1061,    79,    -1,    81,  1065,    83,    -1,    -1,
      86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
     466,    -1,    -1,    -1,    -1,    -1,   472,    -1,  1016,    32,
      -1,    -1,    -1,   109,    -1,  1023,   112,   341,   484,   115,
     486,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1037,
      -1,    -1,   356,  1183,  1184,  1185,  1186,    -1,    -1,    -1,
     435,    -1,    -1,  1122,    -1,    -1,    -1,  1055,    -1,    -1,
      -1,    -1,    -1,  1061,    -1,    -1,    -1,  1065,    -1,  1209,
    1210,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,   165,
      93,   466,    -1,    -1,    -1,   171,     2,   472,    -1,    -1,
     546,    -1,    -1,    -1,   550,    -1,    12,    -1,    -1,   484,
      -1,   486,   558,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    28,    -1,  1183,  1184,  1185,  1186,    -1,    -1,
      -1,   435,    -1,    -1,  1122,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,    -1,   152,
    1209,  1210,    -1,    -1,    -1,    -1,   159,   160,    -1,    -1,
     163,    -1,   466,   166,   167,   168,    -1,     2,   472,    -1,
      -1,   546,    78,    -1,   177,   550,    -1,    12,    -1,    -1,
     484,    -1,   486,   558,    -1,    -1,    -1,    -1,     3,    -1,
      25,    -1,    -1,    28,    -1,  1183,  1184,  1185,  1186,   105,
      -1,    -1,    17,    18,   110,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,  1209,  1210,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,   546,    78,    -1,    -1,   550,    -1,    63,    -1,
      -1,    -1,    -1,    -1,   558,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,   710,    -1,    -1,   713,    -1,    -1,
     105,    -1,    87,    -1,    -1,   110,    -1,    -1,    -1,   185,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,   772,    -1,    -1,    -1,
      44,    -1,   778,    -1,    -1,   710,    -1,    51,   713,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
     185,    -1,    -1,    36,    -1,    38,    39,   172,    -1,    -1,
      -1,    44,    -1,    77,    -1,    79,    -1,    81,    51,    83,
      -1,    -1,    86,    -1,    88,    -1,    90,   283,    -1,    -1,
      -1,    -1,    -1,   289,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,   109,    79,   772,   112,    -1,
      -1,   115,    -1,   778,    -1,    88,   710,    90,    -1,   713,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     866,    -1,   868,    -1,    -1,    -1,   109,   873,    -1,   112,
     876,    -1,   115,    -1,    -1,   341,    -1,    -1,    -1,    -1,
      -1,   887,    -1,   889,    -1,    -1,   892,    -1,   283,    -1,
     356,   165,    -1,    -1,   289,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   772,    -1,
      -1,    -1,    -1,    -1,   778,   921,    -1,    -1,    -1,    -1,
      -1,    -1,   928,    -1,   930,    -1,    -1,    -1,    -1,    -1,
      -1,   866,    -1,   868,    -1,    -1,    -1,    -1,   873,    -1,
      -1,   876,    44,    45,    46,    -1,   341,    -1,    -1,    -1,
      -1,    -1,   887,    -1,   889,    -1,    -1,   892,    -1,    -1,
      -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   435,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,   921,    -1,    -1,    -1,
      -1,    -1,    -1,   928,    -1,   930,    -1,    -1,    -1,    -1,
     466,    -1,   866,    -1,   868,    -1,   472,    -1,    -1,   873,
    1016,    -1,   876,    -1,    -1,     3,    -1,  1023,   484,    -1,
     486,    -1,    -1,   887,    -1,   889,    -1,    -1,   892,    17,
      18,  1037,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
     435,    -1,    -1,    -1,    -1,   147,    34,   149,    -1,  1055,
      -1,    -1,    40,    -1,    -1,  1061,    -1,   921,    46,  1065,
      -1,    -1,    50,    -1,   928,    53,   930,    72,    -1,    -1,
      -1,   466,    -1,    -1,    -1,    63,    -1,   472,    -1,    -1,
     546,  1016,    70,    71,   550,    -1,    -1,    -1,  1023,   484,
      78,    -1,   558,   195,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,  1037,    -1,    -1,    -1,    -1,    -1,    96,    97,
     212,    99,    -1,   101,    -1,   103,  1122,    -1,   106,    -1,
    1055,    -1,   110,    -1,    -1,   113,  1061,    -1,   116,    -1,
    1065,    -1,    -1,    -1,   236,    -1,    -1,   239,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   250,   251,
      -1,   546,  1016,    -1,    -1,   550,    -1,    -1,    -1,  1023,
      -1,    -1,    -1,   558,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1037,    -1,    -1,    -1,  1183,  1184,  1185,
    1186,    -1,    -1,   285,   172,    -1,    -1,  1122,    -1,    -1,
      -1,  1055,    -1,    -1,    -1,    -1,    -1,  1061,    -1,    -1,
      -1,  1065,    -1,  1209,  1210,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,   241,   242,   243,   244,
     245,   246,    -1,    -1,   710,    -1,    -1,   713,  1183,  1184,
    1185,  1186,    -1,    -1,    -1,    -1,    -1,    -1,  1122,   361,
      77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    86,
      -1,    88,    -1,    90,  1209,  1210,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,   294,
      -1,    -1,   109,    -1,    -1,   112,   301,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   772,    -1,    -1,    -1,
      -1,    -1,   778,    -1,   319,   710,    -1,    -1,   713,  1183,
    1184,  1185,  1186,    -1,    -1,   330,   331,    -1,    -1,    -1,
      -1,    -1,   434,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1209,  1210,    -1,   165,    -1,
      -1,   453,    -1,    -1,   171,    -1,   458,    -1,   460,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   772,    -1,    -1,
      -1,    -1,    -1,   778,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,   501,
     866,    -1,   868,    48,    -1,    -1,    -1,   873,    -1,    -1,
     876,    -1,   514,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   887,    -1,   889,    -1,    -1,   892,    -1,    -1,    -1,
      -1,    -1,    -1,   438,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   921,    -1,   559,    -1,    -1,
      -1,    -1,   928,    -1,   930,    -1,   471,   569,    -1,    -1,
     475,   866,    -1,   868,    -1,   120,    -1,    -1,   873,   124,
      -1,   876,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   887,    -1,   889,   140,   141,   892,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,   170,   921,    -1,    -1,    -1,
      -1,    -1,   177,   928,    -1,   930,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   650,    -1,
    1016,   653,   654,    -1,    -1,    -1,    -1,  1023,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,  1037,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,  1055,
      -1,    -1,    -1,    -1,    -1,  1061,    -1,    -1,    -1,  1065,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    86,     3,    88,    -1,    90,    -1,    -1,    -1,
     732,    -1,    12,    -1,   736,    -1,    -1,    -1,    -1,    19,
      20,    -1,  1037,    -1,    -1,   109,    -1,    -1,   112,    -1,
     752,   115,    -1,    -1,    34,    -1,  1122,    -1,   663,   664,
    1055,    -1,   764,    -1,   669,   670,  1061,   672,   673,    -1,
    1065,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   796,    -1,    -1,    -1,    -1,   801,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,   811,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1183,  1184,  1185,
    1186,    -1,    -1,   103,    -1,    -1,    -1,  1122,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1209,  1210,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,   147,    -1,    -1,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,   168,   169,
     170,    -1,   894,    -1,   896,    -1,    -1,    -1,   900,    -1,
      -1,   903,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   849,   850,   169,    -1,    -1,   172,
     952,   174,    -1,    -1,    -1,    -1,    -1,   959,   863,    -1,
     962,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     875,     0,     1,   878,     3,    -1,     5,     6,     7,     8,
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
     119,    -1,   121,  1095,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     1,
      -1,     3,  1057,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    12,    -1,    -1,    -1,
     149,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    34,   168,
      36,    -1,    -1,   172,    40,   174,    -1,    -1,   177,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,   168,   169,    -1,     3,   172,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    32,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    89,    82,    -1,    -1,    93,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,    -1,   142,    -1,   152,    -1,
     146,    -1,    -1,    -1,   150,   159,   160,    -1,    -1,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,   170,    -1,   172,   173,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    61,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,   105,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
     164,   165,    25,    -1,    -1,    -1,   170,    -1,   172,    -1,
     174,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,   164,    -1,    25,    -1,    -1,    -1,   170,    -1,   172,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     101,    -1,   103,    -1,    -1,   106,    -1,     3,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    12,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,   142,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,   172,    -1,   174,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      96,    97,    -1,    99,    -1,   101,    19,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    32,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
      -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,   168,   169,    -1,    12,    89,    -1,    -1,    -1,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   102,
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
     161,   162,   163,    -1,    89,   166,   167,   168,    93,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    89,    -1,    -1,    -1,    93,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    48,    89,   152,   153,   154,
      93,    -1,   157,   158,   159,   160,    -1,    -1,   163,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    73,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,   165,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,   120,   166,   167,   168,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    48,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    22,   166,   167,   168,    26,    27,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    36,    -1,    38,    39,    -1,
      32,    33,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    66,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      32,    33,    -1,    -1,    85,    -1,    -1,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    -1,    -1,    19,   109,    -1,
      -1,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
     102,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    36,    19,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    32,    51,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    13,    77,
      -1,    79,    -1,    -1,    19,    -1,    -1,    85,    -1,    -1,
      88,    -1,    90,    73,    -1,    -1,    -1,    32,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,   109,    -1,    93,   112,    -1,    -1,   115,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    89,   175,    -1,   177,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,   124,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   124,   170,    -1,    -1,    -1,    89,    -1,    -1,   177,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,   165,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,   165,   166,   167,   168,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   102,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    13,   166,   167,   168,    -1,    -1,
      19,    -1,   173,    -1,    -1,    -1,   177,    22,    -1,    -1,
      -1,    26,    27,    32,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    12,    -1,    -1,    51,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,
      89,    86,    -1,    88,    93,    90,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    32,    -1,
     115,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
     165,    -1,    -1,    -1,    -1,    89,   171,    -1,   177,    93,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,   120,   166,   167,   168,
     124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    13,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
     167,   168,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
     177,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
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
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,   173,    -1,    89,   165,   177,    -1,    93,
     140,   141,   171,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,   123,
     124,    19,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    13,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,   175,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   102,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,   102,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    13,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
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
     168,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,     3,   166,   167,
     168,    -1,     8,    19,    -1,    -1,    -1,   175,    -1,   177,
      -1,    17,    18,    -1,    20,    -1,    32,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    12,    -1,    -1,    -1,
      46,    -1,    -1,    19,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    89,    -1,    -1,    12,    93,    -1,    -1,
      -1,    87,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    32,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,   124,    -1,
     116,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   102,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,   102,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,     3,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    17,    18,    -1,    20,    -1,    32,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    12,    -1,
      -1,    -1,    46,    -1,    -1,    19,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    87,    -1,    19,    -1,    -1,   102,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    32,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
     124,    -1,   116,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   102,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,   102,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,     3,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    17,    18,    -1,    20,    -1,
      32,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      12,    -1,    -1,    -1,    46,    -1,    -1,    19,    50,    61,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    87,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      32,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,   124,    -1,   116,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    22,    -1,    -1,    -1,
      26,    27,    32,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,    89,
      86,    -1,    88,    93,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,   112,    32,    -1,   115,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,   165,
      32,    -1,    -1,    -1,    89,   171,    -1,   177,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    89,   143,   144,
     145,    93,   147,   148,   149,    -1,   151,   152,   153,   154,
      -1,    -1,   157,   158,   159,   160,   161,    -1,   163,    -1,
      -1,   166,   167,   168,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,   177,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,   140,   141,
      -1,   143,   144,   145,    51,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,    -1,   157,   158,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      77,     1,    79,     3,    81,   177,    83,    -1,    -1,    86,
      -1,    88,    -1,    90,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,   109,    -1,    34,   112,    -1,    -1,   115,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,   165,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    17,    18,   106,    20,    -1,    -1,
     110,    -1,    25,   113,    27,    -1,   116,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,     3,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    17,    18,   106,    20,
      -1,    -1,   110,    -1,    25,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    17,    18,   106,    20,    -1,    -1,   110,
      -1,    25,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116
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
     354,   165,   186,   338,   120,   327,   328,     3,   209,    14,
      22,    36,    41,    42,    45,    56,    88,   101,   170,   174,
     241,   264,   320,   325,   336,   337,   338,   341,   343,   344,
     327,   338,   109,   299,    90,   209,   186,   314,   338,     8,
      22,    36,    39,    83,    86,    88,   189,   186,   172,     8,
      88,   338,   339,     8,    11,    88,   109,   112,   339,    79,
     122,   235,   338,   235,   338,   235,   338,    26,   300,   338,
      27,   115,   237,   338,   196,   172,     3,    17,    18,    20,
      25,    34,    40,    46,    50,    53,    63,    70,    71,    78,
      87,    96,    97,    99,   101,   103,   106,   110,   113,   116,
     211,   213,   211,   211,   265,   172,   211,   301,   302,    33,
     197,   216,   338,   218,   219,   338,   338,    18,    78,    96,
     113,   338,   338,   338,     8,   172,   223,   224,     4,   290,
     313,   338,   107,   108,   165,   338,   340,   338,   216,   338,
     338,   338,   100,   172,   186,   338,   338,   211,   297,   338,
     237,   338,   338,   338,   338,   338,   338,   338,     1,   171,
     184,   198,   313,   111,   150,   290,   315,   316,   340,   235,
     313,   338,   349,   338,    81,   186,   170,    85,   193,    47,
     114,    56,   187,   197,   211,   211,    54,    74,    84,   285,
     301,   164,   165,   156,   338,    12,    19,    32,    89,    93,
     124,   140,   141,   143,   144,   145,   147,   148,   149,   151,
     152,   153,   154,   155,   157,   158,   159,   160,   161,   162,
     163,   166,   167,   168,   177,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     169,   275,   172,   174,    89,    93,   172,   186,   165,   338,
     338,   338,   211,   313,    56,   170,   197,    48,   327,   297,
     301,   165,   146,   165,   189,   119,   212,   290,   317,   318,
     319,   340,    88,   231,   268,   299,    88,   112,   227,   297,
     229,   268,   299,   211,   172,   216,    33,    48,   216,   120,
     216,   330,    33,    48,   216,   330,   216,    48,   216,    37,
      73,   165,   211,   211,   102,   197,   102,   125,   197,   275,
      83,    86,   194,   317,   172,   172,   197,   186,   172,   278,
     111,   172,   211,   303,   304,     1,   145,   308,    48,   146,
     186,   216,   146,   216,    13,   172,   172,   216,   317,   224,
     146,   165,   338,   338,   165,   170,   216,   172,   317,   165,
     125,   298,   165,   216,   216,   165,   171,   171,   184,   146,
     171,   338,   146,   173,   146,   173,   175,   330,    48,   146,
     175,   330,   123,   146,   175,     8,     1,   171,   198,    66,
     205,   206,   338,   200,   338,   211,   248,   145,   257,   170,
     269,   165,   338,   338,   338,   338,   235,   338,   235,   338,
     338,   338,   338,   338,   338,   338,     3,    20,    34,    63,
     103,   109,   212,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
      69,   340,   340,   340,   340,   340,   317,   317,   235,   338,
     235,   338,    27,    48,    90,   115,   329,   332,   333,   338,
     354,    33,    48,    33,    48,   102,   172,    48,   175,   211,
     235,   338,   216,   165,   165,   338,   338,   125,   173,   146,
     232,   211,   301,   228,   230,   211,   165,   211,   301,    48,
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
     175,    48,    90,   146,   173,    17,    20,    25,    46,    53,
      63,    71,    87,    99,   110,   116,   320,    89,    89,    45,
     235,   338,    45,   235,   338,   318,   235,   338,   172,   327,
     327,   165,   290,   340,   319,   211,   257,   165,   211,   211,
     257,   257,   165,   338,   173,   338,    33,   216,    33,   216,
     331,   332,   338,    33,   216,   330,    33,   216,   330,   216,
     216,   146,   165,    13,   165,   338,   338,    35,   186,    35,
      35,   186,   102,   197,    66,   173,   146,   173,    48,    90,
     282,   146,   173,   172,   211,    27,    79,    90,   109,   286,
     173,   304,   308,     1,   313,    69,   340,   211,   173,   173,
     171,    75,   117,   171,   254,   173,   297,   186,   175,   330,
     175,   330,   186,   123,   205,   212,   140,   141,   142,   143,
     144,   159,   163,   168,   170,   276,   338,   111,   338,   199,
     201,   318,   172,   197,   211,   250,     1,   258,   171,     8,
     263,   125,   146,   171,    90,   270,     1,     3,    17,    20,
      25,    40,    46,    53,    63,    70,    71,    87,    99,   103,
     106,   110,   116,   172,   210,   211,   213,   272,   273,   274,
     275,   320,   173,   332,   308,   320,   320,   338,    33,    33,
     338,    33,    33,   173,   175,   175,   318,   216,   216,   257,
     170,   257,   257,   170,   170,   216,   102,    45,   338,    45,
     338,   146,   173,   102,    45,   338,   216,    45,   338,   216,
     276,   338,   338,   186,   186,   338,   186,    35,   211,   165,
     240,   197,   211,   281,   304,   145,   312,    90,   308,   305,
     175,    48,   175,   172,   172,    33,   186,   313,    45,   186,
     338,   175,    45,   186,   338,   175,   338,   199,    13,    37,
      73,    37,    73,   165,   165,   173,   250,   145,   197,   171,
      31,    83,    86,   171,   185,   221,   222,   263,   338,   262,
     286,   172,   277,   338,   140,   148,   277,   277,   305,   102,
      45,    45,   102,    45,    45,    45,    45,   173,   170,   258,
     170,   170,   258,   258,   338,   338,   338,   332,   338,   338,
     338,    13,    35,   186,   165,   312,   173,   174,   215,   290,
     311,   321,   150,   291,   305,    61,   118,   292,   338,   309,
     321,   317,   317,   186,   216,   338,   186,   338,   186,   171,
     111,   338,   199,   201,   199,   201,   165,   173,   338,     8,
     222,   221,     1,   145,   307,   280,   173,     3,   103,   273,
     275,   338,   338,   338,   338,   338,   338,   258,   171,   258,
     258,   171,   171,   102,   102,   102,   102,   338,   186,   291,
     305,   312,   175,   313,   290,   338,     3,    92,   103,   293,
     294,   295,   338,   197,   217,   289,   175,   173,   173,   102,
     102,   165,   165,   165,   165,   197,   222,   174,   215,   306,
     321,   105,   287,   173,   277,   277,   102,   102,   102,   102,
     102,   102,   171,   171,   171,   338,   338,   338,   338,   291,
     305,   290,   310,   311,   321,    48,   175,   295,   118,   146,
     125,   151,   153,   154,   157,   158,    61,   321,   164,   164,
     338,   338,     1,   175,   313,   292,   338,   310,   311,   338,
     294,   295,   295,   295,   295,   295,   295,   293,   175,   306,
     321,   175,   165,   288,   289,   175,   306,   321,   310
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   188,   188,   188,   189,   189,   190,
     191,   191,   191,   192,   192,   192,   193,   193,   194,   194,
     194,   196,   195,   197,   197,   197,   198,   198,   199,   199,
     199,   199,   199,   199,   200,   200,   200,   200,   200,   200,
     201,   201,   202,   202,   202,   203,   203,   203,   203,   203,
     203,   203,   204,   205,   205,   205,   205,   206,   206,   207,
     208,   208,   208,   208,   208,   208,   209,   209,   210,   210,
     210,   210,   210,   210,   211,   211,   211,   211,   211,   211,
     212,   212,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   215,   215,   215,   215,   216,   216,   217,
     217,   218,   218,   218,   219,   219,   220,   221,   221,   221,
     222,   222,   222,   223,   222,   224,   224,   224,   224,   224,
     224,   224,   225,   225,   225,   225,   227,   226,   228,   226,
     229,   226,   230,   226,   231,   226,   232,   226,   226,   226,
     226,   226,   233,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   235,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   237,   237,   238,
     238,   239,   239,   240,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   242,   242,   242,
     242,   242,   242,   243,   243,   243,   244,   244,   244,   245,
     246,   246,   247,   247,   248,   248,   249,   249,   249,   250,
     250,   251,   252,   252,   253,   253,   254,   254,   254,   255,
     255,   256,   256,   256,   257,   257,   258,   258,   258,   259,
     259,   260,   261,   261,   261,   262,   262,   262,   263,   263,
     265,   266,   264,   267,   267,   267,   269,   270,   268,   271,
     271,   271,   271,   271,   272,   272,   273,   273,   273,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   276,   276,   277,
     277,   278,   279,   279,   280,   280,   281,   281,   281,   281,
     281,   281,   282,   282,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   284,   284,   284,   284,   284,   284,   285,
     285,   285,   286,   286,   286,   286,   286,   286,   287,   287,
     288,   288,   289,   289,   290,   291,   291,   291,   292,   292,
     292,   292,   292,   293,   293,   294,   294,   294,   294,   294,
     294,   294,   295,   295,   296,   296,   296,   297,   297,   298,
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
       1,     1,     2,     3,     3,     3,     3,     2,     1,     1,
       1,     1,     1,     1,     2,     3,     3,     3,     3,     2,
       3,     3,     2,     2,     1,     3,     2,     1,     1,     4,
       3,     4,     4,     0,     1,     1,     0,     1,     0,     1,
       1,     0,     7,     2,     3,     3,     1,     2,     1,     1,
       3,     3,     3,     5,     1,     3,     3,     3,     5,     5,
       0,     1,     0,     1,     1,     4,     6,     8,     8,     6,
       8,     8,     4,     1,     3,     3,     5,     1,     3,     3,
       4,     4,     4,     4,     4,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       3,     4,     3,     1,     1,     3,     3,     1,     3,     2,
       1,     1,     2,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     5,     5,     2,     0,     8,     0,     9,
       0,     8,     0,     9,     0,     8,     0,     9,     3,     3,
       5,     5,     2,     5,     3,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     5,     5,     3,     3,     6,     7,     6,     7,
       4,     5,     4,     5,     4,     4,     3,     6,     5,     4,
       3,     6,     5,     6,     5,     8,     7,     4,     4,     6,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     4,     7,     5,     3,     6,     2,
       1,     1,     2,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     5,     0,     2,     3,     2,     3,     6,
       6,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       5,     1,     1,     2,     3,     1,     3,     2,     1,     3,
       0,     0,     8,     0,     1,     1,     0,     0,    10,     3,
       3,     5,     5,     3,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
#line 6612 "bison-chapel.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 523 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6618 "bison-chapel.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 524 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6624 "bison-chapel.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 531 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6630 "bison-chapel.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 536 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6636 "bison-chapel.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 537 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6642 "bison-chapel.cpp"
    break;

  case 22: /* stmt: TATOMIC stmt  */
#line 555 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6648 "bison-chapel.cpp"
    break;

  case 23: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 556 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6654 "bison-chapel.cpp"
    break;

  case 24: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 557 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6660 "bison-chapel.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6666 "bison-chapel.cpp"
    break;

  case 26: /* stmt: TYIELD expr TSEMI  */
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6672 "bison-chapel.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6678 "bison-chapel.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 570 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6684 "bison-chapel.cpp"
    break;

  case 35: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 571 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6690 "bison-chapel.cpp"
    break;

  case 36: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 572 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6696 "bison-chapel.cpp"
    break;

  case 37: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 573 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6702 "bison-chapel.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 574 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6708 "bison-chapel.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL do_stmt  */
#line 575 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6714 "bison-chapel.cpp"
    break;

  case 40: /* tryable_stmt: TON expr do_stmt  */
#line 576 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6720 "bison-chapel.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 577 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6726 "bison-chapel.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL do_stmt  */
#line 578 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6732 "bison-chapel.cpp"
    break;

  case 43: /* tryable_stmt: TSYNC stmt  */
#line 579 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6738 "bison-chapel.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 585 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 6744 "bison-chapel.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 587 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 6750 "bison-chapel.cpp"
    break;

  case 49: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 597 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6764 "bison-chapel.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 610 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6773 "bison-chapel.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 615 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6782 "bison-chapel.cpp"
    break;

  case 52: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 620 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6791 "bison-chapel.cpp"
    break;

  case 53: /* access_control: %empty  */
#line 627 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6797 "bison-chapel.cpp"
    break;

  case 54: /* access_control: TPUBLIC  */
#line 628 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6803 "bison-chapel.cpp"
    break;

  case 55: /* access_control: TPRIVATE  */
#line 629 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6809 "bison-chapel.cpp"
    break;

  case 56: /* opt_prototype: %empty  */
#line 633 "chapel.ypp"
             { (yyval.b) = false; }
#line 6815 "bison-chapel.cpp"
    break;

  case 57: /* opt_prototype: TPROTOTYPE  */
#line 634 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6821 "bison-chapel.cpp"
    break;

  case 58: /* include_access_control: %empty  */
#line 638 "chapel.ypp"
           { (yyval.b) = false; }
#line 6827 "bison-chapel.cpp"
    break;

  case 59: /* include_access_control: TPUBLIC  */
#line 639 "chapel.ypp"
           { (yyval.b) = false; }
#line 6833 "bison-chapel.cpp"
    break;

  case 60: /* include_access_control: TPRIVATE  */
#line 640 "chapel.ypp"
           { (yyval.b) = true; }
#line 6839 "bison-chapel.cpp"
    break;

  case 61: /* $@1: %empty  */
#line 645 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6848 "bison-chapel.cpp"
    break;

  case 62: /* include_module_stmt: TINCLUDE $@1 include_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 650 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6856 "bison-chapel.cpp"
    break;

  case 63: /* block_stmt: TLCBR TRCBR  */
#line 665 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6862 "bison-chapel.cpp"
    break;

  case 64: /* block_stmt: TLCBR stmt_ls TRCBR  */
#line 666 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6868 "bison-chapel.cpp"
    break;

  case 65: /* block_stmt: TLCBR error TRCBR  */
#line 667 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6874 "bison-chapel.cpp"
    break;

  case 66: /* stmt_ls: toplevel_stmt  */
#line 672 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6880 "bison-chapel.cpp"
    break;

  case 67: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 673 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6886 "bison-chapel.cpp"
    break;

  case 68: /* renames_ls: expr  */
#line 678 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6896 "bison-chapel.cpp"
    break;

  case 69: /* renames_ls: all_op_name  */
#line 683 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6906 "bison-chapel.cpp"
    break;

  case 70: /* renames_ls: expr TAS expr  */
#line 688 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6916 "bison-chapel.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA expr  */
#line 693 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6925 "bison-chapel.cpp"
    break;

  case 72: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 697 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6934 "bison-chapel.cpp"
    break;

  case 73: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 701 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6943 "bison-chapel.cpp"
    break;

  case 74: /* use_renames_ls: expr  */
#line 710 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6953 "bison-chapel.cpp"
    break;

  case 75: /* use_renames_ls: expr TAS expr  */
#line 715 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6963 "bison-chapel.cpp"
    break;

  case 76: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 720 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6973 "bison-chapel.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 725 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6982 "bison-chapel.cpp"
    break;

  case 78: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 729 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6991 "bison-chapel.cpp"
    break;

  case 79: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 733 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 7000 "bison-chapel.cpp"
    break;

  case 80: /* opt_only_ls: %empty  */
#line 741 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 7010 "bison-chapel.cpp"
    break;

  case 82: /* use_access_control: %empty  */
#line 750 "chapel.ypp"
           { (yyval.b) = true; }
#line 7016 "bison-chapel.cpp"
    break;

  case 83: /* use_access_control: TPUBLIC  */
#line 751 "chapel.ypp"
           { (yyval.b) = false; }
#line 7022 "bison-chapel.cpp"
    break;

  case 84: /* use_access_control: TPRIVATE  */
#line 752 "chapel.ypp"
           { (yyval.b) = true; }
#line 7028 "bison-chapel.cpp"
    break;

  case 85: /* use_stmt: use_access_control TUSE use_renames_ls TSEMI  */
#line 756 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7034 "bison-chapel.cpp"
    break;

  case 86: /* use_stmt: use_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 757 "chapel.ypp"
                                                           { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7040 "bison-chapel.cpp"
    break;

  case 87: /* use_stmt: use_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 758 "chapel.ypp"
                                                                    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7046 "bison-chapel.cpp"
    break;

  case 88: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 759 "chapel.ypp"
                                                                           { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7052 "bison-chapel.cpp"
    break;

  case 89: /* use_stmt: use_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 760 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7058 "bison-chapel.cpp"
    break;

  case 90: /* use_stmt: use_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 761 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7064 "bison-chapel.cpp"
    break;

  case 91: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 762 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7070 "bison-chapel.cpp"
    break;

  case 92: /* import_stmt: use_access_control TIMPORT import_ls TSEMI  */
#line 766 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7077 "bison-chapel.cpp"
    break;

  case 93: /* import_expr: expr  */
#line 771 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7083 "bison-chapel.cpp"
    break;

  case 94: /* import_expr: expr TDOT all_op_name  */
#line 772 "chapel.ypp"
                        { std::vector<PotentialRename*>* renames = new std::vector<PotentialRename*>();
                          PotentialRename* nameInMod = new PotentialRename();
                          nameInMod->tag = PotentialRename::SINGLE;
                          nameInMod->elem = new UnresolvedSymExpr((yyvsp[0].pch));
                          renames->push_back(nameInMod);
                          (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), renames); }
#line 7094 "bison-chapel.cpp"
    break;

  case 95: /* import_expr: expr TAS ident_use  */
#line 778 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7100 "bison-chapel.cpp"
    break;

  case 96: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 779 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7106 "bison-chapel.cpp"
    break;

  case 97: /* import_ls: import_expr  */
#line 783 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7112 "bison-chapel.cpp"
    break;

  case 98: /* import_ls: import_ls TCOMMA import_expr  */
#line 784 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7118 "bison-chapel.cpp"
    break;

  case 99: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 788 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7124 "bison-chapel.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 793 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7130 "bison-chapel.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 795 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7136 "bison-chapel.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 797 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7142 "bison-chapel.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 799 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7148 "bison-chapel.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 801 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7154 "bison-chapel.cpp"
    break;

  case 105: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 803 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7160 "bison-chapel.cpp"
    break;

  case 106: /* opt_label_ident: %empty  */
#line 809 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7166 "bison-chapel.cpp"
    break;

  case 107: /* opt_label_ident: TIDENT  */
#line 810 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7172 "bison-chapel.cpp"
    break;

  case 108: /* ident_fn_def: TIDENT  */
#line 814 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7178 "bison-chapel.cpp"
    break;

  case 109: /* ident_fn_def: TNONE  */
#line 815 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7184 "bison-chapel.cpp"
    break;

  case 110: /* ident_fn_def: TTHIS  */
#line 816 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7190 "bison-chapel.cpp"
    break;

  case 111: /* ident_fn_def: TFALSE  */
#line 817 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7196 "bison-chapel.cpp"
    break;

  case 112: /* ident_fn_def: TTRUE  */
#line 818 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7202 "bison-chapel.cpp"
    break;

  case 113: /* ident_fn_def: internal_type_ident_def  */
#line 819 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7208 "bison-chapel.cpp"
    break;

  case 114: /* ident_def: TIDENT  */
#line 822 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7214 "bison-chapel.cpp"
    break;

  case 115: /* ident_def: TNONE  */
#line 823 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7220 "bison-chapel.cpp"
    break;

  case 116: /* ident_def: TTHIS  */
#line 824 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7226 "bison-chapel.cpp"
    break;

  case 117: /* ident_def: TFALSE  */
#line 825 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7232 "bison-chapel.cpp"
    break;

  case 118: /* ident_def: TTRUE  */
#line 826 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7238 "bison-chapel.cpp"
    break;

  case 119: /* ident_def: internal_type_ident_def  */
#line 827 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7244 "bison-chapel.cpp"
    break;

  case 120: /* ident_use: TIDENT  */
#line 839 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7250 "bison-chapel.cpp"
    break;

  case 121: /* ident_use: TTHIS  */
#line 840 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7256 "bison-chapel.cpp"
    break;

  case 122: /* internal_type_ident_def: TBOOL  */
#line 851 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7262 "bison-chapel.cpp"
    break;

  case 123: /* internal_type_ident_def: TINT  */
#line 852 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7268 "bison-chapel.cpp"
    break;

  case 124: /* internal_type_ident_def: TUINT  */
#line 853 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7274 "bison-chapel.cpp"
    break;

  case 125: /* internal_type_ident_def: TREAL  */
#line 854 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7280 "bison-chapel.cpp"
    break;

  case 126: /* internal_type_ident_def: TIMAG  */
#line 855 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7286 "bison-chapel.cpp"
    break;

  case 127: /* internal_type_ident_def: TCOMPLEX  */
#line 856 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7292 "bison-chapel.cpp"
    break;

  case 128: /* internal_type_ident_def: TBYTES  */
#line 857 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7298 "bison-chapel.cpp"
    break;

  case 129: /* internal_type_ident_def: TSTRING  */
#line 858 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7304 "bison-chapel.cpp"
    break;

  case 130: /* internal_type_ident_def: TSYNC  */
#line 859 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7310 "bison-chapel.cpp"
    break;

  case 131: /* internal_type_ident_def: TSINGLE  */
#line 860 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7316 "bison-chapel.cpp"
    break;

  case 132: /* internal_type_ident_def: TOWNED  */
#line 861 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7322 "bison-chapel.cpp"
    break;

  case 133: /* internal_type_ident_def: TSHARED  */
#line 862 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7328 "bison-chapel.cpp"
    break;

  case 134: /* internal_type_ident_def: TBORROWED  */
#line 863 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7334 "bison-chapel.cpp"
    break;

  case 135: /* internal_type_ident_def: TUNMANAGED  */
#line 864 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7340 "bison-chapel.cpp"
    break;

  case 136: /* internal_type_ident_def: TDOMAIN  */
#line 865 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7346 "bison-chapel.cpp"
    break;

  case 137: /* internal_type_ident_def: TINDEX  */
#line 866 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7352 "bison-chapel.cpp"
    break;

  case 138: /* internal_type_ident_def: TLOCALE  */
#line 867 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7358 "bison-chapel.cpp"
    break;

  case 139: /* internal_type_ident_def: TNOTHING  */
#line 868 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7364 "bison-chapel.cpp"
    break;

  case 140: /* internal_type_ident_def: TVOID  */
#line 869 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7370 "bison-chapel.cpp"
    break;

  case 141: /* scalar_type: TBOOL  */
#line 873 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7376 "bison-chapel.cpp"
    break;

  case 142: /* scalar_type: TENUM  */
#line 874 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7382 "bison-chapel.cpp"
    break;

  case 143: /* scalar_type: TINT  */
#line 875 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7388 "bison-chapel.cpp"
    break;

  case 144: /* scalar_type: TUINT  */
#line 876 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7394 "bison-chapel.cpp"
    break;

  case 145: /* scalar_type: TREAL  */
#line 877 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7400 "bison-chapel.cpp"
    break;

  case 146: /* scalar_type: TIMAG  */
#line 878 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7406 "bison-chapel.cpp"
    break;

  case 147: /* scalar_type: TCOMPLEX  */
#line 879 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7412 "bison-chapel.cpp"
    break;

  case 148: /* scalar_type: TBYTES  */
#line 880 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7418 "bison-chapel.cpp"
    break;

  case 149: /* scalar_type: TSTRING  */
#line 881 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7424 "bison-chapel.cpp"
    break;

  case 150: /* scalar_type: TLOCALE  */
#line 882 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7430 "bison-chapel.cpp"
    break;

  case 151: /* scalar_type: TNOTHING  */
#line 883 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7436 "bison-chapel.cpp"
    break;

  case 152: /* scalar_type: TVOID  */
#line 884 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7442 "bison-chapel.cpp"
    break;

  case 153: /* reserved_type_ident_use: TSYNC  */
#line 891 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7448 "bison-chapel.cpp"
    break;

  case 154: /* reserved_type_ident_use: TSINGLE  */
#line 892 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7454 "bison-chapel.cpp"
    break;

  case 155: /* reserved_type_ident_use: TDOMAIN  */
#line 893 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7460 "bison-chapel.cpp"
    break;

  case 156: /* reserved_type_ident_use: TINDEX  */
#line 894 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7466 "bison-chapel.cpp"
    break;

  case 157: /* do_stmt: TDO stmt  */
#line 898 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7472 "bison-chapel.cpp"
    break;

  case 158: /* do_stmt: block_stmt  */
#line 899 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7478 "bison-chapel.cpp"
    break;

  case 159: /* return_stmt: TRETURN TSEMI  */
#line 903 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7484 "bison-chapel.cpp"
    break;

  case 160: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 904 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7490 "bison-chapel.cpp"
    break;

  case 161: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 908 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-3].pexpr), (yyvsp[-1].pflagset), (yyvsp[0].pch)); }
#line 7496 "bison-chapel.cpp"
    break;

  case 162: /* manager_expr: expr TAS ident_def  */
#line 909 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-2].pexpr), NULL, (yyvsp[0].pch)); }
#line 7502 "bison-chapel.cpp"
    break;

  case 163: /* manager_expr: expr  */
#line 910 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[0].pexpr), NULL, NULL); }
#line 7508 "bison-chapel.cpp"
    break;

  case 164: /* manager_expr_ls: manager_expr  */
#line 914 "chapel.ypp"
                                        { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7514 "bison-chapel.cpp"
    break;

  case 165: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 915 "chapel.ypp"
                                        { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7520 "bison-chapel.cpp"
    break;

  case 166: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 919 "chapel.ypp"
                                  { (yyval.pblockstmt) = buildManageStmt((yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7526 "bison-chapel.cpp"
    break;

  case 168: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 925 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 7532 "bison-chapel.cpp"
    break;

  case 169: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 927 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 7538 "bison-chapel.cpp"
    break;

  case 170: /* class_level_stmt: TSEMI  */
#line 931 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7544 "bison-chapel.cpp"
    break;

  case 172: /* class_level_stmt: TPUBLIC private_decl  */
#line 933 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7550 "bison-chapel.cpp"
    break;

  case 173: /* @2: %empty  */
#line 943 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7556 "bison-chapel.cpp"
    break;

  case 174: /* class_level_stmt: TPRIVATE @2 private_decl  */
#line 944 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7562 "bison-chapel.cpp"
    break;

  case 182: /* forwarding_stmt: TFORWARDING expr TSEMI  */
#line 958 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7568 "bison-chapel.cpp"
    break;

  case 183: /* forwarding_stmt: TFORWARDING expr TEXCEPT renames_ls TSEMI  */
#line 959 "chapel.ypp"
                                             { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7574 "bison-chapel.cpp"
    break;

  case 184: /* forwarding_stmt: TFORWARDING expr TONLY opt_only_ls TSEMI  */
#line 960 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7580 "bison-chapel.cpp"
    break;

  case 185: /* forwarding_stmt: TFORWARDING var_decl_stmt  */
#line 961 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7586 "bison-chapel.cpp"
    break;

  case 186: /* $@3: %empty  */
#line 966 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7595 "bison-chapel.cpp"
    break;

  case 187: /* extern_export_decl_stmt: TEXTERN TRECORD $@3 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 971 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7609 "bison-chapel.cpp"
    break;

  case 188: /* $@4: %empty  */
#line 982 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7618 "bison-chapel.cpp"
    break;

  case 189: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TRECORD $@4 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 987 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7633 "bison-chapel.cpp"
    break;

  case 190: /* $@5: %empty  */
#line 999 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7642 "bison-chapel.cpp"
    break;

  case 191: /* extern_export_decl_stmt: TEXTERN TUNION $@5 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1004 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7656 "bison-chapel.cpp"
    break;

  case 192: /* $@6: %empty  */
#line 1015 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7665 "bison-chapel.cpp"
    break;

  case 193: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TUNION $@6 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1020 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7680 "bison-chapel.cpp"
    break;

  case 194: /* $@7: %empty  */
#line 1032 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7689 "bison-chapel.cpp"
    break;

  case 195: /* extern_export_decl_stmt: TEXPORT TRECORD $@7 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1037 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7703 "bison-chapel.cpp"
    break;

  case 196: /* $@8: %empty  */
#line 1047 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7712 "bison-chapel.cpp"
    break;

  case 197: /* extern_export_decl_stmt: TEXPORT STRINGLITERAL TRECORD $@8 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1052 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7726 "bison-chapel.cpp"
    break;

  case 198: /* extern_export_decl_stmt: TEXTERN opt_expr fn_decl_stmt  */
#line 1063 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7734 "bison-chapel.cpp"
    break;

  case 199: /* extern_export_decl_stmt: TEXPORT opt_expr fn_decl_stmt  */
#line 1067 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7742 "bison-chapel.cpp"
    break;

  case 200: /* extern_export_decl_stmt: TEXTERN opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1072 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7754 "bison-chapel.cpp"
    break;

  case 201: /* extern_export_decl_stmt: TEXPORT opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1080 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7766 "bison-chapel.cpp"
    break;

  case 202: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1091 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7774 "bison-chapel.cpp"
    break;

  case 203: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1097 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7780 "bison-chapel.cpp"
    break;

  case 204: /* loop_stmt: TWHILE expr do_stmt  */
#line 1098 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7786 "bison-chapel.cpp"
    break;

  case 205: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1099 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7792 "bison-chapel.cpp"
    break;

  case 206: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1100 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7798 "bison-chapel.cpp"
    break;

  case 207: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1101 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7804 "bison-chapel.cpp"
    break;

  case 208: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1102 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7810 "bison-chapel.cpp"
    break;

  case 209: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1103 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7816 "bison-chapel.cpp"
    break;

  case 210: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1104 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7822 "bison-chapel.cpp"
    break;

  case 211: /* loop_stmt: TFOR expr do_stmt  */
#line 1105 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7828 "bison-chapel.cpp"
    break;

  case 212: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1106 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7834 "bison-chapel.cpp"
    break;

  case 213: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1107 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7840 "bison-chapel.cpp"
    break;

  case 214: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1108 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7846 "bison-chapel.cpp"
    break;

  case 215: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1109 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7852 "bison-chapel.cpp"
    break;

  case 216: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1110 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7858 "bison-chapel.cpp"
    break;

  case 217: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1111 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7864 "bison-chapel.cpp"
    break;

  case 218: /* loop_stmt: TFORALL expr do_stmt  */
#line 1112 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7870 "bison-chapel.cpp"
    break;

  case 219: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1113 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7876 "bison-chapel.cpp"
    break;

  case 220: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1114 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7882 "bison-chapel.cpp"
    break;

  case 221: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1115 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7888 "bison-chapel.cpp"
    break;

  case 222: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1116 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7894 "bison-chapel.cpp"
    break;

  case 223: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1117 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7900 "bison-chapel.cpp"
    break;

  case 224: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1118 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7906 "bison-chapel.cpp"
    break;

  case 225: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1119 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7912 "bison-chapel.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1121 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7922 "bison-chapel.cpp"
    break;

  case 227: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1127 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7932 "bison-chapel.cpp"
    break;

  case 228: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1133 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7942 "bison-chapel.cpp"
    break;

  case 229: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1139 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7952 "bison-chapel.cpp"
    break;

  case 230: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1145 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7963 "bison-chapel.cpp"
    break;

  case 231: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1152 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7974 "bison-chapel.cpp"
    break;

  case 232: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1159 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7982 "bison-chapel.cpp"
    break;

  case 233: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1163 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7990 "bison-chapel.cpp"
    break;

  case 234: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1169 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7996 "bison-chapel.cpp"
    break;

  case 235: /* if_stmt: TIF expr TTHEN stmt  */
#line 1173 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8002 "bison-chapel.cpp"
    break;

  case 236: /* if_stmt: TIF expr block_stmt  */
#line 1174 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8008 "bison-chapel.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1175 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8014 "bison-chapel.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1176 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8020 "bison-chapel.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1178 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 8026 "bison-chapel.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar block_stmt  */
#line 1179 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8032 "bison-chapel.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1180 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8038 "bison-chapel.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1181 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8044 "bison-chapel.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1183 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8051 "bison-chapel.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1185 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8058 "bison-chapel.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1187 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8065 "bison-chapel.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1189 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8072 "bison-chapel.cpp"
    break;

  case 247: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1194 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 8078 "bison-chapel.cpp"
    break;

  case 248: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1195 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 8084 "bison-chapel.cpp"
    break;

  case 249: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1200 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 8090 "bison-chapel.cpp"
    break;

  case 250: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1202 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8099 "bison-chapel.cpp"
    break;

  case 251: /* ifc_formal_ls: ifc_formal  */
#line 1209 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8105 "bison-chapel.cpp"
    break;

  case 252: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1210 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8111 "bison-chapel.cpp"
    break;

  case 253: /* ifc_formal: ident_def  */
#line 1215 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8117 "bison-chapel.cpp"
    break;

  case 254: /* implements_type_ident: TIDENT  */
#line 1220 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 8123 "bison-chapel.cpp"
    break;

  case 255: /* implements_type_ident: TBOOL  */
#line 1221 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 8129 "bison-chapel.cpp"
    break;

  case 256: /* implements_type_ident: TINT  */
#line 1222 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 8135 "bison-chapel.cpp"
    break;

  case 257: /* implements_type_ident: TUINT  */
#line 1223 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 8141 "bison-chapel.cpp"
    break;

  case 258: /* implements_type_ident: TREAL  */
#line 1224 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 8147 "bison-chapel.cpp"
    break;

  case 259: /* implements_type_ident: TIMAG  */
#line 1225 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 8153 "bison-chapel.cpp"
    break;

  case 260: /* implements_type_ident: TCOMPLEX  */
#line 1226 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 8159 "bison-chapel.cpp"
    break;

  case 261: /* implements_type_ident: TBYTES  */
#line 1227 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 8165 "bison-chapel.cpp"
    break;

  case 262: /* implements_type_ident: TSTRING  */
#line 1228 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 8171 "bison-chapel.cpp"
    break;

  case 263: /* implements_type_ident: TLOCALE  */
#line 1229 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 8177 "bison-chapel.cpp"
    break;

  case 264: /* implements_type_ident: TNOTHING  */
#line 1230 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 8183 "bison-chapel.cpp"
    break;

  case 265: /* implements_type_ident: TVOID  */
#line 1231 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 8189 "bison-chapel.cpp"
    break;

  case 266: /* implements_type_ident: implements_type_error_ident  */
#line 1233 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8196 "bison-chapel.cpp"
    break;

  case 267: /* implements_type_error_ident: TNONE  */
#line 1239 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 8202 "bison-chapel.cpp"
    break;

  case 268: /* implements_type_error_ident: TTHIS  */
#line 1240 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 8208 "bison-chapel.cpp"
    break;

  case 269: /* implements_type_error_ident: TFALSE  */
#line 1241 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 8214 "bison-chapel.cpp"
    break;

  case 270: /* implements_type_error_ident: TTRUE  */
#line 1242 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 8220 "bison-chapel.cpp"
    break;

  case 271: /* implements_type_error_ident: TDOMAIN  */
#line 1251 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 8226 "bison-chapel.cpp"
    break;

  case 272: /* implements_type_error_ident: TINDEX  */
#line 1252 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 8232 "bison-chapel.cpp"
    break;

  case 273: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1257 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8238 "bison-chapel.cpp"
    break;

  case 274: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1259 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8245 "bison-chapel.cpp"
    break;

  case 275: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1262 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8252 "bison-chapel.cpp"
    break;

  case 276: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1268 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8258 "bison-chapel.cpp"
    break;

  case 277: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1270 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8265 "bison-chapel.cpp"
    break;

  case 278: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1273 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8272 "bison-chapel.cpp"
    break;

  case 279: /* defer_stmt: TDEFER stmt  */
#line 1278 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8278 "bison-chapel.cpp"
    break;

  case 280: /* try_token: TTRY  */
#line 1281 "chapel.ypp"
           { (yyval.b) = false; }
#line 8284 "bison-chapel.cpp"
    break;

  case 281: /* try_token: TTRYBANG  */
#line 1282 "chapel.ypp"
           { (yyval.b) = true; }
#line 8290 "bison-chapel.cpp"
    break;

  case 282: /* try_stmt: try_token tryable_stmt  */
#line 1285 "chapel.ypp"
                         { (yyval.pblockstmt) = TryStmt::build((yyvsp[-1].b), (yyvsp[0].pblockstmt)); }
#line 8296 "bison-chapel.cpp"
    break;

  case 283: /* try_stmt: try_token block_stmt catch_stmt_ls  */
#line 1286 "chapel.ypp"
                                       { (yyval.pblockstmt) = TryStmt::build((yyvsp[-2].b), (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8302 "bison-chapel.cpp"
    break;

  case 284: /* catch_stmt_ls: %empty  */
#line 1290 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8308 "bison-chapel.cpp"
    break;

  case 285: /* catch_stmt_ls: catch_stmt_ls catch_stmt  */
#line 1291 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8314 "bison-chapel.cpp"
    break;

  case 286: /* catch_stmt: TCATCH block_stmt  */
#line 1295 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8320 "bison-chapel.cpp"
    break;

  case 287: /* catch_stmt: TCATCH catch_expr block_stmt  */
#line 1296 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8326 "bison-chapel.cpp"
    break;

  case 288: /* catch_stmt: TCATCH TLP catch_expr TRP block_stmt  */
#line 1297 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8332 "bison-chapel.cpp"
    break;

  case 289: /* catch_expr: ident_def  */
#line 1301 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8338 "bison-chapel.cpp"
    break;

  case 290: /* catch_expr: ident_def TCOLON expr  */
#line 1302 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8344 "bison-chapel.cpp"
    break;

  case 291: /* throw_stmt: TTHROW expr TSEMI  */
#line 1306 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8350 "bison-chapel.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1310 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8356 "bison-chapel.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1312 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8362 "bison-chapel.cpp"
    break;

  case 294: /* when_stmt_ls: %empty  */
#line 1316 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8368 "bison-chapel.cpp"
    break;

  case 295: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1317 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8374 "bison-chapel.cpp"
    break;

  case 296: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1322 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8380 "bison-chapel.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE stmt  */
#line 1324 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8386 "bison-chapel.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1326 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8392 "bison-chapel.cpp"
    break;

  case 299: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1333 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8406 "bison-chapel.cpp"
    break;

  case 300: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR error TRCBR  */
#line 1343 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8420 "bison-chapel.cpp"
    break;

  case 301: /* class_tag: TCLASS  */
#line 1356 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8430 "bison-chapel.cpp"
    break;

  case 302: /* class_tag: TRECORD  */
#line 1361 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8440 "bison-chapel.cpp"
    break;

  case 303: /* class_tag: TUNION  */
#line 1366 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8450 "bison-chapel.cpp"
    break;

  case 304: /* opt_inherit: %empty  */
#line 1374 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8456 "bison-chapel.cpp"
    break;

  case 305: /* opt_inherit: TCOLON expr_ls  */
#line 1375 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8462 "bison-chapel.cpp"
    break;

  case 306: /* class_level_stmt_ls: %empty  */
#line 1380 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8468 "bison-chapel.cpp"
    break;

  case 307: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1382 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8474 "bison-chapel.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1384 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8480 "bison-chapel.cpp"
    break;

  case 309: /* enum_decl_stmt: enum_header ident_def TLCBR enum_ls TRCBR  */
#line 1389 "chapel.ypp"
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
#line 8500 "bison-chapel.cpp"
    break;

  case 310: /* enum_decl_stmt: enum_header ident_def TLCBR error TRCBR  */
#line 1405 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8508 "bison-chapel.cpp"
    break;

  case 311: /* enum_header: TENUM  */
#line 1412 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8518 "bison-chapel.cpp"
    break;

  case 312: /* enum_ls: deprecated_enum_item  */
#line 1421 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8530 "bison-chapel.cpp"
    break;

  case 313: /* enum_ls: enum_ls TCOMMA  */
#line 1429 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8538 "bison-chapel.cpp"
    break;

  case 314: /* enum_ls: enum_ls TCOMMA deprecated_enum_item  */
#line 1433 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8546 "bison-chapel.cpp"
    break;

  case 316: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1441 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr), (yyvsp[-1].pch)); }
#line 8552 "bison-chapel.cpp"
    break;

  case 317: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1443 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr)); }
#line 8558 "bison-chapel.cpp"
    break;

  case 318: /* enum_item: ident_def  */
#line 1447 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8564 "bison-chapel.cpp"
    break;

  case 319: /* enum_item: ident_def TASSIGN expr  */
#line 1448 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8570 "bison-chapel.cpp"
    break;

  case 320: /* $@9: %empty  */
#line 1453 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8579 "bison-chapel.cpp"
    break;

  case 321: /* $@10: %empty  */
#line 1458 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8588 "bison-chapel.cpp"
    break;

  case 322: /* lambda_decl_expr: TLAMBDA $@9 req_formal_ls $@10 opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1463 "chapel.ypp"
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
#line 8610 "bison-chapel.cpp"
    break;

  case 323: /* linkage_spec: %empty  */
#line 1485 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8618 "bison-chapel.cpp"
    break;

  case 324: /* linkage_spec: TINLINE  */
#line 1488 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8627 "bison-chapel.cpp"
    break;

  case 325: /* linkage_spec: TOVERRIDE  */
#line 1492 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8636 "bison-chapel.cpp"
    break;

  case 326: /* $@11: %empty  */
#line 1500 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8650 "bison-chapel.cpp"
    break;

  case 327: /* $@12: %empty  */
#line 1510 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8661 "bison-chapel.cpp"
    break;

  case 328: /* fn_decl_stmt: linkage_spec proc_iter_or_op $@11 fn_decl_stmt_inner $@12 opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where opt_function_body_stmt  */
#line 1517 "chapel.ypp"
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
#line 8695 "bison-chapel.cpp"
    break;

  case 329: /* fn_decl_stmt_inner: opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1550 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8703 "bison-chapel.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1554 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8712 "bison-chapel.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1559 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8720 "bison-chapel.cpp"
    break;

  case 332: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1563 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8729 "bison-chapel.cpp"
    break;

  case 333: /* fn_decl_stmt_inner: opt_this_intent_tag error opt_formal_ls  */
#line 1568 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8737 "bison-chapel.cpp"
    break;

  case 335: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1575 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8743 "bison-chapel.cpp"
    break;

  case 336: /* fn_ident: ident_fn_def  */
#line 1579 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8749 "bison-chapel.cpp"
    break;

  case 337: /* fn_ident: ident_def TBANG  */
#line 1580 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8755 "bison-chapel.cpp"
    break;

  case 338: /* fn_ident: op_ident  */
#line 1581 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8761 "bison-chapel.cpp"
    break;

  case 339: /* op_ident: TBAND  */
#line 1585 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8767 "bison-chapel.cpp"
    break;

  case 340: /* op_ident: TBOR  */
#line 1586 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8773 "bison-chapel.cpp"
    break;

  case 341: /* op_ident: TBXOR  */
#line 1587 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8779 "bison-chapel.cpp"
    break;

  case 342: /* op_ident: TBNOT  */
#line 1588 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8785 "bison-chapel.cpp"
    break;

  case 343: /* op_ident: TEQUAL  */
#line 1589 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8791 "bison-chapel.cpp"
    break;

  case 344: /* op_ident: TNOTEQUAL  */
#line 1590 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8797 "bison-chapel.cpp"
    break;

  case 345: /* op_ident: TLESSEQUAL  */
#line 1591 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8803 "bison-chapel.cpp"
    break;

  case 346: /* op_ident: TGREATEREQUAL  */
#line 1592 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8809 "bison-chapel.cpp"
    break;

  case 347: /* op_ident: TLESS  */
#line 1593 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8815 "bison-chapel.cpp"
    break;

  case 348: /* op_ident: TGREATER  */
#line 1594 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8821 "bison-chapel.cpp"
    break;

  case 349: /* op_ident: TPLUS  */
#line 1595 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8827 "bison-chapel.cpp"
    break;

  case 350: /* op_ident: TMINUS  */
#line 1596 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8833 "bison-chapel.cpp"
    break;

  case 351: /* op_ident: TSTAR  */
#line 1597 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8839 "bison-chapel.cpp"
    break;

  case 352: /* op_ident: TDIVIDE  */
#line 1598 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8845 "bison-chapel.cpp"
    break;

  case 353: /* op_ident: TSHIFTLEFT  */
#line 1599 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8851 "bison-chapel.cpp"
    break;

  case 354: /* op_ident: TSHIFTRIGHT  */
#line 1600 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8857 "bison-chapel.cpp"
    break;

  case 355: /* op_ident: TMOD  */
#line 1601 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8863 "bison-chapel.cpp"
    break;

  case 356: /* op_ident: TEXP  */
#line 1602 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8869 "bison-chapel.cpp"
    break;

  case 357: /* op_ident: TBANG  */
#line 1603 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8875 "bison-chapel.cpp"
    break;

  case 358: /* op_ident: TBY  */
#line 1604 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8881 "bison-chapel.cpp"
    break;

  case 359: /* op_ident: THASH  */
#line 1605 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8887 "bison-chapel.cpp"
    break;

  case 360: /* op_ident: TALIGN  */
#line 1606 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8893 "bison-chapel.cpp"
    break;

  case 361: /* op_ident: TSWAP  */
#line 1607 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8899 "bison-chapel.cpp"
    break;

  case 362: /* op_ident: TIO  */
#line 1608 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8905 "bison-chapel.cpp"
    break;

  case 363: /* op_ident: TINITEQUALS  */
#line 1609 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8911 "bison-chapel.cpp"
    break;

  case 364: /* op_ident: TCOLON  */
#line 1610 "chapel.ypp"
                 { (yyval.pch) = ":"; }
#line 8917 "bison-chapel.cpp"
    break;

  case 365: /* assignop_ident: TASSIGN  */
#line 1614 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8923 "bison-chapel.cpp"
    break;

  case 366: /* assignop_ident: TASSIGNPLUS  */
#line 1615 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8929 "bison-chapel.cpp"
    break;

  case 367: /* assignop_ident: TASSIGNMINUS  */
#line 1616 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8935 "bison-chapel.cpp"
    break;

  case 368: /* assignop_ident: TASSIGNMULTIPLY  */
#line 1617 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8941 "bison-chapel.cpp"
    break;

  case 369: /* assignop_ident: TASSIGNDIVIDE  */
#line 1618 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8947 "bison-chapel.cpp"
    break;

  case 370: /* assignop_ident: TASSIGNMOD  */
#line 1619 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8953 "bison-chapel.cpp"
    break;

  case 371: /* assignop_ident: TASSIGNEXP  */
#line 1620 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8959 "bison-chapel.cpp"
    break;

  case 372: /* assignop_ident: TASSIGNBAND  */
#line 1621 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8965 "bison-chapel.cpp"
    break;

  case 373: /* assignop_ident: TASSIGNBOR  */
#line 1622 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8971 "bison-chapel.cpp"
    break;

  case 374: /* assignop_ident: TASSIGNBXOR  */
#line 1623 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8977 "bison-chapel.cpp"
    break;

  case 375: /* assignop_ident: TASSIGNSR  */
#line 1624 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8983 "bison-chapel.cpp"
    break;

  case 376: /* assignop_ident: TASSIGNSL  */
#line 1625 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 8989 "bison-chapel.cpp"
    break;

  case 377: /* all_op_name: op_ident  */
#line 1629 "chapel.ypp"
                 { (yyval.pch) = (yyvsp[0].pch); }
#line 8995 "bison-chapel.cpp"
    break;

  case 378: /* all_op_name: assignop_ident  */
#line 1630 "chapel.ypp"
                 { (yyval.pch) = (yyvsp[0].pch); }
#line 9001 "bison-chapel.cpp"
    break;

  case 379: /* opt_formal_ls: %empty  */
#line 1634 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 9007 "bison-chapel.cpp"
    break;

  case 380: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1635 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9013 "bison-chapel.cpp"
    break;

  case 381: /* req_formal_ls: TLP formal_ls TRP  */
#line 1639 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 9019 "bison-chapel.cpp"
    break;

  case 382: /* formal_ls_inner: formal  */
#line 1643 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 9025 "bison-chapel.cpp"
    break;

  case 383: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 1644 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 9031 "bison-chapel.cpp"
    break;

  case 384: /* formal_ls: %empty  */
#line 1648 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 9037 "bison-chapel.cpp"
    break;

  case 385: /* formal_ls: formal_ls_inner  */
#line 1649 "chapel.ypp"
                           { (yyval.pfnsymbol) = (yyvsp[0].pfnsymbol); }
#line 9043 "bison-chapel.cpp"
    break;

  case 386: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1654 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 9049 "bison-chapel.cpp"
    break;

  case 387: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1656 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 9055 "bison-chapel.cpp"
    break;

  case 388: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1658 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 9061 "bison-chapel.cpp"
    break;

  case 389: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1660 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 9067 "bison-chapel.cpp"
    break;

  case 390: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1662 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9073 "bison-chapel.cpp"
    break;

  case 391: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1664 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 9079 "bison-chapel.cpp"
    break;

  case 392: /* opt_intent_tag: %empty  */
#line 1668 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 9085 "bison-chapel.cpp"
    break;

  case 393: /* opt_intent_tag: required_intent_tag  */
#line 1669 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 9091 "bison-chapel.cpp"
    break;

  case 394: /* required_intent_tag: TIN  */
#line 1673 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 9097 "bison-chapel.cpp"
    break;

  case 395: /* required_intent_tag: TINOUT  */
#line 1674 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 9103 "bison-chapel.cpp"
    break;

  case 396: /* required_intent_tag: TOUT  */
#line 1675 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 9109 "bison-chapel.cpp"
    break;

  case 397: /* required_intent_tag: TCONST  */
#line 1676 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 9115 "bison-chapel.cpp"
    break;

  case 398: /* required_intent_tag: TCONST TIN  */
#line 1677 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 9121 "bison-chapel.cpp"
    break;

  case 399: /* required_intent_tag: TCONST TREF  */
#line 1678 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 9127 "bison-chapel.cpp"
    break;

  case 400: /* required_intent_tag: TPARAM  */
#line 1679 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 9133 "bison-chapel.cpp"
    break;

  case 401: /* required_intent_tag: TREF  */
#line 1680 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 9139 "bison-chapel.cpp"
    break;

  case 402: /* required_intent_tag: TTYPE  */
#line 1681 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 9145 "bison-chapel.cpp"
    break;

  case 403: /* opt_this_intent_tag: %empty  */
#line 1685 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 9151 "bison-chapel.cpp"
    break;

  case 404: /* opt_this_intent_tag: TPARAM  */
#line 1686 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 9157 "bison-chapel.cpp"
    break;

  case 405: /* opt_this_intent_tag: TREF  */
#line 1687 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 9163 "bison-chapel.cpp"
    break;

  case 406: /* opt_this_intent_tag: TCONST TREF  */
#line 1688 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 9169 "bison-chapel.cpp"
    break;

  case 407: /* opt_this_intent_tag: TCONST  */
#line 1689 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 9175 "bison-chapel.cpp"
    break;

  case 408: /* opt_this_intent_tag: TTYPE  */
#line 1690 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 9181 "bison-chapel.cpp"
    break;

  case 409: /* proc_iter_or_op: TPROC  */
#line 1694 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 9187 "bison-chapel.cpp"
    break;

  case 410: /* proc_iter_or_op: TITER  */
#line 1695 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 9193 "bison-chapel.cpp"
    break;

  case 411: /* proc_iter_or_op: TOPERATOR  */
#line 1696 "chapel.ypp"
            { (yyval.procIterOp) = ProcIterOp_OP; }
#line 9199 "bison-chapel.cpp"
    break;

  case 412: /* opt_ret_tag: %empty  */
#line 1700 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9205 "bison-chapel.cpp"
    break;

  case 413: /* opt_ret_tag: TCONST  */
#line 1701 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9211 "bison-chapel.cpp"
    break;

  case 414: /* opt_ret_tag: TCONST TREF  */
#line 1702 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 9217 "bison-chapel.cpp"
    break;

  case 415: /* opt_ret_tag: TREF  */
#line 1703 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 9223 "bison-chapel.cpp"
    break;

  case 416: /* opt_ret_tag: TPARAM  */
#line 1704 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 9229 "bison-chapel.cpp"
    break;

  case 417: /* opt_ret_tag: TTYPE  */
#line 1705 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 9235 "bison-chapel.cpp"
    break;

  case 418: /* opt_throws_error: %empty  */
#line 1709 "chapel.ypp"
          { (yyval.b) = false; }
#line 9241 "bison-chapel.cpp"
    break;

  case 419: /* opt_throws_error: TTHROWS  */
#line 1710 "chapel.ypp"
          { (yyval.b) = true;  }
#line 9247 "bison-chapel.cpp"
    break;

  case 420: /* opt_function_body_stmt: TSEMI  */
#line 1713 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 9253 "bison-chapel.cpp"
    break;

  case 423: /* function_body_stmt: return_stmt  */
#line 1719 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9259 "bison-chapel.cpp"
    break;

  case 424: /* query_expr: TQUERIEDIDENT  */
#line 1723 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9265 "bison-chapel.cpp"
    break;

  case 425: /* var_arg_expr: TDOTDOTDOT  */
#line 1727 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9271 "bison-chapel.cpp"
    break;

  case 426: /* var_arg_expr: TDOTDOTDOT expr  */
#line 1728 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9277 "bison-chapel.cpp"
    break;

  case 427: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 1729 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9287 "bison-chapel.cpp"
    break;

  case 428: /* opt_lifetime_where: %empty  */
#line 1737 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9293 "bison-chapel.cpp"
    break;

  case 429: /* opt_lifetime_where: TWHERE expr  */
#line 1739 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9299 "bison-chapel.cpp"
    break;

  case 430: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 1741 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9305 "bison-chapel.cpp"
    break;

  case 431: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 1743 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9311 "bison-chapel.cpp"
    break;

  case 432: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 1745 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9317 "bison-chapel.cpp"
    break;

  case 433: /* lifetime_components_expr: lifetime_expr  */
#line 1750 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9323 "bison-chapel.cpp"
    break;

  case 434: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 1752 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9329 "bison-chapel.cpp"
    break;

  case 435: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 1756 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9335 "bison-chapel.cpp"
    break;

  case 436: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 1757 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9341 "bison-chapel.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 1758 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9347 "bison-chapel.cpp"
    break;

  case 438: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 1759 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9353 "bison-chapel.cpp"
    break;

  case 439: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 1760 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9359 "bison-chapel.cpp"
    break;

  case 440: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 1761 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9365 "bison-chapel.cpp"
    break;

  case 441: /* lifetime_expr: TRETURN lifetime_ident  */
#line 1762 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9371 "bison-chapel.cpp"
    break;

  case 442: /* lifetime_ident: TIDENT  */
#line 1767 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9377 "bison-chapel.cpp"
    break;

  case 443: /* lifetime_ident: TTHIS  */
#line 1769 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9383 "bison-chapel.cpp"
    break;

  case 444: /* type_alias_decl_stmt: TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1774 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9389 "bison-chapel.cpp"
    break;

  case 445: /* type_alias_decl_stmt: TCONFIG TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1776 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9395 "bison-chapel.cpp"
    break;

  case 446: /* type_alias_decl_stmt: TEXTERN TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1778 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9401 "bison-chapel.cpp"
    break;

  case 447: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 1783 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9418 "bison-chapel.cpp"
    break;

  case 448: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 1796 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9436 "bison-chapel.cpp"
    break;

  case 449: /* opt_init_type: %empty  */
#line 1812 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9442 "bison-chapel.cpp"
    break;

  case 450: /* opt_init_type: TASSIGN type_level_expr  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9448 "bison-chapel.cpp"
    break;

  case 451: /* opt_init_type: TASSIGN array_type  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9454 "bison-chapel.cpp"
    break;

  case 452: /* var_decl_type: TPARAM  */
#line 1820 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9460 "bison-chapel.cpp"
    break;

  case 453: /* var_decl_type: TCONST  */
#line 1821 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9466 "bison-chapel.cpp"
    break;

  case 454: /* var_decl_type: TREF  */
#line 1822 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9472 "bison-chapel.cpp"
    break;

  case 455: /* var_decl_type: TCONST TREF  */
#line 1823 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9478 "bison-chapel.cpp"
    break;

  case 456: /* var_decl_type: TVAR  */
#line 1824 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9484 "bison-chapel.cpp"
    break;

  case 457: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1829 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9494 "bison-chapel.cpp"
    break;

  case 458: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1835 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9503 "bison-chapel.cpp"
    break;

  case 460: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 1844 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9512 "bison-chapel.cpp"
    break;

  case 461: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 1852 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9518 "bison-chapel.cpp"
    break;

  case 462: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 1854 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9524 "bison-chapel.cpp"
    break;

  case 463: /* tuple_var_decl_component: TUNDERSCORE  */
#line 1859 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9530 "bison-chapel.cpp"
    break;

  case 464: /* tuple_var_decl_component: ident_def  */
#line 1861 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9536 "bison-chapel.cpp"
    break;

  case 465: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 1863 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9542 "bison-chapel.cpp"
    break;

  case 466: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 1868 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9548 "bison-chapel.cpp"
    break;

  case 467: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 1870 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9554 "bison-chapel.cpp"
    break;

  case 468: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 1872 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9560 "bison-chapel.cpp"
    break;

  case 469: /* opt_init_expr: %empty  */
#line 1878 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9566 "bison-chapel.cpp"
    break;

  case 470: /* opt_init_expr: TASSIGN TNOINIT  */
#line 1879 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9572 "bison-chapel.cpp"
    break;

  case 471: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 1880 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9578 "bison-chapel.cpp"
    break;

  case 472: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 1886 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9584 "bison-chapel.cpp"
    break;

  case 473: /* ret_array_type: TLSBR TRSBR  */
#line 1888 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9590 "bison-chapel.cpp"
    break;

  case 474: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1890 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9598 "bison-chapel.cpp"
    break;

  case 475: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 1894 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9606 "bison-chapel.cpp"
    break;

  case 476: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 1898 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9612 "bison-chapel.cpp"
    break;

  case 477: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 1900 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9620 "bison-chapel.cpp"
    break;

  case 478: /* ret_array_type: TLSBR error TRSBR  */
#line 1904 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9628 "bison-chapel.cpp"
    break;

  case 479: /* opt_ret_type: %empty  */
#line 1911 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9634 "bison-chapel.cpp"
    break;

  case 480: /* opt_ret_type: TCOLON type_level_expr  */
#line 1912 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9640 "bison-chapel.cpp"
    break;

  case 481: /* opt_ret_type: TCOLON ret_array_type  */
#line 1913 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9646 "bison-chapel.cpp"
    break;

  case 482: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 1914 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9652 "bison-chapel.cpp"
    break;

  case 483: /* opt_ret_type: error  */
#line 1915 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9658 "bison-chapel.cpp"
    break;

  case 484: /* opt_type: %empty  */
#line 1920 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9664 "bison-chapel.cpp"
    break;

  case 485: /* opt_type: TCOLON type_level_expr  */
#line 1921 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9670 "bison-chapel.cpp"
    break;

  case 486: /* opt_type: TCOLON array_type  */
#line 1922 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9676 "bison-chapel.cpp"
    break;

  case 487: /* opt_type: TCOLON reserved_type_ident_use  */
#line 1923 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9682 "bison-chapel.cpp"
    break;

  case 488: /* opt_type: error  */
#line 1924 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9688 "bison-chapel.cpp"
    break;

  case 489: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1945 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9696 "bison-chapel.cpp"
    break;

  case 490: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 1949 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9704 "bison-chapel.cpp"
    break;

  case 491: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 1953 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9716 "bison-chapel.cpp"
    break;

  case 492: /* array_type: TLSBR error TRSBR  */
#line 1961 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9724 "bison-chapel.cpp"
    break;

  case 493: /* opt_formal_array_elt_type: %empty  */
#line 1967 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9730 "bison-chapel.cpp"
    break;

  case 494: /* opt_formal_array_elt_type: type_level_expr  */
#line 1968 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9736 "bison-chapel.cpp"
    break;

  case 495: /* opt_formal_array_elt_type: query_expr  */
#line 1969 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9742 "bison-chapel.cpp"
    break;

  case 496: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9748 "bison-chapel.cpp"
    break;

  case 497: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9754 "bison-chapel.cpp"
    break;

  case 498: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9760 "bison-chapel.cpp"
    break;

  case 499: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 1984 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9766 "bison-chapel.cpp"
    break;

  case 500: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 1986 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9772 "bison-chapel.cpp"
    break;

  case 501: /* opt_formal_type: %empty  */
#line 1990 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9778 "bison-chapel.cpp"
    break;

  case 502: /* opt_formal_type: TCOLON type_level_expr  */
#line 1991 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9784 "bison-chapel.cpp"
    break;

  case 503: /* opt_formal_type: TCOLON query_expr  */
#line 1992 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9790 "bison-chapel.cpp"
    break;

  case 504: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 1993 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9796 "bison-chapel.cpp"
    break;

  case 505: /* opt_formal_type: TCOLON formal_array_type  */
#line 1994 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9802 "bison-chapel.cpp"
    break;

  case 506: /* expr_ls: expr  */
#line 2000 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9808 "bison-chapel.cpp"
    break;

  case 507: /* expr_ls: query_expr  */
#line 2001 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9814 "bison-chapel.cpp"
    break;

  case 508: /* expr_ls: expr_ls TCOMMA expr  */
#line 2002 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9820 "bison-chapel.cpp"
    break;

  case 509: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2003 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9826 "bison-chapel.cpp"
    break;

  case 510: /* simple_expr_ls: expr  */
#line 2007 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9832 "bison-chapel.cpp"
    break;

  case 511: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2008 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9838 "bison-chapel.cpp"
    break;

  case 512: /* tuple_component: TUNDERSCORE  */
#line 2012 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9844 "bison-chapel.cpp"
    break;

  case 513: /* tuple_component: opt_try_expr  */
#line 2013 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9850 "bison-chapel.cpp"
    break;

  case 514: /* tuple_component: query_expr  */
#line 2014 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9856 "bison-chapel.cpp"
    break;

  case 515: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2018 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9862 "bison-chapel.cpp"
    break;

  case 516: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2019 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9868 "bison-chapel.cpp"
    break;

  case 517: /* opt_actual_ls: %empty  */
#line 2023 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9874 "bison-chapel.cpp"
    break;

  case 519: /* actual_ls: actual_expr  */
#line 2028 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9880 "bison-chapel.cpp"
    break;

  case 520: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2029 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9886 "bison-chapel.cpp"
    break;

  case 521: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2033 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9892 "bison-chapel.cpp"
    break;

  case 522: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2034 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9898 "bison-chapel.cpp"
    break;

  case 523: /* actual_expr: query_expr  */
#line 2035 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9904 "bison-chapel.cpp"
    break;

  case 524: /* actual_expr: opt_try_expr  */
#line 2036 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9910 "bison-chapel.cpp"
    break;

  case 525: /* ident_expr: ident_use  */
#line 2040 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9916 "bison-chapel.cpp"
    break;

  case 526: /* ident_expr: scalar_type  */
#line 2041 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9922 "bison-chapel.cpp"
    break;

  case 527: /* type_level_expr: sub_type_level_expr  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9928 "bison-chapel.cpp"
    break;

  case 528: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9934 "bison-chapel.cpp"
    break;

  case 529: /* type_level_expr: TQUESTION  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9940 "bison-chapel.cpp"
    break;

  case 535: /* sub_type_level_expr: TSINGLE expr  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9946 "bison-chapel.cpp"
    break;

  case 536: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9952 "bison-chapel.cpp"
    break;

  case 537: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2072 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9958 "bison-chapel.cpp"
    break;

  case 538: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9964 "bison-chapel.cpp"
    break;

  case 539: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pexpr)); }
#line 9970 "bison-chapel.cpp"
    break;

  case 540: /* sub_type_level_expr: TATOMIC expr  */
#line 2078 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9976 "bison-chapel.cpp"
    break;

  case 541: /* sub_type_level_expr: TSYNC expr  */
#line 2080 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9982 "bison-chapel.cpp"
    break;

  case 542: /* sub_type_level_expr: TOWNED  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9988 "bison-chapel.cpp"
    break;

  case 543: /* sub_type_level_expr: TOWNED expr  */
#line 2085 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9994 "bison-chapel.cpp"
    break;

  case 544: /* sub_type_level_expr: TUNMANAGED  */
#line 2087 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 10000 "bison-chapel.cpp"
    break;

  case 545: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2089 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10006 "bison-chapel.cpp"
    break;

  case 546: /* sub_type_level_expr: TSHARED  */
#line 2091 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 10012 "bison-chapel.cpp"
    break;

  case 547: /* sub_type_level_expr: TSHARED expr  */
#line 2093 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 10018 "bison-chapel.cpp"
    break;

  case 548: /* sub_type_level_expr: TBORROWED  */
#line 2095 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 10024 "bison-chapel.cpp"
    break;

  case 549: /* sub_type_level_expr: TBORROWED expr  */
#line 2097 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 10030 "bison-chapel.cpp"
    break;

  case 550: /* sub_type_level_expr: TCLASS  */
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 10036 "bison-chapel.cpp"
    break;

  case 551: /* sub_type_level_expr: TRECORD  */
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 10042 "bison-chapel.cpp"
    break;

  case 552: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2107 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10048 "bison-chapel.cpp"
    break;

  case 553: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2109 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10054 "bison-chapel.cpp"
    break;

  case 554: /* for_expr: TFOR expr TDO expr  */
#line 2111 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10060 "bison-chapel.cpp"
    break;

  case 555: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2113 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10066 "bison-chapel.cpp"
    break;

  case 556: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2115 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10072 "bison-chapel.cpp"
    break;

  case 557: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2117 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10078 "bison-chapel.cpp"
    break;

  case 558: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2119 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10084 "bison-chapel.cpp"
    break;

  case 559: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2121 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10090 "bison-chapel.cpp"
    break;

  case 560: /* for_expr: TFORALL expr TDO expr  */
#line 2123 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10096 "bison-chapel.cpp"
    break;

  case 561: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2125 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10102 "bison-chapel.cpp"
    break;

  case 562: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2127 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10108 "bison-chapel.cpp"
    break;

  case 563: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2129 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10114 "bison-chapel.cpp"
    break;

  case 564: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2131 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 10125 "bison-chapel.cpp"
    break;

  case 565: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2138 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 10135 "bison-chapel.cpp"
    break;

  case 566: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2144 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10145 "bison-chapel.cpp"
    break;

  case 567: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2150 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10155 "bison-chapel.cpp"
    break;

  case 568: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2156 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10165 "bison-chapel.cpp"
    break;

  case 569: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2165 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10171 "bison-chapel.cpp"
    break;

  case 570: /* nil_expr: TNIL  */
#line 2174 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 10177 "bison-chapel.cpp"
    break;

  case 578: /* stmt_level_expr: io_expr TIO expr  */
#line 2190 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10183 "bison-chapel.cpp"
    break;

  case 579: /* opt_task_intent_ls: %empty  */
#line 2194 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 10189 "bison-chapel.cpp"
    break;

  case 581: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2199 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10195 "bison-chapel.cpp"
    break;

  case 582: /* task_intent_ls: intent_expr  */
#line 2203 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10201 "bison-chapel.cpp"
    break;

  case 583: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2204 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10207 "bison-chapel.cpp"
    break;

  case 584: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2208 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10213 "bison-chapel.cpp"
    break;

  case 585: /* forall_intent_ls: intent_expr  */
#line 2212 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10219 "bison-chapel.cpp"
    break;

  case 586: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2213 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10225 "bison-chapel.cpp"
    break;

  case 587: /* intent_expr: shadow_var_prefix ident_expr opt_type opt_init_expr  */
#line 2218 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10233 "bison-chapel.cpp"
    break;

  case 588: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2222 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10241 "bison-chapel.cpp"
    break;

  case 589: /* intent_expr: expr TREDUCE ident_expr  */
#line 2226 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10249 "bison-chapel.cpp"
    break;

  case 590: /* shadow_var_prefix: TCONST  */
#line 2232 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10255 "bison-chapel.cpp"
    break;

  case 591: /* shadow_var_prefix: TIN  */
#line 2233 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10261 "bison-chapel.cpp"
    break;

  case 592: /* shadow_var_prefix: TCONST TIN  */
#line 2234 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10267 "bison-chapel.cpp"
    break;

  case 593: /* shadow_var_prefix: TREF  */
#line 2235 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10273 "bison-chapel.cpp"
    break;

  case 594: /* shadow_var_prefix: TCONST TREF  */
#line 2236 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10279 "bison-chapel.cpp"
    break;

  case 595: /* shadow_var_prefix: TVAR  */
#line 2237 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10285 "bison-chapel.cpp"
    break;

  case 597: /* io_expr: io_expr TIO expr  */
#line 2243 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10291 "bison-chapel.cpp"
    break;

  case 598: /* new_maybe_decorated: TNEW  */
#line 2248 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10297 "bison-chapel.cpp"
    break;

  case 599: /* new_maybe_decorated: TNEW TOWNED  */
#line 2250 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10305 "bison-chapel.cpp"
    break;

  case 600: /* new_maybe_decorated: TNEW TSHARED  */
#line 2254 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10313 "bison-chapel.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2258 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10321 "bison-chapel.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2262 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10329 "bison-chapel.cpp"
    break;

  case 603: /* new_expr: new_maybe_decorated expr  */
#line 2270 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10336 "bison-chapel.cpp"
    break;

  case 604: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2275 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10346 "bison-chapel.cpp"
    break;

  case 605: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2281 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10356 "bison-chapel.cpp"
    break;

  case 606: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2287 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10367 "bison-chapel.cpp"
    break;

  case 607: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2294 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10378 "bison-chapel.cpp"
    break;

  case 608: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2304 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10384 "bison-chapel.cpp"
    break;

  case 618: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2321 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10390 "bison-chapel.cpp"
    break;

  case 619: /* expr: expr TCOLON expr  */
#line 2323 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10396 "bison-chapel.cpp"
    break;

  case 620: /* expr: expr TDOTDOT expr  */
#line 2325 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10402 "bison-chapel.cpp"
    break;

  case 621: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2327 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10408 "bison-chapel.cpp"
    break;

  case 622: /* expr: expr TDOTDOT  */
#line 2342 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10414 "bison-chapel.cpp"
    break;

  case 623: /* expr: TDOTDOT expr  */
#line 2344 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10420 "bison-chapel.cpp"
    break;

  case 624: /* expr: TDOTDOTOPENHIGH expr  */
#line 2346 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10426 "bison-chapel.cpp"
    break;

  case 625: /* expr: TDOTDOT  */
#line 2348 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10432 "bison-chapel.cpp"
    break;

  case 626: /* opt_expr: %empty  */
#line 2352 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10438 "bison-chapel.cpp"
    break;

  case 627: /* opt_expr: expr  */
#line 2353 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10444 "bison-chapel.cpp"
    break;

  case 628: /* opt_try_expr: TTRY expr  */
#line 2357 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10450 "bison-chapel.cpp"
    break;

  case 629: /* opt_try_expr: TTRYBANG expr  */
#line 2358 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10456 "bison-chapel.cpp"
    break;

  case 630: /* opt_try_expr: expr  */
#line 2359 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10462 "bison-chapel.cpp"
    break;

  case 635: /* call_base_expr: lhs_expr  */
#line 2375 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10468 "bison-chapel.cpp"
    break;

  case 636: /* call_base_expr: expr TBANG  */
#line 2376 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10474 "bison-chapel.cpp"
    break;

  case 637: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2377 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10480 "bison-chapel.cpp"
    break;

  case 638: /* call_base_expr: lambda_decl_expr  */
#line 2378 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10486 "bison-chapel.cpp"
    break;

  case 640: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2383 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10492 "bison-chapel.cpp"
    break;

  case 641: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2384 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10498 "bison-chapel.cpp"
    break;

  case 642: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2385 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10504 "bison-chapel.cpp"
    break;

  case 643: /* dot_expr: expr TDOT ident_use  */
#line 2389 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10510 "bison-chapel.cpp"
    break;

  case 644: /* dot_expr: expr TDOT TTYPE  */
#line 2390 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10516 "bison-chapel.cpp"
    break;

  case 645: /* dot_expr: expr TDOT TDOMAIN  */
#line 2391 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10522 "bison-chapel.cpp"
    break;

  case 646: /* dot_expr: expr TDOT TLOCALE  */
#line 2392 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10528 "bison-chapel.cpp"
    break;

  case 647: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2393 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10534 "bison-chapel.cpp"
    break;

  case 648: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2401 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10540 "bison-chapel.cpp"
    break;

  case 649: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2402 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10546 "bison-chapel.cpp"
    break;

  case 650: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2403 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10552 "bison-chapel.cpp"
    break;

  case 651: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2404 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10558 "bison-chapel.cpp"
    break;

  case 652: /* bool_literal: TFALSE  */
#line 2408 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10564 "bison-chapel.cpp"
    break;

  case 653: /* bool_literal: TTRUE  */
#line 2409 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10570 "bison-chapel.cpp"
    break;

  case 654: /* str_bytes_literal: STRINGLITERAL  */
#line 2413 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10576 "bison-chapel.cpp"
    break;

  case 655: /* str_bytes_literal: BYTESLITERAL  */
#line 2414 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10582 "bison-chapel.cpp"
    break;

  case 658: /* literal_expr: INTLITERAL  */
#line 2420 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10588 "bison-chapel.cpp"
    break;

  case 659: /* literal_expr: REALLITERAL  */
#line 2421 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10594 "bison-chapel.cpp"
    break;

  case 660: /* literal_expr: IMAGLITERAL  */
#line 2422 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10600 "bison-chapel.cpp"
    break;

  case 661: /* literal_expr: CSTRINGLITERAL  */
#line 2423 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10606 "bison-chapel.cpp"
    break;

  case 662: /* literal_expr: TNONE  */
#line 2424 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10612 "bison-chapel.cpp"
    break;

  case 663: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2425 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10619 "bison-chapel.cpp"
    break;

  case 664: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2427 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10626 "bison-chapel.cpp"
    break;

  case 665: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2429 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10632 "bison-chapel.cpp"
    break;

  case 666: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2430 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10638 "bison-chapel.cpp"
    break;

  case 667: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2432 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10646 "bison-chapel.cpp"
    break;

  case 668: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2436 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10654 "bison-chapel.cpp"
    break;

  case 669: /* assoc_expr_ls: expr TALIAS expr  */
#line 2443 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10660 "bison-chapel.cpp"
    break;

  case 670: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2444 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10666 "bison-chapel.cpp"
    break;

  case 671: /* binary_op_expr: expr TPLUS expr  */
#line 2448 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10672 "bison-chapel.cpp"
    break;

  case 672: /* binary_op_expr: expr TMINUS expr  */
#line 2449 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10678 "bison-chapel.cpp"
    break;

  case 673: /* binary_op_expr: expr TSTAR expr  */
#line 2450 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10684 "bison-chapel.cpp"
    break;

  case 674: /* binary_op_expr: expr TDIVIDE expr  */
#line 2451 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10690 "bison-chapel.cpp"
    break;

  case 675: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2452 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10696 "bison-chapel.cpp"
    break;

  case 676: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2453 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10702 "bison-chapel.cpp"
    break;

  case 677: /* binary_op_expr: expr TMOD expr  */
#line 2454 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10708 "bison-chapel.cpp"
    break;

  case 678: /* binary_op_expr: expr TEQUAL expr  */
#line 2455 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10714 "bison-chapel.cpp"
    break;

  case 679: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2456 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10720 "bison-chapel.cpp"
    break;

  case 680: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2457 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10726 "bison-chapel.cpp"
    break;

  case 681: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2458 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10732 "bison-chapel.cpp"
    break;

  case 682: /* binary_op_expr: expr TLESS expr  */
#line 2459 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10738 "bison-chapel.cpp"
    break;

  case 683: /* binary_op_expr: expr TGREATER expr  */
#line 2460 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10744 "bison-chapel.cpp"
    break;

  case 684: /* binary_op_expr: expr TBAND expr  */
#line 2461 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10750 "bison-chapel.cpp"
    break;

  case 685: /* binary_op_expr: expr TBOR expr  */
#line 2462 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10756 "bison-chapel.cpp"
    break;

  case 686: /* binary_op_expr: expr TBXOR expr  */
#line 2463 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10762 "bison-chapel.cpp"
    break;

  case 687: /* binary_op_expr: expr TAND expr  */
#line 2464 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10768 "bison-chapel.cpp"
    break;

  case 688: /* binary_op_expr: expr TOR expr  */
#line 2465 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10774 "bison-chapel.cpp"
    break;

  case 689: /* binary_op_expr: expr TEXP expr  */
#line 2466 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10780 "bison-chapel.cpp"
    break;

  case 690: /* binary_op_expr: expr TBY expr  */
#line 2467 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10786 "bison-chapel.cpp"
    break;

  case 691: /* binary_op_expr: expr TALIGN expr  */
#line 2468 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10792 "bison-chapel.cpp"
    break;

  case 692: /* binary_op_expr: expr THASH expr  */
#line 2469 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10798 "bison-chapel.cpp"
    break;

  case 693: /* binary_op_expr: expr TDMAPPED expr  */
#line 2470 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10805 "bison-chapel.cpp"
    break;

  case 694: /* unary_op_expr: TPLUS expr  */
#line 2475 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10811 "bison-chapel.cpp"
    break;

  case 695: /* unary_op_expr: TMINUS expr  */
#line 2476 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10817 "bison-chapel.cpp"
    break;

  case 696: /* unary_op_expr: TMINUSMINUS expr  */
#line 2477 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10823 "bison-chapel.cpp"
    break;

  case 697: /* unary_op_expr: TPLUSPLUS expr  */
#line 2478 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10829 "bison-chapel.cpp"
    break;

  case 698: /* unary_op_expr: TBANG expr  */
#line 2479 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10835 "bison-chapel.cpp"
    break;

  case 699: /* unary_op_expr: expr TBANG  */
#line 2480 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10841 "bison-chapel.cpp"
    break;

  case 700: /* unary_op_expr: TBNOT expr  */
#line 2481 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10847 "bison-chapel.cpp"
    break;

  case 701: /* reduce_expr: expr TREDUCE expr  */
#line 2485 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10853 "bison-chapel.cpp"
    break;

  case 702: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2486 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10859 "bison-chapel.cpp"
    break;

  case 703: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2487 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10865 "bison-chapel.cpp"
    break;

  case 704: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2488 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10871 "bison-chapel.cpp"
    break;

  case 705: /* scan_expr: expr TSCAN expr  */
#line 2492 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10877 "bison-chapel.cpp"
    break;

  case 706: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2493 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10883 "bison-chapel.cpp"
    break;

  case 707: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2494 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10889 "bison-chapel.cpp"
    break;

  case 708: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2495 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10895 "bison-chapel.cpp"
    break;

  case 709: /* reduce_scan_op_expr: TPLUS  */
#line 2500 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10901 "bison-chapel.cpp"
    break;

  case 710: /* reduce_scan_op_expr: TSTAR  */
#line 2501 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10907 "bison-chapel.cpp"
    break;

  case 711: /* reduce_scan_op_expr: TAND  */
#line 2502 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10913 "bison-chapel.cpp"
    break;

  case 712: /* reduce_scan_op_expr: TOR  */
#line 2503 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10919 "bison-chapel.cpp"
    break;

  case 713: /* reduce_scan_op_expr: TBAND  */
#line 2504 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10925 "bison-chapel.cpp"
    break;

  case 714: /* reduce_scan_op_expr: TBOR  */
#line 2505 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10931 "bison-chapel.cpp"
    break;

  case 715: /* reduce_scan_op_expr: TBXOR  */
#line 2506 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10937 "bison-chapel.cpp"
    break;


#line 10941 "bison-chapel.cpp"

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
