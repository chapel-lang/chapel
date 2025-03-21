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

#include "chpl/uast/Decl.h"

namespace chpl {
namespace uast {


Decl::~Decl() {
}

void Decl::dumpFieldsInner(const DumpSettings& s) const {
  const char* v = visibilityToString(visibility_);
  const char* k = linkageToString(linkage_);
  if (v[0] != 0) {
    s.out << " " << v;
  }
  if (k[0] != 0) {
    s.out << " " << k;
  }
}
std::string Decl::dumpChildLabelInner(int i) const {
  if (i == linkageNameChildNum_) {
    return "linkage-name";
  }

  return "";
}

const char* Decl::visibilityToString(Visibility v) {
  switch (v) {
    case Visibility::DEFAULT_VISIBILITY: return "";
    case Visibility::PUBLIC:             return "public";
    case Visibility::PRIVATE:            return "private";
  }
  CHPL_ASSERT(false);
  return "<unknown>";
}


const char* Decl::linkageToString(Linkage x) {
  switch (x) {
    case Linkage::DEFAULT_LINKAGE: return "";
    case Linkage::EXTERN:          return "extern";
    case Linkage::EXPORT:          return "export";
  }
  CHPL_ASSERT(false);
  return "<unknown>";
}


} // namespace uast
} // namespace chpl
