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

#ifndef CHPL_AST_CALL_H
#define CHPL_AST_CALL_H

#include "chpl/AST/Exp.h"

#include <vector>

namespace chpl {
namespace ast {

/**
  This abstract class represents something call-like
 */
class Call : public Exp {
 public:
  ~Call() override = 0;

  // note: the reason for the +/- 1 below is that the
  // 0'th child is the called expression, which does
  // not count as an "actual".

  int numActuals() const {
    return this->numChildren() - 1;
  }
  const Exp* actual(int i) const {
    return this->getChild(i+1);
  }
  Exp* baseExpr() const {
    return this->getChild(0);
  }
};

} // end namespace ast
} // end namespace chpl

#endif
