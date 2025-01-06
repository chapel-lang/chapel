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

#include "chpl/uast/VisibilityClause.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {

const char* VisibilityClause::limitationKindToString(LimitationKind kind) {
  switch (kind) {
    case BRACES: return "braces";
    case EXCEPT: return "except";
    case ONLY:   return "only";
    case NONE:   return "none";
  }
  return "<unknown>";
}

void VisibilityClause::dumpFieldsInner(const DumpSettings& s) const {
  const char* kindStr = limitationKindToString(limitationKind_);
  if (kindStr[0] != '\0') {
    s.out << " " << kindStr;
  }
}
std::string VisibilityClause::dumpChildLabelInner(int i) const {
  if (i == symbolChildNum_) {
    return "symbol";
  } else if (limitationChildNum_ <= i &&
             i < limitationChildNum_ + numLimitations_) {
    return "limitation " + std::to_string(i - limitationChildNum_);
  }
  return "";
}

owned<VisibilityClause>
VisibilityClause::build(Builder* builder, Location loc,
                        owned<AstNode> symbol,
                        LimitationKind limitationKind,
                        AstList limitations) {
  CHPL_ASSERT(symbol.get() != nullptr);

  AstList lst;
  int numLimitations = 0;

  lst.push_back(std::move(symbol));

  if (limitations.size()) {
    numLimitations = limitations.size();
    for (auto& ast : limitations) {
      lst.push_back(std::move(ast));
    }
  }

  VisibilityClause* ret = new VisibilityClause(std::move(lst),
                                               limitationKind,
                                               numLimitations);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<VisibilityClause>
VisibilityClause::build(Builder* builder, Location loc,
                        owned<AstNode> symbol) {
  return build(builder, loc, std::move(symbol),
               VisibilityClause::NONE,
               AstList());
}

} // namespace uast
} // namespace chpl
