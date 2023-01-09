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

#ifndef CHPL_UAST_ATTRIBUTE_H
#define CHPL_UAST_ATTRIBUTE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Pragma.h"
#include "chpl/util/iteration.h"
#include <set>

namespace chpl {
namespace uast {

class Attribute final: public AstNode {

private:
  UniqueString name_; // the attribute name - nodoc or deprecated for example
  std::vector<UniqueString> toolspace_; //outername.innername, for example
  // what's the purpose of a single actual index here?
  int8_t actualChildNum_;

  int numActuals_; // number of child actuals

  Attribute(UniqueString name, std::vector<UniqueString> toolspace,
            int8_t actualChildNum, int numActuals)
    : AstNode(asttags::Attribute),
      name_(name),
      toolspace_(std::move(toolspace)),
      actualChildNum_(actualChildNum),
      numActuals_(numActuals) {
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Attribute* lhs = this;
    const Attribute* rhs = (const Attribute*) other;
    return lhs->name_ == rhs->name_ &&
           lhs->toolspace_ == rhs->toolspace_ &&
           lhs->actualChildNum_ == rhs->actualChildNum_ &&
           lhs->numActuals_ == rhs->numActuals_;
  }

  void markUniqueStringsInner(Context* context) const override {
    name_.mark(context);
    for (auto& ts : toolspace_) {
      ts.mark(context);
    }
  }

  void dumpInner(const DumpSettings& s) const;

public:
  ~Attribute() override = default;

  static owned<Attribute> build(Builder* builder, Location loc,
                                UniqueString name,
                                std::vector<UniqueString> toolspace,
                                int8_t actualChildNum, int numActuals);




}; // end Attribute


} // end namespace uast


} // end namespace chpl

#endif //CHPL_UAST_ATTRIBUTE_H