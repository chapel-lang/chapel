/* -*-Mode: c++;-*-
*/
#ifndef _ast_H_
#define _ast_H_

#include <stdio.h>
#include "gc_cpp.h"
#include "map.h"
#include "vec.h"

class Fun;
class PNode;
class Sym;
class IF1;
class AST;
class BaseAST;
class Var;

class ASTCopyContext : public gc {
 public:
  Map<Fun *, Fun *> fmap;
  Map<PNode *, PNode *> *nmap;
  Map<Var *, Var *> *vmap;
  Map<Sym *, Sym *> smap;
  ASTCopyContext() : nmap(0), vmap(0) {}
};

class AST : public gc {
 public:
  Vec<PNode *> pnodes;

  virtual char *pathname() = 0;
  virtual int line() = 0;
  virtual Sym *symbol() = 0;
  virtual Vec<Fun *> *visible_functions(Sym *arg0) { return NULL; }
  virtual AST *copy_tree(ASTCopyContext* context) = 0;
  virtual AST *copy_node(ASTCopyContext* context) = 0;
  virtual void dump(FILE *fp, Fun *f);
  virtual void graph(FILE *fp);
};
#define forv_AST(_x, _v) forv_Vec(AST, _x, _v)

void build_module(Sym *sym, Sym *init);
void init_ast();
void finalize_types(IF1 *);
void make_type_sym(Sym *s);
void build_type_hierarchy();

#endif
