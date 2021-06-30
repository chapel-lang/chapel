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

#include "chpl/types/Type.h"

namespace chpl {
namespace types {


Type::~Type() {
}

bool Type::completeMatch(const Type* other) const {
  const Type* lhs = this;
  const Type* rhs = other;
  if (lhs->tag() != rhs->tag())
    return false;
  if (!lhs->contentsMatchInner(rhs))
    return false;

  return true;
}

bool Type::updateType(owned<Type>& keep, owned<Type>& addin) {
  if (keep->completeMatch(addin.get())) {
    // no changes are necessary
    return false;
  } else {
    // swap the AST
    keep.swap(addin);
    return true;
  }
}

void Type::markType(Context* context, const Type* keep) {
  if (keep == nullptr) return;
  // run markUniqueStrings on the node
  keep->markUniqueStringsInner(context);
}

void Type::dump(const Type* type, int leadingSpaces) {
  for (int i = 0; i < leadingSpaces; i++) {
    printf("  ");
  }

  printf("type %s \n", typetags::tagToString(type->tag()));
}


} // end namespace types
} // end namespace chpl
