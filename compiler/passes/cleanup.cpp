/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// cleanup
//
// This pass cleans up the AST after parsing. It handles
// transformations that would be difficult to do while parsing.
//

#include "passes.h"

#include "astutil.h"
#include "build.h"
#include "CatchStmt.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

static void cleanup(ModuleSymbol* module);

static void normalizeNestedFunctionExpressions(FnSymbol* fn);

static void replaceIsSubtypeWithPrimitive(CallExpr* call,
                                          bool proper, bool coerce);
static void addIntentRefMaybeConst(ArgSymbol* arg);

static void flattenPrimaryMethod(TypeSymbol* ts, FnSymbol* fn);

static void applyAtomicTypeToPrimaryMethod(TypeSymbol* ts, FnSymbol* fn);

static void fixupVoidReturnFn(FnSymbol* fn);

void cleanup() {
  std::vector<ModuleSymbol*> mods;

  ModuleSymbol::getTopLevelModules(mods);

  for_vector(ModuleSymbol, mod, mods) {
    cleanup(mod);
  }
}

/************************************* | **************************************
*                                                                             *
    if (DefExpr* def = toDefExpr(ast)) {

*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool areMultiDefExprsInAList(AList& list){ 

   int numStmts = 0;

    for_alist(stmt, list){
      if (isDefExpr(stmt)) numStmts++;
    }
    
    return numStmts > 1;
}


static bool isValidInit(Expr* initExpr){
  if (SymExpr* se = toSymExpr(initExpr)) {
    if (initExpr && se->symbol() == gSplitInit) {
      return true;
    }
  }
  return false;
}

/*
static bool isgSplitInit(Expr* initExpr){
  if (SymExpr* se = toSymExpr(initExpr)) {
    if (se->symbol() == gSplitInit) {
      return true;
    }
  }
  return false;
}
*/
static void backPropagateInFunction(BlockStmt* block){

    Expr* init = NULL;
    Expr* type = NULL;
    //int numStmts = 0;
    //DefExpr* prev = NULL;

      //For one variable declarations, there's no need to add temporaries
    /*for_alist(stmt, block->body){
      if (isDefExpr(stmt)) numStmts++;
    }
    if (numStmts <= 1) return;
    */

    if (!areMultiDefExprsInAList(block->body)) return;

    VarSymbol* typeTmp = NULL;
    BlockStmt* tmpBlock = new BlockStmt();

    for_alist_backward(stmt, block->body){
      if (DefExpr* def = toDefExpr(stmt)) {
        /*
	if(def->sym->id == 212199 || def->sym->name == astr("left")){
        //if (def->sym->id == 212189 || def->sym->name == astr("avar")){
        //if (def->sym->id == 212193 || def->sym->name == astr("bvar")){
          //printf("in addType\n");
          gdbShouldBreakHere();
	}
        */
        if(def->sym->name == astr("lo_var")) gdbShouldBreakHere();
	//check if not in record
        if(def->parentSymbol->astTag != E_TypeSymbol) {
	  if (def->init || def->exprType) {

            if(def->exprType){
              typeTmp = NULL;
            } else {
              if(typeTmp){
                def->exprType = new SymExpr(typeTmp);
              } else {
		def->exprType = type;
	      }
            }

	    //if(def->init && !isgSplitInit(def->init)){
            if(isValidInit(def->init)){
              def->init = NULL;
              if(init){
                def->init = init->copy();
                //def->init = new UnresolvedSymExpr(prev->sym->name);
              }
            //  init = def->init;
            }
            /*
	      if(init){
		if((def->init)->isNoInitExpr()){
                  def->init = init->copy();
		}
		else {
	          init = new UnresolvedSymExpr(def->sym->name);
	        }
	      }
            */

            init = def->init;
            type = def->exprType;
            //prev = def;
          }  else {
	    //def->exprType = type;
	  }

          if (def->exprType && typeTmp == NULL) {
            typeTmp = newTemp("type_tmp");
            typeTmp->addFlag(FLAG_TYPE_VARIABLE);
            DefExpr* tmpDef = new DefExpr(typeTmp);
            Expr* move = new CallExpr(PRIM_MOVE, typeTmp, def->exprType->remove());
            type = def->exprType;
            def->exprType = new SymExpr(typeTmp);
            tmpBlock->insertAtTail(tmpDef);
            tmpBlock->insertAtTail(move);
          }
	  
       } else {
         if (def->init || def->exprType) {
           type = def->exprType;
         } else {
           def->exprType = type;
       }
       }
      }
    }
      
    if(tmpBlock){
    block->insertAtHead(tmpBlock);
    tmpBlock->flattenAndRemove();
    }

	/*
        if (def->init || def->exprType) {
          init = def->init;
          type = def->exprType;
        } else {
          if (type)
            prev->exprType =
              new CallExpr(PRIM_TYPEOF, new UnresolvedSymExpr(def->sym->name));
          if (init) {
            if (init->isNoInitExpr()) {
              prev->init = init->copy();
          } else if (type) {
              prev->init = new CallExpr("chpl__readXX",
                                      new UnresolvedSymExpr(def->sym->name));
            } else
              prev->init = new UnresolvedSymExpr(def->sym->name);
          }
          def->init = init;
          def->exprType = type;
        }
        prev = def;
        }
	*/
	//else if (toCallExpr(stmt)) {
	//} else{
        //  INT_FATAL(stmt, "expected DefExpr in backPropagateInitsTypes");
        //}
    //}
  //}

}
/*
static void setFieldHelper(BaseAST* parent, Expr*& field, Expr* setTo){
  if(field != NULL) {
    field->remove();
  }

  field = setTo;

  if(setTo != NULL){
    parent_insert_help(parent, field);

  }

}
*/
/*
static void backPropagateInAggregate(AggregateType* at){
  Expr* init = NULL;
  Expr* type = NULL;
  DefExpr* prev = NULL;

  if (!areMultiDefExprsInAList(at->fields)) return;

  for_alist_backward(stmt, at->fields) {
    if (DefExpr* def = toDefExpr(stmt)) {
      if (def->init || def->exprType) {
        init = def->init;
        type = def->exprType;
      } else {
        if (type) {
          setFieldHelper(prev, prev->exprType, 
            new CallExpr(PRIM_TYPEOF, new UnresolvedSymExpr(def->sym->name)));
        }
        if (init) {
          if (init->isNoInitExpr()) {
            setFieldHelper(prev, prev->init, init->copy());
          } else if (type) {
            setFieldHelper(prev, prev->init, new CallExpr("chpl__readXX",
                                      new UnresolvedSymExpr(def->sym->name)));
          } else
            setFieldHelper(prev, prev->init, new UnresolvedSymExpr(def->sym->name));
        }
        setFieldHelper(prev, def->init, init->copy());
        setFieldHelper(prev, def->exprType, type->copy());
      }
      prev = def;
    } else
      INT_FATAL(stmt, "expected DefExpr in backPropagateInitsTypes");
  }

}
*/
static void backPropagate(BaseAST* ast){
  if (BlockStmt* block = toBlockStmt(ast)) {
    backPropagateInFunction(block);
  }
/*
  if (DefExpr* def = toDefExpr(ast)) {
    if (TypeSymbol* ts = toTypeSymbol(def->sym)) {
      if(AggregateType* at = toAggregateType(ts->type)) {
        backPropagateInAggregate(at);
      }
    }
  }
*/
}


static void cleanup(ModuleSymbol* module) {
  std::vector<BaseAST*> asts;

  collect_asts(module, asts);

  for_vector(BaseAST, ast, asts) {
    SET_LINENO(ast);
    //if (DefExpr* def = toDefExpr(ast)) {
      /*
        if(def->sym->id == 212199 || def->sym->name == astr("left")){
        //if (def->sym->id == 212189 || def->sym->name == astr("avar")){
        //if (def->sym->id == 212193 || def->sym->name == astr("bvar")){
          //printf("in addType\n");
          gdbShouldBreakHere();
        }
      */
     //}

	  //call to function to fix
    backPropagate(ast);
    if (DefExpr* def = toDefExpr(ast)) {

	    //add new funciton to  remove backprop, going to visit defexpr in random order, get parent expr, parentSymbol field if def->ParentSymbol is typeSymbol
	    //if AST is a block
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        SET_LINENO(def);

        if (fn->hasFlag(FLAG_COMPILER_NESTED_FUNCTION) == true) {
          normalizeNestedFunctionExpressions(fn);

        }
      }
    }
  }

  for_vector(BaseAST, ast, asts) {
    SET_LINENO(ast);

    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS && block->list != NULL) {
	      //here?
        //Expr* init = NULL;
        //Expr* type = NULL;
	//DefExpr* prev = NULL;
/*
	for_alist(stmt, block->body){
          if (DefExpr* def = toDefExpr(stmt)) {
            if (def->init || def->exprType) {
              init = def->init;
              type = def->exprType;
            } else {
              if (type)
                prev->exprType =
                  new CallExpr(PRIM_TYPEOF, new UnresolvedSymExpr(def->sym->name));
              if (init) {
                if (init->isNoInitExpr()) {
                  prev->init = init->copy();
                } else if (type) {
                  prev->init = new CallExpr("chpl__readXX",
                                      new UnresolvedSymExpr(def->sym->name));
                } else
                  prev->init = new UnresolvedSymExpr(def->sym->name);
              }
              def->init = init;
              def->exprType = type;
            }
            prev = def;
            } else
              INT_FATAL(stmt, "expected DefExpr in backPropagateInitsTypes");
        }
	*/
        block->flattenAndRemove();
      }

    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("isSubtype"))
        replaceIsSubtypeWithPrimitive(call, false, false);
      else if (call->isNamed("isProperSubtype"))
        replaceIsSubtypeWithPrimitive(call, true, false);
      else if (call->isNamed("isCoercible"))
        replaceIsSubtypeWithPrimitive(call, false, true);

    } else if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Is this function defined within a type i.e. is it a method?
        if (TypeSymbol* ts = toTypeSymbol(fn->defPoint->parentSymbol)) {
          flattenPrimaryMethod(ts, fn);
          applyAtomicTypeToPrimaryMethod(ts, fn);
        }

        fixupVoidReturnFn(fn);
      }
    } else if (CatchStmt* catchStmt = toCatchStmt(ast)) {
      catchStmt->cleanup();
    } else if (ArgSymbol* arg = toArgSymbol(ast)) {
      addIntentRefMaybeConst(arg);
    }
  }

  if (module == stringLiteralModule && !fMinimalModules) {
    // Fix calls to chpl_createStringWithLiteral to use resolved expression.
    // For compiler performance reasons, we'd like to have new_StringSymbol
    // emit calls to a resolved function; however new_StringSymbol might
    // run before that function is parsed. So fix up any literals created
    // during parsing here.
    INT_ASSERT(gChplCreateStringWithLiteral != NULL);
    const char* name = gChplCreateStringWithLiteral->name;

    for_vector(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->baseExpr)) {
          if (urse->unresolved == name) {
            SET_LINENO(urse);
            urse->replace(new SymExpr(gChplCreateStringWithLiteral));
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Moves expressions that are parsed as nested function definitions into their *
* own statement; during parsing, these are embedded in call expressions       *
*                                                                             *
************************************** | *************************************/

static void normalizeNestedFunctionExpressions(FnSymbol* fn) {
  DefExpr* def = fn->defPoint;

  if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
    AggregateType* ct = toAggregateType(ts->type);

    INT_ASSERT(ct);

    def->replace(new UnresolvedSymExpr(fn->name));

    ct->addDeclarations(def);

  } else if (isArgSymbol(def->parentSymbol)) {
    Expr* stmt = def->getStmtExpr();

    def->replace(new UnresolvedSymExpr(fn->name));

    stmt->insertBefore(def);

  } else {
    Expr* stmt = def->getStmtExpr();

    def->replace(new UnresolvedSymExpr(fn->name));

    stmt->insertBefore(def);
  }
}


static void replaceIsSubtypeWithPrimitive(CallExpr* call,
                                          bool proper, bool coerce) {
  Expr* sub = call->get(1);
  Expr* sup = call->get(2);
  sub->remove();
  sup->remove();

  PrimitiveTag prim = proper ? PRIM_IS_PROPER_SUBTYPE : PRIM_IS_SUBTYPE;
  if (coerce)
    prim = PRIM_IS_COERCIBLE;

  call->replace(new CallExpr(prim, sup, sub));
}


static void addIntentRefMaybeConst(ArgSymbol* arg) {
  if (arg->hasFlag(FLAG_INTENT_REF_MAYBE_CONST_FORMAL)) {
    arg->intent = INTENT_REF_MAYBE_CONST;
  }
}

//
// If call is an empty return statement, e.g. "return;"
// then change it into a return of the 'void' symbol: "return _void;"
// and mark the function it is in with FLAG_VOID_NO_RETURN_VALUE.
//
static void insertVoidReturnSymbols(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_RETURN));

  if (call->numActuals() == 0) {
    FnSymbol* fn = call->getFunction();
    INT_ASSERT(fn);
    call->insertAtTail(gVoid);
    fn->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  }
}

//
// Mark functions with no return statements and functions with only empty
// return statements with FLAG_VOID_NO_RETURN_VALUE. Change empty return
// statements to return the value '_void'.
//
static void fixupVoidReturnFn(FnSymbol* fn) {
  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn, callExprs);
  bool foundReturn = false;
  // Pass expandExternArrayCalls builds a wrapper for the extern function
  // and returns the value the extern function returned.  It marks the
  // extern function with FLAG_EXTERN_FN_WITH_ARRAY_ARG, which tells us
  // that we need to be able to handle the wrapper returning the result
  // of a call to it.  If the extern function had a 'void' return, treat
  // it as a void value.
  if (fn->hasFlag(FLAG_EXTERN_FN_WITH_ARRAY_ARG)) {
    return;
  }

  for_vector(CallExpr, call, callExprs) {
    if (call->isPrimitive(PRIM_RETURN)) {
      foundReturn = true;
      insertVoidReturnSymbols(call);
    }
  }
  if (!foundReturn) {
    fn->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void flattenPrimaryMethod(TypeSymbol* ts, FnSymbol* fn) {
  Expr*    insertPoint = ts->defPoint;
  DefExpr* def         = fn->defPoint;

  while (isTypeSymbol(insertPoint->parentSymbol)) {
    insertPoint = insertPoint->parentSymbol->defPoint;
  }

  insertPoint->insertBefore(def->remove());

  if (fn->userString != NULL && fn->name != ts->name) {
    if (strncmp(fn->userString, "ref ", 4) == 0) {
      // fn->userString of "ref foo()"
      // Move "ref " before the type name so we end up with "ref Type.foo()"
      // instead of "Type.ref foo()"
      fn->userString = astr("ref ", ts->name, ".", fn->userString + 4);

    } else {
      fn->userString = astr(ts->name, ".", fn->userString);
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void applyAtomicTypeToPrimaryMethod(TypeSymbol* ts, FnSymbol* fn) {
  if (ts->hasFlag(FLAG_ATOMIC_TYPE)) {
    fn->addFlag(FLAG_ATOMIC_TYPE);
  }
}
