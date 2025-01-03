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

#include "InitErrorHandling.h"

#include "AstVisitor.h"
#include "AstVisitorTraverse.h"
#include "ForallStmt.h"
#include "IfExpr.h"
#include "LoopExpr.h"
#include "stmt.h"
#include "astutil.h"

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

InitErrorHandling::InitErrorHandling(FnSymbol* fn) {
  mFn            = fn;
  mPhase         = startPhase(fn);
}

InitErrorHandling::InitErrorHandling(CondStmt* cond, const InitErrorHandling& curr) {
  mFn            = curr.mFn;
  mPhase         = curr.mPhase;
}

void InitErrorHandling::merge(const InitErrorHandling& fork) {
  mPhase     = fork.mPhase;
}

FnSymbol* InitErrorHandling::theFn() const {
  return mFn;
}

InitErrorHandling::InitPhase InitErrorHandling::currPhase() const {
  return mPhase;
}

bool InitErrorHandling::isPhase0() const {
  return mPhase == cPhase0;
}

bool InitErrorHandling::isPhase1() const {
  return mPhase == cPhase1;
}

bool InitErrorHandling::isPhase2() const {
  return mPhase == cPhase2;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/
static const char* initName(CallExpr* stmt);

bool isResolvedSuperInit(CallExpr* stmt) {
  const char* name = initName(stmt);

  return name != NULL && name == astrSuper ? true : false;
}

bool isResolvedThisInit (CallExpr* stmt) {
  const char* name = initName(stmt);

  return name != NULL && name == astrThis ? true: false;
}

bool InitErrorHandling::isInitDone (CallExpr* stmt) const {
  return stmt->isPrimitive(PRIM_INIT_DONE);
}

bool isInitStmt(CallExpr* stmt) {
  return isResolvedSuperInit(stmt) == true || isResolvedThisInit(stmt) == true;
}

static const char* initName(CallExpr* stmt) {
  const char* retval = NULL;

  // Not in a primitive
  if (stmt->baseExpr != NULL) {
    if (SymExpr* base = toSymExpr(stmt->baseExpr)) {

      // Call is to an initializer
      if (base->symbol()->name == astrInit &&
          stmt->numActuals() >= 1) {
        SymExpr* firstArg = toSymExpr(stmt->get(1));
        if (firstArg->symbol()->name == astrSuper ||
            strcmp(firstArg->symbol()->name, "super_tmp") == 0) {
          // In case the code has been normalized such that a temporary was
          // inserted instead of leaving it directly in the call
          retval = astrSuper;
        } else if (firstArg->symbol()->hasFlag(FLAG_ARG_THIS)) {
          retval = astrThis;
        }
      }
    } else if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(stmt->baseExpr)) {
      if (urse->unresolved == astrInit) {
        INT_ASSERT(false); // This shouldn't happen
      }
    }
  }

  return retval;
}

bool InitErrorHandling::hasInitDone(BlockStmt* block) {
  Expr* stmt   = block->body.head;
  bool  retval = false;

  while (stmt != NULL && retval == false) {
    if (CallExpr* callExpr = toCallExpr(stmt)) {
      retval = isInitDone(callExpr);

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      if (cond->elseStmt == NULL) {
        retval = hasInitDone(cond->thenStmt);

      } else {
        retval = hasInitDone(cond->thenStmt) || hasInitDone(cond->elseStmt);
      }

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      retval = hasInitDone(block);

    } else if (ForallStmt* block = toForallStmt(stmt)) {
      retval = hasInitDone(block->loopBody());
    }

    stmt = stmt->next;
  }

  return retval;
}



/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void InitErrorHandling::completePhase1(CallExpr* initStmt) {
  mPhase = cPhase2;
}

void InitErrorHandling::completePhase0(CallExpr* initStmt) {
  if (isResolvedSuperInit(initStmt) == true) {
    mPhase = cPhase1;
  } else {
    INT_FATAL("completePhase0 expected to be called with super.init");
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

InitErrorHandling::InitPhase InitErrorHandling::startPhase(FnSymbol*  fn)    const {
  return startPhase(fn->body);
}

InitErrorHandling::InitPhase InitErrorHandling::startPhase(BlockStmt* block) const {
  Expr*     stmt   = block->body.head;
  const InitPhase defaultPhase = cPhase1;
  InitPhase retval = defaultPhase;

  while (stmt != NULL && retval == defaultPhase) {
    if (isDefExpr(stmt) == true) {
      stmt = stmt->next;

    } else if (CallExpr* callExpr = toCallExpr(stmt)) {
      if        (isResolvedThisInit(callExpr)  == true) {
        retval = cPhase0;
        break;

      } else if (isResolvedSuperInit(callExpr) == true) {
        retval = cPhase0;
        break;

      } else if (isInitDone(callExpr)  == true) {
        retval = cPhase1;
        break;

      } else {
        stmt   = stmt->next;
      }

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      if (cond->elseStmt == NULL) {
        InitPhase thenPhase = startPhase(cond->thenStmt);

        if (thenPhase != defaultPhase) {
          retval = thenPhase;
        } else {
          stmt   = stmt->next;
        }

      } else {
        InitPhase thenPhase = startPhase(cond->thenStmt);
        InitPhase elsePhase = startPhase(cond->elseStmt);

        if        (thenPhase == cPhase0 || elsePhase == cPhase0) {
          retval = cPhase0;

        } else if (thenPhase == cPhase1 || elsePhase == cPhase1) {
          retval = cPhase1;
          // We want to keep looking in case there's a super.init further
          // down.
          stmt = stmt->next;

        } else {
          stmt   = stmt->next;
        }
      }

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      InitPhase phase = startPhase(block);

      if (phase != defaultPhase) {
        retval = phase;
      } else {
        stmt   = stmt->next;
      }

    } else if (ForallStmt* forall = toForallStmt(stmt)) {
      InitPhase phase = startPhase(forall->loopBody());

      if (phase != defaultPhase) {
        retval = phase;
      } else {
        stmt   = stmt->next;
      }

    } else {
      stmt = stmt->next;
    }
  }

  return retval;
}

void InitErrorHandling::describe(int offset) const {
  char pad[512];

  for (int i = 0; i < offset; i++) {
    pad[i] = ' ';
  }

  pad[offset] = '\0';

  printf("%s#<InitErrorHandling\n", pad);

  printf("%s  Phase: %s\n", pad, phaseToString(mPhase));
  printf("%s>\n", pad);
}

const char*
InitErrorHandling::phaseToString(InitErrorHandling::InitPhase phase) const {
  const char* retval = "?";

  switch (phase) {
    case cPhase0:
      retval = "Phase0";
      break;

    case cPhase1:
      retval = "Phase1";
      break;

    case cPhase2:
      retval = "Phase2";
      break;
  }

  return retval;
}

void InitErrorHandling::removeInitDone() {
  // Clean up after ourselves - remove PRIM_INIT_DONE from the function
  std::vector<CallExpr*> calls;
  collectCallExprs(mFn, calls);

  for_vector(CallExpr, call, calls) {
    if (isInitDone(call)) {
      call->remove();
    }
  }
}
