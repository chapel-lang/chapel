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

#ifndef CHPL_UAST_ERRONEOUSEXP_H
#define CHPL_UAST_ERRONEOUSEXP_H

#include "chpl/uast/Exp.h"
#include "chpl/queries/Location.h"

namespace chpl {
namespace uast {


/**
  This class represents some missing AST due to an error.
 */
class ErroneousExp final : public Exp {
 private:
  ErroneousExp();
  bool contentsMatchInner(const ASTBase* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~ErroneousExp() = default;
  static owned<ErroneousExp> build(Builder* builder, Location loc);
};


} // end namespace uast
} // end namespace chpl

#endif
