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

#ifndef CHPL_RESOLUTION_CAN_PASS_H
#define CHPL_RESOLUTION_CAN_PASS_H

#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace uast {
  class ASTNode;
}
namespace types {
  class QualifiedType;
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
    /** Non-subtype conversion that doesn't produce a param */
    OTHER,
  };

 private:
  bool passes_ = false;
  bool instantiates_ = false;
  bool promotes_ = false;
  ConversionKind conversionKind_ = NONE;

  CanPassResult(bool passes, bool instantiates, bool promotes,
                ConversionKind kind)
    : passes_(passes), instantiates_(instantiates), promotes_(promotes),
      conversionKind_(kind) { }

  // these builders make it easier to implement canPass
  static CanPassResult fail() {
    return CanPassResult(false, false, false, NONE);
  }
  static CanPassResult passAsIs() {
    return CanPassResult(true, false, false, NONE);
  }
  static CanPassResult convert(ConversionKind e) {
    return CanPassResult(true, false, false, e);
  }
  static CanPassResult instantiate() {
    return CanPassResult(true, true, false, NONE);
  }
  static CanPassResult convertAndInstantiate(ConversionKind e) {
    return CanPassResult(true, true, false, e);
  }
  static CanPassResult promote(CanPassResult r) {
    CanPassResult ret = r;
    ret.promotes_ = true;
    return ret;
  }

  static bool
  canConvertNumeric(const types::Type* actualT,
                    const types::Type* formalT);
  static bool
  canConvertParamNarrowing(const types::QualifiedType& actualType,
                           const types::QualifiedType& formalType);
  static CanPassResult
  canConvertClassesOrPtrs(const types::QualifiedType& actualType,
                          const types::QualifiedType& formalType);

 public:
  CanPassResult() { }
  ~CanPassResult() = default;

  /** Returns true if the argument is passable */
  bool passes() { return passes_; }

  /** Returns true if passing the argument will require instantiation */
  bool instantiates() { return instantiates_; }

  /** Returns true if passing the argument will require promotion */
  bool promotes() { return promotes_; }

  /** Returns true if implicit conversion is required */
  bool converts() { return conversionKind_ != NONE; }

  /** What type of implicit conversion, if any, is needed? */
  ConversionKind conversionKind() { return conversionKind_; }

  // implementation of canPass to make it easier to access private fields
  static CanPassResult canPass(const types::QualifiedType& actualType,
                               const types::QualifiedType& formalType);
};

/**
  Given an argument with QualifiedType actualType,
  can that argument be passed to a formal with QualifiedType formalType?
 */
static inline
CanPassResult canPass(const types::QualifiedType& actualType,
                      const types::QualifiedType& formalType) {
  return CanPassResult::canPass(actualType, formalType);
}


} // end namespace resolution
} // end namespace chpl

#endif
