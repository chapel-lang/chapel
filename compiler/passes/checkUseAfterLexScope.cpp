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

typedef BlockStmt Scope;
typedef Vec<Scope*> ScopeVec;
/****
     We can say The SyncGraph nodes represents
     a striped CCFG(Concurrent Control Flow Graph)
     Which stores data of sync Points and
     use of External variables.
**/
struct SyncGraph {
  FnSymbol* fnSymbol;
  SyncGraph* parent;
  Vec<SymExpr*>  contents;
  Vec<BlockStmt*> syncScope;
  Vec<CallExpr*> intFuncCalls;
  Vec<SyncGraph*> syncPoints;
  ScopeVec syncedScopes;    // These are the already defined sync points
                            // created due to sync statments, Cobegin, Coforall
                            // and Forall statments.
  std::string syncVar;
  SyncGraph* child; // same function next BB
  SyncGraph* fChild; // internal Functions including Begin
  SyncGraph* cChild; // conditionalNode child
  SyncGraph* joinNode;
  CallExpr* syncExpr;
  GraphNodeStatus syncType;
  bool loopNode;
  bool conditionalNode;
  bool isjoinNode;
  SyncGraph(SyncGraph* i, FnSymbol *f) {
    fnSymbol = f;
    contents.copy(i->contents);
    syncedScopes.append(i->syncedScopes);
    loopNode = i->loopNode;
    conditionalNode = i->conditionalNode;
    syncType = i->syncType;
    syncVar = i->syncVar;
    joinNode = i->joinNode;
    isjoinNode = i->isjoinNode;
  }

  SyncGraph() {
    child = NULL;
    fChild = NULL;
    cChild = NULL;
    syncType = NODE_UNKNOWN;
    loopNode = false;
    conditionalNode = false;
    // insideSyncStmt = false;
    isjoinNode = false;
    joinNode = NULL;
  }
  ~SyncGraph() {}
};

struct UseInfo {
  SymExpr* usePoint;
  SyncGraph* useNode;
};

struct ExternVarDetails {
  std::string varName;
  Scope* scope;
  Vec<UseInfo*> usePoints;
  void addUseInfo(SymExpr* point, SyncGraph* node) {
    UseInfo* useInfo = new UseInfo();
    useInfo->usePoint = point;
    useInfo->useNode = node;
    usePoints.add(useInfo);
  }
  ExternVarDetails() {}
  ~ExternVarDetails() {
    while(usePoints.length() != 0){
      delete(usePoints.pop());
    }
  }
};

typedef Vec<SyncGraph*> SyncGraphVec;
typedef Map<FnSymbol*, SyncGraph*> FnSyncGraphMap;
typedef Map<FnSymbol*, Vec<SyncGraph*>* > FnSyncGraphVecMap;
typedef MapElem<FnSymbol*, Vec<SyncGraph*>* > FnSyncGraphVecMapElem;
typedef Vec<FnSymbol*> FnSymbolVec;
static SyncGraph* syncGraphRoot;
static SyncGraphVec analysisRoots; // We will be creating multiple root for
                                   //  analyzing recursion and loops more
                                   // efficiently.
static SyncGraphVec taskStartPoint; // start of all beginStatements
static Vec<ExternVarDetails*> externVarDetails;
static FnSyncGraphMap funcGraphMap; // start point of each function in Sync Graph
static bool allCallsSynced;
static void deleteSyncGraphNode(SyncGraph *node);
static void cleanUpSyncGraph(SyncGraph *root);
static void checkOrphanStackVar(SyncGraph *root);
static SyncGraph* addChildNode(SyncGraph *cur, FnSymbol* fn);
static SyncGraph* handleBlockStmt(BlockStmt* stmt, SyncGraph *cur);
static SyncGraph* handleCondStmt(CondStmt* cond,SyncGraph* cur);
static SyncGraph* handleDefExpr(DefExpr* def, SyncGraph *cur);
static SyncGraph* handleExpr(Expr* expr, SyncGraph *cur);
static SyncGraph* handleFunction(FnSymbol* fn, SyncGraph *cur);
static SyncGraph* handleBegin(FnSymbol* fn, SyncGraph* cur);
static SyncGraph* handleCallExpr(CallExpr* fn, SyncGraph* cur);
static SyncGraph* handleSyncStatement(BlockStmt* block, SyncGraph* cur);
static SyncGraph* handleLoopStmt(BlockStmt* block,SyncGraph* cur);
static bool isOuterVar(Symbol* sym, FnSymbol* fn);
static SyncGraph* addSyncExprs(Expr *expr, SyncGraph *cur);
static void addSymbolsToGraph(Expr* expr, SyncGraph *cur);
static bool containsBeginFunction(FnSymbol* fn);
static void addExternVarDetails(Scope* s, std::string v,SymExpr* use, SyncGraph* node);
static Scope* getOriginalScope(Symbol* sym);
static ArgSymbol*  getTheArgSymbol(std::string sym, FnSymbol* parent);
static BlockStmt* getSyncBlockStmt(BlockStmt* def, SyncGraph *cur);
static bool isInsideSyncStmt(Expr* expr);
static bool shouldSync(Scope* scope, SyncGraph* cur);
static void checkSyncedCalls(FnSymbol* fn);
static int compareScopes(Scope* a, Scope * b);
static SyncGraph* nextSyncPoint(SyncGraph* start);
static SyncGraph* addElseChildNode(SyncGraph *cur, FnSymbol *fn);
static bool  refersExternalSymbols(Expr* expr, SyncGraph * cur);
static bool  ASTContainsBeginFunction(BaseAST* ast);
static void expandAllInternalFunctions(SyncGraph* root,Vec<FnSymbol*> &fnSymbols, SyncGraph* endPoint = NULL);
static SyncGraph* inlineCFG(SyncGraph* inlineNode, SyncGraph* branch);
static SyncGraphVec getCopyofGraph(SyncGraph* start, FnSymbol* f);
static SyncGraph* nextSyncPoint(SyncGraph* start);
static void provideWarning(SymExpr* expr, ExternVarDetails* var);
static bool sync(SyncGraph* source, SyncGraph* dest);
static void collectNextSyncPoints(SyncGraphVec& startPoints, SyncGraphVec& syncPoints);
static void getSyncPoints(SyncGraph* source, SyncGraphVec& potentialDest, SyncGraphVec& syncPoints);
static bool threadedMahjong(SyncGraph* sourceSyncPoint, SyncGraphVec& destSyncPoints);
static void collectNewBegins(SyncGraph* start, SyncGraph* end, SyncGraphVec& destSyncPoints);
static void removeNewBegins(SyncGraphVec& nextSourceBegins, SyncGraphVec&  destSyncPoints);
static SyncGraph* pullUpNextSyncNode(SyncGraph* curNode, SyncGraphVec& destSyncPoints);
static void pullDownSyncNode(SyncGraph* curNode, SyncGraph * prevNode, SyncGraphVec& destSyncPoints);
static bool isStartPointAfterLastSync(SyncGraph* defFunction,SyncGraph* useNode);
/************** HEADER ENDS *******************/
static void deleteSyncGraphNode(SyncGraph *node) {
  if (node != NULL) {
    deleteSyncGraphNode(node->child);
    deleteSyncGraphNode(node->fChild);
    deleteSyncGraphNode(node->cChild);
    delete node;
  }
}

static SyncGraphVec getCopyofGraph(SyncGraph* start, FnSymbol* f) {
  /**
     TODO decide what to de when we have a fChild here
  **/
  SyncGraphVec copy;
  SyncGraph* node = start;
  while (node != NULL) {
    SyncGraph * newnode = new SyncGraph(node, f);
    if (copy.length() > 0) {
      newnode->parent = copy.tail();
      copy.tail()->child = newnode;
    }
    copy.add(node);
  }
  return copy;
}


static SyncGraph* inlineCFG(SyncGraph* inlineNode, SyncGraph* branch) {
  if (inlineNode) {
    SyncGraph *oldChild = inlineNode->child;
    SyncGraphVec copy = getCopyofGraph(branch, inlineNode->fnSymbol);
    if (copy.length() >0) {
      inlineNode->child = copy.head();
      copy.head()->parent = inlineNode;
      copy.tail()->child = oldChild;
      oldChild->parent = copy.tail();
    }
    return copy.tail();
  }
  return NULL;
}


static void expandAllInternalFunctions(SyncGraph* root, Vec<FnSymbol*> &fnSymbols, SyncGraph* endPoint) {
  SyncGraph* cur = root;
  SyncGraphVec endPoints;
  while(cur != NULL) {
    if(cur == endPoint)
      return;
    if(cur->intFuncCalls.count() != 0) {
      forv_Vec(CallExpr, fnCall, cur->intFuncCalls) {
        FnSymbol* curFun = fnCall->theFnSymbol();
        SyncGraph* intFuncNode = funcGraphMap.get(curFun);
        if(intFuncNode != NULL && fnSymbols.in(curFun) == NULL
           && fnSymbols.add_exclusive(curFun) == 1 ) {
          SyncGraph* endPoint = inlineCFG(cur, intFuncNode);
          endPoints.add(endPoint);
        }
      }
    }

    if(cur->cChild != NULL ) {
      expandAllInternalFunctions(cur->cChild, fnSymbols,cur->cChild->joinNode);
    }

    if(cur->fChild != NULL && cur->fChild->fnSymbol->hasFlag(FLAG_BEGIN)) {
      expandAllInternalFunctions(cur->fChild, fnSymbols);
    }

    if(endPoints.tail() == cur) {
      fnSymbols.pop();
      endPoints.pop();
    }
    cur = cur->child;
  }
  return;
}

/**
   clean up the Data-Control Graph.
**/
static void cleanUpSyncGraph(SyncGraph *node) {
  deleteSyncGraphNode(node);
  forv_Vec (ExternVarDetails, cur, externVarDetails) {
    delete cur;
  }
  externVarDetails.clear();
  funcGraphMap.clear();
  analysisRoots.clear();
  //  syncGraphRoot = NULL;
}

/**
   This function will compare two scopes and return the relationship.
   Return 0  : Not compareable
   Return 1  : The Scope A is embedded in Scope B. (includes A == B)
   Return -1 : The Scope B is strictly
   embedded in Scope A. (does not include B == A ).
**/
static int compareScopes(Scope* a, Scope* b) {
  if(a == b) {
    return 1;
  }
  Expr* parent = a->parentExpr;
  if(a->parentSymbol == b->parentSymbol) {
    while(parent) {
      if (parent == b)
        return 1;
      parent = parent->parentExpr;
    }
    parent = b->parentExpr;
    while(parent) {
      if (parent == a)
        return -1;
      parent = parent->parentExpr;
    }
  }
  return 0;
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
  if (fn == cur->fnSymbol){
    cur->child = childNode;
    childNode->syncedScopes.copy(cur->syncedScopes);
  } else {
    cur->fChild = childNode;
    if(fn->hasFlag(FLAG_BEGIN)){
      childNode->syncedScopes.copy(cur->syncedScopes);
    }
  }
  return childNode;
}

static SyncGraph* addElseChildNode(SyncGraph *cur, FnSymbol *fn){
  SyncGraph* childNode = new SyncGraph();
  childNode->parent = cur;
  childNode->fnSymbol = fn;
  cur->cChild = childNode;
  return childNode;
}


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

static void collectNextSyncPoints(SyncGraphVec& startPoints, SyncGraphVec& syncPoints) {
  forv_Vec(SyncGraph,start,startPoints) {
    SyncGraph* syncPoint = nextSyncPoint(start);
    if(syncPoint != NULL) {
      syncPoints.add(syncPoint);
    }
  }
}

static SyncGraph* nextSyncPoint(SyncGraph* start) {
  SyncGraph * cur =  start;
  while(cur != NULL) {
    if(cur->syncType ==  NODE_SINGLE_WAIT_FULL ||
       cur->syncType ==  NODE_SINGLE_SIGNAL_FULL ||
       cur->syncType ==  NODE_SYNC_SIGNAL_FULL ||
       cur->syncType == NODE_SYNC_SIGNAL_EMPTY)
      return cur;
    if(cur->cChild == NULL)
      cur = cur->child;
    else {
      // TODO : how to choose between
      // else part and if part
      cur = cur->child;
    }
  }
  return NULL;
}

static void collectAllAvailableBeginGraphs(SyncGraph* root, SyncGraphVec& endPoints, SyncGraphVec& taskPoints) {
  SyncGraph* cur  = root;
  while (cur != NULL && endPoints.in(cur) == NULL) {
    if (cur->fChild != NULL && cur->fChild->fnSymbol->hasFlag(FLAG_BEGIN)){
      SyncGraph* newBegin = cur->fChild;
      SyncGraph* syncPoint= nextSyncPoint(newBegin);
      if(nextSyncPoint != NULL) {
        taskPoints.add(syncPoint);
        endPoints.add(syncPoint);
      }
      collectAllAvailableBeginGraphs(newBegin, endPoints,taskPoints);
      if(syncPoint != NULL)
        endPoints.remove(endPoints.index(syncPoint));
    }
    if(cur->cChild!= NULL) {
      endPoints.add(cur->cChild->joinNode);
      collectAllAvailableBeginGraphs(cur->cChild,endPoints,taskPoints);
      endPoints.remove(endPoints.index(cur->cChild->joinNode));
    }
    cur = cur->child;
  }
  return;
}

static void provideWarning(SymExpr* expr, ExternVarDetails* var) {
  USR_WARN(expr,"Potential unsafe (use after free) use of variable %s here. 
    Please make sure the variable use is properly synced", var->varName);
}

static void collectNewBegins(SyncGraph* start, SyncGraph* end, SyncGraphVec& destSyncPoints) {
  SyncGraph* curNode = start;
  while (curNode  != NULL && curNode != end) {
    if(curNode->fChild != NULL && curNode->fChild->fnSymbol->hasFlag(FLAG_BEGIN))
      destSyncPoints.add(curNode->fChild);
  }
}

static void removeNewBegins(SyncGraphVec& nextSourceBegins, SyncGraphVec&  destSyncPoints) {
  forv_Vec(SyncGraph, beginNode, nextSourceBegins) {
    destSyncPoints.remove(destSyncPoints.index(beginNode));
  }
}

static SyncGraph* pullUpNextSyncNode(SyncGraph* curNode, SyncGraphVec& destSyncPoints) {
  SyncGraph* nextNode = nextSyncPoint(curNode);
  destSyncPoints.remove(destSyncPoints.index(curNode));
  if(nextNode != NULL) destSyncPoints.add(nextNode);
  return nextNode;
}

static void pullDownSyncNode(SyncGraph* curNode, SyncGraph* prevNode, SyncGraphVec& destSyncPoints) {
  if(curNode != NULL)
    destSyncPoints.remove(destSyncPoints.index(curNode));
  destSyncPoints.add(prevNode);
}

static bool sync(SyncGraph* source, SyncGraph* dest) {
  bool compatible = false;
  switch(source->syncType) {
  case NODE_SINGLE_WAIT_FULL:
    if(dest->syncType == NODE_SINGLE_SIGNAL_FULL)
      compatible = true;
    break;

  case  NODE_SINGLE_SIGNAL_FULL:
    if(dest->syncType == NODE_SINGLE_WAIT_FULL)
      compatible = true;
    break;

  case NODE_SYNC_SIGNAL_FULL:
    if(dest->syncType == NODE_SYNC_SIGNAL_EMPTY)
      compatible = true;
    break;

  case NODE_SYNC_SIGNAL_EMPTY:
    if(dest->syncType == NODE_SYNC_SIGNAL_FULL)
      compatible = true;
    break;

  default:
    return false;
  }
  if(compatible)
    if(dest->syncVar.compare(source->syncVar) == 0) {
      return true;
    }
  return false;
}

static bool isStartPointAfterLastSync(SyncGraph* defFunction,SyncGraph* useNode) {
  SyncGraph* syncPoint;
  SyncGraph* fnNode = funcGraphMap.get(useNode->fnSymbol);
  INT_ASSERT(defFunction != fnNode);
  while(defFunction != fnNode) {
    syncPoint = fnNode->parent;
    if(syncPoint != NULL)
      fnNode = funcGraphMap.get(syncPoint->fnSymbol);
    else
      return true;
    INT_ASSERT(fnNode != NULL);
  }
  if(nextSyncPoint(syncPoint) == NULL)
    return true;
  return false;
}

static void getSyncPoints(SyncGraph* source, SyncGraphVec& potentialDest, SyncGraphVec& syncPoints) {
  forv_Vec(SyncGraph, dest, potentialDest) {
    if(sync(source,dest)) {
      syncPoints.add(dest);
    }
  }
}

static bool threadedMahjong(SyncGraph* sourceSyncPoint, SyncGraphVec& destSyncPoints) {
  SyncGraphVec syncPoints;
  SyncGraphVec newSourceBegins;
  SyncGraphVec newDestBegins;
  bool sucess= false;
  if(sourceSyncPoint == NULL)   // endof recursion
    return true;
  if(destSyncPoints.count() == 0)
    return false;
  getSyncPoints(sourceSyncPoint, destSyncPoints, syncPoints);
  SyncGraph* nextSourceNode = sourceSyncPoint;
  collectNewBegins(sourceSyncPoint, nextSourceNode, newSourceBegins);
  destSyncPoints.append(newSourceBegins);
  forv_Vec(SyncGraph, toSyncNode, syncPoints) {
    // TODO : Handle SingleNode
    SyncGraph* next = pullUpNextSyncNode(toSyncNode, destSyncPoints);
    collectNewBegins(toSyncNode, next, newDestBegins);
    destSyncPoints.append(newDestBegins);
    sucess = threadedMahjong (nextSourceNode, destSyncPoints);
    if(sucess) return sucess;
    removeNewBegins(newDestBegins, destSyncPoints);
    pullDownSyncNode(next, toSyncNode, destSyncPoints);
  }

  removeNewBegins(newSourceBegins, destSyncPoints);
  // reset begins as well
  forv_Vec(SyncGraph, destSyncNode, destSyncPoints){
    //TODO : Handle Single Node
    syncPoints.clear();
    newSourceBegins.clear();
    getSyncPoints(destSyncNode, destSyncPoints, syncPoints);
    SyncGraph* nextDestSyncNode = NULL;
    if(syncPoints.count() > 0){
      nextDestSyncNode = pullUpNextSyncNode(destSyncNode, destSyncPoints);
      collectNewBegins(destSyncNode, nextDestSyncNode, newSourceBegins);
      destSyncPoints.append(newSourceBegins);
    }
    forv_Vec(SyncGraph, toSyncNode, syncPoints) {
      SyncGraph* nextToSyncNode = pullUpNextSyncNode(toSyncNode,  destSyncPoints);
      newDestBegins.clear();
      collectNewBegins(toSyncNode, nextToSyncNode, newDestBegins);
      destSyncPoints.append(newDestBegins);
      sucess = threadedMahjong (sourceSyncPoint, destSyncPoints);
      if(sucess) return sucess;
      removeNewBegins(newDestBegins, destSyncPoints);
      pullDownSyncNode(nextToSyncNode, toSyncNode, destSyncPoints);
    }
    if(syncPoints.count() > 0) {
      removeNewBegins(newSourceBegins, destSyncPoints);
      pullDownSyncNode(nextDestSyncNode, destSyncNode, destSyncPoints);
    }
  }
  return false;
}

static void checkOrphanStackVar(SyncGraph *root) {
  // Do the test for each external Var
  forv_Vec (ExternVarDetails, cur, externVarDetails) {
    SyncGraph *defFunction = funcGraphMap.get(cur->scope->getFunction());
    SyncGraphVec taskPoints; // start pointof each tasks (begin functions)
    SyncGraphVec endPoints; // should not go searching beyond this
    SyncGraphVec destSyncPoints; // destination snc point vertex
    SyncGraph* sourceSyncPoint = nextSyncPoint(defFunction);
    forv_Vec(UseInfo, curUseInfo, cur->usePoints){
      // provide warning for obvious cases.
      if( nextSyncPoint(curUseInfo->useNode) == NULL || sourceSyncPoint == NULL
          || isStartPointAfterLastSync(defFunction, curUseInfo->useNode)) {
        provideWarning(curUseInfo->usePoint, cur);
      } else {
        endPoints.add(sourceSyncPoint);
        endPoints.add(curUseInfo->useNode);
        collectAllAvailableBeginGraphs(root, endPoints, taskPoints);
        collectNextSyncPoints(taskPoints, destSyncPoints);
        bool sucess = threadedMahjong(sourceSyncPoint, destSyncPoints);
        if(sucess) {
          provideWarning(curUseInfo->usePoint,cur);
        }
      }
      endPoints.clear();
      taskPoints.clear();
      destSyncPoints.clear();
    }
  }
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


// Get the scope of the symbol.
// If its an external Varaible to the base function we return NULL.
static Scope* getOriginalScope(Symbol* sym) {
  if(isArgSymbol(sym)) {
    while (isArgSymbol(sym)) {
      ArgSymbol* argSym = toArgSymbol(sym);
      Symbol* symParent  = sym->defPoint->parentSymbol;
      Symbol* parent = symParent->defPoint->parentSymbol;
      FnSymbol* parentFunction = toFnSymbol(parent);
      ArgSymbol* parentArgSymbol = NULL;
      if(parentFunction != NULL)
        parentArgSymbol = getTheArgSymbol(sym->name,parentFunction);
      if (argSym->intent == INTENT_REF) {
        if(isFnSymbol(parent)) {
          if(parentArgSymbol != NULL) {
            // Go one level up.
            sym = parentArgSymbol;
          } else {
            // We reached the defining point.
            break;
          }
        } else if (parentArgSymbol != NULL) {
          /* We are at the uppermost level of heirarchy */
          return NULL;
        } else {
          return parentFunction->body;
        }
      } else {
        return sym->defPoint->getScopeBlock();
      }
    }
  }
  return sym->defPoint->getScopeBlock();
}

static bool  refersExternalSymbols(Expr* expr, SyncGraph* cur) {
  std::vector<SymExpr*> symExprs;
  collectSymExprs(expr, symExprs);
  for_vector (SymExpr, se, symExprs) {
    Symbol* sym = se->var;
    if (isOuterVar(sym, expr->getFunction())) {
      Scope* block = getOriginalScope(sym);
      if(shouldSync(block, cur)){
        return true;
      }
    }
  }
  return false;
}

static void addExternVarDetails(Scope* s, std::string v,SymExpr* use, SyncGraph* node) {
  ExternVarDetails * theOne = NULL;
  forv_Vec (ExternVarDetails, cur, externVarDetails) {
    if (cur->varName.compare(v)  == 0 &&  cur->scope == s) {
      theOne  = cur;
      break;
    }
  }
  if ( theOne == NULL) {
    theOne = new ExternVarDetails();
    theOne->varName = v;
    theOne->scope = s;
  }
  theOne->addUseInfo(use,node);
}

static void addSymbolsToGraph(Expr* expr, SyncGraph *cur) {
  std::vector<SymExpr*> symExprs;
  collectSymExprs(expr, symExprs);
  for_vector (SymExpr, se, symExprs) {
    Symbol* sym = se->var;
    if (isOuterVar(sym, expr->getFunction())) {
      BlockStmt* block = getOriginalScope(sym);
      if(shouldSync(block, cur)) {
        addExternVarDetails(block, sym->name, se, cur);
        cur->contents.add_exclusive(se);
        cur->syncScope.add(block);
      }
    }
  }
  /**collect calls to functions **/
  std::vector<CallExpr*> callExprs;
  collectCallExprs(expr, callExprs);
  for_vector (CallExpr, call, callExprs) {
    FnSymbol* caleeFn = call->theFnSymbol();
    if (caleeFn != NULL && !(caleeFn->hasFlag(FLAG_BEGIN))
        && (caleeFn->getModule()->modTag == MOD_USER)
        && isFnSymbol(caleeFn->defPoint->parentSymbol)) {
      // Non begin used mod function
      // that is internal
      cur->intFuncCalls.add_exclusive(call);
    }
  }

  cur = addSyncExprs(expr, cur);
}

// for finding if all calls to the base function is
// through a sync statement. If yes then we need not
// any variable that is passed to as argument.
static bool isInsideSyncStmt(Expr* expr) {
  while(expr != NULL) {
    if(isBlockStmt(expr)) {
      Expr* previous = expr->prev;
      if(isCallExpr(previous)) {
        if(toCallExpr(previous)->isPrimitive(PRIM_SET_END_COUNT)) {
          return true;
        }
      }
    }
    expr = expr->parentExpr;
  }
  return false;
}


// Check if all calls th the fn is synced.
static void checkSyncedCalls(FnSymbol* fn){
  allCallsSynced = true;
  forv_Vec(CallExpr,call,*fn->calledBy){
    if(isInsideSyncStmt(call)){
      allCallsSynced = false;
      break;
    }
  }
}



static BlockStmt* getSyncBlockStmt(BlockStmt* block, SyncGraph *cur) {
  for_alist (node, block->body) {
    if(isCallExpr(node)) {
      if(toCallExpr(node)->isPrimitive(PRIM_SET_END_COUNT)) {
        if(isBlockStmt(node->next))
          return toBlockStmt(node->next);
      }
    }
  }
  return NULL;
}

static SyncGraph* handleSyncStatement(Scope* block, SyncGraph* cur) {
  cur->syncedScopes.add(block);
  cur = handleBlockStmt(block,cur);
  cur->syncedScopes.pop();
  return cur;
}

static SyncGraph* handleCondStmt(CondStmt* cond, SyncGraph* cur) {
  BlockStmt* thenBlock = cond->thenStmt;
  BlockStmt* elseBlock = cond->elseStmt;
  cur = handleBlockStmt(thenBlock,cur);
  if( ASTContainsBeginFunction(elseBlock) ||
      refersExternalSymbols(elseBlock, cur) ){
    SyncGraph* elseNode = addElseChildNode(cur,elseBlock->getFunction());
    elseNode = handleBlockStmt(elseBlock,elseNode);
    cur = addChildNode(cur,cond->getFunction());
    elseNode->child = cur;
  }

  return cur;
}

static SyncGraph* handleLoopStmt(BlockStmt* block,SyncGraph* cur) {
  // TODO
  if(ASTContainsBeginFunction(block) == true || refersExternalSymbols(block, cur)) {
    for_alist (node, block->body) {
      cur = handleExpr(node, cur);
    }
  }
  return cur;
}

static SyncGraph* handleDefExpr(DefExpr* def, SyncGraph *cur) {
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

static bool shouldSync(Scope* block, SyncGraph *cur) {
  // TODO : this should handle all cases where the
  // scope of declaration function fn is beyond
  // a sync statement then we need not worry  about
  // syncing it.

  if(block == NULL &&  allCallsSynced == true){
    return false;
  }
  if(cur->syncedScopes.length() > 0) {
    forv_Vec(Scope, syncedScope, cur->syncedScopes) {
      if(compareScopes(block,syncedScope) == 1)
        return false;
    }
  }
  return true;
}

static SyncGraph* handleBlockStmt(BlockStmt* stmt, SyncGraph *cur) {
  if(stmt->body.length == 0)
    return cur;
  bool handled = false;

  if( stmt->isLoopStmt() || stmt->isWhileStmt() ||
      stmt->isWhileDoStmt() || stmt->isDoWhileStmt()  ||
      stmt->isParamForLoop() ) {
    cur = handleLoopStmt(stmt,cur);
  } else if( stmt->isCoforallLoop() ) {
    // CO FOR ALL LOOP to go.
  }
  if(isDefExpr(stmt->body.first())) {
    DefExpr* def = toDefExpr(stmt->body.first());
    if(isVarSymbol(def->sym)) {
      VarSymbol* var = toVarSymbol(def->sym);
      if(strcmp(var->name,"_endCountSave") == 0) {
        //start of sync statement
        BlockStmt* syncblock = getSyncBlockStmt(stmt,cur);
        if(syncblock !=  NULL){
          handled = true;
          cur = handleSyncStatement(stmt,cur);
        }
      }
    }
  }

  if (handled == false) {
    for_alist (node, stmt->body) {
      cur = handleExpr(node, cur);
    }
  }
  return cur;
}

static SyncGraph* handleCallExpr(CallExpr* callExpr, SyncGraph *cur) {
  if (callExpr->isNamed("begin_fn")) {
    //   Auto created call no need to add.
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
  } else if (CondStmt* contStmt = toCondStmt(expr)) {
    cur = handleCondStmt(contStmt, cur);
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
    analysisRoots.add(syncGraphRoot);
  } else {
    // Internal Function node.
    newNode = addChildNode(cur, fn);
  }
  funcGraphMap.put(fn,newNode);
  handleBlockStmt(fn->body, newNode);
  return cur;
}



/**
   This function checks if the given outer function constains
   begin statement.
   If the current function is begin we return False since,
   we have already (recursively) added its parent into the list.
*/

static bool  ASTContainsBeginFunction(BaseAST* ast) {
  std::vector<CallExpr*> callExprs;
  collectCallExprs(ast, callExprs);
  for_vector (CallExpr, call, callExprs) {
    FnSymbol* caleeFn = call->theFnSymbol();
    if (caleeFn != NULL && caleeFn->hasFlag(FLAG_BEGIN)) {
      return true;
    }
  }
  return false;
}


static bool containsBeginFunction(FnSymbol* fn) {
  // we need not analyze any embedded Begin function
  if (!(fn->getModule()->modTag == MOD_USER) || fn->hasFlag(FLAG_BEGIN)) {
    return false;
  }
  return ASTContainsBeginFunction(fn);
}

void checkUseAfterLexScope(){
  // collect all functions that needs to be analyzed
  Vec<FnSymbol*> aFnSymbols;
  forv_Vec (FnSymbol, fn, gFnSymbols) {
    if (containsBeginFunction(fn) == true) {
      FnSymbol* cur = fn;
      bool found  = false;
      while (!(cur->defPoint->parentSymbol == NULL)) {
        if (toFnSymbol(cur->defPoint->parentSymbol) != NULL) {
          FnSymbol* parent = toFnSymbol(cur->defPoint->parentSymbol);
          if (aFnSymbols.in(parent) != NULL) {
            found  = true;
            break;
          }
          cur = parent;
        }else
          break;
      }
      if(!found)
        aFnSymbols.add_exclusive(fn);
    }
  }


  isInsideSyncStmt(NULL);
  forv_Vec (FnSymbol, fn, aFnSymbols) {
    checkSyncedCalls(fn);
    SyncGraph* syncGraphRoot = handleFunction(fn);
    analysisRoots.add(syncGraphRoot);
    Vec<FnSymbol*> fnSymbols;
    fnSymbols.add(fn);
    expandAllInternalFunctions(syncGraphRoot, fnSymbols);
    checkOrphanStackVar(syncGraphRoot);
    cleanUpSyncGraph(syncGraphRoot);
  }
}
