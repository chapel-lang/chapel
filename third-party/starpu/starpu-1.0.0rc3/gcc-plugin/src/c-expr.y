/* GCC-StarPU
   Copyright (C) 2011 Institut National de Recherche en Informatique et Automatique

   GCC-StarPU is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   GCC-StarPU is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC-StarPU.  If not, see <http://www.gnu.org/licenses/>.  */

/* Parser for simple C expressions in pragmas.  */

%define api.pure
%parse-param { location_t loc }
%parse-param { const char *pragma }
%parse-param { tree *seq }
%debug

%{
  #include <starpu-gcc-config.h>

  #include <gcc-plugin.h>
  #include <plugin.h>
  #include <tree.h>
  #include <cpplib.h>

  #ifdef HAVE_C_FAMILY_C_COMMON_H
  # include <c-family/c-common.h>
  #elif HAVE_C_COMMON_H
  # include <c-common.h>
  #endif

  #ifdef HAVE_C_FAMILY_C_PRAGMA_H
  # include <c-family/c-pragma.h>
  #elif HAVE_C_PRAGMA_H
  # include <c-pragma.h>
  #endif

  #if !HAVE_DECL_BUILD_ARRAY_REF
  /* This declaration is missing in GCC 4.6.1.  */
  extern tree build_array_ref (location_t loc, tree array, tree index);
  #endif


  #define YYSTYPE tree
  #define YYLTYPE location_t

  static void
  yyerror (location_t loc, const char *pragma, tree *seq,
	   char const *message)
  {
    error_at (loc, "parse error in pragma %qs: %s", pragma, message);
  }

  /* Return SOMETHING if it's a VAR_DECL, an identifier bound to a VAR_DECL,
     or another object; raise an error otherwise.  */

  static tree
  ensure_bound (location_t loc, tree something)
  {
    gcc_assert (something != NULL_TREE);

    if (DECL_P (something))
      return something;
    else if (TREE_CODE (something) == IDENTIFIER_NODE)
      {
	tree var = lookup_name (something);
	if (var == NULL_TREE)
	  {
	    error_at (loc, "unbound variable %qE", something);
	    return error_mark_node;
	  }
	else
	  return var;
      }

    return something;
  }

  static tree
  build_component_ref (location_t loc, tree what, tree field)
  {
    sorry ("struct field access not implemented yet"); /* XXX */
    return error_mark_node;
  }
%}

%code {
  /* Mapping of libcpp token names to Bison-generated token names.  This is
     not ideal but Bison cannot be told to use the `enum cpp_ttype'
     values.  */
  static const int cpplib_bison_token_map[] =
    {
      [CPP_NAME] = YCPP_NAME,
      [CPP_NUMBER] = YCPP_NUM,
      [CPP_AND] = YCPP_AND,
      [CPP_OPEN_SQUARE] = YCPP_OPEN_SQUARE,
      [CPP_CLOSE_SQUARE] = YCPP_CLOSE_SQUARE,
      [CPP_OPEN_PAREN] = YCPP_OPEN_PAREN,
      [CPP_CLOSE_PAREN] = YCPP_CLOSE_PAREN,
      [CPP_PLUS] = YCPP_PLUS,
      [CPP_MINUS] = YCPP_MINUS,
      [CPP_MULT] = YCPP_MULT,
      [CPP_DIV] = YCPP_DIV,
      [CPP_DOT] = YCPP_DOT,
      [CPP_DEREF] = YCPP_DEREF
    };

  static int
  yylex (YYSTYPE *lvalp)
  {
    int ret;

    ret = pragma_lex (lvalp);
    if (ret < sizeof cpplib_bison_token_map / sizeof cpplib_bison_token_map[0])
      ret = cpplib_bison_token_map[ret];
    else
      ret = -1;

    return ret;
  }
}

%token YCPP_NAME "identifier"
%token YCPP_NUM "integer"
%token YCPP_AND "&"
%token YCPP_OPEN_SQUARE "["
%token YCPP_CLOSE_SQUARE "]"
%token YCPP_OPEN_PAREN "("
%token YCPP_CLOSE_PAREN ")"
%token YCPP_PLUS "+"
%token YCPP_MINUS "-"
%token YCPP_MULT "*"
%token YCPP_DIV "/"
%token YCPP_DOT "."
%token YCPP_DEREF "->"

%% /* Grammar rules.  */

 /* Always return a TREE_LIST rather than a raw chain, because the elements
    of that list may be already chained for other purposes---e.g., PARM_DECLs
    of a function are chained together.  */

sequence: expression {
          gcc_assert (*seq == NULL_TREE);
	  *seq = tree_cons (NULL_TREE, $1, NULL_TREE);
	  $$ = *seq;
      }
      | expression sequence {
	  gcc_assert ($2 == *seq);
	  *seq = tree_cons (NULL_TREE, $1, $2);
	  $$ = *seq;
      }
;

expression: identifier | binary_expression | unary_expression;

/* XXX: `ensure_bound' below leads to errors raised even for non-significant
   arguments---e.g., junk after pragma.  */
identifier: YCPP_NAME  { $$ = ensure_bound (loc, $1); }
;

binary_expression: multiplicative_expression
     | additive_expression
;

multiplicative_expression: multiplicative_expression YCPP_MULT cast_expression {
       $$ = build_binary_op (UNKNOWN_LOCATION, MULT_EXPR, $1, $3, 0);
     }
     | multiplicative_expression YCPP_DIV cast_expression {
       $$ = build_binary_op (UNKNOWN_LOCATION, TRUNC_DIV_EXPR, $1, $3, 0);
     }
     | cast_expression
;

additive_expression: multiplicative_expression
     | additive_expression YCPP_PLUS multiplicative_expression {
       $$ = build_binary_op (UNKNOWN_LOCATION, PLUS_EXPR, $1, $3, 0);
     }
     | additive_expression YCPP_MINUS multiplicative_expression {
       $$ = build_binary_op (UNKNOWN_LOCATION, MINUS_EXPR, $1, $3, 0);
     }
;

cast_expression: unary_expression
		 /* XXX: No support for '(' TYPE-NAME ')' UNARY-EXPRESSION.  */
;

unary_expression:
       primary_expression
     | postfix_expression
     | YCPP_AND cast_expression {
       $$ = build_addr (ensure_bound (loc, $2), current_function_decl);
     }
;

postfix_expression:
       primary_expression
     | postfix_expression YCPP_OPEN_SQUARE expression YCPP_CLOSE_SQUARE {
#if 1
	 /* Build the array ref with proper error checking.  */
	 $$ = build_array_ref (loc, ensure_bound (loc, $1),
			       ensure_bound (loc, $3));
#else /* TIMTOWTDI */
	 $$ = build_indirect_ref (loc,
	       build_binary_op (loc, PLUS_EXPR, ensure_bound (loc, $1), ensure_bound (loc, $3), 0),
		RO_ARRAY_INDEXING);
#endif
     }
     | postfix_expression YCPP_DOT identifier {
        $$ = build_component_ref (loc, ensure_bound (loc, $1), $2);
     }
     | postfix_expression YCPP_DEREF identifier {
        $$ = build_component_ref (loc,
               build_indirect_ref (loc, ensure_bound (loc, $1), RO_ARRAY_INDEXING),
               $2);
     }
;

primary_expression: identifier
     | constant
     | YCPP_OPEN_PAREN expression YCPP_CLOSE_PAREN { $$ = $2; }
;

constant: YCPP_NUM { $$ = $1; }
;

%%
