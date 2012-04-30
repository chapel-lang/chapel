/* GCC-StarPU
   Copyright (C) 2011, 2012 Institut National de Recherche en Informatique et Automatique

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

/* Use extensions of the GNU C Library.  */
#define _GNU_SOURCE 1

#include <starpu-gcc-config.h>

/* #define ENABLE_TREE_CHECKING 1 */

#include <gcc-plugin.h>
#include <plugin-version.h>

#include <plugin.h>
#include <cpplib.h>
#include <tree.h>
#include <tree-iterator.h>
#include <langhooks.h>

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

#include <tm.h>
#include <gimple.h>
#include <tree-pass.h>
#include <tree-flow.h>
#include <cgraph.h>
#include <gimple.h>
#include <toplev.h>

#include <stdio.h>

/* Don't include the dreaded proprietary headers that we don't need anyway.
   In particular, this waives the obligation to reproduce their silly
   disclaimer.  */
#define STARPU_DONT_INCLUDE_CUDA_HEADERS

#include <starpu.h>  /* for `STARPU_CPU' & co.  */


/* GCC 4.7 requires compilation with `g++', and C++ lacks a number of GNU C
   features, so work around that.  */

#ifdef __cplusplus

/* G++ doesn't implement nested functions, so use C++11 lambdas instead.  */

# include <functional>

# define local_define(ret, name, parms)     auto name = [=]parms
# define function_parm(ret, name, parms)    std::function<ret parms> name

/* G++ lacks designated initializers.  */
# define designated_field_init(name, value) value /* XXX: cross fingers */

#else  /* !__cplusplus */

/* GNU C nested functions.  */

# define local_define(ret, name, parms)	    ret name parms
# define function_parm(ret, name, parms)    ret (*name) parms

/* Designated field initializer.  */

# define designated_field_init(name, value) .name = value

#endif	/* !__cplusplus */


/* C expression parser, possibly with C++ linkage.  */

extern int yyparse (location_t, const char *, tree *);
extern int yydebug;


#ifdef __cplusplus
extern "C" {
#endif

/* Declared with `C' linkage in <gcc-plugin.h>.  */
int plugin_is_GPL_compatible;

/* The name of this plug-in.  */
static const char plugin_name[] = "starpu";

/* Whether to enable verbose output.  */
static bool verbose_output_p = false;

/* Names of public attributes.  */
static const char task_attribute_name[] = "task";
static const char task_implementation_attribute_name[] = "task_implementation";
static const char output_attribute_name[] = "output";
static const char heap_allocated_attribute_name[] = "heap_allocated";

/* Names of attributes used internally.  */
static const char task_codelet_attribute_name[] = ".codelet";
static const char task_implementation_list_attribute_name[] =
  ".task_implementation_list";
static const char task_implementation_wrapper_attribute_name[] =
  ".task_implementation_wrapper";
static const char heap_allocated_orig_type_attribute_name[] =
  ".heap_allocated_original_type";

/* Names of data structures defined in <starpu.h>.  */
static const char codelet_struct_name[] = "starpu_codelet_gcc";

/* Cached function declarations.  */
static tree unpack_fn, data_lookup_fn;


/* Forward declarations.  */

static tree build_codelet_declaration (tree task_decl);
static tree build_cpu_codelet_identifier (const_tree task);
static void define_task (tree task_decl);
static tree build_pointer_lookup (tree pointer);

static bool task_p (const_tree decl);
static bool task_implementation_p (const_tree decl);
static tree task_implementation_task (const_tree task_impl);
static bool implicit_cpu_task_implementation_p (const_tree fn);


static int task_implementation_target_to_int (const_tree target);

static bool heap_allocated_p (const_tree var_decl);


/* Lookup the StarPU function NAME in the global scope and store the result
   in VAR (this can't be done from `lower_starpu'.)  */

#define LOOKUP_STARPU_FUNCTION(var, name)				\
  if ((var) == NULL_TREE)						\
    {									\
      (var) = lookup_name (get_identifier (name));			\
      gcc_assert ((var) != NULL_TREE && TREE_CODE (var) == FUNCTION_DECL); \
    }

/* Compile-time assertions.  */

#if STARPU_GNUC_PREREQ (4, 6)
# define verify(cond, msg) _Static_assert ((cond), msg)
#else
# define verify(cond, msg) assert (cond);
#endif


/* Useful code backported from GCC 4.6.  */

#if !HAVE_DECL_BUILD_CALL_EXPR_LOC_ARRAY

static tree
build_call_expr_loc_array (location_t loc, tree fndecl, int n, tree *argarray)
{
  tree fntype = TREE_TYPE (fndecl);
  tree fn = build1 (ADDR_EXPR, build_pointer_type (fntype), fndecl);

  return fold_builtin_call_array (loc, TREE_TYPE (fntype), fn, n, argarray);
}

#endif

#if !HAVE_DECL_BUILD_CALL_EXPR_LOC_VEC

static tree
build_call_expr_loc_vec (location_t loc, tree fndecl, VEC(tree,gc) *vec)
{
  return build_call_expr_loc_array (loc, fndecl, VEC_length (tree, vec),
				    VEC_address (tree, vec));
}

#endif

#if !HAVE_DECL_BUILD_ZERO_CST

static tree
build_zero_cst (tree type)
{
  switch (TREE_CODE (type))
    {
    case INTEGER_TYPE: case ENUMERAL_TYPE: case BOOLEAN_TYPE:
    case POINTER_TYPE: case REFERENCE_TYPE:
    case OFFSET_TYPE:
      return build_int_cst (type, 0);

    default:
      abort ();
    }
}

#endif

#ifndef VEC_qsort

/* This macro is missing in GCC 4.5.  */

# define VEC_qsort(T,V,CMP) qsort(VEC_address (T,V), VEC_length(T,V),	\
				  sizeof (T), CMP)

#endif

#if !HAVE_DECL_BUILTIN_DECL_EXPLICIT

/* This function was introduced in GCC 4.7 as a replacement for the
   `built_in_decls' array.  */

static inline tree
builtin_decl_explicit (enum built_in_function fncode)
{
  return built_in_decls[fncode];
}

#endif


/* Helpers.  */


/* Return POINTER plus OFFSET, where OFFSET is in bytes.  */

static tree
pointer_plus (tree pointer, size_t offset)
{
  gcc_assert (POINTER_TYPE_P (TREE_TYPE (pointer)));

  if (offset == 0)
    return pointer;
  else
    return build_binary_op (UNKNOWN_LOCATION, PLUS_EXPR,
			    pointer,
			    build_int_cstu (integer_type_node, offset),
			    false);
}

/* Build a reference to the INDEXth element of ARRAY.  `build_array_ref' is
   not exported, so we roll our own.
   FIXME: This version may not work for array types and doesn't do as much
   type-checking as `build_array_ref'.  */

static tree
array_ref (tree array, size_t index)
{
  gcc_assert (POINTER_TYPE_P (TREE_TYPE (array)));

  return build_indirect_ref (UNKNOWN_LOCATION,
			     pointer_plus (array, index),
			     RO_ARRAY_INDEXING);
}

/* Return the number of elements of ARRAY_TYPE, or NULL_TREE if ARRAY_TYPE is
   an incomplete type.  */

static tree
array_type_element_count (location_t loc, const_tree array_type)
{
  gcc_assert (TREE_CODE (array_type) == ARRAY_TYPE);

  tree count, domain = TYPE_DOMAIN (array_type);

  if (domain != NULL_TREE)
    {
      count = build_binary_op (loc, MINUS_EXPR,
			       TYPE_MAX_VALUE (domain),
			       TYPE_MIN_VALUE (domain),
			       false);
      count = build_binary_op (loc, PLUS_EXPR,
			       count,
			       build_int_cstu (integer_type_node, 1),
			       false);
      count = fold_convert (size_type_node, count);
    }
  else
    count = NULL_TREE;

  return count;
}

/* Like `build_constructor_from_list', but sort VALS according to their
   offset in struct TYPE.  Inspired by `gnat_build_constructor'.  */

static tree
build_constructor_from_unsorted_list (tree type, tree vals)
{
  local_define (int, compare_elmt_bitpos, (const void *rt1, const void *rt2))
  {
    const constructor_elt *elmt1 = (constructor_elt *) rt1;
    const constructor_elt *elmt2 = (constructor_elt *) rt2;
    const_tree field1 = elmt1->index;
    const_tree field2 = elmt2->index;
    int ret
      = tree_int_cst_compare (bit_position (field1), bit_position (field2));

    return ret ? ret : (int) (DECL_UID (field1) - DECL_UID (field2));
  };

  tree t;
  VEC(constructor_elt,gc) *v = NULL;

  if (vals)
    {
      v = VEC_alloc (constructor_elt, gc, list_length (vals));
      for (t = vals; t; t = TREE_CHAIN (t))
	CONSTRUCTOR_APPEND_ELT (v, TREE_PURPOSE (t), TREE_VALUE (t));
    }

  /* Sort field initializers by field offset.  */
  VEC_qsort (constructor_elt, v, compare_elmt_bitpos);

  return build_constructor (type, v);
}

/* Return true if LST holds the void type.  */

bool
void_type_p (const_tree lst)
{
  gcc_assert (TREE_CODE (lst) == TREE_LIST);
  return VOID_TYPE_P (TREE_VALUE (lst));
}

/* Return true if LST holds a pointer type.  */

bool
pointer_type_p (const_tree lst)
{
  gcc_assert (TREE_CODE (lst) == TREE_LIST);
  return POINTER_TYPE_P (TREE_VALUE (lst));
}


/* Debugging helpers.  */

static tree build_printf (const char *, ...)
  __attribute__ ((format (printf, 1, 2)));

static tree
build_printf (const char *fmt, ...)
{
  tree call;
  char *str;
  va_list args;

  va_start (args, fmt);
  vasprintf (&str, fmt, args);
  call = build_call_expr (builtin_decl_explicit (BUILT_IN_PUTS), 1,
	 		  build_string_literal (strlen (str) + 1, str));
  free (str);
  va_end (args);

  return call;
}

static tree
build_hello_world (void)
{
  return build_printf ("Hello, StarPU!");
}

/* Given ERROR_VAR, an integer variable holding a StarPU error code, return
   statements that print out an error message and abort.  */

static tree build_error_statements (location_t, tree, const char *, ...)
  __attribute__ ((format (printf, 3, 4)));

static tree
build_error_statements (location_t loc, tree error_var, const char *fmt, ...)
{
  expanded_location xloc = expand_location (loc);

  tree print;
  char *str, *fmt_long;
  va_list args;

  va_start (args, fmt);

  /* Build a longer format.  Since FMT itself contains % escapes, this needs
     to be done in two steps.  */

  vasprintf (&str, fmt, args);

  if (error_var != NULL_TREE)
    {
      /* ERROR_VAR is an error code.  */

      static tree strerror_fn;
      LOOKUP_STARPU_FUNCTION (strerror_fn, "strerror");

      gcc_assert (TREE_CODE (error_var) == VAR_DECL
		  && TREE_TYPE (error_var) == integer_type_node);

      asprintf (&fmt_long, "%s:%d: error: %s: %%s\n",
		xloc.file, xloc.line, str);

      tree error_code =
	build1 (NEGATE_EXPR, TREE_TYPE (error_var), error_var);
      print =
	build_call_expr (builtin_decl_explicit (BUILT_IN_PRINTF), 2,
			 build_string_literal (strlen (fmt_long) + 1,
					       fmt_long),
			 build_call_expr (strerror_fn, 1, error_code));
    }
  else
    {
      /* No error code provided.  */

      asprintf (&fmt_long, "%s:%d: error: %s\n",
		xloc.file, xloc.line, str);

      print =
	build_call_expr (builtin_decl_explicit (BUILT_IN_PUTS), 1,
			 build_string_literal (strlen (fmt_long) + 1,
					       fmt_long));
    }

  free (fmt_long);
  free (str);
  va_end (args);

  tree stmts = NULL;
  append_to_statement_list (print, &stmts);
  append_to_statement_list (build_call_expr
			    (builtin_decl_explicit (BUILT_IN_ABORT), 0),
			    &stmts);

  return stmts;
}


/* List and vector utilities, à la SRFI-1.  */

static tree chain_trees (tree t, ...)
  __attribute__ ((sentinel));

static tree
chain_trees (tree t, ...)
{
  va_list args;

  va_start (args, t);

  tree next, prev = t;
  for (prev = t, next = va_arg (args, tree);
       next != NULL_TREE;
       prev = next, next = va_arg (args, tree))
    TREE_CHAIN (prev) = next;

  va_end (args);

  return t;
}

static tree
filter (function_parm (bool, pred, (const_tree)), tree t)
{
  tree result, lst;

  gcc_assert (TREE_CODE (t) == TREE_LIST);

  result = NULL_TREE;
  for (lst = t; lst != NULL_TREE; lst = TREE_CHAIN (lst))
    {
      if (pred (lst))
	result = tree_cons (TREE_PURPOSE (lst), TREE_VALUE (lst),
			    result);
    }

  return nreverse (result);
}

static tree
list_remove (function_parm (bool, pred, (const_tree)), tree t)
{
  local_define (bool, opposite, (const_tree t))
  {
    return !pred (t);
  };

  return filter (opposite, t);
}

/* Map FUNC over chain T.  T does not have to be `TREE_LIST'; it can be a
   chain of arbitrary tree objects.  */

static tree
map (function_parm (tree, func, (const_tree)), tree t)
{
  tree result, tail, lst;

  result = tail = NULL_TREE;
  for (lst = t; lst != NULL_TREE; lst = TREE_CHAIN (lst))
    {
      tree r = func (lst);
      if (tail != NULL_TREE)
	TREE_CHAIN (tail) = r;
      else
	result = r;

      tail = r;
    }

  return result;
}

static void
for_each (function_parm (void, func, (tree)), tree t)
{
  tree lst;

  gcc_assert (TREE_CODE (t) == TREE_LIST);

  for (lst = t; lst != NULL_TREE; lst = TREE_CHAIN (lst))
    func (TREE_VALUE (lst));
}

static size_t
count (function_parm (bool, pred, (const_tree)), const_tree t)
{
  size_t result;
  const_tree lst;

  for (lst = t, result = 0; lst != NULL_TREE; lst = TREE_CHAIN (lst))
    if (pred (lst))
      result++;

  return result;
}


/* Pragmas.  */

#define STARPU_PRAGMA_NAME_SPACE "starpu"

static void
handle_pragma_hello (struct cpp_reader *reader)
{
  add_stmt (build_hello_world ());
}

/* Process `#pragma starpu initialize'.
   TODO: Parse and initialize some of the fields of `starpu_conf'.  */

static void
handle_pragma_initialize (struct cpp_reader *reader)
{
  static tree init_fn;
  LOOKUP_STARPU_FUNCTION (init_fn, "starpu_init");

  location_t loc = cpp_peek_token (reader, 0)->src_loc;

  /* Call `starpu_init (NULL)'.  */
  tree init = build_call_expr (init_fn, 1, build_zero_cst (ptr_type_node));

  /* Introduce a local variable to hold the error code.  */

  tree error_var = build_decl (loc, VAR_DECL,
  			       create_tmp_var_name (".initialize_error"),
  			       integer_type_node);
  DECL_CONTEXT (error_var) = current_function_decl;
  DECL_ARTIFICIAL (error_var) = true;

  tree assignment = build2 (INIT_EXPR, TREE_TYPE (error_var),
			    error_var, init);

  tree cond = build3 (COND_EXPR, void_type_node,
		      build2 (NE_EXPR, boolean_type_node,
			      error_var, integer_zero_node),
		      build_error_statements (loc, error_var,
					      "failed to initialize StarPU"), 
		      NULL_TREE);

  tree stmts = NULL_TREE;
  append_to_statement_list (assignment, &stmts);
  append_to_statement_list (cond, &stmts);

  tree bind = build3 (BIND_EXPR, void_type_node, error_var, stmts,
  		      NULL_TREE);

  add_stmt (bind);
}

/* Process `#pragma starpu shutdown'.  */

static void
handle_pragma_shutdown (struct cpp_reader *reader)
{
  static tree shutdown_fn;
  LOOKUP_STARPU_FUNCTION (shutdown_fn, "starpu_shutdown");

  tree token;
  if (pragma_lex (&token) != CPP_EOF)
    error_at (cpp_peek_token (reader, 0)->src_loc,
	      "junk after %<starpu shutdown%> pragma");
  else
    /* Call `starpu_shutdown ()'.  */
    add_stmt (build_call_expr (shutdown_fn, 0));
}

static void
handle_pragma_wait (struct cpp_reader *reader)
{
  if (task_implementation_p (current_function_decl))
    {
      location_t loc;

      loc = cpp_peek_token (reader, 0)->src_loc;

      /* TODO: In the future we could generate a task for the continuation
	 and have it depend on what's before here.  */
      error_at (loc, "task implementation is not allowed to wait");
    }
  else
    {
      tree fndecl;

      fndecl = lookup_name (get_identifier ("starpu_task_wait_for_all"));
      gcc_assert (TREE_CODE (fndecl) == FUNCTION_DECL);

      add_stmt (build_call_expr (fndecl, 0));
    }
}

/* The minimal C expression parser.  */

/* Parse expressions from the CPP reader for PRAGMA, which is located at LOC.
   Return a TREE_LIST of C expressions.  */

static tree
read_pragma_expressions (const char *pragma, location_t loc)
{
  tree expr = NULL_TREE;

  if (yyparse (loc, pragma, &expr))
    /* Parse error or memory exhaustion.  */
    expr = NULL_TREE;

  return expr;
}

/* Build a `starpu_vector_data_register' call for the COUNT elements pointed
   to by POINTER.  */

static tree
build_data_register_call (location_t loc, tree pointer, tree count)
{
  tree pointer_type = TREE_TYPE (pointer);

  gcc_assert ((TREE_CODE (pointer_type)  == ARRAY_TYPE
	       && TYPE_DOMAIN (pointer_type) != NULL_TREE)
	      || POINTER_TYPE_P (pointer_type));
  gcc_assert (INTEGRAL_TYPE_P (TREE_TYPE (count)));

  static tree register_fn;
  LOOKUP_STARPU_FUNCTION (register_fn, "starpu_vector_data_register");

  /* Introduce a local variable to hold the handle.  */

  tree handle_var = build_decl (loc, VAR_DECL, create_tmp_var_name (".handle"),
				ptr_type_node);
  DECL_CONTEXT (handle_var) = current_function_decl;
  DECL_ARTIFICIAL (handle_var) = true;
  DECL_INITIAL (handle_var) = NULL_TREE;

  /* If PTR is an array, take its address.  */
  tree actual_pointer =
    POINTER_TYPE_P (pointer_type)
    ? pointer
    : build_addr (pointer, current_function_decl);

  /* Build `starpu_vector_data_register (&HANDLE_VAR, 0, POINTER,
                                         COUNT, sizeof *POINTER)'  */
  tree call =
    build_call_expr (register_fn, 5,
		     build_addr (handle_var, current_function_decl),
		     build_zero_cst (uintptr_type_node), /* home node */
		     actual_pointer, count,
		     size_in_bytes (TREE_TYPE (pointer_type)));

  return build3 (BIND_EXPR, void_type_node, handle_var, call,
		 NULL_TREE);
}

/* Return a `starpu_data_unregister' call for VAR.  */

static tree
build_data_unregister_call (location_t loc, tree var)
{
  static tree unregister_fn;
  LOOKUP_STARPU_FUNCTION (unregister_fn, "starpu_data_unregister");

  /* If VAR is an array, take its address.  */
  tree pointer =
    POINTER_TYPE_P (TREE_TYPE (var))
    ? var
    : build_addr (var, current_function_decl);

  /* Call `starpu_data_unregister (starpu_data_lookup (ptr))'.  */
  return build_call_expr (unregister_fn, 1,
			  build_pointer_lookup (pointer));
}


/* Process `#pragma starpu register VAR [COUNT]' and emit the corresponding
   `starpu_vector_data_register' call.  */

static void
handle_pragma_register (struct cpp_reader *reader)
{
  tree args, ptr, count_arg;
  location_t loc;

  loc = cpp_peek_token (reader, 0)->src_loc;

  args = read_pragma_expressions ("register", loc);
  if (args == NULL_TREE)
    /* Parse error, presumably already handled by the parser.  */
    return;

  /* First argument should be a pointer expression.  */
  ptr = TREE_VALUE (args);
  args = TREE_CHAIN (args);

  if (ptr == error_mark_node)
    return;

  tree ptr_type;

  if (DECL_P (ptr))
    {
      tree heap_attr =
	lookup_attribute (heap_allocated_orig_type_attribute_name,
			  DECL_ATTRIBUTES (ptr));

      if (heap_attr != NULL_TREE)
	/* PTR is `heap_allocated' so use its original array type to
	   determine its size.  */
	ptr_type = TREE_VALUE (heap_attr);
      else
	ptr_type = TREE_TYPE (ptr);
    }
  else
    ptr_type = TREE_TYPE (ptr);

  if (!POINTER_TYPE_P (ptr_type)
      && TREE_CODE (ptr_type) != ARRAY_TYPE)
    {
      error_at (loc, "%qE is neither a pointer nor an array", ptr);
      return;
    }

  /* Since we implicitly use sizeof (*PTR), `void *' is not allowed. */
  if (VOID_TYPE_P (TREE_TYPE (ptr_type)))
    {
      error_at (loc, "pointers to %<void%> not allowed "
		"in %<register%> pragma");
      return;
    }

  TREE_USED (ptr) = true;
#ifdef DECL_READ_P
  if (DECL_P (ptr))
    DECL_READ_P (ptr) = true;
#endif

  if (TREE_CODE (ptr_type) == ARRAY_TYPE
      && !DECL_EXTERNAL (ptr)
      && !TREE_STATIC (ptr)
      && !(TREE_CODE (ptr) == VAR_DECL && heap_allocated_p (ptr))
      && !MAIN_NAME_P (DECL_NAME (current_function_decl)))
    warning_at (loc, 0, "using an on-stack array as a task input "
		"considered unsafe");

  /* Determine the number of elements in the vector.  */
  tree count = NULL_TREE;

  if (TREE_CODE (ptr_type) == ARRAY_TYPE)
    count = array_type_element_count (loc, ptr_type);

  /* Second argument is optional but should be an integer.  */
  count_arg = (args == NULL_TREE) ? NULL_TREE : TREE_VALUE (args);
  if (args != NULL_TREE)
    args = TREE_CHAIN (args);

  if (count_arg == NULL_TREE)
    {
      /* End of line reached: check whether the array size was
	 determined.  */
      if (count == NULL_TREE)
	{
	  error_at (loc, "cannot determine size of array %qE", ptr);
	  return;
	}
    }
  else if (count_arg == error_mark_node)
    /* COUNT_ARG could not be parsed and an error was already reported.  */
    return;
  else if (!INTEGRAL_TYPE_P (TREE_TYPE (count_arg)))
    {
      error_at (loc, "%qE is not an integer", count_arg);
      return;
    }
  else
    {
      TREE_USED (count_arg) = true;
#ifdef DECL_READ_P
      if (DECL_P (count_arg))
	DECL_READ_P (count_arg) = true;
#endif

      if (count != NULL_TREE)
	{
	  /* The number of elements of this array was already determined.  */
	  inform (loc,
		  "element count can be omitted for bounded array %qE",
		  ptr);

	  if (count_arg != NULL_TREE)
	    {
	      if (TREE_CODE (count_arg) == INTEGER_CST)
		{
		  if (!tree_int_cst_equal (count, count_arg))
		    error_at (loc, "specified element count differs "
			      "from actual size of array %qE",
			      ptr);
		}
	      else
		/* Using a variable to determine the array size whereas the
		   array size is actually known statically.  This looks like
		   unreasonable code, so error out.  */
		error_at (loc, "determining array size at run-time "
			  "although array size is known at compile-time");
	    }
	}
      else
	count = count_arg;
    }

  /* Any remaining args?  */
  if (args != NULL_TREE)
    error_at (loc, "junk after %<starpu register%> pragma");

  /* Add a data register call.  */
  add_stmt (build_data_register_call (loc, ptr, count));
}

/* Process `#pragma starpu acquire VAR' and emit the corresponding
   `starpu_data_acquire' call.  */

static void
handle_pragma_acquire (struct cpp_reader *reader)
{
  static tree acquire_fn;
  LOOKUP_STARPU_FUNCTION (acquire_fn, "starpu_data_acquire");

  tree args, var;
  location_t loc;

  loc = cpp_peek_token (reader, 0)->src_loc;

  args = read_pragma_expressions ("acquire", loc);
  if (args == NULL_TREE)
    return;

  var = TREE_VALUE (args);

  if (var == error_mark_node)
    return;
  else if (TREE_CODE (TREE_TYPE (var)) != POINTER_TYPE
	   && TREE_CODE (TREE_TYPE (var)) != ARRAY_TYPE)
    {
      error_at (loc, "%qE is neither a pointer nor an array", var);
      return;
    }
  else if (TREE_CHAIN (args) != NULL_TREE)
    error_at (loc, "junk after %<starpu acquire%> pragma");

  /* If VAR is an array, take its address.  */
  tree pointer =
    POINTER_TYPE_P (TREE_TYPE (var))
    ? var
    : build_addr (var, current_function_decl);

  /* Call `starpu_data_acquire (starpu_data_lookup (ptr), STARPU_RW)'.
     TODO: Support modes other than RW.  */
  add_stmt (build_call_expr (acquire_fn, 2,
			     build_pointer_lookup (pointer),
			     build_int_cst (integer_type_node, STARPU_RW)));
}

/* Process `#pragma starpu release VAR' and emit the corresponding
   `starpu_data_release' call.  */

static void
handle_pragma_release (struct cpp_reader *reader)
{
  static tree release_fn;
  LOOKUP_STARPU_FUNCTION (release_fn, "starpu_data_release");

  tree args, var;
  location_t loc;

  loc = cpp_peek_token (reader, 0)->src_loc;

  args = read_pragma_expressions ("release", loc);
  if (args == NULL_TREE)
    return;

  var = TREE_VALUE (args);

  if (var == error_mark_node)
    return;
  else if (TREE_CODE (TREE_TYPE (var)) != POINTER_TYPE
	   && TREE_CODE (TREE_TYPE (var)) != ARRAY_TYPE)
    {
      error_at (loc, "%qE is neither a pointer nor an array", var);
      return;
    }
  else if (TREE_CHAIN (args) != NULL_TREE)
    error_at (loc, "junk after %<starpu release%> pragma");

  /* If VAR is an array, take its address.  */
  tree pointer =
    POINTER_TYPE_P (TREE_TYPE (var))
    ? var
    : build_addr (var, current_function_decl);

  /* Call `starpu_data_release (starpu_data_lookup (ptr))'.  */
  add_stmt (build_call_expr (release_fn, 1,
			     build_pointer_lookup (pointer)));
}

/* Process `#pragma starpu unregister VAR' and emit the corresponding
   `starpu_data_unregister' call.  */

static void
handle_pragma_unregister (struct cpp_reader *reader)
{
  tree args, var;
  location_t loc;

  loc = cpp_peek_token (reader, 0)->src_loc;

  args = read_pragma_expressions ("unregister", loc);
  if (args == NULL_TREE)
    return;

  var = TREE_VALUE (args);

  if (var == error_mark_node)
    return;
  else if (TREE_CODE (TREE_TYPE (var)) != POINTER_TYPE
	   && TREE_CODE (TREE_TYPE (var)) != ARRAY_TYPE)
    {
      error_at (loc, "%qE is neither a pointer nor an array", var);
      return;
    }
  else if (TREE_CHAIN (args) != NULL_TREE)
    error_at (loc, "junk after %<starpu unregister%> pragma");

  add_stmt (build_data_unregister_call (loc, var));
}

/* Handle the `debug_tree' pragma (for debugging purposes.)  */

static void
handle_pragma_debug_tree (struct cpp_reader *reader)
{
  tree args, obj;
  location_t loc;

  loc = cpp_peek_token (reader, 0)->src_loc;

  args = read_pragma_expressions ("debug_tree", loc);
  if (args == NULL_TREE)
    /* Parse error, presumably already handled by the parser.  */
    return;

  obj = TREE_VALUE (args);
  args = TREE_CHAIN (args);

  if (obj == error_mark_node)
    return;

  if (args != NULL_TREE)
    warning_at (loc, 0, "extraneous arguments ignored");

  inform (loc, "debug_tree:");
  debug_tree (obj);
  printf ("\n");
}

static void
register_pragmas (void *gcc_data, void *user_data)
{
  c_register_pragma (STARPU_PRAGMA_NAME_SPACE, "hello",
		     handle_pragma_hello);
  c_register_pragma (STARPU_PRAGMA_NAME_SPACE, "debug_tree",
		     handle_pragma_debug_tree);

  c_register_pragma_with_expansion (STARPU_PRAGMA_NAME_SPACE, "initialize",
				    handle_pragma_initialize);
  c_register_pragma (STARPU_PRAGMA_NAME_SPACE, "wait",
		     handle_pragma_wait);
  c_register_pragma_with_expansion (STARPU_PRAGMA_NAME_SPACE, "register",
				    handle_pragma_register);
  c_register_pragma_with_expansion (STARPU_PRAGMA_NAME_SPACE, "acquire",
				    handle_pragma_acquire);
  c_register_pragma_with_expansion (STARPU_PRAGMA_NAME_SPACE, "release",
				    handle_pragma_release);
  c_register_pragma_with_expansion (STARPU_PRAGMA_NAME_SPACE, "unregister",
				    handle_pragma_unregister);
  c_register_pragma (STARPU_PRAGMA_NAME_SPACE, "shutdown",
		     handle_pragma_shutdown);
}


/* Attributes.  */


/* Turn FN into a task, and push its associated codelet declaration.  */

static void
taskify_function (tree fn)
{
  gcc_assert (TREE_CODE (fn) == FUNCTION_DECL);

  /* Add a `task' attribute and an empty `task_implementation_list'
     attribute.  */
  DECL_ATTRIBUTES (fn) =
    tree_cons (get_identifier (task_implementation_list_attribute_name),
	       NULL_TREE,
	       tree_cons (get_identifier (task_attribute_name), NULL_TREE,
			  DECL_ATTRIBUTES (fn)));

  /* Push a declaration for the corresponding `struct starpu_codelet' object and
     add it as an attribute of FN.  */
  tree cl = build_codelet_declaration (fn);
  DECL_ATTRIBUTES (fn) =
    tree_cons (get_identifier (task_codelet_attribute_name), cl,
	       DECL_ATTRIBUTES (fn));

  pushdecl (cl);
}

/* Handle the `task' function attribute.  */

static tree
handle_task_attribute (tree *node, tree name, tree args,
		       int flags, bool *no_add_attrs)
{
  tree fn;

  fn = *node;

  /* Get rid of the `task' attribute by default so that FN isn't further
     processed when it's erroneous.  */
  *no_add_attrs = true;

  if (TREE_CODE (fn) != FUNCTION_DECL)
    error_at (DECL_SOURCE_LOCATION (fn),
	      "%<task%> attribute only applies to functions");
  else
    {
      if (!VOID_TYPE_P (TREE_TYPE (TREE_TYPE (fn))))
	/* Raise an error but keep going to avoid spitting out too many
	   errors at the user's face.  */
	error_at (DECL_SOURCE_LOCATION (fn),
		  "task return type must be %<void%>");

      if (count (pointer_type_p, TYPE_ARG_TYPES (TREE_TYPE (fn)))
	  > STARPU_NMAXBUFS)
	error_at (DECL_SOURCE_LOCATION (fn),
		  "maximum number of pointer parameters exceeded");

      /* Turn FN into an actual task.  */
      taskify_function (fn);
    }

  /* Lookup & cache function declarations for later reuse.  */
  LOOKUP_STARPU_FUNCTION (unpack_fn, "starpu_codelet_unpack_args");
  LOOKUP_STARPU_FUNCTION (data_lookup_fn, "starpu_data_lookup");

  return NULL_TREE;
}

/* Diagnose use of C types that are either nonexistent or different in
   OpenCL.  */

static void
validate_opencl_argument_type (location_t loc, const_tree type)
{
  /* When TYPE is a pointer type, get to the base element type.  */
  for (; POINTER_TYPE_P (type); type = TREE_TYPE (type));

  if (!RECORD_OR_UNION_TYPE_P (type) && !VOID_TYPE_P (type))
    {
      tree decl = TYPE_NAME (type);

      if (DECL_P (decl))
	{
	  static const struct { const char *c; const char *cl; }
	  type_map[] =
	    {
	      /* Scalar types defined in OpenCL 1.2.  See
		 <http://www.khronos.org/files/opencl-1-2-quick-reference-card.pdf>.  */
	      { "char", "cl_char" },
	      { "signed char", "cl_char" },
	      { "unsigned char", "cl_uchar" },
	      { "uchar", "cl_uchar" },
	      { "short int", "cl_short" },
	      { "unsigned short", "cl_ushort" },
	      { "int", "cl_int" },
	      { "unsigned int", "cl_uint" },
	      { "uint", "cl_uint" },
	      { "long int", "cl_long" },
	      { "long unsigned int", "cl_ulong" },
	      { "ulong", "cl_ulong" },
	      { "float", "cl_float" },
	      { "double", "cl_double" },
	      { NULL, NULL }
	    };

	  const char *c_name = IDENTIFIER_POINTER (DECL_NAME (decl));
	  const char *cl_name =
	    ({
	      size_t i;
	      for (i = 0; type_map[i].c != NULL; i++)
		{
		  if (strcmp (type_map[i].c, c_name) == 0)
		    break;
		}
	      type_map[i].cl;
	    });

	  if (cl_name != NULL)
	    {
	      tree cl_type = lookup_name (get_identifier (cl_name));

	      if (cl_type != NULL_TREE)
		{
		  if (DECL_P (cl_type))
		    cl_type = TREE_TYPE (cl_type);

		  if (!lang_hooks.types_compatible_p ((tree) type, cl_type))
		    {
		      tree st, sclt;

		      st = c_common_signed_type ((tree) type);
		      sclt = c_common_signed_type (cl_type);

		      if (st == sclt)
			warning_at (loc, 0, "C type %qE differs in signedness "
				    "from the same-named OpenCL type",
				    DECL_NAME (decl));
		      else
			/* TYPE should be avoided because the it differs from
			   CL_TYPE, and thus cannot be used safely in
			   `clSetKernelArg'.  */
			warning_at (loc, 0, "C type %qE differs from the "
				    "same-named OpenCL type",
				    DECL_NAME (decl));
		    }
		}

	      /* Otherwise we can't conclude.  It could be that <CL/cl.h>
		 wasn't included in the program, for instance.  */
	    }
	  else
	    /* Recommend against use of `size_t', etc.  */
	    warning_at (loc, 0, "%qE does not correspond to a known "
			"OpenCL type", DECL_NAME (decl));
	}
    }
}

/* Add FN to the list of implementations of TASK_DECL.  */

static void
add_task_implementation (tree task_decl, tree fn, const_tree where)
{
  location_t loc;
  tree attr, impls;

  attr = lookup_attribute (task_implementation_list_attribute_name,
			   DECL_ATTRIBUTES (task_decl));
  gcc_assert (attr != NULL_TREE);

  gcc_assert (TREE_CODE (where) == STRING_CST);

  loc = DECL_SOURCE_LOCATION (fn);

  impls = tree_cons (NULL_TREE, fn, TREE_VALUE (attr));
  TREE_VALUE (attr) = impls;

  TREE_USED (fn) = true;

  /* Check the `where' argument to raise a warning if needed.  */
  if (task_implementation_target_to_int (where) == 0)
    warning_at (loc, 0,
		"unsupported target %E; task implementation won't be used",
		where);
  else if (task_implementation_target_to_int (where) == STARPU_OPENCL)
    {
      local_define (void, validate, (tree t))
	{
	  validate_opencl_argument_type (loc, t);
	};

      for_each (validate, TYPE_ARG_TYPES (TREE_TYPE (fn)));
    }
}

/* Handle the `task_implementation (WHERE, TASK)' attribute.  WHERE is a
   string constant ("cpu", "cuda", etc.), and TASK is the identifier of a
   function declared with the `task' attribute.  */

static tree
handle_task_implementation_attribute (tree *node, tree name, tree args,
				      int flags, bool *no_add_attrs)
{
  location_t loc;
  tree fn, where, task_decl;

  /* FIXME:TODO: To change the order to (TASK, WHERE):
	  tree cleanup_id = TREE_VALUE (TREE_VALUE (attr));
	  tree cleanup_decl = lookup_name (cleanup_id);
  */

  fn = *node;
  where = TREE_VALUE (args);
  task_decl = TREE_VALUE (TREE_CHAIN (args));

  if (implicit_cpu_task_implementation_p (task_decl))
    /* TASK_DECL is actually a CPU implementation.  Implicit CPU task
       implementations can lead to this situation, because the task is
       renamed and modified to become a CPU implementation.  */
    task_decl = task_implementation_task (task_decl);

  loc = DECL_SOURCE_LOCATION (fn);

  /* Get rid of the `task_implementation' attribute by default so that FN
     isn't further processed when it's erroneous.  */
  *no_add_attrs = true;

  /* Mark FN as used to placate `-Wunused-function' when FN is erroneous
     anyway.  */
  TREE_USED (fn) = true;

  if (TREE_CODE (fn) != FUNCTION_DECL)
    error_at (loc,
	      "%<task_implementation%> attribute only applies to functions");
  else if (TREE_CODE (where) != STRING_CST)
    error_at (loc, "string constant expected "
	      "as the first %<task_implementation%> argument");
  else if (TREE_CODE (task_decl) != FUNCTION_DECL)
    error_at (loc, "%qE is not a function", task_decl);
  else if (lookup_attribute (task_attribute_name,
			DECL_ATTRIBUTES (task_decl)) == NULL_TREE)
    error_at (loc, "function %qE lacks the %<task%> attribute",
	      DECL_NAME (task_decl));
  else if (TYPE_CANONICAL (TREE_TYPE (fn))
	   != TYPE_CANONICAL (TREE_TYPE (task_decl)))
    error_at (loc, "type differs from that of task %qE",
	      DECL_NAME (task_decl));
  else
    {
      /* Add FN to the list of implementations of TASK_DECL.  */
      add_task_implementation (task_decl, fn, where);

      /* Keep the attribute.  */
      *no_add_attrs = false;
    }

  return NULL_TREE;
}

/* Return true when VAR is an automatic variable with complete array type;
   otherwise, return false, and emit error messages mentioning ATTRIBUTE.  */

static bool
automatic_array_variable_p (const char *attribute, tree var)
{
  gcc_assert (TREE_CODE (var) == VAR_DECL);

  location_t loc;

  loc = DECL_SOURCE_LOCATION (var);

  if (DECL_EXTERNAL (var))
    error_at (loc, "attribute %qs cannot be used on external declarations",
	      attribute);
  else if (TREE_PUBLIC (var) || TREE_STATIC (var))
    {
      error_at (loc, "attribute %qs cannot be used on global variables",
		attribute);
      TREE_TYPE (var) = error_mark_node;
    }
  else if (TREE_CODE (TREE_TYPE (var)) != ARRAY_TYPE)
    {
      error_at (loc, "variable %qE must have an array type",
		DECL_NAME (var));
      TREE_TYPE (var) = error_mark_node;
    }
  else if (TYPE_SIZE (TREE_TYPE (var)) == NULL_TREE)
    {
      error_at (loc, "variable %qE has an incomplete array type",
		DECL_NAME (var));
      TREE_TYPE (var) = error_mark_node;
    }
  else
    return true;

  return false;
}

/* Handle the `heap_allocated' attribute on variable *NODE.  */

static tree
handle_heap_allocated_attribute (tree *node, tree name, tree args,
				 int flags, bool *no_add_attrs)
{
  tree var = *node;

  if (automatic_array_variable_p (heap_allocated_attribute_name, var))
    {
      /* Turn VAR into a pointer that feels like an array.  This is what's
	 done for PARM_DECLs that have an array type.  */

      tree array_type = TREE_TYPE (var);
      tree element_type = TREE_TYPE (array_type);
      tree pointer_type = build_pointer_type (element_type);

      /* Keep a copy of VAR's original type.  */
      DECL_ATTRIBUTES (var) =
	tree_cons (get_identifier (heap_allocated_orig_type_attribute_name),
		   array_type, DECL_ATTRIBUTES (var));

      TREE_TYPE (var) = pointer_type;
      DECL_SIZE (var) = TYPE_SIZE (pointer_type);
      DECL_SIZE_UNIT (var) = TYPE_SIZE_UNIT (pointer_type);
      DECL_ALIGN (var) = TYPE_ALIGN (pointer_type);
      DECL_USER_ALIGN (var) = false;
      DECL_MODE (var) = TYPE_MODE (pointer_type);

      tree malloc_fn = lookup_name (get_identifier ("starpu_malloc"));
      gcc_assert (malloc_fn != NULL_TREE);

      tree alloc = build_call_expr (malloc_fn, 2,
				    build_addr (var, current_function_decl),
				    TYPE_SIZE_UNIT (array_type));
      TREE_SIDE_EFFECTS (alloc) = true;
      add_stmt (alloc);

      /* Add a destructor for VAR.  Instead of consing the `cleanup'
	 attribute for VAR, directly use `push_cleanup'.  This guarantees
	 that CLEANUP_ID is looked up in the right context, and allows us to
	 pass VAR directly to `starpu_free', instead of `&VAR'.

	 TODO: Provide a way to disable this.  */

      static tree cleanup_decl;
      LOOKUP_STARPU_FUNCTION (cleanup_decl, "starpu_free");

      push_cleanup (var, build_call_expr (cleanup_decl, 1, var), false);
    }

  return NULL_TREE;
}

/* Handle the `output' attribute on type *NODE, which should be the type of a
   PARM_DECL of a task or task implementation.  */

static tree
handle_output_attribute (tree *node, tree name, tree args,
			 int flags, bool *no_add_attrs)
{
  tree type = *node;

  gcc_assert (TYPE_P (type));

  if (!POINTER_TYPE_P (type) && TREE_CODE (type) != ARRAY_TYPE)
    error ("%<output%> attribute not allowed for non-pointer types");
  else
    /* Keep the attribute.  */
    *no_add_attrs = false;

  return NULL_TREE;
}


/* Return the declaration of the `struct starpu_codelet' variable associated with
   TASK_DECL.  */

static tree
task_codelet_declaration (const_tree task_decl)
{
  tree cl_attr;

  cl_attr = lookup_attribute (task_codelet_attribute_name,
			      DECL_ATTRIBUTES (task_decl));
  gcc_assert (cl_attr != NULL_TREE);

  return TREE_VALUE (cl_attr);
}

/* Return true if DECL is a task.  */

static bool
task_p (const_tree decl)
{
  return (TREE_CODE (decl) == FUNCTION_DECL &&
	  lookup_attribute (task_attribute_name,
			    DECL_ATTRIBUTES (decl)) != NULL_TREE);
}

/* Return true if DECL is a task implementation.  */

static bool
task_implementation_p (const_tree decl)
{
  return (TREE_CODE (decl) == FUNCTION_DECL &&
	  lookup_attribute (task_implementation_attribute_name,
			    DECL_ATTRIBUTES (decl)) != NULL_TREE);
}

/* Return the list of implementations of TASK_DECL.  */

static tree
task_implementation_list (const_tree task_decl)
{
  tree attr;

  attr = lookup_attribute (task_implementation_list_attribute_name,
			   DECL_ATTRIBUTES (task_decl));
  return TREE_VALUE (attr);
}

/* Return the list of pointer parameter types of TASK_DECL.  */

static tree
task_pointer_parameter_types (const_tree task_decl)
{
  return filter (pointer_type_p, TYPE_ARG_TYPES (TREE_TYPE (task_decl)));
}

/* Return the StarPU integer constant corresponding to string TARGET.  */

static int
task_implementation_target_to_int (const_tree target)
{
  gcc_assert (TREE_CODE (target) == STRING_CST);

  int where_int;

  if (!strncmp (TREE_STRING_POINTER (target), "cpu",
		TREE_STRING_LENGTH (target)))
    where_int = STARPU_CPU;
  else if (!strncmp (TREE_STRING_POINTER (target), "opencl",
		     TREE_STRING_LENGTH (target)))
    where_int = STARPU_OPENCL;
  else if (!strncmp (TREE_STRING_POINTER (target), "cuda",
		     TREE_STRING_LENGTH (target)))
    where_int = STARPU_CUDA;
  else if (!strncmp (TREE_STRING_POINTER (target), "gordon",
		     TREE_STRING_LENGTH (target)))
    where_int = STARPU_GORDON;
  else
    where_int = 0;

  return where_int;
}

/* Return a value indicating where TASK_IMPL should execute (`STARPU_CPU',
   `STARPU_CUDA', etc.).  */

static int
task_implementation_where (const_tree task_impl)
{
  tree impl_attr, args, where;

  gcc_assert (TREE_CODE (task_impl) == FUNCTION_DECL);

  impl_attr = lookup_attribute (task_implementation_attribute_name,
				DECL_ATTRIBUTES (task_impl));
  gcc_assert (impl_attr != NULL_TREE);

  args = TREE_VALUE (impl_attr);
  where = TREE_VALUE (args);

  return task_implementation_target_to_int (where);
}

/* Return a bitwise-or of the supported targets of TASK_DECL.  */

static int
task_where (const_tree task_decl)
{
  gcc_assert (task_p (task_decl));

  int where;
  const_tree impl;

  for (impl = task_implementation_list (task_decl), where = 0;
       impl != NULL_TREE;
       impl = TREE_CHAIN (impl))
    where |= task_implementation_where (TREE_VALUE (impl));

  return where;
}

/* Return the task implemented by TASK_IMPL.  */

static tree
task_implementation_task (const_tree task_impl)
{
  tree impl_attr, args, task;

  gcc_assert (TREE_CODE (task_impl) == FUNCTION_DECL);

  impl_attr = lookup_attribute (task_implementation_attribute_name,
				DECL_ATTRIBUTES (task_impl));
  gcc_assert (impl_attr != NULL_TREE);

  args = TREE_VALUE (impl_attr);

  task = TREE_VALUE (TREE_CHAIN (args));
  if (task_implementation_p (task))
    /* TASK is an implicit CPU task implementation, so return its real
       task.  */
    return task_implementation_task (task);

  return task;
}

/* Return the FUNCTION_DECL of the wrapper generated for TASK_IMPL.  */

static tree
task_implementation_wrapper (const_tree task_impl)
{
  tree attr;

  gcc_assert (TREE_CODE (task_impl) == FUNCTION_DECL);

  attr = lookup_attribute (task_implementation_wrapper_attribute_name,
			   DECL_ATTRIBUTES (task_impl));
  gcc_assert (attr != NULL_TREE);

  return TREE_VALUE (attr);
}

/* Return true when FN is an implicit CPU task implementation.  */

static bool
implicit_cpu_task_implementation_p (const_tree fn)
{
  if (task_implementation_p (fn)
      && task_implementation_where (fn) == STARPU_CPU)
    {
      /* XXX: Hackish heuristic.  */
      const_tree cpu_id;
      cpu_id = build_cpu_codelet_identifier (task_implementation_task (fn));
      return cpu_id == DECL_NAME (fn);
    }

  return false;
}

/* Return true when VAR_DECL has the `heap_allocated' attribute.  */

static bool
heap_allocated_p (const_tree var_decl)
{
  gcc_assert (TREE_CODE (var_decl) == VAR_DECL);

  return lookup_attribute (heap_allocated_attribute_name,
			   DECL_ATTRIBUTES (var_decl)) != NULL_TREE;
}

/* Return true if TYPE is `output'-qualified.  */

static bool
output_type_p (const_tree type)
{
  return (lookup_attribute (output_attribute_name,
			    TYPE_ATTRIBUTES (type)) != NULL_TREE);
}

/* Return the access mode for POINTER, a PARM_DECL of a task.  */

static enum starpu_access_mode
access_mode (const_tree type)
{
  gcc_assert (POINTER_TYPE_P (type));

  /* If TYPE points to a const-qualified type, then mark the data as
     read-only; if is has the `output' attribute, then mark it as write-only;
     otherwise default to read-write.  */
  return ((TYPE_QUALS (TREE_TYPE (type)) & TYPE_QUAL_CONST)
	  ? STARPU_R
	  : (output_type_p (type) ? STARPU_W : STARPU_RW));
}

static void
register_task_attributes (void *gcc_data, void *user_data)
{
  static const struct attribute_spec task_attr =
    {
      task_attribute_name, 0, 0, true, false, false,
      handle_task_attribute
#ifdef HAVE_ATTRIBUTE_SPEC_AFFECTS_TYPE_IDENTITY
      , false
#endif
    };

  static const struct attribute_spec task_implementation_attr =
    {
      task_implementation_attribute_name, 2, 2, true, false, false,
      handle_task_implementation_attribute
#ifdef HAVE_ATTRIBUTE_SPEC_AFFECTS_TYPE_IDENTITY
      , false
#endif
    };

  static const struct attribute_spec heap_allocated_attr =
    {
      heap_allocated_attribute_name, 0, 0, true, false, false,
      handle_heap_allocated_attribute
#ifdef HAVE_ATTRIBUTE_SPEC_AFFECTS_TYPE_IDENTITY
      , false
#endif
    };

  static const struct attribute_spec output_attr =
    {
      output_attribute_name, 0, 0, true, true, false,
      handle_output_attribute
#ifdef HAVE_ATTRIBUTE_SPEC_AFFECTS_TYPE_IDENTITY
      , true /* affects type identity */
#endif
    };

  register_attribute (&task_attr);
  register_attribute (&task_implementation_attr);
  register_attribute (&heap_allocated_attr);
  register_attribute (&output_attr);
}



/* Return the type of a codelet function, i.e.,
   `void (*) (void **, void *)'.  */

static tree
build_codelet_wrapper_type (void)
{
  tree void_ptr_ptr;

  void_ptr_ptr = build_pointer_type (ptr_type_node);

  return build_function_type_list (void_type_node,
				   void_ptr_ptr, ptr_type_node,
				   NULL_TREE);
}

/* Return an identifier for the wrapper of TASK_IMPL, a task
   implementation.  */

static tree
build_codelet_wrapper_identifier (tree task_impl)
{
  static const char suffix[] = ".task_implementation_wrapper";

  tree id;
  char *cl_name;
  const char *task_name;

  id = DECL_NAME (task_impl);
  task_name = IDENTIFIER_POINTER (id);

  cl_name = (char *) alloca (IDENTIFIER_LENGTH (id) + strlen (suffix) + 1);
  memcpy (cl_name, task_name, IDENTIFIER_LENGTH (id));
  strcpy (&cl_name[IDENTIFIER_LENGTH (id)], suffix);

  return get_identifier (cl_name);
}

/* Return a function of type `void (*) (void **, void *)' that calls function
   TASK_IMPL, the FUNCTION_DECL of a task implementation whose prototype may
   be arbitrary.  */

static tree
build_codelet_wrapper_definition (tree task_impl)
{
  location_t loc;
  tree task_decl, wrapper_name, decl;

  loc = DECL_SOURCE_LOCATION (task_impl);
  task_decl = task_implementation_task (task_impl);

  wrapper_name = build_codelet_wrapper_identifier (task_impl);
  decl = build_decl (loc, FUNCTION_DECL, wrapper_name,
		     build_codelet_wrapper_type ());

  local_define (tree, build_local_var, (const_tree type))
  {
    tree var, t;
    const char *seed;

    t = TREE_VALUE (type);
    seed = POINTER_TYPE_P (t) ? "pointer_arg" : "scalar_arg";

    var = build_decl (loc, VAR_DECL, create_tmp_var_name (seed), t);
    DECL_CONTEXT (var) = decl;
    DECL_ARTIFICIAL (var) = true;

    return var;
  };

  /* Return the body of the wrapper, which unpacks `cl_args' and calls the
     user-defined task implementation.  */

  local_define (tree, build_body, (tree wrapper_decl, tree vars))
  {
    bool opencl_p;
    tree stmts = NULL, call, v;
    VEC(tree, gc) *args;

    opencl_p = (task_implementation_where (task_impl) == STARPU_OPENCL);

    /* Build `var0 = STARPU_VECTOR_GET_PTR (buffers[0]); ...' or
       `var0 = STARPU_VECTOR_GET_DEV_HANDLE (buffers[0])' for OpenCL.  */

    size_t index = 0;
    for (v = vars; v != NULL_TREE; v = TREE_CHAIN (v))
      {
	if (POINTER_TYPE_P (TREE_TYPE (v)))
	  {
	    /* Compute `void *VDESC = buffers[0];'.  */
	    tree vdesc = array_ref (DECL_ARGUMENTS (wrapper_decl), index);

	    /* Use the right field, depending on OPENCL_P.  */
	    size_t offset =
	      opencl_p
	      ? offsetof (struct starpu_vector_interface, dev_handle)
	      : offsetof (struct starpu_vector_interface, ptr);

	    gcc_assert (POINTER_TYPE_P (TREE_TYPE (vdesc)));

	    /* Compute `type *PTR = *(type **) VDESC;'.  */
	    tree ptr =
	      build_indirect_ref (UNKNOWN_LOCATION,
				  fold_convert (build_pointer_type (TREE_TYPE (v)),
						pointer_plus (vdesc, offset)),
				  RO_ARRAY_INDEXING);

	    append_to_statement_list (build2 (MODIFY_EXPR, TREE_TYPE (v),
					      v, ptr),
				      &stmts);

	    index++;
	  }
      }

    /* Build `starpu_codelet_unpack_args (cl_args, &var1, &var2, ...)'.  */

    args = NULL;
    VEC_safe_push (tree, gc, args, TREE_CHAIN (DECL_ARGUMENTS (wrapper_decl)));
    for (v = vars; v != NULL_TREE; v = TREE_CHAIN (v))
      {
	if (!POINTER_TYPE_P (TREE_TYPE (v)))
	  VEC_safe_push (tree, gc, args, build_addr (v, wrapper_decl));
      }

    if (VEC_length (tree, args) > 1)
      {
	call = build_call_expr_loc_vec (UNKNOWN_LOCATION, unpack_fn, args);
	TREE_SIDE_EFFECTS (call) = 1;
	append_to_statement_list (call, &stmts);
      }

    /* Build `my_task_impl (var1, var2, ...)'.  */

    args = NULL;
    for (v = vars; v != NULL_TREE; v = TREE_CHAIN (v))
      VEC_safe_push (tree, gc, args, v);

    call = build_call_expr_loc_vec (UNKNOWN_LOCATION, task_impl, args);
    TREE_SIDE_EFFECTS (call) = 1;
    append_to_statement_list (call, &stmts);

    tree bind;
    bind = build3 (BIND_EXPR, void_type_node, vars, stmts,
		   DECL_INITIAL (wrapper_decl));
    TREE_TYPE (bind) = TREE_TYPE (TREE_TYPE (wrapper_decl));

    return bind;
  };

  /* Return the parameter list of the wrapper:
     `(void **BUFFERS, void *CL_ARGS)'.  */

  local_define (tree, build_parameters, (tree wrapper_decl))
  {
    tree param1, param2;

    param1 = build_decl (loc, PARM_DECL,
			 create_tmp_var_name ("buffers"),
			 build_pointer_type (ptr_type_node));
    DECL_ARG_TYPE (param1) = ptr_type_node;
    DECL_CONTEXT (param1) = wrapper_decl;
    TREE_USED (param1) = true;

    param2 = build_decl (loc, PARM_DECL,
			 create_tmp_var_name ("cl_args"),
			 ptr_type_node);
    DECL_ARG_TYPE (param2) = ptr_type_node;
    DECL_CONTEXT (param2) = wrapper_decl;
    TREE_USED (param2) = true;

    return chainon (param1, param2);
  };

  tree vars, result;

  vars = map (build_local_var,
	      list_remove (void_type_p,
			   TYPE_ARG_TYPES (TREE_TYPE (task_decl))));

  DECL_CONTEXT (decl) = NULL_TREE;
  DECL_ARGUMENTS (decl) = build_parameters (decl);

  result = build_decl (loc, RESULT_DECL, NULL_TREE, void_type_node);
  DECL_CONTEXT (result) = decl;
  DECL_ARTIFICIAL (result) = true;
  DECL_IGNORED_P (result) = true;
  DECL_RESULT (decl) = result;

  DECL_INITIAL (decl) = build_block (vars, NULL_TREE, decl, NULL_TREE);

  DECL_SAVED_TREE (decl) = build_body (decl, vars);

  TREE_PUBLIC (decl) = TREE_PUBLIC (task_impl);
  TREE_STATIC (decl) = true;
  TREE_USED (decl) = true;
  DECL_ARTIFICIAL (decl) = true;
  DECL_EXTERNAL (decl) = false;
  DECL_UNINLINABLE (decl) = true;

  rest_of_decl_compilation (decl, true, 0);

  struct function *prev_cfun = cfun;
  set_cfun (NULL);
  allocate_struct_function (decl, false);
  cfun->function_end_locus = DECL_SOURCE_LOCATION (task_impl);

  cgraph_finalize_function (decl, false);

  /* Mark DECL as needed so that it doesn't get removed by
     `cgraph_remove_unreachable_nodes' when it's not public.  */
  cgraph_mark_needed_node (cgraph_get_node (decl));

  set_cfun (prev_cfun);

  return decl;
}

/* Define one wrapper function for each implementation of TASK.  TASK should
   be the FUNCTION_DECL of a task.  */

static void
define_codelet_wrappers (tree task)
{
  local_define (void, define, (tree task_impl))
  {
    tree wrapper_def;

    wrapper_def = build_codelet_wrapper_definition (task_impl);

    DECL_ATTRIBUTES (task_impl) =
      tree_cons (get_identifier (task_implementation_wrapper_attribute_name),
		 wrapper_def,
		 DECL_ATTRIBUTES (task_impl));
  };

  for_each (define, task_implementation_list (task));
}

/* Return a NODE_IDENTIFIER for the variable holding the `struct starpu_codelet'
   structure associated with TASK_DECL.  */

static tree
build_codelet_identifier (tree task_decl)
{
  static const char suffix[] = ".codelet";

  tree id;
  char *cl_name;
  const char *task_name;

  id = DECL_NAME (task_decl);
  task_name = IDENTIFIER_POINTER (id);

  cl_name = (char *) alloca (IDENTIFIER_LENGTH (id) + strlen (suffix) + 1);
  memcpy (cl_name, task_name, IDENTIFIER_LENGTH (id));
  strcpy (&cl_name[IDENTIFIER_LENGTH (id)], suffix);

  return get_identifier (cl_name);
}

static tree
codelet_type (void)
{
  /* XXX: Hack to allow the type declaration to be accessible at lower
     time.  */
  static tree type_decl = NULL_TREE;

  if (type_decl == NULL_TREE)
    {
      /* Lookup the `struct starpu_codelet' struct type.  This should succeed since
	 we push <starpu.h> early on.  */

      type_decl = lookup_name (get_identifier (codelet_struct_name));
      gcc_assert (type_decl != NULL_TREE && TREE_CODE (type_decl) == TYPE_DECL);
    }

  return TREE_TYPE (type_decl);
}

/* Return a VAR_DECL that declares a `struct starpu_codelet' structure for
   TASK_DECL.  */

static tree
build_codelet_declaration (tree task_decl)
{
  tree name, cl_decl;

  name = build_codelet_identifier (task_decl);

  cl_decl = build_decl (DECL_SOURCE_LOCATION (task_decl),
			VAR_DECL, name,
			/* c_build_qualified_type (type, TYPE_QUAL_CONST) */
			codelet_type ());

  DECL_ARTIFICIAL (cl_decl) = true;
  TREE_PUBLIC (cl_decl) = TREE_PUBLIC (task_decl);
  TREE_STATIC (cl_decl) = false;
  TREE_USED (cl_decl) = true;
  DECL_EXTERNAL (cl_decl) = true;
  DECL_CONTEXT (cl_decl) = NULL_TREE;

  return cl_decl;
}

/* Return a `struct starpu_codelet' initializer for TASK_DECL.  */

static tree
build_codelet_initializer (tree task_decl)
{
  tree fields;

  fields = TYPE_FIELDS (codelet_type ());
  gcc_assert (TREE_CODE (fields) == FIELD_DECL);

  local_define (tree, lookup_field, (const char *name))
  {
    tree fdecl, fname;

    fname = get_identifier (name);
    for (fdecl = fields;
	 fdecl != NULL_TREE;
	 fdecl = TREE_CHAIN (fdecl))
      {
	if (DECL_NAME (fdecl) == fname)
	  return fdecl;
      }

    /* Field NAME wasn't found.  */
    gcc_assert (false);
  };

  local_define (tree, field_initializer, (const char *name, tree value))
  {
    tree field, init;

    field = lookup_field (name);
    init = make_node (TREE_LIST);
    TREE_PURPOSE (init) = field;
    TREE_CHAIN (init) = NULL_TREE;

    if (TREE_CODE (TREE_TYPE (value)) != ARRAY_TYPE)
      TREE_VALUE (init) = fold_convert (TREE_TYPE (field), value);
    else
      TREE_VALUE (init) = value;

    return init;
  };

  local_define (tree, codelet_name, ())
  {
    const char *name = IDENTIFIER_POINTER (DECL_NAME (task_decl));
    return build_string_literal (strlen (name) + 1, name);
  };

  local_define (tree, where_init, (tree impls))
  {
    tree impl;
    int where_int = 0;

    for (impl = impls;
	 impl != NULL_TREE;
	 impl = TREE_CHAIN (impl))
      {
	tree impl_decl;

	impl_decl = TREE_VALUE (impl);
	gcc_assert (TREE_CODE (impl_decl) == FUNCTION_DECL);

	if (verbose_output_p)
	  /* List the implementations of TASK_DECL.  */
	  inform (DECL_SOURCE_LOCATION (impl_decl),
		  "   %qE", DECL_NAME (impl_decl));

	where_int |= task_implementation_where (impl_decl);
      }

    return build_int_cstu (integer_type_node, where_int);
  };

  local_define (tree, implementation_pointers, (tree impls, int where))
  {
    size_t len;
    tree impl, pointers;

    for (impl = impls, pointers = NULL_TREE, len = 0;
	 impl != NULL_TREE;
	 impl = TREE_CHAIN (impl))
      {
	tree impl_decl;

	impl_decl = TREE_VALUE (impl);
	if (task_implementation_where (impl_decl) == where)
	  {
	    /* Return a pointer to the wrapper of IMPL_DECL.  */
	    tree addr = build_addr (task_implementation_wrapper (impl_decl),
				    NULL_TREE);
	    pointers = tree_cons (size_int (len), addr, pointers);
	    len++;

	    if (len > STARPU_MAXIMPLEMENTATIONS)
	      error_at (DECL_SOURCE_LOCATION (impl_decl),
			"maximum number of per-target task implementations "
			"exceeded");
	  }
      }

    /* POINTERS must be null-terminated.  */
    pointers = tree_cons (size_int (len), build_zero_cst (ptr_type_node),
			  pointers);
    len++;

    /* Return an array initializer.  */
    tree index_type = build_index_type (size_int (list_length (pointers)));

    return build_constructor_from_list (build_array_type (ptr_type_node,
							  index_type),
					nreverse (pointers));
  };

  local_define (tree, pointer_arg_count, (void))
  {
    size_t len;

    len = list_length (task_pointer_parameter_types (task_decl));
    return build_int_cstu (integer_type_node, len);
  };

  local_define (tree, access_mode_array, (void))
  {
    const_tree type;
    tree modes;
    size_t index;

    for (type = task_pointer_parameter_types (task_decl),
	   modes = NULL_TREE, index = 0;
	 type != NULL_TREE && index < STARPU_NMAXBUFS;
	 type = TREE_CHAIN (type), index++)
      {
	tree value = build_int_cst (integer_type_node,
				    access_mode (TREE_VALUE (type)));

	modes = tree_cons (size_int (index), value, modes);
      }

    tree index_type = build_index_type (size_int (list_length (modes)));

    return build_constructor_from_list (build_array_type (integer_type_node,
							  index_type),
					nreverse (modes));
  };

  if (verbose_output_p)
    inform (DECL_SOURCE_LOCATION (task_decl),
	    "implementations for task %qE:", DECL_NAME (task_decl));

  tree impls, inits;

  impls = task_implementation_list (task_decl);

  inits =
    chain_trees (field_initializer ("name", codelet_name ()),
		 field_initializer ("where", where_init (impls)),
		 field_initializer ("nbuffers", pointer_arg_count ()),
		 field_initializer ("modes", access_mode_array ()),
		 field_initializer ("cpu_funcs",
				    implementation_pointers (impls,
							     STARPU_CPU)),
		 field_initializer ("opencl_funcs",
		 		    implementation_pointers (impls,
							     STARPU_OPENCL)),
		 field_initializer ("cuda_funcs",
		 		    implementation_pointers (impls,
							     STARPU_CUDA)),
		 NULL_TREE);

  return build_constructor_from_unsorted_list (codelet_type (), inits);
}

/* Return the VAR_DECL that defines a `struct starpu_codelet' structure for
   TASK_DECL.  The VAR_DECL is assumed to already exists, so it must not be
   pushed again.  */

static tree
declare_codelet (tree task_decl)
{
  /* Retrieve the declaration of the `struct starpu_codelet' object.  */
  tree cl_decl;
  cl_decl = lookup_name (build_codelet_identifier (task_decl));
  gcc_assert (cl_decl != NULL_TREE && TREE_CODE (cl_decl) == VAR_DECL);

  /* Turn the codelet declaration into a definition.  */
  TREE_TYPE (cl_decl) = codelet_type ();
  TREE_PUBLIC (cl_decl) = TREE_PUBLIC (task_decl);

  return cl_decl;
}

/* Return the identifier for an automatically-generated CPU codelet of
   TASK.  */

static tree
build_cpu_codelet_identifier (const_tree task)
{
  static const char suffix[] = ".cpu_implementation";

  tree id;
  char *cl_name;
  const char *task_name;

  id = DECL_NAME (task);
  task_name = IDENTIFIER_POINTER (id);

  cl_name = (char *) alloca (IDENTIFIER_LENGTH (id) + strlen (suffix) + 1);
  memcpy (cl_name, task_name, IDENTIFIER_LENGTH (id));
  strcpy (&cl_name[IDENTIFIER_LENGTH (id)], suffix);

  return get_identifier (cl_name);
}

static void
handle_pre_genericize (void *gcc_data, void *user_data)
{
  tree fn = (tree) gcc_data;

  gcc_assert (TREE_CODE (fn) == FUNCTION_DECL);

  if (task_p (fn) && TREE_STATIC (fn))
    {
      /* The user defined a body for task FN, which we interpret as being the
	 body of an implicit CPU task implementation for FN.  Thus, rename FN
	 and turn it into the "cpu" implementation of a task that we create
	 under FN's original name (this is easier than moving the body to a
	 different function, which would require traversing the body to
	 rewrite all references to FN to point to the new function.)  Later,
	 `lower_starpu' rewrites calls to FN as calls to the newly created
	 task.  */

      tree task_name = DECL_NAME (fn);

      tree cpu_impl = fn;
      DECL_NAME (cpu_impl) = build_cpu_codelet_identifier (fn);
      if (verbose_output_p)
	inform (DECL_SOURCE_LOCATION (fn),
		"implicit CPU implementation renamed from %qE to %qE",
		task_name, DECL_NAME (cpu_impl));

      tree task = build_decl (DECL_SOURCE_LOCATION (fn), FUNCTION_DECL,
			      task_name, TREE_TYPE (fn));

      TREE_PUBLIC (task) = TREE_PUBLIC (fn);
      TREE_PUBLIC (cpu_impl) = false;

      taskify_function (task);

      /* Inherit the task implementation list from FN.  */
      tree impls = lookup_attribute (task_implementation_list_attribute_name,
				     DECL_ATTRIBUTES (fn));
      gcc_assert (impls != NULL_TREE);
      impls = TREE_VALUE (impls);

      DECL_ATTRIBUTES (task) =
	tree_cons (get_identifier (task_implementation_list_attribute_name),
		   impls, DECL_ATTRIBUTES (task));

      /* Make CPU_IMPL an implementation of FN.  */
      DECL_ATTRIBUTES (cpu_impl) =
	tree_cons (get_identifier (task_implementation_attribute_name),
		   tree_cons (NULL_TREE, build_string (3, "cpu"),
			      tree_cons (NULL_TREE, task, NULL_TREE)),
		   NULL_TREE);

      add_task_implementation (task, cpu_impl, build_string (3, "cpu"));

      /* And now, process CPU_IMPL.  */
    }

  if (task_implementation_p (fn))
    {
      tree task = task_implementation_task (fn);

      if (!TREE_STATIC (task))
	{
	  /* TASK lacks a body.  Declare its codelet, intantiate its codelet
	     wrappers, and its body in this compilation unit.  */

	  local_define (tree, build_parameter, (const_tree lst))
	  {
	    tree param, type;

	    type = TREE_VALUE (lst);
	    param = build_decl (DECL_SOURCE_LOCATION (task), PARM_DECL,
				create_tmp_var_name ("parameter"),
				type);
	    DECL_ARG_TYPE (param) = type;
	    DECL_CONTEXT (param) = task;

	    return param;
	  };

	  /* Declare TASK's codelet.  It cannot be defined yet because the
	     complete list of tasks isn't available at this point.  */
	  declare_codelet (task);

	  /* Set the task's parameter list.  */
	  DECL_ARGUMENTS (task) =
	    map (build_parameter,
		 list_remove (void_type_p,
			      TYPE_ARG_TYPES (TREE_TYPE (task))));

	  /* Build its body.  */
	  current_function_decl = task;
	  define_task (task);
	  current_function_decl = fn;

	  /* Compile TASK's body.  */
	  rest_of_decl_compilation (task, true, 0);
	  allocate_struct_function (task, false);
	  cgraph_finalize_function (task, false);
	  cgraph_mark_needed_node (cgraph_get_node (task));
	}
    }
}

/* Build a "conversion" from a raw C pointer to its data handle.  The
   assumption is that the programmer should have already registered the
   pointer by themselves.  */

static tree
build_pointer_lookup (tree pointer)
{
  /* Make sure DATA_LOOKUP_FN is valid.  */
  LOOKUP_STARPU_FUNCTION (data_lookup_fn, "starpu_data_lookup");

  location_t loc;

  if (DECL_P (pointer))
    loc = DECL_SOURCE_LOCATION (pointer);
  else
    loc = UNKNOWN_LOCATION;

  /* Introduce a local variable to hold the handle.  */

  tree result_var = build_decl (loc, VAR_DECL,
  				create_tmp_var_name (".data_lookup_result"),
  				ptr_type_node);
  DECL_CONTEXT (result_var) = current_function_decl;
  DECL_ARTIFICIAL (result_var) = true;
  DECL_SOURCE_LOCATION (result_var) = loc;

  tree call = build_call_expr (data_lookup_fn, 1, pointer);
  tree assignment = build2 (INIT_EXPR, TREE_TYPE (result_var),
  			    result_var, call);

  /* Build `if (RESULT_VAR == NULL) error ();'.  */

  tree cond = build3 (COND_EXPR, void_type_node,
		      build2 (EQ_EXPR, boolean_type_node,
			      result_var, null_pointer_node),
		      build_error_statements (loc, NULL_TREE,
					      "attempt to use unregistered "
					      "pointer"),
		      NULL_TREE);

  tree stmts = NULL;
  append_to_statement_list (assignment, &stmts);
  append_to_statement_list (cond, &stmts);
  append_to_statement_list (result_var, &stmts);

  return build4 (TARGET_EXPR, ptr_type_node, result_var, stmts, NULL_TREE, NULL_TREE);
}

/* Build the body of TASK_DECL, which will call `starpu_insert_task'.  */

static void
define_task (tree task_decl)
{
  VEC(tree, gc) *args = NULL;
  location_t loc = DECL_SOURCE_LOCATION (task_decl);
  tree p, params = DECL_ARGUMENTS (task_decl);

  /* The first argument will be a pointer to the codelet.  */

  VEC_safe_push (tree, gc, args,
		 build_addr (task_codelet_declaration (task_decl),
			     current_function_decl));

  for (p = params; p != NULL_TREE; p = TREE_CHAIN (p))
    {
      gcc_assert (TREE_CODE (p) == PARM_DECL);

      tree type = TREE_TYPE (p);

      if (POINTER_TYPE_P (type))
	{
	  /* A pointer: the arguments will be:
	     `STARPU_RW, ptr' or similar.  */

	  VEC_safe_push (tree, gc, args,
			 build_int_cst (integer_type_node,
					access_mode (type)));
	  VEC_safe_push (tree, gc, args, build_pointer_lookup (p));
	}
      else
	{
	  /* A scalar: the arguments will be:
	     `STARPU_VALUE, &scalar, sizeof (scalar)'.  */

	  mark_addressable (p);

	  VEC_safe_push (tree, gc, args,
			 build_int_cst (integer_type_node, STARPU_VALUE));
	  VEC_safe_push (tree, gc, args,
			 build_addr (p, current_function_decl));
	  VEC_safe_push (tree, gc, args,
			 size_in_bytes (type));
	}
    }

  /* Push the terminating zero.  */

  VEC_safe_push (tree, gc, args,
		 build_int_cst (integer_type_node, 0));

  /* Introduce a local variable to hold the error code.  */

  tree error_var = build_decl (loc, VAR_DECL,
  			       create_tmp_var_name (".insert_task_error"),
  			       integer_type_node);
  DECL_CONTEXT (error_var) = task_decl;
  DECL_ARTIFICIAL (error_var) = true;

  /* Build this:

       err = starpu_insert_task (...);
       if (err != 0)
         { printf ...; abort (); }
   */

  static tree insert_task_fn;
  LOOKUP_STARPU_FUNCTION (insert_task_fn, "starpu_insert_task");

  tree call = build_call_expr_loc_vec (loc, insert_task_fn, args);

  tree assignment = build2 (INIT_EXPR, TREE_TYPE (error_var),
  			    error_var, call);

  tree name = DECL_NAME (task_decl);
  tree cond = build3 (COND_EXPR, void_type_node,
		      build2 (NE_EXPR, boolean_type_node,
			      error_var, integer_zero_node),
		      build_error_statements (loc, error_var,
					      "failed to insert task `%s'",
					      IDENTIFIER_POINTER (name)),
		      NULL_TREE);

  tree stmts = NULL;
  append_to_statement_list (assignment, &stmts);
  append_to_statement_list (cond, &stmts);

  tree bind = build3 (BIND_EXPR, void_type_node, error_var, stmts,
  		      NULL_TREE);

  /* Put it all together.  */

  DECL_SAVED_TREE (task_decl) = bind;
  TREE_STATIC (task_decl) = true;
  DECL_EXTERNAL (task_decl) = false;
  DECL_ARTIFICIAL (task_decl) = true;
  DECL_INITIAL (task_decl) =
    build_block (error_var, NULL_TREE, task_decl, NULL_TREE);
  DECL_RESULT (task_decl) =
    build_decl (loc, RESULT_DECL, NULL_TREE, void_type_node);
  DECL_CONTEXT (DECL_RESULT (task_decl)) = task_decl;
}

/* Raise warnings if TASK doesn't meet the basic criteria.  */

static void
validate_task (tree task)
{
  gcc_assert (task_p (task));

  static const int supported = 0
#ifdef STARPU_USE_CPU
    | STARPU_CPU
#endif
#ifdef STARPU_USE_CUDA
    | STARPU_CUDA
#endif
#ifdef STARPU_USE_OPENCL
    | STARPU_OPENCL
#endif
#ifdef STARPU_USE_GORDON
    | STARPU_GORDON
#endif
    ;

  int where = task_where (task);

  /* If TASK has no implementations, things will barf elsewhere anyway.  */

  if (task_implementation_list (task) != NULL_TREE)
    if ((where & supported) == 0)
      error_at (DECL_SOURCE_LOCATION (task),
		"none of the implementations of task %qE can be used",
		DECL_NAME (task));
}

/* Raise an error when IMPL doesn't satisfy the constraints of a task
   implementations, such as not invoking another task.  */

static void
validate_task_implementation (tree impl)
{
  gcc_assert (task_implementation_p (impl));

  const struct cgraph_node *cgraph;
  const struct cgraph_edge *callee;

  cgraph = cgraph_get_node (impl);

  /* When a definition of IMPL is available, check its callees.  */
  if (cgraph != NULL)
    for (callee = cgraph->callees;
	 callee != NULL;
	 callee = callee->next_callee)
      {
	if (task_p (callee->callee->decl))
	  {
	    location_t loc;

	    loc = gimple_location (callee->call_stmt);
	    error_at (loc, "task %qE cannot be invoked from task implementation %qE",
		      DECL_NAME (callee->callee->decl),
		      DECL_NAME (impl));
	  }
      }
}

static unsigned int
lower_starpu (void)
{
  tree fndecl;
  const struct cgraph_node *cgraph;
  const struct cgraph_edge *callee;

  fndecl = current_function_decl;
  gcc_assert (TREE_CODE (fndecl) == FUNCTION_DECL);

  if (task_p (fndecl))
    {
      /* Make sure the task and its implementations are valid.  */

      validate_task (fndecl);

      for_each (validate_task_implementation,
		task_implementation_list (fndecl));

      /* Generate a `struct starpu_codelet' structure and a wrapper function for
	 each implementation of TASK_DECL.  This cannot be done earlier
	 because we need to have a complete list of task implementations.  */

      define_codelet_wrappers (fndecl);

      tree cl_def = task_codelet_declaration (fndecl);
      DECL_INITIAL (cl_def) = build_codelet_initializer (fndecl);
      TREE_STATIC (cl_def) = true;
      DECL_EXTERNAL (cl_def) = false;

      varpool_finalize_decl (cl_def);
    }

  /* This pass should occur after `build_cgraph_edges'.  */
  cgraph = cgraph_get_node (fndecl);
  gcc_assert (cgraph != NULL);

  if (MAIN_NAME_P (DECL_NAME (fndecl)))
    {
      /* Check whether FNDECL initializes StarPU and emit a warning if it
	 doesn't.  */
      bool initialized;

      for (initialized = false, callee = cgraph->callees;
	   !initialized && callee != NULL;
	   callee = callee->next_callee)
	{
	  initialized =
	    DECL_NAME (callee->callee->decl) == get_identifier ("starpu_init");
	}

      if (!initialized)
	warning_at (DECL_SOURCE_LOCATION (fndecl), 0,
		    "%qE does not initialize StarPU", DECL_NAME (fndecl));
    }

  for (callee = cgraph->callees;
       callee != NULL;
       callee = callee->next_callee)
    {
      gcc_assert (callee->callee != NULL);

      tree callee_decl, caller_decl;

      callee_decl = callee->callee->decl;
      caller_decl = callee->caller->decl;

      if (implicit_cpu_task_implementation_p (callee_decl)
	  && !DECL_ARTIFICIAL (caller_decl))
	{
	  /* Rewrite the call to point to the actual task beneath
	     CALLEE_DECL.  */
	  callee_decl = task_implementation_task (callee_decl);
	  if (verbose_output_p)
	    inform (gimple_location (callee->call_stmt),
		    "call to %qE rewritten as a call to task %qE",
		    DECL_NAME (callee->callee->decl),
		    DECL_NAME (callee_decl));

	  gimple_call_set_fn (callee->call_stmt,
			      build_addr (callee_decl, callee->caller->decl));
	}

      if (task_p (callee_decl))
	{
	  if (verbose_output_p)
	    inform (gimple_location (callee->call_stmt),
		    "%qE calls task %qE",
		    DECL_NAME (fndecl), DECL_NAME (callee_decl));

	  /* TODO: Insert analysis to check whether the pointer arguments
	     need to be registered.  */
	}
    }

  return 0;
}

static struct opt_pass pass_lower_starpu =
  {
    designated_field_init (type, GIMPLE_PASS),
    designated_field_init (name, "pass_lower_starpu"),
    designated_field_init (gate, NULL),
    designated_field_init (execute, lower_starpu)

    /* The rest is zeroed.  */
  };


/* Initialization.  */

/* Directory where to look up <starpu.h> instead of `STARPU_INCLUDE_DIR'.  */
static const char *include_dir;

static void
define_cpp_macros (void *gcc_data, void *user_data)
{
  cpp_define (parse_in, "STARPU_GCC_PLUGIN=0");

  if (include_dir)
    {
      /* Get the header from the user-specified directory.  This is useful
	 when running the test suite, before StarPU is installed.  */
      char header[strlen (include_dir) + sizeof ("/starpu.h")];
      strcpy (header, include_dir);
      strcat (header, "/starpu.h");
      cpp_push_include (parse_in, header);
    }
  else
    cpp_push_include (parse_in, STARPU_INCLUDE_DIR "/starpu.h");
}

int
plugin_init (struct plugin_name_args *plugin_info,
	     struct plugin_gcc_version *version)
{
  if (!plugin_default_version_check (version, &gcc_version))
    return 1;

  register_callback (plugin_name, PLUGIN_START_UNIT,
		     define_cpp_macros, NULL);
  register_callback (plugin_name, PLUGIN_PRAGMAS,
		     register_pragmas, NULL);
  register_callback (plugin_name, PLUGIN_ATTRIBUTES,
		     register_task_attributes, NULL);
  register_callback (plugin_name, PLUGIN_PRE_GENERICIZE,
  		     handle_pre_genericize, NULL);

  /* Register our pass so that it happens after `build_cgraph_edges' has been
     done.  */

  struct register_pass_info pass_info =
    {
      designated_field_init (pass, &pass_lower_starpu),
      designated_field_init (reference_pass_name, "*build_cgraph_edges"),
      designated_field_init (ref_pass_instance_number, 1),
      designated_field_init (pos_op, PASS_POS_INSERT_AFTER)
    };

  register_callback (plugin_name, PLUGIN_PASS_MANAGER_SETUP,
		     NULL, &pass_info);

  include_dir = getenv ("STARPU_GCC_INCLUDE_DIR");

  int arg;
  for (arg = 0; arg < plugin_info->argc; arg++)
    {
      if (strcmp (plugin_info->argv[arg].key, "include-dir") == 0)
	{
	  if (plugin_info->argv[arg].value == NULL)
	    error_at (UNKNOWN_LOCATION, "missing directory name for option "
		      "%<-fplugin-arg-starpu-include-dir%>");
	  else
	    /* XXX: We assume that `value' has an infinite lifetime.  */
	    include_dir = plugin_info->argv[arg].value;
	}
      else if (strcmp (plugin_info->argv[arg].key, "verbose") == 0)
	verbose_output_p = true;
      else
	error_at (UNKNOWN_LOCATION, "invalid StarPU plug-in argument %qs",
		  plugin_info->argv[arg].key);
    }

  return 0;
}

#ifdef __cplusplus
}
#endif
