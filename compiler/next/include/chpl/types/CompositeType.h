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

 */
class CompositeType : public Type {
 public:
  using SubstitutionsMap =
    std::unordered_map<const uast::Decl*, types::QualifiedType>;

  struct FieldDetail {
    UniqueString name;
    bool hasDefaultValue = false;
    const uast::Decl* decl = nullptr;
    QualifiedType type;

    FieldDetail(UniqueString name,
                bool hasDefaultValue,
                const uast::Decl* decl,
                QualifiedType type)
      : name(name), hasDefaultValue(hasDefaultValue), decl(decl), type(type) {
    }
    bool operator==(const FieldDetail& other) const {
      return name == other.name &&
             hasDefaultValue == other.hasDefaultValue &&
             decl == other.decl &&
             type == other.type;
    }
    bool operator!=(const FieldDetail& other) const {
      return !(*this == other);
    }
    size_t hash() const {
      return chpl::hash(name, hasDefaultValue, decl, type);
    }

    void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
      name.stringify(ss, stringKind);
      //TODO: determine the proper way to do this
      //decl.stringify(ss, stringKind);
      type.stringify(ss, stringKind);
    }

    /// \cond DO_NOT_DOCUMENT
    DECLARE_DUMP;
    /// \endcond DO_NOT_DOCUMENT
  };
 protected:
  // TODO: add fields and accessors for a QualifiedType per field

  ID id_;
  UniqueString name_;
  std::vector<FieldDetail> fields_;

  // Is this CompositeType representing an instantiation?
  // If so, what is the generic CompositeType that was instantiated?
  const CompositeType* instantiatedFrom_ = nullptr;

  bool isGeneric_ = false;
  bool allGenericFieldsHaveDefaultValues_ = false;

  CompositeType(typetags::TypeTag tag,
                ID id, UniqueString name,
                std::vector<FieldDetail> fields,
                const CompositeType* instantiatedFrom,
                SubstitutionsMap subs)
    : Type(tag), id_(id), name_(name), fields_(std::move(fields)),
      instantiatedFrom_(instantiatedFrom) {

    // check instantiated only from same type of object
    assert(instantiatedFrom_ == nullptr || instantiatedFrom_->tag() == tag);

    // check instantiatedFrom_ is a root
    assert(instantiatedFrom_ == nullptr ||
           instantiatedFrom_->instantiatedFrom_ == nullptr);

    // check that subs is consistent with instantiatedFrom
    assert((instantiatedFrom_ == nullptr) == subs.empty());

    // check that types are not established yet
    for (auto field : fields_) {
      assert(field.type.type() == nullptr);
    }
  }

  bool compositeTypeContentsMatchInner(const CompositeType* other,
                                       MatchAssumptions& assumptions) const {
    // check basic properties
    if (id_ != other->id_ || name_ != other->name_)
      return false;

    if (isGeneric_ != other->isGeneric_)
      return false;

    if (allGenericFieldsHaveDefaultValues_ !=
        other->allGenericFieldsHaveDefaultValues_)
      return false;

    // one is instantiated but the other is not
    if ((instantiatedFrom_ != nullptr) !=
        (other->instantiatedFrom_ != nullptr))
      return false;

    // add instantiatedFrom to the assumptions
    if (!assumptions.assume(instantiatedFrom_, other->instantiatedFrom_))
      return false;

    // consider the fields
    size_t nFields = fields_.size();
    for (size_t i = 0; i < nFields; i++) {
      const FieldDetail& field = fields_[i];
      const FieldDetail& otherField = other->fields_[i];

      if (field.name != otherField.name ||
          field.hasDefaultValue != otherField.hasDefaultValue ||
          field.decl != otherField.decl)
        return false;

      // Check the elements of the QualifiedType individually,
      // because we want to handle the Type* in a special way.
      if (field.type.kind() != otherField.type.kind() ||
          field.type.param() != otherField.type.param())
        return false;

      const Type* fieldType = field.type.type();
      const Type* otherFieldType = otherField.type.type();

      // type should be established by the time this runs
      assert(fieldType != nullptr);

      // add an assumption about the type ptrs, if they differ
      if (!assumptions.assume(fieldType, otherFieldType))
        return false;
    }

    return true;
  }

  void compositeTypeMarkUniqueStringsInner(Context* context) const {
    for (auto field : fields_) {
      field.name.mark(context);
    }
  }

 public:
  virtual ~CompositeType() = 0; // this is an abstract base class

  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;

  /** Set a field type. For use only during resolution while the type is still
      being constructed. */
  void setFieldType(int i, QualifiedType type);

  /** Called during resolution after all field types (and parent type)
      are set. */
  void finalizeFieldTypes();

  /** Returns true if this is a generic type */
  bool isGeneric() const override { return isGeneric_; }

  /** Returns true if this is a generic type where all
      generic fields have default values. For classes,
      this includes consideration of the parent class. */
  bool isGenericWithDefaults() const {
    return isGeneric_ && allGenericFieldsHaveDefaultValues_;
  }

  /** Return the uAST ID associated with this CompositeType */
  const ID& id() const { return id_; }

  /** Returns the name of the uAST associated with this CompositeType */
  UniqueString name() const { return name_; }

  /** Return the number of fields */
  int numFields() const {
    return fields_.size();
  }

  /** Return the name of the i'th field */
  UniqueString fieldName(int i) const {
    assert(0 <= i && (size_t) i < fields_.size());
    return fields_[i].name;
  }

  /** Return whether or not the i'th field has a default value */
  bool fieldHasDefaultValue(int i) const {
    assert(0 <= i && (size_t) i < fields_.size());
    return fields_[i].hasDefaultValue;
  }

  /** Return the Decl associated with the i'th field, or nullptr if none */
  const uast::Decl* fieldDecl(int i) const {
    assert(0 <= i && (size_t) i < fields_.size());
    return fields_[i].decl;
  }

  /** Return the QualifiedType of the i'th field */
  const QualifiedType& fieldType(int i) const {
    assert(0 <= i && (size_t) i < fields_.size());
    return fields_[i].type;
  }

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

  bool isInstantiationOf(const CompositeType* genericType) const {
    auto from = instantiatedFromCompositeType();
    return (from != nullptr && from == genericType);
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

template<> struct hash<chpl::types::CompositeType::FieldDetail>
{
  size_t operator()(const chpl::types::CompositeType::FieldDetail& key) const {
    return key.hash();
  }
};
template<> struct hash<chpl::types::CompositeType::SubstitutionsMap>
{
  size_t operator()(const chpl::types::CompositeType::SubstitutionsMap& x) const {
    return hashSubstitutionsMap(x);
  }
};


} // end namespace std

#endif
