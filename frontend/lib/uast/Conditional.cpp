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

#include "chpl/uast/Conditional.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


void Conditional::dumpFieldsInner(const DumpSettings& s) const {
  if (isExpressionLevel_) {
    s.out << " expr";
  }
}
std::string Conditional::dumpChildLabelInner(int i) const {
  if (i == conditionChildNum_) {
    return "condition";
  } else if (i == thenBodyChildNum_) {
    return "then";
  } else if (i == elseBodyChildNum_) {
    return "else";
  }

  return "";
}

owned<Conditional> Conditional::build(Builder* builder, Location loc,
                                      owned<AstNode> condition,
                                      BlockStyle thenBlockStyle,
                                      owned<Block> thenBlock,
                                      BlockStyle elseBlockStyle,
                                      owned<Block> elseBlock,
                                      bool isExpressionLevel) {
  CHPL_ASSERT(condition.get() != nullptr);

  AstList lst;

  lst.push_back(std::move(condition));
  lst.push_back(std::move(thenBlock));

  if (elseBlock.get() != nullptr) {
    lst.push_back(std::move(elseBlock));
  }

  Conditional* ret = new Conditional(std::move(lst),
                                     thenBlockStyle,
                                     elseBlockStyle,
                                     isExpressionLevel);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<Conditional> Conditional::build(Builder* builder, Location loc,
                                      owned<AstNode> condition,
                                      BlockStyle thenBlockStyle,
                                      owned<Block> thenBlock,
                                      bool isExpressionLevel) {
  CHPL_ASSERT(condition.get() != nullptr);
  AstList lst;

  lst.push_back(std::move(condition));
  lst.push_back(std::move(thenBlock));

  const BlockStyle elseBlockStyle = BlockStyle::IMPLICIT;

  Conditional* ret = new Conditional(std::move(lst),
                                     thenBlockStyle,
                                     elseBlockStyle,
                                     isExpressionLevel);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

} // namespace uast
} // namespace chpl
