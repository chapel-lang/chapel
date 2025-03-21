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

#include "chpl/uast/Attribute.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

owned<Attribute> Attribute::build(Builder* builder, Location loc,
                                  UniqueString name,
                                  AstList actuals,
                                  std::vector<UniqueString> actualNames) {
  int numActuals = actuals.size();

  Attribute* ret = new Attribute(name, numActuals, std::move(actuals),
                                 std::move(actualNames));

  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


void Attribute::dumpFieldsInner(const DumpSettings& s) const {
  s.out << " " << fullyQualifiedAttributeName();
  int i = 0;
  for (auto name : actualNames_) {
    if (!name.isEmpty()) {
      s.out << " actual " << i << " name= " << name.str();
    }
    i++;
  }
}

} // end namespace uast


} // end namespace chpl
