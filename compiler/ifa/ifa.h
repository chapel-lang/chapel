/* -*-Mode: c++;-*-
   Public interface classes and functions for Iterative Flow Analysis (IFA)
*/
#ifndef _ifa_H_
#define _ifa_H_

#include "vec.h"
#include "map.h"

class Sym;
class Match;
class Fun;
class ATypeViolation;
class ASTCopyContext;
class PNode;

/*
  Interface object between analysis symbols (Sym) and front end symbols
*/
class IFASymbol : public gc {
 public:
  virtual Sym *clone() = 0;
  virtual char *pathname() = 0;
  virtual int line() = 0;
  virtual int log_line() = 0;
  virtual int ast_id() = 0;
  virtual IFASymbol *copy() = 0;

  Sym *sym;

  IFASymbol() : sym(0) {}
};

/*
  Interface object between analysis to front end AST nodes
*/
class IFAAST : public gc {
 public:
  Vec<PNode *> pnodes;

  virtual char *pathname() = 0;
  virtual int line() = 0;
  virtual Sym *symbol() = 0;
  virtual Vec<Fun *> *visible_functions(Sym *arg0) { return NULL; }
  virtual IFAAST *copy_tree(ASTCopyContext* context) = 0;
  virtual IFAAST *copy_node(ASTCopyContext* context) = 0;
  virtual void dump(FILE *fp, Fun *f);
  virtual void graph(FILE *fp);
};
#define forv_IFAAST(_x, _v) forv_Vec(IFAAST, _x, _v)

/*
  Interface for callbacks from the analysis core to the front end specific translator
 */
class IFACallbacks : public gc {
public:
  virtual void finalize_functions() {}
  virtual Sym *make_LUB_type(Sym *s) { return s; }
  virtual Sym *instantiate(Sym *, Map<Sym *, Sym *> &substitutions) { return 0; }
  virtual Sym *formal_to_generic(Sym *s) { return s; }
  virtual Sym *new_Sym(char *name) = 0;
  virtual Fun* order_wrapper(Match *) { return 0; }
  virtual Fun* coercion_wrapper(Match *) { return 0; }
  virtual Fun* default_wrapper(Match *) { return 0; }
  virtual Fun* instantiate_generic(Match *) { return 0; }
  virtual void report_analysis_errors(Vec<ATypeViolation*> &type_violations) { }
};

void ifa_init(IFACallbacks *callbacks);
int ifa_analyze();
enum GraphType { GraphViz, VCG };
void ifa_graph(char *fn, int graph_type);
void ifa_html(char *fn);
void ifa_cg(char *fn);
void ifa_compile(char *fn);

#endif
