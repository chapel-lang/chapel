/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _ast_H_
#define _ast_H_

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

enum Contructor { Make_TUPLE, Make_VECTOR, Make_SET };

extern char *AST_name[];

typedef Map<char *, AST *> LabelMap;

class AST : public Vec<AST *> {
 public:
  AST_kind kind;
  uint scope_kind:2;
  uint constructor:2;
  uint def_record_type:1;
  uint def_ident_label:1;
  uint op_index:1;
  uint in_tuple:1;
  uint in_apply:1;
  uint is_const:1;
  Sym *sym;
  char *string;
  char *builtin;
  Prim *prim;
  char *pathname;
  int line;
  Scope *scope;
  char *constant_type;

  Immediate val;	// currently unused (constants use string instead)

  Label *label[2];	// before and after for loops (continue,break)
  Code	*code;
  Sym	*rval, *lval;
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
void ast_print(FILE *fp, AST *a, int indent = 0);
void ast_print_recursive(FILE *fp, AST *a, int indent = 0);
void ast_write(AST *a, char *filename);

inline AST *ast_qualified_ident_ident(AST *x) { return x->v[x->n-1]; }
Scope *ast_qualified_ident_scope(AST *qualified_ident, Scope *global);
Sym *ast_qualified_ident_sym(AST *qualified_ident, Scope *global);

#endif
