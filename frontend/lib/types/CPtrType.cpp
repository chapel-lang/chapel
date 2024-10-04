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

#include "chpl/types/CPtrType.h"

#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/intents.h"
#include "chpl/types/Param.h"
#include "chpl/types/VoidType.h"
#include "chpl/resolution/can-pass.h"

namespace chpl {
namespace types {

const owned<CPtrType>& CPtrType::getCPtrType(Context* context,
                                             const CPtrType* instantiatedFrom,
                                             const Type* eltType,
                                             bool isConst) {
  QUERY_BEGIN(getCPtrType, context, instantiatedFrom, eltType, isConst);
  auto result = toOwned(new CPtrType(instantiatedFrom, eltType, isConst));
  return QUERY_END(result);
}

bool CPtrType::isEltTypeInstantiationOf(Context* context, const CPtrType* other) const {
  auto r = resolution::canPass(context,
                               QualifiedType(QualifiedType::TYPE, eltType_),
                               QualifiedType(QualifiedType::TYPE, other->eltType_));
  // instantiation and same-type passing are allowed here
  return r.passes() && !r.promotes() && !r.converts();
}

const CPtrType* CPtrType::get(Context* context) {
  return CPtrType::getCPtrType(context,
                               /* instantiatedFrom */ nullptr,
                               /* eltType */ nullptr,
                               /* isConst */ false).get();
}

const CPtrType* CPtrType::get(Context* context, const Type* eltType) {
  return CPtrType::getCPtrType(context,
                               /* instantiatedFrom */ CPtrType::get(context),
                               eltType,
                               /* isConst */ false).get();
}

// TODO: need to treat the elttype kind as const
const CPtrType* CPtrType::getConst(Context* context) {
  return CPtrType::getCPtrType(context,
                               /* instantiatedFrom */ nullptr,
                               /* eltType */ nullptr,
                               /* isConst */ true).get();
}

// TODO: need to treat the elt type kind as const
const CPtrType* CPtrType::getConst(Context* context, const Type* eltType) {
  return CPtrType::getCPtrType(context,
                               /* instantiatedFrom */ CPtrType::getConst(context),
                               eltType,
                               /*isConst*/ true).get();
}

const CPtrType* CPtrType::getCVoidPtrType(Context* context) {
  return CPtrType::get(context, VoidType::get(context));
}

const ID& CPtrType::getId(Context* context) {
  QUERY_BEGIN(getId, context);
  ID result =
      parsing::getSymbolIdFromTopLevelModule(context, "CTypes", "c_ptr");
  return QUERY_END(result);
}

const CPtrType* CPtrType::withoutConst(Context* context) const {
  const CPtrType* instFrom = nullptr;
  if (instantiatedFrom_) {
    instFrom = instantiatedFrom_->withoutConst(context);
  }

  return CPtrType::getCPtrType(context, instFrom, eltType_, /* isConst */ false).get();
}

const ID& CPtrType::getConstId(Context* context) {
  QUERY_BEGIN(getConstId, context);
  ID result =
      parsing::getSymbolIdFromTopLevelModule(context, "CTypes", "c_ptrConst");
  return QUERY_END(result);
}

bool CPtrType::isInstantiationOf(Context* context, const CPtrType* genericType) const {
  auto thisFrom = instantiatedFromCPtrType();
  auto argFrom = genericType->instantiatedFromCPtrType();
  if (argFrom == nullptr) {
    // if genericType is not a partial instantiation
    return (thisFrom != nullptr && thisFrom == genericType);
  }

  if (thisFrom == argFrom) {
    // handle the case of genericType being partly instantiated
    // (or instantiated with a generic type)
    return isEltTypeInstantiationOf(context, genericType);
  }

  return false;
}

void CPtrType::stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const {
  if (isConst_) {
    ss << "c_ptrConst";
  } else {
    ss << "c_ptr";
  }

  if (eltType_) {
    ss << "(";
    eltType_->stringify(ss, stringKind);
    ss << ")";
  }
}

} // end namespace types
} // end namespace chpl
