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

#include "chpl/parsing/parsing-queries.h"
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
      QualifiedType qt;
      UniqueString name;
      bool isInitialized;
      FieldInitState() = default;
    };

    enum Phase {
      PHASE_NEED_SUPER_INIT,  // Need super.init() calls on all paths.
      PHASE_NEED_COMPLETE,    // Need this.complete() calls on all paths.
      PHASE_COMPLETE          // Fully resolved type past this point.
    };
  
    Context* ctx_;
    Resolver& initResolver_;
    const Function* fn_;
    const Type* initialRecvType_;

    std::map<ID, FieldInitState> fieldToInitState_;
    std::vector<ID> fieldIdsByOrdinal_;
    Phase phase_ = PHASE_NEED_SUPER_INIT;
    int currentFieldIndex_ = 0;
    ID idForCompleteCall_ = ID();
    bool isDescendingIntoAssignment_ = false;
    const Type* currentRecvType_ = initialRecvType_;

    Visitor(Context* ctx, Resolver& visitor,
            const Function* fn,
            const Type* recvType)
      : ctx_(ctx),
        initResolver_(visitor),
        fn_(fn),
        initialRecvType_(recvType) {
    }

    static Visitor create(Context* context, Resolver& visitor,
                          const Function* fn);

    bool isCallToSuperInitRequired(void);
    void doSetupInitialState(void);

    bool isFinalReceiverStateValid(void);
    const Type* computeReceiverTypeConsideringState(void);

    QualifiedType::Kind determineReceiverIntent(void);
    const TypedFnSignature* computeTypedSignature(const Type* newRecvType);
    const TypedFnSignature* run(void);
    void traverse(const AstNode* root);
    void updateResolverVisibleReceiverType(void);
    bool implicitlyResolveFieldType(ID id);

    const AstNode* parentOf(const AstNode* node);
    FieldInitState* fieldStateFromId(ID id);
    FieldInitState* fieldStateFromIndex(int idx);
    bool isMentionOfNodeInLhsOfAssign(const AstNode* node);
    ID fieldIdFromName(UniqueString name);
    ID fieldIdFromPossibleMentionOfField(const AstNode* node);
    bool isFieldInitialized(ID fieldId);

    bool handleCallToThisComplete(const FnCall* node);
    bool handleCallToSuperInit(const FnCall* node);
    bool handleCallToInit(const FnCall* node);
    bool handleAssignmentToField(const OpCall* node);
    bool handleUseOfField(const AstNode* node);

    // Visitors.
    inline bool enter(const AstNode* node) { return true; }
    inline void exit(const AstNode* node) {}
    bool enter(const OpCall* node);
    void exit(const OpCall* node);
    bool enter(const FnCall* node);
    void exit(const FnCall* node);
    bool enter(const Identifier* node);
    void exit(const Identifier* node);
    bool enter(const Dot* node);
    void exit(const Dot* node);
  };

  static const Type* receiverTypeFromTfs(const TypedFnSignature* tfs) {
    auto ret = tfs->formalType(0).type();
    assert(ret);
    return ret;
  }

  static const CompositeType* typeToCompType(const Type* type) {
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
    auto ret = typeToCompType(type);
    assert(ret);
    return ret;
  }

  Visitor Visitor::create(Context* ctx, Resolver& visitor,
                          const Function* fn) {
    auto tfs = visitor.typedSignature;
    assert(tfs);
    auto recvType = receiverTypeFromTfs(tfs);
    auto ret = Visitor(ctx, visitor, fn, recvType);
    return ret;
  }

  // TODO: More general function to fetch the parent type/fields later.
  bool Visitor::isCallToSuperInitRequired(void) {
    if (auto cls = initialRecvType_->toClassType())
      if (auto bct = cls->basicClassType())
        if (bct->parentClassType())
          return true;
    return false;
  }

  void Visitor::doSetupInitialState(void) {
    fieldToInitState_.clear();

    // Determine the initial phase.
    phase_ = isCallToSuperInitRequired() ? PHASE_NEED_SUPER_INIT
                                         : PHASE_NEED_COMPLETE;

    auto ct = typeToCompType(initialRecvType_);
    auto& rf = fieldsForTypeDecl(ctx_, ct, USE_DEFAULTS);

    // Populate the fields with initial values.
    for (int i = 0; i < rf.numFields(); i++) {
      auto id = rf.fieldDeclId(i);
      FieldInitState state;
      state = { i, ID(), rf.fieldType(i), rf.fieldName(i), false };
      fieldToInitState_.insert({id, std::move(state)});
      fieldIdsByOrdinal_.push_back(id);
    }
  }

  bool Visitor::isFinalReceiverStateValid(void) {
    auto ctInitial = typeToCompType(initialRecvType_);
    auto& rfInitial = fieldsForTypeDecl(ctx_, ctInitial, USE_DEFAULTS);
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
        assert(false && "Not handled yet!");
        ret = false;
      }
    }

    return ret;
  }

  const Type* Visitor::computeReceiverTypeConsideringState(void) {
    auto ctInitial = typeToCompType(initialRecvType_);
    auto& rfInitial = fieldsForTypeDecl(ctx_, ctInitial, USE_DEFAULTS);
    CompositeType::SubstitutionsMap subs;

    if (!rfInitial.isGeneric()) return currentRecvType_;

    for (int i = 0; i < rfInitial.numFields(); i++) {
      auto id = rfInitial.fieldDeclId(i);
      auto state = fieldStateFromId(id);
      auto qtInitial = rfInitial.fieldType(i);
      bool isInitiallyConcrete = qtInitial.genericity() == Type::CONCRETE;

      if (isInitiallyConcrete) continue;

      // TODO: Will need to relax this as we go.
      if (state->qt.isType() || state->qt.isParam())
        if (!state->qt.isGenericOrUnknown())
          subs.insert({id, state->qt});
    }

    const Type* ret = nullptr;
    auto initCompType = typeToCompType(initialRecvType_);
    auto root = initCompType->instantiatedFromCompositeType() ?
                initCompType->instantiatedFromCompositeType() :
                initCompType;

    if (auto rec = initialRecvType_->toRecordType()) {
      ret = RecordType::get(ctx_, rec->id(), rec->name(),
                            root->toRecordType(),
                            subs);
    } else if (auto cls = initialRecvType_->toClassType()) {
      auto oldBasic = cls->basicClassType();
      auto basic = BasicClassType::get(ctx_, oldBasic->id(),
                                       oldBasic->name(),
                                       oldBasic->parentClassType(),
                                       root->toBasicClassType(),
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

  QualifiedType::Kind Visitor::determineReceiverIntent(void) {
    if (initialRecvType_->isClassType()) {
      return QualifiedType::CONST_IN;
    } else {
      assert(initialRecvType_->isRecordType());
      return QualifiedType::REF;
    }
  }

  const TypedFnSignature*
  Visitor::computeTypedSignature(const Type* newRecvType) {
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
                                false,
                                tfs->instantiatedFrom(),
                                tfs->parentFn(),
                                formalsInstantiated);
    return ret;
  }

  // TODO: Identifiy cases where we don't need to do anything.
  const TypedFnSignature* Visitor::run(void) {
    doSetupInitialState();

    auto ret = initResolver_.typedSignature;

    this->traverse(fn_->body());

    if (phase_ < PHASE_COMPLETE) {
      int start = currentFieldIndex_;
      int stop = fieldIdsByOrdinal_.size();
      for (int i = start; i < stop; i++) {
        auto id = fieldIdsByOrdinal_[i];
        bool handled = implicitlyResolveFieldType(id);
        assert(handled);
      }
    }

    if (isFinalReceiverStateValid()) {
      auto newRecvType = computeReceiverTypeConsideringState();
      ret = computeTypedSignature(newRecvType);
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
          if (go) go &= initResolver_.enter(casted); \
          if (go && !node->isLeaf()) { \
            for (const AstNode* child : node->children()) { \
              this->traverse(child); \
            } \
          } \
          initResolver_.exit(casted); \
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

  void Visitor::updateResolverVisibleReceiverType(void) {
    auto updated = computeReceiverTypeConsideringState();
    if (updated != currentRecvType_) {
      auto tfs = computeTypedSignature(updated);
      initResolver_.typedSignature = tfs;
      currentRecvType_ = updated;
    }
  }

  bool Visitor::implicitlyResolveFieldType(ID id) {
    auto state = fieldStateFromId(id);
    if (!state || !state->initPointId.isEmpty()) return false;

    if (state->qt.isParam()) {
      assert(0 == "Not handled yet!");
    } else if (state->qt.isType()) {
      assert(0 == "Not handled yet!");
    } else {
      auto ct = typeToCompType(currentRecvType_);
      auto& rf = resolveFieldDecl(ctx_, ct, id, USE_DEFAULTS);
      for (int i = 0; i < rf.numFields(); i++) {
        auto id = rf.fieldDeclId(i);
        auto state = fieldStateFromId(id);
        assert(state);
        assert(state->qt.kind() == rf.fieldType(i).kind());
        state->qt = rf.fieldType(i);
        state->isInitialized = true;
      }
    }

    return true;
  }

  const AstNode* Visitor::parentOf(const AstNode* node) {
    auto parentId = parsing::idToParentId(ctx_, node->id());
    auto ret = parsing::idToAst(ctx_, parentId);
    return ret;
  }

  Visitor::FieldInitState* Visitor::fieldStateFromId(ID id) {
    auto search = fieldToInitState_.find(id);
    if (search != fieldToInitState_.end()) return &search->second;
    return nullptr;
  }

  Visitor::FieldInitState* Visitor::fieldStateFromIndex(int idx) {
    if (idx < 0 || idx >= fieldIdsByOrdinal_.size()) return nullptr;
    auto id = fieldIdsByOrdinal_[idx];
    auto ret = fieldStateFromId(id);
    return ret;
  }

  bool Visitor::isMentionOfNodeInLhsOfAssign(const AstNode* node) {
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

  ID Visitor::fieldIdFromName(UniqueString name) {
    if (!isNameOfField(ctx_, name, initialRecvType_)) return ID();
    // TODO: Need to replace this as we continue to build it up?
    auto ct = typeToCompType(initialRecvType_);
    auto ret = parsing::fieldIdWithName(ctx_, ct->id(), name);
    return ret;
  }

  ID Visitor::fieldIdFromPossibleMentionOfField(const AstNode* node) {
    UniqueString name;

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

    if (phase_ < PHASE_COMPLETE) {
      auto id = re.toId();
      if (!id.isEmpty() && parsing::idIsField(ctx_, id)) return id;

    } else {
      assert(false && "Do we care about this path at all?");
      if (auto tfs = re.mostSpecific().only()) {
        auto ufs = tfs->untyped();
        if (!ufs->isCompilerGenerated() || !ufs->isMethod()) return ret;
        if (auto ct = receiverCompTypeFromTfs(tfs)) {
          auto id = parsing::fieldIdWithName(ctx_, ct->id(), name);
          if (fieldStateFromId(id)) ret = fieldId;
        }
      }
    }

    return ret;
  }

  bool Visitor::isFieldInitialized(ID fieldId) {
    auto state = fieldStateFromId(fieldId);
    auto ret = state && state->isInitialized;
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

  bool Visitor::handleAssignmentToField(const OpCall* node) {
    if (node->op() != USTR("=")) return false;
    assert(node->numActuals() == 2);
    auto lhs = node->actual(0);
    auto rhs = node->actual(1);

    // TODO: Is 'field' or 'this.field' too strict of a pattern?
    auto fieldId = fieldIdFromPossibleMentionOfField(lhs);
    if (fieldId.isEmpty()) return false;

    auto state = fieldStateFromId(fieldId);
    assert(state);

    bool isAlreadyInitialized = !state->initPointId.isEmpty();
    bool isOutOfOrder = state->ordinalPos < currentFieldIndex_;

    // Implicitly initialize any fields between the current index and this.
    int old = currentFieldIndex_;
    currentFieldIndex_ = state->ordinalPos + 1;
    for (int i = old + 1; i < state->ordinalPos; i++) {
        auto id = fieldIdsByOrdinal_[i];
        bool handled = implicitlyResolveFieldType(id);
        assert(handled);
    }

    // TODO: Anything to do if the opposite is true?
    if (!isAlreadyInitialized) {
      auto& reRhs = initResolver_.byPostorder.byAst(rhs);
      state->qt = reRhs.type();
      state->initPointId = node->id();
      state->isInitialized = true;

      // How often do we need to recompute this? More often?
      if (state->qt.isType() || state->qt.isParam()) {
        updateResolverVisibleReceiverType();
      }

    } else {
      assert(0 == "Not handled yet!");
    }

    if (isOutOfOrder) {
      auto name = state->name;
      ctx_->error(node, "Field \"%s\" initialized out of order",
                        name.c_str());
    }



    return true;
  }

  bool Visitor::handleUseOfField(const AstNode* node) {
    auto fieldId = fieldIdFromPossibleMentionOfField(node);
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
    std::ignore = handleAssignmentToField(node);
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

  bool Visitor::enter(const Dot* node) {
    if (auto ident = node->receiver()->toIdentifier()) {
      // TODO: this.this.this
      if (ident->name() == USTR("this")) {
        auto name = node->field();
        auto id = fieldIdFromName(name);
        if (!id.isEmpty()) {
          auto state = fieldStateFromId(id);
          assert(state);
          auto qt = state->qt;
          auto& re = initResolver_.byPostorder.byAst(node);
          re.setToId(id);
          re.setType(qt);
          return false;
        }
      }
    }

    return true;
  }

  void Visitor::exit(const Dot* node) {
    std::ignore = handleUseOfField(node);
  }

  bool Visitor::enter(const Identifier* node) {
    if (phase_ < PHASE_COMPLETE) {
      auto vec = initResolver_.lookupIdentifier(node, nullptr);
      if (vec.size() == 1 && vec[0].numIds() == 1) {
        auto& id = vec[0].id(0);
        if (!parsing::idIsField(ctx_, id)) {
          std::ignore = initResolver_.resolveIdentifier(node, nullptr);
        } else {
          auto state = fieldStateFromId(id);
          auto qt = state->qt;
          auto& re = initResolver_.byPostorder.byAst(node);
          re.setToId(id);
          re.setType(qt);
        }
      }
    } else {
      auto receiverScope = initResolver_.methodReceiverScope();
      std::ignore = initResolver_.resolveIdentifier(node, receiverScope);
    }

    return false;
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
  auto v = Visitor::create(context, visitor, fn);
  outFinalTfs = v.run();
  return true;
}

} // end namespace resolution
} // end namespace chpl
