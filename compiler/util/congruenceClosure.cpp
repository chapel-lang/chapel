#include "congruenceClosure.h"

BaseAST *  CongruenceClosure::get_representative_ast(BaseAST *ast) {
  CCNode *rep = representative(find_or_insert(ast));
  return rep->actualExprOrType;
}

void CongruenceClosure::make_parent_null(BaseAST *ast){
  CCNode *rep = representative(find_or_insert(ast));
  rep->parent = NULL;
}

CCNode * CongruenceClosure::representative(CCNode *node) {
  while (node->parent)
    node = node->parent;

  return node;
}

bool CongruenceClosure::has_implements_relation(BaseAST *implementer, BaseAST *implemented) {
  CCNode *rep_implementer = representative(find_or_insert(implementer));
  CCNode *rep_implemented = representative(find_or_insert(implemented));

  if (std::find(rep_implementer->implements.begin(),
      rep_implementer->implements.end(), rep_implemented)
      != rep_implementer->implements.end())
    return true;
  else
    return false;
}

void CongruenceClosure::add_implements_witness(BaseAST *implementer, BaseAST *implemented) {
  CCNode *rep_implementer = representative(find_or_insert(implementer));
  CCNode *rep_implemented = representative(find_or_insert(implemented));

  if (std::find(rep_implementer->implements.begin(),
      rep_implementer->implements.end(), rep_implemented)
      == rep_implementer->implements.end()) {

    rep_implementer->implements.push_back(rep_implemented);
  }
}

void CongruenceClosure::print_map() {
  for (CCNodeAssocListRIter i = node_assoc_list.rbegin(), e =
      node_assoc_list.rend(); i != e; ++i) {

    printf("%i[%p]: ", i->first, i->second);

    CCNode *cc = i->second;

    while (cc) {
      printf("%i[%p %p] ", cc->unique_id, cc, cc->parent);
      if (!cc->parent) {
        for (unsigned j = 0; j < cc->contains.size(); ++j) {
          printf("(%i)", cc->contains[j]->unique_id);
        }
      }
      cc = cc->parent;
    }
    printf("\n");
  }
}

BaseAST * CongruenceClosure::get_root_type_or_type_expr(BaseAST *ast) {
  BaseAST *retval;

  if (isType(ast)) {
    if ((ast == dtUnknown) || (ast == dtAny))
      return NULL;
    else
      return ast;
  } else if (VarSymbol *vs = toVarSymbol(ast)) {
    if (vs->type && (retval = get_root_type_or_type_expr(vs->type)))
      return retval;
    else
      return vs;
  } else if (TypeSymbol *ts = toTypeSymbol(ast)) {
    if (ts->type && (retval = get_root_type_or_type_expr(ts->type)))
      return retval;
    else
      return ts;
  } else if (ArgSymbol *as = toArgSymbol(ast)) {
    if (as->typeExpr)
      return get_root_type_or_type_expr(as->typeExpr);
    else if (as->type && get_root_type_or_type_expr(as->type))
      return as->type;
    else
      return as;
  } else if (Symbol *s = toSymbol(ast)) {
    return s;
  } else if (isExpr(ast)) {
    if (DefExpr *de = toDefExpr(ast)) {
      if (de->init) {
        return get_root_type_or_type_expr(de->init);
      } else if (de->exprType) {
        return get_root_type_or_type_expr(de->exprType);
      } else if (de->sym) {
        return get_root_type_or_type_expr(de->sym);
      }
    } else if (SymExpr *se = toSymExpr(ast)) {
      return get_root_type_or_type_expr(se->var);
    } else if (CallExpr *ce = toCallExpr(ast)) {
      return ce;
    } else if (BlockStmt *bs = toBlockStmt(ast)) {
      //FIXME: is this strictly correct?
      return get_root_type_or_type_expr(bs->body.head);
    } else if (isUnresolvedSymExpr(ast)) {
      return dtUnknown;
    } else {
      INT_FATAL("Unimplemented case in get_root_type_or_type_expr(expr): %i\n",
          toExpr(ast)->astTag);
    }
  } else {
    INT_FATAL("Unimplemented case in get_root_type_or_type_expr(ast)");
  }
  INT_FATAL("Unimplemented case in get_root_type_or_type_expr");
  return 0;
}

CCNode * CongruenceClosure::find_or_insert(BaseAST *ast) {
  if (BaseAST *rootAST = get_root_type_or_type_expr(ast))
   ast = rootAST;

  int unique_id = ast->id;

  std::vector<CCNode *> children_nodes;

  if (CallExpr *call = toCallExpr(ast)) {
    //Insert the baseExpr
    unique_id = representative(find_or_insert(call->baseExpr))->unique_id;

    //Collect all the children
    for_alist(arg, call->argList) {
     //FIXME: Not sure if this is strictly correct to use the representative here
     children_nodes.push_back(representative(find_or_insert(arg)));
    }
  }

  for (CCNodeAssocListRIter i = node_assoc_list.rbegin(), e =
     node_assoc_list.rend(); i != e; ++i) {
    if (unique_id == i->first) {
      CCNode *current_node = i->second;

      bool found_match = false;

      if (children_nodes.size() == current_node->contains.size()) {
       found_match = true;
       for (unsigned j = 0; j < children_nodes.size(); ++j) {
         if (children_nodes[j] != current_node->contains[j]) {
           found_match = false;
           break;
         }
       }
      }
      if (!found_match)
       continue;

      return current_node;
    }
  }

  // Not found, so we insert and return that
  CCNode *new_node = new CCNode();
  new_node->unique_id = unique_id;
  new_node->actualExprOrType = ast;
  for (unsigned i = 0; i < children_nodes.size(); ++i) {
    // Set up the bi-directional link between containing type and contained type
    children_nodes[i]->contained_by.push_back(new_node);
    new_node->contains.push_back(children_nodes[i]);
  }

  node_assoc_list.push_back(std::make_pair(unique_id, new_node));

  return new_node;
}

CCNode * CongruenceClosure::get_node(int id) {
  for (CCNodeAssocListRIter i = node_assoc_list.rbegin(), e =
      node_assoc_list.rend(); i != e; ++i) {
    if (id == i->first) {
      return i->second;
    }
  }
  return (CCNode *) 0;
}

bool CongruenceClosure::is_equal(BaseAST *ast1, BaseAST *ast2) {
  CCNode *node1 = find_or_insert(ast1);
  CCNode *node2 = find_or_insert(ast2);

  return is_equal_helper(node1, node2);
}

bool CongruenceClosure::is_equal_helper(CCNode *node1, CCNode *node2) {
  node1 = representative(node1);
  node2 = representative(node2);

  return node1 == node2;
}

void CongruenceClosure::equate(BaseAST *ast1, BaseAST *ast2) {
  CCNode *node1 = find_or_insert(ast1);
  CCNode *node2 = find_or_insert(ast2);

  equate_helper(node1, node2);
}

void CongruenceClosure::equate_helper(CCNode *node1, CCNode *node2) {
  CCNode *node1_tmp = representative(node1);
  CCNode *node2_tmp = representative(node2);

  if (node1_tmp == node2_tmp)
    return;

  // Union the two trees, favoring the earlier ids
  // this favors symbols over exprs, and types over symbols (I think, please check)
  if (node1_tmp->unique_id < node2_tmp->unique_id)
    node2_tmp->parent = node1_tmp;
  else
    node1_tmp->parent = node2_tmp;

  // Then, make sure that the "contained by" information is propagated to representative
  for (unsigned i = 0; i < node2->contained_by.size(); ++i) {
    node1_tmp->contained_by.push_back(node2->contained_by[i]);
  }

  // Do the rest of the closure.

  // Step #1, equate all contained nodes
  if (!node1->contains.empty() && !node2->contains.empty())
    INT_ASSERT(
        node1->contains.size() == node2->contains.size() && "Mismatched shape during union");

  if (node1->contains.size() == node2->contains.size()) {
    // if we have the same shape, we can pattern match and also union the contained nodes
    for (unsigned i = 0; i < node1->contains.size(); ++i) {
      equate_helper(node1->contains[i], node2->contains[i]);
    }
  }

  // Step #2, equate contained_by nodes which match by id
  if (!node1->contained_by.empty() && !node2->contained_by.empty()) {
    for (unsigned i = 0; i < node1->contained_by.size(); ++i) {
      for (unsigned j = 0; j < node2->contained_by.size(); ++j) {
        equate_if_children_match(node1->contained_by[i],
            node2->contained_by[j]);
      }
    }
  }
}

void CongruenceClosure::equate_if_children_match(CCNode *node1, CCNode *node2) {
  if (representative(node1) == representative(node2))
    return;

  if (node1->unique_id != node2->unique_id)
    return;

  if (node1->contains.size() != node2->contains.size())
    return;

  for (unsigned i = 0; i < node1->contains.size(); ++i) {
    if (!is_equal_helper(node1->contains[i], node2->contains[i]))
      return;
  }

  equate_helper(node1, node2);
}






