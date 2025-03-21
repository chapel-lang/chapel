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

#include "chpl/uast/Range.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


const char* Range::opKindToString(OpKind kind) {
  switch (kind) {
    case DEFAULT: return "";
    case OPEN_HIGH: return "open-high";
  }

  return "<unknown>";
}

void Range::dumpFieldsInner(const DumpSettings& s) const {
  const char* kindStr = opKindToString(opKind_);
  if (kindStr[0] != '\0') {
    s.out << " " << kindStr;
  }
}
std::string Range::dumpChildLabelInner(int i) const {
  if (i == lowerBoundChildNum_) {
    return "lower";
  } else if (i == upperBoundChildNum_) {
    return "upper";
  }

  return "";
}

owned<Range> Range::build(Builder* builder, Location loc,
                          OpKind opKind,
                          owned<AstNode> lowerBound,
                          owned<AstNode> upperBound) {
  AstList lst;
  int8_t lowerBoundChildNum = NO_CHILD;
  int8_t upperBoundChildNum = NO_CHILD;

  if (lowerBound.get() != nullptr) {
    lowerBoundChildNum = lst.size();
    lst.push_back(std::move(lowerBound));
  }

  if (upperBound.get() != nullptr) {
    upperBoundChildNum = lst.size();
    lst.push_back(std::move(upperBound));
  }

  Range* ret = new Range(std::move(lst), opKind,
                         lowerBoundChildNum,
                         upperBoundChildNum);

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
