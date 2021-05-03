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

#include "chpl/queries/Context.h"
#include "chpl/queries/query-impl.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdlib>

using namespace chpl;

const UniqueString& recursiveQuery(Context* context, UniqueString arg) {
  QUERY_BEGIN(recursiveQuery, context, arg);

  UniqueString result = recursiveQuery(context, arg);

  return QUERY_END(result);
}

int main() {
  auto context = Context::build();
  Context* ctx = context.get();

  auto s = UniqueString::build(ctx, "Nice To See You Again World, Hello");
  recursiveQuery(ctx, s);

  return 0;
}
