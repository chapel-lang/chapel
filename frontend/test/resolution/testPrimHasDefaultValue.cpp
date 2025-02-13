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

#include "test-common.h"
#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static void ensureExpectedDefaultValue(Context* context, const char* type, bool hasDefault, const char* prelude = "", bool skipErrors = false) {
  ErrorGuard guard(context);

  auto primCallProgram = std::string(prelude) + "\nparam result = __primitive(\"type has default value\", " + type + ");";
  auto defaultInitProgram = std::string(prelude) + "\n" + "proc foo() { var x: " + type + "; }";

  static int userFileCounter = 0;
  auto filenameA = UniqueString::get(context, "A" + std::to_string(userFileCounter) + ".chpl");
  auto filenameB = UniqueString::get(context, "B" + std::to_string(userFileCounter++) + ".chpl");

  setFileText(context, filenameA, primCallProgram);
  setFileText(context, filenameB, defaultInitProgram);

  printf("parsing program:\n%s\n\n", primCallProgram.c_str());
  auto modA = parse(context, filenameA, UniqueString())[0];
  auto modAR = resolveModule(context, modA->id());
  auto result = modAR.byAst(findVariable(modA, "result")).type();
  ensureParamBool(result, hasDefault);

  if (skipErrors) {
    printf("skipping checking for default-init errors\n");
    return;
  }
  printf("parsing program (%s):\n%s\n\n",
         hasDefault ? "not expecting errors" : "expecting errors",
         defaultInitProgram.c_str());
  auto modB = parse(context, filenameB, UniqueString())[0];
  std::ignore = resolveConcreteFunction(context, modB->stmt(modB->numStmts()-1)->id());
  if (hasDefault) {
    assert(!guard.realizeErrors());
  } else {
    assert(guard.realizeErrors());
  }
}

int main() {
  auto ctx = buildStdContext();
  ensureExpectedDefaultValue(ctx, "int", true);
  ensureExpectedDefaultValue(ctx, "int(16)", true);
  ensureExpectedDefaultValue(ctx, "string", true);
  ensureExpectedDefaultValue(ctx, "(int, int)", true);
  ensureExpectedDefaultValue(ctx, "owned C", false, "class C {}");
  ensureExpectedDefaultValue(ctx, "owned C?", true, "class C {}");
  ensureExpectedDefaultValue(ctx, "owned C", false, "class C { proc init(x: int) {} }");
  ensureExpectedDefaultValue(ctx, "owned C?", true, "class C { proc init(x: int) {} }");
  ensureExpectedDefaultValue(ctx, "R", true, "record R {}");
  ensureExpectedDefaultValue(ctx, "R", false, "record R { proc init(x: int) {} }");
  ensureExpectedDefaultValue(ctx, "(R, R)", true, "record R {}");
  ensureExpectedDefaultValue(ctx, "(R, R)", false, "record R { proc init(x: int) {} }", /*skipErrors=*/true);

  // seems like we can't build default-initializers for records with default-initializable fields
  // in all cases. Skip these tests as a consequence.
  //
  // ensureExpectedDefaultValue(ctx, "Wrap(R)", true, "record Wrap { var field; } record R {}");
  // ensureExpectedDefaultValue(ctx, "Wrap(R)", false, "record Wrap { var field; } record R { proc init(x: int) {} }");
}
