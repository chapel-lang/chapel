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

#ifndef CHPL_UAST_QUALIFIER_H
#define CHPL_UAST_QUALIFIER_H

#include <string> // to get the definition of std::hash
#include "chpl/framework/serialize-functions.h"

namespace chpl {
namespace uast {


/**
  A centralized list containing all intent and storage specifiers.
*/
enum struct Qualifier {
  /** Represents an unknown intent / variable type. This
      is only used within the compiler. */
  UNKNOWN,

  // 'generic' intents

  /** Represents the default intent for a type */
  DEFAULT_INTENT,

  /** Represents a const in / const ref depending on the type */
  CONST_INTENT,

  /** Represents a formal that will be passed with 'ref' or 'const ref'
      depending on subsequent analysis. */
  REF_MAYBE_CONST,

  // 'concrete' intents

  /** Represents a mutable variable declared with 'var' */
  VAR,

  /** Represents a variable declared with 'const' which
      means that the variable is immutable for its lifetime
      (but note: contained pointers can still point to mutable memory). */
  CONST_VAR,

  /** Represents a const reference - the pointed-to value
      can't be changed through this reference (but it could
      be changed by code referring to it in another way). */
  CONST_REF,

  /** Represents a mutable reference - the pointed-to value
      can be changed through this reference. */
  REF,

  /** Represents an 'in' intent formal that is a mutable value */
  IN,

  /** Represents an 'const in' intent formal that is an immutable value */
  CONST_IN,

  /** Represents an 'out' intent formal that is mutable */
  OUT,

  /** Represents an 'inout' intent formal that is mutable */
  INOUT,

  /** A 'param' variable or formal */
  PARAM,

  /** A 'type' variable or formal */
  TYPE,

  // intents used within the compiler that aren't
  // used in the uAST itself

  /** A type query (e.g. ?t in `proc f(arg: ?t)`) */
  TYPE_QUERY,

  /** A loop index variable */
  INDEX,

  /** A function */
  FUNCTION,

  /** A parenless function */
  PARENLESS_FUNCTION,

  /** A module */
  MODULE,

  /** An 'imaginary' actual to 'init''s this to represent the type being constructed. */
  INIT_RECEIVER,
};

/** Returns 'true' for qualifiers that are generic such as DEFAULT_INTENT */
bool isGenericQualifier(Qualifier kind);

/** Returns 'true' for qualifiers that are known to be const,
    that is, cannot be modified directly, (but might
    be modified by some other aliasing variable).
    Returns 'false' for qualifiers where it is unknown if it is const or not
    (such as DEFAULT_INTENT).
 */
bool isConstQualifier(Qualifier kind);

/** Returns 'true' for qualifiers that are known to be immutable,
    that is, cannot be modified by any task or reference, including
    the current reference.
    Returns 'false' for qualifiers where it is unknown if it is immutable
    (such as DEFAULT_INTENT).
 */
bool isImmutableQualifier(Qualifier kind);

/** Returns 'true' for qualifiers that are 'ref' or 'const ref',
    and returns 'false' for qualifiers where it is unknown if it is ref or
    not (such as DEFAULT_INTENT).
 */
bool isRefQualifier(Qualifier kind);

/** Returns 'true' for qualifiers that are 'in' or 'const in'.
    Returns 'false' for qualifiers where it is unknown if it is 'const in'
    or not, such as DEFAULT_INTENT. */
bool isInQualifier(Qualifier kind);

/** Returns a string describing a Qualifier */
const char* qualifierToString(Qualifier intent);


} // end namespace uast

DECLARE_SERDE_ENUM(uast::Qualifier, uint8_t);

} // end namespace chpl

namespace std {

template<> struct hash<chpl::uast::Qualifier>
{
  size_t operator()(const chpl::uast::Qualifier& key) const {
    return (size_t) key;
  }
};

} // end namespace std

#endif
