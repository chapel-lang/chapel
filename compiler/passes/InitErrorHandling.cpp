/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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
  mBlockType     = cBlockNormal;
  mPrevBlockType = cBlockNormal;
}

InitErrorHandling::InitErrorHandling(BlockStmt* block, const InitErrorHandling& curr) {
  mFn            = curr.mFn;
  mPhase         = curr.mPhase;

  if (CallExpr* blockInfo = block->blockInfoGet()) {
    if        (blockInfo->isPrimitive(PRIM_BLOCK_BEGIN)       == true ||
               blockInfo->isPrimitive(PRIM_BLOCK_BEGIN_ON)    == true) {
      mBlockType = cBlockBegin;

    } else if (blockInfo->isPrimitive(PRIM_BLOCK_COBEGIN)     == true) {
      // Lydia NOTE 2017/07/26: If PRIM_BLOCK_COBEGIN_ONs are ever made, we
      // should match against them here
      mBlockType = cBlockCobegin;

    } else if (blockInfo->isPrimitive(PRIM_BLOCK_COFORALL)    == true ||
               blockInfo->isPrimitive(PRIM_BLOCK_COFORALL_ON) == true) {
      mBlockType = cBlockCoforall;

    } else if (blockInfo->isPrimitive(PRIM_BLOCK_ON) == true ||
               blockInfo->isPrimitive(PRIM_BLOCK_ELIDED_ON)) {
      mBlockType = cBlockOn;

    } else {
      INT_ASSERT(false);
    }

  } else {
    mBlockType = curr.mBlockType;
  }

  if (mBlockType != curr.mBlockType) {
    mPrevBlockType = curr.mBlockType;
  } else {
    mPrevBlockType = curr.mPrevBlockType;
  }
}

InitErrorHandling::InitErrorHandling(CondStmt* cond, const InitErrorHandling& curr) {
  mFn            = curr.mFn;
  mPhase         = curr.mPhase;
  mPrevBlockType = curr.mPrevBlockType;
  mBlockType     = curr.mBlockType;
}

InitErrorHandling::InitErrorHandling(LoopStmt* loop, const InitErrorHandling& curr) {
  mFn            = curr.mFn;
  mPhase         = curr.mPhase;
  mBlockType     = cBlockLoop;

  if (mBlockType != curr.mBlockType) {
    mPrevBlockType = curr.mBlockType;

  } else {
    mPrevBlockType = curr.mPrevBlockType;
  }
}

InitErrorHandling::InitErrorHandling(ForallStmt* loop, const InitErrorHandling& curr) {
  mFn            = curr.mFn;
  mPhase         = curr.mPhase;
  mBlockType     = cBlockForall;

  if (mBlockType != curr.mBlockType) {
    mPrevBlockType = curr.mBlockType;

  } else {
    mPrevBlockType = curr.mPrevBlockType;
  }
}

void InitErrorHandling::merge(const InitErrorHandling& fork) {
  mPhase     = fork.mPhase;
}

AggregateType* InitErrorHandling::type() const {
  return mFn != NULL ? toAggregateType(mFn->_this->type) : NULL;
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

bool InitErrorHandling::inLoopBody() const {
  return mBlockType == cBlockLoop;
}

bool InitErrorHandling::inParallelStmt() const {
  return mBlockType == cBlockBegin   ||
         mBlockType == cBlockCobegin  ;
}

bool InitErrorHandling::inCoforall() const {
  return mBlockType == cBlockCoforall;
}

bool InitErrorHandling::inForall() const {
  return mBlockType == cBlockForall;
}

bool InitErrorHandling::inOn() const {
  return mBlockType == cBlockOn;
}

bool InitErrorHandling::inOnInLoopBody() const {
  return inOn() && mPrevBlockType == cBlockLoop;
}

bool InitErrorHandling::inOnInParallelStmt() const {
  return inOn() && (mPrevBlockType == cBlockBegin ||
                    mPrevBlockType == cBlockCobegin);
}

bool InitErrorHandling::inOnInCoforall() const {
  return inOn() && mPrevBlockType == cBlockCoforall;
}

bool InitErrorHandling::inOnInForall() const {
  return inOn() && mPrevBlockType == cBlockForall;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/
static const char* initName(CallExpr* stmt);

bool isResolvedSuperInit(CallExpr* stmt) {
  const char* name = initName(stmt);

  return name != NULL && strcmp(name, "super") == 0 ? true : false;
}

bool isResolvedThisInit (CallExpr* stmt) {
  const char* name = initName(stmt);

  return name != NULL && strcmp(name, "this") == 0 ? true: false;
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
    SymExpr* base = toSymExpr(stmt->baseExpr);
    INT_ASSERT(base); // Check my assumptions

    // Call is to an initializer
    if (strcmp(base->symbol()->name, "init") == 0 &&
        stmt->numActuals() >= 1) {
      SymExpr* firstArg = toSymExpr(stmt->get(1));
      if (strcmp(firstArg->symbol()->name, "super") == 0 ||
          strcmp(firstArg->symbol()->name, "super_tmp") == 0) {
        retval = "super";
      } else if (firstArg->symbol()->hasFlag(FLAG_ARG_THIS)) {
        retval = "this";
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
      // Nothing to normalize in fRecIter*
      INT_ASSERT(forall->fRecIterIRdef == NULL);

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

//
// Catch case a case like:
//
// proc init(cond:bool) {
//   if cond {
//     this.initDone();
//   } else {
//     this.init();
//   }
// }
//
// The initializer begins in phase zero, which is the state we inherit upon
// entering the 'then' branch of the conditional statement. This is the
// incorrect phase though, so we need to re-check the start phase of the
// 'then' block and use that instead.
//
// Note though that we don't want to update the state unless there is an
// initDone. Otherwise simple conditionals like this would advance the outer
// phase to phase 1:
//
// proc init(cond:bool) {
//   if cond {
//     writeln("then");
//   } else {
//     writeln("else");
//   }
//   super.init();
// }
//
// INIT TODO: Can we restrict this operation to just conditionals?
//
void InitErrorHandling::checkPhase(BlockStmt* block) {
  if (mPhase == cPhase0) {
    InitPhase newPhase = startPhase(block);

    if (newPhase == cPhase1) {
      if (hasInitDone(block)) {
        mPhase = newPhase;
      }
    }
  }
}

void InitErrorHandling::describe(int offset) const {
  char pad[512];

  for (int i = 0; i < offset; i++) {
    pad[i] = ' ';
  }

  pad[offset] = '\0';

  printf("%s#<InitErrorHandling\n", pad);

  printf("%s  Phase: %s\n", pad, phaseToString(mPhase));

  printf("%s  Block: ",       pad);

  switch (mBlockType) {
    case cBlockNormal:
      printf("normal\n");
      break;

    case cBlockLoop:
      printf("loop\n");
      break;

    case cBlockBegin:
      printf("begin\n");
      break;

    case cBlockCobegin:
      printf("cobegin\n");
      break;

    case cBlockCoforall:
      printf("coforall\n");
      break;

    case cBlockForall:
      printf("forall\n");
      break;

    case cBlockOn:
      printf("on\n");
      break;
  }

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
  for_alist(node, mFn->body->body) {
    if (CallExpr* call = toCallExpr(node)) {
      if (isInitDone(call)) {
        call->remove();
      }
    }
  }
}
