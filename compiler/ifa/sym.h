/* -*-Mode: c++;-*-
*/
#ifndef _sym_h_
#define _sym_h_

#include <stdio.h>
#include "extern.h"
#include "num.h"
#include "vec.h"
#include "map.h"
#include "ifa.h"

class Var;
class Fun;
class AType;
class CreationSet;
class MType;
class Scope;
class AST;
class Code;
class LabelMap;
class Sym;

enum Type_kind {
  Type_NONE,            // Sym is not a type
  Type_UNKNOWN,         // type is not given (e.g. type parameter, forward decl)
  Type_LUB,             // one thing or another (e.g. int | float)
  Type_GLB,             // one thing and another (e.g. int & float)
  Type_PRODUCT,         // two things together (e.g. (1, 2.0))
  Type_RECORD,          // things with names (nominally unordered)
  Type_VECTOR,          // multidimensional index type with monomorphic elements
  Type_FUN,             // takes a set of types to a return type
  Type_REF,             // pointer
  Type_TAGGED,          // tag + type (used for variant types)
  Type_PRIMITIVE,       // builtin things (e.g. int and float)
  Type_APPLICATION,     // application of a Type_ALIAS with args
  Type_VARIABLE,        // type variable
  Type_ALIAS            // a type by another name, possible with constraints
};

enum Sym_Intent {
  Sym_IN,
  Sym_INOUT,
  Sym_OUT
}; 
#define is_Sym_OUT(_x) ((_x)->intent == Sym_OUT)

extern char *type_kind_string[];

#define CLEAR_VARIABLE(_m) memset(&(_m),0,sizeof(_m))

class BasicSym : public gc {
 public:
  int                   id;                     // unique number
  char                  *name;                  // user level name
  Sym                   *in;                    // containing module, class, function
  Sym                   *type;                  // type of the symbol
  Sym                   *aspect;                // masquarade as type (e.g. superclass)
  Sym                   *must_specialize;       // dispatch constraints
  Sym                   *must_implement;        // type checking constraints
  IFAAST                *ast;                   // AST node which defined this symbol
  Var                   *var;                   // used by fa.cpp
  IFASymbol             *asymbol;               // front end interface object
  int                   nesting_depth;          // nested function depth
  char                  *cg_string;             // used by cg.cpp

  unsigned int          is_builtin:1;           // Sym is builtin to the compiler
  unsigned int          is_read_only:1;         // Sym is read only
  unsigned int          is_constant:1;          // Sym is a constant
  unsigned int          is_lvalue:1;            // Sym is an lvalue
  unsigned int          is_var:1;               // Sym is a variable (as opposed to let bound)
  unsigned int          is_default_arg:1;       // Sym is a default argument
  unsigned int          is_module:1;            // Sym is a module
  unsigned int          is_fun:1;               // Sym is a function
  unsigned int          is_symbol:1;            // Sym is a user level symbol
  unsigned int          is_pattern:1;           // Sym is a pattern
  unsigned int          is_rest:1;              // Sym is a rest (vararg) parameter
  unsigned int          is_generic:1;           // Sym is a generic parameter
  unsigned int          is_external:1;          // Sym is "external", so constraints are type

  unsigned int          intent:2;               // Sym is "in", "inout", or "out"

  unsigned int          global_scope:1;         // Sym is globally unique (file/module)
  unsigned int          function_scope:1;       // Sym is function unique

  unsigned int          is_structure:1;         // Sym is a structure (C compatibility)
  unsigned int          is_meta_type:1;         // Sym is class of class
  unsigned int          is_value_class:1;       // Sym is a value class
  unsigned int          is_union_class:1;       // Sym is a union

  unsigned int          fun_returns_value:1;    // used by analysis.cpp
  unsigned int          live:1;                 // used by if1.cpp
  unsigned int          incomplete:1;           // used by clone.cpp

  unsigned int          type_kind:4;
  unsigned int          num_kind:3;             // Sort of number class
  unsigned int          num_index:3;            // Precision of number class

  unsigned int          clone_for_constants:1;  // analysis should attempt to make this a constant

  BasicSym(void);
};

class Sym : public BasicSym {
 public:
  char                  *destruct_name;         // name of related destructured element
  char                  *arg_name;              // argument name

  char                  *constant;              // string representing constant value
  Immediate             imm;                    // constant and folded constant immediate values

  int                   size;                   // size as immediate (i.e. references == sizeof(void*))

  Scope                 *scope;                 // used in ast.cpp              *fun* *type* *module*
  LabelMap              *labelmap;              // used by ast.cpp              *fun*

  Fun                   *fun;                   // used by fa.cpp               *fun*
  Code                  *code;                  // for functions, Code          *fun*
  Sym                   *self;                  // self variable for the function *fun*
  Sym                   *ret;                   // return value of functions    *fun*
  Sym                   *cont;                  // continuation (function returning ret) *fun*

  Sym                   *instantiates;          
  Vec<Sym *>            implementors;           // used by fa.cpp, implementors *type*
  Vec<Sym *>            specializers;           // used by fa.cpp, specializers *type*
  Vec<Sym *>            dispatch_order;         // used by fa.cpp, pattern.cpp  *type*

  MType                 *match_type;            // used by pattern.cpp          *type*
  AType                 *abstract_type;         // used by fa.cpp               *type*
  Vec<CreationSet *>    creators;               // used by fa.cpp               *type*

  Vec<Sym *>            specializes;            // declared superclasses        *type*
  Vec<Sym *>            includes;               // included code                *type*
  Vec<Sym *>            implements;             // declared supertypes          *type*
  Vec<Sym *>            has;                    // sub variables/members (currently fun args) *fun* *type*
  Vec<Sym *>            arg;                    // arg variables (currently just meta type args) *type*
  Sym                   *alias;                 // alias of type                *type*
  Sym                   *init;                  // for modules & classes (default init function) *type*
  Sym                   *meta_type;             // meta type and inverse ptr    *type*
  Sym                   *element;               // element type for aggregates  *type*
  Sym                   *domain;                // domain type for  aggregates  *type

  void                  *temp;                  // algorithmic temp             *type*

  Sym(void);
  Sym *                 scalar_type();          // scalar inheritted from or NULL
  Sym *                 coerce_to(Sym *);
  char                  *pathname();
  int                   line();
  int                   log_line();             // squelch line numbers of prelude in log
  char                  *filename();
  int                   ast_id();
  Sym *                 clone();

  void                  copy_values(Sym *);
  void                  inherits_add(Sym *);
  void                  must_implement_and_specialize(Sym *);

  Sym                   *copy();
  int imm_int(int *);
};

#define forv_Sym(_c, _v) forv_Vec(Sym, _c, _v)

Sym *unalias_type(Sym *s);
Sym *meta_apply(Sym *fn, Sym *arg);

void convert_constant_to_immediate(Sym *sym);
int compar_syms(const void *ai, const void *aj);
void coerce_numeric(Sym *s, Sym *t, Immediate *im);

#endif
