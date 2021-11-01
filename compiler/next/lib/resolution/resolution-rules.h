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

#ifndef RESOLUTION_RULES_H
#define RESOLUTION_RULES_H

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
    NONE, // no implicit conversion needed
    PARAM_NARROWING, // narrowing param conversion e.g. 1:int -> param int(8)
    PARAM, // non-narrowing param conversion
    SUBTYPE, // conversion to implement subtyping
    OTHER, // non-param non-subtype conversion
  };

 private:
  bool passes_ = false;
  bool instantiates_ = false;
  ConversionKind conversionKind_ = NONE;

 private:
  /** Returns true if the argument is passable */
  bool passes() { return passes_; }

  /** Returns true if passing the argument will require instantiation */
  bool instantiates() { return instantiates_; }

  /** What type of implicit conversion, if any, is needed? */
  ConversionKind conversionKind() { return conversionKind_; }
};

/**
  Given an argument with QualifiedType actualType,
  can that argument be passed to a formal with QualifiedType formalType?
 */
CanPassResult canPass(const types::QualifiedType& actualType,
                      const types::QualifiedType& formalType);


} // end namespace resolution
} // end namespace chpl

#endif
