/* -*-Mode: c++;-*-
   Public interface classes and functions for Iterative Flow Analysis (IFA)
*/
#ifndef _ifa_H_
#define _ifa_H_

#include "defs.h"

class Sym;
class Match;
class Fun;
class ATypeViolation;
class ASTCopyContext;
class PNode;
class MPosition;

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
  virtual Sym *new_Sym(char *name) = 0;
  virtual Sym *make_LUB_type(Sym *s) { return s; }
  virtual int formal_to_generic(Sym *s, Sym **ret_generic, int *ret_bind_to_value) { return false; }
  virtual Sym *instantiate(Sym *, Map<Sym *, Sym *> &substitutions) { return 0; }
  virtual Fun* order_wrapper(Fun *, Map<MPosition *, MPosition *> &substitutions) { return 0; }
  virtual Sym *promote(Fun *, Sym *, Sym *, Sym *) { return NULL; }
  virtual Fun* promotion_wrapper(Fun *, Map<MPosition *, Sym *> &substitutions) { return 0; }
  virtual Sym *coerce(Sym *actual, Sym *formal) { return NULL; }
  virtual Fun* coercion_wrapper(Fun *, Map<MPosition *, Sym *> &substitutions) { return 0; }
  virtual Fun* default_wrapper(Fun *, Vec<MPosition *> &defaults) { return 0; }
  virtual Fun* instantiate_generic(Fun *, Map<Sym *, Sym*> &substitutions) { return 0; }
  virtual void report_analysis_errors(Vec<ATypeViolation*> &type_violations) { }
};

void ifa_init(IFACallbacks *callbacks);
int ifa_analyze(char *fn);
enum GraphType { GraphViz, VCG };
extern int graph_type;
void ifa_graph(char *fn);
void ifa_html(char *fn, char *mktree_dir);
void ifa_cg(char *fn);
void ifa_compile(char *fn);

extern int print_call_depth;
extern int fanalysis_errors;
extern int fgraph_pass_contours;
extern int fdce_if1;

#endif
