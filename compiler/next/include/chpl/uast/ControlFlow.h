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

#ifndef CHPL_UAST_CONTROLFLOW_H
#define CHPL_UAST_CONTROLFLOW_H

#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {


/**
  This abstract class represents some sort of control flow.
 */
class ControlFlow : public Expression {
 protected:
  ControlFlow(ASTTag tag) : Expression(tag) {}
  ControlFlow(ASTTag tag, ASTList children)
    : Expression(tag, std::move(children)) {};
  bool controlFlowContentsMatchInner(const ControlFlow* other) const {
    return this->expressionContentsMatchInner(other);
  }
  void controlFlowMarkUniqueStringsInner(Context* context) const {
    this->expressionMarkUniqueStringsInner(context);
  }

 public:
  virtual ~ControlFlow() override = 0; // this is an abstract base class
};


} // end namespace uast
} // end namespace chpl

#endif
