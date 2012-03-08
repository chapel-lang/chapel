#ifndef CONGRUENCE_CLOSURE_H
#define CONGRUENCE_CLOSURE_H

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stmt.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"

//FIXME: Convert these over to the Chapel structures
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <algorithm>

//FIXME: This probably already exists in Chapel, but I couldn't find it
//FIXME: A more optimized approach would be to use the congruence closure to find all representative types at
//        the beginning of a compile, and keep the information in the AST instead of looking it up like this once and
//        then forgetting it in later passes

struct CCNode {
  CCNode* parent;

  std::vector<CCNode*> implements;

  BaseAST *actualExprOrType;

  int unique_id;

  std::vector<CCNode*> contains;
  std::vector<CCNode*> contained_by;

  CCNode() :
      parent(0), actualExprOrType(0) {
  }
};

typedef std::vector<std::pair<int, CCNode *> > CCNodeAssocList;
typedef std::vector<std::pair<int, CCNode *> >::iterator CCNodeAssocListIter;
typedef std::vector<std::pair<int, CCNode *> >::reverse_iterator CCNodeAssocListRIter;

struct CongruenceClosure {
  CCNodeAssocList node_assoc_list;
  std::vector<unsigned> scope_stops;

  BaseAST *get_representative_ast(BaseAST *ast);
  void make_parent_null(BaseAST *ast);
  CCNode *representative(CCNode *node);
  bool has_implements_relation(BaseAST *implementer, BaseAST *implemented);
  void add_implements_witness(BaseAST *implementer, BaseAST *implemented);
  void print_map();
  BaseAST *get_root_type_or_type_expr(BaseAST *ast);
  CCNode *find_or_insert(BaseAST *ast);
  CCNode *get_node(int id);
  bool is_equal(BaseAST *ast1, BaseAST *ast2);
  bool is_equal_helper(CCNode *node1, CCNode *node2);
  void equate(BaseAST *ast1, BaseAST *ast2);
  void equate_helper(CCNode *node1, CCNode *node2);
  void equate_if_children_match(CCNode *node1, CCNode *node2);
};


#endif /* CONGRUENCE_CLOSURE_H */
