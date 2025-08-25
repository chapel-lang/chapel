/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RESOLUTION_RESOLUTION_TYPES_H
#define CHPL_RESOLUTION_RESOLUTION_TYPES_H

#include "chpl/framework/query-impl.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/resolution/ResolutionContext.h"
#include "chpl/resolution/scope-types.h"
#include "chpl/types/CompositeType.h"
#include "chpl/types/EnumType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/types/Type.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/OpCall.h"
#include "chpl/uast/For.h"
#include "chpl/uast/Function.h"
#include "chpl/util/bitmap.h"
#include "chpl/util/hash.h"
#include "chpl/util/memory.h"

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>

namespace chpl {
namespace resolution {

using SubstitutionsMap = types::CompositeType::SubstitutionsMap;

template <typename K>
std::unordered_map<K, types::QualifiedType>
substituteInMap(Context* context,
                const std::unordered_map<K, types::QualifiedType>& substituteIn,
                const types::PlaceholderMap& subs) {
  std::unordered_map<K, types::QualifiedType> into;
  for (auto [id, qt] : substituteIn) {
    into.emplace(id, qt.substitute(context, subs));
  }
  return into;
}

/* When adjusting return intent overloads, the context of the overloaded
   call (is it used as a value, a reference, or a const reference?). */
typedef enum {
  REF = 1,
  CONST_REF = 2,
  VALUE = 3,
  REF_MAYBE_CONST = 4, // used temporarily for recursive cases
} Access;

/**

  In some situations, we may decide not to resolve a call. This could
  happen if we believe it to already be ill-formed (e.g., why would we resolve
  f(x) if x is ill-typed?

  This enum contains reasons why we might want that to do that.
 */
enum SkipCallResolutionReason {
  NONE = 0,

  /* an unknown param (e.g. param int, without a value) */
  UNKNOWN_PARAM,
  /* a type that is a generic type unless there are substitutions */
  GENERIC_TYPE,
  /* a value of generic type */
  GENERIC_VALUE,
  /* UnknownType, ErroneousType */
  UNKNOWN_ACT, ERRONEOUS_ACT,
  /* other reason to skip */
  OTHER_REASON,
};

enum struct DefaultsPolicy {
  /** Do not use default values when determining field type. */
  IGNORE_DEFAULTS,
  /** Use default values when determining field type. */
  USE_DEFAULTS,
  /** Do not use default values for the current field (i.e. when set up
     for resolving a field statement), but do use default values
     for all other fields. This policy is useful when determining the
     genericity of individual fields. */
  USE_DEFAULTS_OTHER_FIELDS
};

/**
  An untyped function signature. This is really just the part of a function
  including the formals. It exists so that the process of identifying
  candidates does not need to depend on the bodies of the function
  (in terms of incremental recomputation).

  For type constructors for generic types, the formal decls
  are actually field decls.
 */
class UntypedFnSignature {
 public:
  enum DefaultKind {
    /** Formals that have default values, like `in x = 10` */
    DK_DEFAULT,
    /** Formals that do not have default values, like `ref x` */
    DK_NO_DEFAULT,
    /** Formals that might have a default value. This comes up when working
        with generic initializers; whether an initializer's formal has
        a default depends on if its type has a default value. But if
        the type is unknown -- as in a generic initializer's type signature --
        then we don't know if the formal has a default. */
    DK_MAYBE_DEFAULT,
  };

  struct FormalDetail {
    UniqueString name;
    DefaultKind defaultKind = DK_NO_DEFAULT;
    const uast::Decl* decl = nullptr;
    bool isVarArgs = false;

    FormalDetail(UniqueString name,
                 DefaultKind defaultKind,
                 const uast::Decl* decl,
                 bool isVarArgs = false)
      : name(name),
        defaultKind(defaultKind),
        decl(decl),
        isVarArgs(isVarArgs)
    {
      CHPL_ASSERT(name != USTR("this") || defaultKind == DK_NO_DEFAULT);
    }

    bool operator==(const FormalDetail& other) const {
      return name == other.name &&
             defaultKind == other.defaultKind &&
             decl == other.decl &&
             isVarArgs == other.isVarArgs;
    }
    bool operator!=(const FormalDetail& other) const {
      return !(*this == other);
    }
    size_t hash() const {
      return chpl::hash(name, defaultKind, decl, isVarArgs);
    }

    void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
      name.stringify(ss, stringKind);
      ss << " ";
      if (decl != nullptr) {
        decl->stringify(ss, stringKind);
        ss << " ";
      }
    }
    /// \cond DO_NOT_DOCUMENT
    DECLARE_DUMP;
    /// \endcond DO_NOT_DOCUMENT
  };

 private:
  // the ID of a related uAST node:
  //   * in most cases, a Function
  //   * can be a TypeDecl for a type constructor / init / other method
  //   * can be a Variable for a field accessor
  ID id_;
  UniqueString name_;
  bool isMethod_; // in that case, formals[0] is the receiver
  bool isTypeConstructor_;
  bool isCompilerGenerated_;
  bool throws_;
  uast::asttags::AstTag idTag_; // concrete tag for ID
  uast::Function::Kind kind_;
  std::vector<FormalDetail> formals_;

  // this will not be present for compiler-generated functions
  const uast::AstNode* whereClause_;

  // The ID that this compiler-generated function is based on
  ID compilerGeneratedOrigin_;

  UntypedFnSignature(ID id,
                     UniqueString name,
                     bool isMethod,
                     bool isTypeConstructor,
                     bool isCompilerGenerated,
                     bool throws,
                     uast::asttags::AstTag idTag,
                     uast::Function::Kind kind,
                     std::vector<FormalDetail> formals,
                     const uast::AstNode* whereClause,
                     ID compilerGeneratedOrigin = ID())
    : id_(id),
      name_(name),
      isMethod_(isMethod),
      isTypeConstructor_(isTypeConstructor),
      isCompilerGenerated_(isCompilerGenerated),
      throws_(throws),
      idTag_(idTag),
      kind_(kind),
      formals_(std::move(formals)),
      whereClause_(whereClause),
      compilerGeneratedOrigin_(compilerGeneratedOrigin) {
    CHPL_ASSERT(idTag == uast::asttags::Function ||
           idTag == uast::asttags::Class    ||
           idTag == uast::asttags::Record   ||
           idTag == uast::asttags::Tuple    ||
           idTag == uast::asttags::Union    ||
           idTag == uast::asttags::Variable ||
           idTag == uast::asttags::Enum);
  }

  static const owned<UntypedFnSignature>&
  getUntypedFnSignature(Context* context, ID id,
                        UniqueString name,
                        bool isMethod,
                        bool isTypeConstructor,
                        bool isCompilerGenerated,
                        bool throws,
                        uast::asttags::AstTag idTag,
                        uast::Function::Kind kind,
                        std::vector<FormalDetail> formals,
                        const uast::AstNode* whereClause,
                        ID compilerGeneratedOrigin = ID());

 public:
  /** Get the unique UntypedFnSignature containing these components */
  static const UntypedFnSignature* get(Context* context, ID id,
                                       UniqueString name,
                                       bool isMethod,
                                       bool isTypeConstructor,
                                       bool isCompilerGenerated,
                                       bool throws,
                                       uast::asttags::AstTag idTag,
                                       uast::Function::Kind kind,
                                       std::vector<FormalDetail> formals,
                                       const uast::AstNode* whereClause,
                                       ID compilerGeneratedOrigin = ID());

  /** Get the unique UntypedFnSignature representing a Function's
      signature from a Function uAST pointer. */
  static const UntypedFnSignature* get(Context* context,
                                       const uast::Function* function);

  /** Get the unique UntypedFnSignature representing a Function's
      signature from a Function ID. */
  static const UntypedFnSignature* get(Context* context, ID functionId);

  bool operator==(const UntypedFnSignature& other) const {
    return id_ == other.id_ &&
           name_ == other.name_ &&
           isMethod_ == other.isMethod_ &&
           isTypeConstructor_ == other.isTypeConstructor_ &&
           isCompilerGenerated_ == other.isCompilerGenerated_ &&
           throws_ == other.throws_ &&
           idTag_ == other.idTag_ &&
           kind_ == other.kind_ &&
           formals_ == other.formals_ &&
           whereClause_ == other.whereClause_ &&
           compilerGeneratedOrigin_ == other.compilerGeneratedOrigin_;
  }
  bool operator!=(const UntypedFnSignature& other) const {
    return !(*this == other);
  }

  static bool update(owned<UntypedFnSignature>& keep,
                     owned<UntypedFnSignature>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    id_.mark(context);
    name_.mark(context);
    for (auto const& elt : formals_) {
      elt.name.mark(context);
      context->markPointer(elt.decl);
    }
    context->markPointer(whereClause_);
    compilerGeneratedOrigin_.mark(context);
  }


  /** Returns the id of the relevant uast node (usually a Function
      but it can be a Record or Class for compiler-generated functions) */
  const ID& id() const {
    return id_;
  }

  /** Returns the name of the function this signature represents */
  UniqueString name() const {
    return name_;
  }

  /** Returns the kind of the function this signature represents */
  uast::Function::Kind kind() const {
    return kind_;
  }

  /** Returns true if this compiler generated */
  bool isCompilerGenerated() const {
    return isCompilerGenerated_;
  }

  ID compilerGeneratedOrigin() const {
    return compilerGeneratedOrigin_;
  }

  /** Returns true if id() refers to a Function */
  bool idIsFunction() const {
    return idTag_ == uast::asttags::Function;
  }

  /** Returns true if id() refers to a function declared in an extern block */
  bool idIsExternBlockFunction() const {
    return isCompilerGenerated() && id().isExternBlockElement();
  }

  /** Returns true if id() refers to a Class */
  bool idIsClass() const {
    return idTag_ == uast::asttags::Class;
  }

  /** Returns true if id() refers to a Record */
  bool idIsRecord() const {
    return idTag_ == uast::asttags::Record;
  }

  /** Returns true if id() refers to a Union */
  bool idIsUnion() const {
    return idTag_ == uast::asttags::Union;
  }

  /** Returns true if id() refers to a Variable (for a field) */
  bool idIsField() const {
    return idTag_ == uast::asttags::Variable;
  }

  /** Returns true if this is a type constructor */
  bool isTypeConstructor() const {
    return isTypeConstructor_;
  }

  /** Returns true if this is a method */
  bool isMethod() const {
    return isMethod_;
  }

  /** Returns true if this is a procedure. */
  bool isProcedure() const {
    return kind_ == uast::Function::PROC;
  }

  /** Returns true if this is an operator. */
  bool isOperator() const {
    return kind_ == uast::Function::OPERATOR;
  }

  /** Returns true if this is an iterator. */
  bool isIterator() const {
    return kind_ == uast::Function::ITER;
  }

  /** Returns true if this function throws */
  bool throws() const {
    return throws_;
  }

  /** Returns the number of formals */
  int numFormals() const {
    return formals_.size();
  }
  /** Returns the name of the i'th formal. */
  UniqueString formalName(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < formals_.size());
    return formals_[i].name;
  }

  /** Return whether the i'th formal might have a default value. */
  bool formalMightHaveDefault(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < formals_.size());
    return formals_[i].defaultKind != DK_NO_DEFAULT;
  }

  DefaultKind formalDefaultKind(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < formals_.size());
    return formals_[i].defaultKind;
  }

  /** Returns the Decl for the i'th formal / field.
      This will return nullptr for compiler-generated functions. */
  const uast::Decl* formalDecl(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < formals_.size());
    return formals_[i].decl;
  }

  bool formalIsVarArgs(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < formals_.size());
    return formals_[i].isVarArgs;
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
    name().stringify(ss, stringKind);
    ss << " ";
    id().stringify(ss, stringKind);
    ss << " ";
    ss << std::to_string(numFormals());
    ss << " ";
  }

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

const UntypedFnSignature*
getUntypedFnSignatureForFn(Context* context,
                           const uast::Function* fn,
                           ID const* compilerGeneratedOrigin = nullptr);

/**
  This type stores the outer variables used by a function. For each mention
  of an outer variable, it keeps track of the outer variable's ID and type.
*/
class OuterVariables {
 public:
  using QualifiedType = types::QualifiedType;
  using TargetAndType = std::pair<const ID, QualifiedType>;
 private:
  std::map<ID, QualifiedType> targetIdToType_;
  std::map<ID, ID> mentionIdToTargetId_;

 public:
  OuterVariables() = default;
 ~OuterVariables() = default;

  bool operator==(const OuterVariables& rhs) const {
    return targetIdToType_ == rhs.targetIdToType_ &&
           mentionIdToTargetId_ == rhs.mentionIdToTargetId_;
  }

  bool operator!=(const OuterVariables& rhs) const {
    return !(*this == rhs);
  }

  void swap(OuterVariables& rhs) {
    std::swap (targetIdToType_, rhs.targetIdToType_);
    std::swap (mentionIdToTargetId_, rhs.mentionIdToTargetId_);
  }

  void mark(Context* context) const {
    for (auto& p : targetIdToType_) {
      p.first.mark(context);
      p.second.mark(context);
    }
    for (auto& p : mentionIdToTargetId_) {
      p.first.mark(context);
      p.second.mark(context);
    }
  }

  static inline bool update(owned<OuterVariables>& keep,
                            owned<OuterVariables>& addin) {
    return defaultUpdateOwned(keep, addin);
  }

  size_t hash() const {
    return chpl::hash(targetIdToType_, mentionIdToTargetId_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
    for (auto& p : targetIdToType_) {
      p.first.stringify(ss, stringKind);
      ss << " : ";
      p.second.stringify(ss, stringKind);
    }
  }

  /** Mutating method used to build up state. If either the mention or
      target IDs are empty then nothing happens. */
  void add(const ID& mention, const ID& target, const QualifiedType& qt) {
    if (!mention || !target) return;
    auto p1 = std::make_pair(mention, target);
    if (!mentionIdToTargetId_.insert(std::move(p1)).second) return;
    auto p2 = std::make_pair(target, qt);
    targetIdToType_.insert(std::move(p2));
  }

  /** For a given mention, return the target ID and type if it exists. */
  const TargetAndType* targetAndTypeOrNull(const ID& mention) const {
    auto it = mentionIdToTargetId_.find(mention);
    if (it == mentionIdToTargetId_.end()) return nullptr;
    return &*targetIdToType_.find(it->second);
  }

  /** If available, returns the type associated with an outer variable's ID */
  const std::optional<QualifiedType> targetType(const ID& target) const {
    auto it = targetIdToType_.find(target);
    if (it == targetIdToType_.end()) {
      return {};
    } else {
      return { it->second };
    }
  }

  bool isEmpty() const {
    return mentionIdToTargetId_.empty();
  }
};

class CallInfoActual {
 private:
  types::QualifiedType type_;
  UniqueString byName_;

 public:
  explicit CallInfoActual(types::QualifiedType type)
    : type_(std::move(type)) {
  }
  CallInfoActual(types::QualifiedType type, UniqueString byName)
    : type_(std::move(type)), byName_(byName) {
  }

  /** return the qualified type */
  const types::QualifiedType& type() const { return type_; }

  /** return the name, if any, that the argument was passed with.
      Ex: in f(number=3), byName() would be "number"
   */
  UniqueString byName() const { return byName_; }

  bool operator==(const CallInfoActual &other) const {
    return type_ == other.type_ && byName_ == other.byName_;
  }
  bool operator!=(const CallInfoActual& other) const {
    return !(*this == other);
  }
  void mark(Context* context) const {
    type_.mark(context);
    byName_.mark(context);
  }
  size_t hash() const {
    return chpl::hash(type_, byName_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

class ResolutionResultByPostorderID;

/** CallInfo */
class CallInfo {
 private:
  UniqueString name_;                   // the name of the called thing
  types::QualifiedType calledType_;     // the type of the called thing
  bool isMethodCall_ = false;           // then actuals[0] is receiver
  bool isOpCall_ = false;               // is an operator call
  bool hasQuestionArg_ = false;         // includes ? arg for type constructor
  bool isParenless_ = false;            // is a parenless call

  // Performance TODO: use SmallVector here?
  std::vector<CallInfoActual> actuals_; // types/params/names of actuals

 public:
  using CallInfoActualIterable = Iterable<std::vector<CallInfoActual>>;

  /** default constructor for the query system */
  CallInfo() : name_(), calledType_() {}

  /** Construct a CallInfo that contains QualifiedTypes for actuals */
  CallInfo(UniqueString name, types::QualifiedType calledType,
           bool isMethodCall,
           bool hasQuestionArg,
           bool isParenless,
           std::vector<CallInfoActual> actuals)
      : name_(name), calledType_(calledType),
        isMethodCall_(isMethodCall),
        hasQuestionArg_(hasQuestionArg),
        isParenless_(isParenless),
        actuals_(std::move(actuals)) {
    #ifndef NDEBUG
    if (isMethodCall) {
      CHPL_ASSERT(numActuals() >= 1);
      CHPL_ASSERT(this->actual(0).byName() == "this");
      CHPL_ASSERT(calledType.isUnknown());
    }
    if (isParenless) {
      if (isMethodCall) {
        CHPL_ASSERT(numActuals() == 1);
      } else {
        CHPL_ASSERT(numActuals() == 0);
      }
    }
    #endif
    isOpCall_ = uast::isOpName(name);
  }

  /** Construct a CallInfo with no arguments, for calling a regular function
      by name. Does not handle methods. */
  static CallInfo createSimple(UniqueString calledFnName);

  /** Construct a CallInfo with one argument, for calling a regular function
      by name. Does not handle methods. */
  static CallInfo createSimple(UniqueString calledFnName,
                               types::QualifiedType arg1type);
  /** Construct a CallInfo with two arguments,
      for calling a regular function by name. Does not handle methods. */
  static CallInfo createSimple(UniqueString calledFnName,
                               types::QualifiedType arg1type,
                               types::QualifiedType arg2type);

  /** Construct a CallInfo with unknown types for the actuals
      that can be used for FormalActualMap but not much else.
      Assumes that the calledExpression is an identifier
      and that it is a function name (vs a method invocation).
      */
  static CallInfo createUnknown(const uast::FnCall* call);

  /** Construct a CallInfo from a Call and optionally
      raise errors that occur when doing so.
      Assumes that the actual arguments have already been resolved
      and their types are available in 'byPostorder'.

      If 'raiseErrors' is 'true' (the default), then errors encountered
      will be raised on the current query.

      If actualAsts is provided and not 'nullptr', it will be updated
      to contain the uAST pointers for each actual.

      If moduleScopeId is provided and not 'nullptr', it will be updated
      with the ID of the scope that should be searched for candidates.
      That is, if the call expression is 'M.f(...)' for a module 'M', then
      'moduleScopeId' will be set to the ID of the module 'M'.
   */
  static CallInfo create(Context* context,
                         const uast::Call* call,
                         const ResolutionResultByPostorderID& byPostorder,
                         bool raiseErrors = true,
                         std::vector<const uast::AstNode*>* actualAsts=nullptr,
                         ID* moduleScopeId=nullptr,
                         UniqueString rename = UniqueString());

  /** Construct a CallInfo by adding a method receiver argument to
      the passed CallInfo. */
  static CallInfo createWithReceiver(const CallInfo& ci,
                                     types::QualifiedType receiverType,
                                     UniqueString rename=UniqueString());

  /** Copy and rename a CallInfo. */
  static CallInfo copyAndRename(const CallInfo& ci, UniqueString rename);

  /** Prepare actuals for a call for later use in creating a CallInfo.
      This is a helper function for CallInfo::create that is sometimes
      useful to call separately.

      Sets 'actuals' and 'hasQuestionArg'.

      If actualIds is not 'nullptr', then the toID value of each actual is
      pushed to that array.
    */
  static void prepareActuals(Context* context,
                             const uast::Call* call,
                             const ResolutionResultByPostorderID& byPostorder,
                             bool raiseErrors,
                             std::vector<CallInfoActual>& actuals,
                             const uast::AstNode*& questionArg,
                             std::vector<const uast::AstNode*>* actualAsts);

  /** Same as prepareActuals, but instead of iterating over all the actuals
      of a call, simply prepare a single actual.

      Unlike prepareActuals, call can be null (if setting up an actual
      from a call-like thing, like a domain expression).
   */
  static void prepareActual(Context* context,
                            const uast::Call* call,
                            const uast::AstNode* actual,
                            int actualIdx,
                            const ResolutionResultByPostorderID& byPostorder,
                            bool raiseErrors,
                            std::vector<CallInfoActual>& actuals,
                            const uast::AstNode*& questionArg,
                            std::vector<const uast::AstNode*>* actualAsts);

  /** return the type of the 'this' actual, aka the receiver of a method call. */
  types::QualifiedType methodReceiverType() const {
    CHPL_ASSERT(isMethodCall());
    CHPL_ASSERT(numActuals() > 0);
    auto& firstActual = actual(0);
    CHPL_ASSERT(firstActual.byName() == "this");
    return firstActual.type();
  }

  /** return the name of the called thing */
  const UniqueString name() const { return name_; }

  /** return the type of the called thing */
  types::QualifiedType calledType() const { return calledType_; }

  /** check if the call is a method call */
  bool isMethodCall() const { return isMethodCall_; }

  /** check if the call is an operator call */
  bool isOpCall() const { return isOpCall_; }

  /** check if the call includes ? arg for type constructor */
  bool hasQuestionArg() const { return hasQuestionArg_; }

  /** return true if the call did not use parens */
  bool isParenless() const { return isParenless_; }

  /** return the actuals */
  CallInfoActualIterable actuals() const {
    return CallInfoActualIterable(actuals_);
  }

  /** return the i'th actual */
  const CallInfoActual& actual(size_t i) const {
    CHPL_ASSERT(i < actuals_.size());
    return actuals_[i];
  }

  /** return the number of actuals */
  size_t numActuals() const { return actuals_.size(); }

  bool operator==(const CallInfo& other) const {
    return name_ == other.name_ &&
           calledType_ == other.calledType_ &&
           isMethodCall_ == other.isMethodCall_ &&
           isOpCall_ == other.isOpCall_ &&
           hasQuestionArg_ == other.hasQuestionArg_ &&
           isParenless_ == other.isParenless_ &&
           actuals_ == other.actuals_;
  }
  bool operator!=(const CallInfo& other) const {
    return !(*this == other);
  }
  void mark(Context* context) const {
    name_.mark(context);
    calledType_.mark(context);
    for (auto& actual : actuals_) {
      actual.mark(context);
    }
  }
  size_t hash() const {
    return chpl::hash(name_, calledType_, isMethodCall_, isOpCall_,
                      hasQuestionArg_, isParenless_,
                      actuals_);
  }
  static bool update(CallInfo& keep,
                     CallInfo& addin) {
    return defaultUpdate(keep, addin);
  }

  void swap(CallInfo& other) {
    std::swap(name_, other.name_);
    std::swap(calledType_, other.calledType_);
    std::swap(isMethodCall_, other.isMethodCall_);
    std::swap(isOpCall_, other.isOpCall_);
    std::swap(hasQuestionArg_, other.hasQuestionArg_);
    std::swap(isParenless_, other.isParenless_);
    actuals_.swap(other.actuals_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

using PoiCallIdFnIds = std::set<std::pair<ID, ID>>;
using PoiRecursiveCalls = std::set<std::pair<const TypedFnSignature*,
                                             const PoiScope*>>;

/**
  Contains information about symbols available from point-of-instantiation
  in order to implement caching of instantiations.
 */
class PoiInfo {
 public:
  using Trace = std::tuple<const TypedFnSignature*,
                           PoiCallIdFnIds,
                           PoiRecursiveCalls>;
 private:
  // is this PoiInfo for a function that has been resolved, or
  // for a function we are about to resolve?
  bool resolved_ = false;

  // For a not-yet-resolved instantiation
  const PoiScope* poiScope_ = nullptr;

  // TODO: add VisibilityInfo etc -- names of calls.
  // see PR #16261

  // For a resolved instantiation

  // Tracking how calls using POI were resolved.
  // This is a set of pairs of (Call ID, Function ID).
  // This includes POI calls from functions called in this Function,
  // transitively, unless they are recursive (being resolved)
  // in which case they go into recursiveFnsUsed_.
  PoiCallIdFnIds poiFnIdsUsed_;

  // Tracking recursive calls not analyzed
  // (because queries cannot be recursive).
  // This PoiInfo should be considered to include the
  // PoiInfo from resolving each signature+poi scope in this list.
  PoiRecursiveCalls recursiveFnsUsed_;

 public:
  PoiInfo() = default;
  PoiInfo(const PoiInfo& rhs) = default;
  PoiInfo(PoiInfo&& rhs) = default;
  PoiInfo& operator=(PoiInfo&& rhs) = default;

  // construct a PoiInfo for a not-yet-resolved instantiation
  PoiInfo(const PoiScope* poiScope)
    : resolved_(false), poiScope_(poiScope) {
  }
  // construct a PoiInfo for a resolved instantiation
  PoiInfo(std::set<std::pair<ID, ID>> poiFnIdsUsed)
    : resolved_(true), poiFnIdsUsed_(std::move(poiFnIdsUsed)) {
  }

  /** return the poiScope */
  const PoiScope* poiScope() const { return poiScope_; }

  /** set the poiScope */
  void setPoiScope(const PoiScope* poiScope) { poiScope_ = poiScope; }

  /** set resolved */
  void setResolved(bool resolved) { resolved_ = resolved; }

  const PoiCallIdFnIds& poiFnIdsUsed() const {
    return poiFnIdsUsed_;
  }

  const PoiRecursiveCalls& recursiveFnsUsed() const {
    return recursiveFnsUsed_;
  }

  PoiInfo::Trace createTraceFor(const TypedFnSignature* sig) const {
    return { sig, poiFnIdsUsed_, recursiveFnsUsed_ };
  }

  void addIds(ID a, ID b) {
    poiFnIdsUsed_.emplace(a, b);
  }

  // return true if the two passed PoiInfos represent the same information
  // (for use in an update function)
  static bool updateEquals(const PoiInfo& a, const PoiInfo& b) {
    return a.resolved_ == b.resolved_ &&
           a.poiScope_ == b.poiScope_ &&
           a.poiFnIdsUsed_ == b.poiFnIdsUsed_ &&
           a.recursiveFnsUsed_ == b.recursiveFnsUsed_;
  }

  void swap(PoiInfo& other) {
    std::swap(resolved_, other.resolved_);
    std::swap(poiScope_, other.poiScope_);
    poiFnIdsUsed_.swap(other.poiFnIdsUsed_);
    recursiveFnsUsed_.swap(other.recursiveFnsUsed_);
  }

  // accumulate PoiInfo from a call into this PoiInfo
  void accumulate(const PoiInfo& addPoiInfo);

  // accumulate PoiInfo from a recursive call into this PoiInfo
  void accumulateRecursive(const TypedFnSignature* signature,
                           const PoiScope* poiScope);

  // return true if 'this' represents a resolved function that can
  // be reused given the PoiInfo for a not-yet-resolved function in 'check'.
  bool canReuse(const PoiInfo& check) const;

  // return true if one of the PoiInfos is a resolved function that
  // can be reused given PoiInfo for a not-yet-resolved function.
  static bool reuseEquals(const PoiInfo& a, const PoiInfo& b) {
    if (a.resolved_ && !b.resolved_) {
      return a.canReuse(b);
    }
    if (b.resolved_ && !a.resolved_) {
      return b.canReuse(a);
    }
    return updateEquals(a, b);
  }

  // hashing a PoiInfo gives 0 always
  // (instead we rely on == in the hashtable so that we can
  //  apply canReuse to figure out if an instantiation can be reused).
  size_t hash() const {
    return 0;
  }
  // == and != for the hashtable
  bool operator==(const PoiInfo& other) const {
    return PoiInfo::reuseEquals(*this, other);
  }
  bool operator!=(const PoiInfo& other) const {
    return !(*this == other);
  }

  void mark(Context* context) const {
    context->markPointer(poiScope_);
    for (auto const &elt : poiFnIdsUsed_) {
      elt.first.mark(context);
      elt.second.mark(context);
    }
    for (auto const &elt : recursiveFnsUsed_) {
      context->markPointer(elt.first);
      context->markPointer(elt.second);
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
    ss << "PoiInfo: ";
    if (poiScope()) {
      poiScope()->stringify(ss, stringKind);
    }
  }

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

// TODO: should this actually be types::FunctionType?
/**
  This represents a typed function signature.
*/
class TypedFnSignature {
 public:
  typedef enum {
    WHERE_NONE,  // no where clause
    WHERE_TBD,   // where clause not resolved yet
    WHERE_TRUE,  // where resulted in true
    WHERE_FALSE, // where resulted in false
  } WhereClauseResult;

 private:
  // What is the untyped function signature?
  const UntypedFnSignature* untypedSignature_;
  // What is the type of each of the formals?
  std::vector<types::QualifiedType> formalTypes_;
  // If there was a where clause, what was the result of evaluating it?
  WhereClauseResult whereClauseResult_ = WHERE_TBD;

  // Are any of the formals generic or unknown at this point?
  bool needsInstantiation_ = true;

  // Does this TypedFnSignature represent a refinement of another
  // TypedFnSignature with no new instantiation information?
  // This is used for TypedFnSignatures that represent the inferred type
  // of an 'out' formal.
  // In that case, instantiatedFrom_ stores the TypedFnSignature
  // that was refined.
  bool isRefinementOnly_ = false;

  // Is this TypedFnSignature representing an instantiation?
  // If so, what is the generic TypedFnSignature that was instantiated?
  const TypedFnSignature* instantiatedFrom_ = nullptr;

  // Is this for an inner Function? If so, what is the parent
  // function signature?
  const TypedFnSignature* parentFn_ = nullptr;

  // Which formal arguments were substituted when instantiating?
  Bitmap formalsInstantiated_;

  // What are the the types of outer variables used to construct this?
  // TODO: Can probably flatten into a vector.
  OuterVariables outerVariables_;

  TypedFnSignature(const UntypedFnSignature* untypedSignature,
                   std::vector<types::QualifiedType> formalTypes,
                   WhereClauseResult whereClauseResult,
                   bool needsInstantiation,
                   bool isRefinementOnly,
                   const TypedFnSignature* instantiatedFrom,
                   const TypedFnSignature* parentFn,
                   Bitmap formalsInstantiated,
                   OuterVariables outerVariables)
    : untypedSignature_(untypedSignature),
      formalTypes_(std::move(formalTypes)),
      whereClauseResult_(whereClauseResult),
      needsInstantiation_(needsInstantiation),
      isRefinementOnly_(isRefinementOnly),
      instantiatedFrom_(instantiatedFrom),
      parentFn_(parentFn),
      formalsInstantiated_(std::move(formalsInstantiated)),
      outerVariables_(std::move(outerVariables)) { }

  static const owned<TypedFnSignature>&
  getTypedFnSignature(Context* context,
                      const UntypedFnSignature* untypedSignature,
                      std::vector<types::QualifiedType> formalTypes,
                      TypedFnSignature::WhereClauseResult whereClauseResult,
                      bool needsInstantiation,
                      bool isRefinementOnly,
                      const TypedFnSignature* instantiatedFrom,
                      const TypedFnSignature* parentFn,
                      Bitmap formalsInstantiated,
                      OuterVariables outerVariables);

  /** If this is an iterator, set 'found' to a string representing its
      'iterKind', or "" if it is a serial iterator. Returns 'true' only
      if this is an iterator and a valid 'iterKind' formal was found. */
  bool fetchIterKindStr(Context* context, UniqueString& outIterKindStr) const;

 public:
  /** Get the unique TypedFnSignature containing these components */
  static
  const TypedFnSignature* get(Context* context,
                              const UntypedFnSignature* untypedSignature,
                              std::vector<types::QualifiedType> formalTypes,
                              TypedFnSignature::WhereClauseResult whereClauseResult,
                              bool needsInstantiation,
                              const TypedFnSignature* instantiatedFrom,
                              const TypedFnSignature* parentFn,
                              Bitmap formalsInstantiated,
                              OuterVariables outerVariables);

  /** Get the unique TypedFnSignature containing these components
      for a refinement where some types are inferred (e.g. generic 'out'
      formals have their type inferred from the body). */
  static
  const TypedFnSignature* getInferred(
                              Context* context,
                              std::vector<types::QualifiedType> formalTypes,
                              const TypedFnSignature* inferredFrom);

  const TypedFnSignature* substitute(Context* context,
                                     const types::PlaceholderMap& subs) const;

  bool operator==(const TypedFnSignature& other) const {
    return untypedSignature_ == other.untypedSignature_ &&
           formalTypes_ == other.formalTypes_ &&
           whereClauseResult_ == other.whereClauseResult_ &&
           needsInstantiation_ == other.needsInstantiation_ &&
           isRefinementOnly_ == other.isRefinementOnly_ &&
           instantiatedFrom_ == other.instantiatedFrom_ &&
           parentFn_ == other.parentFn_ &&
           formalsInstantiated_ == other.formalsInstantiated_ &&
           outerVariables_ == other.outerVariables_;
  }
  bool operator!=(const TypedFnSignature& other) const {
    return !(*this == other);
  }

  static bool update(owned<TypedFnSignature>& keep,
                     owned<TypedFnSignature>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    context->markPointer(untypedSignature_);
    for (auto const &elt : formalTypes_) {
      elt.mark(context);
    }
    context->markPointer(instantiatedFrom_);
    context->markPointer(parentFn_);
    (void) formalsInstantiated_; // nothing to mark
    outerVariables_.mark(context);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /** Returns the id of the relevant uast node (usually a Function
      but it can be a Record or Class for compiler-generated functions) */
  const ID& id() const {
    return untypedSignature_->id();
  }

  /** Returns the UntypedFnSignature */
  const UntypedFnSignature* untyped() const {
    return untypedSignature_;
  }

  bool isCompilerGenerated() const {
    return untyped()->isCompilerGenerated();
  }

  /** Returns the result of evaluating the where clause */
  WhereClauseResult whereClauseResult() const {
    return whereClauseResult_;
  }

  /** Returns if any of the formals are generic or unknown */
  bool needsInstantiation() const {
    return needsInstantiation_;
  }

  bool isMethod() const { return untyped()->isMethod(); }

  bool isInit() const {
    return untyped()->name() == USTR("init") && isMethod();
  }

  bool isInitEquals() const {
    return untyped()->name() == USTR("init=") && isMethod();
  }

  bool isInitializer() const {
    return isInit() || isInitEquals();
  }

  bool isDeinit() const {
    return untyped()->name() == USTR("deinit") && isMethod();
  }

  /** If this TypedFnSignature represents the result of additional
      inference, return the most basic TypedFnSignature that was
      inferred from. */
  const TypedFnSignature* inferredFrom() const {
    const TypedFnSignature* ret = this;
    if (ret->isRefinementOnly_) {
      ret = ret->instantiatedFrom_;
      CHPL_ASSERT(!ret->isRefinementOnly_);
    }
    return ret;
  }

  /**
    Is this TypedFnSignature representing an instantiation?  If so, returns the
    generic TypedFnSignature that was instantiated.  Otherwise, returns nullptr.

    This function always returns the signature for the fully generic
    function and never a partial instantiation. That is, the result
    will either be nullptr or result->instantiatedFrom() will be nullptr.
   */
  const TypedFnSignature* instantiatedFrom() const {
    const TypedFnSignature* sig = inferredFrom();
    CHPL_ASSERT(sig->instantiatedFrom_ == nullptr ||
                sig->instantiatedFrom_->instantiatedFrom_ == nullptr);
    return sig->instantiatedFrom_;
  }

  /**
    Returns 'true' if formal argument i was instantiated; that is,
    it was present in the SubstitutionsMap when instantiating.
   */
  bool formalIsInstantiated(int i) const {
    const TypedFnSignature* sig = inferredFrom();
    if (sig->instantiatedFrom_ == nullptr)
      return false;

    return sig->formalsInstantiated_[i];
  }

  const Bitmap& formalsInstantiatedBitmap() const {
    const TypedFnSignature* sig = inferredFrom();
    return sig->formalsInstantiated_;
  }

  /**
     Is this for an inner Function? If so, what is the parent
     function signature?
   */
  const TypedFnSignature* parentFn() const {
    const TypedFnSignature* sig = inferredFrom();
    return sig->parentFn_;
  }

  bool isNestedFunction() const {
    return parentFn() != nullptr;
  }

  /** Return the outer variables that were used to type this signature. */
  const OuterVariables& outerVariables() const {
    return outerVariables_;
  }

  /** Returns true if this signature uses any outer variables. */
  bool usesOuterVariables() const {
    return !outerVariables_.isEmpty();
  }

  /** Returns the number of formals */
  int numFormals() const {
    int ret = formalTypes_.size();
    CHPL_ASSERT(untypedSignature_ && ret == untypedSignature_->numFormals());
    return ret;
  }
  /** Returns the name of the i'th formal */
  UniqueString formalName(int i) const {
    return untypedSignature_->formalName(i);
  }
  /** Returns the type of the i'th formal */
  const types::QualifiedType& formalType(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < formalTypes_.size());
    return formalTypes_[i];
  }

  bool isOperator() const {
    return untypedSignature_->isOperator();
  }

  bool isIterator() const {
    return untypedSignature_->isIterator();
  }

  /** Returns 'true' if this signature is for a standalone parallel iterator. */
  bool isParallelStandaloneIterator(Context* context) const {
    UniqueString str;
    return fetchIterKindStr(context, str) && str == USTR("standalone");
  }

  /** Returns 'true' if this signature is for a parallel leader iterator. */
  bool isParallelLeaderIterator(Context* context) const {
    UniqueString str;
    return fetchIterKindStr(context, str) && str == USTR("leader");
  }

  /** Returns 'true' if this signature is for a parallel follower iterator. */
  bool isParallelFollowerIterator(Context* context) const {
    UniqueString str;
    return fetchIterKindStr(context, str) && str == USTR("follower");
  }

  /** Returns 'true' if this signature is for a parallel iterator. */
  bool isParallelIterator(Context* context) const {
    return isParallelStandaloneIterator(context) ||
           isParallelLeaderIterator(context) ||
           isParallelFollowerIterator(context);
  }

  /** Returns 'true' if this signature is for a serial iterator. */
  bool isSerialIterator(Context* context) const {
    UniqueString str;
    return fetchIterKindStr(context, str) && str.isEmpty();
  }

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

// Container for resolution candidates and (if applicable) their corresponding
// forwarding-to types.
struct CandidatesAndForwardingInfo {
 private:
  std::vector<const TypedFnSignature*> candidates;
  // Note we have a (small) storage footprint for forwardingInfo even in the
  // relatively common case where it is unused; could potentially use something
  // lighter than this struct for candidates without forwarding info.
  std::vector<types::QualifiedType> forwardingInfo;

 public:
  using const_iterator = std::vector<const TypedFnSignature*>::const_iterator;

  // Add a candidate without forwarding info.
  void addCandidate(const TypedFnSignature* candidate) {
    candidates.push_back(candidate);
  }

  // Compute and fill in forwarding info for a range of newly-added candidates.
  void helpComputeForwardingTo(const CallInfo& fci, size_t start) {
    CHPL_ASSERT(fci.isMethodCall());
    CHPL_ASSERT(forwardingInfo.size() <= start);
    forwardingInfo.resize(start);
    types::QualifiedType forwardingReceiverActualType = fci.methodReceiverType();
    for (size_t i = start; i < candidates.size(); i++) {
      forwardingInfo.push_back(forwardingReceiverActualType);
    }
  }

  // Move the contents of another container into this one, clearing out the
  // other.
  void takeFromOther(CandidatesAndForwardingInfo& other) {
    candidates.insert(candidates.end(),
                      std::make_move_iterator(other.candidates.begin()),
                      std::make_move_iterator(other.candidates.end()));
    forwardingInfo.insert(forwardingInfo.end(),
                          std::make_move_iterator(other.forwardingInfo.begin()),
                          std::make_move_iterator(other.forwardingInfo.end()));
    other.candidates.clear();
    other.forwardingInfo.clear();
  }

  // Get the candidate at the provided index with no bounds checking.
  inline const TypedFnSignature* get(size_t i) const { return candidates[i]; }

  // Get the forwarding info at the provided index.
  // Fails if there isn't forwarding info saved for each candidate.
  inline const types::QualifiedType& getForwardingInfo(size_t i) const {
    CHPL_ASSERT(candidates.size() == forwardingInfo.size());
    return forwardingInfo[i];
  }

  // Check if any candidates are present
  inline bool empty() const { return candidates.empty(); }

  // Get the number of candidates
  inline size_t size() const { return candidates.size(); }

  // Return true if this container stores any forwarding info
  inline bool hasForwardingInfo() const { return !forwardingInfo.empty(); }

  // Iterator over contained candidates
  const_iterator begin() const { return candidates.begin(); }
  const_iterator end() const { return candidates.end(); }

  /* Query system supporting functions */

  static bool update(CandidatesAndForwardingInfo& keep,
                     CandidatesAndForwardingInfo& addin) {
    return defaultUpdate(keep, addin);
  }
  size_t hash() const { return chpl::hash(candidates, forwardingInfo); }
  void mark(Context* context) const {
    chpl::mark<decltype(candidates)>{}(context, candidates);
    chpl::mark<decltype(forwardingInfo)>{}(context, forwardingInfo);
  }
  bool operator==(const CandidatesAndForwardingInfo& other) const {
    return candidates == other.candidates &&
           forwardingInfo == other.forwardingInfo;
  }
  void swap(CandidatesAndForwardingInfo& other) {
    std::swap(candidates, other.candidates);
    std::swap(forwardingInfo, other.forwardingInfo);
  }
  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;
};

/**
  An enum that represents the reason why a function candidate was filtered out
  during call resolution.
 */
enum CandidateFailureReason {
  /* Cannot pass an actual to one of the candidate's formals. */
  FAIL_CANNOT_PASS,
  /* Not a valid formal-actual mapping for this candidate. */
  FAIL_FORMAL_ACTUAL_MISMATCH,
  /* Special case of formal/actual mismatch when we tried to call a parallel iterator without a tag. */
  FAIL_FORMAL_ACTUAL_MISMATCH_ITERATOR_API,
  /* The wrong number of varargs were given to the function. */
  FAIL_VARARG_MISMATCH,
  /* The where clause returned 'false'. */
  FAIL_WHERE_CLAUSE,
  /* A parenful call to a parenless function or vice versa. */
  FAIL_PARENLESS_MISMATCH,
  /* An interface tried to resolve an associated type function but it didn't return a type */
  FAIL_INTERFACE_NOT_TYPE_INTENT,
  /* Some other, generic reason. */
  FAIL_CANDIDATE_OTHER,
};

/**
  An enum that represents the reason why an actual couldn't be passed
  to a formal.
 */
enum PassingFailureReason {
  /* Incompatible nilability (e.g. nilable to non-nilable formal) */
  FAIL_INCOMPATIBLE_NILABILITY,
  /* Incompatible management (e.g. borrowed to owned formal) */
  FAIL_INCOMPATIBLE_MGMT,
  /* Incompatible manager (e.g. owned to shared formal) */
  FAIL_INCOMPATIBLE_MGR,
  /* The actual is not a subtype of the formal, but had to be. */
  FAIL_EXPECTED_SUBTYPE,
  /* Tuple size doesn't match */
  FAIL_INCOMPATIBLE_TUPLE_SIZE,
  /* Tuple star-ness doesn't match */
  FAIL_INCOMPATIBLE_TUPLE_STAR,
  /* A conversion was needed but is not possible. */
  FAIL_CANNOT_CONVERT,
  /* An instantiation was needed but is not possible. */
  FAIL_CANNOT_INSTANTIATE,
  /* We had a generic formal, but the actual did not instantiate it; actual
     might be generic. */
  FAIL_DID_NOT_INSTANTIATE,
  /* A type was used as an argument to a value, or the other way around. */
  FAIL_TYPE_VS_NONTYPE,
  /* A param value was expected, but a non-param value was given. */
  FAIL_NOT_PARAM,
  /* One param value was expected, another was given. */
  FAIL_MISMATCHED_PARAM,
  /* An unestablished actual type was given to the function. */
  FAIL_UNKNOWN_ACTUAL_TYPE,
  /* An unestablished formal type in the function. */
  FAIL_UNKNOWN_FORMAL_TYPE,
  /* A generic type was given as an argument to a non-type formal. */
  FAIL_GENERIC_TO_NONTYPE,
  /* A type was expected to be the exact match of the formal, but wasn't. */
  FAIL_NOT_EXACT_MATCH,
  /* Some other, generic reason. */
  FAIL_FORMAL_OTHER,
};

/**
  Represents either a function that was accepted during call resolution, or
  the reason why that function was rejected.
 */
class ApplicabilityResult {
 private:
   using ErrorVariant = std::variant<ID, const UntypedFnSignature*, const TypedFnSignature*>;
  /*
    If the result was a success, this will be an empty ID. Otherwise, this
    will contain some information about the candidate that was rejected.
    IDs are included in untyped signatures and those are included in typed
    signatures, so the last case of this variant has the most information;
    we will try to include as much as possible depending on context.
   */
  ErrorVariant rejected_;
  /**
    If the ApplicabilityResult is a success, the function candidate that
    was accepted by call resolution.
   */
  const TypedFnSignature* candidate_;
  /**
    If the ApplicabilityResult is a failure, the reason why the candidate
    was rejected. Set to FAIL_CANDIDATE_OTHER on success as a placeholder.
   */
  CandidateFailureReason candidateReason_;
  /**
    If the ApplicabilityResult is a failure because we couldn't pass an actual
    to a formal, the reason why passing didn't work. Set to FAIL_FORMAL_OTHER
    on success as a placeholder.
   */
  PassingFailureReason formalReason_;
  /**
    If the ApplicabilityResult is a failure because we couldn't pass an actual
    to a formal, the index of the formal that we couldn't pass to. Set to -1
    on success as a placeholder.
   */
  int formalIdx_;

  ApplicabilityResult(ErrorVariant rejected,
                      const TypedFnSignature* candidate,
                      CandidateFailureReason candidateReason,
                      PassingFailureReason formalReason,
                      int formalIdx) :
    rejected_(std::move(rejected)), candidate_(candidate),
    candidateReason_(candidateReason), formalReason_(formalReason),
    formalIdx_(formalIdx) {
    CHPL_ASSERT(!candidate_ || (formalIdx_ == -1 &&
                                candidateReason_ == FAIL_CANDIDATE_OTHER &&
                                formalReason_ == FAIL_FORMAL_OTHER));
  }

 public:
  ApplicabilityResult()
    : ApplicabilityResult(ID(), nullptr, FAIL_CANDIDATE_OTHER, FAIL_FORMAL_OTHER, -1) {}

  static ApplicabilityResult success(const TypedFnSignature* candidate) {
    return ApplicabilityResult(ID(), candidate, FAIL_CANDIDATE_OTHER, FAIL_FORMAL_OTHER, -1);
  }

  static ApplicabilityResult failure(const TypedFnSignature* initialForErr,
                                     PassingFailureReason reason,
                                     int formalIdx) {
    return ApplicabilityResult(initialForErr, nullptr, FAIL_CANNOT_PASS, reason, formalIdx);
  }

  static ApplicabilityResult failure(const TypedFnSignature* rejected, CandidateFailureReason reason) {
    return ApplicabilityResult(rejected, nullptr, reason, FAIL_FORMAL_OTHER, -1);
  }

  static ApplicabilityResult failure(const UntypedFnSignature* rejected, CandidateFailureReason reason) {
    return ApplicabilityResult(rejected, nullptr, reason, FAIL_FORMAL_OTHER, -1);
  }

  static ApplicabilityResult failure(ID idForErr, CandidateFailureReason reason) {
    return ApplicabilityResult(std::move(idForErr), nullptr, reason, FAIL_FORMAL_OTHER, -1);
  }

  static bool update(ApplicabilityResult& keep, ApplicabilityResult& addin) {
    bool update = false;
    update |= defaultUpdateBasic(keep.rejected_, addin.rejected_);
    update |= defaultUpdateBasic(keep.candidate_, addin.candidate_);
    update |= defaultUpdateBasic(keep.candidateReason_, addin.candidateReason_);
    update |= defaultUpdateBasic(keep.formalReason_, addin.formalReason_);
    update |= defaultUpdateBasic(keep.formalIdx_, addin.formalIdx_);
    return update;
  }

  bool operator ==(const ApplicabilityResult& other) const {
    return rejected_ == other.rejected_ &&
           candidate_ == other.candidate_ &&
           candidateReason_ == other.candidateReason_ &&
           formalReason_ == other.formalReason_ &&
           formalIdx_ == other.formalIdx_;
  }

  bool operator !=(const ApplicabilityResult& other) const {
    return !(*this==other);
  }

  void mark(Context* context) const;

  size_t hash() const {
    return chpl::hash(rejected_, candidate_, candidateReason_, formalReason_, formalIdx_);
  }

  const ID& idForErr() const;

  const UntypedFnSignature* untypedForErr() const;

  const TypedFnSignature* initialForErr() const;

  const TypedFnSignature* candidate() const { return candidate_; }

  bool success() const { return candidate_ != nullptr; }

  CandidateFailureReason reason() const { return candidateReason_; }

  PassingFailureReason formalReason() const { return formalReason_; }

  int formalIdx() const { return formalIdx_; }

  inline bool failedDueToWrongActual() const {
    return candidateReason_ == resolution::FAIL_CANNOT_PASS &&
           formalReason_ != resolution::FAIL_UNKNOWN_FORMAL_TYPE;
  }
};

/** FormalActual holds information on a function formal and its binding (if any) */
class FormalActual {
 friend class FormalActualMap;
 private:
  types::QualifiedType formalType_;
  types::QualifiedType actualType_;
  const uast::Decl* formal_ = nullptr;
  int formalIdx_ = -1;
  int actualIdx_ = -1;
  bool hasActual_ = false; // == false means uses formal default value
  bool formalInstantiated_ = false;
  bool hasDefault_ = false;
  bool isVarArgEntry_ = false;

 public:
  bool operator==(const FormalActual& other) const {
    return formalType_ == other.formalType_ &&
           actualType_ == other.actualType_ &&
           formal_ == other.formal_ &&
           formalIdx_ == other.formalIdx_ &&
           actualIdx_ == other.actualIdx_ &&
           hasActual_ == other.hasActual_ &&
           formalInstantiated_ == other.formalInstantiated_ &&
           hasDefault_ == other.hasDefault_ &&
           isVarArgEntry_ == other.isVarArgEntry_;
  }
  bool operator!=(const FormalActual& other) const {
    return !(*this == other);
  }

  void mark(Context* context) const {
    formalType_.mark(context);
    actualType_.mark(context);
    context->markPointer(formal_);
    (void) formalIdx_; // nothing to mark
    (void) actualIdx_; // nothing to mark
    (void) hasActual_; // nothing to mark
    (void) formalInstantiated_; // nothing to mark
    (void) hasDefault_; // nothing to mark
    (void) isVarArgEntry_; // nothing to mark
  }

  size_t hash() const {
    return chpl::hash(formalType_, actualType_, formal_, formalIdx_, actualIdx_,
                      hasActual_, formalInstantiated_, hasDefault_, isVarArgEntry_);
  }

  const types::QualifiedType& formalType() const { return formalType_; }
  const types::QualifiedType& actualType() const { return actualType_; }
  const uast::Decl* formal() const { return formal_; }
  int formalIdx() const { return formalIdx_; }
  int actualIdx() const { return actualIdx_; }
  bool hasActual() const { return hasActual_; }
  bool formalInstantiated() const { return formalInstantiated_; }
  bool hasDefault() const { return hasDefault_; }
  bool isVarArgEntry() const { return isVarArgEntry_; }
};

/** FormalActualMap maps formals to actuals */
class FormalActualMap {
 private:
  std::vector<FormalActual> byFormalIdx_;
  std::vector<int> actualIdxToFormalIdx_;
  bool mappingIsValid_ = false;
  int failingActualIdx_ = -1;
  int failingFormalIdx_ = -1;

  // A standalone iterator will have an extra formal for the iterKind.
  // If we call foo() and fail to get a formal-actual mapping, but only
  // because we're missing the iterKind, we should still come back to it.
  bool missingIteratorActuals_ = false;

 public:

  using FormalActualIterable = Iterable<std::vector<FormalActual>>;

  FormalActualMap(const UntypedFnSignature* sig, const CallInfo& call) {
    mappingIsValid_ = computeAlignment(sig, nullptr, call);
  }
  FormalActualMap(const TypedFnSignature* sig, const CallInfo& call) {
    mappingIsValid_ = computeAlignment(sig->untyped(), sig, call);
  }

  bool operator==(const FormalActualMap& other) const {
    return byFormalIdx_ == other.byFormalIdx_ &&
           actualIdxToFormalIdx_ == other.actualIdxToFormalIdx_ &&
           mappingIsValid_ == other.mappingIsValid_ &&
           failingActualIdx_ == other.failingActualIdx_ &&
           failingFormalIdx_ == other.failingFormalIdx_ &&
           missingIteratorActuals_ == other.missingIteratorActuals_;
  }

  bool operator!=(const FormalActualMap& other) const {
    return !(*this == other);
  }

  void mark(Context* context) const {
    for (auto const &elt : byFormalIdx_) {
      elt.mark(context);
    }
    (void) actualIdxToFormalIdx_; // nothing to mark
    (void) mappingIsValid_; // nothing to mark
    (void) failingActualIdx_; // nothing to mark
    (void) failingFormalIdx_; // nothing to mark
    (void) missingIteratorActuals_; // nothing to mark
  }

  size_t hash() const {
    return chpl::hash(byFormalIdx_, actualIdxToFormalIdx_, mappingIsValid_,
                      failingActualIdx_, failingFormalIdx_,
                      missingIteratorActuals_);
  }

  /** check if mapping is valid */
  bool isValid() const { return mappingIsValid_; }

  /** Check why the mapping was invalid. */
  CandidateFailureReason reason() const {
    CHPL_ASSERT(!mappingIsValid_);
    if (missingIteratorActuals_) {
      return FAIL_FORMAL_ACTUAL_MISMATCH_ITERATOR_API;
    }
    return FAIL_FORMAL_ACTUAL_MISMATCH;
  }

  /** get the FormalActuals in the order of the formal arguments */
  FormalActualIterable byFormals() const {
    return FormalActualIterable(byFormalIdx_);
  }

  /** get the FormalActual for a particular formal index */
  const FormalActual& byFormalIdx(int formalIdx) const {
    CHPL_ASSERT(0 <= formalIdx && (size_t) formalIdx < byFormalIdx_.size());
    return byFormalIdx_[formalIdx];
  }

  /** get the FormalActual for a particular actual index,
      and returns nullptr if none was found. */
  const FormalActual* byActualIdx(int actualIdx) const {
    if (actualIdx < 0 || (size_t) actualIdx >= actualIdxToFormalIdx_.size())
      return nullptr;
    int formalIdx = actualIdxToFormalIdx_[actualIdx];
    if (formalIdx < 0 || (size_t) formalIdx >= byFormalIdx_.size())
      return nullptr;
    return &byFormalIdx_[formalIdx];
  }

  int failingFormalIdx() const { return failingFormalIdx_; }

  int failingActualIdx() const { return failingActualIdx_; }

  /* if we resolved the body of an initializer and got a more specific type
     for the receiver, update the receiver type in the map */
  void updateReceiverType(const TypedFnSignature* initializer) {
    CHPL_ASSERT(initializer->isInitializer());
    CHPL_ASSERT(byFormalIdx_[0].formal()->isNamedDecl());
    CHPL_ASSERT(byFormalIdx_[0].formal()->toNamedDecl()->name() == USTR("this"));
    byFormalIdx_[0].formalType_ = initializer->formalType(0);
  }

  /** Return the number of formals in this mapping. The number of actuals
      may be more (e.g., for a 'varargs') or less (e.g., default-argument
      values) than the number of formals, but there will always be as many
      entries in this mapping as are needed to invoke the given call. This
      quantity may be useful for consumers of this 'FormalActualMap'. */
  int numFormalsMapped() const { return byFormalIdx_.size(); }

 private:
  bool computeAlignment(const UntypedFnSignature* untyped,
                        const TypedFnSignature* typed, const CallInfo& call);
};


/**
  A map from a formal index (represented by `int` to match formalIdk field
  in FormalActual) to a qualified type representing the scalar type.
  This is not represented using a `SubstitutionsMap` because for some
  compiler-generated functions, we don't have formal IDs / decls.
 */
using PromotedFormalMap = std::unordered_map<int, types::QualifiedType>;

size_t hashPromotedFormalMap(const PromotedFormalMap& map);

/**
  Stores a function candidate. This information includes both the
  TypedFnSignature* representing the candidate, as well as information
  about this candidate's applicability. In particular, we store whether or
  not this candidate requires a const ref coercion, which is allowed by
  the spec, but is not allowed in practice due to the aliasing rules of C.
  */
class MostSpecificCandidate {
 private:
  friend class MostSpecificCandidates;

  const TypedFnSignature* fn_;
  owned<FormalActualMap> faMap_;
  owned<PromotedFormalMap> promotedFormals_;
  int constRefCoercionFormal_;
  int constRefCoercionActual_;

  MostSpecificCandidate(const TypedFnSignature* fn,
                        FormalActualMap faMap,
                        PromotedFormalMap promotedFormals,
                        int constRefCoercionFormal,
                        int constRefCoercionActual)
    : fn_(fn), faMap_(new FormalActualMap(std::move(faMap))),
      promotedFormals_(new PromotedFormalMap(std::move(promotedFormals))),
      constRefCoercionFormal_(constRefCoercionFormal),
      constRefCoercionActual_(constRefCoercionActual) {}

 public:
  MostSpecificCandidate()
    : fn_(nullptr), faMap_(), promotedFormals_(),
      constRefCoercionFormal_(-1),
      constRefCoercionActual_(-1) {}

  MostSpecificCandidate& operator=(MostSpecificCandidate&& other) = default;
  MostSpecificCandidate& operator=(const MostSpecificCandidate& other) {
    fn_ = other.fn_;
    if (other.faMap_) {
      faMap_ = toOwned(new FormalActualMap(*other.faMap_));
    }
    if (other.promotedFormals_) {
      promotedFormals_ = toOwned(new PromotedFormalMap(*other.promotedFormals_));
    }
    constRefCoercionFormal_ = other.constRefCoercionFormal_;
    constRefCoercionActual_ = other.constRefCoercionActual_;
    return *this;
  }

  MostSpecificCandidate(MostSpecificCandidate&& other) = default;
  MostSpecificCandidate(const MostSpecificCandidate& other) { *this = other; }

  static MostSpecificCandidate fromTypedFnSignature(ResolutionContext* rc,
                                        const TypedFnSignature* fn,
                                        const FormalActualMap& faMap,
                                        const Scope* scope,
                                        const PoiScope* poiScope,
                                        const PromotedFormalMap& promotedFormals);

  static MostSpecificCandidate fromTypedFnSignature(ResolutionContext* rc,
                                        const TypedFnSignature* fn,
                                        const CallInfo& info,
                                        const Scope* scope,
                                        const PoiScope* poiScope,
                                        const PromotedFormalMap& promotedFormals);

  const TypedFnSignature* fn() const { return fn_; }

  const FormalActualMap& formalActualMap() const { return *faMap_; }

  const PromotedFormalMap& promotedFormals() const { return *promotedFormals_; }

  int constRefCoercionFormal() const { return constRefCoercionFormal_; }

  int constRefCoercionActual() const { return constRefCoercionActual_; }

  bool hasConstRefCoercion() const { return constRefCoercionFormal_ != -1; }

  operator bool() const {
    CHPL_ASSERT(fn_ || (constRefCoercionFormal_ == -1 && constRefCoercionActual_ == -1));
    return fn_ != nullptr;
  }

  bool operator==(const MostSpecificCandidate& other) const {
    bool faMapsEqual = faMap_ == other.faMap_;
    if (!faMapsEqual && faMap_ && other.faMap_) {
      // Try a deep comparison to avoid unnecessary cache invalidation when
      // bumping generations. The pointers may differ, but the contained
      // maps can be equivalent.
      faMapsEqual = *faMap_ == *other.faMap_;
    }

    bool promotedFormalsEqual = promotedFormals_ == other.promotedFormals_;
    if (!promotedFormalsEqual && promotedFormals_ && other.promotedFormals_) {
      // See above comment.
      promotedFormalsEqual = *promotedFormals_ == *other.promotedFormals_;
    }

    return fn_ == other.fn_ &&
           faMapsEqual &&
           constRefCoercionFormal_ == other.constRefCoercionFormal_ &&
           constRefCoercionActual_ == other.constRefCoercionActual_;
  }

  bool operator!=(const MostSpecificCandidate& other) const {
    return !(*this == other);
  }

  void mark(Context* context) const {
    context->markPointer(fn_);
    if (faMap_) faMap_->mark(context);
    if (promotedFormals_) {
      chpl::mark<PromotedFormalMap>{}(context, *promotedFormals_);
    }
    (void) constRefCoercionFormal_; // nothing to mark
    (void) constRefCoercionActual_; // nothing to mark
  }

  size_t hash() const {
    return chpl::hash(fn_, faMap_, promotedFormals_, constRefCoercionFormal_, constRefCoercionActual_);
  }

  static bool update(MostSpecificCandidate& keep,
                     MostSpecificCandidate& addin) {
    return defaultUpdate(keep, addin);
  }

  void swap(MostSpecificCandidate& other) {
    std::swap(fn_, other.fn_);
    std::swap(faMap_, other.faMap_);
    std::swap(promotedFormals_, other.promotedFormals_);
    std::swap(constRefCoercionFormal_, other.constRefCoercionFormal_);
    std::swap(constRefCoercionActual_, other.constRefCoercionActual_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;
};

/**
  Stores the most specific candidates when resolving a function call.
*/
class MostSpecificCandidates {
 public:
  typedef enum {
    // the slots in the candidates array for return intent
    // overloading
    ONLY = 0, // same slot as REF
    REF = 0,
    CONST_REF,
    VALUE,
    // NUM_INTENTS is the size of the candidates array
    NUM_INTENTS,
  } Intent;

 private:
  MostSpecificCandidate candidates[NUM_INTENTS] = {MostSpecificCandidate()};
  bool emptyDueToAmbiguity = false;

 public:
  /**
    Default-initialize MostSpecificCandidates with no candidates
    which is not empty due to ambiguity.
   */
  MostSpecificCandidates() { }

  /**
    If fn is not nullptr, creates a MostSpecificCandidates with that function.
    Otherwise, default-initializes a MostSpecificCandidates with no candidates
    that is not empty due to ambiguity.
   */
  static MostSpecificCandidates getOnly(MostSpecificCandidate c) {
    MostSpecificCandidates ret;
    if (c.fn() != nullptr)
      ret.setBestOnly(std::move(c));
    return ret;
  }

  /**
    Creates a MostSpecificCandidates with no candidates that is not
    empty due to ambiguity.
   */
  static MostSpecificCandidates getEmpty() {
    return MostSpecificCandidates();
  }

  /**
    Creates a MostSpecificCandidates that represents a no candidates
    but that is empty due to ambiguity.
   */
  static MostSpecificCandidates getAmbiguous() {
    MostSpecificCandidates ret;
    ret.emptyDueToAmbiguity = true;
    return ret;
  }

  /**
    Adjust each candidate signature by inferring generic 'out' intent formals
    if there are any.
   */
  void inferOutFormals(ResolutionContext* rc,
                       const PoiScope* instantiationPoiScope);

  MostSpecificCandidate const* begin() const {
    return &candidates[0];
  }
  MostSpecificCandidate const* end() const {
    return &candidates[NUM_INTENTS];
  }

  void setBestRef(MostSpecificCandidate c) {
    candidates[REF] = std::move(c);
  }
  void setBestConstRef(MostSpecificCandidate c) {
    candidates[CONST_REF] = std::move(c);
  }
  void setBestValue(MostSpecificCandidate c) {
    candidates[VALUE] = std::move(c);
  }

  void setBestOnly(MostSpecificCandidate c) {
    candidates[ONLY] = std::move(c);
  }

  const MostSpecificCandidate& bestRef() const {
    return candidates[REF];
  }
  const MostSpecificCandidate& bestConstRef() const {
    return candidates[CONST_REF];
  }
  const MostSpecificCandidate& bestValue() const {
    return candidates[VALUE];
  }

  /**
    If there is exactly one candidate, return that candidate.
    Otherwise, return an empty candidate.
   */
  const MostSpecificCandidate& only() const {
    static MostSpecificCandidate empty;

    const MostSpecificCandidate* ret = nullptr;
    int nPresent = 0;
    for (const MostSpecificCandidate& sig : *this) {
      if (sig.fn() != nullptr) {
        ret = &sig;
        nPresent++;
      }
    }
    if (nPresent != 1) {
      return empty;
    }

    // if there is only one candidate, it should be in slot ONLY
    CHPL_ASSERT(candidates[ONLY] == *ret);
    return *ret;
  }

  /**
   Returns the number of best candidates that are contained here.
   */
  int numBest() const {
    int ret = 0;
    for (const MostSpecificCandidate& sig : *this) {
      if (sig.fn() != nullptr) {
        ret++;
      }
    }
    return ret;
  }

  /**
    Returns true if there are no most specific candidates.
   */
  bool isEmpty() const {
    return numBest() == 0;
  }

  /**
    Returns true if there are no most specific candidates due to ambiguity.
   */
  bool isAmbiguous() const {
    // if emptyDueToAmbiguity is set, isEmpty should return true
    CHPL_ASSERT(!emptyDueToAmbiguity || isEmpty());

    return emptyDueToAmbiguity;
  }

  /**
    Returns 'true' if any candidate was found, including
    if there was no best candidate due to ambiguity.
   */
  bool foundCandidates() const {
    return isAmbiguous() || !isEmpty();
  }

  bool operator==(const MostSpecificCandidates& other) const {
    for (int i = 0; i < NUM_INTENTS; i++) {
      if (candidates[i] != other.candidates[i])
        return false;
    }

    if (emptyDueToAmbiguity != other.emptyDueToAmbiguity)
      return false;

    return true;
  }
  bool operator!=(const MostSpecificCandidates& other) const {
    return !(*this == other);
  }
  void swap(MostSpecificCandidates& other) {
    for (int i = 0; i < NUM_INTENTS; i++) {
      std::swap(candidates[i], other.candidates[i]);
    }
    std::swap(emptyDueToAmbiguity, other.emptyDueToAmbiguity);
  }
  static bool update(MostSpecificCandidates& keep,
                     MostSpecificCandidates& addin) {
    return defaultUpdate(keep, addin);
  }
  void mark(Context* context) const {
    for (const MostSpecificCandidate& sig : candidates) {
      sig.mark(context);
    }
  }
  size_t hash() const {
    size_t hash = 0;
    for (const MostSpecificCandidate& sig : candidates) {
      hash = chpl::hash(hash, sig);
    }
    return chpl::hash(hash, emptyDueToAmbiguity);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/** CallResolutionResult */
class CallResolutionResult {
 private:
  // what are the candidates for return-intent overloading?
  MostSpecificCandidates mostSpecific_;
  // what is the type of the call expression?
  types::QualifiedType exprType_;
  // if the called expression is an iterator, what is the yielded type?
  types::QualifiedType yieldedType_;
  // if any of the candidates were instantiated, what point-of-instantiation
  // scopes were used when resolving their signature or body?
  PoiInfo poiInfo_;
  // whether the resolution result was handled using some compiler-level logic,
  // which does not correspond to a TypedSignature or AST.
  bool speciallyHandled_ = false;
  bool rejectedPossibleIteratorCandidates_ = false;

 public:
  CallResolutionResult() {}

  // for simple cases where mostSpecific and poiInfo are irrelevant.
  // Since the result was handled using some compiler-level logic (hence no
  // 'mostSpecific'), the result is marked as specially handled.
  CallResolutionResult(types::QualifiedType exprType,
                       types::QualifiedType yieldedType = types::QualifiedType())
    : exprType_(std::move(exprType)),
      yieldedType_(std::move(yieldedType)),
      speciallyHandled_(true) {
  }

  CallResolutionResult(MostSpecificCandidates mostSpecific,
                       bool rejectedPossibleIteratorCandidates,
                       types::QualifiedType exprType,
                       types::QualifiedType yieldedType,
                       PoiInfo poiInfo,
                       bool speciallyHandled = false)
    : mostSpecific_(std::move(mostSpecific)),
      exprType_(std::move(exprType)),
      yieldedType_(std::move(yieldedType)),
      poiInfo_(std::move(poiInfo)),
      speciallyHandled_(speciallyHandled),
      rejectedPossibleIteratorCandidates_(rejectedPossibleIteratorCandidates)
  {
  }

  static CallResolutionResult getEmpty() {
    return CallResolutionResult();
  }

  /** get the most specific candidates for return-intent overloading */
  const MostSpecificCandidates& mostSpecific() const { return mostSpecific_; }

  /** type of the call expression */
  const types::QualifiedType& exprType() const { return exprType_; }

  /** type of the yielded expression, if the call expression is an iterator */
  const types::QualifiedType& yieldedType() const { return yieldedType_; }

  /** point-of-instantiation scopes used when resolving signature or body */
  const PoiInfo& poiInfo() const { return poiInfo_; }

  /** whether the resolution result was handled using some compiler-level logic */
  bool speciallyHandled() const { return speciallyHandled_; }

  /** whether we rejected candidates because they expected a tag or followThis.
      This might indicate that we need to re-resolve with tag to find parallel
      iterators. */
  bool rejectedPossibleIteratorCandidates() const {
    return rejectedPossibleIteratorCandidates_;
  }

  bool operator==(const CallResolutionResult& other) const {
    return mostSpecific_ == other.mostSpecific_ &&
           exprType_ == other.exprType_ &&
           yieldedType_ == other.yieldedType_ &&
           PoiInfo::updateEquals(poiInfo_, other.poiInfo_) &&
           speciallyHandled_ == other.speciallyHandled_ &&
           rejectedPossibleIteratorCandidates_ == other.rejectedPossibleIteratorCandidates_;
  }
  bool operator!=(const CallResolutionResult& other) const {
    return !(*this == other);
  }

  void mark(Context* context) const {
    mostSpecific_.mark(context);
    exprType_.mark(context);
    yieldedType_.mark(context);
    poiInfo_.mark(context);
  }

  static bool update(CallResolutionResult& keep,
                     CallResolutionResult& addin) {
    return defaultUpdate(keep, addin);
  }

  size_t hash() const {
    return chpl::hash(mostSpecific_, exprType_, yieldedType_, poiInfo_,
                      speciallyHandled_, rejectedPossibleIteratorCandidates_);
  }

  void swap(CallResolutionResult& other) {
    mostSpecific_.swap(other.mostSpecific_);
    exprType_.swap(other.exprType_);
    yieldedType_.swap(other.yieldedType_);
    poiInfo_.swap(other.poiInfo_);
    std::swap(speciallyHandled_, other.speciallyHandled_);
    std::swap(rejectedPossibleIteratorCandidates_,
              other.rejectedPossibleIteratorCandidates_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/* The result of resolving a 'these' call. This contains more information
   than a general call resolution result, because we can encode what threw off
   the iteration (e.g., lack of a follower iterator)

   In the case of failure due to zippering (e.g., we're iterating over a
   loop expression, but the Nth zippered iterand doesn't have the appropriate
   iterator), this type contains a 'zipperedFailure_' field, which contains
   the result of trying to iterate the Nth iterand, and a 'zipperedFailureIndex_'
   which is the index of the failed iterand.
*/
struct TheseResolutionResult {
 public:
  enum FailureReason {
    /* We didn't fail, finding an iterator. */
    THESE_SUCCESS = 0,
    /* We didn't attempt to resolve this iterator, possibly because it's
       not supported in this context. */
    THESE_NOT_ATTEMPTED,
    /* We tried to resolve a standalone iterator on a loop expression, but
       loop expressions don't have standalone iterators. */
    THESE_FAIL_NO_LOOP_EXPR_STANDALONE,
    /* We tried to resolve a standalone iterator on a promoted expression, but
       promoted expressions don't have standalone iterators. */
    THESE_FAIL_NO_PROMO_STANDALONE,
    /* We tried to resolve a parallel iterator but we're iterating over a serial
       loop expression. */
    THESE_FAIL_SERIAL_LOOP_EXPR,
    /* We couldn't find the given iterator's definition. */
    THESE_FAIL_NO_ITERATOR_WITH_TAG,
    /* We found a leader and a follower, but the leader's yield type doesn't match
       the follower's 'followThis'. */
    THESE_FAIL_LEADER_FOLLOWER_MISMATCH,
    /* We tried to resolve a zippered-like iterator (zippered loop expression,
       promoted expressions) and one of the iterables that makes it up failed
       to resolve. In this case, the zipperedFailure_ and zipperedFailureIndex_
       fields are set. */
    THESE_FAIL_ZIPPERED_ARG_FAILED,
    /* Mismatch between the claimed promotion type and the type yielded by the
       iterator. */
    THESE_FAIL_PROMOTION_TYPE_YIELD_MISMATCH,
  };

 private:
  // The reason that we couldn't resolve the 'these' iterator in this case.
  FailureReason reason_;
  // If a call was attempted, the result of the call. Stored as a pointer
  // to avoid the additional memory overhead of storing an empty CallResolutionResult.
  std::unique_ptr<CallResolutionResult> callResult_;
  // If the failure was due to one the zippered arguments failing to resolve,
  // the failure that threw off the zippered argument.
  std::unique_ptr<TheseResolutionResult> zipperedFailure_ = nullptr;
  // If the failure was due to one of the zippered arguments failing to resolve,
  // the index of the failed argument.
  int zipperedFailureIndex_ = -1;
  // The type of the iterand that we were trying to resolve.
  types::QualifiedType iterandType_;

  TheseResolutionResult(FailureReason reason,
                        std::unique_ptr<CallResolutionResult> callResult,
                        std::unique_ptr<TheseResolutionResult> zipperedResult,
                        int zipperedResultIndex,
                        types::QualifiedType iterandType)
    : reason_(reason),
      callResult_(std::move(callResult)),
      zipperedFailure_(std::move(zipperedResult)),
      zipperedFailureIndex_(zipperedResultIndex),
      iterandType_(std::move(iterandType)) {}

 public:
  TheseResolutionResult() : reason_(THESE_NOT_ATTEMPTED) {}
  TheseResolutionResult(TheseResolutionResult&& other) = default;
  TheseResolutionResult& operator=(TheseResolutionResult&& other) = default;

  // Needed because we copy error messages, which can contain 'TheseResolutionResults'.
  TheseResolutionResult(const TheseResolutionResult& other)
    : reason_(other.reason_),
      iterandType_(other.iterandType_) {
    if (other.zipperedFailure_) {
      zipperedFailure_ = toOwned(new TheseResolutionResult(*other.zipperedFailure_));
    }
    if (other.callResult_) {
      callResult_ = toOwned(new CallResolutionResult(*other.callResult_));
    }
  }

  /** Construct a TheseResolutionResult for a successful call to 'these' on
      a given iterand type. */
  static TheseResolutionResult success(CallResolutionResult callResult,
                                       types::QualifiedType iterandType) {
    CHPL_ASSERT(!callResult.exprType().isUnknownOrErroneous());
    auto callResultPtr =
      toOwned(new CallResolutionResult(std::move(callResult)));
    return TheseResolutionResult(THESE_SUCCESS, std::move(callResultPtr),
                                 nullptr, -1, std::move(iterandType));
  }

  /** Construct a successful TheseResolutionResult without any underlying call. */
  static TheseResolutionResult success(types::QualifiedType iterandType) {
    return TheseResolutionResult(THESE_SUCCESS, nullptr,
                                 nullptr, -1, std::move(iterandType));
  }

  /** Construct a TheseResolutionResult failed call to 'these' on a given
      iterand type, with 'reason' as the reason for failure. */
  static TheseResolutionResult failure(FailureReason reason,
                                       types::QualifiedType iterandType) {
    return TheseResolutionResult(reason, nullptr, nullptr, -1,
                                 std::move(iterandType));
  }

  /** Same as the previous overload, but include the failed call resolution result. */
  static TheseResolutionResult failure(FailureReason reason,
                                       types::QualifiedType iterandType,
                                       CallResolutionResult callResult) {
    auto callResultPtr =
      toOwned(new CallResolutionResult(std::move(callResult)));
    return TheseResolutionResult(reason, std::move(callResultPtr),
                                 nullptr, -1, std::move(iterandType));
  }

  /** Construct a TheseResolutionResult for a failed call to 'these' on a given
      iterand type, caused by failing to resolve 'these' for a zippered argument
      at a given index. */
  static TheseResolutionResult failure(std::unique_ptr<TheseResolutionResult>
                                       zipperedFailure, int zipperedResultIndex,
                                       types::QualifiedType iterandType) {
    return TheseResolutionResult(THESE_FAIL_ZIPPERED_ARG_FAILED, nullptr,
                                 std::move(zipperedFailure),
                                 zipperedResultIndex,
                                 std::move(iterandType));
  }

  /** Checks whether the resolution was successful. */
  operator bool() const { return reason_ == THESE_SUCCESS; }

  bool operator==(const TheseResolutionResult& other) const {
    bool callResultEqual = this->callResult_ == other.callResult_;
    if (!callResultEqual && this->callResult_ && other.callResult_) {
      callResultEqual = *this->callResult_ == *other.callResult_;
    }

    bool zipperedFailureEqual = this->zipperedFailure_ == other.zipperedFailure_;
    if (!zipperedFailureEqual && this->zipperedFailure_ && other.zipperedFailure_) {
      zipperedFailureEqual = *this->zipperedFailure_ == *other.zipperedFailure_;
    }

    return reason_ == other.reason_ &&
           callResultEqual &&
           zipperedFailureEqual &&
           zipperedFailureIndex_ == other.zipperedFailureIndex_ &&
           iterandType_ == other.iterandType_;
  }

  bool operator!=(const TheseResolutionResult& other) const {
    return !(*this == other);
  }

  void mark(Context* context) const {
    (void) reason_; // nothing to mark
    if (callResult_) callResult_->mark(context);
    if (zipperedFailure_) zipperedFailure_->mark(context);
    iterandType_.mark(context);
  }

  size_t hash() const {
    return chpl::hash(reason_, callResult_, zipperedFailure_,
                      zipperedFailureIndex_, iterandType_);
  }

  /** Returns the reason for failure. */
  FailureReason reason() const { return reason_; }

  /** Returns the result of resolving a 'these' call, if it was attempted. */
  const CallResolutionResult* callResult() const { return callResult_.get(); }

  /** Returns the result of resolving a 'these' call on a zippered argument,
      if it was attempted. */
  const TheseResolutionResult* zipperedFailure() const { return zipperedFailure_.get(); }

  /** Returns the index of the failed zippered argument, if the failure was due
      to a zippered argument failing to resolve. Otherwise, returns -1. */
  int zipperedFailureIndex() const { return zipperedFailureIndex_; }

  /** Returns the type of the iterand that we were trying to resolve. */
  const types::QualifiedType& iterandType() const { return iterandType_; }

  /** Returns the type of resolving the 'these' call. */
  types::QualifiedType exprType() const {
    if (callResult_) return callResult_->exprType();
    return types::QualifiedType();
  }

  /** Returns the yield type from resolving the 'these' call. */
  types::QualifiedType yieldedType() const {
    if (callResult_) return callResult_->yieldedType();
    return types::QualifiedType();
  }
};

/**

  When resolving calls like f(), we need three scopes to search.
  * The 'call scope', which becomes relevant if we're resolving a generic function.
    When we resolve a generic function, and come across other calls,
    this 'call scope' becomes the 'poi scope' for resolving those dependent calls.
  * The 'lookup scope', which is used to restrict where we search for candidates.
    For instance, when resolving `M.f()`, we don't want to look for `f` in
    the current scope, only in the scope of `M`. The call scope is not
    always the same as the 'lookup scope' because while resolving `M.f`,
    we still want to use the 'call scope' for POI.
  * The 'POI scope', which is used when resolving calls in generic functions
    as described in the first bullet.

  This data structure bundles all three scopes for convenient threading through
  the call resolution process.
 */
class CallScopeInfo {
 private:
  const Scope* callScope_;
  const Scope* lookupScope_;
  const PoiScope* poiScope_;

  CallScopeInfo(const Scope* callScope, const Scope* lookupScope, const PoiScope* poiScope)
    : callScope_(callScope), lookupScope_(lookupScope), poiScope_(poiScope) {
  }

 public:
  static CallScopeInfo forNormalCall(const Scope* scope, const PoiScope* poiScope);
  static CallScopeInfo forQualifiedCall(Context* context, const ID& moduleId,
                                        const Scope* scope, const PoiScope* poiScope);
  static CallScopeInfo forIteratorOverloadSearch(const Scope* callScope,
                                                 const Scope* lookupScope,
                                                 const PoiScope* poiScope);

  const Scope* callScope() const { return callScope_; }
  const Scope* lookupScope() const { return lookupScope_; }
  const PoiScope* poiScope() const { return poiScope_; }
};

/**
  This type represents an associated action (for use within a
  ResolvedExpression).
 */
class AssociatedAction {
 public:
  enum Action {
    ASSIGN,           // same type or different type assign
    COPY_INIT,        // init= from same type
    INIT_OTHER,       // init= from other type
    CUSTOM_COPY_INIT, // chpl__copyInit for specialized behavior
    DEFAULT_INIT,
    DEINIT,
    ITERATE,          // aka "these"
    NEW_INIT,
    REDUCE_SCAN,      // resolution of "generate" for a reduce/scan operation.
    REDUCE_ASSIGN,    // resolution of "accumulateOntoState" for a reduce= operation.
    INFER_TYPE,
    COMPARE,          // == , e.g., for select-statements
    RUNTIME_TYPE,     // create runtime type
    ENTER_CONTEXT,
    EXIT_CONTEXT,
  };

 private:
  Action action_;
  const TypedFnSignature* fn_;
  ID id_;
  types::QualifiedType type_;

 public:
  AssociatedAction(Action action, const TypedFnSignature* fn, ID id,
                   types::QualifiedType type)
    : action_(action), fn_(fn), id_(id), type_(type) {
  }
  bool operator==(const AssociatedAction& other) const {
    return action_ == other.action_ &&
           fn_ == other.fn_ &&
           id_ == other.id_ &&
           type_ == other.type_;
  }
  bool operator!=(const AssociatedAction& other) const {
    return !(*this == other);
  }
  /** Returns which action this represents */
  Action action() const { return action_; }

  /** Return which function is called to help with the action */
  const TypedFnSignature* fn() const { return fn_; }

  /** Return the ID is associated with the action */
  const ID& id() const { return id_; }

  const types::QualifiedType type() const { return type_; }

  void mark(Context* context) const {
    if (fn_ != nullptr) fn_->mark(context);
    id_.mark(context);
    type_.mark(context);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  static const char* kindToString(Action a);
};

class ResolvedParamLoop;

/**
  This type represents a resolved expression.
*/
class ResolvedExpression {
 private:
  // What is its type and param value?
  types::QualifiedType type_;
  // For simple (non-function Identifier) cases,
  // the ID of a NamedDecl it refers to
  ID toId_;
  // Is this a reference to a compiler-created primitive?
  bool isBuiltin_ = false;
  // Did this expression cause the function to stop being resolved?
  bool causedFatalError_ = false;

  // For a function call, what is the most specific candidate,
  // or when using return intent overloading, what are the most specific
  // candidates?
  // The choice between these needs to happen
  // later than the main function resolution.
  MostSpecificCandidates mostSpecific_;
  // What point of instantiation scope should be used when
  // resolving functions in mostSpecific?
  const PoiScope *poiScope_ = nullptr;

  // actions associated with this expression
  // (e.g. default init, copy init, deinit)
  std::vector<AssociatedAction> associatedActions_;

  const ResolvedParamLoop* paramLoop_ = nullptr;

 public:
  using AssociatedActions = std::vector<AssociatedAction>;

  ResolvedExpression() { }

  /** get the qualified type */
  const types::QualifiedType& type() const { return type_; }

  /** for simple (non-function Identifier) cases, the ID of a NamedDecl it
   * refers to */
  const ID& toId() const { return toId_; }

  /** check whether this resolution result refers to a compiler builtin like `bool`. */
  bool isBuiltin() const { return isBuiltin_; }

  /** check whether this resolution result caused a fatal error. */
  bool causedFatalError() const { return causedFatalError_; }

  /** For a function call, what is the most specific candidate, or when using
   * return intent overloading, what are the most specific candidates? The
   * choice between these needs to happen later than the main function
   * resolution.
   */
  const MostSpecificCandidates& mostSpecific() const { return mostSpecific_; }

  const PoiScope* poiScope() const { return poiScope_; }

  bool hasAssociatedActions() const {
    return !associatedActions_.empty();
  }

  const AssociatedActions& associatedActions() const {
    return associatedActions_;
  }

  // TODO: Expected to be a placeholder as we look towards updating the
  // representation of associated actions.
  std::optional<AssociatedAction> getAction(AssociatedAction::Action action) const {
    // TODO: what if there are multiple instances of the same action?
    auto it = std::find_if(associatedActions_.begin(), associatedActions_.end(),
                [&](const AssociatedAction a) { return a.action() == action; });
    if (it != associatedActions_.end()) {
      return *it;
    } else {
      return {};
    }
  }

  const ResolvedParamLoop* paramLoop() const {
    return paramLoop_;
  }

  /** set the isPrimitive flag */
  void setIsBuiltin(bool isBuiltin) { isBuiltin_ = isBuiltin; }

  /** set the causedFatalError flag */
  void setCausedFatalError(bool causedFatalError) { causedFatalError_ = causedFatalError; }

  /** set the toId */
  void setToId(ID toId) { toId_ = toId; }

  /** set the qualified type */
  void setType(const types::QualifiedType& type) { type_ = type; }

  /** set the kind of the qualified type */
  void setKind(types::QualifiedType::Kind kind) {
    type_ = { kind, type_.type(), type_.param() };
  }

  /** set the most specific */
  void setMostSpecific(const MostSpecificCandidates& mostSpecific) {
    mostSpecific_ = mostSpecific;
  }

  /** set the point-of-instantiation scope */
  void setPoiScope(const PoiScope* poiScope) { poiScope_ = poiScope; }

  /** add an associated function */
  void addAssociatedAction(AssociatedAction::Action action,
                           const TypedFnSignature* fn,
                           ID id,
                           types::QualifiedType type) {
    associatedActions_.push_back(AssociatedAction(action, fn, id, type));
  }

  void setParamLoop(const ResolvedParamLoop* paramLoop) { paramLoop_ = paramLoop; }

  bool operator==(const ResolvedExpression& other) const {
    return type_ == other.type_ &&
           toId_ == other.toId_ &&
           isBuiltin_ == other.isBuiltin_ &&
           causedFatalError_ == other.causedFatalError_ &&
           mostSpecific_ == other.mostSpecific_ &&
           poiScope_ == other.poiScope_ &&
           associatedActions_ == other.associatedActions_ &&
           paramLoop_ == other.paramLoop_;
  }
  bool operator!=(const ResolvedExpression& other) const {
    return !(*this == other);
  }
  void swap(ResolvedExpression& other) {
    type_.swap(other.type_);
    toId_.swap(other.toId_);
    std::swap(isBuiltin_, other.isBuiltin_);
    std::swap(causedFatalError_, other.causedFatalError_);
    mostSpecific_.swap(other.mostSpecific_);
    std::swap(poiScope_, other.poiScope_);
    std::swap(associatedActions_, other.associatedActions_);
    std::swap(paramLoop_, other.paramLoop_);
  }
  static bool update(ResolvedExpression& keep, ResolvedExpression& addin) {
    return defaultUpdate(keep, addin);
  }
  void mark(Context* context) const {
    type_.mark(context);
    toId_.mark(context);
    mostSpecific_.mark(context);
    context->markPointer(poiScope_);
    for (const auto& a : associatedActions_) {
      a.mark(context);
    }
    context->markPointer(paramLoop_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/**
 This type is a mapping from postOrderId (which is an integer) to
 ResolvedExpression for storing resolution results *within* a symbol.

 Note that an inner Function would not be covered here.
 */
class ResolutionResultByPostorderID {
 private:
  ID symbolId_;
  // This map is generally accessed with operator[] to default-construct a new
  // ResolvedExpression if none exists for an ID. at() is used instead only
  // when const-ness is required.
  std::unordered_map<int, ResolvedExpression> map;

 public:
  /** prepare to resolve the contents of the passed symbol */
  void setupForSymbol(const uast::AstNode* ast);
  /** prepare to resolve the signature of the passed function */
  void setupForSignature(const uast::Function* func);
  /** prepare to resolve the body of the passed function */
  void setupForFunction(const uast::Function* func);
  /** prepare to resolve the body of a For loop */
  void setupForParamLoop(const uast::For* loop, ResolutionResultByPostorderID& parent);

  const ID& symbolId() const {
    return symbolId_;
  }

  /* ID query functions */
  bool hasId(const ID& id) const {
    auto postorder = id.postOrderId();
    if (id.symbolPath() == symbolId_.symbolPath() &&
        0 <= postorder && (map.count(postorder) > 0))
      return true;

    return false;
  }
  ResolvedExpression& byId(const ID& id) {
    auto postorder = id.postOrderId();
    return map[postorder];
  }
  const ResolvedExpression& byId(const ID& id) const {
    CHPL_ASSERT(hasId(id));
    auto postorder = id.postOrderId();
    return map.at(postorder);
  }
  const ResolvedExpression* byIdOrNull(const ID& id) const {
    if (hasId(id)) {
      auto postorder = id.postOrderId();
      return &map.at(postorder);
    }
    return nullptr;
  }

  /* AST query functions */
  bool hasAst(const uast::AstNode* ast) const {
    return ast != nullptr && hasId(ast->id());
  }
  ResolvedExpression& byAst(const uast::AstNode* ast) {
    return byId(ast->id());
  }
  const ResolvedExpression& byAst(const uast::AstNode* ast) const {
    return byId(ast->id());
  }
  const ResolvedExpression* byAstOrNull(const uast::AstNode* ast) const {
    return byIdOrNull(ast->id());
  }

  bool operator==(const ResolutionResultByPostorderID& other) const {
    return symbolId_ == other.symbolId_ &&
           map == other.map;
  }
  bool operator!=(const ResolutionResultByPostorderID& other) const {
    return !(*this == other);
  }
  void swap(ResolutionResultByPostorderID& other) {
    symbolId_.swap(other.symbolId_);
    map.swap(other.map);
  }
  static bool update(ResolutionResultByPostorderID& keep,
                     ResolutionResultByPostorderID& addin);
  void mark(Context* context) const {
    symbolId_.mark(context);
    for (auto const &elt : map) {
      // mark ResolvedExpressions
      elt.second.mark(context);
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  auto begin() const {
    return map.begin();
  }

  auto end() const {
    return map.end();
  }

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/**
  Represents a compiler diagnostic emit from Chapel code, e.g. via
  'compilerError' or 'compilerWarning'. These diagnostics can be emitted
  at various call stack depths (to hide invocations to intermediate utility
  functions, for example). As a result, this type stores the depth in
  addition to the diagnostic message.

  For the purposes of properly integrating into the query system and
  `runAndTrackErrors`, the compiler immediately issues an Error diagnostic,
  but doesn't print it. Otherwise, (e.g., if the error message requests
  a depth 10,000), the error may be encountered but never emitted.

  Error messages are presently not tracked through the query system; however,
  if they were, it would be interesting to include the Error* into this
  diagnostic and thereby reason about whether each (immediately) noted error
  message is printed.
 */
struct CompilerDiagnostic {
 public:
  enum Kind {
    ERROR, WARNING
  };
 private:
  UniqueString message_;
  Kind kind_;
  int64_t depth_;

 public:
  CompilerDiagnostic(UniqueString message, Kind kind, int64_t depth)
    : message_(std::move(message)), kind_(kind), depth_(depth) {}

  bool operator==(const CompilerDiagnostic& other) const {
    return message_ == other.message_ &&
           kind_ == other.kind_ &&
           depth_ == other.depth_;
  }

  bool operator!=(const CompilerDiagnostic& other) const {
    return !(*this == other);
  }

  void mark(Context* context) const {
    message_.mark(context);
  }

  size_t hash() const {
    return chpl::hash(message_, kind_, depth_);
  }

  UniqueString message() const { return message_; }

  Kind kind() const { return kind_; }

  int64_t depth() const { return depth_; }

  bool isError() const { return kind_ == ERROR; }

  bool isWarning() const { return kind_ == WARNING; }
};

/**
  This type represents a resolved function.

  When a function 'F' is resolved, any child functions that were resolved
  as a result of resolving the body of F will be stored in this type as
  well. For a generic child 'NF' of F, this does not mean that _all_
  instantiations of NF will be stored here (only instantiations made
  for interior calls issued within F).
*/
class ResolvedFunction {
 public:
  using PoiTrace = PoiInfo::Trace;
  using Child = owned<ResolvedFunction>;
  using ChildPtr = const ResolvedFunction*;
  using PoiTraceToChildMap = std::unordered_map<PoiTrace, Child>;
  using SigAndInfo = std::tuple<const TypedFnSignature*, PoiInfo>;
  using SigAndInfoToChildPtrMap = std::unordered_map<SigAndInfo, ChildPtr>;

 private:
  const TypedFnSignature* signature_ = nullptr;

  uast::Function::ReturnIntent returnIntent_ =
      uast::Function::DEFAULT_RETURN_INTENT;

  // this is the output of the resolution process
  ResolutionResultByPostorderID resolutionById_;

  // the set of point-of-instantiation scopes used by the instantiation
  PoiInfo poiInfo_;

  // the linkage name computed for this function
  UniqueString linkageName_;

  // the return type computed for this function
  types::QualifiedType returnType_;

  // diagnostics that callers of this function should emit
  std::vector<CompilerDiagnostic> diagnostics_;

  // These two maps attempt to mimick what is done to cache generic
  // instantiations in the queries 'resolveFunctionByInfo' and
  // 'resolveFunctionByPois'. The maps take the place of the query cache
  // (since it cannot be used to store the results of resolving a nested
  // function).
  PoiTraceToChildMap poiTraceToChild_;
  SigAndInfoToChildPtrMap sigAndInfoToChildPtr_;

 public:
  ResolvedFunction(const TypedFnSignature *signature,
                   uast::Function::ReturnIntent returnIntent,
                   ResolutionResultByPostorderID resolutionById,
                   PoiInfo poiInfo,
                   UniqueString linkageName,
                   types::QualifiedType returnType,
                   std::vector<CompilerDiagnostic> diagnostics,
                   PoiTraceToChildMap poiTraceToChild,
                   SigAndInfoToChildPtrMap sigAndInfoToChildPtr)
      : signature_(signature),
        returnIntent_(returnIntent),
        resolutionById_(std::move(resolutionById)),
        poiInfo_(std::move(poiInfo)),
        linkageName_(linkageName),
        returnType_(std::move(returnType)),
        diagnostics_(std::move(diagnostics)),
        poiTraceToChild_(std::move(poiTraceToChild)),
        sigAndInfoToChildPtr_(std::move(sigAndInfoToChildPtr)) {}
 ~ResolvedFunction() = default;
  ResolvedFunction(const ResolvedFunction& rhs) = delete;
  ResolvedFunction(ResolvedFunction&& rhs) = default;

  /** The type signature */
  const TypedFnSignature* signature() const { return signature_; }

  /** the return intent */
  uast::Function::ReturnIntent returnIntent() const { return returnIntent_; }

  /** the linkage name */
  UniqueString linkageName() const { return linkageName_; }

  /** the return type */
  const types::QualifiedType& returnType() const {
    return returnType_;
  }

  /** the diagnostic incurred by this function */
  const std::vector<CompilerDiagnostic>& diagnostics() const {
    return diagnostics_;
  }

  /** this is the output of the resolution process */
  const ResolutionResultByPostorderID& resolutionById() const {
    return resolutionById_;
  }

  /** the set of point-of-instantiations used by the instantiation */
  const PoiInfo& poiInfo() const { return poiInfo_; }

  const ResolvedFunction* getNestedResult(const TypedFnSignature* sig,
                                          const PoiScope* poiScope) const {
    auto poiInfo = PoiInfo(poiScope);
    auto it = sigAndInfoToChildPtr_.find(std::make_pair(sig, poiScope));
    if (it != sigAndInfoToChildPtr_.end()) {
      return it->second;
    } else {
      return nullptr;
    }
  }

  bool operator==(const ResolvedFunction& other) const {
    return signature_ == other.signature_ &&
           returnIntent_ == other.returnIntent_ &&
           resolutionById_ == other.resolutionById_ &&
           PoiInfo::updateEquals(poiInfo_, other.poiInfo_) &&
           linkageName_ == other.linkageName_ &&
           returnType_ == other.returnType_ &&
           diagnostics_ == other.diagnostics_ &&
           poiTraceToChild_ == other.poiTraceToChild_ &&
           sigAndInfoToChildPtr_ == other.sigAndInfoToChildPtr_;
  }
  bool operator!=(const ResolvedFunction& other) const {
    return !(*this == other);
  }

  void swap(ResolvedFunction& other) {
    std::swap(signature_, other.signature_);
    std::swap(returnIntent_, other.returnIntent_);
    resolutionById_.swap(other.resolutionById_);
    poiInfo_.swap(other.poiInfo_);
    linkageName_.swap(other.linkageName_);
    returnType_.swap(other.returnType_);
    std::swap(diagnostics_, other.diagnostics_);
    std::swap(poiTraceToChild_, other.poiTraceToChild_);
    std::swap(sigAndInfoToChildPtr_, other.sigAndInfoToChildPtr_);
  }
  static bool update(owned<ResolvedFunction>& keep,
                     owned<ResolvedFunction>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    context->markPointer(signature_);
    resolutionById_.mark(context);
    poiInfo_.mark(context);
    linkageName_.mark(context);
    returnType_.mark(context);
    chpl::mark<decltype(diagnostics_)>{}(context, diagnostics_);
    for (auto& p : poiTraceToChild_) {
      chpl::mark<decltype(p.first)>{}(context, p.first);
      context->markPointer(p.second);
    }
    for (auto& p : sigAndInfoToChildPtr_) {
      chpl::mark<decltype(p.first)>{}(context, p.first);
      context->markPointer(p.second);
    }
  }
  size_t hash() const {
    // Skip 'resolutionById_' since it can be quite large.
    std::ignore = resolutionById_;
    size_t ret = chpl::hash(signature_, returnIntent_, poiInfo_, linkageName_, returnType_, diagnostics_);
    for (auto& p : poiTraceToChild_) {
      ret = hash_combine(ret, chpl::hash(p.first));
      ret = hash_combine(ret, chpl::hash(p.second));
    }
    for (auto& p : sigAndInfoToChildPtr_) {
      ret = hash_combine(ret, chpl::hash(p.first));
      ret = hash_combine(ret, chpl::hash(p.second));
    }
    return ret;
  }

  const ResolvedExpression& byId(const ID& id) const {
    return resolutionById_.byId(id);
  }
  const ResolvedExpression& byAst(const uast::AstNode* ast) const {
    return resolutionById_.byAst(ast);
  }
  const ResolvedExpression* byAstOrNull(const uast::AstNode* ast) const {
    return resolutionById_.byAstOrNull(ast);
  }

  const ID& id() const {
    return signature_->id();
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/// \cond DO_NOT_DOCUMENT
struct FieldDetail {
  UniqueString name;
  bool hasDefaultValue = false;
  ID declId;
  types::QualifiedType type;

  FieldDetail(UniqueString name,
              bool hasDefaultValue,
              ID declId,
              types::QualifiedType type)
    : name(name), hasDefaultValue(hasDefaultValue), declId(declId), type(type) {
  }
  bool operator==(const FieldDetail& other) const {
    return name == other.name &&
           hasDefaultValue == other.hasDefaultValue &&
           declId == other.declId &&
           type == other.type;
  }
  bool operator!=(const FieldDetail& other) const {
    return !(*this == other);
  }
  void mark(Context* context) const {
    name.mark(context);
    declId.mark(context);
    type.mark(context);
  }
  size_t hash() const {
    return chpl::hash(name, hasDefaultValue, declId, type);
  }
};
/// \endcond DO_NOT_DOCUMENT

/// \cond DO_NOT_DOCUMENT
struct ForwardingDetail {
  ID forwardingStmt;
  types::QualifiedType receiverType;
  ForwardingDetail(ID forwardingStmt, types::QualifiedType receiverType)
   : forwardingStmt(std::move(forwardingStmt)),
     receiverType(std::move(receiverType)) {
  }
  bool operator==(const ForwardingDetail& other) const {
    return forwardingStmt == other.forwardingStmt &&
           receiverType == other.receiverType;
  }
  bool operator!=(const ForwardingDetail& other) const {
    return !(*this == other);
  }
  void swap(ForwardingDetail& other) {
    forwardingStmt.swap(other.forwardingStmt);
    receiverType.swap(other.receiverType);
  }
  void mark(Context* context) const {
    forwardingStmt.mark(context);
    receiverType.mark(context);
  }
  size_t hash() const {
    return chpl::hash(forwardingStmt, receiverType);
  }
};
/// \endcond DO_NOT_DOCUMENT

/** ResolvedFields represents the fully resolved fields for a
    class/record/union/tuple type.

    It also stores the result of computing the types of 'forwarding' statements.
 */
class ResolvedFields {
  const types::CompositeType* type_ = nullptr;
  std::vector<FieldDetail> fields_;
  std::vector<ForwardingDetail> forwarding_;

  // Summary information that is computed after the field types are known
  bool isGeneric_ = false;
  bool allGenericFieldsHaveDefaultValues_ = false;

 public:
  ResolvedFields() { }

  void setType(const types::CompositeType* type) {
    type_ = type;
  }

  void addField(UniqueString name,
                bool hasDefaultValue,
                ID declId,
                types::QualifiedType type) {
    fields_.push_back(FieldDetail(name, hasDefaultValue, declId, type));
  }

  void addForwarding(ID forwardingId, types::QualifiedType receiverType) {
    forwarding_.push_back(ForwardingDetail(forwardingId, receiverType));
  }

  void addForwarding(const ResolvedFields& other) {
    for (int i = 0; i < other.numForwards(); i++) {
      addForwarding(other.forwardingStmt(i), other.forwardingToType(i));
    }
  }

  void validateFieldGenericity(Context* context, const types::CompositeType* fieldsOfType) const;

  void finalizeFields(Context* context, bool syntaxOnly);

  /** Returns true if this is a generic type */
  bool isGeneric() const { return isGeneric_; }

  /** Returns true if this is a generic type where all
      generic fields have default values. For classes,
      this does not include consideration of the parent class. */
  bool isGenericWithDefaults() const {
    return isGeneric_ && allGenericFieldsHaveDefaultValues_;
  }

  /** Returns the number of fields represented here */
  int numFields() const {
    return fields_.size();
  }

  /** Returns the field name for the i'th field */
  UniqueString fieldName(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < fields_.size());
    return fields_[i].name;
  }
  /** Returns 'true' if the i'th field has a default value */
  bool fieldHasDefaultValue(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < fields_.size());
    return fields_[i].hasDefaultValue;
  }
  /** Returns the i'th field's declaration ID */
  ID fieldDeclId(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < fields_.size());
    return fields_[i].declId;
  }
  /** Returns the type of the i'th field */
  types::QualifiedType fieldType(int i) const {
    CHPL_ASSERT(0 <= i && (size_t) i < fields_.size());
    return fields_[i].type;
  }

  /** Returns the number of 'forwarding' statements */
  int numForwards() const {
    return forwarding_.size();
  }
  /** Returns the ID of the i'th 'forwarding' statement */
  const ID& forwardingStmt(int i) const {
    assert(0 <= i && (size_t) i < forwarding_.size());
    return forwarding_[i].forwardingStmt;
  }
  /** Returns the type that the i'th 'forwarding' statement forwards to */
  const types::QualifiedType& forwardingToType(int i) const {
    assert(0 <= i && (size_t) i < forwarding_.size());
    return forwarding_[i].receiverType;
  }

  bool operator==(const ResolvedFields& other) const {
    return type_ == other.type_ &&
           fields_ == other.fields_ &&
           forwarding_ == other.forwarding_ &&
           isGeneric_ == other.isGeneric_ &&
           allGenericFieldsHaveDefaultValues_ ==
             other.allGenericFieldsHaveDefaultValues_;
  }
  bool operator!=(const ResolvedFields& other) const {
    return !(*this == other);
  }
  void swap(ResolvedFields& other) {
    std::swap(type_, other.type_);
    fields_.swap(other.fields_);
    forwarding_.swap(other.forwarding_);
    std::swap(isGeneric_, other.isGeneric_);
    std::swap(allGenericFieldsHaveDefaultValues_,
              other.allGenericFieldsHaveDefaultValues_);
  }
  static bool update(ResolvedFields& keep,
                     ResolvedFields& addin) {
    return defaultUpdate(keep, addin);
  }
  void mark(Context* context) const {
    for (auto const &elt : fields_) {
      elt.mark(context);
    }
    for (auto const &elt : forwarding_) {
      elt.mark(context);
    }
    context->markPointer(type_);
  }

  size_t hash() const {
    return chpl::hash(type_, fields_, forwarding_, isGeneric_, allGenericFieldsHaveDefaultValues_);
  }
};

class ResolvedParamLoop {
  public:
    using LoopBodies = std::vector<ResolutionResultByPostorderID>;

  private:
    const uast::For* loop_;
    LoopBodies loopBodies_;

  public:
    ResolvedParamLoop(const uast::For* loop) : loop_(loop) { }

    const uast::For* loop() const {
      return loop_;
    }

    const LoopBodies& loopBodies() const {
      return loopBodies_;
    }

    void setLoopBodies(const LoopBodies& loopBodies) {
      loopBodies_ = loopBodies;
    }

    void mark(Context* context) const {
      for (const auto& postorder : loopBodies_) {
        postorder.mark(context);
      }
      context->markPointer(loop_);
    }

    bool operator==(const ResolvedParamLoop& other) const {
      return loop_ == other.loop_ &&
             loopBodies_ == other.loopBodies_;
    }
    bool operator!=(const ResolvedParamLoop& other) const {
      return !(*this == other);
    }
    void swap(ResolvedParamLoop& other) {
      std::swap(loop_, other.loop_);
      loopBodies_.swap(other.loopBodies_);
    }
    static bool update(ResolvedParamLoop& keep, ResolvedParamLoop& addin) {
      return defaultUpdate(keep, addin);
    }
};

// Represents result info on either a type's copyability or assignability, from
// ref and/or from const.
struct CopyableAssignableInfo {
 private:
  bool fromConst_ = false;
  bool fromRef_ = false;

  CopyableAssignableInfo(bool fromConst, bool fromRef)
      : fromConst_(fromConst), fromRef_(fromRef) {
    assert(!fromConst || fromRef);
  }

 public:
  CopyableAssignableInfo() {}

  bool isFromConst() const { return fromConst_; }
  bool isFromRef() const { return fromRef_; }

  static CopyableAssignableInfo fromConst() {
    return CopyableAssignableInfo(true, true);
  }

  static CopyableAssignableInfo fromRef() {
    return CopyableAssignableInfo(false, true);
  }

  static CopyableAssignableInfo fromNone() {
    return CopyableAssignableInfo(false, false);
  }

  // Set this to the "minimum" copyability between this and other.
  void intersectWith(const CopyableAssignableInfo& other) {
    fromConst_ &= other.fromConst_;
    fromRef_ &= other.fromRef_;
  }

  bool operator==(const CopyableAssignableInfo& other) const {
    return fromConst_ == other.fromConst_ &&
           fromRef_ == other.fromRef_;
  }
  bool operator!=(const CopyableAssignableInfo& other) const {
    return !(*this == other);
  }
  void swap(CopyableAssignableInfo& other) {
    std::swap(fromConst_, other.fromConst_);
    std::swap(fromRef_, other.fromRef_);
  }
  static bool update(CopyableAssignableInfo& keep,
                     CopyableAssignableInfo& addin) {
    return defaultUpdate(keep, addin);
  }
  void mark(Context* context) const {
  }
  size_t hash() const {
    return chpl::hash(fromConst_, fromRef_);
  }
};

/* SimpleMethodLookupHelper helps lookupInScope to find matches
   due to the method receiver during scope resolution.
   This version is temporary and long-term, TypedMethodLookupHelper
   should be used instead. */
class SimpleMethodLookupHelper final : public MethodLookupHelper {
 public:
  using ReceiverScopesVec = llvm::SmallVector<const Scope*, 3>;
 private:
  ID receiverTypeId_;
  ReceiverScopesVec scopes_;
 public:
  SimpleMethodLookupHelper() { }

  SimpleMethodLookupHelper(ID receiverTypeId,
                           ReceiverScopesVec scopes)
    : receiverTypeId_(receiverTypeId), scopes_(scopes) {
  }

  bool isEmpty() const {
    return receiverTypeId_.isEmpty();
  }
  llvm::ArrayRef<const Scope*> receiverScopes() const override;
  bool isReceiverApplicable(Context* context, const ID& methodId) const override;
  bool shouldCheckForTertiaryMethods(Context* context, const VisibilitySymbols* toCheck) const override;

  bool operator==(const SimpleMethodLookupHelper &other) const {
    return receiverTypeId_ == other.receiverTypeId_ &&
           scopes_ == other.scopes_;
  }
  bool operator!=(const SimpleMethodLookupHelper& other) const {
    return !(*this == other);
  }

  void swap(SimpleMethodLookupHelper& other) {
    receiverTypeId_.swap(other.receiverTypeId_);
    scopes_.swap(other.scopes_);
  }

  static bool update(SimpleMethodLookupHelper& keep,
                     SimpleMethodLookupHelper& addin) {
    return defaultUpdate(keep, addin);
  }

  void mark(Context* context) const {
    receiverTypeId_.mark(context);
    for (auto p : scopes_) {
      context->markPointer(p);
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

class ReceiverScopeSimpleHelper final : public ReceiverScopeHelper {
 public:
  ReceiverScopeSimpleHelper() { }
  const SimpleMethodLookupHelper*
  methodLookupForTypeId(Context* context, const ID& typeId) const;
  const SimpleMethodLookupHelper*
  methodLookupForMethodId(Context* context, const ID& methodId) const override;
};

class TypedMethodLookupHelper final : public MethodLookupHelper {
 public:
  using ReceiverScopesVec = llvm::SmallVector<const Scope*, 3>;
 private:
  types::QualifiedType receiverType_;
  ReceiverScopesVec scopes_;
 public:
  TypedMethodLookupHelper() { }

  TypedMethodLookupHelper(types::QualifiedType receiverType,
                          ReceiverScopesVec scopes)
    : receiverType_(std::move(receiverType)),
      scopes_(std::move(scopes)) {
  }

  bool isEmpty() const {
    return receiverType_.type() == nullptr;
  }

  llvm::ArrayRef<const Scope*> receiverScopes() const override;
  bool isReceiverApplicable(Context* context, const ID& methodId) const override;
  bool shouldCheckForTertiaryMethods(Context* context, const VisibilitySymbols* toCheck) const override;

  bool operator==(const TypedMethodLookupHelper &other) const {
    return receiverType_ == other.receiverType_ &&
           scopes_ == other.scopes_;
  }
  bool operator!=(const TypedMethodLookupHelper& other) const {
    return !(*this == other);
  }

  void swap(TypedMethodLookupHelper& other) {
    receiverType_.swap(other.receiverType_);
    scopes_.swap(other.scopes_);
  }

  static bool update(TypedMethodLookupHelper& keep,
                     TypedMethodLookupHelper& addin) {
    return defaultUpdate(keep, addin);
  }

  void mark(Context* context) const {
    receiverType_.mark(context);
    for (auto p : scopes_) {
      context->markPointer(p);
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

struct ReceiverScopeTypedHelper final : public ReceiverScopeHelper {
 private:
  // TODO: these should be a map, to support nested functions
  ID resolvingMethodId_;
  types::QualifiedType resolvingMethodReceiverType_;
 public:
  ReceiverScopeTypedHelper() { }

  ReceiverScopeTypedHelper(ID resolvingMethodId,
                           types::QualifiedType resolvingMethodReceiverType)
    : resolvingMethodId_(std::move(resolvingMethodId)),
      resolvingMethodReceiverType_(std::move(resolvingMethodReceiverType))
  {
  }

  const TypedMethodLookupHelper*
  methodLookupForType(Context* context, types::QualifiedType type, bool checkScalarTypes = false) const;

  const TypedMethodLookupHelper*
  methodLookupForMethodId(Context* context, const ID& methodId) const override;
};


} // end namespace resolution


/// \cond DO_NOT_DOCUMENT

template<> struct stringify<resolution::DefaultsPolicy>
{
  void operator()(std::ostream& streamOut,
                  StringifyKind stringKind,
                  const resolution::DefaultsPolicy& stringMe) const {
    using DefaultsPolicy = resolution::DefaultsPolicy;
    switch (stringMe) {
      case DefaultsPolicy::IGNORE_DEFAULTS:
        streamOut << "IGNORE_DEFAULTS";
        break;
      case DefaultsPolicy::USE_DEFAULTS:
        streamOut << "USE_DEFAULTS";
        break;
      case DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS:
        streamOut << "USE_DEFAULTS_OTHER_FIELDS";
        break;
    }
  }
};

template<> struct stringify<resolution::TypedFnSignature::WhereClauseResult>
{
  void operator()(std::ostream& streamOut,
                  StringifyKind stringKind,
                  const resolution::TypedFnSignature::WhereClauseResult& stringMe) const {
    using WhereClauseResult = resolution::TypedFnSignature::WhereClauseResult;
    switch(stringMe) {
      case WhereClauseResult::WHERE_NONE:
        streamOut << "WHERE_NONE";
        break;
      case WhereClauseResult::WHERE_TBD:
        streamOut <<  "WHERE_TBD";
        break;
      case WhereClauseResult::WHERE_TRUE:
        streamOut <<  "WHERE_TRUE";
        break;
      case WhereClauseResult::WHERE_FALSE:
        streamOut <<  "WHERE_FALSE";
        break;
    }
}
};
/// \endcond DO_NOT_DOCUMENT


} // end namespace chpl


namespace std {

// Stamp out a bunch of 'std::hash' specializations with simple bodies.
// The first arg is the type (without the namespace, and the second arg
// is the expression to return.
#define CHPL_DEFINE_STD_HASH_(T__, expr__) \
  template<> struct hash<chpl::resolution::T__> { \
    size_t operator()(const chpl::resolution::T__& key) const { \
      return (expr__); \
    } \
  }

CHPL_DEFINE_STD_HASH_(DefaultsPolicy, ((size_t) key));
CHPL_DEFINE_STD_HASH_(UntypedFnSignature::FormalDetail, (key.hash()));
CHPL_DEFINE_STD_HASH_(CallInfoActual, (key.hash()));
CHPL_DEFINE_STD_HASH_(CallInfo, (key.hash()));
CHPL_DEFINE_STD_HASH_(CandidatesAndForwardingInfo, (key.hash()));
CHPL_DEFINE_STD_HASH_(PoiInfo, (key.hash()));
CHPL_DEFINE_STD_HASH_(TypedFnSignature::WhereClauseResult, ((size_t) key));
CHPL_DEFINE_STD_HASH_(CandidateFailureReason, ((size_t) key));
CHPL_DEFINE_STD_HASH_(PassingFailureReason, ((size_t) key));
CHPL_DEFINE_STD_HASH_(FormalActual, (key.hash()));
CHPL_DEFINE_STD_HASH_(FormalActualMap, (key.hash()));
CHPL_DEFINE_STD_HASH_(OuterVariables, (key.hash()));
CHPL_DEFINE_STD_HASH_(ApplicabilityResult, (key.hash()));
CHPL_DEFINE_STD_HASH_(CompilerDiagnostic, (key.hash()));
CHPL_DEFINE_STD_HASH_(ResolvedFunction, (key.hash()));
CHPL_DEFINE_STD_HASH_(PromotedFormalMap, (chpl::resolution::hashPromotedFormalMap(key)));
CHPL_DEFINE_STD_HASH_(MostSpecificCandidate, (key.hash()));
CHPL_DEFINE_STD_HASH_(MostSpecificCandidates, (key.hash()));
CHPL_DEFINE_STD_HASH_(CallResolutionResult, (key.hash()));
CHPL_DEFINE_STD_HASH_(TheseResolutionResult, (key.hash()));
CHPL_DEFINE_STD_HASH_(ResolvedFields, (key.hash()));
CHPL_DEFINE_STD_HASH_(FieldDetail, (key.hash()));
CHPL_DEFINE_STD_HASH_(ForwardingDetail, (key.hash()));
CHPL_DEFINE_STD_HASH_(CopyableAssignableInfo, (key.hash()));
#undef CHPL_DEFINE_STD_HASH_

} // end namespace std

#endif
