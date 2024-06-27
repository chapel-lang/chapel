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
#include "view.h"

static bool exprSuitableForProtoSlice(Expr* e, bool isLhs) {
  if (CallExpr* call = toCallExpr(e)) {
    if (SymExpr* callBase = toSymExpr(call->baseExpr)) {
      if (!isFnSymbol(callBase->symbol()) &&
          (!isLhs || !callBase->symbol()->isConstant())) {
        return true;
      }
    }
  }
  return false;
}


static CallExpr* generateCreateProtoSlice(CallExpr* call) {
  INT_ASSERT(call);

  SymExpr* base = toSymExpr(call->baseExpr);
  INT_ASSERT(base);

  const bool isConst = base->symbol()->isConstant();

  const char* factory = isConst ? "chpl__createConstProtoSlice" :
                                  "chpl__createProtoSlice";

  CallExpr* ret = new CallExpr(factory, base->copy());
  for_actuals(actual, call) {
    ret->insertAtTail(actual->copy());
  }

  return ret;
}

void arrayViewElision() {
  if (!fArrayViewElision) return;

  std::vector<CallExpr*> candidates;

  for_alive_in_Vec (CallExpr, call, gCallExprs) {
    if (FnSymbol* parentFn = toFnSymbol(call->parentSymbol)) {
      if (parentFn->hasFlag(FLAG_NO_ARRAY_VIEW_ELISION)) {
        continue;
      }
    }

    if (call->isNamed("=")) {
      if (exprSuitableForProtoSlice(call->get(1), /* isLhs */ true) &&
          exprSuitableForProtoSlice(call->get(2), /* isLhs */ false)) {
        //std::cout << call->stringLoc() << std::endl;
        //nprint_view(call);
        candidates.push_back(call);
      }
    }
  }


  for_vector(CallExpr, call, candidates) {
    SET_LINENO(call);

    CallExpr* lhs = toCallExpr(call->get(1));
    CallExpr* rhs = toCallExpr(call->get(2));

    CallExpr* lhsPSCall = generateCreateProtoSlice(lhs);
    CallExpr* rhsPSCall = generateCreateProtoSlice(rhs);

    // arrayview elision placeholder
    VarSymbol* placeholder = new VarSymbol("array_view_elision_flag", dtBool);
    placeholder->addFlag(FLAG_ARRAY_VIEW_ELISION_FLAG);

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

  supported_ = handleOneProtoSlice(newProtoSliceLhs_, /* isLhs */ true) &&
               handleOneProtoSlice(newProtoSliceRhs_, /* isLhs */ false);

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

Symbol* ProtoSliceAssignHelper::getFlagReplacement() {
  if (!supported_) {
    // we can't optimize
    return gFalse;
  }
  else if (fLocal) {
    // no need to check anything else, we can determine this statically
    return gTrue;
  }
  else {
    INT_ASSERT(lhsBase_);
    INT_ASSERT(rhsBase_);
    // we need to check if the arrays are on the same locale at run time
    CallExpr* localeCheck = new CallExpr("chpl__bothLocal", lhsBase_, rhsBase_);

    VarSymbol* flagTmp = newTemp(dtBool);
    DefExpr* flagDef = new DefExpr(flagTmp);

    condStmt_->insertBefore(flagDef);
    condStmt_->insertBefore(new CallExpr(PRIM_MOVE, flagTmp, localeCheck));

    resolveExpr(localeCheck);
    resolveExpr(flagDef);
    return flagTmp;
  }
}

void ProtoSliceAssignHelper::updateAndFoldConditional() {
  condStmt_->condExpr->replace(new SymExpr(getFlagReplacement()));
  condStmt_->foldConstantCondition(/*addEndOfStatement*/ false);
}

void ProtoSliceAssignHelper::report() {
  if (!fReportArrayViewElision) return;
  if (ModuleSymbol* mod = call_->getModule()) {
    // if there's no user module, getModule could return null
    if (mod->modTag != MOD_USER) return;
  }

  std::string isSupported = supported() ? "supported" : "not supported";
  std::string isDynamic = !fLocal ? " (dynamic locality check required)" : "";
  std::cout << "ArrayViewElision " << isSupported <<  isDynamic << " " <<
               call_->stringLoc() << std::endl;

  std::cout << "\t" << "lhsBaseType: " << lhsBaseType_ << std::endl;
  std::cout << "\t" << "lhsIndexingExprs: " << std::endl;
  for (auto typeName: lhsIndexExprTypes_) {
    std::cout << "\t\t" << typeName << std::endl;
  }

  std::cout << "\t" << "rhsBaseType: " << rhsBaseType_ << std::endl;
  std::cout << "\t" << "rhsIndexingExprs: " << std::endl;
  for (auto typeName: rhsIndexExprTypes_) {
    std::cout << "\t\t" << typeName << std::endl;
  }

  std::cout << std::endl;
}

bool ProtoSliceAssignHelper::handleOneProtoSlice(CallExpr* call, bool isLhs) {
  INT_ASSERT(call->isNamed("chpl__createProtoSlice") ||
             call->isNamed("chpl__createConstProtoSlice"));

  // stash some information while working on the call
  if (fReportArrayViewElision) {
    std::string& baseType = isLhs ? lhsBaseType_ : rhsBaseType_;
    std::vector<std::string>& indexExprTypes = isLhs ? lhsIndexExprTypes_ :
                                                       rhsIndexExprTypes_;

    bool baseRecorded = false;
    for_actuals (actual, call) {
      std::string typeName = std::string(actual->typeInfo()->symbol->name);
      if (!baseRecorded) {
        baseType = typeName;
        baseRecorded = true;
      }
      else {
        indexExprTypes.push_back(typeName);
      }
    }
  }

  CallExpr* typeCheck = new CallExpr("chpl__typesSupportArrayViewElision");
  for_actuals (actual, call) {
    //nprint_view(actual);
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

  // record the base symbol here for further checks
  Symbol*& baseToCapture = isLhs ? lhsBase_ : rhsBase_;
  baseToCapture = toSymExpr(call->get(1))->symbol();

  return ret;
}

// e must be the lhs or rhs of PRIM_ASSIGN_PROTO_SLICES
// returns the `chpl__createProtoSlice call
CallExpr* ProtoSliceAssignHelper::findOneProtoSliceCall(Expr* e) {
  SymExpr* symExpr = toSymExpr(e);
  INT_ASSERT(symExpr);

  Symbol* sym = symExpr->symbol();
  CallExpr* tmpMove = toCallExpr(sym->getSingleDef()->getStmtExpr());
  INT_ASSERT(tmpMove && tmpMove->isPrimitive(PRIM_MOVE));

  //nprint_view(lhsTmpMove);

  SymExpr* tmpSymExpr = toSymExpr(tmpMove->get(2));
  INT_ASSERT(tmpSymExpr);

  Symbol* tmpSym = tmpSymExpr->symbol();
  CallExpr* move = toCallExpr(tmpSym->getSingleDef()->getStmtExpr());
  INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));

  //nprint_view(lhsMove);

  return toCallExpr(move->get(2));
}

void ProtoSliceAssignHelper::findProtoSlices() {
  //nprint_view(call_);
  newProtoSliceLhs_ = findOneProtoSliceCall(call_->get(1));
  newProtoSliceRhs_ = findOneProtoSliceCall(call_->get(2));
}

void ProtoSliceAssignHelper::findCondStmt() {
  Expr* cur = call_;
  while (cur) {
    if (CondStmt* condStmt = toCondStmt(cur)) {
      if (SymExpr* condExpr = toSymExpr(condStmt->condExpr)) {
        if (condExpr->symbol()->hasFlag(FLAG_ARRAY_VIEW_ELISION_FLAG)) {
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

