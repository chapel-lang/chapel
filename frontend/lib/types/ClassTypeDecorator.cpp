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


} // end namespace types
} // end namespace chpl
