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

#ifndef CHPL_AST_OPCALL_H
#define CHPL_AST_OPCALL_H

#include "chpl/uast/Call.h"

namespace chpl {
namespace uast {


/**
  This class represents a call to an operator.
 */
class OpCall final : public Call {
 private:
  // which operator
  UniqueString op_;

  bool matchesInner(const ASTBase* other) const override;
  void markUniqueStringsInner(Context* context) const override;
 public:
  ~OpCall() override = default;

  /** Returns the name of the operator called */
  UniqueString operatorName() const { return op_; }
};


} // end namespace uast
} // end namespace chpl

#endif
