/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/AttributeGroup.h"

#include "chpl/uast/Builder.h"

namespace chpl {
namespace uast {


owned<AttributeGroup> AttributeGroup::build(Builder* builder, Location loc,
                                            std::set<PragmaTag> pragmas,
                                            bool isDeprecated,
                                            bool isUnstable,
                                            bool isParenfulDeprecated,
                                            UniqueString deprecationMessage,
                                            UniqueString unstableMessage,
                                            UniqueString parenfulDeprecationMessage) {
  #ifndef NDEBUG
    for (auto tag : pragmas) {
      CHPL_ASSERT(tag >= 0 && tag < NUM_KNOWN_PRAGMAS);
    }
  #endif

  AttributeGroup* ret = new AttributeGroup(std::move(pragmas), isDeprecated,
                                           isUnstable,
                                           isParenfulDeprecated,
                                           deprecationMessage,
                                           unstableMessage,
                                           parenfulDeprecationMessage);
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

owned<AttributeGroup> AttributeGroup::build(Builder* builder, Location loc,
                                            std::set<PragmaTag> pragmas,
                                            bool isDeprecated,
                                            bool isUnstable,
                                            bool isParenfulDeprecated,
                                            UniqueString deprecationMessage,
                                            UniqueString unstableMessage,
                                            UniqueString parenfulDeprecationMessage,
                                            AstList attributes) {
  #ifndef NDEBUG
    for (auto tag : pragmas) {
      CHPL_ASSERT(tag >= 0 && tag < NUM_KNOWN_PRAGMAS);
    }
  #endif

  AttributeGroup* ret = new AttributeGroup(std::move(pragmas), isDeprecated,
                                           isUnstable,
                                           isParenfulDeprecated,
                                           deprecationMessage,
                                           unstableMessage,
                                           parenfulDeprecationMessage,
                                           std::move(attributes));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}


} // namespace uast
} // namespace chpl
