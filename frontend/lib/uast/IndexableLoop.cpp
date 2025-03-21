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

#include "chpl/uast/IndexableLoop.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void IndexableLoop::dumpFieldsInner(const DumpSettings& s) const {
  if (isExpressionLevel_) {
    s.out << " expr";
  }
  return Loop::dumpFieldsInner(s);
}

std::string IndexableLoop::dumpChildLabelInner(int i) const {
  if (i == indexChildNum_) {
    return "index";
  } else if (i == iterandChildNum_) {
    return "iterand";
  } else if (i == withClauseChildNum_) {
    return "with";
  }

  return Loop::dumpChildLabelInner(i);
}

IndexableLoop::~IndexableLoop(){
}


} // namespace uast
} // namespace chpl
