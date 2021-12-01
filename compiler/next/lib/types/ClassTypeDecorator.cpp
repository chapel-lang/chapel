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

#include "chpl/types/ClassTypeDecorator.h"

namespace chpl {
namespace types {

using ClassTypeDecoratorEnum = ClassTypeDecorator::ClassTypeDecoratorEnum;

ClassTypeDecoratorEnum
ClassTypeDecorator::combineDecorators(ClassTypeDecoratorEnum formalDecorator,
                                      ClassTypeDecoratorEnum actualDecorator) {
  ClassTypeDecoratorEnum d = formalDecorator;

  // Combine management information
  if (isDecoratorUnknownManagement(formalDecorator)) {
    // Get the management from the other decorator, but get the
    // nilability from this one if it's specified.
    d = removeNilableFromDecorator(actualDecorator);
    if (isDecoratorNilable(formalDecorator))
      d = addNilableToDecorator(d);
    else if (isDecoratorNonNilable(formalDecorator))
      d = addNonNilToDecorator(d);
  }

  // Combine nilability information
  if (isDecoratorUnknownNilability(d)) {
    // If it's unknown, use the nilability from the other decorator
    if (isDecoratorNilable(actualDecorator))
      d = addNilableToDecorator(d);
    else if (isDecoratorNonNilable(actualDecorator))
      d = addNonNilToDecorator(d);
  }

  return d;
}

static
ClassTypeDecoratorEnum removeGenericNilability(ClassTypeDecoratorEnum actual) {
  // Normalize actuals to remove generic-ness
  if (actual == ClassTypeDecorator::BORROWED)
    actual = ClassTypeDecorator::BORROWED_NONNIL;
  if (actual == ClassTypeDecorator::UNMANAGED)
    actual = ClassTypeDecorator::UNMANAGED_NONNIL;
  if (actual == ClassTypeDecorator::MANAGED)
    actual = ClassTypeDecorator::MANAGED_NONNIL;

  return actual;
}

/* ClassTypeDecorator::BORROWED e.g. can represent any nilability,
   but this function assumes that an actual with type
   ClassTypeDecorator::BORROWED
   is actually the same as ClassTypeDecorator::BORROWED_NONNIL.
 */
bool ClassTypeDecorator::canCoerceDecorators(
                           ClassTypeDecoratorEnum actual,
                           ClassTypeDecoratorEnum formal,
                           bool allowNonSubtypes,
                           bool implicitBang) {

  if (actual == formal)
    return true;

  // Normalize actuals to remove generic-ness
  actual = removeGenericNilability(actual);

  if (actual == formal)
    return true;

  // Don't consider implicit coercion from nilable to non-nilable as subtyping.
  if (allowNonSubtypes == false)
    implicitBang = false;

  switch (formal) {
    case ClassTypeDecorator::BORROWED:
      // borrowed but generic nilability
      // This would be instantiation
      return false;
    case ClassTypeDecorator::BORROWED_NONNIL:
      // Can't coerce away nilable
      return isDecoratorNonNilable(actual) || implicitBang;
    case ClassTypeDecorator::BORROWED_NILABLE:
      // Everything can coerce to a nilable borrowed
      // but only subtypes if the actual is already nilable.
      return allowNonSubtypes || isDecoratorNilable(actual);
    case ClassTypeDecorator::UNMANAGED:
      // unmanaged but generic nilability
      // This would be instantiation
      return false;
    case ClassTypeDecorator::UNMANAGED_NONNIL:
      // Can't coerce away nilable
      // Can't coerce borrowed to unmanaged
      return (implicitBang && actual == ClassTypeDecorator::UNMANAGED_NILABLE);
    case ClassTypeDecorator::UNMANAGED_NILABLE:
      // Can't coerce borrowed to unmanaged
      return (allowNonSubtypes && actual == ClassTypeDecorator::UNMANAGED_NONNIL);

    case ClassTypeDecorator::MANAGED:
      // managed but generic nilability
      // this would be instantiation
      return false;
    case ClassTypeDecorator::MANAGED_NONNIL:
      // Can't coerce away nilable
      // Can't coerce borrowed to managed
      return (implicitBang && actual == ClassTypeDecorator::MANAGED_NILABLE);
    case ClassTypeDecorator::MANAGED_NILABLE:
      // Can't coerce borrowed to managed
      return (allowNonSubtypes && actual == ClassTypeDecorator::MANAGED_NONNIL);

    case ClassTypeDecorator::GENERIC:
      return false; // instantiation not coercion
    case ClassTypeDecorator::GENERIC_NONNIL:
      // generally instantiation
      return implicitBang && actual == ClassTypeDecorator::GENERIC_NILABLE;
    case ClassTypeDecorator::GENERIC_NILABLE:
      // generally instantiation
      return allowNonSubtypes && actual == ClassTypeDecorator::GENERIC_NONNIL;

    // no default for compiler warnings to know when to update it
  }

  return false;
}

// Returns true if actual has the same meaning as formal or
// if passing actual to formal should result in instantiation.
bool ClassTypeDecorator::canInstantiateDecorators(
                              ClassTypeDecoratorEnum actual,
                              ClassTypeDecoratorEnum formal) {

  if (actual == formal)
    return true;

  // Normalize actuals to remove generic-ness
  actual = removeGenericNilability(actual);

  if (actual == formal)
    return true;

  switch (formal) {
    case ClassTypeDecorator::BORROWED:
      return actual == ClassTypeDecorator::BORROWED_NONNIL ||
             actual == ClassTypeDecorator::BORROWED_NILABLE;
    case ClassTypeDecorator::BORROWED_NONNIL:
    case ClassTypeDecorator::BORROWED_NILABLE:
      return false;

    case ClassTypeDecorator::UNMANAGED:
      return actual == ClassTypeDecorator::UNMANAGED_NONNIL ||
             actual == ClassTypeDecorator::UNMANAGED_NILABLE;
    case ClassTypeDecorator::UNMANAGED_NONNIL:
    case ClassTypeDecorator::UNMANAGED_NILABLE:
      return false;

    case ClassTypeDecorator::MANAGED:
      return actual == ClassTypeDecorator::MANAGED_NONNIL ||
             actual == ClassTypeDecorator::MANAGED_NILABLE;
    case ClassTypeDecorator::MANAGED_NONNIL:
    case ClassTypeDecorator::MANAGED_NILABLE:
      return false;

    case ClassTypeDecorator::GENERIC:
      return true;
    case ClassTypeDecorator::GENERIC_NONNIL:
      return actual == ClassTypeDecorator::GENERIC_NONNIL ||
             actual == ClassTypeDecorator::BORROWED_NONNIL ||
             actual == ClassTypeDecorator::UNMANAGED_NONNIL ||
             actual == ClassTypeDecorator::MANAGED_NONNIL;
    case ClassTypeDecorator::GENERIC_NILABLE:
      return actual == ClassTypeDecorator::GENERIC_NILABLE ||
             actual == ClassTypeDecorator::BORROWED_NILABLE||
             actual == ClassTypeDecorator::UNMANAGED_NILABLE||
             actual == ClassTypeDecorator::MANAGED_NILABLE;

    // no default for compiler warnings to know when to update it
  }

  return false;
}

// Can we instantiate or coerce or both?
bool ClassTypeDecorator::canInstantiateOrCoerceDecorators(
                                      ClassTypeDecoratorEnum actual,
                                      ClassTypeDecoratorEnum formal,
                                      bool allowNonSubtypes,
                                      bool implicitBang) {
  if (actual == formal)
    return true;

  // Normalize actuals to remove generic-ness
  actual = removeGenericNilability(actual);

  if (actual == formal)
    return true;

  // Don't consider implicit coercion from nilable to non-nilable as subtyping.
  if (allowNonSubtypes == false)
    implicitBang = false;

  switch (formal) {
    case ClassTypeDecorator::BORROWED:
      // can borrow from anything, could instantiate as borrowed?
      return true;
    case ClassTypeDecorator::BORROWED_NONNIL:
      // can borrow from anything, but can't coerce away nilability
      return isDecoratorNonNilable(actual) || implicitBang;
    case ClassTypeDecorator::BORROWED_NILABLE:
      // can borrow from anything, can always coerce to nilable
      return allowNonSubtypes || isDecoratorNilable(actual);;

    case ClassTypeDecorator::UNMANAGED:
      // no coercions to unmanaged
      return actual == ClassTypeDecorator::UNMANAGED_NONNIL ||
             actual == ClassTypeDecorator::UNMANAGED_NILABLE;
    case ClassTypeDecorator::UNMANAGED_NONNIL:
      return (implicitBang && actual == ClassTypeDecorator::UNMANAGED_NILABLE);
    case ClassTypeDecorator::UNMANAGED_NILABLE:
      return (allowNonSubtypes && actual == ClassTypeDecorator::UNMANAGED_NONNIL);

    case ClassTypeDecorator::MANAGED:
      return actual == ClassTypeDecorator::MANAGED_NONNIL ||
             actual == ClassTypeDecorator::MANAGED_NILABLE;
    case ClassTypeDecorator::MANAGED_NONNIL:
      return (implicitBang && actual == ClassTypeDecorator::MANAGED_NILABLE);
    case ClassTypeDecorator::MANAGED_NILABLE:
      return (allowNonSubtypes && actual == ClassTypeDecorator::MANAGED_NONNIL);

    case ClassTypeDecorator::GENERIC:
      // accepts anything
      return true;
    case ClassTypeDecorator::GENERIC_NONNIL:
      // accepts anything nonnil
      return isDecoratorNonNilable(actual) || implicitBang;
    case ClassTypeDecorator::GENERIC_NILABLE:
      return allowNonSubtypes || isDecoratorNilable(actual);

    // no default for compiler warnings to know when to update it
  }

  return false;
}


} // end namespace types
} // end namespace chpl
