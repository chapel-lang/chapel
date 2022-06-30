/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/types/all-types.h"

#include "Resolver.h"

#include <cmath>
#include <cinttypes>

namespace {
  using namespace chpl;
  using namespace resolution;
  using namespace types;
  using namespace uast;

  struct Visitor {

    // Used to keep track of field state while walking the initializer.
    struct FieldInitState {
      int ordinalPos = -1;
      ID initPointId = ID();
      QualifiedType initPointQt;
      FieldInitState() = default;
    };

    enum Phase {
      PHASE_NEED_SUPER_INIT,  // Need super.init() calls on all paths.
      PHASE_NEED_COMPLETE,    // Need this.complete() calls on all paths.
      PHASE_COMPLETE          // Fully resolved type past this point.
    };
  
    Context* ctx_;
    Resolver& resolverVisitor_;
    const Function* fn_;
    const Type* recvType_;
    const ResolvedFields& rf_;

    std::map<ID, FieldInitState> fieldToInitState_;
    Phase phase_ = PHASE_NEED_SUPER_INIT;
    int currentFieldIdx_ = 0;
    ID idForCompleteCall_ = ID();
    bool isDescendingIntoAssignment_ = false;

    Visitor(Context* context, Resolver& visitor,
            const Function* fn,
            const Type* recvType,
            const ResolvedFields& rf)
      : ctx_(context),
        resolverVisitor_(visitor),
        fn_(fn),
        recvType_(recvType),
        rf_(rf) {
    }

    static Visitor build(Context* context, Resolver& visitor,
                         const Function* fn);

    void doSetupInitialFieldState(void);
    bool isCallToSuperInitRequired(void) const;
    void doDetermineInitialPhase(void);
    const Type* computeFinalReceiverTypeConsideringFields(void);
    const TypedFnSignature* run(void);
    void traverse(const AstNode* root);

    const AstNode* parentOf(const AstNode* node) const;
    bool isNameOfAnyField(UniqueString name) const;
    bool isValidFieldId(ID fieldId) const;
    FieldInitState* fieldStateFromId(ID fieldId);
    const FieldInitState* fieldStateFromId(ID fieldId) const;
    bool isMentionOfNodeInLhsOfAssign(const AstNode* node) const;
    ID idFromPossibleMentionOfField(const AstNode* node) const;
    bool isFieldInitialized(ID fieldId) const;

    bool handleCallToThisComplete(const FnCall* node);
    bool handleCallToSuperInit(const FnCall* node);
    bool handleCallToInit(const FnCall* node);
    bool handleInitializationOfField(const OpCall* node);
    bool handleUseOfField(const AstNode* node);

    // Visitors.
    inline bool enter(const AstNode* node) { return true; }
    inline void exit(const AstNode* node) {}
    bool enter(const OpCall* node);
    void exit(const OpCall* node);
    bool enter(const FnCall* node);
    void exit(const FnCall* node);
    void exit(const Identifier* node);
    void exit(const Dot* node);
  };

  static const Type* receiverTypeFromTfs(const TypedFnSignature* tfs) {
    auto ret = tfs->formalType(0).type();
    assert(ret);
    return ret;
  }

  static const CompositeType* unwrapCompType(const Type* type) {
    if (auto cls = type->toClassType()) {
      return cls->basicClassType();
    } else {
      auto ret = type->toCompositeType();
      return ret;
    }
  }

  static const CompositeType*
  receiverCompTypeFromTfs(const TypedFnSignature* tfs) {
    auto type = receiverTypeFromTfs(tfs);
    auto ret = unwrapCompType(type);
    assert(ret);
    return ret;
  }

  Visitor Visitor::build(Context* ctx, Resolver& visitor,
                         const Function* fn) {
    auto tfs = visitor.typedSignature;
    assert(tfs);
    auto recvType = receiverTypeFromTfs(tfs);
    const CompositeType* ct = receiverCompTypeFromTfs(tfs);
    auto& rf = fieldsForTypeDecl(ctx, ct, USE_DEFAULTS);
    auto ret = Visitor(ctx, visitor, fn, recvType, rf);
    return ret;
  }

  void Visitor::doSetupInitialFieldState(void) {
    fieldToInitState_.clear();

    for (int i = 0; i < rf_.numFields(); i++) {
      auto declPoint = rf_.fieldDeclId(i);
      FieldInitState state = { i, ID(), QualifiedType() };
      fieldToInitState_.insert({declPoint, std::move(state)});
    }
  }

  // TODO: More general function to fetch the parent type/fields later.
  bool Visitor::isCallToSuperInitRequired(void) const {
    if (auto cls = recvType_->toClassType())
      if (auto bct = cls->basicClassType())
        if (bct->parentClassType())
          return true;
    return false;
  }

  void Visitor::doDetermineInitialPhase(void) {
    phase_ = isCallToSuperInitRequired() ? PHASE_NEED_SUPER_INIT
                                         : PHASE_NEED_COMPLETE;
  }

  static bool isFastToComputeConcrete(QualifiedType qt) {
    auto ret = qt.genericity() != Type::CONCRETE;
    return ret;
  }

  //
  // TODO:
  // - Get the parent class type (the current basic class type if generic)
  // - Build up all the substitutions
  // - Construct either a BasicClassType or RecordType using subs/parent
  // - Additionally, for classes, wrap in BORROWED_NONNIL
  // - Return the final type (fully non-generic)
  // - TODO TODO: Super class stuff?
  //
  const Type* Visitor::computeFinalReceiverTypeConsideringFields(void) {
    if (!rf_.isGeneric()) return recvType_;

    auto ct = unwrapCompType(recvType_);
    auto& rfNoDefaults = fieldsForTypeDecl(ctx_, ct, IGNORE_DEFAULTS);
    CompositeType::SubstitutionsMap subs;

    for (int i = 0; i < rf_.numFields(); i++) {
      auto fieldName = rf_.fieldName(i);
      auto fieldQt = rf_.fieldType(i);
      auto fieldId = rf_.fieldDeclId(i);
      auto fieldDefaultQt = rfNoDefaults.fieldType(i);

      auto state = fieldStateFromId(fieldId);
      auto initPointQt = state->initPointQt;
      auto initPointId = state->initPointId;

      bool isGenericByDefault = isFastToComputeConcrete(fieldDefaultQt);
      bool isDefaultValuePresent = rf_.fieldHasDefaultValue(i);
      bool isInitPointPresent = !initPointId.isEmpty();

      if (!isGenericByDefault) continue;

      if (!isInitPointPresent && !isDefaultValuePresent) {
        ctx_->error(fn_, "can't omit initialization of field \"%s\", "
                         "no type or default value provided",
                         fieldName.c_str());
      }

      auto consideredQt = isInitPointPresent ? initPointQt : fieldQt;

      if (consideredQt.genericity() == Type::GENERIC) {
        assert(false && "Not handled yet!");
      }

      subs.insert({fieldId, consideredQt});
    }

    const Type* ret = nullptr;

    if (auto rec = recvType_->toRecordType()) {
      ret = RecordType::get(ctx_, rec->id(), rec->name(), rec, subs);
    } else if (auto cls = recvType_->toClassType()) {
      auto oldBasic = cls->basicClassType();
      auto basic = BasicClassType::get(ctx_, oldBasic->id(),
                                       oldBasic->name(),
                                       oldBasic->parentClassType(),
                                       oldBasic,
                                       subs);
      auto manager = AnyOwnedType::get(ctx_);
      auto dec = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
      ret = ClassType::get(ctx_, basic, manager, dec);
    } else {
      assert(false && "Not handled!");
    }

    assert(ret);

    return ret;
  }

  const TypedFnSignature* Visitor::run(void) {
    doSetupInitialFieldState();
    doDetermineInitialPhase();

    this->traverse(fn_->body());

    auto newRecvType = computeFinalReceiverTypeConsideringFields();
    assert(newRecvType);

    auto ret = resolverVisitor_.typedSignature; 

    // TODO: Compute the new TFS.
    if (newRecvType != recvType_) {
      assert(false && "Not handled yet!");
    }

    return ret;
  }

  // Stack like concept for this traversal, LIFO ordering on visitors?
  void Visitor::traverse(const AstNode* node) {
    switch (node->tag()) {
      #define AST_NODE(NAME) \
        case asttags::NAME: \
        { \
          const NAME* casted = (const NAME*) node; \
          bool go = this->enter(casted); \
          if (go) go &= resolverVisitor_.enter(casted); \
          if (go && !node->isLeaf()) { \
            for (const AstNode* child : node->children()) { \
              this->traverse(child); \
            } \
          } \
          resolverVisitor_.exit(casted); \
          this->exit(casted); \
          break; \
        }
      #define ERROR_CASE(NAME) \
        case asttags::NAME: \
        { \
          assert(false && "this code should never be run"); \
          break; \
        }

      #define AST_LEAF(NAME) AST_NODE(NAME)
      #define AST_BEGIN_SUBCLASSES(NAME) ERROR_CASE(START_##NAME)
      #define AST_END_SUBCLASSES(NAME) ERROR_CASE(END_##NAME)

      // Apply the above macros to uast-classes-list.h
      // to fill in the cases
      #include "chpl/uast/uast-classes-list.h"
      ERROR_CASE(NUM_AST_TAGS)
      ERROR_CASE(AST_TAG_UNKNOWN)

      #undef AST_NODE
      #undef AST_LEAF
      #undef AST_BEGIN_SUBCLASSES
      #undef AST_END_SUBCLASSES
      #undef ERROR_CASE
    }

    return;
  }

  const AstNode* Visitor::parentOf(const AstNode* node) const {
    assert(false && "Not handled!");
    return nullptr;
  }

  bool Visitor::isNameOfAnyField(UniqueString name) const {
    auto ret = isNameOfField(ctx_, name, recvType_);
    return ret;
  }

  bool Visitor::isValidFieldId(ID fieldId) const {
    if (fieldId.isEmpty()) return false;
    auto ret = fieldToInitState_.find(fieldId) != fieldToInitState_.end();
    return ret;
  }

  Visitor::FieldInitState* Visitor::fieldStateFromId(ID fieldId) {
    if (!isValidFieldId(fieldId)) return nullptr;
    return &(fieldToInitState_[fieldId]);
  }

  const Visitor::FieldInitState*
  Visitor::fieldStateFromId(ID fieldId) const {
    if (!isValidFieldId(fieldId)) return nullptr;
    return &(fieldToInitState_.find(fieldId)->second);
  }

  bool Visitor::isMentionOfNodeInLhsOfAssign(const AstNode* node) const {
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

  ID Visitor::idFromPossibleMentionOfField(const AstNode* node) const {
    UniqueString name;

    if (auto ident = node->toIdentifier()) {
      name = ident->name();
    } else if (auto dot = node->toDot()) {
      if (auto lhs = dot->receiver()->toIdentifier()) {
        if (lhs->name() == USTR("this")) {
          name = dot->field();
        }
      }
    }

    if (name.isEmpty() || !isNameOfAnyField(name)) return ID();

    auto& re = resolverVisitor_.byPostorder.byAst(node);
    auto ret = ID();

    if (!re.toId().isEmpty())
      if (isValidFieldId(re.toId()))
        ret = re.toId();

    return ret;
  }

  bool Visitor::isFieldInitialized(ID fieldId) const {
    assert(isValidFieldId(fieldId));
    auto ret = !fieldStateFromId(fieldId)->initPointId.isEmpty();
    return ret;
  }

  bool Visitor::handleCallToThisComplete(const FnCall* node) {
    if (!node->calledExpression()) return false;
    bool isCompleteCall = false;

    // TODO: Sanitize things like 'this.this.complete()' in post-parse.
    if (auto dot = node->calledExpression()->toDot())
      if (auto ident = dot->receiver()->toIdentifier())
        if (ident->name() == USTR("this"))
          if (dot->field() == USTR("complete"))
            isCompleteCall = true;

    if (!isCompleteCall) return false;

    // TODO: Better/more appropriate user facing error message for this?
    if (!idForCompleteCall_.isEmpty()) {
      assert(phase_ == PHASE_COMPLETE);
      ctx_->error(node, "use of this.complete() call in phase 2");
    } else {
      idForCompleteCall_ = node->id();
      phase_ = PHASE_COMPLETE;
    }

    return true;
  }

  bool Visitor::handleCallToSuperInit(const FnCall* node) {
    if (!isCallToSuperInitRequired()) assert(false && "Not handled!");
    assert(false && "Not handled!");
    return false;
  }

  bool Visitor::handleCallToInit(const FnCall* node) {
    assert(false && "Not handled!");
    return false;
  }

  bool Visitor::handleInitializationOfField(const OpCall* node) {
    if (node->op() != USTR("=")) return false;
    assert(node->numActuals() == 2);
    auto lhs = node->actual(0);
    auto rhs = node->actual(1);

    // TODO: Is 'field' or 'this.field' too strict of a pattern?
    auto fieldId = idFromPossibleMentionOfField(lhs);
    if (fieldId.isEmpty()) return false;

    auto state = fieldStateFromId(fieldId);
    assert(state);

    bool isAlreadyInitialized = !state->initPointId.isEmpty();
    bool isOutOfOrder = state->ordinalPos < currentFieldIdx_;

    // TODO: Anything to do if the opposite is true?
    if (!isAlreadyInitialized) {
      auto& reRhs = resolverVisitor_.byPostorder.byAst(rhs);
      state->initPointQt = reRhs.type();
      state->initPointId = node->id();
    }

    if (isOutOfOrder) {
      auto name = rf_.fieldName(state->ordinalPos);
      ctx_->error(node, "Field \"%s\" initialized out of order",
                        name.c_str());
    } else {
      currentFieldIdx_ = state->ordinalPos;
    }

    return true;
  }

  bool Visitor::handleUseOfField(const AstNode* node) {
    auto fieldId = idFromPossibleMentionOfField(node);
    if (fieldId.isEmpty()) return false;

    if (isFieldInitialized(fieldId)) return true;

    bool isValidPreInitMention = false;
    if (isDescendingIntoAssignment_)
      if (isMentionOfNodeInLhsOfAssign(node))
        isValidPreInitMention = true;

    if (!isValidPreInitMention) {
      assert(false && "Not handled!");
    }

    return true;
  }

  bool Visitor::enter(const OpCall* node) {
    if (node->op() == USTR("=")) isDescendingIntoAssignment_ = true;
    return true;
  }

  void Visitor::exit(const OpCall* node) {
    std::ignore = handleInitializationOfField(node);
    isDescendingIntoAssignment_ = false;
  }

  bool Visitor::enter(const FnCall* node) {
    (void) node;
    return true;
  }

  // TODO: Error about calls on receiver before 'this.complete()'.
  void Visitor::exit(const FnCall* node) {
    std::ignore = handleCallToThisComplete(node);
    std::ignore = handleCallToSuperInit(node);
    std::ignore = handleCallToInit(node);
  }

  void Visitor::exit(const Dot* node) {
    std::ignore = handleUseOfField(node);
  }

  void Visitor::exit(const Identifier* node) {
    std::ignore = handleUseOfField(node);
  }

} // end anonymous namespace

namespace chpl {
namespace resolution {

using namespace uast;

// Determine if we can work on the function before calling the builder.
static bool isFunctionToConsider(Context* ctx, Resolver& visitor,
                                 const Function* fn) {
  // TODO: Methodness
  // TODO: Can get receiver and see its definition point
  if (fn->name() != "init") return false;
  return true;
}

bool
handleResolvingInitFn(Context* context, Resolver& visitor,
                      const Function* fn,
                      const TypedFnSignature*& outFinalTfs) {
  if (!isFunctionToConsider(context, visitor, fn)) return false;
  auto v = Visitor::build(context, visitor, fn);
  outFinalTfs = v.run();
  return true;
}

} // end namespace resolution
} // end namespace chpl
