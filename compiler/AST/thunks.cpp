/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include "symbol.h"
#include "expr.h"
#include "stmt.h"
#include "alist.h"
#include "iterator.h"
#include "LoopExpr.h"
#include "AstVisitorTraverse.h"
#include "passes.h"
#include "resolution.h"
#include "thunks.h"

/************************************* | **************************************
*                               Pass 8: Normalize                             *
* Convert thunk primitive calls into calls to a builder function. Like in the *
* case of iterators, the builder function starts as a placeholder for the     *
* body of the thunk.                                                          *
*                                                                             *
************************************** | *************************************/

class LowerThunkPrimsVisitor final : public AstVisitorTraverse
{
  public:
    LowerThunkPrimsVisitor()          = default;
   ~LowerThunkPrimsVisitor() override = default;

    bool enterCallExpr(CallExpr* node) override;
};

static void collectThunkOuterVars(Expr* expr, std::set<Symbol*>& outerVars) {
  std::vector<SymExpr*> uses;
  collectSymExprs(expr, uses);
  for (auto use : uses) {
    Symbol* sym = use->symbol();
    if (considerForOuter(sym) && isOuterVarLoop(sym, expr))
      outerVars.insert(sym);
  }
}

static CallExpr* buildThunkPrimFunctions(CallExpr* node) {
  CHPL_ASSERT(node->numActuals() == 1);
  auto delayedExpr = node->get(1);

  std::set<Symbol*> outerVars;
  collectThunkOuterVars(delayedExpr, outerVars);

  static int thunkUid = 0;
  thunkUid++;
  auto thunkUidStr = istr(thunkUid);
  auto thunkName = astr("chpl__thunk", thunkUidStr);
  auto thunkFn = new FnSymbol(thunkName);
  thunkFn->addFlag(FLAG_COMPILER_GENERATED);
  thunkFn->addFlag(FLAG_THUNK_BUILDER);
  if (outerVars.size() > 0) thunkFn->setGeneric(true);
  node->getStmtExpr()->insertBefore(new DefExpr(thunkFn));

  auto thunkResultTmp = newTemp("thunkResult");
  thunkResultTmp->addFlag(FLAG_NO_AUTO_DESTROY);
  auto thunkBlock = new BlockStmt();
  thunkBlock->insertAtTail(new DefExpr(thunkResultTmp));
  thunkBlock->insertAtTail(new CallExpr(PRIM_INIT_VAR, thunkResultTmp, delayedExpr->remove()));
  thunkBlock->insertAtTail(new CallExpr(PRIM_THUNK_RESULT, thunkResultTmp));
  thunkFn->body->insertAtTail(thunkBlock);

  SymbolMap map;
  auto thunkCall = new CallExpr(thunkFn);
  for (auto outerVar : outerVars) {
    auto outerVarArg = newOuterVarArg(outerVar);
    map.put(outerVar, outerVarArg);
    thunkFn->insertFormalAtTail(outerVarArg);
    thunkCall->insertAtTail(new SymExpr(outerVar));
  }
  update_symbols(thunkFn, &map);

  normalizeGeneratedLoweringFn(thunkFn);

  return thunkCall;
}

//
// To match lowering LoopExprs (see LoopExpr.cpp, lowerLoopExpr), lower
// outer-most calls to thunk primitive first, in order to simplify scope-resolution
// of newly-created functions.
//
bool LowerThunkPrimsVisitor::enterCallExpr(CallExpr* node) {
  if (!node->isPrimitive(PRIM_CREATE_THUNK)) return true;

  SET_LINENO(node);
  CallExpr* replacement = buildThunkPrimFunctions(node);
  node->replace(replacement);
  normalize(replacement);

  return false;
}

void lowerThunkPrims(BaseAST* ast) {
  INT_ASSERT(ast->inTree()); // otherwise nothing to do
  LowerThunkPrimsVisitor vis;
  ast->accept(&vis);
}


/************************************* | **************************************
*                               Pass 12: Resolve                              *
* Insert an "invoke" function that calls the thunk's body, build a thunk      *
* record that will eventually (but not yet) contain the required outer vars,  *
* configure the builder function to return an instance of that record.        *
*                                                                             *
************************************** | *************************************/

static Type* findThunkResultType(FnSymbol* fn) {
  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn->body, callExprs);
  Type* thunkResultType = nullptr;
  for (auto call : callExprs) {
    if (call->isPrimitive(PRIM_THUNK_RESULT)) {
      thunkResultType = call->typeInfo();
      break;
    }
  }
  return thunkResultType;
}

static AggregateType* buildThunkRecord(FnSymbol* fn) {
  auto newRecord = new AggregateType(AGGREGATE_RECORD);
  auto recordName = astr("_tr_", fn->name);
  auto recordSym = new TypeSymbol(recordName, newRecord);

  recordSym->addFlag(FLAG_THUNK_RECORD);
  recordSym->addFlag(FLAG_COMPILER_GENERATED);
  return newRecord;
}

static FnSymbol* buildThunkInvokeMethod(FnSymbol* fn,
                                        Type* thunkResultType,
                                        AggregateType* newRecord) {
  FnSymbol* invokeFn = new FnSymbol("invoke");
  newRecord->thunkInvoke = invokeFn;

  invokeFn->addFlag(FLAG_THUNK_INVOKE);
  invokeFn->addFlag(FLAG_COMPILER_GENERATED);
  invokeFn->addFlag(FLAG_INLINE);
  invokeFn->setMethod(true);

  invokeFn->_this   = new ArgSymbol(INTENT_REF, "this", newRecord);
  invokeFn->_this->addFlag(FLAG_ARG_THIS);

  invokeFn->retType = thunkResultType;

  invokeFn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  invokeFn->insertFormalAtTail(invokeFn->_this);
  return invokeFn;
}

void protoThunkRecord(FnSymbol* fn) {
  auto thunkResultType = findThunkResultType(fn);
  INT_ASSERT(thunkResultType);

  // Create a record that contains the thunk state (currently: captured variables),
  // which has a method to perform the computation.
  auto newRecord = buildThunkRecord(fn);
  auto invokeFn = buildThunkInvokeMethod(fn, thunkResultType, newRecord);

  // Adjust the function's return type and the type of the return symbol
  setReturnAndReturnSymbolType(fn, newRecord);

  fn->defPoint->insertBefore(new DefExpr(newRecord->symbol));
  fn->defPoint->insertBefore(new DefExpr(invokeFn));

  makeRefType(newRecord);
  normalize(invokeFn);

  // Pretend that this function is already resolved.
  // Its body will be filled in during the lowerIterators pass.
  invokeFn->addFlag(FLAG_RESOLVED);
}

/************************************* | **************************************
*                           Pass 20: Lower Iterators                          *
* Relocate the body of the thunk into the constructed "invoke" method, while  *
* replacing references to outer variables with field accesses in the thunk    *
* record. Modify the builder to actually build the thunk record (previously   *
* it didn't return anything, and contained the thunk body).                   *
*                                                                             *
************************************** | *************************************/


static AggregateType* getThunkBuilderReturnType(FnSymbol* fn) {
  Type* returnType = nullptr;
  if (!fn->hasFlag(FLAG_FN_RETARG)) {
    returnType = fn->retType->getValType();
  } else {
    for_formals(formal, fn) {
      if (formal->hasFlag(FLAG_RETARG)) returnType = formal->type->getValType();
    }
  }

  INT_ASSERT(returnType);
  auto recordType = toAggregateType(returnType);
  INT_ASSERT(recordType);
  INT_ASSERT(recordType->aggregateTag == AGGREGATE_RECORD);
  INT_ASSERT(recordType->symbol->hasFlag(FLAG_THUNK_RECORD));

  return recordType;
}

static void addLocalsToThunkRecord(FnSymbol* fn,
                                   Vec<Symbol*>& locals,
                                   SymbolMap& local2field,
                                   AggregateType* thunkRecord) {
  int counter = 0;
  for (auto local : locals) {
    Symbol* field = createICField(counter, local, NULL, /* isValueField */ false, fn);
    local2field.put(local, field);
    thunkRecord->fields.insertAtTail(new DefExpr(field));
  }
}

static void replaceLocalsWithThunkFields(FnSymbol* fn,
                                         Symbol* tr,
                                         std::vector<BaseAST*>& asts,
                                         Vec<Symbol*>& locals,
                                         SymbolMap& local2field) {
  Vec<SymExpr*> defSet;
  Vec<SymExpr*> useSet;
  buildDefUseSets(locals, fn, defSet, useSet);

  for (BaseAST* ast : chpl::expandingIterator(asts)) {
    auto se = toSymExpr(ast);
    if (!se || se->parentSymbol == nullptr) continue;

    replaceLocalUseOrDefWithFieldRef(se, tr, asts, local2field, defSet, useSet);
  }
}

static void populateThunkInvokeFn(FnSymbol* fn, FnSymbol* invokeFn) {
  auto thunkBody = fn->body->body.last();
  invokeFn->body->replace(thunkBody->remove());

  // Find the thunk result, transform it into a return at the end of the function.
  for_alist_backward(expr, invokeFn->body->body) {
    if (auto call = toCallExpr(expr)) {
      if (call->isPrimitive(PRIM_THUNK_RESULT)) {
        auto toReturn = call->get(1);
        call->remove();
        insertReturn(invokeFn, toSymExpr(toReturn)->symbol());
        break;
      }
    }
  }
}

static void rebuildThunkBuilder(FnSymbol* fn,
                                Vec<Symbol*>& locals,
                                SymbolMap& local2field,
                                AggregateType* thunkRecord) {
  fn->retSymbol = NULL;
  for_alist(expr, fn->body->body)
    expr->remove();

  Symbol* thunk = newTemp("_tr", thunkRecord);
  fn->insertAtTail(new DefExpr(thunk));

  // Avoids a valgrind warning about uninitialized memory when performing
  // indirect modification checks on const and default intent arguments
  if (fWarnUnstable && !fNoConstArgChecks) {
    fn->insertAtTail(new CallExpr(PRIM_ZERO_VARIABLE, new SymExpr(thunk)));
  }

  initializeRecordFieldWithArgLocals(fn, thunk, locals, local2field);

  // Return the filled-in iterator record.
  insertReturn(fn, thunk);

  fn->addFlag(FLAG_INLINE);
}

void lowerThunk(FnSymbol* fn) {
  SET_LINENO(fn);

  std::vector<BaseAST*> asts;
  collect_asts_postorder(fn, asts);

  // Match the behavior of lowerIterator, though I'm not sure that this
  // is necessary.
  removeRetSymbolAndUses(fn);

  // The builder function produces a thunk record, so get its return type
  // to get a handle on the record.
  auto thunkRecord = getThunkBuilderReturnType(fn);
  auto invokeFn = thunkRecord->thunkInvoke;

  // We'll be creating fields for all variables needed to invoke the thunk.
  // Store such variables into locals, and their corresponding fields by
  // using local2field.
  SymbolMap local2field;
  Vec<Symbol*> locals;

  // All function formals need to be packaged in with the thunk record.
  for_formals(formal, fn) {
    if (formal->hasFlag(FLAG_RETARG)) continue;
    locals.push_back(formal);
  }

  addLocalsToThunkRecord(fn, locals, local2field, thunkRecord);

  auto tr = invokeFn->getFormal(1);

  replaceLocalsWithThunkFields(fn, tr, asts, locals, local2field);

  populateThunkInvokeFn(fn, invokeFn);

  rebuildThunkBuilder(fn, locals, local2field, thunkRecord);
}
