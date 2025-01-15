/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_LOOP_EXPR_ITERATOR_TYPE_H
#define CHPL_TYPES_LOOP_EXPR_ITERATOR_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/types/IteratorType.h"

namespace chpl {
namespace types {

class LoopExprIteratorType final : public IteratorType {
 private:
  /*
    The type produced by this iterator. E.g., in a loop such
    as the right hand side in the below assignment:

       var A = foreach i in 1..10 do (i,i);

    the yield type is (int, int).
   */
  QualifiedType yieldType_;
  /*
     Whether the loop expression is zippered. In production, zippered
     loop expressions contain all of their iterands and separately invoke
     leader/follower iterators on each one
   */
  bool isZippered_;
  /*
     Whether this loop can be iterated over in parallel. Some loops
     ('for' and 'foreach') can't, even when their iterands support parallel
     iteration.
   */
  bool supportsParallel_;
  /*
     The type of the thing being iterated over, used for resolving the
     needed leader/follower iterators. If the loop is zippered, iterand
     should be a tuple type.
   */
  QualifiedType iterand_;
  /*
     The ID of the AST node that caused this loop type to be constructed.
     This is part of the type so that loop expressions created at different
     places are considered to have a different type.

     Note: this may need to change if we compiler-generated loop expressions,
     since the IDs of those would be nebulous.
   */
  ID sourceLocation_;

  LoopExprIteratorType(QualifiedType yieldType,
                       const resolution::PoiScope* poiScope,
                       bool isZippered,
                       bool supportsParallel,
                       QualifiedType iterand,
                       ID sourceLocation)
    : IteratorType(typetags::LoopExprIteratorType, poiScope),
      yieldType_(std::move(yieldType)),
      isZippered_(isZippered), supportsParallel_(supportsParallel),
      iterand_(std::move(iterand)), sourceLocation_(std::move(sourceLocation)) {
    if (isZippered_) {
      CHPL_ASSERT(iterand_.type() && iterand_.type()->isTupleType());
    }
  }

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (LoopExprIteratorType*) other;
    return iteratorTypeContentsMatchInner(rhs) &&
           isZippered_ == rhs->isZippered_ &&
           supportsParallel_ == rhs->supportsParallel_ &&
           iterand_ == rhs->iterand_ &&
           sourceLocation_ == rhs->sourceLocation_;
  }

  void markUniqueStringsInner(Context* context) const override {
    yieldType_.mark(context);
    iterand_.mark(context);
    sourceLocation_.mark(context);
  }

  static const owned<LoopExprIteratorType>&
  getLoopExprIteratorType(Context* context,
                          QualifiedType yieldType,
                          const resolution::PoiScope* poiScope,
                          bool isZippered,
                          bool supportsParallel,
                          QualifiedType iterand,
                          ID sourceLocation);

 public:
  static const LoopExprIteratorType* get(Context* context,
                                         QualifiedType yieldType,
                                         const resolution::PoiScope* poiScope,
                                         bool isZippered,
                                         bool supportsParallel,
                                         QualifiedType iterand,
                                         ID sourceLocation);

  const Type* substitute(Context* context,
                         const PlaceholderMap& subs) const override {
    return get(context, yieldType_.substitute(context, subs),
               poiScope_, isZippered_, supportsParallel_,
               iterand_.substitute(context, subs), sourceLocation_);
  }

  const QualifiedType& yieldType() const {
    return yieldType_;
  }

  bool isZippered() const {
    return isZippered_;
  }

  bool supportsParallel() const {
    return supportsParallel_;
  }

  const ID& sourceLocation() const {
    return sourceLocation_;
  }

  const QualifiedType& iterand() const {
    return iterand_;
  }
};

} // end namespace types
} // end namespace chpl

#endif
