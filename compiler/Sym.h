/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _Sym_h_
#define _Sym_h_

class Var;
class Fun;
class AType;
class CreationSet;

enum IF1_num_type {
  IF1_NUM_TYPE_NONE, IF1_NUM_TYPE_UINT, IF1_NUM_TYPE_INT, IF1_NUM_TYPE_FLOAT
};

enum Type_kind {
  Type_NONE,      	// Sym is not a type
  Type_UNKNOWN,     	// Type is not given (e.g. type parameter)
  Type_FORWARD,  	// Forward declaration (like UNKNOWN but allow redefinition)
  Type_SUM,  		// one thing or another (e.g. int | float)
  Type_PRODUCT, 	// two things together (e.g. (1, 2.0))
  Type_RECORD,    	// things with names (nominally unordered)
  Type_VECTOR,      	// multidimensional index type with monomorphic elements
  Type_FUN,		// takes a set of types to a return type
  Type_REF,  		// pointer
  Type_TAGGED, 		// tag + type (used for variant types)
  Type_PRIMITIVE, 	// builtin things (e.g. int and float)
  Type_ABSTRACTION, 	// parameterized type
  Type_APPLICATION, 	// application of a Type_ABSTRACTION
  Type_ALIAS		// a type by another name
};

class Sym : public gc {
 public:
  char 			*name;			// user level name
  int			id;			// unique number
  Sym 			*type;			// true type
  Sym 			*declared_type;		// type constraint (may be subtype)
  char 			*constant;		// string representing constant value
  Sym  			*aspect;		// mascarade as (e.g. superclass)
  Sym  			*in;			// containing module, class or function

  char 			*builtin;		// one of builtin_symbols.h
  Type_kind		type_kind;
  Vec<Sym *>		supertypes;		// implemented interfaces/is of types
  Vec<Sym *>		includes;		// included code
  Vec<Sym *>		has;			// instance variables
  Vec<Sym *>		args;			// parameters of functions and classes
  Sym			*ret;			// return value of functions
  Sym			*cont;			// continuation (function returning ret)
  Sym			*init;			// for modules & classes (default init function)
  Code			*code;			// for functions, Code
  Sym 			*next_sym;	       	// for function overloading
  AST			*ast;			// AST node which defined this symbol

  unsigned int 		module:1;		// Sym is a module
  unsigned int 		symbol:1;		// Sym is a user level symbol
  unsigned int 		value:1;		// Sym is a value (immediate)
  unsigned int		pattern:1;		// Sym is a pattern
  unsigned int		vararg:1;		// Sym is a vararg parameter
  unsigned int		structure:1;		// Sym is a structure (C compatibility)

  unsigned int		num_type:2;		// used by if1.cpp
  unsigned int		num_index:3;		// used by if1.cpp
  unsigned int		live:1;			// used by if1.cpp
  unsigned int		incomplete:1;		// used by clone.cpp
  Scope 		*scope;			// used in ast.cpp
  LabelMap		*labelmap;		// used by ast.cpp
  Vec<Sym *>		subtypes;		// used by fa.cpp
  AType			*as_AType;		// used by fa.cpp
  CreationSet		*as_CreationSet;	// used by fa.cpp
  Map<char *, Var *>	has_map;		// used by fa.cpp
  Vec<CreationSet *>	creators;		// used by fa.cpp
  Var			*var;			// used by fa.cpp
  Fun			*fun;			// used by fa.cpp
  char 			*cg_string;		// used by cg.cpp

  Sym() { memset(this, 0, sizeof *this); }
};
#define forv_Sym(_c, _v) forv_Vec(Sym, _c, _v)

Sym *unalias_type(Sym *s);

#endif
