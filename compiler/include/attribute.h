/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

#include <memory>
#include <cstdint>
#include <vector>

enum LLVMAttributeType {
  LAT_NO_VALUE, LAT_INT, LAT_BOOL, LAT_STRING, LAT_ATTRIBUTE
};
struct LLVMAttribute;
using LLVMAttributePtr = LLVMAttribute*;
using LLVMAttributeList = std::vector<LLVMAttributePtr>;
struct LLVMAttribute {
  const char* key;
  LLVMAttributeType kind;
  union {
    int64_t int_val;
    bool bool_val;
    const char* string_val;
    LLVMAttributePtr attribute_val;
  };
  LLVMAttribute(const char* key, enum LLVMAttributeType kind);
  static LLVMAttributePtr construct(const char*);
  static LLVMAttributePtr constructInt(const char*, int64_t);
  static LLVMAttributePtr constructBool(const char*, bool);
  static LLVMAttributePtr constructString(const char*, const char*);
  static LLVMAttributePtr constructAttribute(const char*, LLVMAttributePtr);
};

#endif
