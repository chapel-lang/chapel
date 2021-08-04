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

#include "chpl/types/Param.h"

#include "chpl/queries/query-impl.h"

namespace chpl {
namespace types {


Param::~Param() {
}

bool Param::completeMatch(const Param* other) const {
  const Param* lhs = this;
  const Param* rhs = other;
  if (lhs->tag() != rhs->tag())
    return false;
  if (!lhs->contentsMatchInner(rhs))
    return false;

  return true;
}

bool Param::updateParam(owned<Param>& keep, owned<Param>& addin) {
  if (keep->completeMatch(addin.get())) {
    // no changes are necessary
    return false;
  } else {
    // swap the Param
    keep.swap(addin);
    return true;
  }
}

void Param::markParam(Context* context, const Param* keep) {
  if (keep == nullptr) return;
  // run markUniqueStrings on the node
  keep->markUniqueStringsInner(context);
}

std::string Param::toString() const {
  std::string ret;

  switch (tag_) {
#define PARAM_NODE(NAME, VALTYPE, TYPEEXPR) \
    case paramtags::NAME: { \
      const NAME* casted = (const NAME*) this; \
      auto value = casted->value(); \
      ret += Param::valueToString(value); \
      break; \
    }
// Apply the above macros to ParamClassesList.h
#include "chpl/types/ParamClassesList.h"
// clear the macros
#undef PARAM_NODE
  }

  return ret;
}

// implement the subclasses using macros and ParamClassesList.h
#define PARAM_NODE(NAME, VALTYPE, TYPEEXPR) \
  const owned<NAME>& NAME::get##NAME(Context* context, VALTYPE value) { \
    QUERY_BEGIN(get##NAME, context, value); \
    auto result = toOwned(new NAME(value)); \
    return QUERY_END(result); \
  }

// Apply the above macros to ParamClassesList.h
#include "chpl/types/ParamClassesList.h"

// clear the macros
#undef PARAM_NODE


} // end namespace types
} // end namespace chpl
