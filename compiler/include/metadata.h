/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifndef _METADATA_H_
#define _METADATA_H_

#include <memory>
#include <cstdint>
#include <vector>
#include "genret.h"

enum LLVMMetadataType {
  LAT_NO_VALUE, LAT_INT, LAT_BOOL, LAT_STRING, LAT_ATTRIBUTE
};
struct LLVMMetadata;
using LLVMMetadataPtr = std::shared_ptr<LLVMMetadata>;
using LLVMMetadataList = std::vector<LLVMMetadataPtr>;
struct LLVMMetadata {
  const char* key;
  // TODO: This absolutely should be a C++17 variant
  LLVMMetadataType kind;
  union {
    int64_t int_val;
    bool bool_val;
    const char* string_val;
    LLVMMetadataPtr attribute_val;
  };
  LLVMMetadata(const char* key, enum LLVMMetadataType kind);
  ~LLVMMetadata();

  static LLVMMetadataPtr construct(const char*);
  static LLVMMetadataPtr constructInt(const char*, int64_t);
  static LLVMMetadataPtr constructBool(const char*, bool);
  static LLVMMetadataPtr constructString(const char*, const char*);
  static LLVMMetadataPtr constructMetadata(const char*, LLVMMetadataPtr);

  GenRet codegen();
};

#endif
