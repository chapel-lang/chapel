#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stmt.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stringutil.h"
#include "symbol.h"

//FIXME: Convert these over to the Chapel structures
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>

struct CCNode {
  CCNode* parent;

  int unique_id;

  std::vector<CCNode*> contains;
  std::vector<CCNode*> contained_by;

  CCNode() : parent(0) {}
};

typedef std::vector<std::pair<int, CCNode *> > CCNodeAssocList;
typedef std::vector<std::pair<int, CCNode *> >::iterator CCNodeAssocListIter;
typedef std::vector<std::pair<int, CCNode *> >::reverse_iterator CCNodeAssocListRIter;

struct CongruenceClosure {
  CCNodeAssocList node_assoc_list;
  std::vector<unsigned> scope_stops;

  void enter_scope() {
    scope_stops.push_back(node_assoc_list.size());
  }

  void exit_scope() {
    unsigned last_scope_stop;

    INT_ASSERT(!scope_stops.empty() && "Popping an empty scope");
    last_scope_stop = scope_stops.back();
    scope_stops.pop_back();

    //FIXME: this is pessimized, you can make this faster
    while (node_assoc_list.size() != last_scope_stop) {
      node_assoc_list.pop_back();
    }
  }

  CCNode *representative(CCNode *node) {
    while (node->parent)
      node = node->parent;

    return node;
  }

  CCNode *find_or_insert(BaseAST *ast) {
    int unique_id = get_symbol_id(ast);
    std::vector<CCNode *> children_nodes;

    if (BlockStmt *bs = toBlockStmt(ast)) {
      //FIXME: This is a shortcut but isn't strictly correct
      find_or_insert(bs->body.head);
    }
    else if (CallExpr *call = toCallExpr(ast)) {
      //Collect all the children first

      for_alist(arg, call->argList) {
        //FIXME: Not sure if this is strictly correct to use the representative here
        children_nodes.push_back(representative(find_or_insert(arg)));
      }
    }

    for (CCNodeAssocListRIter i = node_assoc_list.rbegin(), e = node_assoc_list.rend(); i != e; ++i) {
      if (unique_id == i->first) {
        CCNode *current_node = i->second;

        bool found_match = true;

        if (children_nodes.size() == current_node->contains.size()) {
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
    for (unsigned i = 0; i < children_nodes.size(); ++i) {
      // Set up the bi-directional link between containing type and contained type
      children_nodes[i]->contained_by.push_back(new_node);
      new_node->contains.push_back(children_nodes[i]);

      for (unsigned j = 0; j < children_nodes[i]->contained_by.size(); ++j) {
        equate_if_children_match(new_node, children_nodes[i]->contained_by[j]);
      }
    }

    node_assoc_list.push_back(std::make_pair(unique_id, new_node));

    return new_node;
  }

  //FIXME: This probably already exists in Chapel, but I couldn't find it
  int get_symbol_id(BaseAST *ast) {
    if (isType(ast)) {
      return ast->id;
    }
    else if (VarSymbol *vs = toVarSymbol(ast)) {
      return vs->type->id;
    }
    else if (TypeSymbol *ts = toTypeSymbol(ast)) {
      return ts->type->id;
    }
    else if (Symbol *s = toSymbol(ast)) {
      return s->id;
    }
    else if (isExpr(ast)) {
      if (DefExpr *de = toDefExpr(ast)) {
        if (de->init) {
          return get_symbol_id(de->init);
        }
        else if (de->exprType) {
          return get_symbol_id(de->exprType);
        }
        else if (de->sym) {
          return get_symbol_id(de->sym);
        }
      }
      else if (SymExpr *se = toSymExpr(ast)) {
        return get_symbol_id(se->var);
      }
      else if (CallExpr *ce = toCallExpr(ast)) {
        return get_symbol_id(ce->baseExpr);
      }
      else if (BlockStmt *bs = toBlockStmt(ast)) {
        //FIXME: This is a shortcut, it isn't strictly correct
        return get_symbol_id(bs->body.head);
      }
      else {
        INT_FATAL("Unimplemented case in getSymbolId(expr)");
      }
    }
    else {
      printf("TAG: %i\n", ast->astTag);
      INT_FATAL("Unimplemented case in getSymbolId(ast)");
    }
    INT_FATAL("Unimplemented case in getSymbolId");
    return 0;
  }

  CCNode *get_node(int id) {
    for (CCNodeAssocListRIter i = node_assoc_list.rbegin(), e = node_assoc_list.rend(); i != e; ++i) {
      if (id == i->first) {
        return i->second;
      }
    }
    return (CCNode *)0;
  }

  bool is_equal(BaseAST *ast1, BaseAST *ast2) {
    CCNode *node1 = find_or_insert(ast1);
    CCNode *node2 = find_or_insert(ast2);

    return is_equal_helper(node1, node2);
  }

  bool is_equal_helper(CCNode *node1, CCNode *node2) {
    node1 = representative(node1);
    node2 = representative(node2);

    return node1 == node2;
  }

  void equate(BaseAST *ast1, BaseAST *ast2) {
    CCNode *node1 = find_or_insert(ast1);
    CCNode *node2 = find_or_insert(ast2);

    equate_helper(node1, node2);
  }
  void equate_helper(CCNode *node1, CCNode *node2) {
    CCNode *node1_tmp = representative(node1);
    CCNode *node2_tmp = representative(node2);

    if (node1_tmp == node2_tmp)
      return;

    // Union the two trees
    node2_tmp->parent = node1_tmp;

    // Then, make sure that the "contained by" information is propagated to representative
    for (unsigned i = 0; i < node2->contained_by.size(); ++i) {
      node1_tmp->contained_by.push_back(node2->contained_by[i]);
    }

    // Do the rest of the closure.

    // Step #1, equate all contained nodes

    if (!node1->contains.empty() && !node2->contains.empty())
      INT_ASSERT(node1->contains.size() == node2->contains.size() && "Mismatched shape during union");

    if (node1->contains.size() == node2->contains.size()) {
      // if we have the same shape, we can pattern match and also union the contained nodes
      for (unsigned i = 0; i < node1->contains.size(); ++i) {
        equate_helper(node1->contains[i], node2->contains[i]);
      }
    }

    if (!node1->contained_by.empty() && !node2->contained_by.empty()) {
      for (unsigned i = 0; i < node1->contained_by.size(); ++i) {
        for (unsigned j = 0; j < node2->contained_by.size(); ++j) {
          equate_if_children_match(node1->contained_by[i], node2->contained_by[j]);
        }
      }
    }
  }

  void equate_if_children_match(CCNode *node1, CCNode *node2) {
    if (representative(node1) == representative(node2))
      return;

    if (node1->contains.size() == node2->contains.size()) {
      for (unsigned i = 0; i < node1->contains.size(); ++i) {
        if (!is_equal_helper(node1->contains[i], node2->contains[i]))
          return;
      }

      //std::cout << "Found matching children" << std::endl;
      CCNode *pnode1 = get_node(node1->unique_id);
      CCNode *pnode2 = get_node(node2->unique_id);

      if (pnode1 && (pnode1 == pnode2)) {
        equate_helper(node1, node2);
      }
      else
        return;
    }
  }
};

CongruenceClosure cclosure;


BaseAST *typeCheckFn(FnSymbol *fn);

class VisibleFunctionBlock2 {
 public:
  Map<const char*,Vec<FnSymbol*>*> visibleFunctions;
  VisibleFunctionBlock2() { }
};

class CallInfo2 {
 public:
  CallExpr*        call;        // call expression
  BlockStmt*       scope;       // module scope as in M.call
  const char*      name;        // function name
  Vec<Symbol*>     actuals;     // actual symbols
  Vec<const char*> actualNames; // named arguments
  CallInfo2(CallExpr* icall);
};

CallInfo2::CallInfo2(CallExpr* icall) : call(icall), scope(NULL) {
  if (SymExpr* se = toSymExpr(call->baseExpr))
    name = se->var->name;
  else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(call->baseExpr))
    name = use->unresolved;
  if (call->numActuals() >= 2) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (se->var == gModuleToken) {
        se->remove();
        se = toSymExpr(call->get(1));
        INT_ASSERT(se);
        ModuleSymbol* mod = toModuleSymbol(se->var);
        INT_ASSERT(mod);
        se->remove();
        scope = mod->block;
      }
    }
  }
  for_actuals(actual, call) {
    if (NamedExpr* named = toNamedExpr(actual)) {
      actualNames.add(named->name);
      actual = named->actual;
    } else {
      actualNames.add(NULL);
    }
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    Type* t = se->var->type;
    if (t == dtUnknown || t->symbol->hasFlag(FLAG_GENERIC))
      INT_FATAL(call, "the type of the actual argument '%s' is unknown or generic", se->var->name);
    actuals.add(se->var);
  }
}

static Map<BlockStmt*,VisibleFunctionBlock2*> visibleFunctionMap;
static int nVisibleFunctions = 0; // for incremental build
static Map<BlockStmt*,BlockStmt*> visibilityBlockCache;
static Vec<BlockStmt*> standardModuleSet;

static BlockStmt*
getVisibleFunctions(BlockStmt* block,
                    const char* name,
                    Vec<FnSymbol*>& visibleFns,
                    Vec<BlockStmt*>& visited) {
  //
  // all functions in standard modules are stored in a single block
  //
  if (standardModuleSet.set_in(block))
    block = theProgram->block;

  //
  // avoid infinite recursion due to modules with mutual uses
  //
  if (visited.set_in(block))
    return NULL;
  else if (isModuleSymbol(block->parentSymbol))
    visited.set_add(block);

  bool canSkipThisBlock = true;

  VisibleFunctionBlock2* vfb = visibleFunctionMap.get(block);
  if (vfb) {
    canSkipThisBlock = false; // cannot skip if this block defines functions
    Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name);
    if (fns) {
      visibleFns.append(*fns);
    }
  }

  if (block->modUses) {
    for_actuals(expr, block->modUses) {
      SymExpr* se = toSymExpr(expr);
      INT_ASSERT(se);
      ModuleSymbol* mod = toModuleSymbol(se->var);
      INT_ASSERT(mod);
      canSkipThisBlock = false; // cannot skip if this block uses modules
      getVisibleFunctions(mod->block, name, visibleFns, visited);
    }
  }

  //
  // visibilityBlockCache contains blocks that can be skipped
  //
  if (BlockStmt* next = visibilityBlockCache.get(block)) {
    getVisibleFunctions(next, name, visibleFns, visited);
    return (canSkipThisBlock) ? next : block;
  }

  if (block != rootModule->block) {
    BlockStmt* next = getVisibilityBlock(block);
    BlockStmt* cache = getVisibleFunctions(next, name, visibleFns, visited);
    if (cache)
      visibilityBlockCache.put(block, cache);
    return (canSkipThisBlock) ? cache : block;
  }

  return NULL;
}

static void buildVisibleFunctionMap2() {
  for (int i = nVisibleFunctions; i < gFnSymbols.n; i++) {
    FnSymbol* fn = gFnSymbols.v[i];
    if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->defPoint->parentSymbol &&
        !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block = NULL;
      if (fn->hasFlag(FLAG_AUTO_II)) {
        block = theProgram->block;
      } else {
        block = getVisibilityBlock(fn->defPoint);
        //
        // add all functions in standard modules to theProgram
        //
        if (standardModuleSet.set_in(block))
          block = theProgram->block;
      }
      VisibleFunctionBlock2* vfb = visibleFunctionMap.get(block);
      if (!vfb) {
        vfb = new VisibleFunctionBlock2();
        visibleFunctionMap.put(block, vfb);
      }
      Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(fn->name);
      if (!fns) {
        fns = new Vec<FnSymbol*>();
        vfb->visibleFunctions.put(fn->name, fns);
      }
      fns->add(fn);
    }
  }
  nVisibleFunctions = gFnSymbols.n;
}
/*
bool compareTypeExprs(Expr *lhs, Expr *rhs) {
  if (lhs && rhs) {
    printf("Checking %p (%i) %p (%i)\n", lhs, lhs->astTag, rhs, rhs->astTag);
  }
  else {
    printf("Checking NULL lhs/rhs: %p %p\n", lhs, rhs);
  }

  if (SymExpr *lhs_se = toSymExpr(lhs)) {
    if (SymExpr *rhs_se = toSymExpr(rhs)) {
      if (lhs_se->var->type && rhs_se->var->type) {
        printf("Both are resolved types: %s %s %p %p\n", lhs_se->var->cname,
            rhs_se->var->cname, lhs_se->var->type, rhs_se->var->type);

        //FIXME: Why do we special case rhs here?  Refactor to make more
        //flexible.
        if (VarSymbol *rhs_v = toVarSymbol(rhs_se->var)) {
          printf("  rhs is a var symbol\n");
          if (lhs_v->immediate &&  rhs_v->immediate) {
            printf("  rhs is an immediate\n");
            //If we've already resolved to a type, just use that
            return lhs_se->var->type == rhs_se->var->type;
          }
        }
        else if (TypeSymbol *rhs_ts = toTypeSymbol(rhs_se->var)) {
          printf("  rhs is a type symbol\n");
          //If we've already resolved to a type, just use that
          return lhs_se->var->type == rhs_ts->type;
        }

      }
      printf("Both are sym exprs: %s %s\n", lhs_se->var->cname,
             rhs_se->var->cname);

      //if (VarSymbol *v = toVarSymbol(lhs_se->var)) {
      //  if (v->immediate && v->immediate->const_kind == NUM_KIND_INT &&
      //      v->immediate->num_index == INT_SIZE_32) {
      //    return (!strcmp(rhs_se->var->cname, "int32_t"));
      //  }
      }

      return (lhs_se->var == rhs_se->var);
    }
  }
  else if (CallExpr *lhs_call = toCallExpr(lhs)) {
    if (CallExpr *rhs_call = toCallExpr(rhs)) {
      bool result = true;
      if ((lhs_call->baseExpr) && (rhs_call->baseExpr)) {
        result = result && compareTypeExprs(lhs_call->baseExpr,
                                            rhs_call->baseExpr);
      }
      for (Expr *e_lhs = lhs_call->argList.head,
             *e_rhs = rhs_call->argList.head; e_lhs; e_lhs = e_lhs->next,
             e_rhs = e_rhs->next) {
        if (!e_rhs) {
          return false;
        }
        result = result && compareTypeExprs(e_lhs, e_rhs);
      }
      return result;
    }
  }
  else if (BlockStmt *lhs_block = toBlockStmt(lhs)) {
    if (BlockStmt *rhs_block = toBlockStmt(rhs)) {
      bool result = true;
      for (Expr *e_lhs = lhs_block->body.head, *e_rhs = rhs_block->body.head;
           e_lhs; e_lhs = e_lhs->next, e_rhs = e_rhs->next) {
        if (!e_rhs) {
          return false;
        }
        result = result && compareTypeExprs(e_lhs, e_rhs);
      }
      return result;
    }
  }
  return false;
}
*/
Type *translateToType(Expr *type_expr) {
  //if it already has a type, use that
  if (type_expr->getValType() != dtUnknown) {
    return type_expr->getValType();
  }
  else {
    //Do something
    return NULL;
  }
}

Expr *translateToTypeExpr(Type *type) {
  if (type == dtBool) {
  }
  return NULL;

}

BaseAST *typeCheckExpr(BaseAST *currentExpr, BaseAST *expectedReturnTypeExpr) {
  if (SymExpr *se_actual = toSymExpr(currentExpr)) {
    printf("  CurrentSymExpr: %s (type: %p)\n", se_actual->var->name,
        se_actual->var->type);

    if (se_actual->var->type && se_actual->var->type != dtUnknown)
      return se_actual->var->type;

    ArgSymbol *argSym;
    DefExpr *defPt;
    if ((argSym = toArgSymbol(se_actual->var)) && argSym->typeExpr) {
      printf ("     Arg->%p\n", argSym->typeExpr);
      return argSym->typeExpr;
    }
    else if ((defPt = se_actual->var->defPoint)) {
      printf ("     Arg(defexpr)->%p %p %i\n", defPt->exprType, defPt->init,
              defPt->sym->astTag);
      if (defPt->exprType && defPt->init) {
        if (cclosure.is_equal(defPt->exprType, defPt->init)) {
          printf("Declared variable type matches initialization expression\n");
        }
        else {
          printf("ERROR: Declared variable type does not match initialization expression\n");
        }
      }
      else if (defPt->exprType) {
        return defPt->exprType;
      }
      else if (defPt->init) {
        return defPt->init;
      }
      else if (se_actual->var->type) {
        return currentExpr;
      }
      else {
        printf("ERROR: Generic variable without initializing expression\n");
      }
    }
    else if (/*Type *actualType =*/ se_actual->var->type) {
      // Stop here, we've hit an expression that's been resolved already
      return currentExpr;
      /*
      if (SymExpr *se_expected = toSymExpr(expectedReturnTypeExpr)) {
        printf("  ExpectedSymExpr: %s (type: %p)\n", se_expected->var->name,
            se_expected->var->type);
      }
      else if (BlockStmt *block = toBlockStmt(expectedReturnTypeExpr)) {
        printf("  Expected is block stmt\n", expectedReturnTypeExpr->astTag);
        if (Expr *contents = block->body.head) {
          if (SymExpr *se_contents = toSymExpr(contents)) {
            printf("  ExpectedSymExpr: %s (type: %p)\n", se_contents->var->name,
                se_contents->var->type);
          }
        }
      }
      else {
        printf("  Expected not a sym expression: %i\n", expectedReturnTypeExpr->astTag);
      }
      */
    }
  }
  else if (CallExpr* call = toCallExpr(currentExpr)) {
    printf("  CallExpr: %p\n", call);
    if (call->baseExpr) {
      printf("  BaseExpr:\n");
      typeCheckExpr(call->baseExpr, expectedReturnTypeExpr);
    }
    else {
      printf("  Primitive: %s\n", call->primitive->name);
    }
    //printf("  BEGIN ARGS\n");

    Vec<Expr*> checked_arg_exprs;
    /*
    //for_alist(e, call->argList) {
    for_actuals(e, call) {
      printf("  PRE-TYPECHECK:\n");
      Expr *e_typeExpr = typeCheckExpr(e, expectedReturnTypeExpr);
      printf("  TYPECHECKED: %p\n", e_typeExpr);
      checked_arg_exprs.add(e_typeExpr);
    }
    */
    if (call->primitive && (call->primitive->tag == PRIM_RETURN)) {
      printf("-->Expression check of return argument\n");
      BaseAST *e_typeAST = typeCheckExpr(call->argList.head, expectedReturnTypeExpr);
      printf("<--Done with expression check of return argument\n");
      if ((!isBlockStmt(e_typeAST)) && (isBlockStmt(expectedReturnTypeExpr))) {
        BlockStmt *retBody = toBlockStmt(expectedReturnTypeExpr);
        if (!cclosure.is_equal(e_typeAST, retBody->body.head)) {
          printf("**********ERROR: Mismatched type expressions in return\n");
        }
        else {
          printf("**********SUCCESS: Return expressions match: %p\n",
              e_typeAST);

          return e_typeAST;
        }
      }
      else{
        if (!cclosure.is_equal(e_typeAST, expectedReturnTypeExpr)) {
          printf("**********ERROR: Mismatched type expressions in return\n");
        }
        else {
          printf("**********SUCCESS: Return expressions match: %p\n",
              e_typeAST);

          return e_typeAST;
        }
      }
    }
    else if (call->primitive) {
      printf("  PRIMITIVE OP\n");
    }
    else if (!call->primitive) {
      //First, check to see if it should be a primitive but it hasn't been
      //resolved yet

      if (UnresolvedSymExpr *use = toUnresolvedSymExpr(call->baseExpr)) {
        printf("  Unresolved SymExpr: %s\n", use->unresolved);
        if (PrimitiveOp *op = primitives_map.get(use->unresolved)) {
          printf("Found a matching primitive op.  Type: %p\n",
              op->returnInfo(call));
          //FIXME: Come up with a better way to resolve here.
          if (BlockStmt *block = toBlockStmt(expectedReturnTypeExpr)) {
            expectedReturnTypeExpr = block->body.head;
          }
          if (SymExpr *expectedReturn_se = toSymExpr(expectedReturnTypeExpr)) {
            printf("  Expected type: %s %p\n", expectedReturn_se->var->cname,
                expectedReturn_se->var->type);
            printf("  Op type: %s %p\n", op->returnInfo(call)->symbol->cname,
                op->returnInfo(call));
            if (expectedReturn_se->var->type == op->returnInfo(call)) {
              return expectedReturnTypeExpr;
            }
          }

        }
      }

      CallInfo2 info(call);

      Vec<FnSymbol*> visibleFns;                    // visible functions
      /*
      Vec<FnSymbol*> candidateFns;
      Vec<Vec<ArgSymbol*>*> candidateActualFormals; // candidate functions
      */

      if (gFnSymbols.n != nVisibleFunctions)
        buildVisibleFunctionMap2();

      if (!call->isResolved()) {
        if (!info.scope) {
          Vec<BlockStmt*> visited;
          getVisibleFunctions(getVisibilityBlock(call), info.name, visibleFns,
                              visited);
        } else {
          if (VisibleFunctionBlock2* vfb = visibleFunctionMap.get(info.scope))
            if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info.name))
              visibleFns.append(*fns);
        }
      } else {
        visibleFns.add(call->isResolved());
      }

      forv_Vec(FnSymbol, visibleFn, visibleFns) {
        printf("$$$ Checking against: %s %s\n", visibleFn->name,
               visibleFn->cname);
        bool mismatch = false;
        Expr *e_actual = call->argList.head;
        ArgSymbol *s_formal = ((visibleFn)->formals.head) ?
          toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) : NULL;
        for (; e_actual; e_actual = e_actual->next,
               s_formal = (s_formal && s_formal->defPoint->next) ?
               toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) :
               NULL) {
          if (!s_formal) {
            mismatch = true;
            break;
          }
          printf("Comparing %i to %i\n", e_actual->astTag,
                 s_formal->typeExpr->astTag);
          if ((!isBlockStmt(e_actual)) && (isBlockStmt(s_formal->typeExpr))) {
            printf("Looking inside of body\n");
            BlockStmt *retBody = toBlockStmt(s_formal->typeExpr);
            if (!cclosure.is_equal(e_actual, retBody->body.head)) {
              mismatch = true;
              break;
            }
          }
          else {
            printf("NOT looking inside of body\n");
            if (!cclosure.is_equal(e_actual, s_formal->typeExpr)) {
              mismatch = true;
              break;
            }
          }
        }
        if (!mismatch) {
          printf("!Found matching function to check!\n");
          return typeCheckFn(visibleFn);
        }
      }
    }

    //printf("  END ARGS\n");
  }
  else if (UnresolvedSymExpr *use = toUnresolvedSymExpr(currentExpr)) {
    printf("  Unresolved SymExpr: %s\n", use->unresolved);
  }
  else if (DefExpr *de = toDefExpr(currentExpr)) {
    printf("  DefExpr: %p\n", de);
    }
  else if (NamedExpr *ne = toNamedExpr(currentExpr)) {
    printf("  NamedExpr: %p\n", ne);
  }
  else if (BlockStmt *block = toBlockStmt(currentExpr)) {
    printf("  BlockStmt: %p\n", block);
    Expr *ret = NULL;
    for_alist(e, block->body) {
      ret = toExpr(typeCheckExpr(e, expectedReturnTypeExpr));
    }
    return ret;
  }
  else if (CondStmt *cond = toCondStmt(currentExpr)) {
    printf("  CondStmt: %p\n", cond);
  }
  else if (GotoStmt *gotoStmt = toGotoStmt(currentExpr)) {
    printf("  GotoStmt: %p\n", gotoStmt);
  }
  else {
    if (currentExpr) {
      printf("  <expr type unknown: %i %i %i %i>\n", currentExpr->astTag, E_Expr,
             E_Symbol, E_Type);
    }
    else {
      printf("  <expr is NULL\n");
    }
  }
  printf("## RETURNING NULL\n");
  return NULL;
}

BaseAST *typeCheckFn(FnSymbol *fn) {
  printf("Checking function: %s\n", fn->cname);

  // Look through our formal arguments
  for_formals(formal, fn) {
    printf("  Formal: %s %p\n", formal->cname, formal->typeExpr);
    if (!formal->typeExpr) {
      printf("  ERROR: formal is missing explicit type annotation\n");
    }
  }
  if (!fn->retExprType) {
    printf("  ERROR: function missing explicit return type annotation\n");
  }
  else {
    printf("  Return: %p\n", fn->retExprType);
  }
  
  if (fn->where) {
    for_alist(expr, fn->where->body) {
      if (CallExpr *ce = toCallExpr(expr)) {
        if (UnresolvedSymExpr *callsymexpr = toUnresolvedSymExpr(ce->baseExpr)) {
          if (!strcmp(callsymexpr->unresolved, "==")) {
            //Equality constraint, let's add it
            BaseAST *arg1 = ce->argList.get(1);
            BaseAST *arg2 = ce->argList.get(2);
            cclosure.equate(arg1, arg2);
          }
        }
      }
    }
  }

  printf("Body:\n");

  // Now, look through the function body
  return typeCheckExpr(fn->body, fn->retExprType);
}

void earlyTypeCheck(void) { 
  bool found_early_type_checked = false;
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_SEPARATELY_TYPE_CHECKED)) {
      found_early_type_checked = true;
      typeCheckFn(fn);
    }
  }  
  if (found_early_type_checked) {
    //Hackish workaround to stop early when we're early type-checking until we
    //tie into the rest of the passes
    INT_FATAL("Done");
  }
}
