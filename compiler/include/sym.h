/* -*-Mode: c++;-*-
*/
#ifndef _sym_h_
#define _sym_h_

#include <stdio.h>
#include "extern.h"
#include "vec.h"
#include "map.h"

class Var;
class Fun;
class AType;
class CreationSet;
class MType;
class Scope;
class AST;
class Code;
class LabelMap;

enum IF1_num_kind {
  IF1_NUM_KIND_NONE, IF1_NUM_KIND_UINT, IF1_NUM_KIND_INT, IF1_NUM_KIND_FLOAT, IF1_NUM_KIND_COMPLEX
};

enum Type_kind {
  Type_NONE,      	// Sym is not a type
  Type_UNKNOWN,     	// type is not given (e.g. type parameter, forward decl)
  Type_SUM,  		// one thing or another (e.g. int | float)
  Type_PRODUCT, 	// two things together (e.g. (1, 2.0))
  Type_RECORD,    	// things with names (nominally unordered)
  Type_VECTOR,      	// multidimensional index type with monomorphic elements
  Type_FUN,		// takes a set of types to a return type
  Type_REF,  		// pointer
  Type_TAGGED, 		// tag + type (used for variant types)
  Type_PRIMITIVE, 	// builtin things (e.g. int and float)
  Type_APPLICATION, 	// application of a Type_ALIAS with args
  Type_ALIAS		// a type by another name, possible with constraints
};

#define CPP_IS_LAME {						\
  "NONE", "UNKNOWN", "SUM", "PRODUCT", "RECORD", "VECTOR",	\
 "FUN", "REF", "TAGGED", "PRIMITIVE", "APPLICATION", "ALIAS"}
EXTERN char *type_kind_string[] EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

union Immediate {
  bool v_bool;
  uint8 v_uint8;
  int8 v_int8;
  uint16 v_uint16;
  int16 v_int16;
  uint32 v_uint32;
  int32 v_int32;
  uint64 v_uint64;
  int64 v_int64;
  float32 v_float32;
  float64 v_float64;
  complex32 v_complex32;
  complex64 v_complex64;
  char *v_string;
};

class Sym : public gc {
 public:
  int			id;			// unique number
  char 			*name;			// user level name
  Sym  			*in;			// containing module, class, function
  Sym 			*type;			// true type
  Sym  			*aspect;		// mascarade as type (e.g. superclass)
  Vec<Sym *>		*constraints;		// must-implement
  AST			*ast;			// AST node which defined this symbol
  Var			*var;			// used by fa.cpp
  char 			*cg_string;		// used by cg.cpp

  unsigned int		is_builtin:1;		// Sym is builtin to the compiler
  unsigned int		is_read_only:1;		// Sym is read only
  unsigned int		is_constant:1;		// Sym is a constant
  unsigned int		is_lvalue:1;		// Sym is an lvalue
  unsigned int		is_var:1;		// Sym is a variable (as opposed to let bound)
  unsigned int		is_default_arg:1;	// Sym is a default argument
  unsigned int 		is_module:1;		// Sym is a module
  unsigned int		is_fun:1;		// Sym is a function
  unsigned int 		is_symbol:1;		// Sym is a user level symbol
  unsigned int		is_pattern:1;		// Sym is a pattern
  unsigned int		is_rest:1;		// Sym is a rest (vararg) parameter
  unsigned int		is_external:1;		// Sym is "external", so constraints are type

  unsigned int		global_scope:1;		// Sym is globally unique (file/module)
  unsigned int		function_scope:1;	// Sym is function unique

  unsigned int		is_structure:1;		// Sym is a structure (C compatibility)
  unsigned int		is_meta_class:1;	// Sym is class of class
  unsigned int 		is_value_class:1;	// Sym is a value class

  unsigned int		live:1;			// used by if1.cpp
  unsigned int		incomplete:1;		// used by clone.cpp

  unsigned int		type_kind:4;
  unsigned int		num_kind:3;		// Sort of number class
  unsigned int		num_index:3;		// Precision of number class

  char			*alt_name;		// alternative name (pattern/extern)

  char 			*constant;		// string representing constant value
  Immediate		imm;			// constant and folded constant immediate values

  Vec<Sym *>		implements;
  Vec<Sym *>		includes;		// included code
  Vec<Sym *>		has;			// sub variables/members (currently fun args)
  Vec<Sym *>		arg;			// arg variables (currently just meta type args)
  Sym			*alias;			// alias of type
  Sym			*init;			// for modules & classes (default init function)
  Sym			*type_sym;		// the representative symbol for this type in code
  Sym			*element;

  Scope 		*scope;			// used in ast.cpp
  LabelMap		*labelmap;		// used by ast.cpp
  Vec<Sym *>		dispatch_order;		// used by fa.cpp, pattern.cpp
  Vec<Sym *>		subtypes;		// used by fa.cpp
  Vec<Sym *>		allsubtypes;		// used by fa.cpp
  MType	       		*match_type;		// used by pattern.cpp
  AType			*abstract_type;		// used by fa.cpp
  Vec<CreationSet *>	creators;		// used by fa.cpp

  Fun			*fun;			// used by fa.cpp
  Code			*code;			// for functions, Code
  Sym			*self;			// self variable for the function
  Sym			*ret;			// return value of functions
  Sym			*cont;			// continuation (function returning ret)

  char			*pathname();
  char			*filename();
  int			line();

  virtual Sym		*copy();
  void			copy_values(Sym *);

  int imm_int(int *);
};
#define forv_Sym(_c, _v) forv_Vec(Sym, _c, _v)

Sym *unalias_type(Sym *s);
Sym *meta_apply(Sym *fn, Sym *arg);

void convert_constant_to_immediate(Sym *sym);
int print(FILE *fp, Immediate &imm, Sym *type);
int sprint(char *s, Immediate &imm, Sym *type);
int compar_syms(const void *ai, const void *aj);

#endif
