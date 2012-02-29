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

//Predeclare functions
BaseAST *get_root_type_or_type_expr(BaseAST *ast);
BaseAST *typeCheckExpr(BaseAST *currentExpr, BaseAST *expectedReturnTypeExpr, Map<FnSymbol*, ArgSymbol*>&);
void handle_where_clause_expr(BaseAST *ast);
BaseAST *typeCheckFn(FnSymbol *fn);
BaseAST *checkInterfaceImplementations(BaseAST *s);
Symbol* mapArguments(CallExpr*, FnSymbol*, CallExpr*);
BaseAST* checkFunctionCall(CallExpr*);
void addAdaptationToWitness(BaseAST*, ClassType *witness, FnSymbol *requiredFn, FnSymbol *actualFn);
static void addToImplementsSymbolTable(CallExpr* ce, BaseAST* interface, BaseAST* implementingType, Symbol *implementingWitness);

void getMatchingFunctionsInInterfaces(const char* name, Vec<FnSymbol*>& visibleFns,
    Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces);
static void getWitnessesInWhereClause(Vec<CallExpr*>& implementsClauses, BaseAST *whereClause);
static void getFunctionsInWhereClause(Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces,
    Vec<ArgSymbol*>& witnessObjects, BaseAST *whereClause);

//FIXME: This probably already exists in Chapel, but I couldn't find it
//FIXME: A more optimized approach would be to use the congruence closure to find all representative types at
//        the beginning of a compile, and keep the information in the AST instead of looking it up like this once and
//        then forgetting it in later passes
BaseAST *get_root_type_or_type_expr(BaseAST *ast) {
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

  BaseAST *get_representative_ast(BaseAST *ast) {
    CCNode *rep = representative(find_or_insert(ast));
    return rep->actualExprOrType;
  }

  void make_parent_null(BaseAST *ast){
    CCNode *rep = representative(find_or_insert(ast));
    rep->parent = NULL;
  }

  CCNode *representative(CCNode *node) {
    while (node->parent)
      node = node->parent;

    return node;
  }

  bool has_implements_relation(BaseAST *implementer, BaseAST *implemented) {
    CCNode *rep_implementer = representative(find_or_insert(implementer));
    CCNode *rep_implemented = representative(find_or_insert(implemented));

    if (std::find(rep_implementer->implements.begin(),
        rep_implementer->implements.end(), rep_implemented)
        != rep_implementer->implements.end())
      return true;
    else
      return false;
  }

  void add_implements_witness(BaseAST *implementer, BaseAST *implemented) {
    CCNode *rep_implementer = representative(find_or_insert(implementer));
    CCNode *rep_implemented = representative(find_or_insert(implemented));

    if (std::find(rep_implementer->implements.begin(),
        rep_implementer->implements.end(), rep_implemented)
        == rep_implementer->implements.end()) {

      rep_implementer->implements.push_back(rep_implemented);
    }
  }

  void print_map() {
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

  CCNode *find_or_insert(BaseAST *ast) {
    //int unique_id = get_symbol_id(ast);
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

  CCNode *get_node(int id) {
    for (CCNodeAssocListRIter i = node_assoc_list.rbegin(), e =
        node_assoc_list.rend(); i != e; ++i) {
      if (id == i->first) {
        return i->second;
      }
    }
    return (CCNode *) 0;
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

  void equate_if_children_match(CCNode *node1, CCNode *node2) {
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
};

CongruenceClosure cclosure;

/*
 * BEGIN DUPLICATE CODE
 * FIXME: Need to refactor once we know what we need here
 */

class VisibleFunctionBlock2 {
public:
  Map<const char*, Vec<FnSymbol*>*> visibleFunctions;
  VisibleFunctionBlock2() {
  }
};

/*
 * FIXME: Took out isResolved optimization.  Need to put that back in later
 * for speed improvements.
 */
class CallInfo2 {
public:
  //CallExpr*        call;        // call expression
  //FnSymbol*        interface;   // function interface
  BlockStmt* scope; // module scope as in M.call
  const char* name; // function name
  Vec<Symbol*> actuals; // actual symbols
  Vec<const char*> actualNames; // named arguments
  CallInfo2(CallExpr* icall);
  CallInfo2(FnSymbol* iface);
};

CallInfo2::CallInfo2(FnSymbol* fn) :
    scope(NULL) {
  name = fn->name;

  for_alist (formal, fn->formals) {
    if (DefExpr *de = toDefExpr(formal)) {
      actualNames.add(de->sym->cname);
      BaseAST *tmp = cclosure.get_representative_ast(de);
      if (Symbol *s = toSymbol(tmp)) {
        actuals.add(s);
      } else if (Type *t = toType(tmp)) {
        actuals.add(t->symbol);
      } else {
        INT_FATAL("Unimplemented: trying to handle non-Symbol representation");
      }
    } else {
      INT_FATAL("Unimplemented case in CallInfo2(FnSymbol* fn).  Not DefExpr.");
    }
  }
}

CallInfo2::CallInfo2(CallExpr* icall) :
    scope(NULL) {
  if (SymExpr* se = toSymExpr(icall->baseExpr))
    name = se->var->name;
  else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(icall->baseExpr))
    name = use->unresolved;
  if (icall->numActuals() >= 2) {
    if (SymExpr* se = toSymExpr(icall->get(1))) {
      if (se->var == gModuleToken) {
        se->remove();
        se = toSymExpr(icall->get(1));
        INT_ASSERT(se);
        ModuleSymbol* mod = toModuleSymbol(se->var);
        INT_ASSERT(mod);
        se->remove();
        scope = mod->block;
      }
    }
  }
  for_actuals(actual, icall) {
    if (NamedExpr* named = toNamedExpr(actual)) {
      actualNames.add(named->name);
      actual = named->actual;
    } else {
      actualNames.add(NULL);
    }
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    /*
    Type* t = se->var->type;
    if (t == dtUnknown || t->symbol->hasFlag(FLAG_GENERIC))
      INT_FATAL(icall,
          "the type of the actual argument '%s' is unknown or generic",
          se->var->name);
    */
    actuals.add(se->var);
  }
}

static Map<BlockStmt*, VisibleFunctionBlock2*> visibleFunctionMap;
static int nVisibleFunctions = 0; // for incremental build
static Map<BlockStmt*, BlockStmt*> visibilityBlockCache;
static Vec<BlockStmt*> standardModuleSet;

static BlockStmt*
getVisibleFunctions(BlockStmt* block, const char* name,
    Vec<FnSymbol*>& visibleFns, Vec<BlockStmt*>& visited) {
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
    if (!fn->hasFlag(FLAG_INVISIBLE_FN)
        && fn->defPoint->parentSymbol && !isArgSymbol(fn->defPoint->parentSymbol)
        && !isInterfaceSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt * block = NULL;
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
 * END DUPLICATE CODE
 */

/*
 * Start of code duplication for SymbolTable required for scoped implements clauses
 * Code taken from scopeResolve.cpp
 * Code is modified - structure of SymbolTable
 */

typedef Map<BaseAST*, Symbol*> ImplementingTypeList;
typedef Map<BaseAST*, ImplementingTypeList*> ImplementedInterfaceList;
typedef Map<BaseAST*, ImplementedInterfaceList*> ImplementsSymbolTable;

static ImplementsSymbolTable implementsSymbolTable;

//
// getScope returns the BaseAST that corresponds to the scope where
// 'ast' exists; 'ast' must be an Expr or a Symbol.  Note that if you
// pass this a BaseAST that defines a scope, the BaseAST that defines
// the scope where it exists will be returned.  Thus if a BlockStmt
// nested in another BlockStmt is passed to getScope, the outer
// BlockStmt will be returned.
//
static BaseAST*
getScope2(BaseAST* ast) {
  if (Expr* expr = toExpr(ast)) {
    BlockStmt* block = toBlockStmt(expr->parentExpr);
    if (block && block->blockTag != BLOCK_SCOPELESS) {
      return block;
    } else if (expr->parentExpr) {
      return getScope2(expr->parentExpr);
    } else if (FnSymbol* fn = toFnSymbol(expr->parentSymbol)) {
      return fn;
    } else if (TypeSymbol* ts = toTypeSymbol(expr->parentSymbol)) {
      if (isEnumType(ts->type) || isClassType(ts->type)) {
        return ts;
      }
    }
    if (expr->parentSymbol == rootModule)
      return NULL;
    else
      return getScope2(expr->parentSymbol->defPoint);
  } else if (Symbol* sym = toSymbol(ast)) {
    if (sym == rootModule)
      return NULL;
    else
      return getScope2(sym->defPoint);
  }
  INT_FATAL(ast, "getScope2 expects an Expr or a Symbol");
  return NULL;
}

static void
addToImplementsSymbolTable(CallExpr* ce, BaseAST* interface, BaseAST* implementingType,
    Symbol *implementingWitness){
  BaseAST* scope = getScope2(ce);
  BaseAST* interface_node = cclosure.get_representative_ast(interface);
  BaseAST* implementingType_node = cclosure.get_representative_ast(implementingType);
  ImplementedInterfaceList* interfaceEntry = implementsSymbolTable.get(scope);
  if(!interfaceEntry) {
    interfaceEntry = new ImplementedInterfaceList();
    implementsSymbolTable.put(scope,interfaceEntry);
  }
  ImplementingTypeList* typeEntry = interfaceEntry->get(interface);
  if(!typeEntry) {
    typeEntry = new ImplementingTypeList();
    interfaceEntry->put(interface_node,typeEntry);
  }
  typeEntry->put(implementingType_node,implementingWitness);
}

static bool
lookupImplementsSymbolTable(BaseAST* ce, BaseAST* interface, BaseAST* implementingType) {
  BaseAST* scope = getScope2(ce);
  if(!scope)
    return false;
  BaseAST* interface_node = cclosure.get_representative_ast(interface);
  BaseAST* implementingType_node = cclosure.get_representative_ast(implementingType);
  ImplementedInterfaceList* interfaceEntry = implementsSymbolTable.get(scope);
  if(!interfaceEntry) {
    return lookupImplementsSymbolTable(scope,interface,implementingType);
  }
  ImplementingTypeList* typeEntry = interfaceEntry->get(interface_node);
  if(!typeEntry)
    return lookupImplementsSymbolTable(scope,interface,implementingType);
  Symbol* witness = typeEntry->get(implementingType_node);
  if(!witness)
    return lookupImplementsSymbolTable(scope,interface,implementingType);
  return true;
}

static Symbol*
lookupImplementsWitnessInSymbolTable(BaseAST* ce, BaseAST* interface, BaseAST* implementingType) {
  BaseAST* scope = getScope2(ce);
  if(!scope)
    return NULL;
  BaseAST* interface_node = cclosure.get_representative_ast(interface);
  BaseAST* implementingType_node = cclosure.get_representative_ast(implementingType);
  ImplementedInterfaceList* interfaceEntry = implementsSymbolTable.get(scope);
  if(!interfaceEntry) {
    return lookupImplementsWitnessInSymbolTable(scope,interface,implementingType);
  }
  ImplementingTypeList* typeEntry = interfaceEntry->get(interface_node);
  if(!typeEntry)
    return lookupImplementsWitnessInSymbolTable(scope,interface,implementingType);
  Symbol* witness = typeEntry->get(implementingType_node);
  if(!witness)
    return lookupImplementsWitnessInSymbolTable(scope,interface,implementingType);
  return witness;
}

/*
 * END DUPLICATE CODE 2
 */

typedef MapElem<FnSymbol *, ArgSymbol *> DictElem;

void getMatchingFunctionsInInterfaces(const char* name, Vec<FnSymbol*>& visibleFns,
    Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces) {

  form_Map(DictElem, elem, fnsInInterfaces) {
    if (!strcmp(elem->key->name, name)) {
      visibleFns.add(elem->key);
    }
  }
}

// Typechecks the given ast node with the expected return (in case a return
// is encountered)
BaseAST *typeCheckExpr(BaseAST *currentExpr, BaseAST *expectedReturnTypeExpr,
    Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces) {

  if (SymExpr *se_actual = toSymExpr(currentExpr)) {

    if (se_actual->var->type && se_actual->var->type != dtUnknown)
      return se_actual->var->type;

    ArgSymbol *argSym;
    DefExpr *defPt;
    if ((argSym = toArgSymbol(se_actual->var)) && argSym->typeExpr) {
      return cclosure.get_representative_ast(argSym->typeExpr);
    } else if ((defPt = se_actual->var->defPoint)) {
      return typeCheckExpr(defPt, expectedReturnTypeExpr, fnsInInterfaces);
    }
  } else if (CallExpr* call = toCallExpr(currentExpr)) {
    Vec<Expr*> checked_arg_exprs;

    if (call->primitive && (call->primitive->tag == PRIM_RETURN)) {
      BaseAST *e_typeAST = typeCheckExpr(call->argList.head,
          expectedReturnTypeExpr, fnsInInterfaces);

      if (!cclosure.is_equal(e_typeAST, expectedReturnTypeExpr)) {
        INT_FATAL("Mismatched type expressions in return\n");
      } else {
        return e_typeAST;
      }
    } else if (call->primitive) {
      INT_FATAL("UNIMPLEMENTED: PRIMITIVE OP\n");
    } else if (!call->primitive) {
      //First, check to see if it should be a primitive but it hasn't been
      //resolved yet

      if (UnresolvedSymExpr *use = toUnresolvedSymExpr(call->baseExpr)) {
        if (PrimitiveOp *op = primitives_map.get(use->unresolved)) {
          if (call->argList.length == 0) {
            return op->returnInfo(call, NULL, NULL);
          } else if (call->argList.length == 1) {
            return op->returnInfo(call,
                cclosure.get_representative_ast(call->argList.get(1)), NULL);
          } else {
            return op->returnInfo(call,
                cclosure.get_representative_ast(call->argList.get(1)),
                cclosure.get_representative_ast(call->argList.get(2)));
          }
        }
      }

      CallInfo2 info(call);

      Vec<FnSymbol*> visibleFns; // visible functions

      if (gFnSymbols.n != nVisibleFunctions)
        buildVisibleFunctionMap2();

      if (!call->isResolved()) {
        // Also include the functions that come into scope because of
        // implements statements in the where clause
        //getFunctionsInWhereClause(info.name, visibleFns, whereClause);
        getMatchingFunctionsInInterfaces(info.name, visibleFns, fnsInInterfaces);

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
          bool mismatch = false;
          Expr *e_actual = call->argList.head;
          ArgSymbol *s_formal =
              ((visibleFn)->formals.head) ?
                  toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) : NULL;
          for (;
              e_actual;
              e_actual = e_actual->next, s_formal =
                  (s_formal && s_formal->defPoint->next) ?
                      toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) :
                      NULL) {
            if (!s_formal) {
              mismatch = true;
              break;
            }
            if (s_formal->hasFlag(FLAG_TYPE_VARIABLE)) {
              if (SymExpr *se_actual = toSymExpr(e_actual)) {
                if (!isTypeSymbol(se_actual->var)) {
                  mismatch = true;
                  break;
                } else {
                  cclosure.equate(s_formal,se_actual->var);
                }
              } else {
                mismatch = true;
                break;
              }
            } else if (!cclosure.is_equal(e_actual, s_formal->typeExpr)) {
              mismatch = true;
              break;
            }
          }
          if (!mismatch) {
            if (ArgSymbol *obj = fnsInInterfaces.get(visibleFn)) {
              //printf("Converted to method\n");
              call->baseExpr->replace(new CallExpr(".", new SymExpr(obj),
                  new_StringSymbol(info.name)));
            }
            if (visibleFn->body->body.length == 0) {
              return visibleFn->retExprType;
            }
            else {

              return typeCheckFn(visibleFn);
            }
          }
        }
      INT_FATAL("No matching functions at call");
    }
  } else if (UnresolvedSymExpr *use = toUnresolvedSymExpr(currentExpr)) {
    INT_FATAL("UNIMPLEMENTED: Unresolved SymExpr: %s\n", use->unresolved);
  } else if (DefExpr *de = toDefExpr(currentExpr)) {
    if (de->exprType && de->init) {
      if (!cclosure.is_equal(de->exprType, de->init)) {
        INT_FATAL(
            "Declared variable type does not match initialization expression\n");
      }
    } else if (de->exprType) {
      return de->exprType;
    } else if (de->init) {
      return de->init;
    } else {
      INT_FATAL("Generic variable without initializing expression\n");
    }
  } else if (NamedExpr *ne = toNamedExpr(currentExpr)) {
    return typeCheckExpr(ne->actual, expectedReturnTypeExpr, fnsInInterfaces);
  } else if (BlockStmt *block = toBlockStmt(currentExpr)) {
    BaseAST *ret = NULL;
    for_alist(e, block->body) {
      ret = typeCheckExpr(e, expectedReturnTypeExpr, fnsInInterfaces);
    }
    return ret;
  } else if (CondStmt *cond = toCondStmt(currentExpr)) {
    //INT_FATAL("UNIMPLEMENTED: ContStmt\n");
    if (CallExpr *cond_test = toCallExpr(cond->condExpr)) {
      BaseAST *conditional = typeCheckExpr(cond_test->argList.head,
          expectedReturnTypeExpr, fnsInInterfaces);
      if (cclosure.is_equal(conditional, dtBool)) {
        typeCheckExpr(cond->thenStmt, expectedReturnTypeExpr, fnsInInterfaces);
        typeCheckExpr(cond->elseStmt, expectedReturnTypeExpr, fnsInInterfaces);
      }
      else {
        INT_FATAL("Expected boolean expression in if condition");
      }
    }
    else {
      INT_FATAL("Expected boolean expression in if condition");
    }
  } else if (GotoStmt *gotoStmt = toGotoStmt(currentExpr)) {
    typeCheckExpr(gotoStmt->label, expectedReturnTypeExpr, fnsInInterfaces);
  } else {
    if (currentExpr) {
      INT_FATAL("UNIMPLEMENTED: <expr type unknown: %i %i %i %i>\n",
          currentExpr->astTag, E_Expr, E_Symbol, E_Type);
    } else {
      INT_FATAL("UNIMPLEMENTED: <expr is NULL>\n");
    }
  }
  return NULL;
}

static void getWitnessesInWhereClause(Vec<CallExpr*>& implementsClauses, BaseAST *whereClause) {
  if (CallExpr *ce = toCallExpr(whereClause)) {
    if (UnresolvedSymExpr *callsymexpr = toUnresolvedSymExpr(ce->baseExpr)) {
      if (!strcmp(callsymexpr->unresolved, "implements")) {
        //First find the interface that was implemented and open it up
        BaseAST *interface_implemented = ce->argList.get(2);
        if (SymExpr *se = toSymExpr(interface_implemented)) {
          // Is a symexpr
          if (isInterfaceSymbol(se->var)) {
            implementsClauses.add(ce);
          }
          else {
            INT_FATAL("Implementation phrase with non-interface symbol");
          }
        }
        else {
          // Isn't a symexpr, something is wrong
        }
      } else if (!strcmp(callsymexpr->unresolved, "_build_tuple")) {
        //We have multiple constraints, handle them
        for_alist(arg, ce->argList) {
          handle_where_clause_expr(arg);
        }
      }
    }
    else {
      //printf("Not unresolved\n");
    }
  }
  else if (BlockStmt *block = toBlockStmt(whereClause)) {
    for_alist(expr, block->body) {
      getWitnessesInWhereClause(implementsClauses, expr);
    }
  }
}

/*
static void
getFunctionsInWhereClause(const char* name, Vec<FnSymbol*>& visibleFns,
    BaseAST *whereClause) {
*/

static void
getFunctionsInWhereClause(Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces,
    Vec<ArgSymbol*>& witnessObjects, BaseAST *whereClause) {

  Vec<CallExpr*> implementsClauses;
  getWitnessesInWhereClause(implementsClauses, whereClause);

  forv_Vec(CallExpr, ce, implementsClauses) {
    //First find the interface that was implemented and open it up
    SymExpr *implementing_type = toSymExpr(ce->argList.get(1));
    BaseAST *interface_implemented = ce->argList.get(2);
    if (SymExpr *se = toSymExpr(interface_implemented)) {
      // Is a symexpr
      if (InterfaceSymbol *is = toInterfaceSymbol(se->var)) {

        // Create an symbol that will represent this dictionary
        ArgSymbol *dict = new ArgSymbol(INTENT_BLANK,
            astr(implementing_type->var->name, "_impl_", se->var->name), dtAny);
        dict->addFlag(FLAG_GENERIC);

        witnessObjects.add(dict);

        addToImplementsSymbolTable(ce, is, se, dict);

        forv_Vec(FnSymbol, fn, is->functionSignatures) {
          //For now, copy the function prototype and replace the types
          //with the ones we know
          FnSymbol *fn_copy = fn->copy();
          fn_copy->addFlag(FLAG_INVISIBLE_FN);

          //replace formal types
          for_formals(arg, fn_copy) {
            //list_view(arg);
            if (BlockStmt *bs = toBlockStmt(arg->typeExpr)) {
              if (UnresolvedSymExpr *use = toUnresolvedSymExpr(bs->body.head)) {
                if (!strcmp(use->unresolved, "self")) {
                  bs->body.head = ce->argList.get(1)->copy();
                }
              }
            }
          }
          //replace return type
          if (BlockStmt *bs = toBlockStmt(fn_copy->retExprType)) {
            if (UnresolvedSymExpr *use = toUnresolvedSymExpr(bs->body.head)) {
              if (!strcmp(use->unresolved, "self")) {
                bs->body.head = ce->argList.get(1)->copy();
              }
            }
          }

          fnsInInterfaces.put(fn_copy, dict);
          //list_view(fn_copy);
        }
      }
      else {
        INT_FATAL("Implementation phrase with non-interface symbol");
      }
    }
  }
}

void handle_where_clause_expr(BaseAST *ast) {
  if (CallExpr *ce = toCallExpr(ast)) {
    if (UnresolvedSymExpr *callsymexpr = toUnresolvedSymExpr(ce->baseExpr)) {
      if (!strcmp(callsymexpr->unresolved, "==")) {
        //Equality constraint, let's add it
        BaseAST *arg1 = ce->argList.get(1);
        BaseAST *arg2 = ce->argList.get(2);
        cclosure.equate(arg1, arg2);
      } else if (!strcmp(callsymexpr->unresolved, "_build_tuple")) {
        //We have multiple constraints, handle them
        for_alist(arg, ce->argList) {
          handle_where_clause_expr(arg);
        }
      }
    } else {
      //printf("Not unresolved\n");
    }
  }
}

BaseAST *typeCheckFn(FnSymbol *fn) {
  // Look through our formal arguments
  for_formals(formal, fn) {
    if (!formal->typeExpr && (!formal->flags.test(FLAG_TYPE_VARIABLE))) {
      INT_FATAL("Formal is missing explicit type annotation\n");
    }
  }
  if (!fn->retExprType) {
    INT_FATAL("function missing explicit return type annotation\n");
  }

  if (fn->where) {
    for_alist(expr, fn->where->body) {
      handle_where_clause_expr(expr);
    }
  }

  Map<FnSymbol*, ArgSymbol*> fnsInInterfaces;
  Vec<ArgSymbol*> witnessObjects;
  getFunctionsInWhereClause(fnsInInterfaces, witnessObjects, fn->where);

  forv_Vec(ArgSymbol, arg, witnessObjects) {
    fn->insertFormalAtTail(arg);
  }

  // Now, look through the function body
  BaseAST *tmp = typeCheckExpr(fn->body, fn->retExprType, fnsInInterfaces);

  //FIXME: This is a workaround until we have a clean-up pass
  fn->where->remove();

  if (witnessObjects.length() > 0) {
    fn->addFlag(FLAG_ALLOW_REF);
    fn->addFlag(FLAG_GENERIC);
    fn->removeFlag(FLAG_CHECKED);
  }

  return tmp;
}

Symbol* mapArguments(CallExpr* where, FnSymbol* visibleFn, CallExpr* call) {
  //UnresolvedSymExpr *ur_where = toUnresolvedSymExpr(where->baseExpr);
  BaseAST *arg1 = where->argList.get(1);
  BaseAST *arg2 = where->argList.get(2);
  //printf("%s\n", ur_where->unresolved);
  //Expr* type = where->list->get(1);
  SymExpr *s_arg1 = toSymExpr(arg1);
  //SymExpr *s_arg2 = toSymExpr(arg2);
  //printf("%s %d implements the interface %d\n", s_arg1->var->cname,s_arg1->var->id, arg2->id);
  Expr *e_actual = call->argList.head;
  ArgSymbol *s_formal =
      ((visibleFn)->formals.head) ?
          toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) : NULL;
  for (;
      e_actual;
      e_actual = e_actual->next, s_formal =
          (s_formal && s_formal->defPoint->next) ?
              toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) : NULL) {
    //printf("%s %d\n", s_formal->cname, s_formal->id);
    if (s_formal->id == s_arg1->var->id) {
      //if(checkImplementation(e_actual,s_arg2)){
      //if (cclosure.has_implements_relation(e_actual, arg2)) {
      if(Symbol *witness = lookupImplementsWitnessInSymbolTable(call, e_actual, arg2)) {
        //printf("Interface is implemented!\n");
        return witness;
      } else {
        //printf("Interface not implemented\n");
        return NULL;
      }

    }
  }
  return NULL;
}

BaseAST* checkFunctionCall(CallExpr* call) {
  //printf("In checkFunctioncall\n");
  BaseAST *retExpr;
  if (!call->primitive) {
    //printf("Inside not primitive\n");
    CallInfo2 info(call);

    Vec<FnSymbol*> visibleFns; // visible functions

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
        //printf("Inside for loop Checking against %s\n", visibleFn->cname);
        bool mismatch = false;
        Expr *e_actual = call->argList.head;
        ArgSymbol *s_formal =
            ((visibleFn)->formals.head) ?
                toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) : NULL;
        for (;
            e_actual;
            e_actual = e_actual->next, s_formal =
                (s_formal && s_formal->defPoint->next) ?
                    toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) :
                    NULL) {
          //printf("Inside actuals loop\n");
          if (!s_formal) {
            //printf("Mismatched\n");
            mismatch = true;
            break;
          }
          //printf("Inside Actuals 2\n");
          if (s_formal->hasFlag(FLAG_TYPE_VARIABLE)) {
            if (SymExpr *se_actual = toSymExpr(e_actual)) {
              //printf("Type symbol %s",se_actual->var->cname);
              if (!isTypeSymbol(se_actual->var)) {
                mismatch = true;
                break;
              } else {
                cclosure.equate(s_formal,se_actual->var);
                //typeMap.put(s_formal, toTypeSymbol(se_actual->var));
              }
            } else {
              mismatch = true;
              break;
            }
          } else if (!cclosure.is_equal(e_actual, s_formal->typeExpr)) {
            //if(UnresolvedSymExpr *s_type = toUnresolvedSymExpr(toExpr(s_formal->typeExpr)))
            //printf("Mismatched\n");
            mismatch = true;
            break;
          }
        }
        //printf("Out of actuals loop\n");
        if (!mismatch) {
          //printf ("Matching function found %s\n",visibleFn->cname);
          if (visibleFn->where) {
            for_alist(expr,visibleFn->where->body) {
              if (CallExpr* where_expr = toCallExpr(expr)) {
                if (UnresolvedSymExpr *callsymexpr = toUnresolvedSymExpr(where_expr->baseExpr)) {
                  if ((!strcmp(callsymexpr->unresolved, "_build_tuple"))
                      || (!strcmp(callsymexpr->unresolved, "PRIM_ACTUALS_LIST"))) {
                    //printf("inside list of exprs\n");
                    for_alist(wl, where_expr->argList) {
                      if (CallExpr *where_ce = toCallExpr(wl)) {
                        if (UnresolvedSymExpr *use = toUnresolvedSymExpr(where_ce->baseExpr)) {
                          if (!strcmp(use->unresolved, "implements")) {
                            //printf("where with implements found\n");
                            if(Symbol *witness = mapArguments(where_ce, visibleFn, call)){
                              retExpr = cclosure.get_representative_ast((visibleFn->retExprType)->body.head);
                              cclosure.make_parent_null((visibleFn->retExprType)->body.head);

                              call->insertAtTail(new SymExpr(witness));

                              return retExpr;
                            }
                            else
                              return dtUnknown;
                          }
                        }
                      }
                    }
                  } else if (!strcmp(callsymexpr->unresolved, "implements")) {
                    //printf("where with implements found\n");
                    if(Symbol *witness = mapArguments(where_expr, visibleFn, call)){
                      retExpr = cclosure.get_representative_ast((visibleFn->retExprType)->body.head);
                      cclosure.make_parent_null((visibleFn->retExprType)->body.head);

                      call->insertAtTail(new SymExpr(witness));

                      return retExpr;
                    }
                    else
                      return dtUnknown;
                  }
                }
              }
            }
          }
          else{
            retExpr = cclosure.get_representative_ast((visibleFn->retExprType)->body.head);
            cclosure.make_parent_null((visibleFn->retExprType)->body.head);
            return retExpr;
          }
        }
      }
    //printf("No matching functions at call");
    return dtUnknown;
  }
  //printf("No matching functions at call");
  return dtUnknown;
}

/*
  Creates the parent class which will represent the function's type.  Children of the parent class will capture different functions which
  happen to share the same function type.  By using the parent class we can assign new values onto variable that match the function type
  but may currently be pointing at a different function.
*/
static ClassType* createAndInsertInstanceWitnessClass(BaseAST *witness, const char *name) {
  ClassType *parent = new ClassType(CLASS_CLASS);
  TypeSymbol *parent_ts = new TypeSymbol(name, parent);

  //parent_ts->addFlag(FLAG);

  // Because this function type needs to be globally visible (because we don't know the modules it will be passed to), we put
  // it at the highest scope
  //theProgram->block->body.insertAtTail(new DefExpr(parent_ts));

  //witness->insertBefore(new DefExpr(parent_ts));

  //Adding supprot for implements stmt
  if(ImplementsStmt* s = toImplementsStmt(witness)) {
    s->insertBefore(new DefExpr(parent_ts));
  } else if (CallExpr* ce = toCallExpr(witness)) {
    ce->insertBefore(new DefExpr(parent_ts));
  }

  parent->dispatchParents.add(dtObject);
  dtObject->dispatchChildren.add(parent);
  VarSymbol* parent_super = new VarSymbol("super", dtObject);
  parent_super->addFlag(FLAG_SUPER_CLASS);
  parent->fields.insertAtHead(new DefExpr(parent_super));
  build_constructor(parent);
  build_type_constructor(parent);

  return parent;
}

void addAdaptationToWitness(BaseAST *insertBefore, ClassType *witness, FnSymbol *requiredFn, FnSymbol *actualFn) {
  FnSymbol* adapted_method = new FnSymbol(requiredFn->name);
  adapted_method->addFlag(FLAG_INLINE);
  //adapted_method->addFlag(FLAG_INVISIBLE_FN);
  adapted_method->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  ArgSymbol* thisWitnessSymbol = new ArgSymbol(INTENT_BLANK, "this", witness);
  thisWitnessSymbol->addFlag(FLAG_ARG_THIS);
  adapted_method->insertFormalAtTail(thisWitnessSymbol);
  adapted_method->_this = thisWitnessSymbol;

  CallExpr *ce = new CallExpr(actualFn);

  for_formals(formal, requiredFn) {
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, formal->name, dtAny);
    adapted_method->insertFormalAtTail(arg);
    ce->insertAtTail(new SymExpr(arg));
  }

  if (actualFn->retType == dtVoid) {
    adapted_method->insertAtTail(ce);
  }
  else {
    VarSymbol *tmp = newTemp("_return_tmp_");
    adapted_method->insertAtTail(new DefExpr(tmp));
    adapted_method->insertAtTail(new CallExpr(PRIM_MOVE, tmp, ce));

    adapted_method->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  //insertBefore->insertBefore(new DefExpr(adapted_method));
  if(ImplementsStmt* s = toImplementsStmt(insertBefore)) {
      s->insertBefore(new DefExpr(adapted_method));
    } else if (CallExpr* c = toCallExpr(insertBefore)) {
      c->insertBefore(new DefExpr(adapted_method));
    }

  normalize(adapted_method);

  witness->methods.add(adapted_method);
}

BaseAST* checkInterfaceImplementations(BaseAST *s) {
  BaseAST * returnExpr;

  if(BlockStmt* block = toBlockStmt(s)) {
    for_alist(stmt, block->body) {
      checkInterfaceImplementations(stmt);
    }
  }

  else if (DefExpr *de = toDefExpr(s)) {
      if (de->sym && isFnSymbol(de->sym)) {
        FnSymbol *fn = toFnSymbol(de->sym);
        checkInterfaceImplementations(fn->body);
      }
  }
  else if (isCallExpr(s) || isImplementsStmt(s)) {
    CallExpr *ce;
    if(isCallExpr(s)) {
      ce = toCallExpr(s);
    }
    if(isImplementsStmt(s)) {
      ImplementsStmt* istmt = toImplementsStmt(s);
      ce = toCallExpr(istmt->implementsClause);
    }
    if (UnresolvedSymExpr *use = toUnresolvedSymExpr(ce->baseExpr)) {
      if (!strcmp(use->unresolved, "implements")) {
        ClassType* witness = createAndInsertInstanceWitnessClass(s, "witness");
        if (SymExpr *se = toSymExpr(ce->argList.tail)) {
          //if (SymExpr *implementing_type = toSymExpr(ce->argList.head)) {
          if (isSymExpr(ce->argList.head)) {
            if (InterfaceSymbol *is = toInterfaceSymbol(se->var)) {
              forv_Vec (FnSymbol, fn, is->functionSignatures) {
                  CallInfo2 info(fn);

                if (PrimitiveOp *op = primitives_map.get(fn->cname)) {
                  if (fn->formals.length == 0) {
                    INT_FATAL("Unimplemented Operator: 0 args\n");
                    //if (cclosure.is_equal(e_actual, s_formal->typeExpr))
                    //return op->returnInfo(call, NULL, NULL);
                  } else if (fn->formals.length == 1) {
                    INT_FATAL("Unimplemented Operator: 1 arg\n");
                    //return op->returnInfo(call, cclosure.get_representative_ast(call->argList.get(1)), NULL);
                  } else {
                    //printf("Operator: 2 args\n");
                    //FIXME: I don't like passing NULL here, we need to be more flexible
                    //with what operators need

                    //FIXME: Handle dtUnknown

                    BaseAST *arg1Expr;
                    BaseAST *arg2Expr;

                    bool isArg1Self = false;
                    if (DefExpr *de1 = toDefExpr(fn->formals.get(1))) {
                      if (ArgSymbol *as1 = toArgSymbol(de1->sym)) {
                        if (UnresolvedSymExpr *use1 =
                        toUnresolvedSymExpr(as1->typeExpr->body.head)) {

                          if (!strcmp(use1->unresolved, "self"))
                            isArg1Self = true;
                        }
                      }
                    }
                    if (isArg1Self) {
                      arg1Expr = cclosure.get_representative_ast(
                          ce->argList.head);
                    } else {
                      arg1Expr = cclosure.get_representative_ast(
                          fn->formals.get(1));
                    }

                    bool isArg2Self = false;
                    if (DefExpr *de2 = toDefExpr(fn->formals.get(2))) {
                      if (ArgSymbol *as2 = toArgSymbol(de2->sym)) {
                        if (UnresolvedSymExpr *use2 =
                        toUnresolvedSymExpr(as2->typeExpr->body.head)) {

                          if (!strcmp(use2->unresolved, "self"))
                            isArg2Self = true;
                        }
                      }
                    }
                    if (isArg2Self) {
                      arg2Expr = cclosure.get_representative_ast(
                          ce->argList.head);
                    } else {
                      arg2Expr = cclosure.get_representative_ast(
                          fn->formals.get(2));
                    }

                    Type *retType = op->returnInfo(NULL, arg1Expr,
                        arg2Expr);

                    if (retType != dtUnknown) {
                      //FIXME: Can't assume 'self' is always unresolved
                      BlockStmt *retBlock = toBlockStmt(fn->retExprType);
                      if (retBlock) {
                        if (UnresolvedSymExpr *use3 =
                        toUnresolvedSymExpr(retBlock->body.head)) {
                          if (!strcmp(use3->unresolved, "self")) {
                            if (!cclosure.is_equal(ce->argList.head,
                                retType)) {
                              INT_FATAL(
                                  "Operator which satisfies interface requirement has mismatched type");
                            } else {
                              continue;
                            }
                          } else {
                            INT_FATAL("Return type of operator unknown");
                          }
                        }
                      }
                      if (!cclosure.is_equal(fn->retExprType, retType)) {
                        INT_FATAL(
                            "Operator which satisfies interface requirement has mismatched type");
                      } else {
                        continue;
                      }
                    }
                    //return op->returnInfo(call, cclosure.get_representative_ast(call->argList.get(1)),
                    //		cclosure.get_representative_ast(call->argList.get(2)));
                  }
                }

                if (gFnSymbols.n != nVisibleFunctions)
                  buildVisibleFunctionMap2();

                Vec<FnSymbol*> visibleFns; // visible functions

                Vec<BlockStmt*> visited;

                //First, add the visible functions

                if (ImplementsStmt* istmt = toImplementsStmt(s)) {   //the scope for visible functions for implements statement is its block_stmt
                  const char* function_name = info.name;
                  for_alist(stmt, istmt->statements->body) {
                    if(CallExpr *ce1 = toCallExpr(stmt)) {
                      if (UnresolvedSymExpr *use1 = toUnresolvedSymExpr(ce1->baseExpr)) {
                        if (!strcmp(use1->unresolved, "=")) {
                          Expr* lhs = ce1->argList.head;
                          Expr* rhs = ce1->argList.tail;
                          if(CallExpr* lhs_ce = toCallExpr(lhs)) {
                            Expr* lhs_fn = lhs_ce->argList.head;
                            if(UnresolvedSymExpr* us_lhs = toUnresolvedSymExpr(lhs_fn)) {
                              if(!strcmp(us_lhs->unresolved,info.name)) {
                                if(CallExpr* rhs_ce = toCallExpr(rhs)) {
                                  Expr* rhs_fn = rhs_ce->argList.head;
                                  if(UnresolvedSymExpr* us_rhs = toUnresolvedSymExpr(rhs_fn)) {
                                    function_name = us_rhs->unresolved;
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  getVisibleFunctions(istmt->statements, function_name,
                    visibleFns, visited);
                } else {
                getVisibleFunctions(getVisibilityBlock(ce), info.name,
                    visibleFns, visited);
                }

                bool found_match = false;
                forv_Vec(FnSymbol, visibleFn, visibleFns) {
                  bool mismatch = false;
                  Expr *e_actual = ce->argList.head;
                  ArgSymbol *s_formal =
                      ((visibleFn)->formals.head) ?
                          toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) :
                          NULL;
                  for (;
                      e_actual;
                      e_actual = e_actual->next, s_formal =
                          (s_formal && s_formal->defPoint->next) ?
                              toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) :
                              NULL) {

                    SymExpr *actual_sym = toSymExpr(e_actual);
                    if (!actual_sym || !isInterfaceSymbol(actual_sym->var)) {
                      if (!s_formal) {
                        mismatch = true;
                        break;
                      }
                      if (actual_sym
                          && !strcmp(actual_sym->var->cname, "self")) {
                        if (!cclosure.is_equal(e_actual,
                            s_formal->typeExpr)) {
                          mismatch = true;
                          break;
                        }
                      } else {
                        if (!cclosure.is_equal(ce->argList.head,
                            s_formal->typeExpr)) {
                          mismatch = true;
                          break;
                        }
                      }
                    }
                  }
                  if (!mismatch) {
                    //Next check return type.
                    Expr *retExpr;

                    //printf("Adding adaptation to witness\n");
                    addAdaptationToWitness(s, witness, fn, visibleFn);

                    if (BlockStmt *blockStmt = toBlockStmt(fn->retExprType))
                      retExpr = blockStmt->body.head;
                    else
                      retExpr = fn->retExprType;

                    SymExpr *retSymExpr = toSymExpr(retExpr);
                    UnresolvedSymExpr *retUnSymExpr =
                        toUnresolvedSymExpr(retExpr);
                    if (retSymExpr
                        && !strcmp(retSymExpr->var->cname, "self")) {
                      if (cclosure.is_equal(ce->argList.head,
                          visibleFn->retExprType)) {
                        found_match = true;
                        break;
                      } else {
                        mismatch = true;
                      }
                    } else if (retUnSymExpr
                        && !strcmp(retUnSymExpr->unresolved, "self")) {
                      if (cclosure.is_equal(ce->argList.head,
                          visibleFn->retExprType)) {
                        found_match = true;
                        break;
                      } else {
                        mismatch = true;
                      }
                    } else {
                      if (cclosure.is_equal(fn->retExprType,
                          visibleFn->retExprType)) {
                        found_match = true;
                        break;
                      } else {
                        mismatch = true;
                      }
                    }
                  }
                }
                if (!found_match) {
                  INT_FATAL("No matching functions at call");
                }
              }
            }
            //cclosure.add_implements_witness(ce->argList.head,
                //ce->argList.tail);
            if (ImplementsStmt* istmt = toImplementsStmt(s)) {
              //printf("Inside stmt");
              VarSymbol *tmp = newTemp();
              istmt->insertBefore(new DefExpr(tmp));
              istmt->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                 new CallExpr(witness->defaultConstructor->name)));
              addToImplementsSymbolTable(ce,ce->argList.head,
                  ce->argList.tail, tmp);
              istmt->remove();
            } else {
              printf("Inside clause");
              VarSymbol *tmp = newTemp();
              ce->insertBefore(new DefExpr(tmp));
              ce->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                 new CallExpr(witness->defaultConstructor->name)));

              addToImplementsSymbolTable(ce,ce->argList.head,
                  ce->argList.tail, tmp);
              ce->remove();
            }
          } else {
            INT_FATAL("Implementing type not found\n");
          }
        }
      }
      else if(!strcmp(use->unresolved, "writeln")) {
          //printf("Inside Writeln\n");
          checkInterfaceImplementations(ce->argList.head);
      }
      else  if(!ce->primitive) {
        returnExpr = checkFunctionCall(ce);
        //printf("Return Type: %d\n",returnExpr->id);
        //list_view(returnExpr);
      }
    }
  }
  else if(BlockStmt* bs = toBlockStmt(s)) {
    returnExpr = checkInterfaceImplementations(bs);
  }

  return returnExpr;
}

void earlyTypeCheck(void) {
  //bool found_early_type_checked = false;
  checkInterfaceImplementations(
      userModules.v[0]->block);

  forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_SEPARATELY_TYPE_CHECKED)) {
        //found_early_type_checked = true;
        typeCheckFn(fn);
      }
    }

  forv_Vec(InterfaceSymbol, is, gInterfaceSymbols) {
    is->defPoint->remove();
  }

  //printf("complete\n");
  //if (found_early_type_checked) {
    //Hackish workaround to stop early when we're early type-checking until we
    //tie into the rest of the passes
  INT_FATAL("SUCCESS");
  //}
}
