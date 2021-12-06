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

  void computeSummaryInformation();

  CompositeType(typetags::TypeTag tag,
                ID id, UniqueString name,
                std::vector<FieldDetail> fields,
                const CompositeType* instantiatedFrom)
    : Type(tag), id_(id), name_(name), fields_(std::move(fields)),
      instantiatedFrom_(instantiatedFrom) {

    // check instantiated only from same type of object
    assert(instantiatedFrom_ == nullptr || instantiatedFrom_->tag() == tag);

    if (tag != typetags::BasicClassType)
      computeSummaryInformation();
    // BasicClassType constructor calls computeSummaryInformation
    // so that the parentType_ field will be set when it runs.
  }

  bool compositeTypeContentsMatchInner(const CompositeType* other) const {
    return id_ == other->id_ &&
           name_ == other->name_ &&
           fields_ == other->fields_ &&
           instantiatedFrom_ == other->instantiatedFrom_ &&
           isGeneric_ == other->isGeneric_ &&
           allGenericFieldsHaveDefaultValues_ == other->allGenericFieldsHaveDefaultValues_;
  }

  void compositeTypeMarkUniqueStringsInner(Context* context) const {
    for (auto field : fields_) {
      field.name.mark(context);
    }
  }

 public:
  virtual ~CompositeType() = 0; // this is an abstract base class

  virtual std::string toString() const override;

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
};


} // end namespace uast
/// \cond DO_NOT_DOCUMENT
template<> struct stringify<chpl::types::CompositeType::FieldDetail> {
  void operator()(std::ostream &stringOut,
                  StringifyKind stringKind,
                  const chpl::types::CompositeType::FieldDetail& stringMe) const {
    stringOut << "types::CompositeType::FieldDetail is not stringified";
  }
};
/// \endcond DO_NOT_DOCUMENT
} // end namespace chpl


namespace std {

template<> struct hash<chpl::types::CompositeType::FieldDetail>
{
  size_t operator()(const chpl::types::CompositeType::FieldDetail& key) const {
    return key.hash();
  }
};

} // end namespace std

#endif
