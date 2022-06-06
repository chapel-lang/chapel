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

#include "chpl/resolution/intents.h"

#include "chpl/types/ClassType.h"
#include "chpl/types/RecordType.h"
#include "chpl/types/UnionType.h"
#include "chpl/types/TupleType.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

static QualifiedType::Kind constIntentForType(const Type* t) {

  // anything we don't know the type of has to have unknown intent
  if (t == nullptr || t->isUnknownType() || t->isErroneousType())
    return QualifiedType::UNKNOWN;

  if (t->isPrimitiveType())
    return QualifiedType::CONST_IN;

  if (t->isStringType() || t->isBytesType() ||
      t->isRecordType() || t->isUnionType() || t->isTupleType())
    return QualifiedType::CONST_REF;

  if (auto ct = t->toClassType()) {
    if (ct->decorator().isUnknownManagement())
      return QualifiedType::CONST_INTENT;
    else if (ct->decorator().isManaged())
      return QualifiedType::CONST_REF;
    else
      return QualifiedType::CONST_IN;
  }

  // Otherwise, it should be a generic type that we will
  // instantiate before computing the final intent.
  assert(t->genericity() != Type::CONCRETE);
  return QualifiedType::CONST_INTENT; // leave the intent generic
}

static QualifiedType::Kind defaultIntentForType(const Type* t) {

  // anything we don't know the type of has to have unknown intent
  if (t == nullptr || t->isUnknownType() || t->isErroneousType())
    return QualifiedType::UNKNOWN;

  if (t->isPrimitiveType())
    return QualifiedType::CONST_IN;

  if (t->isStringType() || t->isBytesType() ||
      t->isRecordType() || t->isUnionType() || t->isTupleType())
    return QualifiedType::CONST_REF;

  if (auto ct = t->toClassType()) {
    if (ct->decorator().isUnknownManagement())
      return QualifiedType::DEFAULT_INTENT;
    else if (ct->decorator().isManaged())
      return QualifiedType::CONST_REF;
    else
      return QualifiedType::CONST_IN;
  }

  // Otherwise, it should be a generic type that we will
  // instantiate before computing the final intent.
  assert(t->genericity() != Type::CONCRETE);
  return QualifiedType::DEFAULT_INTENT; // leave the intent generic
}

QualifiedType::Kind resolveIntent(const QualifiedType& t, bool isThis) {
  auto kind = t.kind();
  auto type = t.type();

  // TODO: take into account isThis
  // TODO: REF_MAYBE_CONST or equivalent for arrays, record this

  switch (kind) {
    case QualifiedType::UNKNOWN:
    case QualifiedType::INDEX:
    case QualifiedType::TYPE_QUERY:
    case QualifiedType::PARENLESS_FUNCTION:
    case QualifiedType::FUNCTION:
    case QualifiedType::MODULE:
      // these don't really have an intent
      return QualifiedType::UNKNOWN;

    case QualifiedType::CONST_REF:
    case QualifiedType::REF:
    case QualifiedType::IN:
    case QualifiedType::CONST_IN:
    case QualifiedType::OUT:
    case QualifiedType::INOUT:
    case QualifiedType::PARAM:
    case QualifiedType::TYPE:
      // concrete intents are already resolved
      return kind;

    case QualifiedType::VAR:
      // normalize VAR to IN to make some test codes easier
      return QualifiedType::IN;
    case QualifiedType::CONST_VAR:
      // normalize CONST_VAR to CONST_IN to make some test codes easier
      return QualifiedType::CONST_IN;

    case QualifiedType::DEFAULT_INTENT:
      // compute the default intent if needed
      return defaultIntentForType(type);

    // compute the const intent if needed
    case QualifiedType::CONST_INTENT:
      return constIntentForType(type);
  }

  return QualifiedType::UNKNOWN;
}


} // end namespace resolution
} // end namespace chpl
