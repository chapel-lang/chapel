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

#ifndef CHPL_UAST_LABEL_H
#define CHPL_UAST_LABEL_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/SimpleBlockLike.h"

namespace chpl {
namespace uast {


/**
  This class represents a label. For example:

  \rst
  .. code-block:: chapel

    for 0..15 {
      while true do
        if !(i % 2) then break print; else break end;
      label print writeln(i);
      label end;
    }
 
  \endrst

*/
class Label final : public SimpleBlockLike {
 private:
  Label(ASTList children, UniqueString name, BlockStyle blockStyle,
        int bodyChildNum,
        int numBodyStmts)

    : SimpleBlockLike(asttags::Label, std::move(children),
                      blockStyle,
                      bodyChildNum,
                      numBodyStmts),
      name_(name) {
    assert(isExpressionASTList(children_));
  }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

  UniqueString name_;

 public:
  ~Label() override = default;

  /**
    Create and return a label statement.
  */
  static owned<Label> build(Builder* builder, Location loc,
                            UniqueString name,
                            BlockStyle blockStyle,
                            ASTList stmts);

  /**
    Return the name of this label statement.
  */
  UniqueString name() const {
    return name_;
  }

};


} // end namespace uast
} // end namespace chpl

#endif
