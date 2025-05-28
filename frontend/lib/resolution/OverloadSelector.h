/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Context.h"
#include "chpl/framework/ID.h"
#include "chpl/uast/BuilderResult.h"

namespace chpl {
namespace resolution {

using BuilderFunc = uast::BuilderResult const&(*)(Context* context, ID typeId, int overloadIdx);

// note: this uses a templated implementation because in modern C++,
// we should be able to make `dispatch` select the function at
// compile time. However, presently, some of the older compilers we support
// do not consider function pointers to be constant expressions, so we
// instead use an array that's iterated at runtime.

/*
 The OverloadSelector is a utility struct that keeps track of the ordering
 of compiler-generated overloads. Specifically, there are two processes that
 occur when a compiler-generated function is handled:

 1. When a user call occurs with certain types, the frontend decides if the
    compiler should generate a matching overload. At this point, the builder
    function is invoked. The builder function needs to know the index
    of the overload, since this index is reflected in the generated IDs.

    Here, the `invoke` method can be used to request a call to the appropriate
    builder function, with OverloadSelector providing the function its index
    automatically based on its position in the overload list.
 2. Later, during resolution, we need to retrieve the generated AST for the
    function from its ID. To do this, we effectively have to invoke
    the builder function again. This time, we know the index (it's encoded
    in the overload ID, like `foo#1`), but we don't know the function.

    Here, the `select` method handles mapping the index back to the function
    that produced it.

  By having the OverloadSelector handle both calls to the builder function
  described above, we ensure a consistent ordering of overloads.
 */
template <BuilderFunc... Fs>
struct OverloadSelector {
 private:
  static constexpr std::array<BuilderFunc, sizeof...(Fs)> overloads = {{Fs...}};

 public:
  template <BuilderFunc F>
  static uast::BuilderResult const& invoke(Context* context, const ID& id) {
    int idx = 0;
    for (auto overload : overloads) {
      if (overload == F) {
        return F(context, id, idx);
      }
      idx++;
    }
    CHPL_ASSERT(false && "overload not found, is the function registered?");
    return overloads[0](context, id, 0); // Fallback to the first overload
  }

  static uast::BuilderResult const& select(Context* context, const ID& id, UniqueString overloadPart) {
    // note: this should match the way doAssignIDs in Builder.cpp creates
    // the overloadPart from the index of the overload.
    int targetIdx = 0;
    if (!overloadPart.isEmpty()) {
      targetIdx = atoi(overloadPart.c_str());
    }

    int idx = 0;
    for (auto overload : overloads) {
      if (idx == targetIdx) {
        return overload(context, id, idx);
      }
      idx++;
    }
    CHPL_ASSERT(false && "overload not found, was the overloadPart incorrectly constructed?");
    return overloads[0](context, id, 0); // Fallback to the first overload
  }
};

} // namespace resolution
} // namespace chpl
