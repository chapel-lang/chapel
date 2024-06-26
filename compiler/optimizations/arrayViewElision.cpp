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

#include <vector>

#include "arrayViewElision.h"
#include "global-ast-vecs.h"
#include "passes.h"
#include "resolution.h"

static bool exprSuitableForProtoSlice(Expr* e);

void arrayViewElision() {
  if (!fArrayViewElision) return;

  std::vector<CallExpr*> candidates;

  for_alive_in_Vec (CallExpr, call, gCallExprs) {
    if (call->getModule()->modTag == MOD_USER) {
      if (call->isNamed("=")) {
        if (exprSuitableForProtoSlice(call->get(1)) &&
            exprSuitableForProtoSlice(call->get(2))) {
          //std::cout << call->stringLoc() << std::endl;
          //nprint_view(call);
          candidates.push_back(call);
        }
      }
    }
  }

  for_vector(CallExpr, call, candidates) {
    SET_LINENO(call);

    CallExpr* lhs = toCallExpr(call->get(1));
    CallExpr* rhs = toCallExpr(call->get(2));

    Expr* lhsBase = lhs->baseExpr;
    Expr* rhsBase = rhs->baseExpr;

    CallExpr* lhsPSCall = new CallExpr("chpl__createProtoSlice", lhsBase->copy());
    for_actuals(actual, lhs) {
      lhsPSCall->insertAtTail(actual->copy());
    }

    CallExpr* rhsPSCall = new CallExpr("chpl__createProtoSlice", rhsBase->copy());
    for_actuals(actual, rhs) {
      rhsPSCall->insertAtTail(actual->copy());
    }

    // arrayview elision placeholder
    VarSymbol* placeholder = new VarSymbol("arrayview_elision_flag", dtBool);
    placeholder->addFlag(FLAG_ARRAYVIEW_ELISION_FLAG);

    call->insertBefore(new DefExpr(placeholder, gFalse));

    BlockStmt* thenBlock = new BlockStmt();
    VarSymbol* lhsPS = new VarSymbol("lhs_proto_slice");
    VarSymbol* rhsPS = new VarSymbol("rhs_proto_slice");

    thenBlock->insertAtTail(new DefExpr(lhsPS, lhsPSCall));
    thenBlock->insertAtTail(new DefExpr(rhsPS, rhsPSCall));
    thenBlock->insertAtTail(new CallExpr(PRIM_PROTO_SLICE_ASSIGN, lhsPS,
                                         rhsPS));

    BlockStmt* elseBlock = new BlockStmt();

    CondStmt* cond = new CondStmt(new SymExpr(placeholder), thenBlock,
                                  elseBlock);

    call->insertBefore(cond);
    elseBlock->insertAtTail(call->remove());

    //list_view(cond);
  }
}

ProtoSliceAssignHelper::ProtoSliceAssignHelper(CallExpr* call):
    call_(call),
    newProtoSliceLhs_(nullptr),
    newProtoSliceRhs_(nullptr),
    condStmt_(nullptr),
    supported_(false),
    staticCheckBlock_(nullptr) {

  findProtoSlices();
  INT_ASSERT(newProtoSliceLhs_);
  INT_ASSERT(newProtoSliceRhs_);


  // this is just a temporary block. we add some AST in it, resolve and then
  // remove them. I cannot be sure if those operations could leave any AST in.
  // So, when we destroy this helper, we'll remove this block just to be sure.
  staticCheckBlock_ = new BlockStmt();

  BlockStmt* parentBlock = toBlockStmt(call->parentExpr);
  parentBlock->insertAtHead(staticCheckBlock_);

  supported_ = handleOneProtoSlice(newProtoSliceLhs_) &&
               handleOneProtoSlice(newProtoSliceRhs_);

  findCondStmt();
  INT_ASSERT(condStmt_);
}

ProtoSliceAssignHelper::~ProtoSliceAssignHelper() {
  staticCheckBlock_->remove();
  tmpCondFlag_->getSingleDef()->getStmtExpr()->remove();
  tmpCondFlag_->defPoint->remove();
}

CallExpr* ProtoSliceAssignHelper::getReplacement() {
  return new CallExpr("=", call_->get(1)->copy(), call_->get(2)->copy());
}

void ProtoSliceAssignHelper::report() {
  if (!fReportArrayViewElision) return;

  std::string isSupported = supported() ? "supported" : "not supported";
  std::cout << "ArrayViewElision " << isSupported << " " << call_->stringLoc()
            << std::endl;
}

bool ProtoSliceAssignHelper::handleOneProtoSlice(CallExpr* call) {
  INT_ASSERT(call->isNamed("chpl__createProtoSlice"));

  CallExpr* typeCheck = new CallExpr("chpl__typesSupportArrayViewElision");
  for_actuals (actual, call) {
    INT_ASSERT(isSymExpr(actual));
    typeCheck->insertAtTail(actual->copy());
  }

  VarSymbol* tmp = newTemp("call_tmp", dtBool);
  DefExpr* flagDef = new DefExpr(tmp, typeCheck);

  staticCheckBlock_->insertAtTail(flagDef);

  resolveExpr(typeCheck);
  resolveExpr(flagDef);

  bool ret = (toSymExpr(flagDef->init)->symbol() == gTrue);

  flagDef->remove();

  return ret;
}

// e must be the lhs or rhs of PRIM_ASSIGN_PROTO_SLICES
// returns the `chpl__createProtoSlice call
CallExpr* ProtoSliceAssignHelper::findOneProtoSliceCall(Expr* e) {
  SymExpr* lhsSE = toSymExpr(call_->get(1));
  INT_ASSERT(lhsSE);

  Symbol* lhs = lhsSE->symbol();
  CallExpr* lhsTmpMove = toCallExpr(lhs->getSingleDef()->getStmtExpr());
  INT_ASSERT(lhsTmpMove && lhsTmpMove->isPrimitive(PRIM_MOVE));

  SymExpr* lhsTmpSE = toSymExpr(lhsTmpMove->get(2));
  INT_ASSERT(lhsTmpSE);

  Symbol* lhsTmpSym = lhsTmpSE->symbol();
  CallExpr* lhsMove = toCallExpr(lhsTmpSym->getSingleDef()->getStmtExpr());
  INT_ASSERT(lhsMove && lhsMove->isPrimitive(PRIM_MOVE));

  return toCallExpr(lhsMove->get(2));
}

void ProtoSliceAssignHelper::findProtoSlices() {
  newProtoSliceLhs_ = findOneProtoSliceCall(call_->get(1));
  newProtoSliceRhs_ = findOneProtoSliceCall(call_->get(2));
}

void ProtoSliceAssignHelper::findCondStmt() {
  Expr* cur = call_;
  while (cur) {
    if (CondStmt* condStmt = toCondStmt(cur)) {
      if (SymExpr* condExpr = toSymExpr(condStmt->condExpr)) {
        if (condExpr->symbol()->hasFlag(FLAG_ARRAYVIEW_ELISION_FLAG)) {
          tmpCondFlag_ = condExpr->symbol();
          condStmt_ = condStmt;
          break;
        }
        else {
          // this is an unknown conditional, this shouldn't have happened
          INT_FATAL(call_,
                    "unexpected syntax tree generated by arrayview elision");
        }
      }
    }

    cur = cur->parentExpr;
  }

  if (condStmt_ == nullptr) {
    // where is the conditional?
    INT_FATAL(call_,
              "unexpected syntax tree generated by arrayview elision");
  }
}

static bool exprSuitableForProtoSlice(Expr* e) {
  if (CallExpr* call = toCallExpr(e)) {
    if (SymExpr* callBase = toSymExpr(call->baseExpr)) {
      if (!isFnSymbol(callBase->symbol())) {
        return true;
      }
    }
  }
  return false;
}

