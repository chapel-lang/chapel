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

#ifndef CHPL_TYPES_MANAGEABLE_TYPE_H
#define CHPL_TYPES_MANAGEABLE_TYPE_H

#include "chpl/types/CompositeType.h"
#include "chpl/framework/global-strings.h"

namespace chpl {
namespace types {


/**
  General subclass for "things that have management". Mostly this will be basic
  class types (e.g., the `C` in `managed C?`), but we also want to share the
  code for adding management and nilability to the generic `class` typeclass.
  So both `C` (and other Chapel subclasses of `RootClass`) and `class`
  will inherit from ManageableType.
 */
class ManageableType : public CompositeType {
 protected:
  ManageableType(typetags::TypeTag tag,
                ID id, UniqueString name,
                const CompositeType* instantiatedFrom,
                SubstitutionsMap subs)
    : CompositeType(tag, std::move(id), name, instantiatedFrom, std::move(subs)) {}
};

} // end namespace types


} // end namespace chpl

#endif
