/*
 * Copyright 2004-2015 Cray Inc.
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

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

void
removeEmptyRecords() {
  if (fNoRemoveEmptyRecords)
    return;

  Vec<Type*> emptyRecordTypeSet;
  int numEmptyRecordTypes = 0;
  Vec<Symbol*> emptyRecordSymbolSet;

  //
  // Iteratively identify all empty record types (knowing that a
  // record is empty if all of its fields are empty records (and will
  // therefore be removed)
  //
  bool change = true;
  while (change) {
    change = false;
    forv_Vec(AggregateType, ct, gAggregateTypes) {
      if (isRecord(ct) && ct->symbol->defPoint->parentSymbol && 
          !ct->symbol->hasFlag(FLAG_EXTERN)) {
        bool empty = true;
        for_fields(field, ct) {
          if (!emptyRecordTypeSet.set_in(field->type)) {
            empty = false;
            break;
          }
        }
        if (empty) {
          change = true;
          emptyRecordTypeSet.set_add(ct);
          numEmptyRecordTypes++;
          ct->symbol->defPoint->remove();
          if (ct->refType) {
            emptyRecordTypeSet.set_add(ct->refType);
            numEmptyRecordTypes++;
            ct->refType->symbol->defPoint->remove();
          }
        }
      }
    }
  }

  if (!numEmptyRecordTypes)
    return;

  compute_call_sites();

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (emptyRecordTypeSet.set_in(arg->type)) {
      forv_Vec(CallExpr, call, *arg->getFunction()->calledBy) {
        formal_to_actual(call, arg)->remove();
      }
      emptyRecordSymbolSet.set_add(arg);
      arg->defPoint->remove();
    }
  }

  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (emptyRecordTypeSet.set_in(var->type)) {
      emptyRecordSymbolSet.set_add(var);
      var->defPoint->remove();
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (emptyRecordTypeSet.set_in(fn->retType)) {
      CallExpr* ret = toCallExpr(fn->body->body.last());
      INT_ASSERT(ret && ret->isPrimitive(PRIM_RETURN));
      ret->get(1)->replace(new SymExpr(gVoid));
      fn->retType = dtVoid;
      forv_Vec(CallExpr, call, *fn->calledBy) {
        if (CallExpr* move = toCallExpr(call->parentExpr)) {
          INT_ASSERT(move->isPrimitive(PRIM_MOVE));
          move->replace(call->remove());
        }
      }
    }
    if (fn->_this && emptyRecordTypeSet.set_in(fn->_this->type))
      fn->_this = NULL;
  }

  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->parentSymbol && emptyRecordSymbolSet.set_in(se->var)) {
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        if (call->primitive) {
          //call->isPrimitive(PRIM_SET_MEMBER) ||
          //            call->isPrimitive(PRIM_ADDR_OF) ||
          //            call->isPrimitive(PRIM_DEREF) ||
          //            call->isPrimitive(PRIM_MOVE)) {
          call->getStmtExpr()->remove();
          continue;
        }
      }
      INT_FATAL(se, "unhandled case in remove empty records");
    }
  }
}
