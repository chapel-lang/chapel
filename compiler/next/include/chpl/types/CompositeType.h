/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_COMPOSITE_TYPE_H
#define CHPL_TYPES_COMPOSITE_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"

namespace chpl {
namespace uast {
  class Decl;
}
namespace types {


/**

  This class represents an aggregate type which is a type that contains other
  elements as fields. Subclasses include BasicClass, Record, Union, and Tuple
  types.

  This class itself does not represent the field types. Instead, it
  uses the uAST ID of the class/record/union. For a generic type,
  it also uses a SubstitutionsMap.

  (Storing field types in this class would present significant challenges
   for computing this type in an immutable way within the query framework). 
 */
class CompositeType : public Type {
 public:
  using SubstitutionsMap = std::unordered_map<ID, types::QualifiedType>;
  using SubstitutionPair = std::pair<ID, QualifiedType>;
  using SortedSubstitutions = std::vector<SubstitutionPair>;

 protected:
  // TODO: add fields and accessors for a QualifiedType per field

  ID id_;
  UniqueString name_;

  // Is this CompositeType representing an instantiation?
  // If so, what is the generic CompositeType that was instantiated?
  // TODO: should we remove this and compute it again based on id?
  const CompositeType* instantiatedFrom_ = nullptr;
  // If so, what types/params should we use?
  SubstitutionsMap subs_;

  CompositeType(typetags::TypeTag tag,
                ID id, UniqueString name,
                const CompositeType* instantiatedFrom,
                SubstitutionsMap subs)
    : Type(tag), id_(id), name_(name),
      instantiatedFrom_(instantiatedFrom),
      subs_(std::move(subs)) {

    // check instantiated only from same type of object
    assert(instantiatedFrom_ == nullptr || instantiatedFrom_->tag() == tag);

    // check instantiatedFrom_ is a root
    assert(instantiatedFrom_ == nullptr ||
           instantiatedFrom_->instantiatedFrom_ == nullptr);

    // check that subs is consistent with instantiatedFrom
    assert((instantiatedFrom_ == nullptr) == subs_.empty());
  }

  bool compositeTypeContentsMatchInner(const CompositeType* other,
                                       MatchAssumptions& assumptions) const;

  void compositeTypeMarkUniqueStringsInner(Context* context) const {
    id_.mark(context);
    name_.mark(context);
  }

 public:
  virtual ~CompositeType() = 0; // this is an abstract base class

  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;

  /** Returns true if this is a generic type */
  Genericity genericity() const override { return MAYBE_GENERIC; }

  /** Return the uAST ID associated with this CompositeType */
  const ID& id() const { return id_; }

  /** Returns the name of the uAST associated with this CompositeType */
  UniqueString name() const { return name_; }

  /** If this type represents an instantiated type,
      returns the type it was instantiated from.

      This function always returns the fully generic type and never
      a partial instantiation. That is, the result
      will either be nullptr or result->instantiatedFrom() will
      be nullptr.
   */
  const CompositeType* instantiatedFromCompositeType() const {
    // at present, only expecting a single level of instantiated-from.
    assert(instantiatedFrom_ == nullptr ||
           instantiatedFrom_->instantiatedFrom_ == nullptr);
    return instantiatedFrom_;
  }

  /** Returns true if 'this' is an instantiation of genericType */
  bool isInstantiationOf(const CompositeType* genericType) const {
    auto from = instantiatedFromCompositeType();
    return (from != nullptr && from == genericType);
  }

  /** Returns the substitutions map */
  const SubstitutionsMap& substitutions() const {
    return subs_;
  }

  /** Returns the substitutions in a sorted order */
  SortedSubstitutions sortedSubstitutions() const;

  /** Returns the substitution for a particular declId,
      or an empty QualifiedType if no substitution was found. */
  types::QualifiedType substitution(ID declId) const {
    auto it = subs_.find(declId);
    if (it != subs_.end())
      return it->second;

    return types::QualifiedType();
  }
};

size_t hashSubstitutionsMap(const CompositeType::SubstitutionsMap& subs);

void stringifySubstitutionsMap(std::ostream& streamOut,
                               StringifyKind stringKind,
                               const CompositeType::SubstitutionsMap& subs);


} // end namespace types


// for CompositeType::SubstitutionsMap
template<> struct stringify<types::CompositeType::SubstitutionsMap> {
  void operator()(std::ostream& streamOut,
                  StringifyKind stringKind,
                  const types::CompositeType::SubstitutionsMap& subs) const {

    stringifySubstitutionsMap(streamOut, stringKind, subs);
  }
};


} // end namespace chpl


namespace std {

template<> struct hash<chpl::types::CompositeType::SubstitutionsMap>
{
  size_t operator()(const chpl::types::CompositeType::SubstitutionsMap& x) const {
    return hashSubstitutionsMap(x);
  }
};


} // end namespace std

#endif
