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

#include "chpl/types/ClassType.h"

#include "chpl/framework/query-impl.h"

namespace chpl {
namespace types {

void ClassType::stringify(std::ostream& ss,
                          chpl::StringifyKind stringKind) const {

  // compute the prefix to use
  std::string prefix = "";
  switch (decorator_.val()) {
    case ClassTypeDecorator::BORROWED:
    case ClassTypeDecorator::BORROWED_NONNIL:
    case ClassTypeDecorator::BORROWED_NILABLE:
      prefix = "borrowed";
      break;
    case ClassTypeDecorator::UNMANAGED:
    case ClassTypeDecorator::UNMANAGED_NONNIL:
    case ClassTypeDecorator::UNMANAGED_NILABLE:
      prefix = "unmanaged";
      break;
    case ClassTypeDecorator::MANAGED:
    case ClassTypeDecorator::MANAGED_NONNIL:
    case ClassTypeDecorator::MANAGED_NILABLE:
      prefix = "";
      break;
    case ClassTypeDecorator::GENERIC:
    case ClassTypeDecorator::GENERIC_NONNIL:
    case ClassTypeDecorator::GENERIC_NILABLE:
      prefix = stringKind == StringifyKind::CHPL_SYNTAX ? "" : "<any-management>";
      break;
  }

  // compute the manager, if any
  std::string manager;
  if (decorator_.isManaged()) {
    CHPL_ASSERT(manager_);
    if (manager_->isAnyOwnedType()) {
      manager = "owned";
    } else if (manager_->isAnySharedType()) {
      manager = "shared";
    } else {
      std::ostringstream ss2;
      manager_->stringify(ss2, stringKind);
      manager = ss2.str();
    }
  }

  // at this point, either prefix or manager is not ""
  // emit prefix
  ss << prefix;
  // emit manager
  ss << manager;
  // emit a space
  if (!prefix.empty() || !manager.empty()) {
    ss << " ";
  }

  // emit manageable type name
  CHPL_ASSERT(manageableType_);
  manageableType_->stringify(ss, stringKind);

  // emit ? if nilable
  if (decorator_.isNilable()) {
    ss << "?";
  } else if (decorator_.isUnknownNilability() &&
             stringKind != StringifyKind::CHPL_SYNTAX) {
    ss << " <unknown-nilability>";
  }
}

const owned<ClassType>&
ClassType::getClassType(Context* context,
                        const ManageableType* manageableType,
                        const Type* manager,
                        ClassTypeDecorator decorator) {
  QUERY_BEGIN(getClassType, context, manageableType, manager, decorator);

  auto result = toOwned(new ClassType(manageableType, manager, decorator));

  return QUERY_END(result);
}

const ClassType* ClassType::get(Context* context,
                                const ManageableType* manageableType,
                                const Type* manager,
                                ClassTypeDecorator decorator) {
  return getClassType(context, manageableType, manager, decorator).get();
}


const ClassType* ClassType::withDecorator(Context* context,
                                          ClassTypeDecorator decorator) const {
  return ClassType::get(context, manageableType(), manager(), decorator);
}

const RecordType* ClassType::managerRecordType(Context* context) const {
  // for owned and shared, manager is the builtin AnyClassType
  if (auto myManager = manager()) {
    if (myManager->isAnyOwnedType()) {
      return CompositeType::getOwnedRecordType(context, basicClassType()); // for `owned C`, produce `_owned(C)`
    } else if (myManager->isAnySharedType()) {
      return CompositeType::getSharedRecordType(context, basicClassType());
    } else if (auto mgr = myManager->toRecordType()) {
      return mgr;
    }
  }
  return nullptr;
}

} // end namespace types
} // end namespace chpl
