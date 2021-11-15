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

#include "chpl/types/CompositeType.h"

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
}

std::string CompositeType::toString() const {
  //std::string ret = typetags::tagToString(tag());
  std::string ret = name().toString();
  int nFields = numFields();
  ret += "(";
  for (int i = 0; i < nFields; i++) {
    if (i != 0) ret += ", ";
    ret += fieldName(i).toString();
    ret += ":";
    ret += fieldType(i).toString();
  }
  ret += ")";
  return ret;
}



} // end namespace types
} // end namespace chpl
