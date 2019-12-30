/*
 * Copyright 2004-2020 Cray Inc.
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

AList::AList() :
  head(NULL),
  tail(NULL),
  parent(NULL),
  length(0)
{ }


Expr* AList::first(void) {
  return head;
}


Expr* AList::last(void) {
  return tail;
}


Expr* AList::only(void) {
  if (!head)
    INT_FATAL("only() called on empty list");

  if (head == tail)
    return first();
  else
    INT_FATAL("only() called on list with more than one element");

  return NULL;
}


Expr* AList::get(int index) const {
  if (index <=0) {
    INT_FATAL("Indexing list must use positive integer");
  }

  int i = 0;

  for_alist(node, *this) {
    i++;

    if (i == index) {
      return node;
    }
  }

  INT_FATAL("Indexing list out of bounds");

  return NULL;
}

bool AList::empty() {
  return length == 0;
}

void AList::insertAtHead(Expr* new_ast) {
  if (new_ast->parentSymbol || new_ast->parentExpr)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtHead");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtHead");

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


void AList::insertAtTail(Expr* new_ast) {
  if (new_ast->parentSymbol || new_ast->parentExpr)
    INT_FATAL(new_ast, "Argument is already in AST in AList::insertAtTail");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in AList::insertAtTail");

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
