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
  YYSYMBOL_TFORWARDING = 42,               /* TFORWARDING  */
  YYSYMBOL_TIF = 43,                       /* TIF  */
  YYSYMBOL_TIMAG = 44,                     /* TIMAG  */
  YYSYMBOL_TIMPORT = 45,                   /* TIMPORT  */
  YYSYMBOL_TIN = 46,                       /* TIN  */
  YYSYMBOL_TINCLUDE = 47,                  /* TINCLUDE  */
  YYSYMBOL_TINDEX = 48,                    /* TINDEX  */
  YYSYMBOL_TINLINE = 49,                   /* TINLINE  */
  YYSYMBOL_TINOUT = 50,                    /* TINOUT  */
  YYSYMBOL_TINT = 51,                      /* TINT  */
  YYSYMBOL_TITER = 52,                     /* TITER  */
  YYSYMBOL_TINITEQUALS = 53,               /* TINITEQUALS  */
  YYSYMBOL_TIMPLEMENTS = 54,               /* TIMPLEMENTS  */
  YYSYMBOL_TINTERFACE = 55,                /* TINTERFACE  */
  YYSYMBOL_TLABEL = 56,                    /* TLABEL  */
  YYSYMBOL_TLAMBDA = 57,                   /* TLAMBDA  */
  YYSYMBOL_TLET = 58,                      /* TLET  */
  YYSYMBOL_TLIFETIME = 59,                 /* TLIFETIME  */
  YYSYMBOL_TLOCAL = 60,                    /* TLOCAL  */
  YYSYMBOL_TLOCALE = 61,                   /* TLOCALE  */
  YYSYMBOL_TMINUSMINUS = 62,               /* TMINUSMINUS  */
  YYSYMBOL_TMODULE = 63,                   /* TMODULE  */
  YYSYMBOL_TNEW = 64,                      /* TNEW  */
  YYSYMBOL_TNIL = 65,                      /* TNIL  */
  YYSYMBOL_TNOINIT = 66,                   /* TNOINIT  */
  YYSYMBOL_TNONE = 67,                     /* TNONE  */
  YYSYMBOL_TNOTHING = 68,                  /* TNOTHING  */
  YYSYMBOL_TON = 69,                       /* TON  */
  YYSYMBOL_TONLY = 70,                     /* TONLY  */
  YYSYMBOL_TOPERATOR = 71,                 /* TOPERATOR  */
  YYSYMBOL_TOTHERWISE = 72,                /* TOTHERWISE  */
  YYSYMBOL_TOUT = 73,                      /* TOUT  */
  YYSYMBOL_TOVERRIDE = 74,                 /* TOVERRIDE  */
  YYSYMBOL_TOWNED = 75,                    /* TOWNED  */
  YYSYMBOL_TPARAM = 76,                    /* TPARAM  */
  YYSYMBOL_TPLUSPLUS = 77,                 /* TPLUSPLUS  */
  YYSYMBOL_TPRAGMA = 78,                   /* TPRAGMA  */
  YYSYMBOL_TPRIMITIVE = 79,                /* TPRIMITIVE  */
  YYSYMBOL_TPRIVATE = 80,                  /* TPRIVATE  */
  YYSYMBOL_TPROC = 81,                     /* TPROC  */
  YYSYMBOL_TPROTOTYPE = 82,                /* TPROTOTYPE  */
  YYSYMBOL_TPUBLIC = 83,                   /* TPUBLIC  */
  YYSYMBOL_TREAL = 84,                     /* TREAL  */
  YYSYMBOL_TRECORD = 85,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 86,                   /* TREDUCE  */
  YYSYMBOL_TREF = 87,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 88,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 89,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 90,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 91,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 92,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 93,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 94,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 95,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 96,                   /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 97,                /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 98,                     /* TSYNC  */
  YYSYMBOL_TTHEN = 99,                     /* TTHEN  */
  YYSYMBOL_TTHIS = 100,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 101,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 102,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 103,                    /* TTRUE  */
  YYSYMBOL_TTRY = 104,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 105,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 106,                    /* TTYPE  */
  YYSYMBOL_TUINT = 107,                    /* TUINT  */
  YYSYMBOL_TUNDERSCORE = 108,              /* TUNDERSCORE  */
  YYSYMBOL_TUNION = 109,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 110,               /* TUNMANAGED  */
  YYSYMBOL_TUSE = 111,                     /* TUSE  */
  YYSYMBOL_TVAR = 112,                     /* TVAR  */
  YYSYMBOL_TVOID = 113,                    /* TVOID  */
  YYSYMBOL_TWHEN = 114,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 115,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 116,                   /* TWHILE  */
  YYSYMBOL_TWITH = 117,                    /* TWITH  */
  YYSYMBOL_TYIELD = 118,                   /* TYIELD  */
  YYSYMBOL_TZIP = 119,                     /* TZIP  */
  YYSYMBOL_TALIAS = 120,                   /* TALIAS  */
  YYSYMBOL_TAND = 121,                     /* TAND  */
  YYSYMBOL_TASSIGN = 122,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 123,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 124,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 125,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 126,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 127,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 128,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 129,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 130,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 131,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 132,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 133,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 134,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 135,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 136,                /* TASSIGNSR  */
  YYSYMBOL_TBANG = 137,                    /* TBANG  */
  YYSYMBOL_TBAND = 138,                    /* TBAND  */
  YYSYMBOL_TBNOT = 139,                    /* TBNOT  */
  YYSYMBOL_TBOR = 140,                     /* TBOR  */
  YYSYMBOL_TBXOR = 141,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 142,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 143,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 144,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 145,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 146,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 147,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 148,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 149,                     /* TEXP  */
  YYSYMBOL_TGREATER = 150,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 151,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 152,                    /* THASH  */
  YYSYMBOL_TIO = 153,                      /* TIO  */
  YYSYMBOL_TLESS = 154,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 155,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 156,                   /* TMINUS  */
  YYSYMBOL_TMOD = 157,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 158,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 159,                      /* TOR  */
  YYSYMBOL_TPLUS = 160,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 161,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 162,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 163,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 164,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 165,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 166,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 167,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 168,                    /* TRCBR  */
  YYSYMBOL_TLP = 169,                      /* TLP  */
  YYSYMBOL_TRP = 170,                      /* TRP  */
  YYSYMBOL_TLSBR = 171,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 172,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 173,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 174,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 175,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 176,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 177,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 178,                 /* $accept  */
  YYSYMBOL_program = 179,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 180,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 181,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 182,                /* pragma_ls  */
  YYSYMBOL_stmt = 183,                     /* stmt  */
  YYSYMBOL_module_decl_start = 184,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 185,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 186,           /* access_control  */
  YYSYMBOL_opt_prototype = 187,            /* opt_prototype  */
  YYSYMBOL_include_access_control = 188,   /* include_access_control  */
  YYSYMBOL_include_module_stmt = 189,      /* include_module_stmt  */
  YYSYMBOL_190_1 = 190,                    /* $@1  */
  YYSYMBOL_block_stmt = 191,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 192,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 193,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 194,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 195,              /* opt_only_ls  */
  YYSYMBOL_except_ls = 196,                /* except_ls  */
  YYSYMBOL_use_access_control = 197,       /* use_access_control  */
  YYSYMBOL_use_stmt = 198,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 199,              /* import_stmt  */
  YYSYMBOL_import_expr = 200,              /* import_expr  */
  YYSYMBOL_import_ls = 201,                /* import_ls  */
  YYSYMBOL_require_stmt = 202,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 203,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 204,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 205,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 206,                /* ident_def  */
  YYSYMBOL_ident_use = 207,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 208,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 209,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 210,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 211,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 212,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 213,         /* class_level_stmt  */
  YYSYMBOL_214_2 = 214,                    /* @2  */
  YYSYMBOL_private_decl = 215,             /* private_decl  */
  YYSYMBOL_forwarding_stmt = 216,          /* forwarding_stmt  */
  YYSYMBOL_extern_export_decl_stmt = 217,  /* extern_export_decl_stmt  */
  YYSYMBOL_218_3 = 218,                    /* $@3  */
  YYSYMBOL_219_4 = 219,                    /* $@4  */
  YYSYMBOL_220_5 = 220,                    /* $@5  */
  YYSYMBOL_221_6 = 221,                    /* $@6  */
  YYSYMBOL_extern_block_stmt = 222,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 223,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 224,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 225,                  /* if_stmt  */
  YYSYMBOL_ifvar = 226,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 227,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 228,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 229,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 230,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 231, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 232,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 233,           /* ifc_constraint  */
  YYSYMBOL_defer_stmt = 234,               /* defer_stmt  */
  YYSYMBOL_try_stmt = 235,                 /* try_stmt  */
  YYSYMBOL_catch_stmt_ls = 236,            /* catch_stmt_ls  */
  YYSYMBOL_catch_stmt = 237,               /* catch_stmt  */
  YYSYMBOL_catch_expr = 238,               /* catch_expr  */
  YYSYMBOL_throw_stmt = 239,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 240,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 241,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 242,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 243,          /* class_decl_stmt  */
  YYSYMBOL_class_tag = 244,                /* class_tag  */
  YYSYMBOL_opt_inherit = 245,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 246,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 247,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 248,              /* enum_header  */
  YYSYMBOL_enum_ls = 249,                  /* enum_ls  */
  YYSYMBOL_enum_item = 250,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 251,         /* lambda_decl_expr  */
  YYSYMBOL_252_7 = 252,                    /* $@7  */
  YYSYMBOL_253_8 = 253,                    /* $@8  */
  YYSYMBOL_linkage_spec = 254,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt = 255,             /* fn_decl_stmt  */
  YYSYMBOL_256_9 = 256,                    /* $@9  */
  YYSYMBOL_257_10 = 257,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 258,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_receiver_expr = 259,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 260,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 261,           /* assignop_ident  */
  YYSYMBOL_opt_formal_ls = 262,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 263,            /* req_formal_ls  */
  YYSYMBOL_formal_ls = 264,                /* formal_ls  */
  YYSYMBOL_formal = 265,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 266,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 267,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 268,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 269,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 270,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 271,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 272,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 273,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 274,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 275,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 276,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 277, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 278,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 279,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 280,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_inner = 281, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 282,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 283,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 284,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 285,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 286,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 287, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 288, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 289,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 290,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 291,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 292,                 /* opt_type  */
  YYSYMBOL_array_type = 293,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 294, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 295,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 296,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 297,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 298,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 299,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 300,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 301,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 302,                /* actual_ls  */
  YYSYMBOL_actual_expr = 303,              /* actual_expr  */
  YYSYMBOL_ident_expr = 304,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 305,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 306,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 307,                 /* for_expr  */
  YYSYMBOL_cond_expr = 308,                /* cond_expr  */
  YYSYMBOL_nil_expr = 309,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 310,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 311,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 312,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 313,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 314,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 315,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 316,              /* intent_expr  */
  YYSYMBOL_shadow_var_prefix = 317,        /* shadow_var_prefix  */
  YYSYMBOL_io_expr = 318,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 319,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 320,                 /* new_expr  */
  YYSYMBOL_let_expr = 321,                 /* let_expr  */
  YYSYMBOL_expr = 322,                     /* expr  */
  YYSYMBOL_opt_expr = 323,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 324,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 325,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 326,           /* call_base_expr  */
  YYSYMBOL_call_expr = 327,                /* call_expr  */
  YYSYMBOL_dot_expr = 328,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 329,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 330,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 331,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 332,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 333,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 334,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 335,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 336,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 337,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 338       /* reduce_scan_op_expr  */
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

#line 513 "bison-chapel.cpp"

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
#define YYLAST   19526

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  178
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  161
/* YYNRULES -- Number of rules.  */
#define YYNRULES  684
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1225

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   432


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
     175,   176,   177
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   511,   511,   516,   517,   523,   524,   529,   530,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   572,   585,   590,   595,   603,   604,   605,
     609,   610,   614,   615,   616,   621,   620,   641,   642,   643,
     648,   649,   654,   659,   664,   668,   677,   682,   687,   692,
     696,   700,   708,   713,   717,   722,   726,   727,   728,   732,
     733,   734,   735,   736,   737,   738,   742,   747,   748,   749,
     753,   754,   758,   762,   764,   766,   768,   770,   772,   779,
     780,   784,   785,   786,   787,   788,   789,   792,   793,   794,
     795,   796,   797,   809,   810,   821,   822,   823,   824,   825,
     826,   827,   828,   829,   830,   831,   832,   833,   834,   835,
     836,   837,   838,   839,   843,   844,   845,   846,   847,   848,
     849,   850,   851,   852,   853,   854,   861,   862,   863,   864,
     868,   869,   873,   874,   878,   879,   880,   890,   890,   895,
     896,   897,   898,   899,   900,   901,   905,   906,   907,   908,
     913,   912,   928,   927,   944,   943,   959,   958,   974,   978,
     983,   991,  1002,  1009,  1010,  1011,  1012,  1013,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1033,  1039,  1045,  1051,  1058,  1065,  1069,  1076,
    1080,  1081,  1082,  1083,  1085,  1086,  1087,  1088,  1090,  1092,
    1094,  1096,  1101,  1102,  1106,  1108,  1116,  1117,  1122,  1127,
    1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,
    1138,  1139,  1146,  1147,  1148,  1149,  1158,  1159,  1163,  1165,
    1168,  1174,  1176,  1179,  1185,  1188,  1189,  1190,  1191,  1192,
    1193,  1197,  1198,  1202,  1203,  1204,  1208,  1209,  1213,  1216,
    1218,  1223,  1224,  1228,  1230,  1232,  1239,  1249,  1263,  1268,
    1273,  1281,  1282,  1287,  1288,  1290,  1295,  1311,  1318,  1327,
    1335,  1339,  1346,  1347,  1352,  1357,  1351,  1384,  1387,  1391,
    1399,  1409,  1398,  1448,  1452,  1457,  1461,  1466,  1473,  1474,
    1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,
    1488,  1489,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1501,  1502,  1503,  1504,  1508,  1509,  1510,
    1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1523,
    1524,  1528,  1532,  1533,  1534,  1538,  1540,  1542,  1544,  1546,
    1548,  1553,  1554,  1558,  1559,  1560,  1561,  1562,  1563,  1564,
    1565,  1566,  1570,  1571,  1572,  1573,  1574,  1575,  1579,  1580,
    1581,  1585,  1586,  1587,  1588,  1589,  1590,  1594,  1595,  1598,
    1599,  1603,  1604,  1608,  1612,  1613,  1614,  1622,  1623,  1625,
    1627,  1629,  1634,  1636,  1641,  1642,  1643,  1644,  1645,  1646,
    1647,  1651,  1653,  1658,  1660,  1662,  1667,  1680,  1697,  1698,
    1700,  1705,  1706,  1707,  1708,  1709,  1713,  1719,  1727,  1728,
    1736,  1738,  1743,  1745,  1747,  1752,  1754,  1756,  1763,  1764,
    1765,  1770,  1772,  1774,  1778,  1782,  1784,  1788,  1796,  1797,
    1798,  1799,  1800,  1805,  1806,  1807,  1808,  1809,  1829,  1833,
    1837,  1845,  1852,  1853,  1854,  1858,  1860,  1866,  1868,  1870,
    1875,  1876,  1877,  1878,  1879,  1885,  1886,  1887,  1888,  1892,
    1893,  1897,  1898,  1899,  1903,  1904,  1908,  1909,  1913,  1914,
    1918,  1919,  1920,  1921,  1925,  1926,  1937,  1939,  1941,  1947,
    1948,  1949,  1950,  1951,  1952,  1954,  1956,  1958,  1960,  1962,
    1964,  1967,  1969,  1971,  1973,  1975,  1977,  1979,  1981,  1984,
    1986,  1991,  1993,  1995,  1997,  1999,  2001,  2003,  2005,  2007,
    2009,  2011,  2013,  2015,  2022,  2028,  2034,  2040,  2049,  2059,
    2067,  2068,  2069,  2070,  2071,  2072,  2073,  2074,  2079,  2080,
    2084,  2088,  2089,  2093,  2097,  2098,  2102,  2106,  2110,  2117,
    2118,  2119,  2120,  2121,  2122,  2126,  2127,  2132,  2134,  2138,
    2142,  2146,  2154,  2159,  2165,  2171,  2178,  2188,  2196,  2197,
    2198,  2199,  2200,  2201,  2202,  2203,  2204,  2205,  2207,  2209,
    2211,  2226,  2228,  2230,  2232,  2237,  2238,  2242,  2243,  2244,
    2248,  2249,  2250,  2251,  2260,  2261,  2262,  2263,  2264,  2268,
    2269,  2270,  2274,  2275,  2276,  2277,  2278,  2286,  2287,  2288,
    2289,  2293,  2294,  2298,  2299,  2303,  2304,  2305,  2306,  2307,
    2308,  2309,  2310,  2312,  2314,  2315,  2316,  2320,  2328,  2329,
    2333,  2334,  2335,  2336,  2337,  2338,  2339,  2340,  2341,  2342,
    2343,  2344,  2345,  2346,  2347,  2348,  2349,  2350,  2351,  2352,
    2353,  2354,  2355,  2360,  2361,  2362,  2363,  2364,  2365,  2366,
    2370,  2371,  2372,  2373,  2377,  2378,  2379,  2380,  2385,  2386,
    2387,  2388,  2389,  2390,  2391
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
  "TCONFIG", "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDMAPPED", "TDO",
  "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE",
  "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN",
  "TINCLUDE", "TINDEX", "TINLINE", "TINOUT", "TINT", "TITER",
  "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLAMBDA", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW",
  "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY", "TOPERATOR",
  "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS",
  "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC",
  "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN",
  "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING",
  "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE",
  "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION",
  "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS",
  "TUMINUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype",
  "include_access_control", "include_module_stmt", "$@1", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls", "except_ls",
  "use_access_control", "use_stmt", "import_stmt", "import_expr",
  "import_ls", "require_stmt", "assignment_stmt", "opt_label_ident",
  "ident_fn_def", "ident_def", "ident_use", "internal_type_ident_def",
  "scalar_type", "reserved_type_ident_use", "do_stmt", "return_stmt",
  "class_level_stmt", "@2", "private_decl", "forwarding_stmt",
  "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@7", "$@8", "linkage_spec",
  "fn_decl_stmt", "$@9", "$@10", "fn_decl_stmt_inner",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
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
     425,   426,   427,   428,   429,   430,   431,   432
};
#endif

#define YYPACT_NINF (-1027)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-635)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1027,   106,  3344, -1027,   -52,   117, -1027, -1027, -1027, -1027,
   -1027, -1027,  4720,    98,   213,   256, 14547,   271, 19106,    98,
   11226,   272,   303,   252,   213,  4720, 11226,  4720,   215, 19159,
   11396,  8150,   286,  8320,  9518,  6782,  8490,   359, -1027,   253,
   -1027,   418, 19212, 19212, 19212, -1027, 14901,  9688,   448, 11226,
     171, -1027,   471,   484, 11226, -1027, 14547, -1027, 11226,   545,
     385,   207, 19414,   506, 19265, -1027,  9860,  7638, 11226,  9688,
   14547, 11226,   466,   510,   400,  4720,   516, 11226,   535, 11566,
   11566, 19212,   538, -1027, 14547, -1027,   558, 11226, 11226, -1027,
   11226, -1027, 11226, -1027, -1027, 14074, 11226, -1027, 11226, -1027,
   -1027, -1027,  3688,  6954,  8662, 11226, -1027,  4548, -1027,   451,
   -1027,   537, -1027, -1027,    37, -1027, -1027, -1027, -1027, -1027,
   -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027,
     569, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, 19212,
   -1027, 19212,   357,   206, -1027, -1027, 14901, -1027,   476, -1027,
     467, -1027, -1027,   478,   492,   493, 11226,   495,   496, 18815,
    1625,   235,   498,   500, -1027, -1027,   390, -1027, -1027, -1027,
   -1027, -1027,   183, -1027, -1027, 18815,   462,  4720, -1027, -1027,
     508, 11226, -1027, -1027, 11226, 11226, 11226, 19212, -1027, 11226,
    9860,  9860,   614,   410, -1027, -1027, -1027, -1027,   297,   416,
   -1027, -1027,   504, 16202, 19212, 14901, -1027,   512, -1027,   -36,
   18815,   557,  7810,   594, 19318, 18815,   953,   605, -1027, 19371,
   19212,   953, 19212,   522,     1, 15880,    13, 15804,   226, -1027,
   15963, 19212, 19212,   -18, 15090,   469,  7810, -1027, -1027, -1027,
   -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027,
   -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027,
   -1027,   523, -1027,   421,  4720,   525,  3067,    70,    51, -1027,
    4720, -1027, -1027, 16278,   699, -1027,   529,   532, -1027, 16278,
     297,   699, -1027,  7810,   892, -1027, -1027, 10030, -1027, -1027,
   -1027, -1027,   143, 18815, 11226, 11226, -1027, 18815,   540, 16749,
   -1027, 16278,   297, 18815,   534,  7810, -1027, 18815, 16789, -1027,
   -1027, 16829,  2856, -1027, -1027, 16945,   582,   543,   297, 16278,
   16985,   666,   666,  1867,   699,   699,   -57, -1027, -1027,  3860,
     166, -1027, 11226, -1027,    92,   149, -1027,   112,    88, 17031,
     -49,  1867,   698, -1027,  4032, -1027,   645, 11226, 11226, 19212,
     572,   549, -1027, -1027, -1027, -1027,   307,   437, -1027, 11226,
     576, 11226, 11226, 11226,  9518,  9518, 11226,   444, 11226, 11226,
   11226, 11226, 11226,   511, 14074, 11226, 11226, 11226, 11226, 11226,
   11226, 11226, 11226, 11226, 11226, 11226, 11226, 11226, 11226, 11226,
   11226,   665, -1027, -1027, -1027, -1027, -1027,  8832,  8832, -1027,
   -1027, -1027, -1027,  8832, -1027, -1027,  8832,  8832,  7810,  7810,
    9518,  9518,  7468, -1027, -1027, 16358, 16434, 17134,   564,    93,
   19212,  4204, -1027,  9518,     1,   573,   317, -1027, 11226, -1027,
   11226,   612, -1027,   568,   603, -1027, -1027, -1027, 19212, -1027,
   14901, -1027, 19212,   578, -1027, 14901,   702,  9860, -1027,  4892,
    9518, -1027,   581, -1027,     1,  5064,  9518, -1027,     1, 11736,
   11226, -1027,   627,   632,  4720,   723,  4720, -1027,   724, 11226,
   -1027, -1027,   537,   591,  7810, 19212, -1027, -1027,   161, -1027,
   -1027,  3067, -1027,   620,   595, -1027, 11906,   648, 11226, 14901,
   -1027, -1027, 11226, 11226, -1027,   601, -1027,  9860, -1027, 18815,
   18815, -1027,    40, -1027,  7810,   604, -1027,   752, -1027,   752,
   -1027, 12076,   633, -1027, -1027, -1027, -1027, -1027, -1027,  9004,
   -1027, 17180,  7126, -1027,  7298, -1027,  4720,   609,  9518,  9176,
    3516,   610, 11226, 10200, -1027, -1027,   305, -1027,  4376, 19212,
   -1027,   331, 17225,   332, 16046,   309,  9860,   616, 19053,   236,
   -1027, 17329, 18699, 18699,   387, -1027,   387, -1027,   387, 18895,
    1128,   735,   689,   297,   666, -1027,   608, -1027, -1027, -1027,
   -1027, -1027,  1867,  2034,   387,  1252,  1252, 18699,  1252,  1252,
    1350,   666,  2034,  1085,  1350,   699,   699,   666,  1867,   626,
     631,   635,   638,   640,   644,   629,   642, -1027,   387, -1027,
     387,    83, -1027, -1027, -1027,   152, -1027,  2518, 18855,   419,
   12246,  9518, 12416,  9518, 11226,  7810,  9518, 14817,   643,    98,
   17374, -1027, -1027, -1027, 18815, 17414,  7810, -1027,  7810, 19212,
     572,   341, 19212,   572, -1027,   353, 11226,   212,  8490, 18815,
      20, 16514,  7468, -1027,  8490, 18815,    29, 16122, -1027,   657,
     677,   661, 17530,   677,   667, 11226, 11226,   794,  4720,   798,
   17570,  4720, 16474,   774, -1027,   238, -1027,   244, -1027,   220,
   -1027, -1027, -1027, -1027, -1027, -1027,   637,   250, -1027, 14954,
   -1027,   334,   669,  3067,    70,    -4,     6, 11226, 11226,  6612,
   -1027, -1027,   424,  7980, -1027, 18815, -1027, 17610, 17686, -1027,
   -1027, 18815,   672,    36,   675, -1027,  3170, -1027, -1027,   361,
   19212, -1027, -1027, -1027, -1027, -1027, -1027, -1027,  4720,   125,
   16590, -1027, -1027, 18815,  4720, 18815, -1027, 17767, -1027, -1027,
   -1027, 11226, -1027,    63,    76, 11226, -1027, 10370, 11736, 11226,
   -1027,  7810,   706,  1524,   674,   728,   173, -1027,   764, -1027,
   -1027, -1027, -1027, 13909,   685, -1027, -1027, -1027, -1027, -1027,
   -1027, -1027, -1027, -1027, -1027,  7468, -1027, -1027, -1027, -1027,
   -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027,    16,  9518,
    9518, 11226,   826, 17843, 11226,   828, 17923,   258,   691, 17883,
    7810,     1,     1, -1027, -1027, -1027, -1027,   572,   700, -1027,
     572,   707, -1027, 16278, -1027, 15166,  5236, -1027,  5408, -1027,
     259, -1027, 15248,  5580, -1027,     1,  5752, -1027,     1, 11226,
   -1027, 11226, -1027, 18815, 18815,  4720, -1027,  4720, 11226, -1027,
    4720,   832, 19212,   716, 19212,   504, -1027, -1027, 19212,   966,
   -1027,  3067,   727,   796, -1027, -1027, -1027,    42, -1027, -1027,
     648,   713,   107, -1027, -1027,   717,   718, -1027,  5924,  9860,
   -1027, -1027, -1027, 19212, -1027,   748,   504, -1027, -1027,  6096,
     719,  6268,   721, -1027, 11226, -1027, -1027, 11226, 17999,    66,
   16673,   726,   733,   269,   729,  1542, -1027, 11226, 19212, -1027,
   -1027,   334,   732,   350, -1027,   757, -1027,   758,   761,   770,
     767,   775, -1027,   776,   772,   779,   780,   783,   411,   778,
     786,   787, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027,
   -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027,
   -1027, -1027, -1027, -1027, 11226, -1027,   801,   808,   788,   732,
     732, -1027, -1027, -1027,   648,   281,   282, 18039, 12586, 12756,
   18079, 12926, 13096, -1027, 13266, 13436,   293, -1027, -1027,   768,
   -1027,   784, -1027, -1027,  4720,  8490, 18815,  8490, 18815,  7468,
   -1027,  4720,  8490, 18815, -1027,  8490, 18815, -1027, 18155, 18815,
   -1027, -1027, 18815,   912,  4720,   785, -1027, -1027, -1027,   727,
   -1027,   791, 10542,   270, -1027,    28, -1027, -1027,  9518, 14682,
    7810,  7810,  4720, -1027,    -1,   792, 11226, -1027,  8490, -1027,
   18815,  4720,  8490, -1027, 18815,  4720, 18815,   262, 10712, 11736,
   11226, 11736, 11226, -1027, -1027,   802, -1027, -1027,   892, -1027,
    2249, -1027, 18815, -1027,   344,   161, -1027, 18195, -1027, 15007,
   -1027, -1027, -1027, 11226, 11226, 11226, 11226, 11226, 11226, 11226,
   11226, -1027, -1027,  1894, -1027,  2439, 17570, 15324, 15406, -1027,
   17570, 15482, 15564, 11226,  4720, -1027, -1027,   270,   727,  9348,
   -1027, -1027, -1027,   -19,  9860, -1027, -1027,   138, 11226,    30,
   18235, -1027,   574,   795,   799,   557, -1027,   504, 18815, 15640,
   -1027, 15722, -1027, -1027, -1027, 18815,   810,   811,   812,   813,
   -1027, -1027, -1027, 13606,   848,   298, -1027,   817,   819,   732,
     732, 18311, 18351, 18427, 18467, 18543, 18583,  2773, -1027,  3034,
   -1027,  4720,  4720,  4720,  4720, 18815, -1027, -1027, -1027,   270,
   10884,   116, -1027, 18815, -1027,    95, -1027,   185, -1027,   423,
   18659, -1027, -1027, -1027, 13436,   797,   820, -1027,  4720,  4720,
   -1027, -1027, -1027, -1027,  6440, -1027, -1027,   570, -1027,    28,
   -1027, -1027, -1027, 11226, 11226, 11226, 11226, 11226, 11226, -1027,
   -1027, 17570, 17570, 17570, 17570, -1027, -1027, -1027, -1027, -1027,
     129,  9518, 14243, -1027, 11226,   138,    95,    95,    95,    95,
      95,    95,   138,   746, -1027, -1027, 17570, 17570,   806, 13776,
     118,   -27, 18739, -1027, -1027, 18815, -1027, -1027, -1027, -1027,
   -1027, -1027, -1027,   840, -1027, -1027,   580, 14412, -1027, -1027,
   -1027, 11056, -1027,   597, -1027
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   113,   627,   628,   629,   623,
     624,   630,     0,   548,    99,   134,   517,   141,   519,   548,
       0,   140,     0,   422,    99,     0,     0,     0,   246,   135,
     595,   595,   621,     0,     0,     0,     0,   139,    55,   247,
     298,   136,     0,     0,     0,   294,     0,     0,   143,     0,
     567,   539,   631,   144,     0,   299,   511,   421,     0,     0,
       0,   157,   297,   138,   520,   423,     0,     0,     0,     0,
     515,     0,     0,   142,     0,     0,   114,     0,   622,     0,
       0,     0,   137,   280,   513,   425,   145,     0,     0,   680,
       0,   682,     0,   683,   684,   594,     0,   681,   678,   498,
     154,   679,     0,     0,     0,     0,     4,     0,     5,     0,
       9,    50,    10,    11,     0,    12,    13,    14,    16,   494,
     495,    26,    15,   155,   164,   165,    17,    21,    18,    20,
       0,   241,    19,   586,    23,    24,    25,    22,   163,     0,
     161,     0,   583,     0,   159,   162,     0,   160,   600,   579,
     496,   580,   501,   499,     0,     0,     0,   584,   585,     0,
     500,     0,   601,   602,   603,   625,   626,   578,   503,   502,
     581,   582,     0,    42,    28,   509,     0,     0,   549,   100,
       0,     0,   519,   135,     0,     0,     0,     0,   520,     0,
       0,     0,     0,   583,   600,   499,   584,   585,   518,   500,
     601,   602,     0,   548,     0,     0,   424,     0,   254,     0,
     479,     0,   486,   623,   520,   596,   297,   623,   182,   520,
       0,   297,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,     0,     0,    52,   486,   107,   115,   127,
     121,   120,   129,   110,   119,   130,   116,   131,   108,   132,
     125,   118,   126,   124,   122,   123,   109,   111,   117,   128,
     133,     0,   112,     0,     0,     0,     0,     0,     0,   428,
       0,   151,    36,     0,   665,   571,   568,   569,   570,     0,
     512,   666,     7,   486,   297,   278,   288,   595,   279,   156,
     393,   476,     0,   475,     0,     0,   152,   599,     0,     0,
      39,     0,   516,   504,     0,   486,    40,   510,     0,   261,
     257,     0,   500,   261,   258,     0,   418,     0,   514,     0,
       0,   667,   669,   592,   664,   663,     0,    57,    60,     0,
       0,   481,     0,   483,     0,     0,   482,     0,     0,   475,
       0,   593,     0,     6,     0,    51,     0,     0,     0,     0,
     281,     0,   379,   380,   378,   300,     0,   497,    27,     0,
     572,     0,     0,     0,     0,     0,     0,   668,     0,     0,
       0,     0,     0,     0,   591,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   337,   344,   345,   346,   341,   343,     0,     0,   339,
     342,   340,   338,     0,   348,   347,     0,     0,   486,   486,
       0,     0,     0,    29,    30,     0,     0,     0,     0,     0,
       0,     0,    31,     0,     0,     0,     0,    32,     0,    33,
       0,   494,   492,     0,   487,   488,   493,   176,     0,   179,
       0,   172,     0,     0,   178,     0,     0,     0,   191,     0,
       0,   190,     0,   199,     0,     0,     0,   197,     0,     0,
      72,   166,     0,     0,     0,   215,     0,   337,   211,     0,
      54,    53,    50,     0,     0,     0,   225,    34,   361,   295,
     432,     0,   433,   435,     0,   457,     0,   438,     0,     0,
     150,    35,     0,     0,    37,     0,   158,     0,    92,   597,
     598,   153,     0,    38,   486,     0,   268,   259,   255,   260,
     256,     0,   416,   413,   184,    41,    59,    58,    61,     0,
     632,     0,     0,   617,     0,   619,     0,     0,     0,     0,
       0,     0,     0,     0,   636,     8,     0,    44,     0,     0,
      90,     0,    87,     0,    66,   252,     0,     0,     0,   372,
     427,   547,   660,   659,   662,   671,   670,   675,   674,   656,
     653,   654,   655,   588,   643,   113,     0,   614,   615,   114,
     613,   612,   589,   647,   658,   652,   650,   661,   651,   649,
     641,   646,   648,   657,   640,   644,   645,   642,   590,     0,
       0,     0,     0,     0,     0,     0,     0,   673,   672,   677,
     676,   559,   560,   562,   564,     0,   551,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   634,   252,   548,
     548,   187,   414,   426,   480,     0,     0,   506,     0,     0,
     281,     0,     0,   281,   415,     0,     0,     0,     0,   523,
       0,     0,     0,   200,     0,   529,     0,     0,   198,   679,
      75,     0,    62,    73,     0,     0,     0,   214,     0,   210,
       0,     0,     0,     0,   505,     0,   228,     0,   226,   366,
     363,   364,   365,   369,   370,   371,   361,     0,   353,     0,
     362,   381,     0,   436,     0,   148,   149,   147,   146,     0,
     456,   455,   579,     0,   430,   577,   429,     0,     0,   611,
     478,   477,     0,     0,     0,   507,     0,   262,   420,   579,
       0,   633,   587,   618,   484,   620,   485,   207,     0,     0,
       0,   635,   205,   533,     0,   638,   637,     0,    46,    45,
      43,     0,    86,     0,     0,     0,    79,     0,     0,    72,
     249,     0,   282,     0,     0,   292,     0,   289,   376,   373,
     374,   377,   301,     0,     0,    98,    96,    97,    95,    94,
      93,   609,   610,   561,   563,     0,   550,   134,   141,   140,
     139,   136,   143,   144,   138,   142,   137,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,   490,   491,   489,   281,     0,   181,
     281,     0,   180,     0,   209,     0,     0,   189,     0,   188,
       0,   554,     0,     0,   195,     0,     0,   193,     0,     0,
     167,     0,   168,   223,   222,     0,   217,     0,     0,   213,
       0,   219,     0,   251,     0,     0,   367,   368,     0,   361,
     351,     0,   470,   382,   385,   384,   386,     0,   434,   437,
     438,     0,     0,   439,   440,     0,     0,   270,     0,     0,
     269,   272,   508,     0,   263,   266,     0,   417,   208,     0,
       0,     0,     0,   206,     0,    91,    88,     0,    69,    68,
      67,     0,     0,     0,     0,   297,   287,     0,   290,   286,
     375,   381,   349,   101,   332,   115,   330,   121,   120,   104,
     119,   116,   335,   131,   102,   132,   118,   122,   103,   105,
     117,   133,   329,   311,   314,   312,   313,   324,   315,   328,
     320,   318,   331,   334,   319,   317,   322,   327,   316,   321,
     325,   326,   323,   333,     0,   310,     0,   106,     0,   349,
     349,   308,   616,   552,   438,   600,   600,     0,     0,     0,
       0,     0,     0,   251,     0,     0,     0,   186,   185,     0,
     283,     0,   283,   192,     0,     0,   522,     0,   521,     0,
     553,     0,     0,   528,   196,     0,   527,   194,    64,    63,
     216,   212,   538,   218,     0,     0,   248,   227,   224,   470,
     354,     0,     0,   438,   383,   397,   431,   461,     0,   634,
     486,   486,     0,   274,     0,     0,     0,   264,     0,   203,
     535,     0,     0,   201,   534,     0,   639,     0,     0,     0,
      72,     0,    72,    80,    83,   253,   277,   157,   297,   276,
     297,   284,   293,   291,     0,   361,   307,     0,   336,     0,
     303,   304,   556,     0,     0,     0,     0,     0,     0,     0,
       0,   253,   283,   297,   283,   297,   526,     0,     0,   555,
     532,     0,     0,     0,     0,   221,    56,   438,   470,     0,
     473,   472,   474,   579,   394,   357,   355,     0,     0,     0,
       0,   459,   579,     0,     0,   275,   273,     0,   267,     0,
     204,     0,   202,    89,    71,    70,     0,     0,     0,     0,
     250,   285,   452,     0,   387,     0,   309,   101,   103,   349,
     349,     0,     0,     0,     0,     0,     0,   297,   175,   297,
     171,     0,     0,     0,     0,    65,   220,   358,   356,   438,
     462,     0,   396,   395,   411,     0,   412,   399,   402,     0,
     398,   391,   392,   296,     0,   573,   574,   265,     0,     0,
      82,    85,    81,    84,     0,   451,   450,   579,   388,   397,
     350,   305,   306,     0,     0,     0,     0,     0,     0,   177,
     173,   525,   524,   531,   530,   360,   359,   464,   465,   467,
     579,     0,   634,   410,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   579,   575,   576,   537,   536,     0,   442,
       0,     0,     0,   466,   468,   401,   403,   404,   407,   408,
     409,   405,   406,   400,   447,   445,   579,   634,   389,   302,
     390,   462,   446,   579,   469
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1027, -1027, -1027,     3,  -408,  2238, -1027, -1027, -1027,   513,
   -1027, -1027, -1027,   264,   651,  -454, -1027,  -719,  -713, -1027,
   -1027, -1027,   261, -1027, -1027,    41,   960, -1027,  2185,  -183,
    -741, -1027,  -893,    -5, -1010,  -870, -1027,   -61, -1027, -1027,
   -1027, -1027, -1027, -1027, -1027, -1027,   211, -1027, -1027, -1027,
   -1027,   177,  1018, -1027, -1027, -1027, -1027, -1027,   701, -1027,
     150, -1027, -1027, -1027, -1027, -1027, -1027,  -594,  -706, -1027,
   -1027, -1027,   127,  1242, -1027, -1027, -1027,   326, -1027, -1027,
   -1027, -1027,   -22,  -157,  -892, -1027,   -17,   180,   345, -1027,
   -1027, -1027,   131, -1027, -1027,  -178,    11,  -982,  -135,  -167,
    -159,  -538, -1027,  -188, -1027,    12,   993,  -116,   552, -1027,
    -453,  -812,  -810, -1027,  -633,  -498,  -941, -1026,  -901,   -64,
   -1027,    71, -1027,  -210,  -463,   420,   653,  -437, -1027, -1027,
   -1027,  1277, -1027,   -10, -1027, -1027,  -220, -1027,  -632, -1027,
   -1027, -1027,  1632,  1781,   -12,  1017,    58,   869, -1027,  1848,
    2208, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027, -1027,
    -385
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   328,   107,   659,   109,   110,   111,   346,
     472,   112,   235,   113,   329,   650,   543,   654,   651,   114,
     115,   116,   540,   541,   117,   118,   180,   935,   267,   119,
     262,   120,   690,   272,   121,   122,   284,   123,   124,   125,
     442,   632,   438,   629,   126,   127,   788,   128,   233,   129,
     667,   668,   192,   131,   132,   133,   134,   135,   507,   707,
     866,   136,   137,   703,   861,   138,   139,   547,   885,   140,
     141,   746,   747,   193,   265,   681,   143,   144,   549,   891,
     752,   938,   939,   469,  1036,   479,   677,   678,   679,   680,
     753,   355,   847,  1159,  1219,  1143,   432,  1075,  1079,  1137,
    1138,  1139,   145,   317,   512,   146,   147,   268,   269,   483,
     484,   694,  1156,  1104,   487,   691,  1178,  1072,   993,   330,
     209,   334,   335,   433,   434,   435,   194,   149,   150,   151,
     152,   195,   154,   177,   178,   605,   454,   810,   606,   607,
     155,   156,   196,   197,   159,   221,   436,   199,   161,   200,
     201,   164,   165,   166,   167,   340,   168,   169,   170,   171,
     172
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     175,   289,   292,   407,   198,   106,   653,   458,   203,   202,
     811,   665,   937,   708,   210,  1031,   425,   485,   215,   215,
     882,   225,   227,   230,   234,   881,   473,   609,   682,   431,
     356,   270,   443,   270,   205,   273,   798,   274,   996,   801,
     338,   702,   279,   485,   280,   270,   281,  1040,  1041,   692,
    -246,   850,   806,   431,   293,   297,   299,   301,   302,   303,
    -247,   813,    67,   307,   300,   308,   565,   311,   315,  1142,
     676,   485,   318,   495,   709,   319,   320,   291,   321,   565,
     322,   464,   347,   323,   324,  1127,   325,  1077,  1067,   426,
     293,   297,   339,   341,   533,   505,   566,   488,  1134,  1070,
     431,  -453,  1019,  -471,  1179,   173,     3,   428,   858,   567,
     173,   516,  -271,   291,   333,   291,  -453,   527,   531,    67,
     310,   314,   431,   534,  -471,   298,   429,   419,  -471,   763,
     452,  -453,  1042,   943,   528,  1218,  1020,   568,  -453,   616,
     421,  1134,   497,  1078,   360,   944,   452,  1175,   348,   421,
     859,  -471,   787,   998,  -271,   407,  1204,  -453,   486,  -453,
    1101,   336,  1181,   569,   616,   212,   421,  1129,   421,   175,
     764,  -229,   415,   416,   417,   236,   569,   307,   293,   339,
     421,  1076,   570,  1031,   486,  1031,  -453,   421,   669,   275,
     571,  1142,  -453,   424,   489,  1136,   421,   421,   595,   596,
     297,   291,   291,   959,   860,   452,   961,   670,  -271,  -453,
    1155,   671,   486,  -453,   995,   176,   179,  1161,  1162,   448,
     451,   453,   457,   496,   297,   431,   431,  1135,   440,   452,
     849,   529,  -453,   445,   672,   522,   529,   673,  1136,    59,
     205,  1203,   452,   877,   224,   226,   276,  1031,   674,  1031,
     529,  -463,   -78,    23,  1053,  1128,  1055,   609,   352,   529,
     530,   529,   523,   748,   277,   617,   836,   675,   491,   410,
     -49,   297,  -463,   411,   494,   215,  -463,   353,   883,   999,
    1224,   278,   499,   500,   526,   653,   497,   354,  1182,   -49,
    1217,   431,   524,   297,   704,   765,   503,   869,   937,  -463,
    1184,  1097,    57,  1099,  -352,   498,  1096,   837,  1098,   519,
    -230,   271,   749,    65,   514,   337,   888,  1176,   -78,   525,
     521,   431,   766,   750,   631,  -236,  -235,   956,  1185,   635,
      23,  -352,   518,   271,   520,   542,   544,  1059,    85,   206,
    -244,   889,   751,   309,   313,  1102,  1117,   551,  1119,   552,
     553,   554,   556,   558,   559,   497,   560,   561,   562,   563,
     564,   843,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,    57,
     609,   628,   804,   637,   212,   297,   297,   834,   991,  1215,
      65,   297,   693,   839,   297,   297,   297,   297,   598,   600,
     608,   628,   969,  -448,   408,   819,   409,  1222,   833,   204,
     844,   620,   628,  -234,   835,    85,   624,  1074,   625,   621,
     840,   845,   236,  1017,  -558,  -557,   815,   818,   953,   970,
    1093,   676,   431,  -448,   367,   293,   628,   639,   641,  1025,
     846,   839,   373,   645,   647,   431,  -448,   652,   652,   643,
     489,  -558,  -557,   648,   660,   590,   591,   662,   291,  -448,
     489,   592,   297,  1051,   593,   594,   422,   173,  1160,   550,
    -454,   740,  -231,   728,   731,   735,   695,  1030,   741,   623,
     697,   698,   742,  -454,   489,   701,  1103,  -107,   271,   271,
     271,   271,   297,   732,   736,  -113,   489,   465,   468,   870,
     872,  1081,  -238,   799,  -419,   780,  -448,   701,   700,   411,
     297,  -448,   297,  -454,   565,   802,   720,   701,   723,  -544,
     725,   727,   867,  -419,   367,  -242,  -607,   476,  -607,   371,
     700,   566,   373,   333,   293,   333,   376,   271,  -239,  -454,
     700,   518,   439,   271,   567,  1186,  -454,   444,  -109,   470,
     876,   571,   471,   282,   283,  1073,  -114,   291,   431,  -608,
    -233,  -608,  1082,   304,  -237,   271,   653,  -454,   653,   305,
    -243,  1187,   568,  1188,  1189,   555,   557,  1190,  1191,  -607,
     336,  -607,   336,   271,   609,  -604,  -454,  -604,   421,  -245,
     475,  -454,  -232,   714,  -454,   716,   940,  1183,   639,   783,
     645,   786,   660,   297,   789,   723,  -606,   431,  -606,   791,
     792,   569,  -240,  -605,   297,  -605,   297,   570,   344,   345,
    -458,   597,   599,   349,   803,   852,   805,   676,   357,  -449,
     608,   412,   812,  -458,   619,   807,   809,   794,  -541,  -441,
    -540,   814,   817,   823,   824,  1030,   359,  1030,  1207,  1208,
    1209,  1210,  1211,  1212,   358,   148,  -443,  -545,  -546,  -449,
    -543,   640,  -542,  -458,   669,   148,  1157,   646,   420,  -441,
     414,   421,  -449,   430,   427,   303,   307,   339,   148,   437,
     148,   297,  -441,   670,   795,  -449,  -443,   671,   271,  -458,
     441,   447,   474,  1180,   478,  -441,  -458,   363,   492,  -443,
     291,   493,   501,   504,   511,   513,   535,  1193,   539,  1030,
     672,  1030,  -443,   673,   546,   342,   548,  -458,   271,   542,
     363,   373,   271,   878,   674,   880,   652,   652,   148,   297,
     363,   589,  -449,   615,   626,   622,  -458,  -449,   627,   719,
     634,  -458,  -441,   675,  -458,  1180,   628,  -441,   636,   655,
     642,   854,   364,   608,   656,   148,   365,   658,   661,  -443,
     148,   664,  1216,   683,  -443,   684,   363,   556,   598,   947,
     693,   699,   950,   706,   705,   364,   710,   754,   297,   365,
    1223,   718,   724,   743,  1180,   364,   957,   958,   755,   365,
    1083,  1084,  -460,   756,   966,  1004,   968,   757,   963,   761,
     758,   973,   759,   367,   976,  -460,   760,   978,   371,   979,
     974,   373,   790,   977,   762,   376,   982,   431,   431,   -74,
     819,   364,   782,   820,   785,   365,   367,   368,   825,   822,
     148,   371,   827,   372,   373,  -460,   367,   832,   376,   848,
     857,   371,   886,   372,   373,   862,   383,   293,   376,   497,
     887,   890,   387,   388,   389,   942,   383,  1010,   948,  1014,
     951,  -460,  1016,   954,   389,   652,   984,   960,  -460,   992,
     291,   160,   367,   368,   962,  1032,   370,   371,   986,   372,
     373,   160,  1110,   994,   376,   997,  1000,  1001,  1023,  -460,
    1006,  1011,   383,  1015,   160,  1024,   160,  1026,   387,   388,
     389,  1035,  -134,  -141,   271,   271,  -140,  -110,  -460,  -108,
     271,   271,  -139,  -460,   285,  -111,  -460,   148,    22,    23,
    -136,  -143,  1037,   148,  -144,  -138,   831,   286,  -142,    30,
     287,  -137,  -145,  1039,    35,  1052,   966,   968,  1038,   973,
     976,    40,  1010,  1014,   160,  -112,  1064,  1066,   312,   312,
    1158,  1054,  1056,  1057,  -107,  1058,  -109,   608,  1194,  1060,
    1061,  1068,  1087,  1062,  1100,  1145,    55,   289,    57,  1146,
     864,   160,  1150,  1151,  1152,  1153,   160,   288,  1214,    65,
      23,  1195,   148,  1185,   207,   663,  1080,   723,   297,   297,
     555,   597,   875,   669,  1088,   538,  1089,   148,    81,  1086,
    1091,    83,    40,  1071,    85,  1131,  1095,   652,   652,   652,
     652,   987,   670,  1005,   509,  1033,   671,  1109,  1105,   990,
     130,   838,  1034,  1220,  1201,  1213,  1206,    55,   229,    57,
     130,  1056,  1111,  1112,  1060,  1113,  1114,  1115,  1116,   672,
      65,   696,   673,   130,    59,   130,   160,   216,   796,     0,
       0,  1125,     0,   674,     0,   271,   271,   339,     0,     0,
       0,     0,  1133,     0,     0,    85,  1140,   271,     0,   468,
       0,     0,   675,     0,   148,     0,   468,     0,     0,   271,
     291,     0,   271,     0,     0,  1132,     0,     0,     0,     0,
    1200,     0,     0,   130,     0,     0,     0,     0,     0,   988,
       0,     0,   148,     0,     0,     0,     0,     0,   148,  1171,
    1172,  1173,  1174,     0,     0,     0,   363,   148,     0,   148,
     130,     0,     0,     0,     0,   130,     0,     0,     0,     0,
    1007,     0,  1014,   160,     0,     0,  1196,  1197,     0,   160,
       0,  1177,   339,     0,     0,     0,     0,     0,     0,     0,
       0,  1171,  1172,  1173,  1174,  1196,  1197,     0,     0,   363,
       0,     0,     0,     0,     0,   291,     0,     0,     0,  1202,
     723,   364,  1205,     0,     0,   365,     0,     0,     0,   148,
       0,     0,     0,   148,     0,     0,     0,     0,     0,     0,
       0,   148,     0,  1177,     0,   130,     0,     0,   160,     0,
       0,     0,     0,     0,     0,   723,   366,     0,     0,  1014,
       0,     0,     0,   160,   364,     0,     0,     0,   365,     0,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,  1177,   375,   376,   377,   378,     0,     0,   380,
     381,   382,   383,   384,   142,   386,     0,     0,   387,   388,
     389,     0,     0,     0,   142,     0,     0,     0,     0,   390,
     778,     0,     0,     0,     0,   367,     0,   142,   271,   142,
     371,     0,   372,   373,     0,     0,     0,   376,     0,   153,
       0,     0,   130,   363,     0,   383,     0,     0,   130,   153,
     160,   387,   388,   389,     0,     0,     0,     0,     0,     0,
       0,     0,   153,     0,   153,     0,     0,     0,     0,     0,
       0,   148,     0,     0,   148,     0,     0,   142,   160,     0,
       0,   468,   468,     0,   160,   468,   468,     0,     0,     0,
       0,     0,     0,   160,     0,   160,     0,     0,   364,     0,
       0,     0,   365,  1141,   142,     0,     0,   130,     0,   142,
       0,  1147,   153,   468,     0,   468,     0,     0,     0,     0,
       0,     0,   130,     0,     0,     0,     0,     0,     0,     0,
       0,   148,     0,     0,     0,     0,     0,   148,     0,   153,
       0,   363,     0,     0,   153,     0,     0,     0,     0,   367,
     368,     0,   369,   370,   371,   160,   372,   373,   374,   160,
       0,   376,     0,     0,     0,     0,   941,   160,   382,   383,
       0,     0,   386,     0,     0,   387,   388,   389,     0,   142,
       0,     0,     0,     0,     0,     0,   390,     0,     0,     0,
       0,     0,   945,   946,     0,     0,   364,     0,     0,   130,
     365,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   153,     0,     0,     0,     0,   148,
       0,   148,     0,     0,     0,  1141,   148,   130,     0,   148,
       0,     0,     0,   130,     0,     0,     0,     0,   148,     0,
     148,     0,   130,   148,   130,     0,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,     0,     0,     0,   376,
       0,     0,     0,     0,     0,     0,   142,   383,     0,     0,
       0,   148,   142,   387,   388,   389,     0,     0,     0,     0,
       0,     0,   148,     0,   148,   884,     0,   160,     0,     0,
     160,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   153,     0,     0,   130,     0,  -283,   153,   130,     0,
    -283,  -283,     0,     0,     0,     0,   130,     0,     0,  -283,
       0,  -283,  -283,     0,   285,     0,  -283,     0,    22,    23,
       0,   142,     0,  -283,     0,     0,  -283,   286,     0,    30,
     287,     0,     0,     0,    35,     0,   142,   160,     0,     0,
       0,    40,     0,   160,     0,  -283,     0,     0,  -283,     0,
    -283,     0,  -283,     0,  -283,  -283,   153,  -283,     0,  -283,
       0,  -283,     0,     0,     0,     0,    55,   148,    57,     0,
      59,   153,  1027,     0,   148,  1028,     0,   288,     0,    65,
    -283,     0,     0,  -283,   157,     0,  -283,   148,     0,     0,
       0,     0,     0,     0,   157,     0,     0,     0,    81,     0,
       0,    83,     0,     0,    85,   148,     0,   157,     0,   157,
       0,     0,     0,   142,   148,     0,     0,     0,   148,     0,
       0,     0,     0,     0,     0,   160,   130,   160,     0,   130,
       0,     0,   160,     0,     0,   160,  -283,     0,     0,     0,
       0,   142,  -283,     0,   160,     0,   160,   142,   153,   160,
       0,     0,     0,     0,   100,     0,   142,   157,   142,     0,
    1029,     0,     0,     0,     0,     0,     0,   148,     0,     0,
       0,     0,     0,     0,     0,     0,   153,   160,     0,     0,
       0,     0,   153,     0,   157,     0,   130,     0,   160,   157,
     160,   153,   130,   153,     0,     0,     0,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,     0,     0,     0,     0,     0,     0,   142,     0,
       0,     0,   142,     0,   148,   148,   148,   148,  -565,     0,
     142,     0,     0,   158,     0,     0,     0,     0,     0,     0,
       0,   406,     0,   158,  -604,     0,  -604,     0,     0,     0,
       0,   148,   148,   153,     0,     0,   158,   153,   158,   157,
       0,     0,     0,     0,     0,   153,     0,     0,     0,     0,
       0,     0,     0,     0,   130,     0,   130,     0,     0,     0,
       0,   130,     0,   160,   130,     0,     0,     0,     0,     0,
     160,     0,     0,   130,     0,   130,     0,     0,   130,     0,
     162,     0,     0,   160,     0,     0,   158,     0,     0,     0,
     162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   160,     0,   162,     0,   162,   130,     0,     0,     0,
     160,     0,     0,   158,   160,     0,     0,   130,   158,   130,
       0,     0,     0,     0,     0,     0,   157,     0,   363,     0,
     142,     0,   157,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   285,     0,     0,     0,
      22,    23,     0,   162,     0,     0,     0,     0,     0,   286,
       0,    30,   287,   160,     0,   153,    35,     0,   153,     0,
       0,     0,     0,    40,     0,     0,     0,     0,     0,     0,
     162,     0,     0,   364,     0,   162,     0,   365,   158,     0,
     142,   157,     0,     0,     0,     0,   142,     0,    55,     0,
      57,     0,    59,     0,  1027,     0,   157,  1028,     0,   288,
       0,    65,   130,     0,     0,     0,     0,     0,     0,   130,
     160,   160,   160,   160,     0,   153,     0,     0,     0,     0,
      81,   153,   130,    83,   367,   368,    85,   369,   370,   371,
       0,   372,   373,     0,     0,     0,   376,   160,   160,     0,
     130,     0,     0,   382,   383,   162,     0,   386,     0,   130,
     387,   388,   389,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   158,     0,     0,   142,     0,
     142,   158,     0,   157,     0,   142,   100,     0,   142,     0,
       0,     0,  1118,     0,     0,   363,     0,   142,     0,   142,
       0,     0,   142,     0,     0,     0,     0,     0,     0,     0,
       0,   157,   130,   153,     0,   153,     0,   157,     0,     0,
     153,     0,     0,   153,     0,     0,   157,     0,   157,     0,
     142,     0,   153,     0,   153,     0,     0,   153,     0,     0,
     158,   142,   162,   142,     0,     0,     0,     0,   162,     0,
     364,     0,     0,     0,   365,   158,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   153,     0,     0,     0,   130,
     130,   130,   130,     0,     0,     0,   153,     0,   153,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   157,     0,
       0,     0,   157,     0,     0,     0,   130,   130,     0,     0,
     157,   367,   368,     0,   369,   370,   371,   162,   372,   373,
     374,     0,     0,   376,   377,   378,     0,     0,   380,   381,
     382,   383,   162,     0,   386,     0,     0,   387,   388,   389,
       0,     0,   158,     0,     0,     0,   142,     0,   390,     0,
     163,     0,     0,   142,     0,     0,     0,     0,     0,     0,
     163,     0,     0,     0,     0,     0,   142,   261,   263,   264,
     158,     0,     0,   163,     0,   163,   158,     0,     0,     0,
     108,   153,     0,     0,   142,   158,     0,   158,   153,     0,
     174,     0,     0,   142,     0,     0,     0,   142,     0,     0,
       0,   153,     0,   208,     0,   211,   316,     0,     0,   162,
       0,   285,     0,     0,     0,    22,    23,     0,     0,   153,
       0,     0,     0,   163,   286,     0,    30,   287,   153,     0,
     157,    35,   153,   157,     0,     0,     0,   162,    40,     0,
       0,     0,     0,   162,     0,     0,   142,   158,     0,     0,
     163,   158,   162,   306,   162,   163,     0,     0,     0,   158,
       0,     0,     0,    55,   350,    57,   351,   342,     0,  1027,
       0,     0,  1028,     0,   288,     0,    65,     0,     0,     0,
     108,   153,     0,     0,     0,   343,     0,     0,     0,     0,
     157,     0,     0,     0,     0,    81,   157,     0,    83,     0,
       0,    85,     0,   142,   142,   142,   142,     0,     0,     0,
       0,     0,   418,     0,   162,     0,     0,     0,   162,     0,
       0,     0,     0,     0,     0,   163,   162,     0,     0,   316,
     142,   142,     0,     0,     0,     0,     0,     0,   153,   153,
     153,   153,     0,     0,     0,   316,     0,   446,     0,     0,
       0,   100,     0,     0,     0,   413,   462,   463,     0,     0,
       0,     0,     0,     0,     0,   153,   153,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   157,   158,
     157,     0,   158,     0,     0,   157,     0,     0,   157,     0,
       0,   482,     0,     0,     0,     0,     0,   157,     0,   157,
       0,   285,   157,     0,     0,    22,    23,     0,     0,     0,
       0,     0,   163,     0,   286,     0,    30,   287,   163,     0,
       0,    35,     0,     0,     0,     0,     0,     0,    40,     0,
     157,     0,     0,     0,     0,     0,     0,     0,     0,   158,
       0,   157,   477,   157,     0,   158,   162,     0,   490,   162,
       0,     0,     0,    55,     0,    57,     0,    59,     0,  1027,
       0,   565,  1028,     0,   288,     0,    65,     0,     0,     0,
       0,     0,     0,     0,   545,   767,     0,   163,   768,     0,
       0,     0,     0,   769,     0,    81,     0,     0,    83,     0,
       0,    85,   163,   183,     0,     0,     0,     0,     0,     0,
       0,     0,   770,     0,     0,     0,   162,   108,     0,   771,
       0,     0,   162,     0,     0,     0,     0,     0,     0,   772,
       0,     0,   108,     0,     0,     0,   773,   158,     0,   158,
       0,     0,     0,     0,   158,     0,   157,   158,     0,     0,
       0,   100,   774,   157,     0,   618,   158,  1120,   158,     0,
       0,   158,     0,     0,   775,     0,   157,     0,   569,     0,
       0,     0,     0,   630,     0,   776,     0,   633,     0,   163,
       0,   777,     0,     0,   157,     0,     0,     0,     0,   158,
       0,     0,     0,   157,     0,     0,     0,   157,     0,     0,
     158,     0,   158,     0,   162,     0,   162,   163,     0,   108,
     666,   162,     0,   163,   162,     0,   482,     0,     0,     0,
       0,     0,   163,   162,   163,   162,     0,     0,   162,     0,
       0,     0,     0,     0,     0,     0,     0,   490,     0,     0,
       0,     0,     0,   490,     0,     0,   157,     0,     0,     0,
       0,     0,   657,     0,     0,     0,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   162,     0,   162,
       0,     0,     0,     0,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   745,   163,     0,     0,     0,   163,     0,
       0,     0,     0,     0,     0,   158,   163,     0,     0,     0,
       0,     0,   158,   157,   157,   157,   157,     0,     0,     0,
       0,     0,     0,     0,   717,   158,     0,     0,   722,     0,
       0,     0,     0,     0,     0,     0,   108,     0,     0,     0,
     157,   157,     0,   158,     0,     0,     0,     0,     0,     0,
       0,     0,   158,     0,     0,   285,   158,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,   286,     0,
      30,   287,   162,     0,   797,    35,     0,   800,     0,   162,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   158,     0,    55,     0,    57,
     162,    59,     0,  1027,     0,     0,  1028,     0,   288,   162,
      65,     0,     0,   162,   842,     0,   163,     0,   482,   163,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,   865,     0,     0,     0,   316,   826,     0,     0,   829,
       0,     0,   158,   158,   158,   158,     0,     0,     0,     0,
       0,     0,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,     0,     0,   158,
     158,     0,   163,     0,     0,   100,     0,     0,   936,     0,
       0,  1169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   868,     0,     0,     0,
       0,     0,   873,     0,     0,     0,     0,     0,     0,   162,
     162,   162,   162,     0,     0,     0,     0,     0,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,     0,     0,     0,   162,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,     0,   163,   985,     0,   666,
       0,   163,   406,   989,   163,  -604,   482,  -604,     0,     0,
       0,     0,     0,   163,     0,   163,     0,     0,   163,     0,
       0,     0,     0,     0,   490,     0,   490,     0,   865,     0,
       0,   490,     0,     0,   490,     0,   285,     0,     0,     0,
      22,    23,     0,   980,     0,   981,   163,     0,   983,   286,
     237,    30,   287,   745,     0,     0,    35,   163,     0,   163,
       0,     0,     0,    40,   238,   239,     0,   240,     0,     0,
       0,     0,   241,     0,     0,     0,  1003,     0,     0,     0,
     242,     0,     0,     0,     0,     0,   243,  1009,    55,  1013,
      57,   244,    59,     0,  1027,   245,     0,  1028,   246,   288,
       0,    65,     0,     0,     0,     0,     0,     0,   247,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
      81,     0,   250,    83,     0,     0,    85,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,   253,     0,   254,     0,   255,     0,   256,     0,     0,
     257,     0,   163,   237,   258,   480,     0,   259,     0,   163,
     260,     0,     0,     0,     0,     0,     0,   238,   239,     0,
     240,     0,   163,     0,     0,   241,   100,     0,     0,     0,
       0,     0,  1170,   242,     0,     0,     0,     0,     0,   243,
     163,     0,     0,     0,   244,     0,     0,     0,   245,   163,
       0,   246,  1065,   163,   936,     0,     0,     0,     0,     0,
       0,   247,     0,     0,     0,     0,   481,   248,   249,     0,
    1085,     0,     0,     0,     0,   250,     0,     0,     0,  1090,
       0,     0,     0,  1092,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,   253,     0,   254,     0,   255,     0,
     256,     0,   163,   257,     0,     0,     0,   258,     0,     0,
     259,     0,     0,   260,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1126,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   163,
     163,   163,   163,     0,     0,     0,     0,   421,     0,   863,
       0,     0,     0,     0,    -2,     4,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,   163,   163,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -76,
       0,    38,    39,    40,     0,    41,  -297,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,  -297,     0,     0,    55,    56,
      57,    58,    59,    60,    61,  -297,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,   104,     0,     4,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,  -634,     0,
      12,    13,    14,    15,    16,  -634,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,  -634,    27,    28,
    -634,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -76,     0,    38,    39,    40,     0,    41,  -297,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,  -297,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -297,   -47,    62,
      63,    64,  -634,    65,    66,    67,  -634,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,  -634,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -634,  -634,    92,  -634,  -634,  -634,  -634,
    -634,  -634,  -634,     0,  -634,  -634,  -634,  -634,  -634,     0,
    -634,  -634,  -634,  -634,  -634,  -634,  -634,  -634,   100,  -634,
    -634,  -634,     0,   102,  -634,   103,     0,   104,     0,   326,
    -634,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -76,     0,    38,    39,    40,     0,    41,
    -297,     0,    42,    43,    44,    45,    46,     0,    47,    48,
      49,   -47,    50,    51,     0,    52,    53,    54,     0,  -297,
       0,     0,    55,    56,    57,    58,    59,    60,    61,  -297,
     -47,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -76,
      85,    86,     0,     0,    87,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,   327,   103,     0,   104,
       0,     4,   105,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,   -76,     0,    38,    39,    40,
       0,    41,  -297,     0,    42,    43,    44,    45,    46,     0,
      47,    48,    49,   -47,    50,    51,     0,    52,    53,    54,
       0,  -297,     0,     0,    55,    56,    57,    58,    59,    60,
      61,  -297,   -47,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,   517,   103,
       0,   104,     0,   536,   105,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -76,     0,    38,
      39,    40,     0,    41,  -297,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,  -297,     0,     0,    55,    56,    57,    58,
      59,    60,    61,  -297,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
     537,   103,     0,   104,     0,   326,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -76,
       0,    38,    39,    40,     0,    41,  -297,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,  -297,     0,     0,    55,    56,
      57,    58,    59,    60,    61,  -297,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,   327,   103,     0,   104,     0,     4,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -76,     0,    38,    39,    40,     0,    41,  -297,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,  -297,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -297,   -47,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,   729,   103,     0,   104,     0,     4,
     105,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -76,     0,    38,    39,    40,     0,    41,
    -297,     0,    42,    43,    44,    45,    46,     0,    47,    48,
      49,   -47,    50,    51,     0,    52,    53,    54,     0,  -297,
       0,     0,    55,    56,    57,    58,   342,    60,    61,  -297,
     -47,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -76,
      85,    86,     0,     0,    87,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,   104,
       0,     4,   105,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,   -76,     0,    38,    39,    40,
       0,    41,  -297,     0,    42,    43,    44,    45,    46,     0,
      47,    48,    49,   -47,    50,    51,     0,    52,    53,    54,
       0,  -297,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -297,   -47,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,   104,     0,     4,   105,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   638,    37,   -76,     0,    38,
      39,    40,     0,    41,  -297,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,  -297,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -297,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,   104,     0,     4,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   644,    37,   -76,
       0,    38,    39,    40,     0,    41,  -297,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,  -297,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -297,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,   104,     0,     4,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   965,
      37,   -76,     0,    38,    39,    40,     0,    41,  -297,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,  -297,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -297,   -47,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,   104,     0,     4,
     105,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   967,    37,   -76,     0,    38,    39,    40,     0,    41,
    -297,     0,    42,    43,    44,    45,    46,     0,    47,    48,
      49,   -47,    50,    51,     0,    52,    53,    54,     0,  -297,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -297,
     -47,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -76,
      85,    86,     0,     0,    87,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,   104,
       0,     4,   105,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   972,    37,   -76,     0,    38,    39,    40,
       0,    41,  -297,     0,    42,    43,    44,    45,    46,     0,
      47,    48,    49,   -47,    50,    51,     0,    52,    53,    54,
       0,  -297,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -297,   -47,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,     0,    83,
      84,   -76,    85,    86,     0,     0,    87,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,   104,     0,     4,   105,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   975,    37,   -76,     0,    38,
      39,    40,     0,    41,  -297,     0,    42,    43,    44,    45,
      46,     0,    47,    48,    49,   -47,    50,    51,     0,    52,
      53,    54,     0,  -297,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -297,   -47,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,    79,    80,    81,    82,
       0,    83,    84,   -76,    85,    86,     0,     0,    87,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,   104,     0,     4,   105,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,  1002,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -76,
       0,    38,    39,    40,     0,    41,  -297,     0,    42,    43,
      44,    45,    46,     0,    47,    48,    49,   -47,    50,    51,
       0,    52,    53,    54,     0,  -297,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -297,   -47,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,     0,    83,    84,   -76,    85,    86,     0,     0,
      87,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,     0,   103,     0,   104,     0,     4,   105,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,  1008,
      37,   -76,     0,    38,    39,    40,     0,    41,  -297,     0,
      42,    43,    44,    45,    46,     0,    47,    48,    49,   -47,
      50,    51,     0,    52,    53,    54,     0,  -297,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -297,   -47,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,     0,    83,    84,   -76,    85,    86,
       0,     0,    87,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,   104,     0,     4,
     105,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,  1012,    37,   -76,     0,    38,    39,    40,     0,    41,
    -297,     0,    42,    43,    44,    45,    46,     0,    47,    48,
      49,   -47,    50,    51,     0,    52,    53,    54,     0,  -297,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -297,
     -47,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,     0,    76,    77,
       0,    78,    79,    80,    81,    82,     0,    83,    84,   -76,
      85,    86,     0,     0,    87,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,   104,
       0,  1198,   105,     5,   290,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,  1199,   851,   105,     5,   290,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,   191,     0,     5,   105,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,   228,    23,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,    57,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,    65,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,    85,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,   191,     0,     0,   105,     5,   290,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,   294,   295,
       0,    82,   331,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,   332,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     0,     0,   105,     5,
     290,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
     294,   295,     0,    82,   331,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,   713,   191,     0,     0,
     105,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,   294,   295,     0,    82,   331,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,   715,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,   601,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,   602,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,   603,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
     604,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,   294,   295,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     296,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     0,   105,     5,   290,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,   294,   295,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,   853,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,   294,   295,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,   217,    10,
      11,   218,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   219,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,   220,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,   222,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,   223,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,   231,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,   232,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     0,   105,     5,   290,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,   223,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,   191,     0,     5,   105,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,   294,   295,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,   191,     0,     0,   105,     5,   290,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,   711,   103,     0,   191,     0,     0,   105,     5,
     290,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,   721,     0,
     105,     5,   290,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
    1130,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,   223,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   190,     0,   103,     0,   191,
       0,     5,   105,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   181,     0,     0,    15,    16,     0,    17,     0,
     182,     0,     0,    21,     0,     0,     0,     0,     0,     0,
     270,    28,     0,   183,     0,     0,     0,    32,   184,   185,
       0,   186,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,   187,     0,     0,    45,    46,     0,     0,    48,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,   188,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,    74,   189,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   102,     0,   103,     0,   191,
       0,     0,   105,     5,   290,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,   217,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   219,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,   220,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,   726,     5,   105,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   181,     0,     0,    15,    16,     0,
      17,     0,   182,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   183,     0,     0,     0,    32,
     184,   185,     0,   186,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,   187,     0,     0,    45,    46,     0,
       0,    48,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,   188,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,    74,   189,     0,
      76,     0,     0,    78,     0,     0,     0,    82,   879,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   190,     0,   103,
       0,   191,     0,     0,   105,     5,   290,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   685,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
     686,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,   687,    72,    73,    74,
     688,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,  1069,     0,     5,   105,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
    1094,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   190,
       0,   103,     0,   191,     0,     0,   105,     5,   290,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,  1069,     0,     0,   105,     5,
     290,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1050,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,   213,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   214,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   102,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   649,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   685,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,   686,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
     687,    72,    73,    74,   688,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   689,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   689,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   781,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   784,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1044,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1045,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1047,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1048,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1049,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,  1050,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,   191,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   685,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,   686,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
     687,    72,    73,    74,   688,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   190,     0,   103,     0,  1154,     0,     5,
     105,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     181,     0,     0,    15,    16,     0,    17,     0,   182,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   183,     0,     0,     0,    32,   184,   185,     0,   186,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
     187,     0,     0,    45,    46,     0,     0,    48,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,   188,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,    74,   189,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     892,     0,   893,    90,    91,    92,    93,    94,     0,     0,
       0,   894,    95,     0,     0,     0,   895,   239,   896,   897,
       0,     0,    96,     0,   898,    97,    98,    99,     0,     0,
       0,   101,   242,     0,   183,   103,     0,  1154,   899,     0,
     105,     0,     0,   900,     0,     0,     0,   245,     0,     0,
     901,     0,   902,     0,     0,     0,     0,     0,     0,     0,
     903,     0,     0,     0,     0,     0,   904,   905,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,   906,     0,     0,     0,     0,     0,     0,
       0,     0,   252,   253,     0,   907,     0,   255,     0,   908,
       0,     0,   909,     0,     0,     0,   910,     0,     0,   259,
       0,     0,   911,     0,     0,     0,     0,     0,     0,     0,
       0,   467,   392,   393,   394,   395,   396,     0,     0,   399,
     400,   401,   402,     0,   404,   405,   912,   913,   914,   915,
     916,     0,     0,   917,     0,     0,     0,   918,   919,   920,
     921,   922,   923,   924,   925,   926,   927,   928,     0,   929,
       0,     0,   930,   931,   932,   933,     0,     5,   934,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   181,     0,
       0,    15,    16,     0,    17,     0,   182,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   183,
       0,     0,     0,    32,   184,   185,     0,   186,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,   187,     0,
       0,    45,    46,     0,     0,    48,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,   188,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,   189,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,     0,    98,    99,     0,     0,     0,   101,
       0,   190,     0,   103,     0,   191,     5,   290,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   181,     0,     0,
      15,    16,     0,    17,     0,   182,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   183,     0,
       0,     0,    32,   184,   185,     0,   186,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,   187,     0,     0,
      45,    46,     0,     0,    48,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,   188,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
      74,   189,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,  -462,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,  -462,     0,     0,     0,
    -462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
     190,     0,   103,  -462,  1069,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   183,     0,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,  -444,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,  -444,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,  -444,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,  -444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    92,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,    99,  -444,     0,     0,     0,     0,  -444,
      28,   103,   183,  1154,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     5,    92,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   181,     0,     0,    15,
      16,     0,    17,     0,   182,     0,     0,    21,    99,     0,
       0,     0,     0,     0,   190,    28,   103,   183,   191,     0,
       0,    32,   184,   185,     0,   186,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,   187,     0,     0,    45,
      46,     0,     0,    48,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,   188,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,    74,
     189,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    92,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   181,     0,     0,    15,    16,     0,    17,     0,   182,
       0,     0,    21,    99,     0,     0,     0,     0,     0,   190,
      28,   103,   183,   689,     0,     0,    32,   184,   185,     0,
     186,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,   187,     0,     0,    45,    46,     0,     0,    48,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,   188,     0,   237,     0,     0,     0,     0,     0,
      70,    71,    72,    73,    74,   189,     0,    76,   238,   239,
      78,   240,     0,     0,    82,     0,   241,    84,     0,     0,
      86,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,   244,     0,     0,     0,   245,
       0,     0,   246,     0,     0,     0,    92,   237,     0,     0,
       0,     0,   247,     0,     0,     0,     0,     0,   248,   249,
       0,   238,   239,     0,   240,     0,   250,     0,     0,   241,
       0,     0,     0,     0,     0,   251,   103,   242,   191,     0,
       0,     0,     0,   243,   252,   253,     0,   254,   244,   255,
       0,   256,   245,     0,   257,   246,     0,     0,   258,     0,
    1107,   259,     0,     0,   260,   247,     0,     0,     0,   894,
       0,   248,   249,     0,   238,   239,   896,   240,     0,   250,
       0,     0,   241,     0,     0,     0,     0,     0,   251,     0,
     242,     0,     0,     0,     0,     0,   899,   252,   253,     0,
     254,   244,   255,     0,   256,   245,     0,   257,   246,     0,
     902,   258,     0,     0,   259,     0,     0,   260,   247,     0,
     266,     0,     0,     0,   904,   249,     0,     0,     0,     0,
       0,     0,   250,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,   253,   361,   254,     0,   255,     0,  1108,     0,   362,
     909,     0,     0,     0,   258,     0,     0,   259,     0,     0,
     260,   363,     0,   841,     0,     0,     0,     0,     0,   467,
     392,   393,   394,   395,   396,     0,     0,   399,   400,   401,
     402,     0,   404,   405,   912,   913,   914,   915,   916,     0,
       0,   917,     0,     0,     0,   918,   919,   920,   921,   922,
     923,   924,   925,   926,   927,   928,     0,   929,     0,     0,
     930,   931,   932,   933,     0,     0,   364,     0,   361,     0,
     365,     0,     0,     0,     0,   362,     0,     0,     0,   466,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   467,   392,   393,   394,   395,   396,     0,     0,
     399,   400,   401,   402,     0,   404,   405,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,     0,   364,   387,   388,   389,   365,   421,     0,     0,
     361,     0,     0,     0,   390,   964,     0,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,   366,   467,   392,
     393,   394,   395,   396,     0,     0,   399,   400,   401,   402,
       0,   404,   405,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,     0,     0,   387,
     388,   389,     0,   421,   364,     0,   361,     0,   365,     0,
     390,     0,     0,   362,     0,     0,     0,   971,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     467,   392,   393,   394,   395,   396,     0,     0,   399,   400,
     401,   402,     0,   404,   405,   367,   368,     0,   369,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,   378,
     379,     0,   380,   381,   382,   383,   384,   385,   386,     0,
     364,   387,   388,   389,   365,   421,     0,     0,   361,     0,
       0,     0,   390,  1121,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
       0,     0,     0,     0,     0,   366,   467,   392,   393,   394,
     395,   396,     0,     0,   399,   400,   401,   402,     0,   404,
     405,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,     0,   387,   388,   389,
       0,   421,   364,     0,   361,     0,   365,     0,   390,     0,
       0,   362,     0,     0,     0,  1122,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,   467,   392,
     393,   394,   395,   396,     0,     0,   399,   400,   401,   402,
       0,   404,   405,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,     0,   364,   387,
     388,   389,   365,   421,     0,     0,   361,     0,     0,     0,
     390,  1123,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,     0,     0,   366,   467,   392,   393,   394,   395,   396,
       0,     0,   399,   400,   401,   402,     0,   404,   405,   367,
     368,     0,   369,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,   378,   379,     0,   380,   381,   382,   383,
     384,   385,   386,     0,     0,   387,   388,   389,     0,   421,
     364,     0,   361,     0,   365,     0,   390,     0,     0,   362,
       0,     0,     0,  1124,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   467,   392,   393,   394,
     395,   396,     0,     0,   399,   400,   401,   402,     0,   404,
     405,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,   364,   387,   388,   389,
     365,   421,     0,     0,   361,     0,     0,     0,   390,  1148,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,     0,     0,
       0,   366,   467,   392,   393,   394,   395,   396,     0,     0,
     399,   400,   401,   402,     0,   404,   405,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,     0,     0,   387,   388,   389,     0,   421,   364,     0,
       0,     0,   365,     0,   390,     0,   361,     0,     0,     0,
       0,  1149,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   455,     0,     0,     0,
       0,     0,     0,   366,   467,   392,   393,   394,   395,   396,
     456,     0,   399,   400,   401,   402,     0,   404,   405,   367,
     368,     0,   369,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,   378,   379,     0,   380,   381,   382,   383,
     384,   385,   386,     0,     0,   387,   388,   389,     0,   421,
     364,     0,   361,     0,   365,     0,   390,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,   449,     0,     0,     0,     0,     0,     0,     0,
       0,   452,     0,     0,     0,   366,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,   364,   387,   388,   389,
     365,   421,     0,     0,     0,   361,     0,     0,   390,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,     0,     0,     0,     0,   459,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,   460,   380,   381,   382,   383,   384,   385,
     386,     0,     0,   387,   388,   389,     0,   421,     0,   364,
       0,     0,     0,   365,   390,     0,     0,     0,   361,   737,
       0,     0,     0,     0,     0,   362,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   363,     0,     0,
       0,     0,   738,     0,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
       0,   375,   376,   377,   378,   379,   739,   380,   381,   382,
     383,   384,   385,   386,     0,   461,   387,   388,   389,     0,
       0,     0,   364,     0,   361,     0,   365,   390,     0,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,   816,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,     0,   364,   387,
     388,   389,   365,     0,   361,     0,     0,     0,     0,     0,
     390,   362,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   363,     0,     0,     0,     0,     0,   452,
       0,     0,     0,   366,     0,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     368,     0,   369,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,   378,   379,     0,   380,   381,   382,   383,
     384,   385,   386,     0,     0,   387,   388,   389,   364,   421,
     361,     0,   365,     0,     0,     0,   390,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     270,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,   366,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
     368,     0,   369,   370,   371,     0,   372,   373,   374,     0,
     375,   376,   377,   378,   379,     0,   380,   381,   382,   383,
     384,   385,   386,     0,   364,   387,   388,   389,   365,     0,
     361,     0,     0,     0,     0,     0,   390,   362,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
     610,     0,     0,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,   611,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,   368,     0,   369,   370,
     371,     0,   372,   373,   374,     0,   375,   376,   377,   378,
     379,     0,   380,   381,   382,   383,   384,   385,   386,     0,
       0,   387,   388,   389,   364,   421,   361,     0,   365,     0,
       0,     0,   390,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   612,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
     613,     0,     0,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,   362,     0,   367,   368,     0,   369,   370,
     371,     0,   372,   373,   374,   363,   375,   376,   377,   378,
     379,     0,   380,   381,   382,   383,   384,   385,   386,     0,
     364,   387,   388,   389,   365,     0,   361,     0,     0,     0,
       0,     0,   390,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,   808,     0,     0,     0,
       0,     0,     0,     0,     0,   366,     0,     0,     0,     0,
     364,     0,     0,     0,   365,     0,     0,     0,     0,     0,
       0,   367,   368,   830,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,   366,     0,   387,   388,   389,
     364,     0,   361,     0,   365,     0,     0,     0,   390,   362,
       0,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,   363,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,   366,     0,   387,   388,   389,
       0,   421,     0,     0,     0,     0,     0,     0,   390,     0,
       0,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,   364,   387,   388,   389,
     365,   421,     0,     0,     0,   361,     0,     0,   390,     0,
       0,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,     0,     0,   452,     0,  1021,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,  1022,   380,   381,   382,   383,   384,   385,
     386,     0,     0,   387,   388,   389,     0,     0,     0,   364,
       0,   361,   871,   365,   390,     0,     0,     0,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
       0,   361,     0,     0,     0,     0,     0,     0,   362,     0,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
     363,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,     0,   364,   387,   388,   389,   365,
       0,   361,     0,     0,     0,     0,     0,   390,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,   364,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     366,     0,   387,   388,   389,   364,   502,     0,     0,   365,
       0,     0,     0,   390,     0,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     366,   506,   387,   388,   389,     0,     0,   361,     0,     0,
       0,     0,     0,   390,   362,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
       0,   508,   387,   388,   389,     0,     0,   361,     0,     0,
       0,     0,     0,   390,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   364,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,     0,   361,     0,     0,     0,     0,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,     0,     0,     0,   366,     0,     0,     0,
       0,   364,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,   510,   387,   388,
     389,     0,     0,     0,     0,     0,     0,   364,     0,   390,
       0,   365,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   361,   515,   387,   388,
     389,   532,   366,   362,     0,     0,     0,     0,     0,   390,
       0,     0,     0,     0,     0,   363,     0,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,     0,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   361,     0,   387,   388,   389,     0,     0,   362,
       0,     0,     0,     0,     0,   390,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,     0,     0,     0,     0,
     364,     0,     0,     0,   365,     0,     0,     0,     0,     0,
       0,     0,     0,   614,     0,     0,     0,   361,   733,     0,
       0,     0,     0,     0,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   364,     0,     0,     0,
     365,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,     0,   387,   388,   389,
       0,   366,     0,     0,     0,     0,     0,     0,   390,     0,
       0,   364,     0,     0,     0,   365,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   361,     0,   387,   388,   389,   366,     0,   362,     0,
     712,     0,     0,     0,   390,     0,     0,     0,     0,     0,
     363,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     734,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   361,     0,   387,   388,
     389,     0,     0,   362,     0,     0,     0,     0,     0,   390,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   364,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,   361,     0,     0,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
     366,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     364,     0,     0,     0,   365,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
     378,   379,  -566,   380,   381,   382,   383,   384,   385,   386,
       0,   176,   387,   388,   389,   366,     0,     0,     0,     0,
     364,     0,     0,   390,   365,     0,     0,     0,     0,     0,
       0,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,     0,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,   366,     0,   387,   388,   389,
       0,     0,   361,   821,     0,     0,     0,     0,   390,   362,
       0,   367,   368,     0,   369,   370,   371,     0,   372,   373,
     374,   363,   375,   376,   377,   378,   379,     0,   380,   381,
     382,   383,   384,   385,   386,     0,   793,   387,   388,   389,
       0,     0,   361,     0,     0,     0,     0,     0,   390,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,   828,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   364,     0,     0,     0,
     365,     0,   361,     0,     0,     0,     0,     0,     0,   362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,   364,     0,     0,     0,
     365,     0,     0,     0,     0,     0,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   366,     0,   387,   388,   389,   364,     0,   361,     0,
     365,     0,     0,     0,   390,   362,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,   363,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,   366,     0,   387,   388,   389,     0,     0,     0,     0,
       0,     0,     0,     0,   390,     0,     0,   367,   368,     0,
     369,   370,   371,     0,   372,   373,   374,     0,   375,   376,
     377,   378,   379,     0,   380,   381,   382,   383,   384,   385,
     386,     0,   364,   387,   388,   389,   365,     0,     0,   361,
     855,     0,     0,     0,   390,     0,   362,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   363,     0,
       0,     0,     0,     0,     0,     0,     0,   366,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   367,   368,     0,   369,   370,   371,     0,
     372,   373,   374,     0,   375,   376,   377,   378,   379,     0,
     380,   381,   382,   383,   384,   385,   386,     0,     0,   387,
     388,   389,     0,   364,     0,   361,   856,   365,     0,     0,
     390,     0,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   949,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   874,   366,     0,
       0,     0,     0,     0,     0,   361,     0,     0,     0,     0,
       0,     0,   362,     0,   367,   368,     0,   369,   370,   371,
       0,   372,   373,   374,   363,   375,   376,   377,   378,   379,
       0,   380,   381,   382,   383,   384,   385,   386,     0,   364,
     387,   388,   389,   365,     0,   361,     0,     0,     0,     0,
       0,   390,   362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   952,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,   364,
       0,     0,     0,   365,     0,     0,     0,     0,     0,     0,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
       0,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   366,     0,   387,   388,   389,   364,
       0,   361,  1018,   365,     0,     0,     0,   390,   362,     0,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
     363,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   366,     0,   387,   388,   389,     0,
       0,   361,     0,     0,     0,   955,     0,   390,   362,     0,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
     363,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,     0,   364,   387,   388,   389,   365,
       0,   361,     0,     0,     0,     0,     0,   390,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,   364,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,   367,   368,  1043,   369,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     366,     0,   387,   388,   389,   364,     0,   361,  1063,   365,
       0,     0,     0,   390,   362,     0,   367,   368,  1046,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     366,     0,   387,   388,   389,     0,     0,   361,     0,     0,
       0,     0,     0,   390,   362,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
       0,   364,   387,   388,   389,   365,     0,   361,     0,     0,
       0,     0,     0,   390,   362,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,   364,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,     0,   387,   388,
     389,   364,     0,   361,     0,   365,     0,     0,     0,   390,
     362,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,   363,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,     0,   387,   388,
     389,     0,     0,   361,     0,  1106,     0,     0,     0,   390,
     362,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,   363,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,     0,   364,   387,   388,
     389,   365,     0,     0,     0,     0,     0,  1144,     0,   390,
    1163,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   366,     0,     0,     0,     0,   364,     0,   361,
       0,   365,     0,     0,     0,     0,   362,     0,   367,   368,
    1164,   369,   370,   371,     0,   372,   373,   374,   363,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,   366,     0,   387,   388,   389,     0,     0,   361,
       0,     0,     0,     0,     0,   390,   362,     0,   367,   368,
       0,   369,   370,   371,     0,   372,   373,   374,   363,   375,
     376,   377,   378,   379,     0,   380,   381,   382,   383,   384,
     385,   386,     0,   364,   387,   388,   389,   365,     0,     0,
       0,     0,     0,     0,     0,   390,  1165,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,     0,   364,     0,   361,     0,   365,     0,     0,
       0,     0,   362,     0,   367,   368,  1166,   369,   370,   371,
       0,   372,   373,   374,   363,   375,   376,   377,   378,   379,
       0,   380,   381,   382,   383,   384,   385,   386,   366,     0,
     387,   388,   389,     0,     0,   361,     0,     0,     0,     0,
       0,   390,   362,     0,   367,   368,     0,   369,   370,   371,
       0,   372,   373,   374,   363,   375,   376,   377,   378,   379,
       0,   380,   381,   382,   383,   384,   385,   386,     0,   364,
     387,   388,   389,   365,     0,     0,     0,     0,     0,     0,
       0,   390,  1167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,   364,
       0,   361,     0,   365,     0,     0,     0,     0,   362,     0,
     367,   368,  1168,   369,   370,   371,     0,   372,   373,   374,
     363,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,   366,     0,   387,   388,   389,     0,
       0,     0,     0,     0,     0,     0,     0,   390,  1192,     0,
     367,   368,     0,   369,   370,   371,     0,   372,   373,   374,
     363,   375,   376,   377,   378,   379,     0,   380,   381,   382,
     383,   384,   385,   386,     0,   364,   387,   388,   389,   365,
       0,   361,     0,     0,     0,     0,     0,   390,   362,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     363,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     366,     0,     0,     0,     0,   364,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,     0,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
     366,     0,   387,   388,   389,   364,     0,   361,     0,   365,
       0,     0,     0,   390,   362,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,     0,     0,   380,   381,   382,   383,   384,   385,   386,
     366,     0,   387,   388,   389,     0,     0,   361,     0,     0,
       0,     0,     0,   390,   362,     0,   367,   368,     0,   369,
     370,   371,     0,   372,   373,   374,   363,   375,   376,   377,
     378,   379,     0,   380,   381,   382,   383,   384,   385,   386,
       0,   364,   387,   388,   389,   365,     0,     0,     0,     0,
       0,  1221,     0,   390,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   366,     0,     0,     0,
       0,   779,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,   366,     0,   387,   388,
     389,   364,     0,     0,     0,   365,     0,     0,     0,   390,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,   379,     0,   380,
     381,   382,   383,   384,   385,   386,     0,     0,   387,   388,
     389,     0,     0,     0,     0,     0,     0,     0,     0,   390,
       0,     0,   367,   368,     0,   369,   370,   371,     0,   372,
     373,   374,     0,   375,   376,   377,   378,     0,     0,   380,
     381,   382,   383,   384,   744,   386,   237,     0,   387,   388,
     389,     0,     0,     0,     0,     0,     0,     0,     0,   390,
     238,   239,     0,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,   243,     0,     0,     0,     0,   244,     0,     0,
       0,   245,     0,     0,   246,     0,     0,     0,     0,  -278,
       0,     0,     0,     0,   247,     0,     0,     0,     0,     0,
     248,   249,     0,  -278,  -278,     0,  -278,     0,   250,     0,
       0,  -278,     0,     0,     0,     0,     0,   251,     0,  -278,
       0,     0,     0,     0,     0,  -278,   252,   253,     0,   254,
    -278,   255,     0,   256,  -278,     0,   257,  -278,     0,     0,
     258,     0,  -288,   259,     0,     0,   260,  -278,     0,     0,
       0,     0,     0,  -278,  -278,     0,  -288,  -288,     0,  -288,
       0,  -278,     0,     0,  -288,     0,     0,     0,     0,     0,
    -278,     0,  -288,     0,     0,     0,     0,     0,  -288,  -278,
    -278,     0,  -278,  -288,  -278,     0,  -278,  -288,     0,  -278,
    -288,     0,     0,  -278,     0,   237,  -278,     0,     0,  -278,
    -288,     0,     0,     0,     0,     0,  -288,  -288,     0,   238,
     239,     0,   240,     0,  -288,     0,     0,   241,     0,     0,
       0,     0,     0,  -288,     0,   242,     0,     0,     0,     0,
       0,   243,  -288,  -288,     0,  -288,   244,  -288,     0,  -288,
     245,     0,  -288,   246,     0,     0,  -288,     0,  -279,  -288,
       0,     0,  -288,   247,     0,     0,     0,     0,     0,   248,
     249,     0,  -279,  -279,     0,  -279,     0,   250,     0,     0,
    -279,     0,     0,     0,     0,     0,   251,     0,  -279,     0,
       0,     0,     0,     0,  -279,   252,   253,     0,   254,  -279,
     255,     0,   256,  -279,     0,   257,  -279,     0,     0,   258,
       0,  -174,   259,     0,     0,   260,  -279,     0,     0,     0,
       0,     0,  -279,  -279,     0,  -174,  -174,     0,  -174,     0,
    -279,     0,     0,  -174,     0,     0,     0,     0,     0,  -279,
       0,  -174,     0,     0,     0,     0,     0,  -174,  -279,  -279,
       0,  -279,  -174,  -279,     0,  -279,  -174,     0,  -279,  -174,
       0,     0,  -279,     0,  -170,  -279,     0,     0,  -279,  -174,
       0,     0,     0,     0,     0,  -174,  -174,     0,  -170,  -170,
       0,  -170,     0,  -174,     0,     0,  -170,     0,     0,     0,
       0,     0,  -174,     0,  -170,     0,     0,     0,     0,     0,
    -170,  -174,  -174,     0,  -174,  -170,  -174,     0,  -174,  -170,
       0,  -174,  -170,     0,     0,  -174,     0,     0,  -174,     0,
       0,  -174,  -170,     0,     0,     0,   285,     0,  -170,  -170,
      22,    23,     0,     0,     0,     0,  -170,     0,     0,   286,
       0,    30,   287,     0,     0,  -170,    35,     0,     0,   -77,
       0,     0,     0,    40,  -170,  -170,     0,  -170,     0,  -170,
       0,  -170,     0,     0,  -170,     0,     0,   -48,  -170,     0,
       0,  -170,     0,     0,  -170,     0,     0,     0,    55,     0,
      57,     0,     0,     0,     0,     0,   -48,     0,     0,   288,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      81,     0,     0,    83,     0,   -77,    85
};

static const yytype_int16 yycheck[] =
{
      12,    62,    66,   160,    16,     2,   460,   227,    20,    19,
     642,   474,   753,   511,    26,   885,   204,     1,    30,    31,
     739,    33,    34,    35,    36,   738,   236,   412,   481,   212,
     146,    32,   220,    32,    22,    47,   630,    49,   850,   633,
     104,     1,    54,     1,    56,    32,    58,   939,   940,   486,
      54,   684,    32,   236,    66,    67,    68,    69,    70,    71,
      54,    32,    89,    75,    69,    77,     3,    79,    80,  1079,
     478,     1,    84,   283,   511,    87,    88,    66,    90,     3,
      92,    99,    45,    95,    96,  1067,    98,    59,   989,   205,
     102,   103,   104,   105,   143,   305,    20,    46,     3,   992,
     283,    59,    36,   122,  1130,   162,     0,   143,    72,    33,
     162,   168,    72,   102,   103,   104,    46,   337,   338,    89,
      79,    80,   305,   172,   143,    67,   162,   191,   147,    46,
     117,    89,   944,   765,    46,   162,    70,    61,   122,    46,
     167,     3,   143,   115,   156,   778,   117,  1129,   111,   167,
     114,   170,   615,    46,   114,   312,  1182,   115,   142,   143,
    1030,   103,    46,   100,    46,   169,   167,  1068,   167,   181,
      87,    54,   184,   185,   186,   169,   100,   189,   190,   191,
     167,   993,   106,  1053,   142,  1055,   170,   167,    27,    18,
     373,  1201,   122,   203,   143,   100,   167,   167,   408,   409,
     212,   190,   191,   797,   168,   117,   800,    46,   168,   167,
    1103,    50,   142,   143,   847,   117,     3,  1109,  1110,   224,
     225,   226,   227,   284,   236,   408,   409,    89,   216,   117,
     683,   143,   162,   221,    73,   143,   143,    76,   100,    78,
     228,  1182,   117,   167,    33,    34,    75,  1117,    87,  1119,
     143,   122,    45,    27,   960,  1067,   962,   642,    52,   143,
     172,   143,   170,    27,    93,   172,    46,   106,   273,    86,
      63,   283,   143,    90,   279,   287,   147,    71,   741,   172,
    1221,   110,   294,   295,   172,   739,   143,    81,   172,    82,
     172,   474,   143,   305,   504,   143,   301,   172,  1039,   170,
     115,  1020,    76,  1022,   143,   162,  1019,    87,  1021,   143,
      54,    47,    76,    87,   319,   104,   143,  1129,   111,   170,
     332,   504,   170,    87,   440,    54,    54,   790,   143,   445,
      27,   170,   329,    69,   168,   347,   348,   969,   112,    87,
      54,   168,   106,    79,    80,     1,  1052,   359,  1054,   361,
     362,   363,   364,   365,   366,   143,   368,   369,   370,   371,
     372,    27,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,    76,
     765,   143,   170,   447,   169,   397,   398,   143,   841,  1199,
      87,   403,   122,   143,   406,   407,   408,   409,   410,   411,
     412,   143,   143,    59,   169,   143,   171,  1217,   170,   106,
      76,   423,   143,    54,   170,   112,   428,   147,   430,   424,
     170,    87,   169,   877,   143,   143,   646,   647,   170,   170,
     168,   839,   615,    89,   137,   447,   143,   449,   450,   170,
     106,   143,   145,   455,   456,   628,   102,   459,   460,   454,
     143,   170,   170,   458,   466,   397,   398,   469,   447,   115,
     143,   403,   474,   170,   406,   407,   202,   162,   170,   162,
      46,   162,    54,   168,   143,   143,   488,   885,   169,   162,
     492,   493,   546,    59,   143,   497,   142,   137,   224,   225,
     226,   227,   504,   162,   162,   145,   143,   233,   234,   719,
     720,   999,    54,   162,   143,    86,   162,   519,   497,    90,
     522,   167,   524,    89,     3,   162,   528,   529,   530,   162,
     532,   533,   710,   162,   137,    54,   169,   263,   171,   142,
     519,    20,   145,   522,   546,   524,   149,   273,    54,   115,
     529,   538,   216,   279,    33,   122,   122,   221,   137,    80,
     733,   734,    83,     8,   169,   992,   145,   546,   741,   169,
      54,   171,   999,    97,    54,   301,  1020,   143,  1022,   169,
      54,   148,    61,   150,   151,   364,   365,   154,   155,   169,
     522,   171,   524,   319,   969,   169,   162,   171,   167,    54,
     169,   167,    54,   522,   170,   524,   753,  1135,   610,   611,
     612,   613,   614,   615,   616,   617,   169,   790,   171,   619,
     620,   100,    54,   169,   626,   171,   628,   106,   167,    82,
      46,   410,   411,    54,   636,   689,   638,  1035,   161,    59,
     642,   169,   644,    59,   423,   640,   641,   626,   162,    59,
     162,   646,   647,   655,   656,  1053,   153,  1055,  1186,  1187,
    1188,  1189,  1190,  1191,   162,     2,    59,   162,   162,    89,
     162,   450,   162,    89,    27,    12,  1103,   456,    54,    89,
     162,   167,   102,   116,   162,   687,   688,   689,    25,    85,
      27,   693,   102,    46,   626,   115,    89,    50,   424,   115,
      85,   169,   169,  1130,   169,   115,   122,    31,   169,   102,
     689,   169,   162,   169,   122,   162,     8,  1144,    63,  1117,
      73,  1119,   115,    76,   142,    78,   167,   143,   454,   731,
      31,   145,   458,   735,    87,   737,   738,   739,    75,   741,
      31,    66,   162,   169,   122,   162,   162,   167,   170,   528,
     162,   167,   162,   106,   170,  1182,   143,   167,    46,   122,
     169,   693,    86,   765,   122,   102,    90,    34,    34,   162,
     107,   170,  1199,   143,   167,   170,    31,   779,   780,   781,
     122,   170,   784,    21,   170,    86,   143,   169,   790,    90,
    1217,   172,   172,   167,  1221,    86,   791,   792,   162,    90,
    1000,  1001,    46,   162,   806,   859,   808,   162,   803,   170,
     162,   813,   162,   137,   816,    59,   162,   819,   142,   821,
     815,   145,   169,   818,   172,   149,   828,  1000,  1001,   162,
     143,    86,   611,   162,   613,    90,   137,   138,    34,   162,
     177,   142,    34,   144,   145,    89,   137,    63,   149,   170,
     168,   142,   168,   144,   145,   170,   157,   859,   149,   143,
     122,    87,   163,   164,   165,   170,   157,   869,    32,   871,
      32,   115,   874,   172,   165,   877,    34,   167,   122,   142,
     859,     2,   137,   138,   167,   887,   141,   142,   162,   144,
     145,    12,  1039,    87,   149,   172,   169,   169,   162,   143,
     142,   172,   157,   172,    25,   162,    27,   168,   163,   164,
     165,   169,   145,   145,   640,   641,   145,   137,   162,   137,
     646,   647,   145,   167,    22,   137,   170,   264,    26,    27,
     145,   145,   934,   270,   145,   145,   662,    35,   145,    37,
      38,   145,   145,   145,    42,   167,   948,   949,   137,   951,
     952,    49,   954,   955,    75,   137,    34,   162,    79,    80,
     102,   167,   964,   965,   137,   967,   137,   969,   161,   971,
     972,   170,   170,   975,   162,   170,    74,  1028,    76,   170,
     706,   102,   162,   162,   162,   162,   107,    85,   172,    87,
      27,   161,   329,   143,    24,   472,   998,   999,  1000,  1001,
     779,   780,   731,    27,  1006,   344,  1008,   344,   106,  1004,
    1012,   109,    49,   992,   112,  1069,  1018,  1019,  1020,  1021,
    1022,   834,    46,   863,   313,   888,    50,  1039,  1035,   839,
       2,   676,   891,  1201,  1159,  1192,  1185,    74,    35,    76,
      12,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,    73,
      87,   489,    76,    25,    78,    27,   177,    30,   628,    -1,
      -1,  1063,    -1,    87,    -1,   791,   792,  1069,    -1,    -1,
      -1,    -1,  1074,    -1,    -1,   112,  1078,   803,    -1,   805,
      -1,    -1,   106,    -1,   421,    -1,   812,    -1,    -1,   815,
    1069,    -1,   818,    -1,    -1,  1074,    -1,    -1,    -1,    -1,
    1154,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,   835,
      -1,    -1,   449,    -1,    -1,    -1,    -1,    -1,   455,  1121,
    1122,  1123,  1124,    -1,    -1,    -1,    31,   464,    -1,   466,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
     866,    -1,  1144,   264,    -1,    -1,  1148,  1149,    -1,   270,
      -1,  1130,  1154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1163,  1164,  1165,  1166,  1167,  1168,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,  1154,    -1,    -1,    -1,  1181,
    1182,    86,  1184,    -1,    -1,    90,    -1,    -1,    -1,   526,
      -1,    -1,    -1,   530,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   538,    -1,  1182,    -1,   177,    -1,    -1,   329,    -1,
      -1,    -1,    -1,    -1,    -1,  1217,   121,    -1,    -1,  1221,
      -1,    -1,    -1,   344,    86,    -1,    -1,    -1,    90,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,  1221,   148,   149,   150,   151,    -1,    -1,   154,
     155,   156,   157,   158,     2,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   174,
     607,    -1,    -1,    -1,    -1,   137,    -1,    25,  1004,    27,
     142,    -1,   144,   145,    -1,    -1,    -1,   149,    -1,     2,
      -1,    -1,   264,    31,    -1,   157,    -1,    -1,   270,    12,
     421,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,   658,    -1,    -1,   661,    -1,    -1,    75,   449,    -1,
      -1,  1057,  1058,    -1,   455,  1061,  1062,    -1,    -1,    -1,
      -1,    -1,    -1,   464,    -1,   466,    -1,    -1,    86,    -1,
      -1,    -1,    90,  1079,   102,    -1,    -1,   329,    -1,   107,
      -1,  1087,    75,  1089,    -1,  1091,    -1,    -1,    -1,    -1,
      -1,    -1,   344,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   718,    -1,    -1,    -1,    -1,    -1,   724,    -1,   102,
      -1,    31,    -1,    -1,   107,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,   526,   144,   145,   146,   530,
      -1,   149,    -1,    -1,    -1,    -1,   753,   538,   156,   157,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,
      -1,    -1,   779,   780,    -1,    -1,    86,    -1,    -1,   421,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,   806,
      -1,   808,    -1,    -1,    -1,  1201,   813,   449,    -1,   816,
      -1,    -1,    -1,   455,    -1,    -1,    -1,    -1,   825,    -1,
     827,    -1,   464,   830,   466,    -1,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,   264,   157,    -1,    -1,
      -1,   858,   270,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,   869,    -1,   871,     1,    -1,   658,    -1,    -1,
     661,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   264,    -1,    -1,   526,    -1,    22,   270,   530,    -1,
      26,    27,    -1,    -1,    -1,    -1,   538,    -1,    -1,    35,
      -1,    37,    38,    -1,    22,    -1,    42,    -1,    26,    27,
      -1,   329,    -1,    49,    -1,    -1,    52,    35,    -1,    37,
      38,    -1,    -1,    -1,    42,    -1,   344,   718,    -1,    -1,
      -1,    49,    -1,   724,    -1,    71,    -1,    -1,    74,    -1,
      76,    -1,    78,    -1,    80,    81,   329,    83,    -1,    85,
      -1,    87,    -1,    -1,    -1,    -1,    74,   964,    76,    -1,
      78,   344,    80,    -1,   971,    83,    -1,    85,    -1,    87,
     106,    -1,    -1,   109,     2,    -1,   112,   984,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,   106,    -1,
      -1,   109,    -1,    -1,   112,  1002,    -1,    25,    -1,    27,
      -1,    -1,    -1,   421,  1011,    -1,    -1,    -1,  1015,    -1,
      -1,    -1,    -1,    -1,    -1,   806,   658,   808,    -1,   661,
      -1,    -1,   813,    -1,    -1,   816,   162,    -1,    -1,    -1,
      -1,   449,   168,    -1,   825,    -1,   827,   455,   421,   830,
      -1,    -1,    -1,    -1,   162,    -1,   464,    75,   466,    -1,
     168,    -1,    -1,    -1,    -1,    -1,    -1,  1064,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   449,   858,    -1,    -1,
      -1,    -1,   455,    -1,   102,    -1,   718,    -1,   869,   107,
     871,   464,   724,   466,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,
      -1,    -1,   530,    -1,  1121,  1122,  1123,  1124,   153,    -1,
     538,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    12,   169,    -1,   171,    -1,    -1,    -1,
      -1,  1148,  1149,   526,    -1,    -1,    25,   530,    27,   177,
      -1,    -1,    -1,    -1,    -1,   538,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   806,    -1,   808,    -1,    -1,    -1,
      -1,   813,    -1,   964,   816,    -1,    -1,    -1,    -1,    -1,
     971,    -1,    -1,   825,    -1,   827,    -1,    -1,   830,    -1,
       2,    -1,    -1,   984,    -1,    -1,    75,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1002,    -1,    25,    -1,    27,   858,    -1,    -1,    -1,
    1011,    -1,    -1,   102,  1015,    -1,    -1,   869,   107,   871,
      -1,    -1,    -1,    -1,    -1,    -1,   264,    -1,    31,    -1,
     658,    -1,   270,   661,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    75,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,  1064,    -1,   658,    42,    -1,   661,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    86,    -1,   107,    -1,    90,   177,    -1,
     718,   329,    -1,    -1,    -1,    -1,   724,    -1,    74,    -1,
      76,    -1,    78,    -1,    80,    -1,   344,    83,    -1,    85,
      -1,    87,   964,    -1,    -1,    -1,    -1,    -1,    -1,   971,
    1121,  1122,  1123,  1124,    -1,   718,    -1,    -1,    -1,    -1,
     106,   724,   984,   109,   137,   138,   112,   140,   141,   142,
      -1,   144,   145,    -1,    -1,    -1,   149,  1148,  1149,    -1,
    1002,    -1,    -1,   156,   157,   177,    -1,   160,    -1,  1011,
     163,   164,   165,  1015,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   264,    -1,    -1,   806,    -1,
     808,   270,    -1,   421,    -1,   813,   162,    -1,   816,    -1,
      -1,    -1,   168,    -1,    -1,    31,    -1,   825,    -1,   827,
      -1,    -1,   830,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   449,  1064,   806,    -1,   808,    -1,   455,    -1,    -1,
     813,    -1,    -1,   816,    -1,    -1,   464,    -1,   466,    -1,
     858,    -1,   825,    -1,   827,    -1,    -1,   830,    -1,    -1,
     329,   869,   264,   871,    -1,    -1,    -1,    -1,   270,    -1,
      86,    -1,    -1,    -1,    90,   344,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   858,    -1,    -1,    -1,  1121,
    1122,  1123,  1124,    -1,    -1,    -1,   869,    -1,   871,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,
      -1,    -1,   530,    -1,    -1,    -1,  1148,  1149,    -1,    -1,
     538,   137,   138,    -1,   140,   141,   142,   329,   144,   145,
     146,    -1,    -1,   149,   150,   151,    -1,    -1,   154,   155,
     156,   157,   344,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,   421,    -1,    -1,    -1,   964,    -1,   174,    -1,
       2,    -1,    -1,   971,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   984,    42,    43,    44,
     449,    -1,    -1,    25,    -1,    27,   455,    -1,    -1,    -1,
       2,   964,    -1,    -1,  1002,   464,    -1,   466,   971,    -1,
      12,    -1,    -1,  1011,    -1,    -1,    -1,  1015,    -1,    -1,
      -1,   984,    -1,    25,    -1,    27,    81,    -1,    -1,   421,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,  1002,
      -1,    -1,    -1,    75,    35,    -1,    37,    38,  1011,    -1,
     658,    42,  1015,   661,    -1,    -1,    -1,   449,    49,    -1,
      -1,    -1,    -1,   455,    -1,    -1,  1064,   526,    -1,    -1,
     102,   530,   464,    75,   466,   107,    -1,    -1,    -1,   538,
      -1,    -1,    -1,    74,   139,    76,   141,    78,    -1,    80,
      -1,    -1,    83,    -1,    85,    -1,    87,    -1,    -1,    -1,
     102,  1064,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
     718,    -1,    -1,    -1,    -1,   106,   724,    -1,   109,    -1,
      -1,   112,    -1,  1121,  1122,  1123,  1124,    -1,    -1,    -1,
      -1,    -1,   187,    -1,   526,    -1,    -1,    -1,   530,    -1,
      -1,    -1,    -1,    -1,    -1,   177,   538,    -1,    -1,   204,
    1148,  1149,    -1,    -1,    -1,    -1,    -1,    -1,  1121,  1122,
    1123,  1124,    -1,    -1,    -1,   220,    -1,   222,    -1,    -1,
      -1,   162,    -1,    -1,    -1,   177,   231,   232,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1148,  1149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   806,   658,
     808,    -1,   661,    -1,    -1,   813,    -1,    -1,   816,    -1,
      -1,   266,    -1,    -1,    -1,    -1,    -1,   825,    -1,   827,
      -1,    22,   830,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,   264,    -1,    35,    -1,    37,    38,   270,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
     858,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   718,
      -1,   869,   264,   871,    -1,   724,   658,    -1,   270,   661,
      -1,    -1,    -1,    74,    -1,    76,    -1,    78,    -1,    80,
      -1,     3,    83,    -1,    85,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   349,    17,    -1,   329,    20,    -1,
      -1,    -1,    -1,    25,    -1,   106,    -1,    -1,   109,    -1,
      -1,   112,   344,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,   718,   329,    -1,    51,
      -1,    -1,   724,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,   344,    -1,    -1,    -1,    68,   806,    -1,   808,
      -1,    -1,    -1,    -1,   813,    -1,   964,   816,    -1,    -1,
      -1,   162,    84,   971,    -1,   420,   825,   168,   827,    -1,
      -1,   830,    -1,    -1,    96,    -1,   984,    -1,   100,    -1,
      -1,    -1,    -1,   438,    -1,   107,    -1,   442,    -1,   421,
      -1,   113,    -1,    -1,  1002,    -1,    -1,    -1,    -1,   858,
      -1,    -1,    -1,  1011,    -1,    -1,    -1,  1015,    -1,    -1,
     869,    -1,   871,    -1,   806,    -1,   808,   449,    -1,   421,
     475,   813,    -1,   455,   816,    -1,   481,    -1,    -1,    -1,
      -1,    -1,   464,   825,   466,   827,    -1,    -1,   830,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   449,    -1,    -1,
      -1,    -1,    -1,   455,    -1,    -1,  1064,    -1,    -1,    -1,
      -1,    -1,   464,    -1,    -1,    -1,   858,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   869,    -1,   871,
      -1,    -1,    -1,    -1,   539,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   548,   526,    -1,    -1,    -1,   530,    -1,
      -1,    -1,    -1,    -1,    -1,   964,   538,    -1,    -1,    -1,
      -1,    -1,   971,  1121,  1122,  1123,  1124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   526,   984,    -1,    -1,   530,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   538,    -1,    -1,    -1,
    1148,  1149,    -1,  1002,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1011,    -1,    -1,    22,  1015,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,   964,    -1,   629,    42,    -1,   632,    -1,   971,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   984,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1064,    -1,    74,    -1,    76,
    1002,    78,    -1,    80,    -1,    -1,    83,    -1,    85,  1011,
      87,    -1,    -1,  1015,   679,    -1,   658,    -1,   683,   661,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,   706,    -1,    -1,    -1,   710,   658,    -1,    -1,   661,
      -1,    -1,  1121,  1122,  1123,  1124,    -1,    -1,    -1,    -1,
      -1,    -1,  1064,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   718,    -1,    -1,  1148,
    1149,    -1,   724,    -1,    -1,   162,    -1,    -1,   753,    -1,
      -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   718,    -1,    -1,    -1,
      -1,    -1,   724,    -1,    -1,    -1,    -1,    -1,    -1,  1121,
    1122,  1123,  1124,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,    -1,    -1,    -1,  1148,  1149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   806,    -1,   808,   832,    -1,   834,
      -1,   813,   166,   838,   816,   169,   841,   171,    -1,    -1,
      -1,    -1,    -1,   825,    -1,   827,    -1,    -1,   830,    -1,
      -1,    -1,    -1,    -1,   806,    -1,   808,    -1,   863,    -1,
      -1,   813,    -1,    -1,   816,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,   825,    -1,   827,   858,    -1,   830,    35,
       3,    37,    38,   888,    -1,    -1,    42,   869,    -1,   871,
      -1,    -1,    -1,    49,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,   858,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,   869,    74,   871,
      76,    44,    78,    -1,    80,    48,    -1,    83,    51,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,
     106,    -1,    75,   109,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    96,    -1,    98,    -1,   100,    -1,    -1,
     103,    -1,   964,     3,   107,   108,    -1,   110,    -1,   971,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,   984,    -1,    -1,    25,   162,    -1,    -1,    -1,
      -1,    -1,   168,    33,    -1,    -1,    -1,    -1,    -1,    39,
    1002,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,  1011,
      -1,    51,   984,  1015,  1039,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    -1,   169,    67,    68,    -1,
    1002,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,  1011,
      -1,    -1,    -1,  1015,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    96,    -1,    98,    -1,
     100,    -1,  1064,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1064,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1121,
    1122,  1123,  1124,    -1,    -1,    -1,    -1,   167,    -1,   169,
      -1,    -1,    -1,    -1,     0,     1,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,  1148,  1149,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    71,    -1,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     1,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    -1,    -1,
      74,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,   167,   168,   169,    -1,   171,    -1,     1,
     174,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    71,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,   168,   169,    -1,   171,
      -1,     1,   174,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    71,    -1,    -1,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,   162,    -1,    -1,   165,    -1,   167,   168,   169,
      -1,   171,    -1,     1,   174,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    71,    -1,    -1,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,   167,
     168,   169,    -1,   171,    -1,     1,   174,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    71,    -1,    -1,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,
      -1,   167,   168,   169,    -1,   171,    -1,     1,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    -1,    -1,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,
      -1,   165,    -1,   167,   168,   169,    -1,   171,    -1,     1,
     174,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    71,
      -1,    -1,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,     1,   174,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,   162,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     1,   174,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    71,    -1,    -1,    74,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     1,   174,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    71,    -1,    -1,    74,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     1,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    -1,    -1,
      74,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     1,
     174,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    71,
      -1,    -1,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,     1,   174,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      60,    61,    62,    63,    64,    65,    -1,    67,    68,    69,
      -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
     100,   101,    -1,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,    -1,    -1,   116,    -1,   118,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,   162,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     1,   174,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    57,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    71,    -1,    -1,    74,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,   100,   101,    -1,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,    -1,    -1,   116,    -1,
     118,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,   162,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     1,   174,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    57,    58,    -1,    60,    61,    62,    63,    64,    65,
      -1,    67,    68,    69,    -1,    71,    -1,    -1,    74,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    85,
      -1,    87,    88,    89,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,   100,   101,    -1,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,    -1,    -1,
     116,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,   162,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,     1,   174,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    57,    58,    -1,    60,    61,    62,    63,
      64,    65,    -1,    67,    68,    69,    -1,    71,    -1,    -1,
      74,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,   100,   101,    -1,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
      -1,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,   162,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     1,
     174,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    57,    58,    -1,    60,    61,
      62,    63,    64,    65,    -1,    67,    68,    69,    -1,    71,
      -1,    -1,    74,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    -1,    87,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,   100,   101,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,     1,   174,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,   172,     1,   174,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     3,   174,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,   112,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,    -1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,   104,   105,
      -1,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
     104,   105,    -1,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,   170,   171,    -1,    -1,
     174,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,   104,   105,    -1,   107,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,    -1,   169,   170,   171,
      -1,     3,   174,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    46,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
     112,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,     3,   174,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,   104,   105,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
     162,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,    -1,   174,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,   104,   105,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    66,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,   104,   105,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,    -1,   174,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     3,   174,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,   104,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,    -1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,   168,   169,    -1,   171,    -1,    -1,   174,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,   172,    -1,
     174,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
     172,     3,   174,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,     3,   174,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,
      62,    -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,   100,    -1,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,   165,    -1,   167,    -1,   169,    -1,   171,
      -1,    -1,   174,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,   106,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,   172,     3,   174,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    57,    58,    -1,
      -1,    61,    62,    -1,    64,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    79,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
     100,    -1,    -1,   103,    -1,    -1,    -1,   107,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,   165,    -1,   167,    -1,   169,
      -1,   171,    -1,    -1,   174,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,     3,   174,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,   165,    -1,   167,
      -1,   169,    -1,   171,    -1,    -1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,   165,    -1,   167,    -1,   169,    -1,   171,    -1,     3,
     174,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,    -1,
      64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,   100,    -1,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    12,   146,    -1,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,   156,    -1,    25,   159,   160,   161,    -1,    -1,
      -1,   165,    33,    -1,    35,   169,    -1,   171,    39,    -1,
     174,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    96,    -1,    98,    -1,   100,
      -1,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,    -1,    -1,   130,
     131,   132,   133,    -1,   135,   136,   137,   138,   139,   140,
     141,    -1,    -1,   144,    -1,    -1,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,     3,   169,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    57,    58,    -1,    -1,    61,    62,    -1,    64,    65,
      -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,   139,   140,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,    -1,    -1,    -1,   160,   161,    -1,    -1,    -1,   165,
      -1,   167,    -1,   169,    -1,   171,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      57,    58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,   143,    -1,    -1,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
     167,    -1,   169,   170,   171,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    59,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,   102,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   139,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   161,   162,    -1,    -1,    -1,    -1,   167,
      33,   169,    35,   171,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    -1,    -1,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   137,     3,   139,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,   161,    -1,
      -1,    -1,    -1,    -1,   167,    33,   169,    35,   171,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      58,    -1,    -1,    61,    62,    -1,    64,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,   100,    -1,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   139,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   161,    -1,    -1,    -1,    -1,    -1,   167,
      33,   169,    35,   171,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    58,    -1,    -1,    61,    62,
      -1,    64,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    84,    85,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    98,    -1,   100,    17,    18,
     103,    20,    -1,    -1,   107,    -1,    25,   110,    -1,    -1,
     113,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,   139,     3,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    17,    18,    -1,    20,    -1,    75,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    84,   169,    33,   171,    -1,
      -1,    -1,    -1,    39,    93,    94,    -1,    96,    44,    98,
      -1,   100,    48,    -1,   103,    51,    -1,    -1,   107,    -1,
       3,   110,    -1,    -1,   113,    61,    -1,    -1,    -1,    12,
      -1,    67,    68,    -1,    17,    18,    19,    20,    -1,    75,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    93,    94,    -1,
      96,    44,    98,    -1,   100,    48,    -1,   103,    51,    -1,
      53,   107,    -1,    -1,   110,    -1,    -1,   113,    61,    -1,
     169,    -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    12,    96,    -1,    98,    -1,   100,    -1,    19,
     103,    -1,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
     113,    31,    -1,   169,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,   131,   132,
     133,    -1,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,   144,    -1,    -1,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,    86,    -1,    12,    -1,
      90,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    -1,    86,   163,   164,   165,    90,   167,    -1,    -1,
      12,    -1,    -1,    -1,   174,    99,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,   133,
      -1,   135,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,    -1,   167,    86,    -1,    12,    -1,    90,    -1,
     174,    -1,    -1,    19,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
     122,   123,   124,   125,   126,   127,    -1,    -1,   130,   131,
     132,   133,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    -1,
      86,   163,   164,   165,    90,   167,    -1,    -1,    12,    -1,
      -1,    -1,   174,    99,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,    -1,   135,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,   163,   164,   165,
      -1,   167,    86,    -1,    12,    -1,    90,    -1,   174,    -1,
      -1,    19,    -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   123,
     124,   125,   126,   127,    -1,    -1,   130,   131,   132,   133,
      -1,   135,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    -1,    86,   163,
     164,   165,    90,   167,    -1,    -1,    12,    -1,    -1,    -1,
     174,    99,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,   131,   132,   133,    -1,   135,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,   163,   164,   165,    -1,   167,
      86,    -1,    12,    -1,    90,    -1,   174,    -1,    -1,    19,
      -1,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,   122,   123,   124,   125,
     126,   127,    -1,    -1,   130,   131,   132,   133,    -1,   135,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    86,   163,   164,   165,
      90,   167,    -1,    -1,    12,    -1,    -1,    -1,   174,    99,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,   131,   132,   133,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,   163,   164,   165,    -1,   167,    86,    -1,
      -1,    -1,    90,    -1,   174,    -1,    12,    -1,    -1,    -1,
      -1,    99,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   122,   123,   124,   125,   126,   127,
      46,    -1,   130,   131,   132,   133,    -1,   135,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,   163,   164,   165,    -1,   167,
      86,    -1,    12,    -1,    90,    -1,   174,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,   121,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    86,   163,   164,   165,
      90,   167,    -1,    -1,    -1,    12,    -1,    -1,   174,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    70,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,   163,   164,   165,    -1,   167,    -1,    86,
      -1,    -1,    -1,    90,   174,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    70,   154,   155,   156,
     157,   158,   159,   160,    -1,   162,   163,   164,   165,    -1,
      -1,    -1,    86,    -1,    12,    -1,    90,   174,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    -1,    86,   163,
     164,   165,    90,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     174,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    -1,   163,   164,   165,    86,   167,
      12,    -1,    90,    -1,    -1,    -1,   174,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,    -1,    86,   163,   164,   165,    90,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   174,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    -1,
      -1,   163,   164,   165,    86,   167,    12,    -1,    90,    -1,
      -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      46,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,    31,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,    -1,
      86,   163,   164,   165,    90,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,    99,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   121,    -1,   163,   164,   165,
      86,    -1,    12,    -1,    90,    -1,    -1,    -1,   174,    19,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    31,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   121,    -1,   163,   164,   165,
      -1,   167,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    86,   163,   164,   165,
      90,   167,    -1,    -1,    -1,    12,    -1,    -1,   174,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   117,    -1,    36,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    70,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,    86,
      -1,    12,   172,    90,   174,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   174,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    86,   167,    -1,    -1,    90,
      -1,    -1,    -1,   174,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,   162,   163,   164,   165,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,   162,   163,   164,   165,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,   174,
      -1,    90,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    12,   162,   163,   164,
     165,   120,   121,    19,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    12,    -1,   163,   164,   165,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      90,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,   163,   164,   165,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,   174,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    12,    -1,   163,   164,   165,   121,    -1,    19,    -1,
     170,    -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,
      31,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    12,    -1,   163,   164,
     165,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    90,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,   117,   163,   164,   165,   121,    -1,    -1,    -1,    -1,
      86,    -1,    -1,   174,    90,    -1,    -1,    -1,    -1,    -1,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   121,    -1,   163,   164,   165,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,   174,    19,
      -1,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,    31,   148,   149,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,    -1,   162,   163,   164,   165,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   174,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      90,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   121,    -1,   163,   164,   165,    86,    -1,    12,    -1,
      90,    -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    31,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   121,    -1,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   174,    -1,    -1,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,    -1,    86,   163,   164,   165,    90,    -1,    -1,    12,
     170,    -1,    -1,    -1,   174,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,   163,
     164,   165,    -1,    86,    -1,    12,   170,    90,    -1,    -1,
     174,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    86,
     163,   164,   165,    90,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   174,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   121,    -1,   163,   164,   165,    86,
      -1,    12,    13,    90,    -1,    -1,    -1,   174,    19,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   121,    -1,   163,   164,   165,    -1,
      -1,    12,    -1,    -1,    -1,   172,    -1,   174,    19,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   174,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    99,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    86,    -1,    12,    13,    90,
      -1,    -1,    -1,   174,    19,    -1,   137,   138,    99,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    86,   163,   164,   165,    90,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    86,    -1,    12,    -1,    90,    -1,    -1,    -1,   174,
      19,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    -1,    -1,    12,    -1,   170,    -1,    -1,    -1,   174,
      19,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    86,   163,   164,
     165,    90,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    86,    -1,    12,
      -1,    90,    -1,    -1,    -1,    -1,    19,    -1,   137,   138,
      99,   140,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   121,    -1,   163,   164,   165,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   174,    19,    -1,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,    -1,    86,   163,   164,   165,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    86,    -1,    12,    -1,    90,    -1,    -1,
      -1,    -1,    19,    -1,   137,   138,    99,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   121,    -1,
     163,   164,   165,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   174,    19,    -1,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,    -1,    86,
     163,   164,   165,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    86,
      -1,    12,    -1,    90,    -1,    -1,    -1,    -1,    19,    -1,
     137,   138,    99,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   121,    -1,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,    59,    -1,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,    -1,    86,   163,   164,   165,    90,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   174,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    86,    -1,    12,    -1,    90,
      -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,    -1,    -1,   154,   155,   156,   157,   158,   159,   160,
     121,    -1,   163,   164,   165,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   174,    19,    -1,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
      -1,    86,   163,   164,   165,    90,    -1,    -1,    -1,    -1,
      -1,   172,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   121,    -1,   163,   164,
     165,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,   174,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      -1,    -1,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,   151,    -1,    -1,   154,
     155,   156,   157,   158,     1,   160,     3,    -1,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,    17,    18,    -1,    20,    -1,    75,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    84,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    93,    94,    -1,    96,
      44,    98,    -1,   100,    48,    -1,   103,    51,    -1,    -1,
     107,    -1,     3,   110,    -1,    -1,   113,    61,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,    17,    18,    -1,    20,
      -1,    75,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    93,
      94,    -1,    96,    44,    98,    -1,   100,    48,    -1,   103,
      51,    -1,    -1,   107,    -1,     3,   110,    -1,    -1,   113,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    17,
      18,    -1,    20,    -1,    75,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    93,    94,    -1,    96,    44,    98,    -1,   100,
      48,    -1,   103,    51,    -1,    -1,   107,    -1,     3,   110,
      -1,    -1,   113,    61,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,    17,    18,    -1,    20,    -1,    75,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    84,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    93,    94,    -1,    96,    44,
      98,    -1,   100,    48,    -1,   103,    51,    -1,    -1,   107,
      -1,     3,   110,    -1,    -1,   113,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,    17,    18,    -1,    20,    -1,
      75,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    93,    94,
      -1,    96,    44,    98,    -1,   100,    48,    -1,   103,    51,
      -1,    -1,   107,    -1,     3,   110,    -1,    -1,   113,    61,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    17,    18,
      -1,    20,    -1,    75,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    93,    94,    -1,    96,    44,    98,    -1,   100,    48,
      -1,   103,    51,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,   113,    61,    -1,    -1,    -1,    22,    -1,    67,    68,
      26,    27,    -1,    -1,    -1,    -1,    75,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    84,    42,    -1,    -1,    45,
      -1,    -1,    -1,    49,    93,    94,    -1,    96,    -1,    98,
      -1,   100,    -1,    -1,   103,    -1,    -1,    63,   107,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,   109,    -1,   111,   112
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   179,   180,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      49,    51,    54,    55,    56,    57,    58,    60,    61,    62,
      64,    65,    67,    68,    69,    74,    75,    76,    77,    78,
      79,    80,    83,    84,    85,    87,    88,    89,    91,    92,
      93,    94,    95,    96,    97,    98,   100,   101,   103,   104,
     105,   106,   107,   109,   110,   112,   113,   116,   118,   121,
     137,   138,   139,   140,   141,   146,   156,   159,   160,   161,
     162,   165,   167,   169,   171,   174,   181,   182,   183,   184,
     185,   186,   189,   191,   197,   198,   199,   202,   203,   207,
     209,   212,   213,   215,   216,   217,   222,   223,   225,   227,
     230,   231,   232,   233,   234,   235,   239,   240,   243,   244,
     247,   248,   251,   254,   255,   280,   283,   284,   304,   305,
     306,   307,   308,   309,   310,   318,   319,   320,   321,   322,
     325,   326,   327,   328,   329,   330,   331,   332,   334,   335,
     336,   337,   338,   162,   183,   322,   117,   311,   312,     3,
     204,    14,    22,    35,    40,    41,    43,    54,    85,    98,
     167,   171,   230,   251,   304,   309,   320,   321,   322,   325,
     327,   328,   311,   322,   106,   283,    87,   204,   183,   298,
     322,   183,   169,     8,    85,   322,   323,     8,    11,    85,
     106,   323,    76,   119,   224,   322,   224,   322,    26,   284,
     322,    27,   112,   226,   322,   190,   169,     3,    17,    18,
      20,    25,    33,    39,    44,    48,    51,    61,    67,    68,
      75,    84,    93,    94,    96,    98,   100,   103,   107,   110,
     113,   206,   208,   206,   206,   252,   169,   206,   285,   286,
      32,   191,   211,   322,   322,    18,    75,    93,   110,   322,
     322,   322,     8,   169,   214,    22,    35,    38,    85,   215,
       4,   274,   297,   322,   104,   105,   162,   322,   324,   322,
     211,   322,   322,   322,    97,   169,   183,   322,   322,   191,
     203,   322,   325,   191,   203,   322,   206,   281,   322,   322,
     322,   322,   322,   322,   322,   322,     1,   168,   181,   192,
     297,   108,   147,   274,   299,   300,   324,   224,   297,   322,
     333,   322,    78,   183,   167,    82,   187,    45,   111,    54,
     206,   206,    52,    71,    81,   269,   285,   161,   162,   153,
     322,    12,    19,    31,    86,    90,   121,   137,   138,   140,
     141,   142,   144,   145,   146,   148,   149,   150,   151,   152,
     154,   155,   156,   157,   158,   159,   160,   163,   164,   165,
     174,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   166,   261,   169,   171,
      86,    90,   169,   183,   162,   322,   322,   322,   206,   297,
      54,   167,   191,    46,   311,   281,   285,   162,   143,   162,
     116,   207,   274,   301,   302,   303,   324,    85,   220,   255,
     283,    85,   218,   281,   255,   283,   206,   169,   211,    32,
      46,   211,   117,   211,   314,    32,    46,   211,   314,    36,
      70,   162,   206,   206,    99,   191,    99,   122,   191,   261,
      80,    83,   188,   301,   169,   169,   191,   183,   169,   263,
     108,   169,   206,   287,   288,     1,   142,   292,    46,   143,
     183,   211,   169,   169,   211,   301,   215,   143,   162,   322,
     322,   162,   167,   211,   169,   301,   162,   236,   162,   236,
     162,   122,   282,   162,   211,   162,   168,   168,   181,   143,
     168,   322,   143,   170,   143,   170,   172,   314,    46,   143,
     172,   314,   120,   143,   172,     8,     1,   168,   192,    63,
     200,   201,   322,   194,   322,   206,   142,   245,   167,   256,
     162,   322,   322,   322,   322,   224,   322,   224,   322,   322,
     322,   322,   322,   322,   322,     3,    20,    33,    61,   100,
     106,   207,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,    66,
     324,   324,   324,   324,   324,   301,   301,   224,   322,   224,
     322,    27,    46,    87,   112,   313,   316,   317,   322,   338,
      32,    46,    32,    46,    99,   169,    46,   172,   206,   224,
     322,   211,   162,   162,   322,   322,   122,   170,   143,   221,
     206,   285,   219,   206,   162,   285,    46,   297,    43,   322,
     224,   322,   169,   211,    43,   322,   224,   322,   211,   165,
     193,   196,   322,   193,   195,   122,   122,   183,    34,   183,
     322,    34,   322,   187,   170,   302,   206,   228,   229,    27,
      46,    50,    73,    76,    87,   106,   182,   264,   265,   266,
     267,   253,   288,   143,   170,    33,    48,    94,    98,   171,
     210,   293,   305,   122,   289,   322,   286,   322,   322,   170,
     274,   322,     1,   241,   301,   170,    21,   237,   293,   305,
     143,   168,   170,   170,   299,   170,   299,   183,   172,   224,
     322,   172,   183,   322,   172,   322,   172,   322,   168,   168,
     206,   143,   162,    13,   145,   143,   162,    13,    36,    70,
     162,   169,   297,   167,     1,   206,   249,   250,    27,    76,
      87,   106,   258,   268,   169,   162,   162,   162,   162,   162,
     162,   170,   172,    46,    87,   143,   170,    17,    20,    25,
      44,    51,    61,    68,    84,    96,   107,   113,   304,    86,
      86,    43,   224,   322,    43,   224,   322,   302,   224,   322,
     169,   311,   311,   162,   274,   324,   303,   206,   245,   162,
     206,   245,   162,   322,   170,   322,    32,   211,    32,   211,
     315,   316,   322,    32,   211,   314,    32,   211,   314,   143,
     162,    13,   162,   322,   322,    34,   183,    34,    34,   183,
      99,   191,    63,   170,   143,   170,    46,    87,   266,   143,
     170,   169,   206,    27,    76,    87,   106,   270,   170,   288,
     292,     1,   297,    66,   324,   170,   170,   168,    72,   114,
     168,   242,   170,   169,   191,   206,   238,   281,   183,   172,
     314,   172,   314,   183,   120,   200,   207,   167,   322,   108,
     322,   196,   195,   302,     1,   246,   168,   122,   143,   168,
      87,   257,     1,     3,    12,    17,    19,    20,    25,    39,
      44,    51,    53,    61,    67,    68,    84,    96,   100,   103,
     107,   113,   137,   138,   139,   140,   141,   144,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   160,
     163,   164,   165,   166,   169,   205,   206,   208,   259,   260,
     261,   304,   170,   316,   292,   304,   304,   322,    32,    32,
     322,    32,    32,   170,   172,   172,   302,   211,   211,   245,
     167,   245,   167,   211,    99,    43,   322,    43,   322,   143,
     170,    99,    43,   322,   211,    43,   322,   211,   322,   322,
     183,   183,   322,   183,    34,   206,   162,   229,   191,   206,
     265,   288,   142,   296,    87,   292,   289,   172,    46,   172,
     169,   169,    32,   183,   297,   238,   142,   191,    43,   183,
     322,   172,    43,   183,   322,   172,   322,   193,    13,    36,
      70,    36,    70,   162,   162,   170,   168,    80,    83,   168,
     182,   213,   322,   250,   270,   169,   262,   322,   137,   145,
     262,   262,   289,    99,    43,    43,    99,    43,    43,    43,
      43,   170,   167,   246,   167,   246,   322,   322,   322,   316,
     322,   322,   322,    13,    34,   183,   162,   296,   170,   171,
     210,   274,   295,   305,   147,   275,   289,    59,   115,   276,
     322,   293,   305,   301,   301,   183,   211,   170,   322,   322,
     183,   322,   183,   168,   108,   322,   196,   195,   196,   195,
     162,   213,     1,   142,   291,   264,   170,     3,   100,   260,
     261,   322,   322,   322,   322,   322,   322,   246,   168,   246,
     168,    99,    99,    99,    99,   322,   183,   275,   289,   296,
     172,   297,   274,   322,     3,    89,   100,   277,   278,   279,
     322,   191,   212,   273,   172,   170,   170,   191,    99,    99,
     162,   162,   162,   162,   171,   210,   290,   305,   102,   271,
     170,   262,   262,    99,    99,    99,    99,    99,    99,   168,
     168,   322,   322,   322,   322,   275,   289,   274,   294,   295,
     305,    46,   172,   279,   115,   143,   122,   148,   150,   151,
     154,   155,    59,   305,   161,   161,   322,   322,     1,   172,
     297,   276,   322,   294,   295,   322,   278,   279,   279,   279,
     279,   279,   279,   277,   172,   290,   305,   172,   162,   272,
     273,   172,   290,   305,   294
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   178,   179,   180,   180,   181,   181,   182,   182,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   184,   185,   185,   185,   186,   186,   186,
     187,   187,   188,   188,   188,   190,   189,   191,   191,   191,
     192,   192,   193,   193,   193,   193,   194,   194,   194,   194,
     194,   194,   195,   195,   196,   196,   197,   197,   197,   198,
     198,   198,   198,   198,   198,   198,   199,   200,   200,   200,
     201,   201,   202,   203,   203,   203,   203,   203,   203,   204,
     204,   205,   205,   205,   205,   205,   205,   206,   206,   206,
     206,   206,   206,   207,   207,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   210,   210,   210,   210,
     211,   211,   212,   212,   213,   213,   213,   214,   213,   215,
     215,   215,   215,   215,   215,   215,   216,   216,   216,   216,
     218,   217,   219,   217,   220,   217,   221,   217,   217,   217,
     217,   217,   222,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   224,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   226,   226,   227,   227,   228,   228,   229,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   231,   231,   231,   231,   231,   231,   232,   232,
     232,   233,   233,   233,   234,   235,   235,   235,   235,   235,
     235,   236,   236,   237,   237,   237,   238,   238,   239,   240,
     240,   241,   241,   242,   242,   242,   243,   243,   244,   244,
     244,   245,   245,   246,   246,   246,   247,   247,   248,   249,
     249,   249,   250,   250,   252,   253,   251,   254,   254,   254,
     256,   257,   255,   258,   258,   258,   258,   258,   259,   259,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   262,
     262,   263,   264,   264,   264,   265,   265,   265,   265,   265,
     265,   266,   266,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   268,   268,   268,   268,   268,   268,   269,   269,
     269,   270,   270,   270,   270,   270,   270,   271,   271,   272,
     272,   273,   273,   274,   275,   275,   275,   276,   276,   276,
     276,   276,   277,   277,   278,   278,   278,   278,   278,   278,
     278,   279,   279,   280,   280,   280,   281,   281,   282,   282,
     282,   283,   283,   283,   283,   283,   284,   284,   285,   285,
     286,   286,   287,   287,   287,   288,   288,   288,   289,   289,
     289,   290,   290,   290,   290,   290,   290,   290,   291,   291,
     291,   291,   291,   292,   292,   292,   292,   292,   293,   293,
     293,   293,   294,   294,   294,   295,   295,   295,   295,   295,
     296,   296,   296,   296,   296,   297,   297,   297,   297,   298,
     298,   299,   299,   299,   300,   300,   301,   301,   302,   302,
     303,   303,   303,   303,   304,   304,   305,   305,   305,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   308,   309,
     310,   310,   310,   310,   310,   310,   310,   310,   311,   311,
     312,   313,   313,   314,   315,   315,   316,   316,   316,   317,
     317,   317,   317,   317,   317,   318,   318,   319,   319,   319,
     319,   319,   320,   320,   320,   320,   320,   321,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   323,   323,   324,   324,   324,
     325,   325,   325,   325,   326,   326,   326,   326,   326,   327,
     327,   327,   328,   328,   328,   328,   328,   329,   329,   329,
     329,   330,   330,   331,   331,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   333,   333,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   335,   335,   335,   335,   335,   335,   335,
     336,   336,   336,   336,   337,   337,   337,   337,   338,   338,
     338,   338,   338,   338,   338
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     2,
       2,     3,     2,     4,     3,     4,     4,     0,     1,     1,
       0,     1,     0,     1,     1,     0,     7,     2,     3,     3,
       1,     2,     1,     3,     3,     5,     1,     3,     3,     3,
       5,     5,     0,     1,     1,     1,     0,     1,     1,     4,
       6,     8,     8,     6,     8,     8,     4,     1,     3,     5,
       1,     3,     3,     4,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     1,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     5,     2,
       0,     8,     0,     9,     0,     8,     0,     9,     3,     3,
       5,     5,     2,     5,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     3,
       4,     6,     7,     6,     7,     4,     5,     4,     5,     4,
       4,     3,     6,     5,     4,     3,     6,     5,     6,     5,
       8,     7,     4,     4,     6,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     6,     4,
       7,     5,     3,     6,     2,     3,     3,     2,     2,     3,
       3,     0,     2,     2,     3,     5,     1,     3,     3,     5,
       5,     0,     2,     3,     2,     3,     6,     6,     1,     1,
       1,     0,     2,     0,     2,     3,     5,     5,     1,     1,
       2,     3,     1,     3,     0,     0,     8,     0,     1,     1,
       0,     0,    10,     3,     3,     5,     5,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     3,     0,     1,     3,     4,     5,     4,     5,     6,
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
#line 511 "chapel.ypp"
                                       { yyblock = (yyval.pblockstmt); }
#line 6238 "bison-chapel.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 516 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6244 "bison-chapel.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 517 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6250 "bison-chapel.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 524 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6256 "bison-chapel.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 529 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6262 "bison-chapel.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 530 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6268 "bison-chapel.cpp"
    break;

  case 27: /* stmt: stmt_level_expr TSEMI  */
#line 553 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6274 "bison-chapel.cpp"
    break;

  case 28: /* stmt: TATOMIC stmt  */
#line 554 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6280 "bison-chapel.cpp"
    break;

  case 29: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 555 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6286 "bison-chapel.cpp"
    break;

  case 30: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 556 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6292 "bison-chapel.cpp"
    break;

  case 31: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 557 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6298 "bison-chapel.cpp"
    break;

  case 32: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 558 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6304 "bison-chapel.cpp"
    break;

  case 33: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 559 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6310 "bison-chapel.cpp"
    break;

  case 34: /* stmt: TLABEL ident_def stmt  */
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6316 "bison-chapel.cpp"
    break;

  case 35: /* stmt: TLOCAL expr do_stmt  */
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6322 "bison-chapel.cpp"
    break;

  case 36: /* stmt: TLOCAL do_stmt  */
#line 562 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6328 "bison-chapel.cpp"
    break;

  case 37: /* stmt: TON expr do_stmt  */
#line 563 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6334 "bison-chapel.cpp"
    break;

  case 38: /* stmt: TSERIAL expr do_stmt  */
#line 564 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6340 "bison-chapel.cpp"
    break;

  case 39: /* stmt: TSERIAL do_stmt  */
#line 565 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6346 "bison-chapel.cpp"
    break;

  case 40: /* stmt: TSYNC stmt  */
#line 566 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6352 "bison-chapel.cpp"
    break;

  case 41: /* stmt: TYIELD expr TSEMI  */
#line 567 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6358 "bison-chapel.cpp"
    break;

  case 42: /* stmt: error TSEMI  */
#line 568 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6364 "bison-chapel.cpp"
    break;

  case 43: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 573 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6378 "bison-chapel.cpp"
    break;

  case 44: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 586 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6387 "bison-chapel.cpp"
    break;

  case 45: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 591 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6396 "bison-chapel.cpp"
    break;

  case 46: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 596 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6405 "bison-chapel.cpp"
    break;

  case 47: /* access_control: %empty  */
#line 603 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6411 "bison-chapel.cpp"
    break;

  case 48: /* access_control: TPUBLIC  */
#line 604 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6417 "bison-chapel.cpp"
    break;

  case 49: /* access_control: TPRIVATE  */
#line 605 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6423 "bison-chapel.cpp"
    break;

  case 50: /* opt_prototype: %empty  */
#line 609 "chapel.ypp"
             { (yyval.b) = false; }
#line 6429 "bison-chapel.cpp"
    break;

  case 51: /* opt_prototype: TPROTOTYPE  */
#line 610 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6435 "bison-chapel.cpp"
    break;

  case 52: /* include_access_control: %empty  */
#line 614 "chapel.ypp"
           { (yyval.b) = false; }
#line 6441 "bison-chapel.cpp"
    break;

  case 53: /* include_access_control: TPUBLIC  */
#line 615 "chapel.ypp"
           { (yyval.b) = false; }
#line 6447 "bison-chapel.cpp"
    break;

  case 54: /* include_access_control: TPRIVATE  */
#line 616 "chapel.ypp"
           { (yyval.b) = true; }
#line 6453 "bison-chapel.cpp"
    break;

  case 55: /* $@1: %empty  */
#line 621 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6462 "bison-chapel.cpp"
    break;

  case 56: /* include_module_stmt: TINCLUDE $@1 include_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 626 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6470 "bison-chapel.cpp"
    break;

  case 57: /* block_stmt: TLCBR TRCBR  */
#line 641 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6476 "bison-chapel.cpp"
    break;

  case 58: /* block_stmt: TLCBR stmt_ls TRCBR  */
#line 642 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6482 "bison-chapel.cpp"
    break;

  case 59: /* block_stmt: TLCBR error TRCBR  */
#line 643 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6488 "bison-chapel.cpp"
    break;

  case 60: /* stmt_ls: toplevel_stmt  */
#line 648 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6494 "bison-chapel.cpp"
    break;

  case 61: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 649 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6500 "bison-chapel.cpp"
    break;

  case 62: /* renames_ls: expr  */
#line 654 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6510 "bison-chapel.cpp"
    break;

  case 63: /* renames_ls: expr TAS expr  */
#line 659 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6520 "bison-chapel.cpp"
    break;

  case 64: /* renames_ls: renames_ls TCOMMA expr  */
#line 664 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6529 "bison-chapel.cpp"
    break;

  case 65: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 668 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6538 "bison-chapel.cpp"
    break;

  case 66: /* use_renames_ls: expr  */
#line 677 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6548 "bison-chapel.cpp"
    break;

  case 67: /* use_renames_ls: expr TAS expr  */
#line 682 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6558 "bison-chapel.cpp"
    break;

  case 68: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 687 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6568 "bison-chapel.cpp"
    break;

  case 69: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 692 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6577 "bison-chapel.cpp"
    break;

  case 70: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 696 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6586 "bison-chapel.cpp"
    break;

  case 71: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 700 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6595 "bison-chapel.cpp"
    break;

  case 72: /* opt_only_ls: %empty  */
#line 708 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6605 "bison-chapel.cpp"
    break;

  case 74: /* except_ls: TSTAR  */
#line 717 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6615 "bison-chapel.cpp"
    break;

  case 76: /* use_access_control: %empty  */
#line 726 "chapel.ypp"
           { (yyval.b) = true; }
#line 6621 "bison-chapel.cpp"
    break;

  case 77: /* use_access_control: TPUBLIC  */
#line 727 "chapel.ypp"
           { (yyval.b) = false; }
#line 6627 "bison-chapel.cpp"
    break;

  case 78: /* use_access_control: TPRIVATE  */
#line 728 "chapel.ypp"
           { (yyval.b) = true; }
#line 6633 "bison-chapel.cpp"
    break;

  case 79: /* use_stmt: use_access_control TUSE use_renames_ls TSEMI  */
#line 732 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6639 "bison-chapel.cpp"
    break;

  case 80: /* use_stmt: use_access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 733 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6645 "bison-chapel.cpp"
    break;

  case 81: /* use_stmt: use_access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 734 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6651 "bison-chapel.cpp"
    break;

  case 82: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 735 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6657 "bison-chapel.cpp"
    break;

  case 83: /* use_stmt: use_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 736 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6663 "bison-chapel.cpp"
    break;

  case 84: /* use_stmt: use_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 737 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6669 "bison-chapel.cpp"
    break;

  case 85: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 738 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6675 "bison-chapel.cpp"
    break;

  case 86: /* import_stmt: use_access_control TIMPORT import_ls TSEMI  */
#line 742 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 6682 "bison-chapel.cpp"
    break;

  case 87: /* import_expr: expr  */
#line 747 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 6688 "bison-chapel.cpp"
    break;

  case 88: /* import_expr: expr TAS ident_use  */
#line 748 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6694 "bison-chapel.cpp"
    break;

  case 89: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 749 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 6700 "bison-chapel.cpp"
    break;

  case 90: /* import_ls: import_expr  */
#line 753 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 6706 "bison-chapel.cpp"
    break;

  case 91: /* import_ls: import_ls TCOMMA import_expr  */
#line 754 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 6712 "bison-chapel.cpp"
    break;

  case 92: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 758 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6718 "bison-chapel.cpp"
    break;

  case 93: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 763 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6724 "bison-chapel.cpp"
    break;

  case 94: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 765 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6730 "bison-chapel.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 767 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6736 "bison-chapel.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6742 "bison-chapel.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6748 "bison-chapel.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 6754 "bison-chapel.cpp"
    break;

  case 99: /* opt_label_ident: %empty  */
#line 779 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 6760 "bison-chapel.cpp"
    break;

  case 100: /* opt_label_ident: TIDENT  */
#line 780 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 6766 "bison-chapel.cpp"
    break;

  case 101: /* ident_fn_def: TIDENT  */
#line 784 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 6772 "bison-chapel.cpp"
    break;

  case 102: /* ident_fn_def: TNONE  */
#line 785 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6778 "bison-chapel.cpp"
    break;

  case 103: /* ident_fn_def: TTHIS  */
#line 786 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 6784 "bison-chapel.cpp"
    break;

  case 104: /* ident_fn_def: TFALSE  */
#line 787 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6790 "bison-chapel.cpp"
    break;

  case 105: /* ident_fn_def: TTRUE  */
#line 788 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6796 "bison-chapel.cpp"
    break;

  case 106: /* ident_fn_def: internal_type_ident_def  */
#line 789 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6802 "bison-chapel.cpp"
    break;

  case 107: /* ident_def: TIDENT  */
#line 792 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 6808 "bison-chapel.cpp"
    break;

  case 108: /* ident_def: TNONE  */
#line 793 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6814 "bison-chapel.cpp"
    break;

  case 109: /* ident_def: TTHIS  */
#line 794 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6820 "bison-chapel.cpp"
    break;

  case 110: /* ident_def: TFALSE  */
#line 795 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6826 "bison-chapel.cpp"
    break;

  case 111: /* ident_def: TTRUE  */
#line 796 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6832 "bison-chapel.cpp"
    break;

  case 112: /* ident_def: internal_type_ident_def  */
#line 797 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6838 "bison-chapel.cpp"
    break;

  case 113: /* ident_use: TIDENT  */
#line 809 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 6844 "bison-chapel.cpp"
    break;

  case 114: /* ident_use: TTHIS  */
#line 810 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 6850 "bison-chapel.cpp"
    break;

  case 115: /* internal_type_ident_def: TBOOL  */
#line 821 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 6856 "bison-chapel.cpp"
    break;

  case 116: /* internal_type_ident_def: TINT  */
#line 822 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 6862 "bison-chapel.cpp"
    break;

  case 117: /* internal_type_ident_def: TUINT  */
#line 823 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 6868 "bison-chapel.cpp"
    break;

  case 118: /* internal_type_ident_def: TREAL  */
#line 824 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 6874 "bison-chapel.cpp"
    break;

  case 119: /* internal_type_ident_def: TIMAG  */
#line 825 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 6880 "bison-chapel.cpp"
    break;

  case 120: /* internal_type_ident_def: TCOMPLEX  */
#line 826 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 6886 "bison-chapel.cpp"
    break;

  case 121: /* internal_type_ident_def: TBYTES  */
#line 827 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 6892 "bison-chapel.cpp"
    break;

  case 122: /* internal_type_ident_def: TSTRING  */
#line 828 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 6898 "bison-chapel.cpp"
    break;

  case 123: /* internal_type_ident_def: TSYNC  */
#line 829 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 6904 "bison-chapel.cpp"
    break;

  case 124: /* internal_type_ident_def: TSINGLE  */
#line 830 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 6910 "bison-chapel.cpp"
    break;

  case 125: /* internal_type_ident_def: TOWNED  */
#line 831 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 6916 "bison-chapel.cpp"
    break;

  case 126: /* internal_type_ident_def: TSHARED  */
#line 832 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 6922 "bison-chapel.cpp"
    break;

  case 127: /* internal_type_ident_def: TBORROWED  */
#line 833 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 6928 "bison-chapel.cpp"
    break;

  case 128: /* internal_type_ident_def: TUNMANAGED  */
#line 834 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 6934 "bison-chapel.cpp"
    break;

  case 129: /* internal_type_ident_def: TDOMAIN  */
#line 835 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 6940 "bison-chapel.cpp"
    break;

  case 130: /* internal_type_ident_def: TINDEX  */
#line 836 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 6946 "bison-chapel.cpp"
    break;

  case 131: /* internal_type_ident_def: TLOCALE  */
#line 837 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 6952 "bison-chapel.cpp"
    break;

  case 132: /* internal_type_ident_def: TNOTHING  */
#line 838 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 6958 "bison-chapel.cpp"
    break;

  case 133: /* internal_type_ident_def: TVOID  */
#line 839 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 6964 "bison-chapel.cpp"
    break;

  case 134: /* scalar_type: TBOOL  */
#line 843 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6970 "bison-chapel.cpp"
    break;

  case 135: /* scalar_type: TENUM  */
#line 844 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 6976 "bison-chapel.cpp"
    break;

  case 136: /* scalar_type: TINT  */
#line 845 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6982 "bison-chapel.cpp"
    break;

  case 137: /* scalar_type: TUINT  */
#line 846 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6988 "bison-chapel.cpp"
    break;

  case 138: /* scalar_type: TREAL  */
#line 847 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6994 "bison-chapel.cpp"
    break;

  case 139: /* scalar_type: TIMAG  */
#line 848 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7000 "bison-chapel.cpp"
    break;

  case 140: /* scalar_type: TCOMPLEX  */
#line 849 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7006 "bison-chapel.cpp"
    break;

  case 141: /* scalar_type: TBYTES  */
#line 850 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7012 "bison-chapel.cpp"
    break;

  case 142: /* scalar_type: TSTRING  */
#line 851 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7018 "bison-chapel.cpp"
    break;

  case 143: /* scalar_type: TLOCALE  */
#line 852 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7024 "bison-chapel.cpp"
    break;

  case 144: /* scalar_type: TNOTHING  */
#line 853 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7030 "bison-chapel.cpp"
    break;

  case 145: /* scalar_type: TVOID  */
#line 854 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7036 "bison-chapel.cpp"
    break;

  case 146: /* reserved_type_ident_use: TSYNC  */
#line 861 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7042 "bison-chapel.cpp"
    break;

  case 147: /* reserved_type_ident_use: TSINGLE  */
#line 862 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7048 "bison-chapel.cpp"
    break;

  case 148: /* reserved_type_ident_use: TDOMAIN  */
#line 863 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7054 "bison-chapel.cpp"
    break;

  case 149: /* reserved_type_ident_use: TINDEX  */
#line 864 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7060 "bison-chapel.cpp"
    break;

  case 150: /* do_stmt: TDO stmt  */
#line 868 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7066 "bison-chapel.cpp"
    break;

  case 151: /* do_stmt: block_stmt  */
#line 869 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7072 "bison-chapel.cpp"
    break;

  case 152: /* return_stmt: TRETURN TSEMI  */
#line 873 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7078 "bison-chapel.cpp"
    break;

  case 153: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 874 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7084 "bison-chapel.cpp"
    break;

  case 154: /* class_level_stmt: TSEMI  */
#line 878 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7090 "bison-chapel.cpp"
    break;

  case 156: /* class_level_stmt: TPUBLIC private_decl  */
#line 880 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7096 "bison-chapel.cpp"
    break;

  case 157: /* @2: %empty  */
#line 890 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7102 "bison-chapel.cpp"
    break;

  case 158: /* class_level_stmt: TPRIVATE @2 private_decl  */
#line 891 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7108 "bison-chapel.cpp"
    break;

  case 166: /* forwarding_stmt: TFORWARDING expr TSEMI  */
#line 905 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7114 "bison-chapel.cpp"
    break;

  case 167: /* forwarding_stmt: TFORWARDING expr TEXCEPT except_ls TSEMI  */
#line 906 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7120 "bison-chapel.cpp"
    break;

  case 168: /* forwarding_stmt: TFORWARDING expr TONLY opt_only_ls TSEMI  */
#line 907 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7126 "bison-chapel.cpp"
    break;

  case 169: /* forwarding_stmt: TFORWARDING var_decl_stmt  */
#line 908 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7132 "bison-chapel.cpp"
    break;

  case 170: /* $@3: %empty  */
#line 913 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7141 "bison-chapel.cpp"
    break;

  case 171: /* extern_export_decl_stmt: TEXTERN TRECORD $@3 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 918 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7155 "bison-chapel.cpp"
    break;

  case 172: /* $@4: %empty  */
#line 928 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7164 "bison-chapel.cpp"
    break;

  case 173: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TRECORD $@4 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 933 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7179 "bison-chapel.cpp"
    break;

  case 174: /* $@5: %empty  */
#line 944 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7188 "bison-chapel.cpp"
    break;

  case 175: /* extern_export_decl_stmt: TEXPORT TRECORD $@5 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 949 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7202 "bison-chapel.cpp"
    break;

  case 176: /* $@6: %empty  */
#line 959 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7211 "bison-chapel.cpp"
    break;

  case 177: /* extern_export_decl_stmt: TEXPORT STRINGLITERAL TRECORD $@6 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 964 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7225 "bison-chapel.cpp"
    break;

  case 178: /* extern_export_decl_stmt: TEXTERN opt_expr fn_decl_stmt  */
#line 975 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7233 "bison-chapel.cpp"
    break;

  case 179: /* extern_export_decl_stmt: TEXPORT opt_expr fn_decl_stmt  */
#line 979 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7241 "bison-chapel.cpp"
    break;

  case 180: /* extern_export_decl_stmt: TEXTERN opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 984 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7253 "bison-chapel.cpp"
    break;

  case 181: /* extern_export_decl_stmt: TEXPORT opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 992 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7265 "bison-chapel.cpp"
    break;

  case 182: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1003 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7273 "bison-chapel.cpp"
    break;

  case 183: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1009 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7279 "bison-chapel.cpp"
    break;

  case 184: /* loop_stmt: TWHILE expr do_stmt  */
#line 1010 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7285 "bison-chapel.cpp"
    break;

  case 185: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1011 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7291 "bison-chapel.cpp"
    break;

  case 186: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1012 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7297 "bison-chapel.cpp"
    break;

  case 187: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1013 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7303 "bison-chapel.cpp"
    break;

  case 188: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1014 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7309 "bison-chapel.cpp"
    break;

  case 189: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1015 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7315 "bison-chapel.cpp"
    break;

  case 190: /* loop_stmt: TFOR expr do_stmt  */
#line 1016 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7321 "bison-chapel.cpp"
    break;

  case 191: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1017 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7327 "bison-chapel.cpp"
    break;

  case 192: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1018 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7333 "bison-chapel.cpp"
    break;

  case 193: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1019 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7339 "bison-chapel.cpp"
    break;

  case 194: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1020 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7345 "bison-chapel.cpp"
    break;

  case 195: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1021 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7351 "bison-chapel.cpp"
    break;

  case 196: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1022 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7357 "bison-chapel.cpp"
    break;

  case 197: /* loop_stmt: TFORALL expr do_stmt  */
#line 1023 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7363 "bison-chapel.cpp"
    break;

  case 198: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1024 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7369 "bison-chapel.cpp"
    break;

  case 199: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1025 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7375 "bison-chapel.cpp"
    break;

  case 200: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1026 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7381 "bison-chapel.cpp"
    break;

  case 201: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1028 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7391 "bison-chapel.cpp"
    break;

  case 202: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1034 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7401 "bison-chapel.cpp"
    break;

  case 203: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1040 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7411 "bison-chapel.cpp"
    break;

  case 204: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1046 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7421 "bison-chapel.cpp"
    break;

  case 205: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1052 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7432 "bison-chapel.cpp"
    break;

  case 206: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1059 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7443 "bison-chapel.cpp"
    break;

  case 207: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1066 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7451 "bison-chapel.cpp"
    break;

  case 208: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1070 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7459 "bison-chapel.cpp"
    break;

  case 209: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1076 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7465 "bison-chapel.cpp"
    break;

  case 210: /* if_stmt: TIF expr TTHEN stmt  */
#line 1080 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7471 "bison-chapel.cpp"
    break;

  case 211: /* if_stmt: TIF expr block_stmt  */
#line 1081 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7477 "bison-chapel.cpp"
    break;

  case 212: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1082 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7483 "bison-chapel.cpp"
    break;

  case 213: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1083 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7489 "bison-chapel.cpp"
    break;

  case 214: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1085 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7495 "bison-chapel.cpp"
    break;

  case 215: /* if_stmt: TIF ifvar block_stmt  */
#line 1086 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7501 "bison-chapel.cpp"
    break;

  case 216: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1087 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7507 "bison-chapel.cpp"
    break;

  case 217: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1088 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7513 "bison-chapel.cpp"
    break;

  case 218: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1090 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7520 "bison-chapel.cpp"
    break;

  case 219: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1092 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7527 "bison-chapel.cpp"
    break;

  case 220: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1094 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7534 "bison-chapel.cpp"
    break;

  case 221: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1096 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7541 "bison-chapel.cpp"
    break;

  case 222: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1101 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 7547 "bison-chapel.cpp"
    break;

  case 223: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1102 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 7553 "bison-chapel.cpp"
    break;

  case 224: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1107 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 7559 "bison-chapel.cpp"
    break;

  case 225: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1109 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 7568 "bison-chapel.cpp"
    break;

  case 226: /* ifc_formal_ls: ifc_formal  */
#line 1116 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7574 "bison-chapel.cpp"
    break;

  case 227: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1117 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7580 "bison-chapel.cpp"
    break;

  case 228: /* ifc_formal: ident_def  */
#line 1122 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 7586 "bison-chapel.cpp"
    break;

  case 229: /* implements_type_ident: TIDENT  */
#line 1127 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 7592 "bison-chapel.cpp"
    break;

  case 230: /* implements_type_ident: TBOOL  */
#line 1128 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7598 "bison-chapel.cpp"
    break;

  case 231: /* implements_type_ident: TINT  */
#line 1129 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7604 "bison-chapel.cpp"
    break;

  case 232: /* implements_type_ident: TUINT  */
#line 1130 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7610 "bison-chapel.cpp"
    break;

  case 233: /* implements_type_ident: TREAL  */
#line 1131 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7616 "bison-chapel.cpp"
    break;

  case 234: /* implements_type_ident: TIMAG  */
#line 1132 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7622 "bison-chapel.cpp"
    break;

  case 235: /* implements_type_ident: TCOMPLEX  */
#line 1133 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7628 "bison-chapel.cpp"
    break;

  case 236: /* implements_type_ident: TBYTES  */
#line 1134 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7634 "bison-chapel.cpp"
    break;

  case 237: /* implements_type_ident: TSTRING  */
#line 1135 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7640 "bison-chapel.cpp"
    break;

  case 238: /* implements_type_ident: TLOCALE  */
#line 1136 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7646 "bison-chapel.cpp"
    break;

  case 239: /* implements_type_ident: TNOTHING  */
#line 1137 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7652 "bison-chapel.cpp"
    break;

  case 240: /* implements_type_ident: TVOID  */
#line 1138 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7658 "bison-chapel.cpp"
    break;

  case 241: /* implements_type_ident: implements_type_error_ident  */
#line 1140 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 7665 "bison-chapel.cpp"
    break;

  case 242: /* implements_type_error_ident: TNONE  */
#line 1146 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 7671 "bison-chapel.cpp"
    break;

  case 243: /* implements_type_error_ident: TTHIS  */
#line 1147 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 7677 "bison-chapel.cpp"
    break;

  case 244: /* implements_type_error_ident: TFALSE  */
#line 1148 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 7683 "bison-chapel.cpp"
    break;

  case 245: /* implements_type_error_ident: TTRUE  */
#line 1149 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 7689 "bison-chapel.cpp"
    break;

  case 246: /* implements_type_error_ident: TDOMAIN  */
#line 1158 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7695 "bison-chapel.cpp"
    break;

  case 247: /* implements_type_error_ident: TINDEX  */
#line 1159 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7701 "bison-chapel.cpp"
    break;

  case 248: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1164 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 7707 "bison-chapel.cpp"
    break;

  case 249: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1166 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 7714 "bison-chapel.cpp"
    break;

  case 250: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1169 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 7721 "bison-chapel.cpp"
    break;

  case 251: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1175 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 7727 "bison-chapel.cpp"
    break;

  case 252: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1177 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 7734 "bison-chapel.cpp"
    break;

  case 253: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1180 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 7741 "bison-chapel.cpp"
    break;

  case 254: /* defer_stmt: TDEFER stmt  */
#line 1185 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7747 "bison-chapel.cpp"
    break;

  case 255: /* try_stmt: TTRY expr TSEMI  */
#line 1188 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7753 "bison-chapel.cpp"
    break;

  case 256: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1189 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7759 "bison-chapel.cpp"
    break;

  case 257: /* try_stmt: TTRY assignment_stmt  */
#line 1190 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7765 "bison-chapel.cpp"
    break;

  case 258: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1191 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7771 "bison-chapel.cpp"
    break;

  case 259: /* try_stmt: TTRY block_stmt catch_stmt_ls  */
#line 1192 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7777 "bison-chapel.cpp"
    break;

  case 260: /* try_stmt: TTRYBANG block_stmt catch_stmt_ls  */
#line 1193 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7783 "bison-chapel.cpp"
    break;

  case 261: /* catch_stmt_ls: %empty  */
#line 1197 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7789 "bison-chapel.cpp"
    break;

  case 262: /* catch_stmt_ls: catch_stmt_ls catch_stmt  */
#line 1198 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7795 "bison-chapel.cpp"
    break;

  case 263: /* catch_stmt: TCATCH block_stmt  */
#line 1202 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7801 "bison-chapel.cpp"
    break;

  case 264: /* catch_stmt: TCATCH catch_expr block_stmt  */
#line 1203 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7807 "bison-chapel.cpp"
    break;

  case 265: /* catch_stmt: TCATCH TLP catch_expr TRP block_stmt  */
#line 1204 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7813 "bison-chapel.cpp"
    break;

  case 266: /* catch_expr: ident_def  */
#line 1208 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7819 "bison-chapel.cpp"
    break;

  case 267: /* catch_expr: ident_def TCOLON expr  */
#line 1209 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7825 "bison-chapel.cpp"
    break;

  case 268: /* throw_stmt: TTHROW expr TSEMI  */
#line 1213 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7831 "bison-chapel.cpp"
    break;

  case 269: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1217 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7837 "bison-chapel.cpp"
    break;

  case 270: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1219 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7843 "bison-chapel.cpp"
    break;

  case 271: /* when_stmt_ls: %empty  */
#line 1223 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7849 "bison-chapel.cpp"
    break;

  case 272: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1224 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7855 "bison-chapel.cpp"
    break;

  case 273: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1229 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7861 "bison-chapel.cpp"
    break;

  case 274: /* when_stmt: TOTHERWISE stmt  */
#line 1231 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7867 "bison-chapel.cpp"
    break;

  case 275: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1233 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7873 "bison-chapel.cpp"
    break;

  case 276: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1240 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7887 "bison-chapel.cpp"
    break;

  case 277: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR error TRCBR  */
#line 1250 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7901 "bison-chapel.cpp"
    break;

  case 278: /* class_tag: TCLASS  */
#line 1263 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7911 "bison-chapel.cpp"
    break;

  case 279: /* class_tag: TRECORD  */
#line 1268 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7921 "bison-chapel.cpp"
    break;

  case 280: /* class_tag: TUNION  */
#line 1273 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7931 "bison-chapel.cpp"
    break;

  case 281: /* opt_inherit: %empty  */
#line 1281 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 7937 "bison-chapel.cpp"
    break;

  case 282: /* opt_inherit: TCOLON expr_ls  */
#line 1282 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7943 "bison-chapel.cpp"
    break;

  case 283: /* class_level_stmt_ls: %empty  */
#line 1287 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7949 "bison-chapel.cpp"
    break;

  case 284: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 1289 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7955 "bison-chapel.cpp"
    break;

  case 285: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 1291 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7961 "bison-chapel.cpp"
    break;

  case 286: /* enum_decl_stmt: enum_header ident_def TLCBR enum_ls TRCBR  */
#line 1296 "chapel.ypp"
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
#line 7981 "bison-chapel.cpp"
    break;

  case 287: /* enum_decl_stmt: enum_header ident_def TLCBR error TRCBR  */
#line 1312 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7989 "bison-chapel.cpp"
    break;

  case 288: /* enum_header: TENUM  */
#line 1319 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7999 "bison-chapel.cpp"
    break;

  case 289: /* enum_ls: enum_item  */
#line 1328 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8011 "bison-chapel.cpp"
    break;

  case 290: /* enum_ls: enum_ls TCOMMA  */
#line 1336 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8019 "bison-chapel.cpp"
    break;

  case 291: /* enum_ls: enum_ls TCOMMA enum_item  */
#line 1340 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8027 "bison-chapel.cpp"
    break;

  case 292: /* enum_item: ident_def  */
#line 1346 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8033 "bison-chapel.cpp"
    break;

  case 293: /* enum_item: ident_def TASSIGN expr  */
#line 1347 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8039 "bison-chapel.cpp"
    break;

  case 294: /* $@7: %empty  */
#line 1352 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8048 "bison-chapel.cpp"
    break;

  case 295: /* $@8: %empty  */
#line 1357 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8057 "bison-chapel.cpp"
    break;

  case 296: /* lambda_decl_expr: TLAMBDA $@7 req_formal_ls $@8 opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1362 "chapel.ypp"
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
#line 8079 "bison-chapel.cpp"
    break;

  case 297: /* linkage_spec: %empty  */
#line 1384 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8087 "bison-chapel.cpp"
    break;

  case 298: /* linkage_spec: TINLINE  */
#line 1387 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8096 "bison-chapel.cpp"
    break;

  case 299: /* linkage_spec: TOVERRIDE  */
#line 1391 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8105 "bison-chapel.cpp"
    break;

  case 300: /* $@9: %empty  */
#line 1399 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8119 "bison-chapel.cpp"
    break;

  case 301: /* $@10: %empty  */
#line 1409 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8130 "bison-chapel.cpp"
    break;

  case 302: /* fn_decl_stmt: linkage_spec proc_iter_or_op $@9 fn_decl_stmt_inner $@10 opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where opt_function_body_stmt  */
#line 1416 "chapel.ypp"
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
#line 8164 "bison-chapel.cpp"
    break;

  case 303: /* fn_decl_stmt_inner: opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1449 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8172 "bison-chapel.cpp"
    break;

  case 304: /* fn_decl_stmt_inner: opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1453 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8181 "bison-chapel.cpp"
    break;

  case 305: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1458 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8189 "bison-chapel.cpp"
    break;

  case 306: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1462 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8198 "bison-chapel.cpp"
    break;

  case 307: /* fn_decl_stmt_inner: opt_this_intent_tag error opt_formal_ls  */
#line 1467 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8206 "bison-chapel.cpp"
    break;

  case 309: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1474 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8212 "bison-chapel.cpp"
    break;

  case 310: /* fn_ident: ident_fn_def  */
#line 1478 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8218 "bison-chapel.cpp"
    break;

  case 311: /* fn_ident: TBAND  */
#line 1479 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8224 "bison-chapel.cpp"
    break;

  case 312: /* fn_ident: TBOR  */
#line 1480 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8230 "bison-chapel.cpp"
    break;

  case 313: /* fn_ident: TBXOR  */
#line 1481 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8236 "bison-chapel.cpp"
    break;

  case 314: /* fn_ident: TBNOT  */
#line 1482 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8242 "bison-chapel.cpp"
    break;

  case 315: /* fn_ident: TEQUAL  */
#line 1483 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8248 "bison-chapel.cpp"
    break;

  case 316: /* fn_ident: TNOTEQUAL  */
#line 1484 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8254 "bison-chapel.cpp"
    break;

  case 317: /* fn_ident: TLESSEQUAL  */
#line 1485 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8260 "bison-chapel.cpp"
    break;

  case 318: /* fn_ident: TGREATEREQUAL  */
#line 1486 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8266 "bison-chapel.cpp"
    break;

  case 319: /* fn_ident: TLESS  */
#line 1487 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8272 "bison-chapel.cpp"
    break;

  case 320: /* fn_ident: TGREATER  */
#line 1488 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8278 "bison-chapel.cpp"
    break;

  case 321: /* fn_ident: TPLUS  */
#line 1489 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8284 "bison-chapel.cpp"
    break;

  case 322: /* fn_ident: TMINUS  */
#line 1490 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8290 "bison-chapel.cpp"
    break;

  case 323: /* fn_ident: TSTAR  */
#line 1491 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8296 "bison-chapel.cpp"
    break;

  case 324: /* fn_ident: TDIVIDE  */
#line 1492 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8302 "bison-chapel.cpp"
    break;

  case 325: /* fn_ident: TSHIFTLEFT  */
#line 1493 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8308 "bison-chapel.cpp"
    break;

  case 326: /* fn_ident: TSHIFTRIGHT  */
#line 1494 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8314 "bison-chapel.cpp"
    break;

  case 327: /* fn_ident: TMOD  */
#line 1495 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8320 "bison-chapel.cpp"
    break;

  case 328: /* fn_ident: TEXP  */
#line 1496 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8326 "bison-chapel.cpp"
    break;

  case 329: /* fn_ident: TBANG  */
#line 1497 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8332 "bison-chapel.cpp"
    break;

  case 330: /* fn_ident: TBY  */
#line 1498 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8338 "bison-chapel.cpp"
    break;

  case 331: /* fn_ident: THASH  */
#line 1499 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8344 "bison-chapel.cpp"
    break;

  case 332: /* fn_ident: TALIGN  */
#line 1500 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8350 "bison-chapel.cpp"
    break;

  case 333: /* fn_ident: TSWAP  */
#line 1501 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8356 "bison-chapel.cpp"
    break;

  case 334: /* fn_ident: TIO  */
#line 1502 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8362 "bison-chapel.cpp"
    break;

  case 335: /* fn_ident: TINITEQUALS  */
#line 1503 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8368 "bison-chapel.cpp"
    break;

  case 336: /* fn_ident: ident_def TBANG  */
#line 1504 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8374 "bison-chapel.cpp"
    break;

  case 337: /* assignop_ident: TASSIGN  */
#line 1508 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8380 "bison-chapel.cpp"
    break;

  case 338: /* assignop_ident: TASSIGNPLUS  */
#line 1509 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8386 "bison-chapel.cpp"
    break;

  case 339: /* assignop_ident: TASSIGNMINUS  */
#line 1510 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8392 "bison-chapel.cpp"
    break;

  case 340: /* assignop_ident: TASSIGNMULTIPLY  */
#line 1511 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8398 "bison-chapel.cpp"
    break;

  case 341: /* assignop_ident: TASSIGNDIVIDE  */
#line 1512 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8404 "bison-chapel.cpp"
    break;

  case 342: /* assignop_ident: TASSIGNMOD  */
#line 1513 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8410 "bison-chapel.cpp"
    break;

  case 343: /* assignop_ident: TASSIGNEXP  */
#line 1514 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8416 "bison-chapel.cpp"
    break;

  case 344: /* assignop_ident: TASSIGNBAND  */
#line 1515 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8422 "bison-chapel.cpp"
    break;

  case 345: /* assignop_ident: TASSIGNBOR  */
#line 1516 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8428 "bison-chapel.cpp"
    break;

  case 346: /* assignop_ident: TASSIGNBXOR  */
#line 1517 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8434 "bison-chapel.cpp"
    break;

  case 347: /* assignop_ident: TASSIGNSR  */
#line 1518 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8440 "bison-chapel.cpp"
    break;

  case 348: /* assignop_ident: TASSIGNSL  */
#line 1519 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 8446 "bison-chapel.cpp"
    break;

  case 349: /* opt_formal_ls: %empty  */
#line 1523 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8452 "bison-chapel.cpp"
    break;

  case 350: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1524 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8458 "bison-chapel.cpp"
    break;

  case 351: /* req_formal_ls: TLP formal_ls TRP  */
#line 1528 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8464 "bison-chapel.cpp"
    break;

  case 352: /* formal_ls: %empty  */
#line 1532 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8470 "bison-chapel.cpp"
    break;

  case 353: /* formal_ls: formal  */
#line 1533 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8476 "bison-chapel.cpp"
    break;

  case 354: /* formal_ls: formal_ls TCOMMA formal  */
#line 1534 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8482 "bison-chapel.cpp"
    break;

  case 355: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1539 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8488 "bison-chapel.cpp"
    break;

  case 356: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1541 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 8494 "bison-chapel.cpp"
    break;

  case 357: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1543 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 8500 "bison-chapel.cpp"
    break;

  case 358: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1545 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 8506 "bison-chapel.cpp"
    break;

  case 359: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1547 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8512 "bison-chapel.cpp"
    break;

  case 360: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1549 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 8518 "bison-chapel.cpp"
    break;

  case 361: /* opt_intent_tag: %empty  */
#line 1553 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 8524 "bison-chapel.cpp"
    break;

  case 362: /* opt_intent_tag: required_intent_tag  */
#line 1554 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 8530 "bison-chapel.cpp"
    break;

  case 363: /* required_intent_tag: TIN  */
#line 1558 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 8536 "bison-chapel.cpp"
    break;

  case 364: /* required_intent_tag: TINOUT  */
#line 1559 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 8542 "bison-chapel.cpp"
    break;

  case 365: /* required_intent_tag: TOUT  */
#line 1560 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 8548 "bison-chapel.cpp"
    break;

  case 366: /* required_intent_tag: TCONST  */
#line 1561 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 8554 "bison-chapel.cpp"
    break;

  case 367: /* required_intent_tag: TCONST TIN  */
#line 1562 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 8560 "bison-chapel.cpp"
    break;

  case 368: /* required_intent_tag: TCONST TREF  */
#line 1563 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 8566 "bison-chapel.cpp"
    break;

  case 369: /* required_intent_tag: TPARAM  */
#line 1564 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 8572 "bison-chapel.cpp"
    break;

  case 370: /* required_intent_tag: TREF  */
#line 1565 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 8578 "bison-chapel.cpp"
    break;

  case 371: /* required_intent_tag: TTYPE  */
#line 1566 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 8584 "bison-chapel.cpp"
    break;

  case 372: /* opt_this_intent_tag: %empty  */
#line 1570 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 8590 "bison-chapel.cpp"
    break;

  case 373: /* opt_this_intent_tag: TPARAM  */
#line 1571 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 8596 "bison-chapel.cpp"
    break;

  case 374: /* opt_this_intent_tag: TREF  */
#line 1572 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 8602 "bison-chapel.cpp"
    break;

  case 375: /* opt_this_intent_tag: TCONST TREF  */
#line 1573 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 8608 "bison-chapel.cpp"
    break;

  case 376: /* opt_this_intent_tag: TCONST  */
#line 1574 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 8614 "bison-chapel.cpp"
    break;

  case 377: /* opt_this_intent_tag: TTYPE  */
#line 1575 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 8620 "bison-chapel.cpp"
    break;

  case 378: /* proc_iter_or_op: TPROC  */
#line 1579 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 8626 "bison-chapel.cpp"
    break;

  case 379: /* proc_iter_or_op: TITER  */
#line 1580 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 8632 "bison-chapel.cpp"
    break;

  case 380: /* proc_iter_or_op: TOPERATOR  */
#line 1581 "chapel.ypp"
            { (yyval.procIterOp) = ProcIterOp_OP; }
#line 8638 "bison-chapel.cpp"
    break;

  case 381: /* opt_ret_tag: %empty  */
#line 1585 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 8644 "bison-chapel.cpp"
    break;

  case 382: /* opt_ret_tag: TCONST  */
#line 1586 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 8650 "bison-chapel.cpp"
    break;

  case 383: /* opt_ret_tag: TCONST TREF  */
#line 1587 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 8656 "bison-chapel.cpp"
    break;

  case 384: /* opt_ret_tag: TREF  */
#line 1588 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 8662 "bison-chapel.cpp"
    break;

  case 385: /* opt_ret_tag: TPARAM  */
#line 1589 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 8668 "bison-chapel.cpp"
    break;

  case 386: /* opt_ret_tag: TTYPE  */
#line 1590 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 8674 "bison-chapel.cpp"
    break;

  case 387: /* opt_throws_error: %empty  */
#line 1594 "chapel.ypp"
          { (yyval.b) = false; }
#line 8680 "bison-chapel.cpp"
    break;

  case 388: /* opt_throws_error: TTHROWS  */
#line 1595 "chapel.ypp"
          { (yyval.b) = true;  }
#line 8686 "bison-chapel.cpp"
    break;

  case 389: /* opt_function_body_stmt: TSEMI  */
#line 1598 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 8692 "bison-chapel.cpp"
    break;

  case 392: /* function_body_stmt: return_stmt  */
#line 1604 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8698 "bison-chapel.cpp"
    break;

  case 393: /* query_expr: TQUERIEDIDENT  */
#line 1608 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8704 "bison-chapel.cpp"
    break;

  case 394: /* var_arg_expr: TDOTDOTDOT  */
#line 1612 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8710 "bison-chapel.cpp"
    break;

  case 395: /* var_arg_expr: TDOTDOTDOT expr  */
#line 1613 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8716 "bison-chapel.cpp"
    break;

  case 396: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 1614 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8726 "bison-chapel.cpp"
    break;

  case 397: /* opt_lifetime_where: %empty  */
#line 1622 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8732 "bison-chapel.cpp"
    break;

  case 398: /* opt_lifetime_where: TWHERE expr  */
#line 1624 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8738 "bison-chapel.cpp"
    break;

  case 399: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 1626 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8744 "bison-chapel.cpp"
    break;

  case 400: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 1628 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8750 "bison-chapel.cpp"
    break;

  case 401: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 1630 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8756 "bison-chapel.cpp"
    break;

  case 402: /* lifetime_components_expr: lifetime_expr  */
#line 1635 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8762 "bison-chapel.cpp"
    break;

  case 403: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 1637 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8768 "bison-chapel.cpp"
    break;

  case 404: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 1641 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8774 "bison-chapel.cpp"
    break;

  case 405: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 1642 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8780 "bison-chapel.cpp"
    break;

  case 406: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 1643 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8786 "bison-chapel.cpp"
    break;

  case 407: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 1644 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8792 "bison-chapel.cpp"
    break;

  case 408: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 1645 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8798 "bison-chapel.cpp"
    break;

  case 409: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 1646 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8804 "bison-chapel.cpp"
    break;

  case 410: /* lifetime_expr: TRETURN lifetime_ident  */
#line 1647 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8810 "bison-chapel.cpp"
    break;

  case 411: /* lifetime_ident: TIDENT  */
#line 1652 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8816 "bison-chapel.cpp"
    break;

  case 412: /* lifetime_ident: TTHIS  */
#line 1654 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8822 "bison-chapel.cpp"
    break;

  case 413: /* type_alias_decl_stmt: TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1659 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8828 "bison-chapel.cpp"
    break;

  case 414: /* type_alias_decl_stmt: TCONFIG TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1661 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8834 "bison-chapel.cpp"
    break;

  case 415: /* type_alias_decl_stmt: TEXTERN TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1663 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8840 "bison-chapel.cpp"
    break;

  case 416: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 1668 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 8857 "bison-chapel.cpp"
    break;

  case 417: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 1681 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 8875 "bison-chapel.cpp"
    break;

  case 418: /* opt_init_type: %empty  */
#line 1697 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 8881 "bison-chapel.cpp"
    break;

  case 419: /* opt_init_type: TASSIGN type_level_expr  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8887 "bison-chapel.cpp"
    break;

  case 420: /* opt_init_type: TASSIGN array_type  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8893 "bison-chapel.cpp"
    break;

  case 421: /* var_decl_type: TPARAM  */
#line 1705 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8899 "bison-chapel.cpp"
    break;

  case 422: /* var_decl_type: TCONST  */
#line 1706 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8905 "bison-chapel.cpp"
    break;

  case 423: /* var_decl_type: TREF  */
#line 1707 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8911 "bison-chapel.cpp"
    break;

  case 424: /* var_decl_type: TCONST TREF  */
#line 1708 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8917 "bison-chapel.cpp"
    break;

  case 425: /* var_decl_type: TVAR  */
#line 1709 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8923 "bison-chapel.cpp"
    break;

  case 426: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1714 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8933 "bison-chapel.cpp"
    break;

  case 427: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1720 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8942 "bison-chapel.cpp"
    break;

  case 429: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 1729 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8951 "bison-chapel.cpp"
    break;

  case 430: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 1737 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8957 "bison-chapel.cpp"
    break;

  case 431: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 1739 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8963 "bison-chapel.cpp"
    break;

  case 432: /* tuple_var_decl_component: TUNDERSCORE  */
#line 1744 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8969 "bison-chapel.cpp"
    break;

  case 433: /* tuple_var_decl_component: ident_def  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8975 "bison-chapel.cpp"
    break;

  case 434: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8981 "bison-chapel.cpp"
    break;

  case 435: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 1753 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8987 "bison-chapel.cpp"
    break;

  case 436: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 1755 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8993 "bison-chapel.cpp"
    break;

  case 437: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 1757 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8999 "bison-chapel.cpp"
    break;

  case 438: /* opt_init_expr: %empty  */
#line 1763 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9005 "bison-chapel.cpp"
    break;

  case 439: /* opt_init_expr: TASSIGN TNOINIT  */
#line 1764 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9011 "bison-chapel.cpp"
    break;

  case 440: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 1765 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9017 "bison-chapel.cpp"
    break;

  case 441: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 1771 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9023 "bison-chapel.cpp"
    break;

  case 442: /* ret_array_type: TLSBR TRSBR  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9029 "bison-chapel.cpp"
    break;

  case 443: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9037 "bison-chapel.cpp"
    break;

  case 444: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9045 "bison-chapel.cpp"
    break;

  case 445: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9051 "bison-chapel.cpp"
    break;

  case 446: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9059 "bison-chapel.cpp"
    break;

  case 447: /* ret_array_type: TLSBR error TRSBR  */
#line 1789 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9067 "bison-chapel.cpp"
    break;

  case 448: /* opt_ret_type: %empty  */
#line 1796 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9073 "bison-chapel.cpp"
    break;

  case 449: /* opt_ret_type: TCOLON type_level_expr  */
#line 1797 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9079 "bison-chapel.cpp"
    break;

  case 450: /* opt_ret_type: TCOLON ret_array_type  */
#line 1798 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9085 "bison-chapel.cpp"
    break;

  case 451: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 1799 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9091 "bison-chapel.cpp"
    break;

  case 452: /* opt_ret_type: error  */
#line 1800 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9097 "bison-chapel.cpp"
    break;

  case 453: /* opt_type: %empty  */
#line 1805 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9103 "bison-chapel.cpp"
    break;

  case 454: /* opt_type: TCOLON type_level_expr  */
#line 1806 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9109 "bison-chapel.cpp"
    break;

  case 455: /* opt_type: TCOLON array_type  */
#line 1807 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9115 "bison-chapel.cpp"
    break;

  case 456: /* opt_type: TCOLON reserved_type_ident_use  */
#line 1808 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9121 "bison-chapel.cpp"
    break;

  case 457: /* opt_type: error  */
#line 1809 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9127 "bison-chapel.cpp"
    break;

  case 458: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1830 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9135 "bison-chapel.cpp"
    break;

  case 459: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 1834 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9143 "bison-chapel.cpp"
    break;

  case 460: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 1838 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9155 "bison-chapel.cpp"
    break;

  case 461: /* array_type: TLSBR error TRSBR  */
#line 1846 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9163 "bison-chapel.cpp"
    break;

  case 462: /* opt_formal_array_elt_type: %empty  */
#line 1852 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9169 "bison-chapel.cpp"
    break;

  case 463: /* opt_formal_array_elt_type: type_level_expr  */
#line 1853 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9175 "bison-chapel.cpp"
    break;

  case 464: /* opt_formal_array_elt_type: query_expr  */
#line 1854 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9181 "bison-chapel.cpp"
    break;

  case 465: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 1859 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9187 "bison-chapel.cpp"
    break;

  case 466: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 1861 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9193 "bison-chapel.cpp"
    break;

  case 467: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9199 "bison-chapel.cpp"
    break;

  case 468: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9205 "bison-chapel.cpp"
    break;

  case 469: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 1871 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9211 "bison-chapel.cpp"
    break;

  case 470: /* opt_formal_type: %empty  */
#line 1875 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9217 "bison-chapel.cpp"
    break;

  case 471: /* opt_formal_type: TCOLON type_level_expr  */
#line 1876 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9223 "bison-chapel.cpp"
    break;

  case 472: /* opt_formal_type: TCOLON query_expr  */
#line 1877 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9229 "bison-chapel.cpp"
    break;

  case 473: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 1878 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9235 "bison-chapel.cpp"
    break;

  case 474: /* opt_formal_type: TCOLON formal_array_type  */
#line 1879 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9241 "bison-chapel.cpp"
    break;

  case 475: /* expr_ls: expr  */
#line 1885 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9247 "bison-chapel.cpp"
    break;

  case 476: /* expr_ls: query_expr  */
#line 1886 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9253 "bison-chapel.cpp"
    break;

  case 477: /* expr_ls: expr_ls TCOMMA expr  */
#line 1887 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9259 "bison-chapel.cpp"
    break;

  case 478: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 1888 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9265 "bison-chapel.cpp"
    break;

  case 479: /* simple_expr_ls: expr  */
#line 1892 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9271 "bison-chapel.cpp"
    break;

  case 480: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 1893 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9277 "bison-chapel.cpp"
    break;

  case 481: /* tuple_component: TUNDERSCORE  */
#line 1897 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9283 "bison-chapel.cpp"
    break;

  case 482: /* tuple_component: opt_try_expr  */
#line 1898 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9289 "bison-chapel.cpp"
    break;

  case 483: /* tuple_component: query_expr  */
#line 1899 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9295 "bison-chapel.cpp"
    break;

  case 484: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 1903 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9301 "bison-chapel.cpp"
    break;

  case 485: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 1904 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9307 "bison-chapel.cpp"
    break;

  case 486: /* opt_actual_ls: %empty  */
#line 1908 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9313 "bison-chapel.cpp"
    break;

  case 488: /* actual_ls: actual_expr  */
#line 1913 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9319 "bison-chapel.cpp"
    break;

  case 489: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 1914 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9325 "bison-chapel.cpp"
    break;

  case 490: /* actual_expr: ident_use TASSIGN query_expr  */
#line 1918 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9331 "bison-chapel.cpp"
    break;

  case 491: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 1919 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9337 "bison-chapel.cpp"
    break;

  case 492: /* actual_expr: query_expr  */
#line 1920 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9343 "bison-chapel.cpp"
    break;

  case 493: /* actual_expr: opt_try_expr  */
#line 1921 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9349 "bison-chapel.cpp"
    break;

  case 494: /* ident_expr: ident_use  */
#line 1925 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9355 "bison-chapel.cpp"
    break;

  case 495: /* ident_expr: scalar_type  */
#line 1926 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9361 "bison-chapel.cpp"
    break;

  case 496: /* type_level_expr: sub_type_level_expr  */
#line 1938 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9367 "bison-chapel.cpp"
    break;

  case 497: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 1940 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9373 "bison-chapel.cpp"
    break;

  case 498: /* type_level_expr: TQUESTION  */
#line 1942 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9379 "bison-chapel.cpp"
    break;

  case 504: /* sub_type_level_expr: TSINGLE expr  */
#line 1953 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9385 "bison-chapel.cpp"
    break;

  case 505: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 1955 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9391 "bison-chapel.cpp"
    break;

  case 506: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9397 "bison-chapel.cpp"
    break;

  case 507: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 1959 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9403 "bison-chapel.cpp"
    break;

  case 508: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 1961 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 9409 "bison-chapel.cpp"
    break;

  case 509: /* sub_type_level_expr: TATOMIC expr  */
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9415 "bison-chapel.cpp"
    break;

  case 510: /* sub_type_level_expr: TSYNC expr  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9421 "bison-chapel.cpp"
    break;

  case 511: /* sub_type_level_expr: TOWNED  */
#line 1968 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9427 "bison-chapel.cpp"
    break;

  case 512: /* sub_type_level_expr: TOWNED expr  */
#line 1970 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9433 "bison-chapel.cpp"
    break;

  case 513: /* sub_type_level_expr: TUNMANAGED  */
#line 1972 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9439 "bison-chapel.cpp"
    break;

  case 514: /* sub_type_level_expr: TUNMANAGED expr  */
#line 1974 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9445 "bison-chapel.cpp"
    break;

  case 515: /* sub_type_level_expr: TSHARED  */
#line 1976 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9451 "bison-chapel.cpp"
    break;

  case 516: /* sub_type_level_expr: TSHARED expr  */
#line 1978 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9457 "bison-chapel.cpp"
    break;

  case 517: /* sub_type_level_expr: TBORROWED  */
#line 1980 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9463 "bison-chapel.cpp"
    break;

  case 518: /* sub_type_level_expr: TBORROWED expr  */
#line 1982 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9469 "bison-chapel.cpp"
    break;

  case 519: /* sub_type_level_expr: TCLASS  */
#line 1985 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9475 "bison-chapel.cpp"
    break;

  case 520: /* sub_type_level_expr: TRECORD  */
#line 1987 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9481 "bison-chapel.cpp"
    break;

  case 521: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 1992 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9487 "bison-chapel.cpp"
    break;

  case 522: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 1994 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9493 "bison-chapel.cpp"
    break;

  case 523: /* for_expr: TFOR expr TDO expr  */
#line 1996 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9499 "bison-chapel.cpp"
    break;

  case 524: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 1998 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9505 "bison-chapel.cpp"
    break;

  case 525: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2000 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9511 "bison-chapel.cpp"
    break;

  case 526: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2002 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9517 "bison-chapel.cpp"
    break;

  case 527: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2004 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9523 "bison-chapel.cpp"
    break;

  case 528: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2006 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9529 "bison-chapel.cpp"
    break;

  case 529: /* for_expr: TFORALL expr TDO expr  */
#line 2008 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9535 "bison-chapel.cpp"
    break;

  case 530: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2010 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9541 "bison-chapel.cpp"
    break;

  case 531: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2012 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9547 "bison-chapel.cpp"
    break;

  case 532: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2014 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9553 "bison-chapel.cpp"
    break;

  case 533: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2016 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 9564 "bison-chapel.cpp"
    break;

  case 534: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2023 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9574 "bison-chapel.cpp"
    break;

  case 535: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2029 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 9584 "bison-chapel.cpp"
    break;

  case 536: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2035 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9594 "bison-chapel.cpp"
    break;

  case 537: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2041 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9604 "bison-chapel.cpp"
    break;

  case 538: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2050 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9610 "bison-chapel.cpp"
    break;

  case 539: /* nil_expr: TNIL  */
#line 2059 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 9616 "bison-chapel.cpp"
    break;

  case 547: /* stmt_level_expr: io_expr TIO expr  */
#line 2075 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9622 "bison-chapel.cpp"
    break;

  case 548: /* opt_task_intent_ls: %empty  */
#line 2079 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 9628 "bison-chapel.cpp"
    break;

  case 550: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2084 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9634 "bison-chapel.cpp"
    break;

  case 551: /* task_intent_ls: intent_expr  */
#line 2088 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9640 "bison-chapel.cpp"
    break;

  case 552: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2089 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9646 "bison-chapel.cpp"
    break;

  case 553: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2093 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9652 "bison-chapel.cpp"
    break;

  case 554: /* forall_intent_ls: intent_expr  */
#line 2097 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9658 "bison-chapel.cpp"
    break;

  case 555: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2098 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9664 "bison-chapel.cpp"
    break;

  case 556: /* intent_expr: shadow_var_prefix ident_expr opt_type opt_init_expr  */
#line 2103 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9672 "bison-chapel.cpp"
    break;

  case 557: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2107 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9680 "bison-chapel.cpp"
    break;

  case 558: /* intent_expr: expr TREDUCE ident_expr  */
#line 2111 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9688 "bison-chapel.cpp"
    break;

  case 559: /* shadow_var_prefix: TCONST  */
#line 2117 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9694 "bison-chapel.cpp"
    break;

  case 560: /* shadow_var_prefix: TIN  */
#line 2118 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9700 "bison-chapel.cpp"
    break;

  case 561: /* shadow_var_prefix: TCONST TIN  */
#line 2119 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9706 "bison-chapel.cpp"
    break;

  case 562: /* shadow_var_prefix: TREF  */
#line 2120 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9712 "bison-chapel.cpp"
    break;

  case 563: /* shadow_var_prefix: TCONST TREF  */
#line 2121 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9718 "bison-chapel.cpp"
    break;

  case 564: /* shadow_var_prefix: TVAR  */
#line 2122 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9724 "bison-chapel.cpp"
    break;

  case 566: /* io_expr: io_expr TIO expr  */
#line 2128 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9730 "bison-chapel.cpp"
    break;

  case 567: /* new_maybe_decorated: TNEW  */
#line 2133 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9736 "bison-chapel.cpp"
    break;

  case 568: /* new_maybe_decorated: TNEW TOWNED  */
#line 2135 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9744 "bison-chapel.cpp"
    break;

  case 569: /* new_maybe_decorated: TNEW TSHARED  */
#line 2139 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9752 "bison-chapel.cpp"
    break;

  case 570: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2143 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9760 "bison-chapel.cpp"
    break;

  case 571: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2147 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9768 "bison-chapel.cpp"
    break;

  case 572: /* new_expr: new_maybe_decorated expr  */
#line 2155 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9775 "bison-chapel.cpp"
    break;

  case 573: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2160 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9785 "bison-chapel.cpp"
    break;

  case 574: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2166 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9795 "bison-chapel.cpp"
    break;

  case 575: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2172 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9806 "bison-chapel.cpp"
    break;

  case 576: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2179 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9817 "bison-chapel.cpp"
    break;

  case 577: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2189 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9823 "bison-chapel.cpp"
    break;

  case 587: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2206 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9829 "bison-chapel.cpp"
    break;

  case 588: /* expr: expr TCOLON expr  */
#line 2208 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9835 "bison-chapel.cpp"
    break;

  case 589: /* expr: expr TDOTDOT expr  */
#line 2210 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9841 "bison-chapel.cpp"
    break;

  case 590: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2212 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 9847 "bison-chapel.cpp"
    break;

  case 591: /* expr: expr TDOTDOT  */
#line 2227 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 9853 "bison-chapel.cpp"
    break;

  case 592: /* expr: TDOTDOT expr  */
#line 2229 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 9859 "bison-chapel.cpp"
    break;

  case 593: /* expr: TDOTDOTOPENHIGH expr  */
#line 2231 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 9865 "bison-chapel.cpp"
    break;

  case 594: /* expr: TDOTDOT  */
#line 2233 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 9871 "bison-chapel.cpp"
    break;

  case 595: /* opt_expr: %empty  */
#line 2237 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 9877 "bison-chapel.cpp"
    break;

  case 596: /* opt_expr: expr  */
#line 2238 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9883 "bison-chapel.cpp"
    break;

  case 597: /* opt_try_expr: TTRY expr  */
#line 2242 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9889 "bison-chapel.cpp"
    break;

  case 598: /* opt_try_expr: TTRYBANG expr  */
#line 2243 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9895 "bison-chapel.cpp"
    break;

  case 599: /* opt_try_expr: expr  */
#line 2244 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9901 "bison-chapel.cpp"
    break;

  case 604: /* call_base_expr: lhs_expr  */
#line 2260 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9907 "bison-chapel.cpp"
    break;

  case 605: /* call_base_expr: expr TBANG  */
#line 2261 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9913 "bison-chapel.cpp"
    break;

  case 606: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2262 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9919 "bison-chapel.cpp"
    break;

  case 607: /* call_base_expr: lambda_decl_expr  */
#line 2263 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9925 "bison-chapel.cpp"
    break;

  case 609: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2268 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9931 "bison-chapel.cpp"
    break;

  case 610: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2269 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9937 "bison-chapel.cpp"
    break;

  case 611: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2270 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 9943 "bison-chapel.cpp"
    break;

  case 612: /* dot_expr: expr TDOT ident_use  */
#line 2274 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 9949 "bison-chapel.cpp"
    break;

  case 613: /* dot_expr: expr TDOT TTYPE  */
#line 2275 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 9955 "bison-chapel.cpp"
    break;

  case 614: /* dot_expr: expr TDOT TDOMAIN  */
#line 2276 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 9961 "bison-chapel.cpp"
    break;

  case 615: /* dot_expr: expr TDOT TLOCALE  */
#line 2277 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 9967 "bison-chapel.cpp"
    break;

  case 616: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2278 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 9973 "bison-chapel.cpp"
    break;

  case 617: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2286 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9979 "bison-chapel.cpp"
    break;

  case 618: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2287 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9985 "bison-chapel.cpp"
    break;

  case 619: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2288 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9991 "bison-chapel.cpp"
    break;

  case 620: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2289 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9997 "bison-chapel.cpp"
    break;

  case 621: /* bool_literal: TFALSE  */
#line 2293 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10003 "bison-chapel.cpp"
    break;

  case 622: /* bool_literal: TTRUE  */
#line 2294 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10009 "bison-chapel.cpp"
    break;

  case 623: /* str_bytes_literal: STRINGLITERAL  */
#line 2298 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10015 "bison-chapel.cpp"
    break;

  case 624: /* str_bytes_literal: BYTESLITERAL  */
#line 2299 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10021 "bison-chapel.cpp"
    break;

  case 627: /* literal_expr: INTLITERAL  */
#line 2305 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10027 "bison-chapel.cpp"
    break;

  case 628: /* literal_expr: REALLITERAL  */
#line 2306 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10033 "bison-chapel.cpp"
    break;

  case 629: /* literal_expr: IMAGLITERAL  */
#line 2307 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10039 "bison-chapel.cpp"
    break;

  case 630: /* literal_expr: CSTRINGLITERAL  */
#line 2308 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10045 "bison-chapel.cpp"
    break;

  case 631: /* literal_expr: TNONE  */
#line 2309 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10051 "bison-chapel.cpp"
    break;

  case 632: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2310 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10058 "bison-chapel.cpp"
    break;

  case 633: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2312 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10065 "bison-chapel.cpp"
    break;

  case 634: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2314 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10071 "bison-chapel.cpp"
    break;

  case 635: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2315 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10077 "bison-chapel.cpp"
    break;

  case 636: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2317 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10085 "bison-chapel.cpp"
    break;

  case 637: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2321 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10093 "bison-chapel.cpp"
    break;

  case 638: /* assoc_expr_ls: expr TALIAS expr  */
#line 2328 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10099 "bison-chapel.cpp"
    break;

  case 639: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2329 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10105 "bison-chapel.cpp"
    break;

  case 640: /* binary_op_expr: expr TPLUS expr  */
#line 2333 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10111 "bison-chapel.cpp"
    break;

  case 641: /* binary_op_expr: expr TMINUS expr  */
#line 2334 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10117 "bison-chapel.cpp"
    break;

  case 642: /* binary_op_expr: expr TSTAR expr  */
#line 2335 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10123 "bison-chapel.cpp"
    break;

  case 643: /* binary_op_expr: expr TDIVIDE expr  */
#line 2336 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10129 "bison-chapel.cpp"
    break;

  case 644: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2337 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10135 "bison-chapel.cpp"
    break;

  case 645: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2338 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10141 "bison-chapel.cpp"
    break;

  case 646: /* binary_op_expr: expr TMOD expr  */
#line 2339 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10147 "bison-chapel.cpp"
    break;

  case 647: /* binary_op_expr: expr TEQUAL expr  */
#line 2340 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10153 "bison-chapel.cpp"
    break;

  case 648: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2341 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10159 "bison-chapel.cpp"
    break;

  case 649: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2342 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10165 "bison-chapel.cpp"
    break;

  case 650: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2343 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10171 "bison-chapel.cpp"
    break;

  case 651: /* binary_op_expr: expr TLESS expr  */
#line 2344 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10177 "bison-chapel.cpp"
    break;

  case 652: /* binary_op_expr: expr TGREATER expr  */
#line 2345 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10183 "bison-chapel.cpp"
    break;

  case 653: /* binary_op_expr: expr TBAND expr  */
#line 2346 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10189 "bison-chapel.cpp"
    break;

  case 654: /* binary_op_expr: expr TBOR expr  */
#line 2347 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10195 "bison-chapel.cpp"
    break;

  case 655: /* binary_op_expr: expr TBXOR expr  */
#line 2348 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10201 "bison-chapel.cpp"
    break;

  case 656: /* binary_op_expr: expr TAND expr  */
#line 2349 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10207 "bison-chapel.cpp"
    break;

  case 657: /* binary_op_expr: expr TOR expr  */
#line 2350 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10213 "bison-chapel.cpp"
    break;

  case 658: /* binary_op_expr: expr TEXP expr  */
#line 2351 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10219 "bison-chapel.cpp"
    break;

  case 659: /* binary_op_expr: expr TBY expr  */
#line 2352 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10225 "bison-chapel.cpp"
    break;

  case 660: /* binary_op_expr: expr TALIGN expr  */
#line 2353 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10231 "bison-chapel.cpp"
    break;

  case 661: /* binary_op_expr: expr THASH expr  */
#line 2354 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10237 "bison-chapel.cpp"
    break;

  case 662: /* binary_op_expr: expr TDMAPPED expr  */
#line 2355 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10244 "bison-chapel.cpp"
    break;

  case 663: /* unary_op_expr: TPLUS expr  */
#line 2360 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10250 "bison-chapel.cpp"
    break;

  case 664: /* unary_op_expr: TMINUS expr  */
#line 2361 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10256 "bison-chapel.cpp"
    break;

  case 665: /* unary_op_expr: TMINUSMINUS expr  */
#line 2362 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10262 "bison-chapel.cpp"
    break;

  case 666: /* unary_op_expr: TPLUSPLUS expr  */
#line 2363 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10268 "bison-chapel.cpp"
    break;

  case 667: /* unary_op_expr: TBANG expr  */
#line 2364 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10274 "bison-chapel.cpp"
    break;

  case 668: /* unary_op_expr: expr TBANG  */
#line 2365 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10280 "bison-chapel.cpp"
    break;

  case 669: /* unary_op_expr: TBNOT expr  */
#line 2366 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10286 "bison-chapel.cpp"
    break;

  case 670: /* reduce_expr: expr TREDUCE expr  */
#line 2370 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10292 "bison-chapel.cpp"
    break;

  case 671: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2371 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10298 "bison-chapel.cpp"
    break;

  case 672: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2372 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10304 "bison-chapel.cpp"
    break;

  case 673: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2373 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10310 "bison-chapel.cpp"
    break;

  case 674: /* scan_expr: expr TSCAN expr  */
#line 2377 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10316 "bison-chapel.cpp"
    break;

  case 675: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2378 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10322 "bison-chapel.cpp"
    break;

  case 676: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2379 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10328 "bison-chapel.cpp"
    break;

  case 677: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2380 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10334 "bison-chapel.cpp"
    break;

  case 678: /* reduce_scan_op_expr: TPLUS  */
#line 2385 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10340 "bison-chapel.cpp"
    break;

  case 679: /* reduce_scan_op_expr: TSTAR  */
#line 2386 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10346 "bison-chapel.cpp"
    break;

  case 680: /* reduce_scan_op_expr: TAND  */
#line 2387 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10352 "bison-chapel.cpp"
    break;

  case 681: /* reduce_scan_op_expr: TOR  */
#line 2388 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10358 "bison-chapel.cpp"
    break;

  case 682: /* reduce_scan_op_expr: TBAND  */
#line 2389 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10364 "bison-chapel.cpp"
    break;

  case 683: /* reduce_scan_op_expr: TBOR  */
#line 2390 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10370 "bison-chapel.cpp"
    break;

  case 684: /* reduce_scan_op_expr: TBXOR  */
#line 2391 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10376 "bison-chapel.cpp"
    break;


#line 10380 "bison-chapel.cpp"

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
