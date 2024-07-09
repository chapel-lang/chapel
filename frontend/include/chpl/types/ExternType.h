/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_EXTERN_TYPE_H
#define CHPL_TYPES_EXTERN_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/types/QualifiedType.h"

namespace chpl {
namespace types {

class ExternType final : public Type {
 private:
  UniqueString linkageName_;
  ID id_;

  ExternType(UniqueString linkageName, ID id)
    : Type(typetags::ExternType), linkageName_(linkageName), id_(std::move(id)) {}

  bool contentsMatchInner(const Type* other) const override {
    const ExternType* rhs = (const ExternType*) other;
    return linkageName_ == rhs->linkageName_ &&
           id_ == rhs->id_;
  }

  void markUniqueStringsInner(Context* context) const override {
    linkageName_.mark(context);
    id_.mark(context);
  }

  Genericity genericity() const override {
    return CONCRETE;
  }

  static const owned<ExternType>& getExternType(Context* context,
                                                UniqueString linkageName,
                                                ID id);

 public:
  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;

  inline UniqueString linkageName() const { return linkageName_; }

  inline const ID& id() const { return id_; }

  static const ExternType* get(Context* context, UniqueString linkageName, ID id);
};

} // end namespace types
} // end namespace chpl

#endif
