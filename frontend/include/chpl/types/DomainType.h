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

#ifndef CHPL_TYPES_DOMAIN_TYPE_H
#define CHPL_TYPES_DOMAIN_TYPE_H

#include "chpl/types/CompositeType.h"

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
     // TODO: Sparse,
     // TODO: Subdomain,
     Unknown
   };

 private:
  // TODO: distributions
  Kind kind_;

  // Will compute idxType, rank, and stridable from 'subs'
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

 public:

  /** Return the generic domain type */
  static const DomainType* getGenericDomainType(Context* context);

  /** Return a rectangular domain type */
  static const DomainType* getRectangularType(Context* context,
                                              const QualifiedType& rank,
                                              const QualifiedType& idxType,
                                              const QualifiedType& stridable);

  /** Return an associative domain type */
  static const DomainType* getAssociativeType(Context* context,
                                              const QualifiedType& idxType,
                                              const QualifiedType& parSafe);

  const Kind kind() const {
    return kind_;
  }

  const QualifiedType& rank() const {
    CHPL_ASSERT(kind_ == Kind::Rectangular);
    return subs_.at(ID(UniqueString(), 0, 0));
  }

  const QualifiedType& idxType() const {
    if (kind_ == Kind::Rectangular) {
      return subs_.at(ID(UniqueString(), 1, 0));
    } else {
      return subs_.at(ID(UniqueString(), 0, 0));
    }
  }

  const QualifiedType& stridable() const {
    CHPL_ASSERT(kind_ == Kind::Rectangular);
    return subs_.at(ID(UniqueString(), 2, 0));
  }

  const QualifiedType& parSafe() const {
    CHPL_ASSERT(kind_ == Kind::Associative);
    return subs_.at(ID(UniqueString(), 1, 0));
  }

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
