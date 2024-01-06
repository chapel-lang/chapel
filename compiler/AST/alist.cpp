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

#include "alist.h"
#include "astutil.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"

#define VALIDATE_AST_INSERTABLE_HEAD(NEW_AST, LOC_STR) \
  do { \
    if (NEW_AST->parentSymbol || NEW_AST->parentExpr) \
      INT_FATAL(NEW_AST, "Argument is already in AST in " LOC_STR); \
    if (NEW_AST->list) \
      INT_FATAL(NEW_AST, "Argument is in a list in " LOC_STR); \
  } while(false)

#define VALIDATE_AST_INSERTABLE_TAIL(NEW_AST, LOC_STR) \
  do { \
    if (NEW_AST->parentSymbol || NEW_AST->parentExpr) \
      INT_FATAL(NEW_AST, "Argument is already in AST in " LOC_STR); \
    if (NEW_AST->next || NEW_AST->prev) \
      INT_FATAL(NEW_AST, "Argument is in a list in " LOC_STR); \
  } while(false)


void AList::insertAtHead(Expr* new_ast) {
  VALIDATE_AST_INSERTABLE_HEAD(new_ast, "AList::insertAtHead");

  // This should probably be moved into the caller, for cases that require it.
  if (CallExpr* call = toCallExpr(new_ast)) {
    if (call && call->isPrimitive(PRIM_ACTUALS_LIST)) {
      for_actuals(expr, call) {
        expr->remove();
        insertAtHead(expr);
      }
      return;
    }
  }

  performInsertAtHead(new_ast);
}

void AList::insertAtHeadWithoutFlattening(Expr* new_ast) {
  VALIDATE_AST_INSERTABLE_HEAD(new_ast, "AList::insertAtHeadWithoutFlattening");
  performInsertAtHead(new_ast);
}


void AList::insertAtTail(Expr* new_ast) {
  VALIDATE_AST_INSERTABLE_TAIL(new_ast, "AList::insertAtTail");

  // See above, regarding PRIM_ACTUALS_LIST.
  if (CallExpr* call = toCallExpr(new_ast)) {
    if (call && call->isPrimitive(PRIM_ACTUALS_LIST)) {
      for_actuals(expr, call) {
        expr->remove();
        insertAtTail(expr);
      }
      return;
    }
  }

  performInsertAtTail(new_ast);
}

void AList::insertAtTailWithoutFlattening(Expr* new_ast) {
  VALIDATE_AST_INSERTABLE_TAIL(new_ast, "AList::insertAtTailWithoutFlattening");
  performInsertAtTail(new_ast);
}

void AList::performInsertAtHead(Expr* new_ast) {
  if (!head) {
    head = new_ast;
    tail = new_ast;
    new_ast->list = this;
  } else {
    new_ast->next = head;
    head = new_ast;
    new_ast->next->prev = new_ast;
    new_ast->list = this;
  }
  parent_insert_help(parent, new_ast);
  length++;
}

void AList::performInsertAtTail(Expr* new_ast) {
  if (!tail) {
    head = new_ast;
    tail = new_ast;
    new_ast->list = this;
  } else {
    new_ast->prev = tail;
    tail = new_ast;
    new_ast->prev->next = new_ast;
    new_ast->list = this;
  }
  parent_insert_help(parent, new_ast);
  length++;
}
