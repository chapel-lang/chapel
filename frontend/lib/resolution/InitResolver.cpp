/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "InitResolver.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/types/all-types.h"
#include "Resolver.h"

#include <cmath>
#include <cinttypes>

//
// TODO: Error messages we need to capture here:
//
// - [ ] "can't omit initialization of field '%s', no type or default "
//       "value provided"
// - [ ] "can't initialize %s field '%s' with 'new' expression"
//       (for types and params)
// - [ ] "cannot take a reference to 'this' before 'init this'"
// - [ ] "cannot initialize a variable from 'this' before 'init this'"
// - [ ] "cannot pass 'this' to a function before calling super.init() "
//       "or this.init()"
// - [ ] "cannot pass a record to a function before 'init this'"
//
namespace chpl {
namespace resolution {

using namespace types;
using namespace uast;

static const Type* receiverTypeFromTfs(const TypedFnSignature* tfs) {
  auto ret = tfs->formalType(0).type();
  CHPL_ASSERT(ret);
  return ret;
}

owned<InitResolver>
InitResolver::create(Context* ctx, Resolver& visitor, const Function* fn) {
  auto tfs = visitor.typedSignature;
  CHPL_ASSERT(tfs);
  auto recvType = receiverTypeFromTfs(tfs);
  auto ret = toOwned(new InitResolver(ctx, visitor, fn, recvType));
  ret->doSetupInitialState();
  return ret;
}

owned<InitResolver> InitResolver::fork() {
  auto ret = toOwned(new InitResolver(ctx_, initResolver_, fn_, currentRecvType_));
  ret->copyState(*this);
  return ret;
}

// TODO: More general function to fetch the parent type/fields later.
bool InitResolver::isCallToSuperInitRequired(void) {
  if (auto cls = initialRecvType_->toClassType())
    if (auto bct = cls->basicClassType())
      if (bct->parentClassType())
        return true;
  return false;
}

bool InitResolver::setupFromType(const Type* type) {
  fieldToInitState_.clear();
  fieldIdsByOrdinal_.clear();

  if (auto ct = type->toClassType()) {
    if (auto bct = ct->basicClassType()) {
      superType_ = bct->parentClassType();
    }
  }

  auto ct = type->getCompositeType();
  auto& rf = fieldsForTypeDecl(ctx_, ct, DefaultsPolicy::USE_DEFAULTS);

  // If any of the newly-set fields are type or params, setting them
  // effectively means the receiver is a different type.
  bool anyAffectsResultingType = false;

  // Populate the fields with values from the type.
  for (int i = 0; i < rf.numFields(); i++) {
    auto id = rf.fieldDeclId(i);
    FieldInitState state;
    auto fieldQt = rf.fieldType(i);
    state = { i, ID(), fieldQt, rf.fieldName(i), false };
    fieldToInitState_.insert({id, std::move(state)});
    fieldIdsByOrdinal_.push_back(id);

    if (fieldQt.isType() || fieldQt.isParam()) {
      anyAffectsResultingType = true;
    }
  }

  return anyAffectsResultingType;
}

void InitResolver::doSetupInitialState(void) {
  // Determine the initial phase.
  phase_ = isCallToSuperInitRequired() ? PHASE_NEED_SUPER_INIT
                                       : PHASE_NEED_COMPLETE;

  std::ignore = setupFromType(initialRecvType_);
}

void InitResolver::markComplete() {
  phase_ = PHASE_COMPLETE;
  currentFieldIndex_ = fieldIdsByOrdinal_.size();
  for (auto& fieldPair : fieldToInitState_) {
    auto& state = fieldPair.second;
    state.isInitialized = true;
  }
}

void InitResolver::copyState(InitResolver& other) {
  phase_ = other.phase_;
  fieldToInitState_ = other.fieldToInitState_;
  fieldIdsByOrdinal_ = other.fieldIdsByOrdinal_;
  currentFieldIndex_ = other.currentFieldIndex_;
  thisCompleteIds_ = other.thisCompleteIds_;
  isDescendingIntoAssignment_ = other.isDescendingIntoAssignment_;
  currentRecvType_ = other.currentRecvType_;
  superType_ = other.superType_;
}

InitResolver::Phase InitResolver::getMaxPhase(Phase A, Phase B) {
  if (A == PHASE_COMPLETE || B == PHASE_COMPLETE) {
    return PHASE_COMPLETE;
  }
  if (A == PHASE_NEED_COMPLETE || B == PHASE_NEED_COMPLETE) {
    return PHASE_NEED_COMPLETE;
  }
  return PHASE_NEED_SUPER_INIT;
}

// TODO: store multiple init points for each field
// BHARSH TODO: I think we need something more like a list of
// field-initialization information, to account for multiple branches, and so
// that such information can be separate from the current state of what is or
// is not initialized.
void InitResolver::merge(owned<InitResolver>& A, owned<InitResolver>& B) {
  assert(A != nullptr);

  if (B == nullptr) {
    // TODO: Create information to indicate we need to generate code to match
    // the 'then' branch in the case of an absent 'else' branch.
    copyState(*A);
  } else {
    phase_ = getMaxPhase(A->phase_, B->phase_);
    assert(currentFieldIndex_ <= A->currentFieldIndex_);
    assert(currentFieldIndex_ <= B->currentFieldIndex_);

    // Collect any *new* occurrences of 'this.complete'
    for (auto i = thisCompleteIds_.size(); i < A->thisCompleteIds_.size(); i++) {
      thisCompleteIds_.push_back(A->thisCompleteIds_[i]);
    }
    for (auto i = thisCompleteIds_.size(); i < B->thisCompleteIds_.size(); i++) {
      thisCompleteIds_.push_back(B->thisCompleteIds_[i]);
    }

    auto curMax = std::max(A->currentFieldIndex_, B->currentFieldIndex_);

    // If one branch has proceeded further than the other, we need to
    // implicitly resolve fields in the branch that is behind.
    InitResolver& behind = A->currentFieldIndex_ < B->currentFieldIndex_ ? *A : *B;
    for (auto i = behind.currentFieldIndex_; i < curMax; i++) {
      auto& id = behind.fieldIdsByOrdinal_[i];
      std::ignore = behind.implicitlyResolveFieldType(id);
    }

    // Update field states
    for (auto i = currentFieldIndex_; i < curMax; i++) {
      auto& id = fieldIdsByOrdinal_[i];
      auto state = fieldStateFromId(id);
      auto stateA = A->fieldStateFromId(id);
      auto stateB = B->fieldStateFromId(id);

      assert(stateA->isInitialized && stateB->isInitialized);
      state->isInitialized = true;

      // Below, we issue an error if the resulting types do not compute to
      // be the same, so picking one is fine.
      state->qt = stateA->qt;

      // TODO: need to keep track of these in a different way so we can
      // preserve both init points if needed.
      if (!stateA->initPointId.isEmpty()) {
        state->initPointId = stateA->initPointId;
      } else if (!stateB->initPointId.isEmpty()) {
        state->initPointId = stateB->initPointId;
      }
    }

    currentFieldIndex_ = curMax;

    // Error if the computed types are different, otherwise update the receiver
    // type going forward.
    //
    // TODO: improve error message text
    // - point out different types, and where they are initialized
    if (A->currentRecvType_ != B->currentRecvType_) {
      ctx_->error(fn_, "Initializer must compute the same type in each branch");
    }

    // TODO: How should we allow the compiler to make progress if two branches
    // have computed different types?
    updateResolverVisibleReceiverType();
  }
}

bool InitResolver::isFinalReceiverStateValid(void) {
  auto ctInitial = initialRecvType_->getCompositeType();
  auto& rfInitial = fieldsForTypeDecl(ctx_, ctInitial,
                                      DefaultsPolicy::USE_DEFAULTS);
  bool ret = true;

  for (int i = 0; i < rfInitial.numFields(); i++) {
    auto id = rfInitial.fieldDeclId(i);
    auto state = fieldStateFromId(id);
    bool isDefaultValuePresent = rfInitial.fieldHasDefaultValue(i);
    auto qtInitial = rfInitial.fieldType(i);
    bool isInitiallyConcrete = qtInitial.genericity() == Type::CONCRETE;
    auto name = rfInitial.fieldName(i);

    if (isInitiallyConcrete) continue;

    if (!state->isInitialized && !isDefaultValuePresent) {
      ctx_->error(fn_, "can't omit initialization of field \"%s\", "
                       "no type or default value provided",
                       name.c_str());
      ret = false;
    }

    if (state->qt.genericity() == Type::GENERIC) {
      ctx_->error(ctInitial->id(),
                  "unable to instantiate generic type from initializer");
      ret = false;
    }
  }

  return ret;
}

static const Type* ctFromSubs(Context* context,
                              const Type* receiverType,
                              const BasicClassType* superType,
                              const CompositeType* compositeType,
                              const CompositeType::SubstitutionsMap& subs) {
  auto root = compositeType->instantiatedFromCompositeType() ?
              compositeType->instantiatedFromCompositeType() :
              compositeType;

  const Type* ret = nullptr;

  if (auto rec = receiverType->toRecordType()) {
    auto instantatiatedFrom = subs.size() == 0 ? nullptr :
                                                 root->toRecordType();
    ret = RecordType::get(context, rec->id(), rec->name(),
                          instantatiatedFrom,
                          subs);
  } else if (auto cls = receiverType->toClassType()) {
    auto oldBasic = cls->basicClassType();
    CHPL_ASSERT(oldBasic && "Not handled!");

    bool genericParent =
        superType && superType->instantiatedFromCompositeType() != nullptr;
    auto instantiatedFrom = (subs.size() == 0 && !genericParent)
                                  ? nullptr
                                  : root->toBasicClassType();

    auto basic = BasicClassType::get(context, oldBasic->id(),
                                     oldBasic->name(),
                                     superType,
                                     instantiatedFrom,
                                     subs);
    auto manager = AnyOwnedType::get(context);
    auto dec = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
    ret = ClassType::get(context, basic, manager, dec);
  } else {
    CHPL_ASSERT(false && "Not handled!");
  }

  return ret;
}

const Type* InitResolver::computeReceiverTypeConsideringState(void) {
  auto ctInitial = initialRecvType_->getCompositeType();

  // The non-default fields are used to determine if we need to create
  // substitutions. I.e., if a field is concrete even if we ignore defaults,
  // no reason to add a substitution.
  auto& rfNoDefaults = fieldsForTypeDecl(ctx_, ctInitial,
                                        DefaultsPolicy::IGNORE_DEFAULTS);
  auto& rfDefaults = fieldsForTypeDecl(ctx_, ctInitial,
                                       DefaultsPolicy::USE_DEFAULTS);
  CompositeType::SubstitutionsMap subs;

  bool genericParent =
      superType_ && superType_->instantiatedFromCompositeType() != nullptr;

  if (!rfNoDefaults.isGeneric()) {
    if (genericParent) {
      // Might need to update the parent of the initial receiver type after
      // a super.init call
      return ctFromSubs(ctx_, initialRecvType_, superType_, ctInitial, subs);
    } else {
      return currentRecvType_;
    }
  }

  auto isValidQtForSubstitutions = [this](const QualifiedType qt) {
    if (qt.isUnknown()) return false;
    return getTypeGenericity(this->ctx_, qt.type()) == Type::CONCRETE;
  };

  for (int i = 0; i < rfNoDefaults.numFields(); i++) {
    auto id = rfNoDefaults.fieldDeclId(i);
    auto state = fieldStateFromId(id);
    auto qtInitial = rfNoDefaults.fieldType(i);
    bool isInitiallyConcrete = qtInitial.genericity() == Type::CONCRETE;

    if (isInitiallyConcrete) continue;

    if (!isFieldSyntacticallyGeneric(ctx_, id)) continue;

    // TODO: Will need to relax this as we go.
    if (isValidQtForSubstitutions(state->qt)) {
      subs.insert({id, state->qt});
    } else {
      // generic field without a substitution form the initializer.
      // Perhaps we can use a default?

      // First, try a default from the original (base) type.
      QualifiedType qtForSub = rfDefaults.fieldType(i);

      if (!isValidQtForSubstitutions(qtForSub) && !subs.empty()) {
        // There's no default value in the base type. But we already have
        // substitutions from previous fields. If the composite type is
        // dependently typed, we might be able to compute defaults that
        // depend on these prior substitutions.
        auto ctIntermediate = ctFromSubs(ctx_, initialRecvType_, superType_, ctInitial, subs);
        auto& rfIntermediate = fieldsForTypeDecl(ctx_, ctIntermediate->getCompositeType(),
                                                 DefaultsPolicy::USE_DEFAULTS);

        qtForSub = rfIntermediate.fieldType(i);
      }

      if (isValidQtForSubstitutions(qtForSub)) {
        subs.insert({id, qtForSub});
      }
    }
  }

  if (subs.size() != 0 || genericParent) {
    const Type* ret = ctFromSubs(ctx_, initialRecvType_, superType_, ctInitial, subs);
    CHPL_ASSERT(ret);
    return ret;
  } else {
    return currentRecvType_;
  }
}

QualifiedType::Kind InitResolver::determineReceiverIntent(void) {
  if (initialRecvType_->isClassType()) {
    return QualifiedType::CONST_IN;
  } else {
    CHPL_ASSERT(initialRecvType_->isRecordType());
    return QualifiedType::REF;
  }
}

const TypedFnSignature*
InitResolver::computeTypedSignature(const Type* newRecvType) {
  const TypedFnSignature* ret = initResolver_.typedSignature;

  if (newRecvType == currentRecvType_) return ret;

  auto tfs = initResolver_.typedSignature;
  auto ufs = tfs->untyped();
  std::vector<QualifiedType> formalTypes;
  Bitmap formalsInstantiated;

  formalsInstantiated.resize(ufs->numFormals());

  for (int i = 0; i < tfs->numFormals(); i++) {
    if (i == 0) {
      auto qt = QualifiedType(determineReceiverIntent(), newRecvType);
      formalTypes.push_back(std::move(qt));
      formalsInstantiated.setBit(i, true);
    } else {
      formalTypes.push_back(tfs->formalType(i));
      formalsInstantiated.setBit(i, tfs->formalIsInstantiated(i));
    }
  }

  ret = TypedFnSignature::get(ctx_, ufs, formalTypes,
                              tfs->whereClauseResult(),
                              /* needsInstantiation */ false,
                              tfs->instantiatedFrom(),
                              tfs->parentFn(),
                              formalsInstantiated);
  return ret;
}

// TODO: Identify cases where we don't need to do anything.
const TypedFnSignature* InitResolver::finalize(void) {
  if (fn_ == nullptr) CHPL_ASSERT(false && "Not handled yet!");

  auto ret = initResolver_.typedSignature;

  if (phase_ < PHASE_COMPLETE) {
    int start = currentFieldIndex_;
    int stop = fieldIdsByOrdinal_.size();
    for (int i = start; i < stop; i++) {
      auto id = fieldIdsByOrdinal_[i];
      bool handled = implicitlyResolveFieldType(id);
      if (!handled) {
        CHPL_ASSERT(false && "Not handled yet!");
      }
    }
  }

  if (isFinalReceiverStateValid()) {
    auto newRecvType = computeReceiverTypeConsideringState();
    ret = computeTypedSignature(newRecvType);
  }

  return ret;
}

void InitResolver::updateResolverVisibleReceiverType(void) {
  auto updated = computeReceiverTypeConsideringState();
  if (updated != currentRecvType_) {
    auto tfs = computeTypedSignature(updated);
    initResolver_.typedSignature = tfs;
    currentRecvType_ = updated;
  }
}

bool InitResolver::implicitlyResolveFieldType(ID id) {
  auto state = fieldStateFromId(id);
  if (!state || !state->initPointId.isEmpty()) return false;

  auto ct = currentRecvType_->getCompositeType();
  auto& rf = resolveFieldDecl(ctx_, ct, id, DefaultsPolicy::USE_DEFAULTS);
  for (int i = 0; i < rf.numFields(); i++) {
    auto id = rf.fieldDeclId(i);
    auto state = fieldStateFromId(id);
    CHPL_ASSERT(state);
    CHPL_ASSERT(state->qt.kind() == rf.fieldType(i).kind());
    state->qt = rf.fieldType(i);
    state->isInitialized = true;
  }

  return true;
}

const AstNode* InitResolver::parentOf(const AstNode* node) {
  auto parentId = parsing::idToParentId(ctx_, node->id());
  auto ret = parsing::idToAst(ctx_, parentId);
  return ret;
}

InitResolver::FieldInitState* InitResolver::fieldStateFromId(ID id) {
  auto search = fieldToInitState_.find(id);
  if (search != fieldToInitState_.end()) return &search->second;
  return nullptr;
}

InitResolver::FieldInitState* InitResolver::fieldStateFromIndex(int idx) {
  int numFields = fieldIdsByOrdinal_.size();
  if (idx < 0 || idx >= numFields) return nullptr;
  auto id = fieldIdsByOrdinal_[idx];
  auto ret = fieldStateFromId(id);
  return ret;
}

bool InitResolver::isMentionOfNodeInLhsOfAssign(const AstNode* node) {
  const AstNode* parent = parentOf(node);
  const AstNode* prior = node;

  for (; parent != nullptr; parent = parentOf(parent)) {
    if (auto opCall = parent->toOpCall())
      if (opCall->op() == USTR("="))
        if (prior && opCall->actual(0) == prior)
          return true;
    prior = parent;
  }

  return false;
}

ID InitResolver::fieldIdFromName(UniqueString name) {
  if (!isNameOfField(ctx_, name, initialRecvType_)) return ID();
  // TODO: Need to replace this as we continue to build it up?
  auto ct = initialRecvType_->getCompositeType();
  auto ret = parsing::fieldIdWithName(ctx_, ct->id(), name);
  return ret;
}

ID InitResolver::fieldIdFromPossibleMentionOfField(const AstNode* node) {
  UniqueString name;

  // TODO: Just remove this and check the set ID in all cases?
  if (auto ident = node->toIdentifier()) {
    name = ident->name();
  } else if (auto dot = node->toDot()) {
    if (auto lhs = dot->receiver()->toIdentifier()) {
      if (lhs->name() == USTR("this")) name = dot->field();
    }
  }

  if (name.isEmpty()) return ID();

  auto ret = ID();
  auto fieldId = fieldIdFromName(name);
  auto& re = initResolver_.byPostorder.byAst(node);

  if (fieldId.isEmpty()) return ret;

  // TODO: Handle case where "this.foo" resolve to a paren-less method
  auto id = re.toId();
  if (!id.isEmpty() && parsing::idIsField(ctx_, id)) return id;

  return ret;
}

bool InitResolver::isFieldInitialized(ID fieldId) {
  auto state = fieldStateFromId(fieldId);
  auto ret = state && state->isInitialized;
  return ret;
}

void InitResolver::handleInitMarker(const uast::AstNode* node) {
  // TODO: Better/more appropriate user facing error message for this?
  if (thisCompleteIds_.size() > 0) {
    CHPL_ASSERT(phase_ == PHASE_COMPLETE);
    CHPL_REPORT(ctx_, PhaseTwoInitMarker, node, thisCompleteIds_);
  } else {
    thisCompleteIds_.push_back(node->id());
    markComplete();
  }
}

bool InitResolver::handleCallToThisComplete(const FnCall* node) {
  if (!node->calledExpression()) return false;
  bool isCompleteCall = false;

  // TODO: Sanitize things like 'this.this.complete()' in post-parse.
  if (auto dot = node->calledExpression()->toDot())
    if (auto ident = dot->receiver()->toIdentifier())
      if (ident->name() == USTR("this"))
        if (dot->field() == USTR("complete"))
          isCompleteCall = true;

  if (!isCompleteCall) return false;

  handleInitMarker(node);

  return true;
}

bool InitResolver::handleCallToSuperInit(const FnCall* node,
                                         const CallResolutionResult* c) {
  if (auto dot = node->calledExpression()->toDot()) {
    if (auto ident = dot->receiver()->toIdentifier()) {
      if (ident->name() == "super" && dot->field() == "init") {
        auto& msc = c->mostSpecific().only();
        auto superThis = msc.formalActualMap().byFormalIdx(0).formalType();

        const BasicClassType* superType = nullptr;
        if (auto ct = superThis.type()->toClassType()) {
          superType = ct->basicClassType();
        } else {
          superType = superThis.type()->toBasicClassType();
        }

        this->superType_ = superType;

        // Only update the current receiver if the parent was generic.
        if (superType->instantiatedFromCompositeType() != nullptr) {
          updateResolverVisibleReceiverType();
        }

        phase_ = PHASE_NEED_COMPLETE;

        return true;
      }
    }
  }

  return false;
}

bool InitResolver::applyResolvedInitCallToState(const FnCall* node,
                                                const CallResolutionResult* c) {
  if (!c || !c->mostSpecific().only()) return false;

  auto& only = c->mostSpecific().only();
  auto fn = only.fn();

  CHPL_ASSERT(fn->formalName(0) == USTR("this"));
  auto receiverType = fn->formalType(0).type();
  auto receiverCompType = receiverType->getCompositeType();
  if (receiverCompType->instantiatedFromCompositeType()) {
    receiverCompType = receiverCompType->instantiatedFromCompositeType();
  }

  auto initialCompType = initialRecvType_->getCompositeType();
  if (initialCompType->instantiatedFromCompositeType()) {
    initialCompType = initialCompType->instantiatedFromCompositeType();
  }

  CHPL_ASSERT(receiverCompType == initialCompType);
  if (setupFromType(receiverType)) {
    updateResolverVisibleReceiverType();
  }

  markComplete();
  return true;
}

bool InitResolver::handleCallToInit(const FnCall* node,
                                    const CallResolutionResult* c) {
  auto calledExpr = node->calledExpression();
  if (!calledExpr) return false;

  if (auto calledIdent = calledExpr->toIdentifier()) {
    if (calledIdent->name() != USTR("init")) return false;
  } else if (auto calledDot = calledExpr->toDot()) {
    if (calledDot->field() != USTR("init")) return false;

    auto receiver = calledDot->receiver();
    if (!receiver->isIdentifier() ||
        receiver->toIdentifier()->name() != USTR("this")) {
      return false;
    }
  }

  // It's a call to 'this.init', which means any initialized fields are
  // initialized erroneously.
  if (currentFieldIndex_ != 0) {
    std::vector<std::pair<const VarLikeDecl*, ID>> initializationPoints;
    for (auto& fieldPair : fieldToInitState_) {
      auto& state = fieldPair.second;
      if (!state.isInitialized || state.initPointId.isEmpty()) continue;

      auto variable = parsing::idToAst(ctx_, fieldPair.first)->toVarLikeDecl();
      CHPL_ASSERT(variable != nullptr);

      initializationPoints.emplace_back(variable, state.initPointId);
    }
    CHPL_REPORT(ctx_, AssignFieldBeforeInit, node, initializationPoints);
  }

  if (applyResolvedInitCallToState(node, c)) return true;

  // Something went wrong when resolving the 'init' call: we didn't
  // try to resolve it, or we tried and failed, or we found a nonsensical
  // candidate.
  //
  // By the rules of initializers, after this point variables will
  // have been initialized, so mark them as such.
  markComplete();
  return true;
}

void InitResolver::doDetectPossibleAssignmentToField(const OpCall* node) {
  if (node->op() == USTR("=")) isDescendingIntoAssignment_ = true;
}

static void checkInsideBadTag(Context* context,
                              Resolver& resolver, const AstNode* node) {
  if (resolver.isInsideTag(AstTag::START_Loop)) {
    context->error(node, "cannot initialize fields inside of loops");
  }
  if (resolver.isInsideTag(AstTag::Begin)) {
    context->error(node, "cannot initialize fields inside begin statements");
  }
  if (resolver.isInsideTag(AstTag::Cobegin)) {
    context->error(node, "cannot initialize fields inside cobegin statements");
  }
}

bool InitResolver::handleAssignmentToField(const OpCall* node) {
  if (phase_ == PHASE_COMPLETE) return false;
  if (node->op() != USTR("=")) return false;
  CHPL_ASSERT(node->numActuals() == 2);
  auto lhs = node->actual(0);
  auto rhs = node->actual(1);

  // TODO: Is 'field' or 'this.field' too strict of a pattern?
  auto fieldId = fieldIdFromPossibleMentionOfField(lhs);
  if (fieldId.isEmpty()) return false;

  auto state = fieldStateFromId(fieldId);
  CHPL_ASSERT(state);

  bool isAlreadyInitialized = !state->initPointId.isEmpty();
  bool isOutOfOrder = state->ordinalPos < currentFieldIndex_;

  checkInsideBadTag(ctx_, initResolver_, node);

  if (!isOutOfOrder) {
    // Implicitly initialize any fields between the current index and this.
    int old = currentFieldIndex_;
    currentFieldIndex_ = state->ordinalPos + 1;
    for (int i = old; i < state->ordinalPos; i++) {
        auto id = fieldIdsByOrdinal_[i];

        // TODO: Anything to do if this doesn't hold?
        std::ignore = implicitlyResolveFieldType(id);
    }

    // TODO: Anything to do if the opposite is true?
    if (!isAlreadyInitialized) {
      auto& reRhs = initResolver_.byPostorder.byAst(rhs);
      state->qt = reRhs.type();
      state->initPointId = node->id();
      state->isInitialized = true;

      // We could probably get away with running this less, but it's easier
      // to just attempt updating the receiver type for each field even if the
      // field doesn't contribute to the receiver type.
      updateResolverVisibleReceiverType();

      auto lhsKind = state->qt.kind();
      if (lhsKind != QualifiedType::TYPE && lhsKind != QualifiedType::PARAM) {
        // Regardless of the field's intent, it is mutable in this expression.
        lhsKind = QualifiedType::REF;
      }
      auto lhsType = QualifiedType(lhsKind, state->qt.type(), state->qt.param());
      initResolver_.byPostorder.byAst(lhs).setType(lhsType);

    } else {
      CHPL_ASSERT(0 == "Not handled yet!");
    }
  } else if (!isAlreadyInitialized) {
    auto name = state->name;
    ctx_->error(node, "Field \"%s\" initialized out of order",
                      name.c_str());
  }

  isDescendingIntoAssignment_ = false;

  return true;
}

bool InitResolver::handleResolvingCall(const Call* node) {
  auto ret = false;

  if (auto fnCall = node->toFnCall()) {
    ret |= handleCallToThisComplete(fnCall);
  }

  if (auto opCall = node->toOpCall()) {
    ret |= handleAssignmentToField(opCall);
  }

  return ret;
}

bool InitResolver::handleResolvedCall(const Call* node,
                                      const CallResolutionResult* c) {
  auto ret = false;

  if (auto fnCall = node->toFnCall()) {
    ret |= handleCallToInit(fnCall, c);
    ret |= handleCallToSuperInit(fnCall, c);
  }

  return ret;
}

bool InitResolver::handleInitStatement(const uast::Init* node) {
  // current parser rules require this to always be this, but maybe someday
  // they won't.
  if (node->target()->name() != "this") return false;

  handleInitMarker(node);

  return true;
}

bool InitResolver::handleUseOfField(const AstNode* node) {
  auto id = fieldIdFromPossibleMentionOfField(node);
  if (id.isEmpty()) return false;

  if (isFieldInitialized(id)) return false;

  auto state = fieldStateFromId(id);
  bool isValidPreInitMention = false;

  if (isDescendingIntoAssignment_)
    if (isMentionOfNodeInLhsOfAssign(node))
      isValidPreInitMention = true;

  if (!isValidPreInitMention) {
    ctx_->error(node, "'%s' is used before it is initialized",
                      state->name.c_str());
    return true;
  }

  return false;
}

ID InitResolver::solveNameConflictByIgnoringField(const NameVec& vec) {
  if (vec.size() != 2) return ID();
  if (vec[0].numIds() > 1 || vec[1].numIds() > 1) return ID();
  auto one = vec[0].firstId();
  auto two = vec[1].firstId();
  CHPL_ASSERT(one != two);
  if (!parsing::idIsField(ctx_, one) &&
      !parsing::idIsField(ctx_, two)) return ID();
  auto ret = parsing::idIsField(ctx_, one) ? two : one;
  CHPL_ASSERT(!parsing::idIsField(ctx_, ret));
  return ret;
}

bool InitResolver::handleResolvingFieldAccess(const Identifier* node) {
  auto parenlessInfo = Resolver::ParenlessOverloadInfo();
  auto scope = initResolver_.methodReceiverScopes();
  auto vec = initResolver_.lookupIdentifier(node, scope, parenlessInfo);

  // Handle and exit early if there were no ambiguities.
  if (vec.size() == 1 && vec[0].numIds() == 1) {
    auto& id = vec[0].firstId();
    if (parsing::idIsField(ctx_, id)) {
      auto state = fieldStateFromId(id);
      auto qt = state->qt;
      auto& re = initResolver_.byPostorder.byAst(node);
      re.setToId(id);
      re.setType(qt);
      return true;
    }
  }

  // If there are two names and one is a field, get the other name.
  auto id = solveNameConflictByIgnoringField(vec);
  if (!id.isEmpty()) {
    CHPL_ASSERT(!parsing::idIsField(ctx_, id));
    const bool localGenericToUnknown = true;
    auto qt = initResolver_.typeForId(id, localGenericToUnknown);
    auto& re = initResolver_.byPostorder.byAst(node);
    re.setToId(id);
    re.setType(qt);
    return true;
  }

  return false;
}

bool InitResolver::handleResolvingFieldAccess(const Dot* node) {
  if (auto ident = node->receiver()->toIdentifier()) {

    // TODO: Check parent for more context, and 'this.this'.
    if (ident->name() == USTR("this")) {
      auto name = node->field();
      auto id = fieldIdFromName(name);
      if (!id.isEmpty()) {
        auto state = fieldStateFromId(id);
        CHPL_ASSERT(state);
        auto qt = state->qt;
        auto& re = initResolver_.byPostorder.byAst(node);
        re.setToId(id);
        re.setType(qt);
        return true;
      }
    } else {
      // Otherwise, proceed normally.
      return false;
    }
  }

  return false;
}

void InitResolver::checkEarlyReturn(const Return* ret) {
  // Something like:
  //   this.z = 5;
  //   return;
  // where 'z' is the last field shouldn't be an error.
  if (phase_ != PHASE_COMPLETE &&
      (size_t)currentFieldIndex_ < fieldIdsByOrdinal_.size()) {
    ctx_->error(ret, "cannot return from initializer before initialization is complete");
  }
}

} // end namespace resolution
} // end namespace chpl
