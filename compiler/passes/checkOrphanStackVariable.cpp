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

/**********************

TODO : We are using primitive tags to handle  
the Node boundaries.
PRIM_NOOP               :Initial/ End of Function.
PRIM_SINGLE_WAIT_FULL   : single read 
PRIM_SINGLE_SIGNAL_FULL : sigle fill
PRIM_SYNC_SIGNAL_FULL   : sync full
PRIM_SYNC_SIGNAL_EMPTY  : sync empty
PRIM_BLOCK_BEGIN        : begin 
PRIM_UNKNOWN            : Internal Function
PRIM_BLOCK_PARAM_LOOP   :| 
PRIM_BLOCK_WHILEDO_LOOP :|
PRIM_BLOCK_DOWHILE_LOOP :>loop 
PRIM_BLOCK_FOR_LOOP     :|
PRIM_BLOCK_C_FOR_LOOP   :|
 
                        : If-ELse 
*************************/
/****
     We can say The SyncGraph nodes represents 
     a striped CCFG(Concurrent Control Flow Graph) 
     Which stores data of sync Points and
     use of External variables.
 **/
struct SyncGraph{
  /* TODO: */
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
  PrimitiveTag syncType;
  /*
  SyncGraph* child(){
    return child;
  }
  */
  /**
     We always sync the READS  with WRITES 
   **/
  Vec<SyncGraph*> syncPoints;
  SyncGraph(){
    child = NULL;
    fChild = NULL;
    syncType = PRIM_NOOP;
  }
  ~SyncGraph(){}
} *root;

void deleteSyncGraphNode(SyncGraph *node);
void cleanUpSyncGraph(SyncGraph *root);
void checkOrphanStackVar(SyncGraph *root);
SyncGraph* addChildNode(SyncGraph *cur,FnSymbol* fn);
SyncGraph* handleBlockStmt(BlockStmt* stmt,SyncGraph *cur);
SyncGraph* handleDefExpr(DefExpr* def,SyncGraph *cur);
SyncGraph* handleExpr(Expr* expr,SyncGraph *cur);
SyncGraph* handleFunction(FnSymbol* fn,SyncGraph *cur);
SyncGraph* handleBegin(FnSymbol* fn,SyncGraph* cur);
SyncGraph* handleCallExpr(CallExpr* fn,SyncGraph* cur);
bool isOuterVar(Symbol* sym, FnSymbol* fn);
//bool isSymbolvarisOfInterest(Symbol* sym,Expr * node);
SyncGraph* addSyncExprs(SymExpr *expr,SyncGraph *cur);
/************** HEADER ENDS *******************/

/***
    TODO: Handle loops
****/



/**
   We are ging for recursive delete.
   We have to delete all children Nodes before deleting the Node.
 **/
void deleteSyncGraphNode(SyncGraph *node){
  if(node != NULL){
    deleteSyncGraphNode(node->child);
    deleteSyncGraphNode(node->fChild);
    delete node;
  }
}


/**
   clean up the Data-Control Graph.
 **/
void cleanUpSyncGraph(SyncGraph *node){
  deleteSyncGraphNode(node); 
}

void linkSyncNodes(SyncGraph *signal,SyncGraph *wait){
  signal->syncPoints.add(wait);
  wait->syncPoints.add(signal);
}


/**
   Adding a child node to the current node.
   The FnSymbol passed should be the Function
   that will contain the node.
   This will help us distinguish the child nodes
   that are created due yo sync points 
   and the ones that are created for embedded Functions.
 **/ 
SyncGraph* addChildNode(SyncGraph *cur, FnSymbol* fn){
  SyncGraph* childNode = new SyncGraph();
  childNode->parent = cur;
  childNode->fnSymbol = fn;
  if(fn == cur->fnSymbol)
    cur->child = childNode;
  else
    cur->fChild = childNode;
  return childNode;
}


bool argumentsContainsSyncVar(FnSymbol *fn){
  std::vector<SymExpr*> callExprs;
  collectSymExprs(fn, callExprs);    
 
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
SyncGraph* addSyncExprs(Expr *expr,SyncGraph *cur){
  std::vector<CallExpr*> callExprs;
  collectCallExprs(expr, callExprs);
  
  FnSymbol* curFun = expr->getFunction();
  for_vector(CallExpr, call, callExprs) {    
    if(call->theFnSymbol() != NULL)
      if(!strcmp(call->theFnSymbol()->name,"=")){	
	SymExpr* symExpr = toSymExpr(call->getNextExpr(call->getFirstExpr()));
	std::string symName;
	if(symExpr != NULL){
	  symName = symExpr->var->name;
	}
	std::vector<CallExpr*> intCalls;	     
	collectCallExprs(call->theFnSymbol(),intCalls);
	for_vector(CallExpr,intCall,intCalls){
	  if(intCall->theFnSymbol() != NULL)
	    if(!strcmp(intCall->theFnSymbol()->name,"writeEF")){
	      std::vector<CallExpr*> markCalls;
	      collectCallExprs(intCall->theFnSymbol(),markCalls);
	      for_vector(CallExpr,markCall,markCalls){
		if(markCall->isPrimitive(PRIM_SINGLE_SIGNAL_FULL)){
		  cur->syncVar = symName;
		  cur->syncType = PRIM_SINGLE_SIGNAL_FULL;
		  cur->syncExpr = call;
		  cur = addChildNode(cur,curFun);
		  
		}
		else if(markCall->isPrimitive(PRIM_SYNC_SIGNAL_FULL)){
		  cur->syncVar = symName;
		  cur->syncType = PRIM_SYNC_SIGNAL_FULL;
		  cur->syncExpr = call;
		  cur = addChildNode(cur,curFun);
		}
	      }
	    }
	}
      }
      else if(!strcmp(call->theFnSymbol()->name,"_statementLevelSymbol")){
	SymExpr* symExpr = toSymExpr(call->getNextExpr(call->getFirstExpr()));
	std::string symName;
	if(symExpr != NULL){
	  symName = symExpr->var->name;
	}
	std::vector<CallExpr*> intCalls;	     
	collectCallExprs(call->theFnSymbol(),intCalls);
	for_vector(CallExpr,intCall,intCalls){
	  if(intCall->theFnSymbol() != NULL){
	    if(!strcmp(intCall->theFnSymbol()->name,"readFF")){
	      std::vector<CallExpr*> markCalls;
	      collectCallExprs(intCall->theFnSymbol(),markCalls);
	      for_vector(CallExpr,markCall,markCalls){
		if(markCall->isPrimitive(PRIM_SINGLE_WAIT_FULL)){
		  cur->syncVar = symName;
		  cur->syncType = PRIM_SINGLE_WAIT_FULL;
		  cur->syncExpr = call;
		  cur = addChildNode(cur,curFun);
		}
	      }
	    }else if(!strcmp(intCall->theFnSymbol()->name,"readFE")){
	      std::vector<CallExpr*> markCalls;
	      collectCallExprs(intCall->theFnSymbol(),markCalls);
	      for_vector(CallExpr,markCall,markCalls){
		if(markCall->isPrimitive(PRIM_SYNC_SIGNAL_EMPTY)){
		  cur->syncVar = symName;
		  cur->syncType = PRIM_SYNC_SIGNAL_EMPTY;
		  cur->syncExpr = call;
		  cur = addChildNode(cur,curFun);
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
bool isOuterVar(Symbol* sym, FnSymbol* fn) {
  /**
     To do handle function calls.
   **/
  
  if( sym->hasFlag(FLAG_TEMP)          || // exclude these  
      sym->hasFlag(FLAG_CONST)         ||
      sym->hasFlag(FLAG_TYPE_VARIABLE)     // 'type' aliases or formals
     )
    return false;

  if(isArgSymbol(sym)){
    ArgSymbol* argSym = toArgSymbol(sym);
    if(argSym->intent == INTENT_REF){
      Symbol* parent = sym->defPoint->parentSymbol;
      return true;
    }
  }
  /*  
  Symbol* symParent = sym->defPoint->parentSymbol;
  if (symParent == fn                  || // no need to search       
      sym->isParameter()               || // includes isImmediate()
      sym->hasFlag(FLAG_TEMP)          || // exclude these  
      sym->hasFlag(FLAG_CONST)         ||
      sym->hasFlag(FLAG_TYPE_VARIABLE) ||    // 'type' aliases or formals
      sym->hasFlag(FLAG_SYNC)          ||  // We don't want to process Sync
      sym->hasFlag(FLAG_SINGLE)            // and sigle varibales.   
      ) { 
    return false;
  }
  Symbol* parent = fn->defPoint->parentSymbol;
  while (true) {
    if (!isFnSymbol(parent) && !isModuleSymbol(parent))
      return false;
    if (symParent == parent)
      return true;
    if (!parent->defPoint)
      return false;
    INT_ASSERT(parent->defPoint->parentSymbol &&
	       parent->defPoint->parentSymbol != parent); // ensure termination            
    parent = parent->defPoint->parentSymbol;
  }


  */

  return false;
}


void addSymbolsToGraph(Expr* expr,SyncGraph *cur){
  std::vector<SymExpr*> symExprs;
  collectSymExprs(expr, symExprs);
  for_vector(SymExpr, se, symExprs) {
    Symbol* sym = se->var;
    if(isOuterVar(sym,sym->getFunction())){
      cur->contents.add(se);
    }
  }
  cur = addSyncExprs(expr,cur);
}


void collectFuncNodes(SyncGraph *cur,Vec<SyncGraph*>& funcNodes){
  if(cur->fChild != NULL){
    funcNodes.add(cur->fChild);
    collectFuncNodes(cur->fChild,funcNodes);
  }  
  if(cur->child != NULL)
    collectFuncNodes(cur->child,funcNodes);
}

void checkOrphanStackVar(SyncGraph *root){
  /*
    TODO:
    We want to sync only begin functions right now.
    So we will filter out the begin functions. 
   */
  Vec<SyncGraph*> funcNodes;
  funcNodes.add(root);
  collectFuncNodes(root,funcNodes);



  forv_Vec(SyncGraph,funcNode,funcNodes){
    SyncGraph * curNode = funcNode;
    /**
       An optimization: we will not process the thread
       that is created before in
    **/
    int processTask  = 0;
    /* Initialize sync points with each Thread */
    Map<FnSymbol*,SyncGraph*> taskSyncPoints;
    forv_Vec(SyncGraph,nextFuncNode,funcNodes){
      if(processTask == 1){
  	taskSyncPoints.put(nextFuncNode->fnSymbol,nextFuncNode);
      }else if(nextFuncNode == funcNode){
  	processTask  = 1;
      }
    }
    while(curNode != NULL){
      /*
  	We need to consider only about syncVars
      */
      if(!(curNode->syncVar.empty())){
  	std::string syncVar = curNode->syncVar;
	
	int processnextTask = 0;
	forv_Vec(SyncGraph,nextFuncNode,funcNodes){
	  if(processnextTask == 1){
	    if(curNode->syncType ==  PRIM_SINGLE_WAIT_FULL){
	      /*
	    	Since there is no state change in SINGLE variable after 
	    	being full we have to search from begining.
	      */
  	      SyncGraph* candidateSyncNode = nextFuncNode;
  	      while(candidateSyncNode  != NULL){
  	    	if(syncVar.compare(candidateSyncNode->syncVar) == 0  && candidateSyncNode->syncType == PRIM_SINGLE_SIGNAL_FULL){
  	    	  linkSyncNodes(candidateSyncNode,curNode);
  	    	}
		candidateSyncNode = candidateSyncNode->child;
  	      } 
	    }else if(curNode->syncType ==  PRIM_SINGLE_SIGNAL_FULL){
	      SyncGraph* candidateSyncNode = taskSyncPoints.get(nextFuncNode->fnSymbol);
	      while(candidateSyncNode  != NULL){
		if(syncVar.compare(candidateSyncNode->syncVar) == 0  && candidateSyncNode->syncType == PRIM_SINGLE_WAIT_FULL){
		  linkSyncNodes(curNode,candidateSyncNode);
		}
		candidateSyncNode = candidateSyncNode->child;
	      }  
	    }else  if(curNode->syncType == PRIM_SYNC_SIGNAL_FULL){
	      SyncGraph* candidateSyncNode = taskSyncPoints.get(nextFuncNode->fnSymbol);
  	      while(candidateSyncNode  != NULL){
  	    	if(syncVar.compare(candidateSyncNode->syncVar) == 0  && candidateSyncNode->syncType == PRIM_SYNC_SIGNAL_EMPTY){
  	    	  linkSyncNodes(candidateSyncNode,curNode);
  	    	}
		candidateSyncNode = candidateSyncNode->child;
  	      }    
	    }else if(curNode->syncType == PRIM_SYNC_SIGNAL_EMPTY){
	      SyncGraph* candidateSyncNode = taskSyncPoints.get(nextFuncNode->fnSymbol);
	      while(candidateSyncNode  != NULL){
		if(syncVar.compare(candidateSyncNode->syncVar) == 0  && candidateSyncNode->syncType == PRIM_SYNC_SIGNAL_FULL){
		  linkSyncNodes(curNode,candidateSyncNode);
		}
		candidateSyncNode = candidateSyncNode->child;
	      }
	    }
	  }else if(nextFuncNode == funcNode){
	    processnextTask  = 1;
	  }
	}
  	if(curNode->syncPoints.count() == 0 && curNode->syncType != PRIM_SINGLE_WAIT_FULL){
  	  USR_WARN(curNode->syncExpr,"No matching Syncronization Expression for this. This could result in the program entering an infinite wait.");
  	}else if(curNode->syncPoints.count() == 1){
  	  taskSyncPoints.put(curNode->syncPoints.only()->fnSymbol,curNode->syncPoints.only());
  	}
      }
      curNode = curNode->child;
    }
  }
  
  /**
     TODO: We need not analyze the main thread for unsyncronized
     
  **/
  
  funcNodes.remove(0);
  forv_Vec(SyncGraph,funcNode,funcNodes){
    SyncGraph * curNode = funcNode;
    while(curNode != NULL){
      if(curNode->contents.length() > 0){
  	if(curNode->syncType == PRIM_NOOP){
  	  /* No sync point 
  	     report error directly
  	   */

  	  forv_Vec(SymExpr,unsynced,curNode->contents){
  	    USR_WARN(unsynced,"Unsynchronized use of external variable(%s). This could result in incorrect/unintended program behaviour.",unsynced->var->name);
  	  }
  	}else{

  	}
      }
      curNode = curNode->child;
    }
  }   
} 


SyncGraph* handleDefExpr(DefExpr* def,SyncGraph *cur){
  if(isFnSymbol(def->sym)){
    FnSymbol* fn = toFnSymbol(def->sym);
    if(fn->hasFlag(FLAG_BEGIN)){
      handleBegin(fn,cur);
      cur->syncType = PRIM_BLOCK_BEGIN;
    }else{
      handleFunction(fn,cur);
      cur->syncType = PRIM_UNKNOWN;
    }
    
    cur = addChildNode(cur,def->getFunction());
  }else{
    //     addSymbolsToGraph(def,cur);
  }
  return cur;
}



SyncGraph* handleBlockStmt(BlockStmt* stmt,SyncGraph *cur){
  if(stmt->body.length != 0 ){
    for_alist(node,stmt->body){
      cur = handleExpr(node,cur);
    }
  }
  return cur;
}

SyncGraph* handleCallExpr(CallExpr* callExpr, SyncGraph *cur){
  if(callExpr->theFnSymbol() != NULL &&
     callExpr->theFnSymbol()->getModule()->modTag == MOD_USER){
    /**
       TODO:
       We are getting duplicate entries for the function calls.
       So we skip all uses of the variables in function calls.
       Also the begin functions also create a function call at 
       the end of the definition of the begin function.
    **/
    
  }else{
    addSymbolsToGraph(callExpr,cur);
  }
  return cur;
}

SyncGraph* handleExpr(Expr* expr,SyncGraph *cur){
  if(NamedExpr* named = toNamedExpr(expr)){
      cur = handleExpr(named->actual,cur);
  }else if(DefExpr* def = toDefExpr(expr)){
    cur  = handleDefExpr(def,cur);
  }else if(BlockStmt* block = toBlockStmt(expr)){
    cur = handleBlockStmt(block,cur);
  }else if(CallExpr* callExpr = toCallExpr(expr)){
    cur = handleCallExpr(callExpr,cur);
  }else{
    addSymbolsToGraph(expr,cur);
  }
  return cur;
}
SyncGraph* handleBegin(FnSymbol* fn,SyncGraph* cur){
  SyncGraph* childNode = addChildNode(cur,fn);
  handleBlockStmt(fn->body,childNode);
  return cur;
}

SyncGraph* handleFunction(FnSymbol* fn,SyncGraph *cur=NULL){
  SyncGraph* newNode = NULL;
  if(cur == NULL){
    newNode = cur  = root = new SyncGraph();
  }else{
    newNode = addChildNode(cur,fn);
  }
  handleBlockStmt(fn->body,newNode);  
  return cur;
}


// void  checkSync(FnSymbol* fn){
//  /* Function called 
//    we create the Root Node here*/
//   if(root == NULL)
//     cur = root = new SyncGraph();
//   else
//     cur = addChildNode(cur,fn);
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
bool containsBeginFunction(FnSymbol* fn){
  if (!(fn->getModule()->modTag == MOD_USER) || fn->hasFlag(FLAG_BEGIN)){
    return false;
  }
  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn, callExprs);    
  for_vector(CallExpr, call, callExprs) {
    FnSymbol* caleeFn = call->theFnSymbol();
    if(caleeFn != NULL && caleeFn->hasFlag(FLAG_BEGIN)){
      return true;
    }
  }
  return false;
}

void checkUseAfterLexScope(){
  // collect all functions that needs to be analyzed
  Vec<FnSymbol*> aFnSymbols;
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (containsBeginFunction(fn) == true){
      aFnSymbols.add_exclusive(fn);
    }
  }
  forv_Vec(FnSymbol, fn, aFnSymbols) {
    root =  new SyncGraph();
    handleFunction(fn,root);
    checkOrphanStackVar(root);
    cleanUpSyncGraph(root);
  }
}
