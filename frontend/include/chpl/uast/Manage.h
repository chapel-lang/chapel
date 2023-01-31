/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_MANAGE_H
#define CHPL_UAST_MANAGE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/As.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/SimpleBlockLike.h"

namespace chpl {
namespace uast {


/**
  This class represents a manage statement. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      manage myManager as res do writeln(res);

  \endrst

  This code will manage 'myManager', which returns a handle to a resource
  that can be referred to as 'res'.
 */
class Manage final : public SimpleBlockLike {
 private:
  int managerExprChildNum_;
  int numManagerExprs_;

  Manage(AstList stmts, int managerExprChildNum, int numManagerExprs,
         BlockStyle blockStyle,
         int bodyChildNum,
         int numBodyStmts)
    : SimpleBlockLike(asttags::Manage, std::move(stmts), blockStyle,
                      bodyChildNum,
                      numBodyStmts),
      managerExprChildNum_(managerExprChildNum),
      numManagerExprs_(numManagerExprs) {
    CHPL_ASSERT(0 <= managerExprChildNum_);
    CHPL_ASSERT(managerExprChildNum_ < numChildren());

    #ifndef NDEBUG
      for (auto mgr : managers()) {
        if (auto as = mgr->toAs()) CHPL_ASSERT(as->rename()->isVariable());
      }
    #endif
  }

  Manage(Deserializer& des)
    : SimpleBlockLike(asttags::Manage, des) {
      managerExprChildNum_ = des.read<int>();
      numManagerExprs_ = des.read<int>();
    }


  bool contentsMatchInner(const AstNode* other) const override {
    const Manage* lhs = this;
    const Manage* rhs = (const Manage*) other;
    return lhs->managerExprChildNum_ == rhs->managerExprChildNum_ &&
        lhs->numManagerExprs_ == rhs->numManagerExprs_ &&
        lhs->simpleBlockLikeContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    simpleBlockLikeMarkUniqueStringsInner(context);
  }

  std::string dumpChildLabelInner(int i) const override;

 public:
  ~Manage() override = default;

  /**
    Create and return a Manage containing the passed managers and statements.
  */
  static owned<Manage> build(Builder* builder, Location loc,
                             AstList managers,
                             BlockStyle blockStyle,
                             AstList stmts);

  /**
    Iterate over the managers of this manage statement. They may be either
    expressions or 'As' expressions. If a manager is an 'As' expression,
    the right-hand-side component is guaranteed to be a variable
    representing the managed resource.
  */
  AstListIteratorPair<AstNode> managers() const {
    auto begin = children_.begin() + managerExprChildNum_;
    auto end = begin + numManagerExprs_;
    return AstListIteratorPair<AstNode>(begin, end);
  }

  /**
    Return the number of managers in this manage statement.
  */
  int numManagers() const {
    return numManagerExprs_;
  }

  /**
    Return the i'th manager in this manage statement.
  */
  const AstNode* manager(int i) const {
    CHPL_ASSERT(0 <= i && i < numManagerExprs_);
    auto ret = child(managerExprChildNum_ + i);
    return ret;
  }

  void serialize(Serializer& ser) const override {
    SimpleBlockLike::serialize(ser);
    ser.write(managerExprChildNum_);
    ser.write(numManagerExprs_);
  }

  DECLARE_STATIC_DESERIALIZE(Manage);

};


} // end namespace uast
} // end namespace chpl

#endif
