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

#ifndef CHPL_RESOLUTION_RESOLUTION_TYPES_H
#define CHPL_RESOLUTION_RESOLUTION_TYPES_H

#include "chpl/queries/UniqueString.h"
#include "chpl/resolution/scope-types.h"
#include "chpl/types/CompositeType.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/types/Type.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Function.h"
#include "chpl/util/bitmap.h"
#include "chpl/util/memory.h"

#include <unordered_map>
#include <utility>

namespace chpl {
namespace resolution {

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
  struct FormalDetail {
    UniqueString name;
    bool hasDefaultValue = false;
    const uast::Decl* decl = nullptr;

    FormalDetail(UniqueString name,
                 bool hasDefaultValue,
                 const uast::Decl* decl)
      : name(name),
        hasDefaultValue(hasDefaultValue),
        decl(decl)
    { }

    bool operator==(const FormalDetail& other) const {
      return name == other.name &&
             hasDefaultValue == other.hasDefaultValue &&
             decl == other.decl;
    }
    bool operator!=(const FormalDetail& other) const {
      return !(*this == other);
    }

    size_t hash() const {
      return chpl::hash(name, hasDefaultValue, decl);
    }

    void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
      name.stringify(ss, stringKind);
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
  uast::asttags::AstTag idTag_; // concrete tag for ID
  uast::Function::Kind kind_;
  std::vector<FormalDetail> formals_;

  // this will not be present for compiler-generated functions
  const uast::AstNode* whereClause_;

  UntypedFnSignature(ID id,
                     UniqueString name,
                     bool isMethod,
                     bool isTypeConstructor,
                     bool isCompilerGenerated,
                     uast::asttags::AstTag idTag,
                     uast::Function::Kind kind,
                     std::vector<FormalDetail> formals,
                     const uast::AstNode* whereClause)
    : id_(id),
      name_(name),
      isMethod_(isMethod),
      isTypeConstructor_(isTypeConstructor),
      isCompilerGenerated_(isCompilerGenerated),
      idTag_(idTag),
      kind_(kind),
      formals_(std::move(formals)),
      whereClause_(whereClause) {
    assert(idTag == uast::asttags::Function ||
           idTag == uast::asttags::Class    ||
           idTag == uast::asttags::Record   ||
           idTag == uast::asttags::Union    ||
           idTag == uast::asttags::Variable);
  }

  static const owned<UntypedFnSignature>&
  getUntypedFnSignature(Context* context, ID id,
                        UniqueString name,
                        bool isMethod,
                        bool isTypeConstructor,
                        bool isCompilerGenerated,
                        uast::asttags::AstTag idTag,
                        uast::Function::Kind kind,
                        std::vector<FormalDetail> formals,
                        const uast::AstNode* whereClause);

 public:
  /** Get the unique UntypedFnSignature containing these components */
  static const UntypedFnSignature* get(Context* context, ID id,
                                       UniqueString name,
                                       bool isMethod,
                                       bool isTypeConstructor,
                                       bool isCompilerGenerated,
                                       uast::asttags::AstTag idTag,
                                       uast::Function::Kind kind,
                                       std::vector<FormalDetail> formals,
                                       const uast::AstNode* whereClause);

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
           idTag_ == other.idTag_ &&
           kind_ == other.kind_ &&
           formals_ == other.formals_ &&
           whereClause_ == other.whereClause_;
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

  /** Returns true if id() refers to a Function */
  bool idIsFunction() const {
    return idTag_ == uast::asttags::Function;
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

  /** Returns the number of formals */
  int numFormals() const {
    return formals_.size();
  }
  /** Returns the name of the i'th formal. */
  UniqueString formalName(int i) const {
    assert(0 <= i && (size_t) i < formals_.size());
    return formals_[i].name;
  }

  /** Return whether the i'th formal has a default value. */
  bool formalHasDefault(int i) const {
    assert(0 <= i && (size_t) i < formals_.size());
    return formals_[i].hasDefaultValue;
  }

  /** Returns the Decl for the i'th formal / field.
      This will return nullptr for compiler-generated functions. */
  const uast::Decl* formalDecl(int i) const {
    assert(0 <= i && (size_t) i < formals_.size());
    return formals_[i].decl;
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

/** CallInfoActual */
class CallInfoActual {
 private:
  types::QualifiedType type_;
  UniqueString byName_;

 public:
  CallInfoActual(types::QualifiedType type, UniqueString byName)
      : type_(type), byName_(byName) {}

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
  size_t hash() const {
    return chpl::hash(type_, byName_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/** CallInfo */
class CallInfo {
 private:
  UniqueString name_;                   // the name of the called thing
  types::QualifiedType calledType_;     // the type of the called thing
  bool isMethodCall_ = false;           // then actuals[0] is receiver
  bool hasQuestionArg_ = false;         // includes ? arg for type constructor
  bool isParenless_ = false;            // is a parenless call
  std::vector<CallInfoActual> actuals_; // types/params/names of actuals

 public:
  using CallInfoActualIterable = Iterable<std::vector<CallInfoActual>>;

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
      assert(numActuals() >= 1);
      assert(this->actual(0).byName() == "this");
    }
    if (isParenless) {
      if (isMethodCall) {
        assert(numActuals() == 1);
      } else {
        assert(numActuals() == 0);
      }
    }
    #endif
  }

  /** Construct a CallInfo with unknown types for the actuals
      that can be used for FormalActualMap but not much else.  */
  CallInfo(const uast::FnCall* call);

  /** return the name of the called thing */
  UniqueString name() const { return name_; }

  /** return the type of the called thing */
  types::QualifiedType calledType() const { return calledType_; }

  /** check if the call is a method call */
  bool isMethodCall() const { return isMethodCall_; }

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
    assert(i < actuals_.size());
    return actuals_[i];
  }

  /** return the number of actuals */
  size_t numActuals() const { return actuals_.size(); }

  bool operator==(const CallInfo& other) const {
    return name_ == other.name_ &&
           calledType_ == other.calledType_ &&
           isMethodCall_ == other.isMethodCall_ &&
           hasQuestionArg_ == other.hasQuestionArg_ &&
           isParenless_ == other.isParenless_ &&
           actuals_ == other.actuals_;
  }
  bool operator!=(const CallInfo& other) const {
    return !(*this == other);
  }
  size_t hash() const {
    return chpl::hash(name_, calledType_, isMethodCall_,
                      hasQuestionArg_, isParenless_,
                      actuals_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};


/**
  Contains information about symbols available from point-of-instantiation
  in order to implement caching of instantiations.
 */
class PoiInfo {
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
  // transitively
  std::set<std::pair<ID, ID>> poiFnIdsUsed_;

 public:
  // default construct a PoiInfo
  PoiInfo() { }

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

  // TODO callers copy and store this elsewhere, do we return as is? change the
  // getter to poiFnIdsUsedAsSet? make callers do std::set(poiFnIdsUsed.begin(),
  // poiFnIdsUsed.end()) ?
  const std::set<std::pair<ID, ID>> &poiFnIdsUsed() const {
    return poiFnIdsUsed_;
  }

  void addIds(ID a, ID b) {
    poiFnIdsUsed_.emplace(a, b);
  }

  // return true if the two passed PoiInfos represent the same information
  // (for use in an update function)
  static bool updateEquals(const PoiInfo& a, const PoiInfo& b) {
    return a.resolved_ == b.resolved_ &&
           a.poiScope_ == b.poiScope_ &&
           a.poiFnIdsUsed_ == b.poiFnIdsUsed_;
  }

  void swap(PoiInfo& other) {
    std::swap(resolved_, other.resolved_);
    std::swap(poiScope_, other.poiScope_);
    poiFnIdsUsed_.swap(other.poiFnIdsUsed_);
  }

  // accumulate PoiInfo from a call into this PoiInfo
  void accumulate(const PoiInfo& addPoiInfo);

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
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
    ss << "PoiInfo: ";
    poiScope()->stringify(ss, stringKind);
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

  // Is this TypedFnSignature representing an instantiation?
  // If so, what is the generic TypedFnSignature that was instantiated?
  const TypedFnSignature* instantiatedFrom_ = nullptr;

  // Is this for an inner Function? If so, what is the parent
  // function signature?
  const TypedFnSignature* parentFn_ = nullptr;

  // Which formal arguments were substituted when instantiating?
  Bitmap formalsInstantiated_;

  TypedFnSignature(const UntypedFnSignature* untypedSignature,
                   std::vector<types::QualifiedType> formalTypes,
                   WhereClauseResult whereClauseResult,
                   bool needsInstantiation,
                   const TypedFnSignature* instantiatedFrom,
                   const TypedFnSignature* parentFn,
                   Bitmap formalsInstantiated)
    : untypedSignature_(untypedSignature),
      formalTypes_(std::move(formalTypes)),
      whereClauseResult_(whereClauseResult),
      needsInstantiation_(needsInstantiation),
      instantiatedFrom_(instantiatedFrom),
      parentFn_(parentFn),
      formalsInstantiated_(std::move(formalsInstantiated)) { }

  static const owned<TypedFnSignature>&
  getTypedFnSignature(Context* context,
                      const UntypedFnSignature* untypedSignature,
                      std::vector<types::QualifiedType> formalTypes,
                      TypedFnSignature::WhereClauseResult whereClauseResult,
                      bool needsInstantiation,
                      const TypedFnSignature* instantiatedFrom,
                      const TypedFnSignature* parentFn,
                      Bitmap formalsInstantiated);

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
                              Bitmap formalsInstantiated);

  bool operator==(const TypedFnSignature& other) const {
    return untypedSignature_ == other.untypedSignature_ &&
           formalTypes_ == other.formalTypes_ &&
           whereClauseResult_ == other.whereClauseResult_ &&
           needsInstantiation_ == other.needsInstantiation_ &&
           instantiatedFrom_ == other.instantiatedFrom_ &&
           parentFn_ == other.parentFn_ &&
           formalsInstantiated_ == other.formalsInstantiated_;
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

  /** Returns the result of evaluating the where clause */
  WhereClauseResult whereClauseResult() const {
    return whereClauseResult_;
  }

  /** Returns if any of the formals are generic or unknown */
  bool needsInstantiation() const {
    return needsInstantiation_;
  }

  /**
    Is this TypedFnSignature representing an instantiation?  If so, returns the
    generic TypedFnSignature that was instantiated.  Otherwise, returns nullptr.

    This function always returns the signature for the fully generic
    function and never a partial instantiation. That is, the result
    will either be nullptr or result->instantiatedFrom() will be nullptr.
   */
  const TypedFnSignature* instantiatedFrom() const {
    assert(instantiatedFrom_ == nullptr ||
           instantiatedFrom_->instantiatedFrom_ == nullptr);
    return instantiatedFrom_;
  }

  /**
    Returns 'true' if formal argument i was instantiated; that is,
    it was present in the SubstitutionsMap when instantiating.
   */
  bool formalIsInstantiated(int i) const {
    if (instantiatedFrom_ == nullptr)
      return false;

    return formalsInstantiated_[i];
  }

  /**
     Is this for an inner Function? If so, what is the parent
     function signature?
   */
  const TypedFnSignature* parentFn() const {
    return parentFn_;
  }

  /** Returns the number of formals */
  int numFormals() const {
    int ret = formalTypes_.size();
    assert(untypedSignature_ && ret == untypedSignature_->numFormals());
    return ret;
  }
  /** Returns the name of the i'th formal */
  UniqueString formalName(int i) const {
    return untypedSignature_->formalName(i);
  }
  /** Returns the type of the i'th formal */
  const types::QualifiedType& formalType(int i) const {
    assert(0 <= i && (size_t) i < formalTypes_.size());
    return formalTypes_[i];
  }

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
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
  const TypedFnSignature* candidates[NUM_INTENTS] = {nullptr};
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
  static MostSpecificCandidates getOnly(const TypedFnSignature* fn) {
    MostSpecificCandidates ret;
    if (fn != nullptr)
      ret.setBestOnly(fn);
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

  const TypedFnSignature* const* begin() const {
    return &candidates[0];
  }
  const TypedFnSignature* const* end() const {
    return &candidates[NUM_INTENTS];
  }

  void setBestRef(const TypedFnSignature* sig) {
    candidates[REF] = sig;
  }
  void setBestConstRef(const TypedFnSignature* sig) {
    candidates[CONST_REF] = sig;
  }
  void setBestValue(const TypedFnSignature* sig) {
    candidates[VALUE] = sig;
  }

  void setBestOnly(const TypedFnSignature* sig) {
    candidates[ONLY] = sig;
  }

  const TypedFnSignature* bestRef() const {
    return candidates[REF];
  }
  const TypedFnSignature* bestConstRef() const {
    return candidates[CONST_REF];
  }
  const TypedFnSignature* bestValue() const {
    return candidates[VALUE];
  }

  /**
    If there is exactly one candidate, return that candidate.
    Otherwise, return nullptr.
   */
  const TypedFnSignature* only() const {
    const TypedFnSignature* ret = nullptr;
    int nPresent = 0;
    for (const TypedFnSignature* sig : *this) {
      if (sig != nullptr) {
        ret = sig;
        nPresent++;
      }
    }
    if (nPresent != 1) {
      return nullptr;
    }

    // if there is only one candidate, it should be in slot ONLY
    assert(candidates[ONLY] == ret);
    return ret;
  }

  /**
   Returns the number of best candidates that are contained here.
   */
  int numBest() const {
    int ret = 0;
    for (const TypedFnSignature* sig : *this) {
      if (sig != nullptr) {
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
    assert(!emptyDueToAmbiguity || isEmpty());

    return emptyDueToAmbiguity;
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
    for (const TypedFnSignature* sig : candidates) {
      context->markPointer(sig);
    }
  }
};

/** CallResolutionResult */
class CallResolutionResult {
 private:
  // what are the candidates for return-intent overloading?
  MostSpecificCandidates mostSpecific_;
  // what is the type of the call expression?
  types::QualifiedType exprType_;
  // if any of the candidates were instantiated, what point-of-instantiation
  // scopes were used when resolving their signature or body?
  PoiInfo poiInfo_;

 public:
  // for simple cases where mostSpecific and poiInfo are irrelevant
  CallResolutionResult(types::QualifiedType exprType)
    : exprType_(std::move(exprType)) {
  }

  CallResolutionResult(MostSpecificCandidates mostSpecific,
                       types::QualifiedType exprType,
                       PoiInfo poiInfo)
    : mostSpecific_(std::move(mostSpecific)),
      exprType_(std::move(exprType)),
      poiInfo_(std::move(poiInfo)) {
  }

  /** get the most specific candidates for return-intent overloading */
  const MostSpecificCandidates& mostSpecific() const { return mostSpecific_; }

  /** type of the call expression */
  const types::QualifiedType& exprType() const { return exprType_; }

  /** point-of-instantiation scopes used when resolving signature or body */
  const PoiInfo& poiInfo() const { return poiInfo_; }

  bool operator==(const CallResolutionResult& other) const {
    return mostSpecific_ == other.mostSpecific_ &&
           exprType_ == other.exprType_ &&
           PoiInfo::updateEquals(poiInfo_, other.poiInfo_);
  }
  bool operator!=(const CallResolutionResult& other) const {
    return !(*this == other);
  }
  void swap(CallResolutionResult& other) {
    mostSpecific_.swap(other.mostSpecific_);
    exprType_.swap(other.exprType_);
    poiInfo_.swap(other.poiInfo_);
  }
};

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

  // For a function call, what is the most specific candidate,
  // or when using return intent overloading, what are the most specific
  // candidates?
  // The choice between these needs to happen
  // later than the main function resolution.
  MostSpecificCandidates mostSpecific_;
  // What point of instantiation scope should be used when
  // resolving functions in mostSpecific?
  const PoiScope *poiScope_ = nullptr;

  // functions associated with or used to implement this expression
  std::vector<const TypedFnSignature*> associatedFns_;

 public:
  using AssociatedFns = std::vector<const TypedFnSignature*>;

  ResolvedExpression() { }

  /** get the qualified type */
  const types::QualifiedType& type() const { return type_; }

  /** for simple (non-function Identifier) cases, the ID of a NamedDecl it
   * refers to */
  ID toId() const { return toId_; }

  /** For a function call, what is the most specific candidate, or when using
   * return intent overloading, what are the most specific candidates? The
   * choice between these needs to happen later than the main function
   * resolution.
   */
  const MostSpecificCandidates& mostSpecific() const { return mostSpecific_; }

  const PoiScope* poiScope() const { return poiScope_; }

  const AssociatedFns& associatedFns() const {
    return associatedFns_;
  }

  /** set the toId */
  void setToId(ID toId) { toId_ = toId; }

  /** set the type */
  void setType(const types::QualifiedType& type) { type_ = type; }

  /** set the most specific */
  void setMostSpecific(const MostSpecificCandidates& mostSpecific) {
    mostSpecific_ = mostSpecific;
  }

  /** set the point-of-instantiation scope */
  void setPoiScope(const PoiScope* poiScope) { poiScope_ = poiScope; }

  /** add an associated function */
  void addAssociatedFn(const TypedFnSignature* fn) {
    associatedFns_.push_back(fn);
  }

  bool operator==(const ResolvedExpression& other) const {
    return type_ == other.type_ &&
           toId_ == other.toId_ &&
           mostSpecific_ == other.mostSpecific_ &&
           poiScope_ == other.poiScope_ &&
           associatedFns_ == other.associatedFns_;
  }
  bool operator!=(const ResolvedExpression& other) const {
    return !(*this == other);
  }
  void swap(ResolvedExpression& other) {
    type_.swap(other.type_);
    toId_.swap(other.toId_);
    mostSpecific_.swap(other.mostSpecific_);
    std::swap(poiScope_, other.poiScope_);
    std::swap(associatedFns_, other.associatedFns_);
  }
  static bool update(ResolvedExpression& keep, ResolvedExpression& addin) {
    return defaultUpdate(keep, addin);
  }
  void mark(Context* context) const {
    type_.mark(context);
    toId_.mark(context);
    mostSpecific_.mark(context);
    context->markPointer(poiScope_);
    for (auto tfs : associatedFns_) tfs->mark(context);
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
  ID symbolId;
  std::vector<ResolvedExpression> vec;

 public:
  /** prepare to resolve the contents of the passed symbol */
  void setupForSymbol(const uast::AstNode* ast);
  /** prepare to resolve the signature of the passed function */
  void setupForSignature(const uast::Function* func);
  /** prepare to resolve the body of the passed function */
  void setupForFunction(const uast::Function* func);

  ResolvedExpression& byIdExpanding(const ID& id) {
    auto postorder = id.postOrderId();
    assert(id.symbolPath() == symbolId.symbolPath());
    assert(0 <= postorder);
    if ((size_t) postorder < vec.size()) {
      // OK
    } else {
      vec.resize(postorder+1);
    }
    return vec[postorder];
  }
  ResolvedExpression& byAstExpanding(const uast::AstNode* ast) {
    return byIdExpanding(ast->id());
  }
  ResolvedExpression& byId(const ID& id) {
    auto postorder = id.postOrderId();
    assert(id.symbolPath() == symbolId.symbolPath());
    assert(0 <= postorder && (size_t) postorder < vec.size());
    return vec[postorder];
  }
  const ResolvedExpression& byId(const ID& id) const {
    auto postorder = id.postOrderId();
    assert(0 <= postorder && (size_t) postorder < vec.size());
    return vec[postorder];
  }
  ResolvedExpression& byAst(const uast::AstNode* ast) {
    return byId(ast->id());
  }
  const ResolvedExpression& byAst(const uast::AstNode* ast) const {
    return byId(ast->id());
  }

  bool operator==(const ResolutionResultByPostorderID& other) const {
    return symbolId == other.symbolId &&
           vec == other.vec;
  }
  bool operator!=(const ResolutionResultByPostorderID& other) const {
    return !(*this == other);
  }
  void swap(ResolutionResultByPostorderID& other) {
    symbolId.swap(other.symbolId);
    vec.swap(other.vec);
  }
  static bool update(ResolutionResultByPostorderID& keep,
                     ResolutionResultByPostorderID& addin);
  void mark(Context* context) const {
    symbolId.mark(context);
    for (auto const &elt : vec) {
      elt.mark(context);
    }
  }
};

/**
  This type represents a resolved function.
*/
class ResolvedFunction {
 private:
  const TypedFnSignature* signature_ = nullptr;

  uast::Function::ReturnIntent returnIntent_ =
      uast::Function::DEFAULT_RETURN_INTENT;

  // this is the output of the resolution process
  ResolutionResultByPostorderID resolutionById_;

  // the set of point-of-instantiation scopes used by the instantiation
  PoiInfo poiInfo_;

 public:
  ResolvedFunction(const TypedFnSignature *signature,
                   uast::Function::ReturnIntent returnIntent,
                   ResolutionResultByPostorderID resolutionById,
                   PoiInfo poiInfo)
      : signature_(signature), returnIntent_(returnIntent),
        resolutionById_(resolutionById), poiInfo_(poiInfo) {}

  /** The type signature */
  const TypedFnSignature* signature() const { return signature_; }

  /** the return intent */
  uast::Function::ReturnIntent returnIntent() const { return returnIntent_; }

  /** this is the output of the resolution process */
  const ResolutionResultByPostorderID& resolutionById() const {
    return resolutionById_;
  }

  /** the set of point-of-instantiations used by the instantiation */
  const PoiInfo& poiInfo() const { return poiInfo_; }

  bool operator==(const ResolvedFunction& other) const {
    return signature_ == other.signature_ &&
           returnIntent_ == other.returnIntent_ &&
           resolutionById_ == other.resolutionById_ &&
           PoiInfo::updateEquals(poiInfo_, other.poiInfo_);
  }
  bool operator!=(const ResolvedFunction& other) const {
    return !(*this == other);
  }
  void swap(ResolvedFunction& other) {
    std::swap(signature_, other.signature_);
    std::swap(returnIntent_, other.returnIntent_);
    resolutionById_.swap(other.resolutionById_);
    poiInfo_.swap(other.poiInfo_);
  }
  static bool update(owned<ResolvedFunction>& keep,
                     owned<ResolvedFunction>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    context->markPointer(signature_);
    resolutionById_.mark(context);
    poiInfo_.mark(context);
  }

  const ResolvedExpression& byId(const ID& id) const {
    return resolutionById_.byId(id);
  }
  const ResolvedExpression& byAst(const uast::AstNode* ast) const {
    return resolutionById_.byAst(ast);
  }

  const ID& id() const {
    return signature_->id();
  }
};

class FormalActualMap;


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

 public:
  const types::QualifiedType& formalType() const { return formalType_; }
  const types::QualifiedType& actualType() const { return actualType_; }
  const uast::Decl* formal() const { return formal_; }
  int formalIdx() const { return formalIdx_; }
  int actualIdx() const { return actualIdx_; }
  bool hasActual() const { return hasActual_; }
  bool formalInstantiated() const { return formalInstantiated_; }
};

/** FormalActualMap maps formals to actuals */
class FormalActualMap {
 private:
  std::vector<FormalActual> byFormalIdx_;
  std::vector<int> actualIdxToFormalIdx_;
  bool mappingIsValid_ = false;
  int failingActualIdx_ = -1;
  int failingFormalIdx_ = -1;

 public:

  using FormalActualIterable = Iterable<std::vector<FormalActual>>;

  FormalActualMap(const UntypedFnSignature* sig, const CallInfo& call) {
    mappingIsValid_ = computeAlignment(sig, nullptr, call);
  }
  FormalActualMap(const TypedFnSignature* sig, const CallInfo& call) {
    mappingIsValid_ = computeAlignment(sig->untyped(), sig, call);
  }

  /** check if mapping is valid */
  bool isValid() const { return mappingIsValid_; }

  /** get the FormalActuals in the order of the formal arguments */
  FormalActualIterable byFormals() const {
    return FormalActualIterable(byFormalIdx_);
  }

  /** get the FormalActual for a particular formal index */
  const FormalActual& byFormalIdx(int formalIdx) const {
    assert(0 <= formalIdx && (size_t) formalIdx < byFormalIdx_.size());
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

 private:
  bool computeAlignment(const UntypedFnSignature* untyped,
                        const TypedFnSignature* typed, const CallInfo& call);
};

/** ResolvedFields represents the fully resolved fields for a
    class/record/union/tuple type. */
class ResolvedFields {
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
    size_t hash() const {
      return chpl::hash(name, hasDefaultValue, declId, type);
    }

    void mark(Context* context) const {
      name.mark(context);
      declId.mark(context);
      type.mark(context);
    }

    /*
    void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
      name.stringify(ss, stringKind);
      //TODO: determine the proper way to do this
      //decl.stringify(ss, stringKind);
      type.stringify(ss, stringKind);
    }*/
  };

  const types::CompositeType* type_ = nullptr;
  std::vector<FieldDetail> fields_;

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

  void finalizeFields(Context* context);

  /** Returns true if this is a generic type */
  bool isGeneric() const { return isGeneric_; }

  /** Returns true if this is a generic type where all
      generic fields have default values. For classes,
      this does not include consideration of the parent class. */
  bool isGenericWithDefaults() const {
    return isGeneric_ && allGenericFieldsHaveDefaultValues_;
  }

  int numFields() const {
    return fields_.size();
  }

  UniqueString fieldName(int i) const {
    assert(0 <= i && (size_t) i < fields_.size());
    return fields_[i].name;
  }
  bool fieldHasDefaultValue(int i) const {
    assert(0 <= i && (size_t) i < fields_.size());
    return fields_[i].hasDefaultValue;
  }
  ID fieldDeclId(int i) const {
    assert(0 <= i && (size_t) i < fields_.size());
    return fields_[i].declId;
  }
  types::QualifiedType fieldType(int i) const {
    assert(0 <= i && (size_t) i < fields_.size());
    return fields_[i].type;
  }

  bool operator==(const ResolvedFields& other) const {
    return type_ == other.type_ &&
           fields_ == other.fields_ &&
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
    context->markPointer(type_);
  }
};

/** See the documentation for types::CompositeType::SubstitutionsMap. */
using SubstitutionsMap = types::CompositeType::SubstitutionsMap;


} // end namespace resolution


/// \cond DO_NOT_DOCUMENT

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

template<> struct hash<chpl::resolution::UntypedFnSignature::FormalDetail>
{
  size_t operator()(const chpl::resolution::UntypedFnSignature::FormalDetail& key) const {
    return key.hash();
  }
};

template<> struct hash<chpl::resolution::CallInfoActual>
{
  size_t operator()(const chpl::resolution::CallInfoActual& key) const {
    return key.hash();
  }
};

template<> struct hash<chpl::resolution::CallInfo>
{
  size_t operator()(const chpl::resolution::CallInfo& key) const {
    return key.hash();
  }
};

template<> struct hash<chpl::resolution::PoiInfo>
{
  size_t operator()(const chpl::resolution::PoiInfo& key) const {
    return key.hash();
  }
};

template<> struct hash<chpl::resolution::TypedFnSignature::WhereClauseResult>
{
  size_t operator()(const chpl::resolution::TypedFnSignature::WhereClauseResult& key) const {
    return key;
  }
};



} // end namespace std


#endif
