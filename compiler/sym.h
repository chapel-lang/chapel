/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _sym_h_
#define _sym_h_

class Var;
class Fun;
class AType;
class CreationSet;

enum IF1_num_type {
  IF1_NUM_TYPE_NONE, IF1_NUM_TYPE_UINT, IF1_NUM_TYPE_INT, IF1_NUM_TYPE_FLOAT
};

enum Type_kind {
  Type_NONE,      	// Sym is not a type
  Type_UNKNOWN,     	// Type is not given (e.g. type parameter, forward decl)
  Type_SUM,  		// one thing or another (e.g. int | float)
  Type_PRODUCT, 	// two things together (e.g. (1, 2.0))
  Type_RECORD,    	// things with names (nominally unordered)
  Type_VECTOR,      	// multidimensional index type with monomorphic elements
  Type_FUN,		// takes a set of types to a return type
  Type_REF,  		// pointer
  Type_TAGGED, 		// tag + type (used for variant types)
  Type_PRIMITIVE, 	// builtin things (e.g. int and float)
  Type_APPLICATION, 	// application of a Type_ALIAS with args
  Type_ALIAS		// a type by another name
};

union Immediate {
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
  complex64 v_complex64;
  char *v_string;
};

class Sym : public gc {
 public:
  char 			*name;			// user level name
  int			id;			// unique number
  Sym 			*type;			// true type
  char 			*constant;		// string representing constant value
  Sym  			*aspect;		// mascarade as (e.g. superclass)
  Sym  			*in;			// containing module, class or function
  Immediate		imm;			// constant and folded constant immediate values

  char 			*builtin;		// one of builtin_symbols.h
  Type_kind		type_kind;
  Vec<Sym *>		implements;
  Vec<Sym *>		includes;		// included code
  Vec<Sym *>		constraints;		// must-implement
  Vec<Sym *>		has;			// instance variables
  Vec<Sym *>		args;			// parameters of functions and types
  Sym			*self;			// self variable for the function
  Sym			*ret;			// return value of functions
  Sym			*cont;			// continuation (function returning ret)
  Sym			*init;			// for modules & classes (default init function)
  Code			*code;			// for functions, Code
  Sym 			*next_sym;	       	// for function overloading
  AST			*ast;			// AST node which defined this symbol

  unsigned int 		module:1;		// Sym is a module
  unsigned int 		symbol:1;		// Sym is a user level symbol
  unsigned int		pattern:1;		// Sym is a pattern
  unsigned int		vararg:1;		// Sym is a vararg parameter
  unsigned int 		value:1;		// Sym is a value (immediate)
  unsigned int		structure:1;		// Sym is a structure (C compatibility)

  unsigned int		num_type:2;		// used by if1.cpp
  unsigned int		num_index:3;		// used by if1.cpp
  unsigned int		live:1;			// used by if1.cpp
  unsigned int		incomplete:1;		// used by clone.cpp
  Scope 		*scope;			// used in ast.cpp
  LabelMap		*labelmap;		// used by ast.cpp
  Vec<Sym *>		subtypes;		// used by fa.cpp
  AType			*abstract_type;		// used by fa.cpp
  Map<char *, Var *>	has_map;		// used by fa.cpp
  Vec<CreationSet *>	creators;		// used by fa.cpp
  Var			*var;			// used by fa.cpp
  Fun			*fun;			// used by fa.cpp
  char 			*cg_string;		// used by cg.cpp

  Sym() { memset(this, 0, sizeof *this); }
  int imm_int();
};
#define forv_Sym(_c, _v) forv_Vec(Sym, _c, _v)

Sym *unalias_type(Sym *s);

int pp(Immediate &imm, Sym *type);
int print(FILE *fp, Immediate &imm, Sym *type);

#endif
