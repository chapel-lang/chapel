/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _ast_H_
#define _ast_H_

#include <stdio.h>
#include "map.h"
#include "scope.h"

// C++'s manditory heap'o forward declarations
class IF1;
class Code;
class Sym;
struct D_ParseNode;
class Prim;
class Label;
class AST;
class PNode;
typedef Map<PNode *, PNode *> PNodeMap;

// see ast_kinds.h for details
enum AST_kind {
#define S(_x) AST_##_x,
#include "ast_kinds.h"
#undef S
  AST_MAX
};

enum Contructor { Make_TUPLE, Make_VECTOR, Make_SET };

extern char *AST_name[];

typedef Map<char *, AST *> LabelMap;

class AST : public Vec<AST *> {
 public:
  AST_kind kind;
  unsigned int scope_kind:2;       // 0 = {}, 1 = () (BLC:???)
  unsigned int constructor:2;
  unsigned int def_record_type:1;
  unsigned int def_ident_label:1;
  unsigned int op_index:1;
  unsigned int is_var:1;
  unsigned int is_const:1;
  unsigned int is_value:1;
  unsigned int in_tuple:1;
  unsigned int in_apply:1;
  unsigned int is_assign:1;
  unsigned int is_simple_assign:1;
  unsigned int is_ref:1;
  unsigned int is_application:1;
  unsigned int is_comma:1;
  unsigned int is_inc_dec:1;
  unsigned int rank;
  Sym *sym;
  char *string;
  char *builtin;
  Prim *prim;
  char *pathname;
  int line;
  Scope *scope;
  char *constant_type;

  Label *label[2];	// before and after for loops (continue,break)
  Code	*code;
  Sym	*rval;
  Vec<PNode *> pnodes;
  
  AST *last() { return v[n-1]; }
  void add(AST *a);
  void add(D_ParseNode *pn);
  void add_below(D_ParseNode *pn);
  void set_location(D_ParseNode *pn);
  AST *get(AST_kind k);

  AST *copy(PNodeMap *nmap = 0);
  AST(AST_kind k, D_ParseNode *pn = 0);
};
#define forv_AST(_x, _v) forv_Vec(AST, _x, _v)

int ast_gen_if1(IF1 *if1, Vec<AST *> &av);
int ast_constant_fold(IF1 *if1, AST *ast);
void ast_print(FILE *fp, AST *a, int indent = 0);
void ast_print_recursive(FILE *fp, AST *a, int indent = 0);
void ast_write(AST *a, char *filename);

inline AST *ast_qualified_ident_ident(AST *x) { return x->v[x->n-1]; }
Scope *ast_qualified_ident_scope(AST *qualified_ident, Scope *global);
Sym *ast_qualified_ident_sym(AST *qualified_ident, Scope *global);

Sym *new_sym(IF1 *i, Scope *scope = 0, char *s = 0, Sym *sym = 0);

extern char *cannonical_class;
extern char *cannonical_self;
extern char *cannonical_folded;

#endif
