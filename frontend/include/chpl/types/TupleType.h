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

#ifndef CHPL_TYPES_TUPLE_TYPE_H
#define CHPL_TYPES_TUPLE_TYPE_H

#include "chpl/types/CompositeType.h"

namespace chpl {
namespace types {


/**
  This class represents a tuple type e.g. `(int, real)`.
 */
class TupleType final : public CompositeType {
 private:
  bool isStarTuple_ = false; // i.e. all elements have the same type
  bool isVarArgTuple_ = false;
  bool isKnownSize_ = true;
  bool isParamKnown_ = false;

  void computeIsStarTuple();
  void computeIsParamKnown();

  TupleType(ID id, UniqueString name,
            const TupleType* instantiatedFrom,
            SubstitutionsMap subs,
            bool isVarArgTuple)
    : CompositeType(typetags::TupleType, id, name,
                    instantiatedFrom, std::move(subs)),
      isVarArgTuple_(isVarArgTuple)
  {
    // Let a single entry in the SubstitutionsMap with a postOrderId of '-1'
    // represent the star-type of a tuple with an unknown size. This is a
    // useful representation for VarArgs.
    if (subs_.size() == 1) {
      auto& elt = subs_.begin()->first;
      if (elt.postOrderId() == -1) {
        isKnownSize_ = false;
      }
    }
    computeIsStarTuple();
    computeIsParamKnown();
  }


  bool contentsMatchInner(const Type* other) const override {
    const TupleType* rhs = (const TupleType*) other;
    return isStarTuple_ == rhs->isStarTuple_ &&
           compositeTypeContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    compositeTypeMarkUniqueStringsInner(context);
  }

  static const owned<TupleType>&
  getTupleType(Context* context, ID id, UniqueString name,
               const TupleType* instantiatedFrom,
               SubstitutionsMap subs,
               bool isVarArgTuple = false);

 public:
  /** Return a value tuple type based on the vector of actual types. */
  static const TupleType*
  getValueTuple(Context* context, std::vector<const Type*> eltTypes);

  /** Return a referential tuple type based on the vector of actual types. */
  static const TupleType*
  getReferentialTuple(Context* context, std::vector<const Type*> eltTypes);

  static const TupleType*
  getQualifiedTuple(Context* context, std::vector<QualifiedType> eltTypes);

  static const TupleType*
  getStarTuple(Context* context,
               QualifiedType paramSize,
               QualifiedType starEltType);

  /** Return the generic tuple type `_tuple` */
  static const TupleType* getGenericTupleType(Context* context);

  ~TupleType() = default;

  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;

  /** Returns the number of tuple elements.
      Returns '-1' if the size is not known. */
  int numElements() const {
    if (isKnownSize_) {
      return subs_.size();
    } else {
      return -1;
    }
  }

  /** Return the type of the i'th element */
  QualifiedType elementType(int i) const;

  /** Return true if this is a *star tuple* - that is, all of the
      element types are the same. */
  bool isStarTuple() const { return isStarTuple_; }

  /** Return true if this tuple's elements are all non-generic
      params, e.g., (param b: bool = true, param i: int = 0). */
  bool isParamKnown() const { return isParamKnown_; }

  bool isVarArgTuple() const { return isVarArgTuple_; }

  bool isKnownSize() const { return isKnownSize_; }

  QualifiedType starType() const;

  /** Return the value tuple variant of this tuple type */
  const TupleType* toValueTuple(Context* context) const;

  /** Return the referential tuple variant of this tuple type */
  const TupleType* toReferentialTuple(Context* context) const;
};


} // end namespace uast
} // end namespace chpl

#endif
