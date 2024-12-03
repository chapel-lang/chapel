/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/InterfaceType.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/Interface.h"

namespace chpl {
namespace types {

bool InterfaceType::validateSubstitutions(Context* context,
                                          const ID& id,
                                          SubstitutionsMap& subs) {
  auto ast = parsing::idToAst(context, id);
  if (!ast) return false;
  auto ifc = ast->toInterface();
  if (!ifc) return false;

  if (subs.size() != ifc->numFormals()) return false;
  for (auto fml : ifc->formals()) {
    if (subs.count(fml->id()) == 0) return false;
  }

  return true;
}

owned<InterfaceType> const&
InterfaceType::getInterfaceType(Context* context, ID id, UniqueString name, SubstitutionsMap subs) {
  QUERY_BEGIN(getInterfaceType, context, id, name, subs);
  validateSubstitutions(context, id, subs);
  auto result = toOwned(new InterfaceType(id, name, subs));
  return QUERY_END(result);
}

const InterfaceType* InterfaceType::get(Context* context, ID id, UniqueString name, SubstitutionsMap subs) {
  return getInterfaceType(context, id, name, subs).get();
}

} // end namespace types
} // end namespace chpl
