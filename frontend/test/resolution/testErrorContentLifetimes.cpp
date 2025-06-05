/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

using namespace chpl;
using namespace chpl::parsing;


static void test(Context* ctx, const char* program, int numErrors, bool gc) {
  ErrorGuard guard(ctx);
  ctx->advanceToNextRevision(gc);

  auto path = UniqueString::get(ctx, "simple.chpl");
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  assert(vec.size() == 1);

  if (gc) ctx->collectGarbage();
  guard.printErrors();
  assert(guard.realizeErrors() == 2);
}

int main(int argc, char** argv) {

  const char* program1 = R""""(
    proc bar() {}
    module A {
      proc foo() {}
    }
    enum a {}
  )"""";
  const char* program2 = R""""(
    proc bar() {}
    module A {
      proc foo() {}
    }
    enum a {3}
  )"""";

  Context context;
  Context* ctx = &context;

  test(ctx, program1, 2, true);
  test(ctx, program2, 2, true);
  test(ctx, program1, 2, true);
  test(ctx, program2, 2, true);
  test(ctx, program2, 2, true);

  return 0;
}
