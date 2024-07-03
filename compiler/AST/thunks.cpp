/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
