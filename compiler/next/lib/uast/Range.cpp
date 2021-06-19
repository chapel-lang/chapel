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

#include "chpl/uast/Range.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<Range> Range::build(Builder* builder, Location loc,
                          IntervalKind intervalKind,
                          owned<Expression> lowerBound,
                          owned<Expression> upperBound) {
  ASTList lst;
  int8_t lowerBoundChildNum = -1;
  int8_t upperBoundChildNum = -1;

  if (lowerBound.get() != nullptr) {
    lowerBoundChildNum = lst.size();
    lst.push_back(std::move(lowerBound));
  }
 
  if (upperBound.get() != nullptr) {
    upperBoundChildNum = lst.size();
    lst.push_back(std::move(upperBound));
  }

  Range* ret = new Range(std::move(lst), intervalKind,
                         lowerBoundChildNum,
                         upperBoundChildNum); 

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
