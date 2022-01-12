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

#include "chpl/types/CompositeType.h"

#include "chpl/types/BasicClassType.h"

namespace chpl {
namespace types {


CompositeType::~CompositeType() {
}

void CompositeType::computeSummaryInformation() {
  isGeneric_ = false;
  allGenericFieldsHaveDefaultValues_ = true;
  for (auto field : fields_) {
    if (field.type.isGenericOrUnknown()) {
      if (!field.hasDefaultValue) {
        allGenericFieldsHaveDefaultValues_ = false;
      }
      isGeneric_ = true;
    }
  }

  // include also the parent class type for BasicClassType.
  if (auto bct = this->toBasicClassType()) {
    if (const Type* parentT = bct->parentClassType()) {
      if (parentT->isGeneric()) {
        isGeneric_ = true;
        if (auto pct = parentT->toBasicClassType()) {
          allGenericFieldsHaveDefaultValues_ = pct->isGenericWithDefaults();
        } else {
          allGenericFieldsHaveDefaultValues_ = false;
        }
      }
    }
  }
}

void CompositeType::stringify(std::ostream& ss,
                              chpl::StringifyKind stringKind) const {
  // compute the parent class type for BasicClassType
  const Type* superType = nullptr;
  if (auto bct = this->toBasicClassType()) {
    superType = bct->parentClassType();
  }

  //std::string ret = typetags::tagToString(tag());
  name().stringify(ss, stringKind);
  int nFields = numFields();

  if (superType || nFields > 0) {
    bool emittedField = false;
    ss << "(";
    if (superType) {
      ss << "super:";
      superType->stringify(ss, stringKind);
      emittedField = true;
    }

    for (int i = 0; i < nFields; i++) {
      if (emittedField) ss << ", ";
      fieldName(i).stringify(ss, stringKind);
      ss << ":";
      fieldType(i).stringify(ss, stringKind);
      emittedField = true;
    }
    ss << ")";
  }
}



} // end namespace types
} // end namespace chpl
