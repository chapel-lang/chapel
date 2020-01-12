/*
 * Copyright 2004-2020 Cray Inc.
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

typedef enum {
  // When updating, make sure that these numbers work with the masks below
  // (last two bits are generic nilable / non-nilable / nilable)
  CLASS_TYPE_BORROWED          = 0,
  CLASS_TYPE_BORROWED_NONNIL   = 1,
  CLASS_TYPE_BORROWED_NILABLE  = 2,
  CLASS_TYPE_UNMANAGED         = 4,
  CLASS_TYPE_UNMANAGED_NONNIL  = 5,
  CLASS_TYPE_UNMANAGED_NILABLE = 6,
  CLASS_TYPE_MANAGED           = 8, // refers to dtOwned/etc
  CLASS_TYPE_MANAGED_NONNIL    = 9,  
  CLASS_TYPE_MANAGED_NILABLE   = 10,
  CLASS_TYPE_GENERIC           = 12,
  CLASS_TYPE_GENERIC_NONNIL    = 13,
  CLASS_TYPE_GENERIC_NILABLE   = 14,
} ClassTypeDecorator;
#define NUM_DECORATED_CLASS_TYPES (11)
#define CLASS_TYPE_MANAGEMENT_MASK (0xfc)
#define CLASS_TYPE_NILABILITY_MASK (0x03)
#define NUM_PACKED_DECORATED_TYPES 6

static inline ClassTypeDecorator removeNilableFromDecorator(ClassTypeDecorator d) {
  int tmp = d;
  tmp &= CLASS_TYPE_MANAGEMENT_MASK;
  return (ClassTypeDecorator) tmp;
}
static inline ClassTypeDecorator addNonNilToDecorator(ClassTypeDecorator d) {
  int tmp = removeNilableFromDecorator(d);
  tmp |= 1; 
  return (ClassTypeDecorator) tmp;
} 
static inline ClassTypeDecorator addNilableToDecorator(ClassTypeDecorator d) {
  int tmp = removeNilableFromDecorator(d);
  tmp |= 2;
  return (ClassTypeDecorator) tmp;
}
static inline bool isDecoratorUnknownNilability(ClassTypeDecorator d) {
  return (d & CLASS_TYPE_NILABILITY_MASK) == 0;
}
static inline bool isDecoratorNonNilable(ClassTypeDecorator d) {
  return (d & CLASS_TYPE_NILABILITY_MASK) == 1;
}
static inline bool isDecoratorNilable(ClassTypeDecorator d) {
  return (d & CLASS_TYPE_NILABILITY_MASK) == 2;
}
static inline bool isDecoratorManaged(ClassTypeDecorator d) {
  return removeNilableFromDecorator(d) == CLASS_TYPE_MANAGED;
}
static inline bool isDecoratorUnknownManagement(ClassTypeDecorator d) {
  return removeNilableFromDecorator(d) == CLASS_TYPE_GENERIC;
}

const char* decoratedTypeAstr(ClassTypeDecorator d, const char* className);

ClassTypeDecorator combineDecorators(ClassTypeDecorator formalDecorator,
                                     ClassTypeDecorator actualDecorator);

#endif
