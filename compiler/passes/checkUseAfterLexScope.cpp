#include "passes.h"
#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "stmt.h"
#include "stlUtil.h"
#include "primitive.h"
#include "vec.h"
#include "view.h"
#include <set>


/*********** HEADER *********************/

enum GraphNodeStatus {
  NODE_UNKNOWN = 0, //NODE STATUS UNKNOWN
  NODE_SINGLE_WAIT_FULL,
  NODE_SINGLE_SIGNAL_FULL,
  NODE_SYNC_SIGNAL_FULL,
  NODE_SYNC_SIGNAL_EMPTY,
  NODE_BLOCK_BEGIN, //A NEW BEGIN FUNCTION START HERE
  NODE_BLOCK_INTERNAL_FUNCTION, // A NEW FUNCTION NODE STARTS HERE
  NODE_START_LOOP,
  NODE_START_DOWHILE,
  NODE_START_WHILE,
  NODE_LOOP_END,
  NODE_START_IFELSE,
  NODE_END_IFELSE,
  NODE_INTERNAL_FUNCTION_CALL
};


/****
     We can say The SyncGraph nodes represents
     a striped CCFG(Concurrent Control Flow Graph)
     Which stores data of sync Points and
     use of External variables.
**/
struct SyncGraph {
  /* TODO: Function level ID
 Will be zero for root Function.
 For Others it will give the distance of the
 begin Function from Root Node
 */
  int levelID;
  FnSymbol * fnSymbol;
  SyncGraph* parent;
  Vec<SymExpr*>  contents;
  /***
      TODO: Is This model adequate ?

      changing the imlementation such that we will
      atmax 2 children one is proper child due to a
      sync event(child).

      other is the foster child : the one due to
      a function or a begin statement(fChild)
  ***/
  //  Vec<SyncGraph*> children;
  SyncGraph* child;
  SyncGraph* fChild;
  CallExpr* syncExpr;
  std::string syncVar;
  GraphNodeStatus syncType;
  bool loopNode;
  bool conditionalNode;
  /*
    SyncGraph* child() {
    return child;
    }
  */
  /**
     We always sync the READS  with WRITES
  **/


  Vec<SyncGraph*> syncPoints;


  SyncGraph(SyncGraph* i, FnSymbol *f, int lID) {
    levelID = lID;
    fnSymbol = f;
    contents.copy(i->contents);
    loopNode = i->loopNode;
    conditionalNode = i->conditionalNode;
    syncType = i->syncType;
    syncVar = i->syncVar;
  }
  SyncGraph() {
    child = NULL;
    fChild = NULL;
    syncType = NODE_UNKNOWN;
    levelID = 0;
    loopNode = false;
    conditionalNode = false;
  }
  ~SyncGraph() {}
};

typedef Vec<SyncGraph*> SyncGraphVec;
typedef Map<FnSymbol*, SyncGraph*> FnSyncGraphMap;
typedef Map<FnSymbol*, Vec<SyncGraph*>* > FnSyncGraphVecMap;
typedef MapElem<FnSymbol*, Vec<SyncGraph*>* > FnSyncGraphVecMapElem;

static SyncGraph* syncGraphRoot;

static void deleteSyncGraphNode(SyncGraph *node);
static void cleanUpSyncGraph(SyncGraph *root);
static void checkOrphanStackVar(SyncGraph *root);
static SyncGraph* addChildNode(SyncGraph *cur, FnSymbol* fn);
static SyncGraph* handleBlockStmt(BlockStmt* stmt, SyncGraph *cur);
static SyncGraph* handleDefExpr(DefExpr* def, SyncGraph *cur);
static SyncGraph* handleExpr(Expr* expr, SyncGraph *cur);
static SyncGraph* handleFunction(FnSymbol* fn, SyncGraph *cur);
static SyncGraph* handleBegin(FnSymbol* fn, SyncGraph* cur);
static SyncGraph* handleCallExpr(CallExpr* fn, SyncGraph* cur);
static bool isOuterVar(Symbol* sym, FnSymbol* fn);
//bool isSymbolvarisOfInterest(Symbol* sym, Expr * node);
//static SyncGraph* addSyncExprs(SymExpr *expr, SyncGraph *cur);
static void linkSyncNodes(SyncGraph *signal, SyncGraph *wait);
//bool argumentsContainsSyncVar(FnSymbol *fn);
static SyncGraph* addSyncExprs(Expr *expr, SyncGraph *cur);
static void addSymbolsToGraph(Expr* expr, SyncGraph *cur);
static void collectFuncNodes(SyncGraph *cur, SyncGraphVec& funcNodes);
static bool containsBeginFunction(FnSymbol* fn);
static SyncGraph* getNextSyncNode(SyncGraph* cur, SyncGraphVec* funcNodes);
static FnSymbol* getOriginalFunction(Symbol* sym);
static ArgSymbol*  getTheArgSymbol(std::string sym,FnSymbol* parent);
//static bool inlineCFG(SyncGraph* inlineNode, SyncGraph* branch);
//static bool inlineCFG(SyncGraph* inlineNode, SyncGraph* branch);
//static SyncGraphVec getCopyofGraph(SyncGraph* start, FnSymbol* f, int level);
/************** HEADER ENDS *******************/

/***
    TODO: Handle loops
****/



/**
   We are ging for recursive delete.
   We have to delete all children Nodes before deleting the Node.
**/
static void deleteSyncGraphNode(SyncGraph *node) {
  if (node != NULL) {
    deleteSyncGraphNode(node->child);
    deleteSyncGraphNode(node->fChild);
    delete node;
  }
}

// static SyncGraphVec getCopyofGraph(SyncGraph* start, FnSymbol* f, int level) {
//   /**
//      TODO decide what to de when we have a fChild here
//    **/
//   SyncGraphVec copy;
//   SyncGraph* node = start;
//   while (node != NULL) {
//     SyncGraph * newnode = new SyncGraph(node, f, level);
//     if (copy.count() > 0) {
//       newnode->parent = copy.tail();
//       copy.tail()->child = newnode;
//     }
//     copy.add(node);
//   }
//   return copy;
// }


// static bool inlineCFG(SyncGraph* inlineNode, SyncGraph* branch) {
//   if (inlineNode) {
//     SyncGraph *oldChild = inlineNode->child;
//     SyncGraphVec copy = getCopyofGraph(branch, inlineNode->fnSymbol, inlineNode->levelID);
//     if (copy.count() >0) {
//       inlineNode->child = copy.head();
//       copy.head()->parent = inlineNode;
//       copy.tail()->child = oldChild;
//       oldChild->parent = copy.tail();
//     }
//   }
//   return true;
// }


/**
   clean up the Data-Control Graph.
**/
static void cleanUpSyncGraph(SyncGraph *node) {
  deleteSyncGraphNode(node);
}

static void linkSyncNodes(SyncGraph *signal, SyncGraph *wait) {
  signal->syncPoints.add(wait);
  wait->syncPoints.add(signal);
}


/**
   Adding a child node to the current node.
   The FnSymbol passed should be the Function
   that will contain the node.
   This will help us distinguish the child nodes
   that are created due to sync points
   and the ones that are created for embedded Functions.
**/
static SyncGraph* addChildNode(SyncGraph *cur, FnSymbol* fn) {
  SyncGraph* childNode = new SyncGraph();
  childNode->parent = cur;
  childNode->fnSymbol = fn;
  if (fn == cur->fnSymbol)
    cur->child = childNode;
  else {
    cur->fChild = childNode;
    childNode->levelID = cur->levelID + 1;
  }
  return childNode;
}

/*
  bool argumentsContainsSyncVar(FnSymbol *fn) {
  std::vector<SymExpr*> callExprs;
  collectSymExprs(fn, callExprs);
  return true;
  }

*/
/**
   We need to make sure of the change of state in
   the sync/single varibale:

   The point of filling :
   =:
   writeEF:
   single_mark_and_signal_full/sync_mark_and_signal_full:


   The point of emptying:
   =:
   readFE:
   sync_mark_and_signal_empty:
   No emptying for single. So we have to assume all single variables
   that reach readFE to sync with the write point of the single.
   NOTE : TODO: Also we need to remove syncing with duplicate
   write to the single value.
**/
static SyncGraph* addSyncExprs(Expr *expr, SyncGraph *cur) {
  std::vector<CallExpr*> callExprs;
  collectCallExprs(expr, callExprs);
  FnSymbol* curFun = expr->getFunction();
  for_vector (CallExpr, call, callExprs) {
    if (call->theFnSymbol() != NULL) {
      if (!strcmp(call->theFnSymbol()->name, "=")) {
        SymExpr* symExpr = toSymExpr(call->getNextExpr(call->getFirstExpr()));
        std::string symName;
        if (symExpr != NULL) {
          symName = symExpr->var->name;
        }
        std::vector<CallExpr*> intCalls;
        collectCallExprs(call->theFnSymbol(), intCalls);
        for_vector (CallExpr, intCall, intCalls) {
          if (intCall->theFnSymbol() != NULL) {
            if (!strcmp(intCall->theFnSymbol()->name, "writeEF")) {
              std::vector<CallExpr*> markCalls;
              collectCallExprs(intCall->theFnSymbol(), markCalls);
              for_vector (CallExpr, markCall, markCalls){
                if (markCall->isPrimitive(PRIM_SINGLE_SIGNAL_FULL)) {
                  cur->syncVar = symName;
                  cur->syncType = NODE_SINGLE_SIGNAL_FULL;
                  cur->syncExpr = call;
                  cur = addChildNode(cur, curFun);
                } else if (markCall->isPrimitive(PRIM_SYNC_SIGNAL_FULL)) {
                  cur->syncVar = symName;
                  cur->syncType = NODE_SYNC_SIGNAL_FULL;
                  cur->syncExpr = call;
                  cur = addChildNode(cur, curFun);
                }
              }
            }
	  }
        }
      } else if (!strcmp(call->theFnSymbol()->name, "_statementLevelSymbol")) {
        SymExpr* symExpr = toSymExpr(call->getNextExpr(call->getFirstExpr()));
        std::string symName;
        if (symExpr != NULL) {
          symName = symExpr->var->name;
        }
        std::vector<CallExpr*> intCalls;
        collectCallExprs(call->theFnSymbol(), intCalls);
        for_vector (CallExpr, intCall, intCalls) {
          if (intCall->theFnSymbol() != NULL) {
            if (!strcmp(intCall->theFnSymbol()->name, "readFF")) {
              std::vector<CallExpr*> markCalls;
              collectCallExprs(intCall->theFnSymbol(), markCalls);
              for_vector (CallExpr, markCall, markCalls){
                if (markCall->isPrimitive(PRIM_SINGLE_WAIT_FULL)) {
                  cur->syncVar = symName;
                  cur->syncType = NODE_SINGLE_WAIT_FULL;
                  cur->syncExpr = call;
                  cur = addChildNode(cur, curFun);
                }
              }
            } else if (!strcmp(intCall->theFnSymbol()->name, "readFE")) {
              std::vector<CallExpr*> markCalls;
              collectCallExprs(intCall->theFnSymbol(), markCalls);
              for_vector (CallExpr, markCall, markCalls) {
                if (markCall->isPrimitive(PRIM_SYNC_SIGNAL_EMPTY)) {
                  cur->syncVar = symName;
                  cur->syncType = NODE_SYNC_SIGNAL_EMPTY;
                  cur->syncExpr = call;
                  cur = addChildNode(cur, curFun);
                }
              }
            }
          }
        }
      }
    }
  }
  return cur;
}



/***
    A modified copy of isOuterVar in
    flattenFunction.cpp
***/
static bool isOuterVar(Symbol* sym, FnSymbol* fn) {
  /**
     To do handle function calls.
  **/
  if (   sym->isParameter()               || // includes isImmediate()
	 sym->hasFlag(FLAG_TEMP)          || // exclude these
	 sym->hasFlag(FLAG_CONST)         ||
	 sym->hasFlag(FLAG_TYPE_VARIABLE) ||    // 'type' aliases or formals
	 sym->hasFlag(FLAG_SINGLE)        ||
	 sym->hasFlag(FLAG_SYNC)          ||
	 isFnSymbol(sym)
     )
    return false;
  if (isArgSymbol(sym)) {
    ArgSymbol* argSym = toArgSymbol(sym);
    if (argSym->intent == INTENT_REF) {
      return true;
    }
  }

  Symbol* symParent = sym->defPoint->parentSymbol;
  Symbol* parent = fn->defPoint->parentSymbol;  
  bool flag = false;
  while (true) {
    if (!isFnSymbol(parent)) {
      flag = false;
      break;
    } else if (symParent == parent) {
      flag = true;
      break;
    } else {
      INT_ASSERT (parent->defPoint->parentSymbol &&
		  parent->defPoint->parentSymbol != parent); // ensure termination
      parent = parent->defPoint->parentSymbol;
    }
  }
  return flag;
}


static ArgSymbol*  getTheArgSymbol(std::string sym,FnSymbol* parent) {
  for_formals(formal,parent) {
    if(sym.compare(formal->name) == 0) {
      return formal;
    }
  }
  return NULL;
}

static FnSymbol* getOriginalFunction(Symbol* sym) {
  if(isArgSymbol(sym)) {
    while (isArgSymbol(sym)) {
      ArgSymbol* argSym = toArgSymbol(sym);
      Symbol* symParent  = sym->defPoint->parentSymbol;
      Symbol* parent = symParent->defPoint->parentSymbol;  
      if (argSym->intent == INTENT_REF) {
	if(isFnSymbol(parent)) {
	  FnSymbol* parentFunction = toFnSymbol(parent);
	  ArgSymbol* argSymbol = getTheArgSymbol(sym->name,parentFunction);
	  if(argSymbol != NULL) {
	    sym = argSymbol;
	  } else {
	    //	    print_view(argSym->defPoint->parentSymbol);
	    //  USR_PRINT(argSym->defPoint, "Defined here");
	    return parentFunction;
	  }
	} else {
	  return NULL;
	}
      } else {
	return sym->defPoint->parentSymbol->getFunction();
      }
    }
  }
  Symbol* symParent = sym->defPoint->parentSymbol;
  if(isFnSymbol(symParent))
    return toFnSymbol(symParent);
  return NULL;
}

static void addSymbolsToGraph(Expr* expr, SyncGraph *cur) {
  std::vector<SymExpr*> symExprs;
  collectSymExprs(expr, symExprs);
  for_vector (SymExpr, se, symExprs) {
    Symbol* sym = se->var;
    if (isOuterVar(sym, expr->getFunction())) {
      FnSymbol* fn = getOriginalFunction(sym);
      cur->contents.add_exclusive(se);
      cur->syncFunctions.add(fn);
    }
  }
  cur = addSyncExprs(expr, cur);
}


static void collectFuncNodes(SyncGraph *cur, SyncGraphVec& funcNodes) {
  if (cur->fChild != NULL) {
    funcNodes.add(cur->fChild);
    collectFuncNodes(cur->fChild, funcNodes);
  }
  if (cur->child != NULL)
    collectFuncNodes(cur->child, funcNodes);
}

static SyncGraph* getNextSyncNode(SyncGraph* cur, SyncGraphVec* alreadySynced) {
  SyncGraph* next  = cur->child;
  while ( next != NULL &&
	  next->syncType != NODE_SINGLE_WAIT_FULL &&
	  next->syncType != NODE_SINGLE_SIGNAL_FULL &&
	  next->syncType != NODE_SYNC_SIGNAL_FULL &&
	  next->syncType != NODE_SYNC_SIGNAL_EMPTY &&
	  !(alreadySynced->in(next))) {
    next = next->child;
  }
  return next;
}

static void checkOrphanStackVar(SyncGraph *root) {
  /*
    TODO:
    We want to sync only begin functions right now.
    So we will filter out the begin functions.
  */
  SyncGraphVec funcNodes;
  funcNodes.add(root);
  collectFuncNodes(root, funcNodes);
  forv_Vec (SyncGraph, funcNode, funcNodes) {
    SyncGraph * curNode = funcNode;
    /**
       An optimization: we will not process the thread
       that is created before in
    **/
    int processTask  = 0;
    /* Initialize sync points with each Thread */
    FnSyncGraphMap taskSyncPoints;
    FnSyncGraphVecMap syncedNodes;
    forv_Vec (SyncGraph, nextFuncNode, funcNodes) {  
      if (processTask == 1) {
        taskSyncPoints.put(nextFuncNode->fnSymbol, nextFuncNode);
	syncedNodes.put(nextFuncNode->fnSymbol, new SyncGraphVec());
      } else if (nextFuncNode == funcNode) {
        processTask  = 1;
      }
    }
    while (curNode != NULL) {
      /*
        We need to consider only about syncVars
      */
      if (!(curNode->syncVar.empty())) {
        std::string syncVar = curNode->syncVar;
        int processnextTask = 0;
        forv_Vec (SyncGraph, nextFuncNode, funcNodes) {
          if (processnextTask == 1) {
	    SyncGraphVec* syncedNodesinFunc =  syncedNodes.get(nextFuncNode->fnSymbol);
	    SyncGraph* finalCandidateSyncNode = getNextSyncNode(nextFuncNode, syncedNodesinFunc);
	    SyncGraph* endofSearch = NULL;
	    if (finalCandidateSyncNode != NULL)
	      endofSearch = finalCandidateSyncNode->child;
            if (curNode->syncType ==  NODE_SINGLE_WAIT_FULL) {
              /*
                Since there is no state change in SINGLE variable after
                being full we have to search from begining.
              */
              SyncGraph* candidateSyncNode = nextFuncNode;
              while (candidateSyncNode  != endofSearch) {
                if (syncVar.compare(candidateSyncNode->syncVar) == 0  && 
		    candidateSyncNode->syncType == NODE_SINGLE_SIGNAL_FULL) {
                  linkSyncNodes(candidateSyncNode, curNode);
		  syncedNodesinFunc->add_exclusive(candidateSyncNode);
                }
                candidateSyncNode = candidateSyncNode->child;
              }
            } else if (curNode->syncType ==  NODE_SINGLE_SIGNAL_FULL) {
              SyncGraph* candidateSyncNode = taskSyncPoints.get(nextFuncNode->fnSymbol);
              while (candidateSyncNode  != endofSearch) {
                if (syncVar.compare(candidateSyncNode->syncVar) == 0  && 
		    candidateSyncNode->syncType == NODE_SINGLE_WAIT_FULL) {
                  linkSyncNodes(curNode, candidateSyncNode);
		  syncedNodesinFunc->add_exclusive(candidateSyncNode);
                }
                candidateSyncNode = candidateSyncNode->child;
              }
            } else  if (curNode->syncType == NODE_SYNC_SIGNAL_FULL) {
              SyncGraph* candidateSyncNode = taskSyncPoints.get(nextFuncNode->fnSymbol);
              while (candidateSyncNode  != endofSearch) {
                if (syncVar.compare(candidateSyncNode->syncVar) == 0  && 
		    candidateSyncNode->syncType == NODE_SYNC_SIGNAL_EMPTY) {
                  linkSyncNodes(candidateSyncNode, curNode);
		  syncedNodesinFunc->add_exclusive(candidateSyncNode);
                }
                candidateSyncNode = candidateSyncNode->child;
              }
            } else if (curNode->syncType == NODE_SYNC_SIGNAL_EMPTY) {
              SyncGraph* candidateSyncNode = taskSyncPoints.get(nextFuncNode->fnSymbol);
              while (candidateSyncNode  != endofSearch) {
                if (syncVar.compare(candidateSyncNode->syncVar) == 0  && 
		    candidateSyncNode->syncType == NODE_SYNC_SIGNAL_FULL) {
                  linkSyncNodes(curNode, candidateSyncNode);
		  syncedNodesinFunc->add_exclusive(candidateSyncNode);
                }
                candidateSyncNode = candidateSyncNode->child;
              }
            }
          } else if (nextFuncNode == funcNode) {
            processnextTask  = 1;
          }
        }
        if (curNode->syncPoints.count() == 0 && 
	    curNode->syncType != NODE_SINGLE_WAIT_FULL) {
	  //          USR_WARN(curNode->syncExpr, 
	  //	   "No matching Syncronization Expression for this. This could result in the program entering an infinite wait.");
        } else if (curNode->syncPoints.count() == 1) {
          taskSyncPoints.put(curNode->syncPoints.only()->fnSymbol, curNode->syncPoints.only());
        }
      }
      curNode = curNode->child;
    }
    //    syncedNodes;
    form_Map(FnSyncGraphVecMapElem, nextList, syncedNodes) {
      delete(nextList->value);
    }
  }
  /**
     TODO: We need not analyze the main thread for unsyncronized
  **/
  funcNodes.remove(0);
  forv_Vec (SyncGraph, funcNode, funcNodes) {
    SyncGraph * curNode = funcNode;
    while (curNode != NULL) {
      if (curNode->contents.length() > 0) {
        if (curNode->syncType == NODE_UNKNOWN) {
          /* No sync point
             report error directly
          */
          forv_Vec (SymExpr, unsynced, curNode->contents) {
            USR_WARN(unsynced,
		     "Unsynchronized use of external variable(%s). This could result in incorrect/unintended program behaviour.",
		     unsynced->var->name);
          }
        } else {
          /** All syncronization statements should be
              linked with Root Function **/
	  
        }
      }
      curNode = curNode->child;
    }
  }
}


static SyncGraph* handleDefExpr(DefExpr* def, SyncGraph *cur){
  if (isFnSymbol(def->sym)) {
    FnSymbol* fn = toFnSymbol(def->sym);
    if (fn->hasFlag(FLAG_BEGIN)) {
      handleBegin(fn, cur);
      cur->syncType = NODE_BLOCK_BEGIN;
    } else {
      handleFunction(fn, cur);
      cur->syncType = NODE_BLOCK_INTERNAL_FUNCTION;
    }
    cur = addChildNode(cur, def->getFunction());
  } else {
    //     addSymbolsToGraph(def, cur);
  }
  return cur;
}



static SyncGraph* handleBlockStmt(BlockStmt* stmt, SyncGraph *cur){
  if (stmt->body.length != 0 ) {
    for_alist (node, stmt->body) {
      cur = handleExpr(node, cur);
    }
  }
  return cur;
}

static SyncGraph* handleCallExpr(CallExpr* callExpr, SyncGraph *cur){
  if (callExpr->isNamed("begin_fn")) {
    /**
       Auto created call no need to add.
    **/
  } else {
    addSymbolsToGraph(callExpr, cur);
  }
  return cur;
}

static SyncGraph* handleExpr(Expr* expr, SyncGraph *cur){
  if (NamedExpr* named = toNamedExpr(expr)) {
    cur = handleExpr(named->actual, cur);
  } else if (DefExpr* def = toDefExpr(expr)) {
    cur  = handleDefExpr(def, cur);
  } else if (BlockStmt* block = toBlockStmt(expr)) {
    cur = handleBlockStmt(block, cur);
  } else if (CallExpr* callExpr = toCallExpr(expr)) {
    cur = handleCallExpr(callExpr, cur);
  } else {
    addSymbolsToGraph(expr, cur);
  }
  return cur;
}



static SyncGraph* handleBegin(FnSymbol* fn, SyncGraph* cur) {
  SyncGraph* childNode = addChildNode(cur, fn);
  handleBlockStmt(fn->body, childNode);
  return cur;
}

static SyncGraph* handleFunction(FnSymbol* fn, SyncGraph *cur=NULL) {
  SyncGraph* newNode = NULL;
  if (cur == NULL) {
    newNode = cur  = syncGraphRoot = new SyncGraph();
  } else {
    newNode = addChildNode(cur, fn);
  }
  handleBlockStmt(fn->body, newNode);
  return cur;
}


// void  checkSync(FnSymbol* fn){
//  /* Function called
//    we create the Root Node here*/
//   if (root == NULL)
//     cur = root = new SyncGraph();
//   else
//     cur = addChildNode(cur, fn);
//   handleFunction
//   checkOrphanStackVar(root);
//   cleanUpSyncGraph(root);
// }

/**
   This function checks if the given outer function constains
   begin statement.
   If the current function is begin we return False since,
   we have already (recursively) added its parent into the list.
*/
static bool containsBeginFunction(FnSymbol* fn) {
  if (!(fn->getModule()->modTag == MOD_USER) || fn->hasFlag(FLAG_BEGIN)) {
    return false;
  }
  std::vector<CallExpr*> callExprs;
  
  collectCallExprs(fn, callExprs);
  for_vector (CallExpr, call, callExprs) {
    FnSymbol* caleeFn = call->theFnSymbol();
    if (caleeFn != NULL && caleeFn->hasFlag(FLAG_BEGIN)) {
      return true;
    }
  }
  return false;
}

void checkUseAfterLexScope(){
  // collect all functions that needs to be analyzed
  Vec<FnSymbol*> aFnSymbols;
  forv_Vec (FnSymbol, fn, gFnSymbols) {
    if (containsBeginFunction(fn) == true) {
      aFnSymbols.add_exclusive(fn);
    }
  }
  forv_Vec (FnSymbol, fn, aFnSymbols) {
    syncGraphRoot =  new SyncGraph();
    handleFunction(fn, syncGraphRoot);
    checkOrphanStackVar(syncGraphRoot);
    cleanUpSyncGraph(syncGraphRoot);
  }
}
