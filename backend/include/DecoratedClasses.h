/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _DECORATED_CLASSES_H_
#define _DECORATED_CLASSES_H_

#include "chpl/types/ClassTypeDecorator.h"

using ClassTypeDecorator = chpl::types::ClassTypeDecorator;
using ClassTypeDecoratorEnum = ClassTypeDecorator::ClassTypeDecoratorEnum;

#define NUM_PACKED_DECORATED_TYPES 6

static inline
ClassTypeDecoratorEnum removeNilableFromDecorator(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::removeNilableFromDecorator(d);
}
static inline
ClassTypeDecoratorEnum addNonNilToDecorator(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::addNonNilToDecorator(d);
}
static inline
ClassTypeDecoratorEnum addNilableToDecorator(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::addNilableToDecorator(d);
}
static inline bool isDecoratorUnknownNilability(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::isDecoratorUnknownNilability(d);
}
static inline bool isDecoratorNonNilable(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::isDecoratorNonNilable(d);
}
static inline bool isDecoratorNilable(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::isDecoratorNilable(d);
}
static inline bool isDecoratorBorrowed(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::isDecoratorBorrowed(d);
}
static inline bool isDecoratorUnmanaged(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::isDecoratorUnmanaged(d);
}
static inline bool isDecoratorManaged(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::isDecoratorManaged(d);
}
static inline bool isDecoratorUnknownManagement(ClassTypeDecoratorEnum d) {
  return ClassTypeDecorator::isDecoratorUnknownManagement(d);
}
static inline
ClassTypeDecoratorEnum combineDecorators(ClassTypeDecoratorEnum formalD,
                                         ClassTypeDecoratorEnum actualD) {
  return ClassTypeDecorator::combineDecorators(formalD, actualD);
}

const char* decoratedTypeAstr(ClassTypeDecoratorEnum d, const char* className);

#endif
