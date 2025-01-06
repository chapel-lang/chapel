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

#include "test-common.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/Module.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace uast;

// Reparsing gets updated location information
static void test1() {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = UniqueString::get(ctx, "input.chpl");
  std::string contents = "foobarbaz@&$\n"; // Something impossible to parse.
  setFileText(ctx, path, contents);

  auto& mv = parse(ctx, path, UniqueString());
  assert(mv.size() >= 1);
  const Module* mod = mv[0];
  assert(mod);
  assert(guard.realizeErrors());
}

int main() {
  test1();
  return 0;
}
