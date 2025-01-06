/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/VarLikeDecl.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void VarLikeDecl::dumpFieldsInner(const DumpSettings& s) const {
  const char* kind = qualifierToString(storageKind_);
  if (storageKind_ == Qualifier::CONST_VAR) {
    kind = "const";
  }
  s.out << " " << kind;
  NamedDecl::dumpFieldsInner(s);
}

std::string VarLikeDecl::dumpChildLabelInner(int i) const {
  if (i == typeExpressionChildNum_) {
    return "type";
  } else if (i == initExpressionChildNum_) {
    return "init";
  }

  return NamedDecl::dumpChildLabelInner(i);
}


VarLikeDecl::~VarLikeDecl() {
}


} // namespace uast
} // namespace chpl
