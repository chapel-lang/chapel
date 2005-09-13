#ifndef _chapel_ifa_H_
#define _chapel_ifa_H_

#include "chplalloc.h"
#include "alist.h"
#include "vec.h"
#include "map.h"
#include "../ifa/ifa.h"

class BaseIFAAST;
class Label;
class Code;
class IFAASTCopyContext;
class Sym;

class ACallbacks : public IFACallbacks {
public:
  void finalize_functions();
  Sym *make_LUB_type(Sym *);
  Sym *instantiate(Sym *, Map<Sym *, Sym *> &substitutions);
  Sym *formal_to_generic(Sym*);
  Sym *new_Sym(char *name = 0);
  Fun *order_wrapper(Match *);
  Fun *coercion_wrapper(Match *);
  Fun *default_wrapper(Match *);
  Fun *instantiate_generic(Match *);
  void report_analysis_errors(Vec<ATypeViolation*> &type_violations);
};

class ASymbol : public IFASymbol {
 public:
  Sym *clone();
  char *pathname();
  int line();
  int log_line();
  int ast_id();
  ASymbol *copy();

  BaseAST *symbol;

  ASymbol();
};

class AAST : public IFAAST {
 public:
  char *pathname();
  int line();
  Sym *symbol();  
  IFAAST *copy_tree(ASTCopyContext* context);
  IFAAST *copy_node(ASTCopyContext* context);
  Vec<Fun *> *visible_functions(Sym *arg0);

  BaseAST *xast;        // pointer to shadowed BaseAST
  Code *code;           // IF1 Code (including children)
  Label *label[2];      // before and after for loops (continue,break)
  Sym *sym, *rval;      // IF1 Syms

  AAST();
};

#endif
