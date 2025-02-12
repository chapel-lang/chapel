/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/BasicClassType.h"

#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"

namespace chpl {
namespace types {

const owned<BasicClassType>&
BasicClassType::getBasicClassType(Context* context, ID id, UniqueString name,
                                  const BasicClassType* parentType,
                                  const BasicClassType* instantiatedFrom,
                                  SubstitutionsMap subs) {
  QUERY_BEGIN(getBasicClassType, context, id, name,
              parentType, instantiatedFrom, subs);

  auto result = toOwned(new BasicClassType(id, name,
                                           parentType, instantiatedFrom,
                                           std::move(subs)));
  return QUERY_END(result);
}

const BasicClassType*
BasicClassType::get(Context* context, ID id, UniqueString name,
                    const BasicClassType* parentType,
                    const BasicClassType* instantiatedFrom,
                    SubstitutionsMap subs) {
  // getRootClassType should be used to construct RootClass
  // everything else should have a parent type.
  CHPL_ASSERT(parentType != nullptr);
  return getBasicClassType(context, id, name,
                           parentType, instantiatedFrom,
                           std::move(subs)).get();
}

const BasicClassType*
BasicClassType::getRootClassType(Context* context) {
  ID emptyId;
  auto name = UniqueString::get(context, "RootClass");

  return getBasicClassType(context, emptyId, name,
                           /* parentType */ nullptr,
                           /* instantiatedFrom */ nullptr,
                           SubstitutionsMap()).get();
}

const BasicClassType*
BasicClassType::getReduceScanOpType(Context* context) {
  auto [id, name] = parsing::getSymbolFromTopLevelModule(
      context, "ChapelReduce", "ReduceScanOp");
  auto objectType = getRootClassType(context);

  return getBasicClassType(context, id, name,
                           /* parentType */ objectType,
                           /* instantiatedFrom */ nullptr,
                           SubstitutionsMap()).get();
}

bool BasicClassType::isSubtypeOf(Context* context,
                                 const BasicClassType* parentType,
                                 bool& converts,
                                 bool& instantiates) const {

  CHPL_ASSERT(parentType != nullptr); // code below assumes this

  for (const BasicClassType* t = this;
       t != nullptr; // note: ObjectType has no parent
       t = t->parentClassType()) {

    // check if t is parentType indicating use of subclass
    if (t == parentType) {
      if (t != this) converts = true;
      return true;
    }

    // check also if t is an instantiation of parentType
    if (t->isInstantiationOf(context, parentType)) {
      if (t != this) converts = true;
      instantiates = true;
      return true;
    }
  }

  return false;
}


} // end namespace types
} // end namespace chpl
