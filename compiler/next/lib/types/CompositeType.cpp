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

#include "chpl/types/CompositeType.h"

#include "chpl/types/BasicClassType.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/Decl.h"

namespace chpl {
namespace types {


CompositeType::~CompositeType() {
}

void CompositeType::setFieldType(int i, QualifiedType type) {
  assert(0 <= i && (size_t) i < fields_.size());
  fields_[i].type = type;
}

void CompositeType::finalizeFieldTypes() {
  isGeneric_ = false;
  allGenericFieldsHaveDefaultValues_ = true;
  for (auto field : fields_) {
    if (field.type.isGenericOrUnknown()) {
      if (!field.hasDefaultValue) {
        allGenericFieldsHaveDefaultValues_ = false;
      }
      isGeneric_ = true;
    }
  }

  // include also the parent class type for BasicClassType.
  if (auto bct = this->toBasicClassType()) {
    if (const Type* parentT = bct->parentClassType()) {
      if (parentT->isGeneric()) {
        isGeneric_ = true;
        if (auto pct = parentT->toBasicClassType()) {
          allGenericFieldsHaveDefaultValues_ = pct->isGenericWithDefaults();
        } else {
          allGenericFieldsHaveDefaultValues_ = false;
        }
      }
    }
  }
}

void CompositeType::stringify(std::ostream& ss,
                              chpl::StringifyKind stringKind) const {
  // compute the parent class type for BasicClassType
  const Type* superType = nullptr;
  if (auto bct = this->toBasicClassType()) {
    superType = bct->parentClassType();
  }

  //std::string ret = typetags::tagToString(tag());
  name().stringify(ss, stringKind);
  int nFields = numFields();

  if (superType || nFields > 0) {
    bool emittedField = false;
    ss << "(";
    if (superType) {
      ss << "super:";
      superType->stringify(ss, stringKind);
      emittedField = true;
    }

    for (int i = 0; i < nFields; i++) {
      if (emittedField) ss << ", ";
      fieldName(i).stringify(ss, stringKind);
      ss << ":";
      fieldType(i).stringify(ss, stringKind);
      emittedField = true;
    }
    ss << ")";
  }
}

using SubstitutionPair = std::pair<const uast::Decl*,QualifiedType>;

struct SubstitutionsMapCmp {
  bool operator()(const SubstitutionPair& x, const SubstitutionPair& y) {
    return x.first->id() < y.first->id();
  }
};

void stringifySubstitutionsMap(std::ostream& streamOut,
                               StringifyKind stringKind,
                               const CompositeType::SubstitutionsMap& subs) {
  // since it's an unordered map, iteration will occur in a
  // nondeterministic order.
  // it's important to sort the keys / iterate in a deterministic order here,
  // so we create a vector of pair<K,V> and sort that instead
  std::vector<std::pair<const uast::Decl*,QualifiedType>> v(subs.begin(), subs.end());
  SubstitutionsMapCmp cmp;
  std::sort(v.begin(), v.end(), cmp);
  bool first = true;
  for (auto const& x : v)
  {
    const uast::Decl* decl = x.first;
    const uast::NamedDecl* namedDecl = decl->toNamedDecl();
    QualifiedType qt = x.second;

    if (first) {
      first = false;
    } else {
      streamOut << ", ";
    }

    if (namedDecl != nullptr && stringKind == StringifyKind::CHPL_SYNTAX)
      namedDecl->name().stringify(streamOut, stringKind);
    else
      decl->id().stringify(streamOut, stringKind);

    streamOut << "= ";

    qt.stringify(streamOut, stringKind);
  }
}

IMPLEMENT_DUMP(CompositeType::FieldDetail);


} // end namespace types
} // end namespace chpl
