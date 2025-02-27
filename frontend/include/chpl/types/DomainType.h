/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_DOMAIN_TYPE_H
#define CHPL_TYPES_DOMAIN_TYPE_H

#include "chpl/types/CompositeType.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace types {

/**
  This class represents a domain type e.g. `domain(1)`.

  NB: This type's representation is a work-in-progress and subject to change.

  In practice this type is represented by the ``_domain`` record in
  ChapelDomain, but does not follow all of the normal record rules. The record
  implementation primarily exists as a composite type with some fields and on
  which we can call methods.


  Furthermore, this type currently represents its type and param properties
  entirely in the compiler, i.e., not as fields in _domain. These properties
  are stored in the SubstitutionsMap for this type.

TODO: Should _domain store type and param fields directly for 'idxType', etc.?
 */
class DomainType final : public CompositeType {
 public:
   enum Kind {
     Rectangular,
     Associative,
     Sparse,
     Subdomain,
     Unknown
   };

 private:
  // TODO: distributions
  Kind kind_;

  // Will compute idxType, rank, and strides from 'subs'
  DomainType(ID id, UniqueString name,
             const DomainType* instantiatedFrom,
             SubstitutionsMap subs,
             Kind kind)
    : CompositeType(typetags::DomainType, id, name,
                    instantiatedFrom, std::move(subs)),
      kind_(kind)
  {
  }


  bool contentsMatchInner(const Type* other) const override {
    const DomainType* rhs = (const DomainType*) other;
    return kind_ == rhs->kind_ &&
           compositeTypeContentsMatchInner(rhs);
  }

  void markUniqueStringsInner(Context* context) const override {
    compositeTypeMarkUniqueStringsInner(context);
  }

  static const owned<DomainType>&
  getDomainType(Context* context, ID id, UniqueString name,
               const DomainType* instantiatedFrom,
               SubstitutionsMap subs,
               Kind kind = Kind::Unknown);

  static const ID rankId;
  static const ID rectangularIdxTypeId;
  static const ID nonRectangularIdxTypeId;
  static const ID stridesId;
  static const ID parSafeId;
  static const ID parentDomainId;

 public:

  /** Return the generic domain type */
  static const DomainType* getGenericDomainType(Context* context);

  /** Return a rectangular domain type */
  static const DomainType* getRectangularType(Context* context,
                                              const QualifiedType& instance,
                                              const QualifiedType& rank,
                                              const QualifiedType& idxType,
                                              const QualifiedType& strides);

  /** Return an associative domain type */
  static const DomainType* getAssociativeType(Context* context,
                                              const QualifiedType& instance,
                                              const QualifiedType& idxType,
                                              const QualifiedType& parSafe);

  /** Return a subdomain type */
  static const DomainType* getSubdomainType(Context* context,
                                            const QualifiedType& instance,
                                            const QualifiedType& parentDomain);

  /** Return a sparse domain type */
  static const DomainType* getSparseType(Context* context,
                                         const QualifiedType& instance,
                                         const QualifiedType& parentDomain);

  const Type* substitute(Context* context,
                         const PlaceholderMap& subs) const override {
    return getDomainType(context, id(), name(),
                         Type::substitute(context, (const DomainType*) instantiatedFrom_, subs),
                         resolution::substituteInMap(context, subs_, subs),
                         kind_).get();
  }

  /** Get the default distribution type */
  static const QualifiedType& getDefaultDistType(Context* context);

  Kind kind() const {
    return kind_;
  }

  bool isSubdomain() const {
    return kind_ == Kind::Subdomain || kind_ == Kind::Sparse;
  }

  bool isSparse() const {
    return kind_ == Kind::Sparse;
  }

  const DomainType* parentDomain() const {
    CHPL_ASSERT(isSubdomain());
    auto parentDom = subs_.at(parentDomainId);
    CHPL_ASSERT(!parentDom.isUnknownOrErroneous());
    return parentDom.type()->toDomainType();
  }

  // Returns the integer representing the rank of this domain. This is more
  // general than `rank`, because `rank` returns the substitution
  // corresponding to the rank, which only exists for rectangular domains.
  // This method, however, works on associative domains, too.
  int rankInt() const;

  const QualifiedType& rank() const {
    if (isSubdomain()) {
      return parentDomain()->rank();
    }

    CHPL_ASSERT(kind_ == Kind::Rectangular);
    return subs_.at(rankId);
  }

  const QualifiedType& idxType() const {
    if (isSubdomain()) {
      return parentDomain()->idxType();
    }

    if (kind_ == Kind::Rectangular) {
      return subs_.at(rectangularIdxTypeId);
    } else {
      return subs_.at(nonRectangularIdxTypeId);
    }
  }

  const QualifiedType& strides() const {
    if (isSubdomain()) {
      return parentDomain()->strides();
    }

    CHPL_ASSERT(kind_ == Kind::Rectangular);
    return subs_.at(stridesId);
  }

  const QualifiedType& parSafe() const {
    if (isSubdomain()) {
      return parentDomain()->parSafe();
    }

    CHPL_ASSERT(kind_ == Kind::Associative);
    return subs_.at(parSafeId);
  }

  const RuntimeType* runtimeType(Context* context) const;

  ~DomainType() = default;

  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;

};


} // end namespace types

/// \cond DO_NOT_DOCUMENT
template<> struct stringify<chpl::types::DomainType::Kind> {
  void operator()(std::ostream& streamOut,
                  chpl::StringifyKind stringKind,
                  const chpl::types::DomainType::Kind& stringMe) const {
#define SWITCH_KIND(T) \
    case types::T: \
      streamOut << "T"; \
      break;

    switch (stringMe) {
      SWITCH_KIND(DomainType::Kind::Unknown);
      SWITCH_KIND(DomainType::Kind::Associative);
      SWITCH_KIND(DomainType::Kind::Rectangular);
      SWITCH_KIND(DomainType::Kind::Sparse);
      SWITCH_KIND(DomainType::Kind::Subdomain);
    }
  }
};
/// \endcond DO_NOT_DOCUMENT
} // end namespace chpl

namespace std {
  template <> struct hash<chpl::types::DomainType::Kind> {
    size_t operator()(const chpl::types::DomainType::Kind& kind) const {
      return (size_t) kind;
    }
  };
} // end namespace std

#endif
