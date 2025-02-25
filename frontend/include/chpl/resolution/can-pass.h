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

#ifndef CHPL_RESOLUTION_CAN_PASS_H
#define CHPL_RESOLUTION_CAN_PASS_H

#include "chpl/resolution/resolution-types.h"
#include "chpl/types/ClassTypeDecorator.h"

#include "llvm/Support/Error.h"

namespace chpl {
namespace uast {
  class AstNode;
}
namespace types {
  class QualifiedType;
  class ClassType;
  class TupleType;
}
namespace resolution {


class CanPassResult {
 public:
  enum ConversionKind {
    /** No implicit conversion is needed */
    NONE,
    /** A narrowing param conversion is needed.
        These are only applicable to the particular param
        value -- e.g. 1:int converting to int(8) because 1 fits in int(8).
        The input of such a conversion must be param and the result
        is always a param. */
    PARAM_NARROWING,
    /** A numeric or bool conversion. */
    NUMERIC,
    /** A conversion that implements subtyping */
    SUBTYPE,
    /** A conversion that borrows a managed type (without subtyping) */
    BORROWS,
    /** A conversion that implements subtyping AND borrows a managed type */
    BORROWS_SUBTYPE,
    /** Non-subtype conversion that doesn't produce a param */
    OTHER,
  };

 private:
  optional<PassingFailureReason> failReason_ = {};
  bool instantiates_ = false;
  bool promotes_ = false;
  ConversionKind conversionKind_ = NONE;

  CanPassResult(optional<PassingFailureReason> failReason, bool instantiates,
                bool promotes, ConversionKind kind)
    : failReason_(failReason), instantiates_(instantiates), promotes_(promotes),
      conversionKind_(kind) { }

  // these builders make it easier to implement canPass
  static CanPassResult fail(PassingFailureReason reason) {
    return CanPassResult(reason, false, false, NONE);
  }
  static CanPassResult passAsIs() {
    return CanPassResult({}, false, false, NONE);
  }
  static CanPassResult convert(ConversionKind e) {
    return CanPassResult({}, false, false, e);
  }
  static CanPassResult instantiate() {
    return CanPassResult({}, true, false, NONE);
  }
  static CanPassResult convertAndInstantiate(ConversionKind e) {
    return CanPassResult({}, true, false, e);
  }
  static CanPassResult promote(CanPassResult r) {
    CanPassResult ret = r;
    ret.promotes_ = true;
    return ret;
  }

  static bool isTypeGeneric(Context* context, const types::QualifiedType& qt);
  static bool isTypeGeneric(Context* context, const types::Type* t);

  static CanPassResult ensureSubtypeConversionInstantiates(CanPassResult r);

  static bool
  canConvertNumeric(Context* context,
                    const types::Type* actualT,
                    const types::Type* formalT);

  static bool
  canConvertCPtr(Context* context,
                 const types::Type* actualT,
                 const types::Type* formalT);

  static bool canConvertParamNarrowing(Context* context,
                                       const types::QualifiedType& actualType,
                                       const types::QualifiedType& formalType);

  static CanPassResult canPassDecorators(Context* context,
                                         types::ClassTypeDecorator actual,
                                         types::ClassTypeDecorator formal);

  static CanPassResult canPassSubtypeNonBorrowing(Context* context,
                                      const types::Type* actualT,
                                      const types::Type* formalT);

  static CanPassResult canPassSubtypeOrBorrowing(Context* context,
                                                 const types::Type* actualT,
                                                 const types::Type* formalT);

  static CanPassResult canConvertTuples(Context* context,
                                        const types::TupleType* aT,
                                        const types::TupleType* fT);

  static CanPassResult canConvert(Context* context,
                                  const types::QualifiedType& actualType,
                                  const types::QualifiedType& formalType);

  static bool canInstantiateBuiltin(Context* context,
                                    const types::Type* actualT,
                                    const types::Type* formalT);

  static CanPassResult canInstantiate(Context* context,
                                      const types::QualifiedType& actualType,
                                      const types::QualifiedType& formalType);

 public:
  CanPassResult() { }
  ~CanPassResult() = default;

  /** Returns true if the argument is passable */
  bool passes() const { return !failReason_; }

  PassingFailureReason reason() const {
    CHPL_ASSERT((bool) failReason_);
    return *failReason_;
  }

  /** Returns true if passing the argument will require instantiation */
  bool instantiates() const { return instantiates_; }

  /** Returns true if passing the argument will require promotion */
  bool promotes() const { return promotes_; }

  /** Returns true if implicit conversion is required */
  bool converts() const { return conversionKind_ != NONE; }

  /** What type of implicit conversion, if any, is needed? */
  ConversionKind conversionKind() const { return conversionKind_; }

  /** Returns true if an implicit param narrowing conversion is required */
  bool convertsWithParamNarrowing() const {
    return conversionKind_ == PARAM_NARROWING;
  }

  /** Returns true if an implicit borrowing conversion is required.
      Does not include borrowing with implicit subtyping. */
  bool convertsWithBorrowing() const { return conversionKind_ == BORROWS; }

  static CanPassResult canPassScalar(Context* context,
                                     const types::QualifiedType& actualType,
                                     const types::QualifiedType& formalType);

  // implementation of canPass to allow use of private fields
  static CanPassResult canPass(Context* context,
                               const types::QualifiedType& actualType,
                               const types::QualifiedType& formalType);

};

/**
  Helper function that handles the automatic coalescing of e.g. the `_owned`
  record implementing managed types and the `owned C` type used in user code.
  Only does this if mightBeManagerRecord is a `_owned`-like record type,
  and mightBeClass is a class type. In other words, only does something if
  the coalescing is necessary.

  Sets mightBeClass to its record equivalent if the conversion is possible,
  and returns true. Otherwise returns false.
 */
bool
tryConvertClassTypeIntoManagerRecordIfNeeded(Context* context,
                                             const types::Type* const & mightBeManagerRecord,
                                             const types::Type*& mightBeClass);

/**
  Given an argument with QualifiedType actualType,
  can that argument be passed to a formal with QualifiedType formalType?

  Note that a result with passes() and instantiates() indicates
  that the compiler should try instantiating. Once instantiation occurs,
  the compiler may figure out that the argument cannot be passed.
 */
static inline
CanPassResult canPass(Context* context,
                      const types::QualifiedType& actualType,
                      const types::QualifiedType& formalType) {
  return CanPassResult::canPass(context, actualType, formalType);
}

static inline
CanPassResult canPassScalar(Context* context,
                            const types::QualifiedType& actualType,
                            const types::QualifiedType& formalType) {
  return CanPassResult::canPassScalar(context, actualType, formalType);
}

/* Returns true if, all other things equal, a type with substitutions
   'instances' is an instantiation of a type with substitutions 'generics'.

   If 'allowMissing' is true, considers missing substitutions in 'generics'
   to be "any type". Otherwise, requires that each susbtitution in
   instances is matched by an existing substitution in generics. */
bool canInstantiateSubstitutions(Context* context,
                                 const SubstitutionsMap& instances,
                                 const SubstitutionsMap& generics,
                                 bool allowMissing);

/* When trying to combine two kinds, you can't just pick one.
   For instance, if any type in the list is a value, the result
   should be a value, and if any type in the list is const, the
   result should be const. Thus, combining `const ref` and `var`
   should result in `const var`.

   This class is used to describe the "mixing rules" of various kinds.
   To this end, it breaks them down into their properties (const-ness,
   ref-ness, etc) each of which are processed independently from
   the others. */
class KindProperties {
 private:
  bool isConst = false;
  bool isRef = false;
  bool isType = false;
  bool isParam = false;
  bool isValid = false;

  KindProperties() {}

  KindProperties(bool isConst, bool isRef, bool isType,
                 bool isParam)
    : isConst(isConst), isRef(isRef), isType(isType),
      isParam(isParam), isValid(true) {}

 private:
  void invalidate();

  /* Helper to check basic validity of combining two KindProperties. */
  bool checkValidCombine(const KindProperties& other) const;

 public:
  /* Decompose a qualified type kind into its properties. */
  static KindProperties fromKind(types::QualifiedType::Kind kind);

  /* Set the refness property to the given one. */
  void setRef(bool isRef);

  /* Set the paramness property to the given one. */
  void setParam(bool isParam);

  /* Set the constness property to the given one. */
  void setConst(bool isConst);

  /* Combine two sets of kind properties into this one. The resulting
     set of properties is compatible with both arguments (e.g. ref + val = val,
     since values can't be made into references).
     Takes the mathematical join with respect to constness (const + non-const = const). */
  void combineWithJoin(const KindProperties& other);

  /* Like combineWithJoin, but takes the mathematical meet with respect to
     constness (const + non-const = non-const). */
  void combineWithMeet(const KindProperties& other);

  /* Combine two sets of kind properties, strictly enforcing properties of
     the receiver (e.g. (receiver) param + (other) value = invalid, because
     param-ness is required).

     const-ness and ref-ness mismatch doesn't raise issues here since const/ref
     checking is a separate pass. */
  void strictCombineWith(const KindProperties& other);

  /* Combine the properties of two kinds, returning the result as a kind. */
  static types::QualifiedType::Kind combineKindsMeet(
      types::QualifiedType::Kind kind1,
      types::QualifiedType::Kind kind2);

  /* Convert the set of kind properties back into a kind. */
  types::QualifiedType::Kind toKind() const;

  bool valid() const { return isValid; }

  /* Creates a corresponding kind that is const */
  static types::QualifiedType::Kind makeConst(types::QualifiedType::Kind kind);

  /* Creates a corresponding kind that is not a reference */
  static types::QualifiedType::Kind removeRef(types::QualifiedType::Kind kind);
};

/**
  An optional additional constraint on the kind of a type. Used in
  commonType to serve the case of functions that enforce param, type,
  or const returns.
 */
using KindRequirement = optional<chpl::types::QualifiedType::Kind>;

/**
  Given a (non-empty) list of types (e.g., the types of various return
  statements in a function), determine the type, if any, that can be used to
  represent all of them. Returns an optional that contains the qualified
  type if one is found, or is empty otherwise.

  If useRequiredKind=true is specified, the requiredKind argument is treated
  as a strict constraint on the kinds of the given types. For instance,
  specifying requiredKind=PARAM and giving non-param types will
  result in failure to find a common type, even if the types can otherwise
  by unified.
 */
optional<chpl::types::QualifiedType>
commonType(Context* context,
           const std::vector<chpl::types::QualifiedType>& types,
           KindRequirement requiredKind = KindRequirement());
// QualifiedType fully qualified here to prevent "reference target not found"
// in Doxygen.


} // end namespace resolution
} // end namespace chpl

#endif
