/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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
#include "chpl/types/QualifiedType.h"
#include "chpl/types/Type.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/uast/Function.h"
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
 */
class UntypedFnSignature {
 public:
  struct FormalDetail {
    UniqueString name;
    bool hasDefaultValue = false;
    const uast::Decl* decl = nullptr;

    FormalDetail(UniqueString name, bool hasDefault, const uast::Decl* decl)
      : name(name), hasDefaultValue(hasDefault), decl(decl) { }

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
  };

 private:
  // the ID of a uAST Function, if there is one for this signature;
  // or, for compiler-generated functions/methods, the ID of the record/class
  // declaration
  ID id_;
  UniqueString name_;
  bool isMethod_; // in that case, formals[0] is the receiver
  bool idIsFunction_; // whether the ID is of a function
  bool isTypeConstructor_;
  uast::Function::Kind kind_;
  std::vector<FormalDetail> formals_;

  // this will not be present for compiler-generated functions
  const uast::Expression* whereClause_;

  UntypedFnSignature(ID id,
                     UniqueString name,
                     bool isMethod,
                     bool idIsFunction,
                     bool isTypeConstructor,
                     uast::Function::Kind kind,
                     std::vector<FormalDetail> formals,
                     const uast::Expression* whereClause)
    : id_(id),
      name_(name),
      isMethod_(isMethod),
      idIsFunction_(idIsFunction),
      isTypeConstructor_(isTypeConstructor),
      kind_(kind),
      formals_(std::move(formals)),
      whereClause_(whereClause) {
  }

  static const owned<UntypedFnSignature>&
  getUntypedFnSignature(Context* context, ID id,
                        UniqueString name,
                        bool isMethod,
                        bool idIsFunction,
                        bool isTypeConstructor,
                        uast::Function::Kind kind,
                        std::vector<FormalDetail> formals,
                        const uast::Expression* whereClause);

 public:
  /** Get the unique UntypedFnSignature containing these components */
  static const UntypedFnSignature* get(Context* context, ID id,
                                       UniqueString name,
                                       bool isMethod,
                                       bool idIsFunction,
                                       bool isTypeConstructor,
                                       uast::Function::Kind kind,
                                       std::vector<FormalDetail> formals,
                                       const uast::Expression* whereClause);

  /** Get the unique UntypedFnSignature representing a Function's
      signature. */
  static const UntypedFnSignature* get(Context* context,
                                       const uast::Function* function);


  bool operator==(const UntypedFnSignature& other) const {
    return id_ == other.id_ &&
           name_ == other.name_ &&
           isMethod_ == other.isMethod_ &&
           idIsFunction_ == other.idIsFunction_ &&
           isTypeConstructor_ == other.isTypeConstructor_ &&
           kind_ == other.kind_ &&
           formals_ == other.formals_ &&
           whereClause_ == other.whereClause_;
  }
  bool operator!=(const UntypedFnSignature& other) const {
    return !(*this == other);
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

  /** Returns true if id() refers to a Function */
  bool idIsFunction() const {
    return idIsFunction_;
  }

  /** Returns true if this is a type constructor */
  bool isTypeConstructor() const {
    return isTypeConstructor_;
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
};

using SubstitutionsMap = std::unordered_map<const uast::Decl*, types::QualifiedType>;

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
};

/** CallInfo */
class CallInfo {
 private:
  UniqueString name_;                   // the name of the called thing
  bool isMethod_ = false;               // in that case, actuals[0] is receiver
  bool hasQuestionArg_ = false;         // includes ? arg for type constructor
  std::vector<CallInfoActual> actuals_; // types/params/names of actuals

 public:
  using CallInfoActualIterable = Iterable<std::vector<CallInfoActual>>;

  CallInfo(UniqueString name, bool hasQuestionArg,
           std::vector<CallInfoActual> actuals)
      : name_(name), hasQuestionArg_(hasQuestionArg),
        actuals_(std::move(actuals)) {}

  /** return the name of the called thing */
  UniqueString name() const { return name_; }

  /** check if the call is a method call */
  bool isMethod() const { return isMethod_; }

  /** check if the call includes ? arg for type constructor */
  bool hasQuestionArg() const { return hasQuestionArg_; }

  /** return the actuals */
  CallInfoActualIterable actuals() const {
    return CallInfoActualIterable(actuals_);
  }

  /** return the i'th actual */
  const CallInfoActual& actuals(size_t i) const {
    assert(i < actuals_.size());
    return actuals_[i];
  }

  /** return the number of actuals */
  size_t numActuals() const { return actuals_.size(); }

  bool operator==(const CallInfo& other) const {
    return name_ == other.name_ &&
           isMethod_ == other.isMethod_ &&
           hasQuestionArg_ == other.hasQuestionArg_ &&
           actuals_ == other.actuals_;
  }
  bool operator!=(const CallInfo& other) const {
    return !(*this == other);
  }
  size_t hash() const {
    return chpl::hash(name_, isMethod_, hasQuestionArg_, actuals_);
  }
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
  // poiFnidsUsed.end()) ?
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

  // TODO: This could include a substitutions map, if we need it.
  // The formalTypes above might be enough, though.

 public:
  TypedFnSignature(const UntypedFnSignature* untypedSignature,
                   std::vector<types::QualifiedType> formalTypes,
                   WhereClauseResult whereClauseResult,
                   bool needsInstantiation,
                   const TypedFnSignature* instantiatedFrom,
                   const TypedFnSignature* parentFn)
    : untypedSignature_(untypedSignature),
      formalTypes_(std::move(formalTypes)),
      whereClauseResult_(whereClauseResult),
      needsInstantiation_(needsInstantiation),
      instantiatedFrom_(instantiatedFrom),
      parentFn_(parentFn) { }

  bool operator==(const TypedFnSignature& other) const {
    return untypedSignature_ == other.untypedSignature_ &&
           formalTypes_ == other.formalTypes_ &&
           whereClauseResult_ == other.whereClauseResult_ &&
           needsInstantiation_ == other.needsInstantiation_ &&
           instantiatedFrom_ == other.instantiatedFrom_ &&
           parentFn_ == other.parentFn_;
  }
  bool operator!=(const TypedFnSignature& other) const {
    return !(*this == other);
  }

  std::string toString(chpl::StringifyKind stringKind) const;

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
   */
  const TypedFnSignature* instantiatedFrom() const {
    return instantiatedFrom_;
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
};

/**
  Stores the most specific candidates when resolving a function call.
*/
class MostSpecificCandidates {
 public:
  typedef enum {
    // the slots in the candidates array for return intent
    // overloading
    REF = 0,
    CONST_REF,
    VALUE,
    // NUM_INTENTS is the size of the candidates array
    NUM_INTENTS,
  } Intent;

 private:
  const TypedFnSignature* candidates[NUM_INTENTS] = {nullptr};

 public:
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
    for (int i = 0; i < NUM_INTENTS; i++) {
      const TypedFnSignature* sig = candidates[i];
      if (sig != nullptr) {
        ret = sig;
        nPresent++;
      }
    }
    if (nPresent != 1) {
      return nullptr;
    }
    return ret;
  }

  bool operator==(const MostSpecificCandidates& other) const {
    for (int i = 0; i < NUM_INTENTS; i++) {
      if (candidates[i] != other.candidates[i])
        return false;
    }
    return true;
  }
  bool operator!=(const MostSpecificCandidates& other) const {
    return !(*this == other);
  }
  void swap(MostSpecificCandidates& other) {
    for (int i = 0; i < NUM_INTENTS; i++) {
      std::swap(candidates[i], other.candidates[i]);
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

 public:
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

  bool operator==(const ResolvedExpression& other) const {
    return type_ == other.type_ &&
           toId_ == other.toId_ &&
           mostSpecific_ == other.mostSpecific_ &&
           poiScope_ == other.poiScope_;
  }
  bool operator!=(const ResolvedExpression& other) const {
    return !(*this == other);
  }
  void swap(ResolvedExpression& other) {
    type_.swap(other.type_);
    toId_.swap(other.toId_);
    mostSpecific_.swap(other.mostSpecific_);
    std::swap(poiScope_, other.poiScope_);
  }

  std::string toString(chpl::StringifyKind stringKind) const;
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
  void setupForSymbol(const uast::ASTNode* ast);
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
  ResolvedExpression& byAstExpanding(const uast::ASTNode* ast) {
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
  ResolvedExpression& byAst(const uast::ASTNode* ast) {
    return byId(ast->id());
  }
  const ResolvedExpression& byAst(const uast::ASTNode* ast) const {
    return byId(ast->id());
  }

  bool operator==(const ResolutionResultByPostorderID& other) const {
    return vec == other.vec;
  }
  bool operator!=(const ResolutionResultByPostorderID& other) const {
    return !(*this == other);
  }
  void swap(ResolutionResultByPostorderID& other) {
    vec.swap(other.vec);
  }

  static bool update(ResolutionResultByPostorderID& keep,
                     ResolutionResultByPostorderID& addin);
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

  const ResolvedExpression& byId(const ID& id) const {
    return resolutionById_.byId(id);
  }
  const ResolvedExpression& byAst(const uast::ASTNode* ast) const {
    return resolutionById_.byAst(ast);
  }

  const ID& id() const {
    return signature_->id();
  }
};

/** FormalActual holds information on a function formal and its binding (if any) */
struct FormalActual {
  const uast::Decl* formal = nullptr;
  types::QualifiedType formalType;
  bool hasActual = false; // == false means uses formal default value
  int actualIdx = -1;
  types::QualifiedType actualType;
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

  /** get the FormalActuals */
  FormalActualIterable byFormalIdx() const {
    return FormalActualIterable(byFormalIdx_);
  }

 private:
  bool computeAlignment(const UntypedFnSignature* untyped,
                        const TypedFnSignature* typed, const CallInfo& call);
};

} // end namespace resolution


/// \cond DO_NOT_DOCUMENT
template<> struct update<resolution::ResolvedExpression> {
  bool operator()(resolution::ResolvedExpression& keep,
                  resolution::ResolvedExpression& addin) const {
    return defaultUpdate(keep, addin);
  }
};

template<> struct update<resolution::MostSpecificCandidates> {
  bool operator()(resolution::MostSpecificCandidates& keep,
                  resolution::MostSpecificCandidates& addin) const {
    return defaultUpdate(keep, addin);
  }
};

template<> struct update<resolution::ResolutionResultByPostorderID> {
  bool operator()(resolution::ResolutionResultByPostorderID& keep,
                  resolution::ResolutionResultByPostorderID& addin) const {
    return resolution::ResolutionResultByPostorderID::update(keep, addin);
  }
};

template<> struct update<owned<resolution::ResolvedFunction>> {
  bool operator()(owned<resolution::ResolvedFunction>& keep,
                  owned<resolution::ResolvedFunction>& addin) const {
    // this function is just here to make debugging easier
    return defaultUpdateOwned(keep, addin);
  }
};

template<> struct stringify<chpl::resolution::ResolvedExpression> {
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::ResolvedExpression& stringMe) const {
    stringOut << "resolution::ResolvedExpression not stringified";
  }
};

template<> struct stringify<chpl::resolution::MostSpecificCandidates> {
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::MostSpecificCandidates& stringMe) const {
    stringOut << "resolution::MostSpecificCandidates not stringified";
  }
};

template<> struct stringify<chpl::resolution::ResolutionResultByPostorderID> {
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::ResolutionResultByPostorderID& stringMe) const {
    stringOut << "resolution::ResolutionResultByPostorderID not stringified";
  }
};

template<> struct stringify<chpl::resolution::ResolvedFunction> {
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::ResolvedFunction& stringMe) const {
    stringOut << "resolution::ResolvedFunction not stringified";
  }
};


template<> struct stringify<chpl::resolution::UntypedFnSignature>
{
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::UntypedFnSignature& stringMe) const {
    stringOut << "resolution::UntypedFnSignature not stringified";
  }
};


template<> struct stringify<chpl::resolution::CallInfoActual>
{
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::CallInfoActual& stringMe) const {
    stringOut << "resolution::CallInfoActual not stringified";
  }
};

template<> struct stringify<chpl::resolution::CallInfo>
{
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::CallInfo& stringMe) const {
    stringify<UniqueString> nameStringifier;
    stringOut << "resolution::CallInfo(";
    nameStringifier(stringOut, stringKind, stringMe.name());
    stringOut << "isMethod=" << stringMe.isMethod();
    stringOut << "numActuals=" << stringMe.numActuals();
    //types and names of actuals
    stringOut << ")";
    //stringOut << "resolution::CallInfo not stringified";
  }
};

template<> struct stringify<chpl::resolution::PoiInfo>
{
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::PoiInfo& stringMe) const {
    stringOut << "resolution::PoiInfo not stringified";
  }
};

template<> struct stringify<chpl::resolution::TypedFnSignature::WhereClauseResult>
{
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::TypedFnSignature::WhereClauseResult& stringMe) const {
    stringOut << "resolution::TypedFnSignature::WhereClauseResult not stringified";
  }
};

template<> struct stringify<chpl::resolution::UntypedFnSignature::FormalDetail>
{
 void operator()(std::ostream &stringOut,
                 StringifyKind stringKind,
                 const chpl::resolution::UntypedFnSignature::FormalDetail& stringMe) const {
   stringOut << "resolution::UntypedFnSignature::FormalDetail not stringified";
 }
};

template<> struct stringify<chpl::resolution::TypedFnSignature>
{
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::resolution::TypedFnSignature& stringMe) const {
    stringOut << "resolution::TypedFnSignature not stringified";
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
