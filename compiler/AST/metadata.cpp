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

#include "metadata.h"
#include "stringutil.h"

// default initialize by calling shared_ptr constructor
LLVMMetadata::LLVMMetadata(const char* key, LLVMMetadataType kind)
    : key(astr(key)), kind(kind), attribute_val(nullptr) {}

LLVMMetadata::~LLVMMetadata() {
  if(kind == LAT_ATTRIBUTE) {
    attribute_val.reset();
  }
}

LLVMMetadataPtr LLVMMetadata::construct(const char* key) {
  return std::make_shared<LLVMMetadata>(key, LAT_NO_VALUE);
}
LLVMMetadataPtr LLVMMetadata::constructInt(const char* key, int64_t val) {
  auto la = std::make_shared<LLVMMetadata>(key, LAT_INT);
  la->int_val = val;
  return la;
}
LLVMMetadataPtr LLVMMetadata::constructBool(const char* key, bool val) {
  auto la = std::make_shared<LLVMMetadata>(key, LAT_BOOL);
  la->bool_val = val;
  return la;
}
LLVMMetadataPtr LLVMMetadata::constructString(const char* key, const char* val) {
  auto la = std::make_shared<LLVMMetadata>(key, LAT_STRING);
  la->string_val = astr(val);
  return la;
}
LLVMMetadataPtr LLVMMetadata::constructMetadata(const char* key, LLVMMetadataPtr val) {
  auto la = std::make_shared<LLVMMetadata>(key, LAT_ATTRIBUTE);
  la->attribute_val = val;
  return la;
}
