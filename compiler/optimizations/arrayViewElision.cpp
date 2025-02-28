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

#include <vector>

#include "arrayViewElision.h"
#include "global-ast-vecs.h"
#include "passes.h"
#include "resolution.h"

// Array View Elision (AVE) aims to optimize assignments involving array views.
// Currently, this is limited to:
//
//   slice = slice, and
//   rank-change = rank-change
//
// This is mostly out of abundance of caution and could be extended to
// assignments involving arrays, too
//
// The gist of the implementation is based on eliding array views from
// operations such as the ones above. Note that this implies that the following
// cannot be covered:
//
//   ref slice = A[1..5];
//   slice = A[6..10];
//
// As determining whether `slice` can be dropped is more complicated than I
// could bite at the moment. So, both sides of the assignments must be
// array-view generating expressions for this optimization to fire.
//
// There are two parts of this optimization:
//
// 1. Pre-normalize  (ArrayViewElisionTransformer is the type doing this)
//
// Given a statement like
//
//   A[x] = B[y];
//
// we generate
//
//   param array_view_elision: bool;  // will be replaced during resolution
//   if (array_view_elision) {
//     var protoSlice1 = chpl__createProtoSlice(A, x);
//     var protoSlice2 = chpl__createProtoSlice(B, y);
//
//     __primitive(PRIM_PROTO_SLICE_ASSIGN, protoSlice1, protoSlice2);
//   }
//   else {
//     A[x] = B[y];
//   }
//
// Here the "protoSlice" has type `chpl__protoSlice`. See
// modules/internal/ChapelArrayViewElision.chpl for the details of that type.
// The main purpose of that type is to represent the expression that would
// create an array view. But avoid doing that.
//
// 2. During prefold  (ArrayViewElisionPrefolder is the type doing this)
//
// Operation revolves around `PRIM_PROTO_SLICE_ASSIGN`. The
// ArrayViewElisionPrefolder is in charge of finding the other relevant AST (the
// CondStmt, the protoSlice temps etc) and transforming the conditional.
//
// Statically, chpl__ave_exprCanBeProtoSlice is called on both protoSlices to
// make sure that the module code is OK with creating protoSlices out of those
// expressions. We also check whether two protoSlices can be assigned to one
// another. This is done by chpl__ave_protoSlicesSupportAssignment. If `fLocal`,
// that's sufficient. Calls to that function are inserted, resolved, the result
// is collected, and finally the calls are removed. At that point, we drop the
// `array_view_elision` flag completely, and replace it with `true` or `false`,
// after which the conditional statement is constant-folded.
//
// If not `fLocal`, we also call `chpl__bothLocal` an replace the flag with the
// result of that. Note that this is a dynamic check, meaning that the
// conditional will not be removed.
//
// This optimization is on-by-default. It can be controlled with
// `--[no-]array-view-elision`. Additionally, there's also
// `--report-array-view-elision` flag to enable some output during compilation
// to help with understanding what's optimized and what's not.

static const char* createConstProtoSlice = "chpl__createConstProtoSlice";
static const char* createProtoSlice = "chpl__createProtoSlice";

ArrayViewElisionTransformer::ArrayViewElisionTransformer(CallExpr* origCall):
    origCall_(origCall) {

  origLhs_ = toCallExpr(origCall_->get(1));
  origRhs_ = toCallExpr(origCall_->get(2));

  // one side is not a call
  // TODO this should be relaxed for `array=view` or vice versa
  if (origLhs_ == nullptr || origRhs_ == nullptr) {
    candidate_ = false;
    return;
  }

  // calls have different number of actuals
  // TODO this should be relaxed for `slice=rank-change` or vice versa
  if (origLhs_->numActuals() != origRhs_->numActuals()) {
    candidate_ = false;
    return;
  }

  // further analysis per call
  if ( !(exprSuitableForProtoSlice(origLhs_, /*isLhs*/ true) &&
         exprSuitableForProtoSlice(origRhs_, /*isLhs*/ false)) ) {
    candidate_ = false;
    return;
  }

  candidate_ = true;
}

void ArrayViewElisionTransformer::transform() {
  INT_ASSERT(candidate_);

  SET_LINENO(origCall_);

  CallExpr* lhsPSCall = genCreateProtoSlice(origLhs_);
  CallExpr* rhsPSCall = genCreateProtoSlice(origRhs_);

  // arrayview elision placeholder
  VarSymbol* placeholder = new VarSymbol("array_view_elision_flag", dtBool);
  placeholder->addFlag(FLAG_ARRAY_VIEW_ELISION_FLAG);

  origCall_->insertBefore(new DefExpr(placeholder, gFalse));

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

  origCall_->insertBefore(cond);
  elseBlock->insertAtTail(origCall_->remove());
}



bool ArrayViewElisionTransformer::exprSuitableForProtoSlice(CallExpr* call,
                                                            bool isLhs) {
  if (SymExpr* callBase = toSymExpr(call->baseExpr)) {
    if (!isFnSymbol(callBase->symbol()) &&
        (!isLhs || !callBase->symbol()->isConstant())) {
      // we avoid touching const lhs, otherwise this optimization causes the
      // const checking error to point at the internal code rather than users.
      return true;
    }
  }
  return false;
}

CallExpr* ArrayViewElisionTransformer::genCreateProtoSlice(CallExpr* call) {
  INT_ASSERT(call);

  SymExpr* base = toSymExpr(call->baseExpr);
  INT_ASSERT(base);

  const bool isConst = base->symbol()->isConstant();

  const char* factory = isConst ? createConstProtoSlice : createProtoSlice;

  CallExpr* ret = new CallExpr(factory, base->copy());
  for_actuals(actual, call) {
    ret->insertAtTail(actual->copy());
  }

  return ret;
}


void arrayViewElision() {
  if (!fArrayViewElision) return;

  for_alive_in_Vec (CallExpr, call, gCallExprs) {
    if (FnSymbol* parentFn = toFnSymbol(call->parentSymbol)) {
      if (parentFn->hasFlag(FLAG_NO_ARRAY_VIEW_ELISION)) {
        continue;
      }
    }

    if (call->getModule()->modTag == MOD_USER) {
    if (call->isNamed("=")) {
      ArrayViewElisionTransformer transformer(call);

      if (transformer.candidate()) {
        transformer.transform();
      }
    }
    }
  }
}

ArrayViewElisionPrefolder::ArrayViewElisionPrefolder(CallExpr* call):
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

  supported_ = handleOneProtoSlice(/* isLhs */ true) &&
               handleOneProtoSlice(/* isLhs */ false) &&
               canAssign();

  findCondStmt();
  INT_ASSERT(condStmt_);
}

ArrayViewElisionPrefolder::~ArrayViewElisionPrefolder() {
  staticCheckBlock_->remove();
  tmpCondFlag_->getSingleDef()->getStmtExpr()->remove();
  tmpCondFlag_->defPoint->remove();
}

CallExpr* ArrayViewElisionPrefolder::getReplacement() {
  return new CallExpr("=", call_->get(1)->copy(), call_->get(2)->copy());
}

Symbol* ArrayViewElisionPrefolder::getFlagReplacement() {
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

void ArrayViewElisionPrefolder::updateAndFoldConditional() {
  condStmt_->condExpr->replace(new SymExpr(getFlagReplacement()));
  condStmt_->foldConstantCondition(/*addEndOfStatement*/ false);
}

void ArrayViewElisionPrefolder::report() {
  if (!fReportArrayViewElision) return;
  if (ModuleSymbol* mod = call_->getModule()) {
    // if there's no user module, getModule could return null
    if (mod->modTag != MOD_USER) return;
  }

  std::string isSupported = supported() ? "supported" : "not supported";
  std::string isDynamic = (supported() && !fLocal) ?
                            " (dynamic locality check required)" :
                            "";
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

bool ArrayViewElisionPrefolder::handleOneProtoSlice(bool isLhs) {
  CallExpr* call = isLhs ? newProtoSliceLhs_ : newProtoSliceRhs_;

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

  CallExpr* typeCheck = new CallExpr("chpl__ave_exprCanBeProtoSlice");

  // add the isConst flag for a workaround
  //   See: https://github.com/chapel-lang/chapel/issues/26626
  typeCheck->insertAtTail(call->isNamed(createConstProtoSlice) ? gTrue:gFalse);

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

  // record the base symbol here for further checks
  Symbol*& baseToCapture = isLhs ? lhsBase_ : rhsBase_;
  baseToCapture = toSymExpr(call->get(1))->symbol();

  return ret;
}

// make sure we don't try to optimize A[3, 1..3] = B[1..3, 3].
// because the current optimization works by strength-reducing that to use
// slices : A[3..3, 1..3] = B[1..3, 3..3]. That's not a valid assignment as
// opposed to what the user wrote.
bool ArrayViewElisionPrefolder::canAssign() const {
  CallExpr* canAssign = new CallExpr("chpl__ave_protoSlicesSupportAssignment",
                                     call_->get(1)->copy(),
                                     call_->get(2)->copy());

  VarSymbol* tmp = newTemp("call_tmp", dtBool);
  DefExpr* flagDef = new DefExpr(tmp, canAssign);

  staticCheckBlock_->insertAtTail(flagDef);

  resolveExpr(canAssign);
  resolveExpr(flagDef);

  bool ret = (toSymExpr(flagDef->init)->symbol() == gTrue);

  flagDef->remove();

  return ret;
}

// e must be the lhs or rhs of PRIM_ASSIGN_PROTO_SLICES
// returns the `chpl__createProtoSlice call
CallExpr* ArrayViewElisionPrefolder::findOneProtoSliceCall(Expr* e) {
  SymExpr* symExpr = toSymExpr(e);
  INT_ASSERT(symExpr);

  Symbol* sym = symExpr->symbol();
  CallExpr* tmpMove = toCallExpr(sym->getSingleDef()->getStmtExpr());
  INT_ASSERT(tmpMove && tmpMove->isPrimitive(PRIM_MOVE));

  SymExpr* tmpSymExpr = toSymExpr(tmpMove->get(2));
  INT_ASSERT(tmpSymExpr);

  Symbol* tmpSym = tmpSymExpr->symbol();
  CallExpr* move = toCallExpr(tmpSym->getSingleDef()->getStmtExpr());
  INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));

  return toCallExpr(move->get(2));
}

void ArrayViewElisionPrefolder::findProtoSlices() {
  //nprint_view(call_);
  newProtoSliceLhs_ = findOneProtoSliceCall(call_->get(1));
  newProtoSliceRhs_ = findOneProtoSliceCall(call_->get(2));
}

void ArrayViewElisionPrefolder::findCondStmt() {
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

