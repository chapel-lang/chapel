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

#include "test-parsing.h"

#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/CompilerFlags.h"
#include "chpl/framework/Context.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/all-uast.h"

#include <iostream>

static std::string
buildErrorStr(const char* file, int line, const char* msg) {
  std::string ret;
  ret += file;
  ret += ":";
  ret += std::to_string(line);
  ret += ": ";
  ret += msg;
  return ret;
}

static void displayErrors(Context* ctx, ErrorGuard& guard) {
  for (const auto& err : guard.errors()) {
    auto loc = err->location(ctx);
    auto out = buildErrorStr(loc.path().c_str(), loc.firstLine(),
                             err->message().c_str());
    printf("%s\n", out.c_str());
  }
}

static void assertErrorMatches(Context* ctx, ErrorGuard& guard,
                               int idx,
                               const char* file,
                               int line,
                               const char* msg) {
  const auto& err = guard.error(idx);
  auto loc = err->location(ctx);
  auto output = buildErrorStr(loc.path().c_str(), loc.firstLine(),
                              err->message().c_str());
  auto expect = buildErrorStr(file, line, msg);
  assert(output == expect);
}

static void test0(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    var x: [?d] int;
    )"""";
  auto path = UniqueString::get(ctx, "test0.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 1);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test0.chpl", 2,
                     "domain query expressions may currently only be used "
                     "in formal argument types");
  guard.clearErrors();
}

static void test1(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    foo(bar=0, bar=1);
    )"""";
  auto path = UniqueString::get(ctx, "test1.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 1);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test1.chpl", 2,
                     "the named argument 'bar' is used more than once in "
                     "the same function call");
  guard.clearErrors();
}

static void test2(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    var x = new owned shared borrowed unmanaged C();
    )"""";
  auto path = UniqueString::get(ctx, "test2.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 3);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test2.chpl", 2,
                     "type expression uses multiple memory management "
                     "strategies ('owned' and 'shared')");
  assertErrorMatches(ctx, guard, 1, "test2.chpl", 2,
                     "type expression uses multiple memory management "
                     "strategies ('shared' and 'borrowed')");
  assertErrorMatches(ctx, guard, 2, "test2.chpl", 2,
                     "type expression uses multiple memory management "
                     "strategies ('borrowed' and 'unmanaged')");
  guard.clearErrors();
}

static void test3(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    deinit(foo);
    a.deinit(foo, bar);
    a.b.deinit();
    )"""";
  auto path = UniqueString::get(ctx, "test3.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 3);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test3.chpl", 2,
                     "explicit calls to deinit() are not allowed");
  assertErrorMatches(ctx, guard, 1, "test3.chpl", 3,
                     "explicit calls to deinit() are not allowed");
  assertErrorMatches(ctx, guard, 2, "test3.chpl", 4,
                     "explicit calls to deinit() are not allowed");
  guard.clearErrors();
}

static void test4(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    private class C {}
    private record r {}
    private union u {}
    proc foo() {
      private var x = 0;
    }
    class cat {
      private var sleepTime = 0;
      private proc meow() {}
    }
    private proc r.baz() {}
    {
      private var x = 0;
    }
    for i in lo..hi do private var x = 0;
    private type T = int;
    )"""";
  auto path = UniqueString::get(ctx, "test4.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 10);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test4.chpl", 2,
                     "can't apply private to types yet");
  assertErrorMatches(ctx, guard, 1, "test4.chpl", 3,
                     "can't apply private to types yet");
  assertErrorMatches(ctx, guard, 2, "test4.chpl", 4,
                     "can't apply private to types yet");
  assertErrorMatches(ctx, guard, 3, "test4.chpl", 6,
                     "private declarations within function bodies "
                     "are meaningless");
  assertErrorMatches(ctx, guard, 4, "test4.chpl", 9,
                     "can't apply private to the fields or methods of "
                     "a class or record yet");
  assertErrorMatches(ctx, guard, 5, "test4.chpl", 10,
                     "can't apply private to the fields or methods of "
                     "a class or record yet");
  assertErrorMatches(ctx, guard, 6, "test4.chpl", 12,
                     "can't apply private to the fields or methods of "
                     "a class or record yet");
  assertErrorMatches(ctx, guard, 7, "test4.chpl", 14,
                     "private declarations within nested blocks are "
                     "meaningless");
  assertErrorMatches(ctx, guard, 8, "test4.chpl", 16,
                     "private declarations are meaningless outside of "
                     "module level declarations");
  assertErrorMatches(ctx, guard, 9, "test4.chpl", 17,
                     "can't apply private to types yet");
  guard.clearErrors();
}

static void test5(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    const x = noinit;
    const ref y = noinit;
    )"""";
  auto path = UniqueString::get(ctx, "test5.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 2);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test5.chpl", 2,
                     "const variables specified with noinit must be "
                     "explicitly initialized");
  assertErrorMatches(ctx, guard, 1, "test5.chpl", 3,
                     "const variables specified with noinit must be "
                     "explicitly initialized");
  guard.clearErrors();
}

static void test6(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    proc foo() {
      config const x = 0;
      config const ref y = 0;
      config param p = 0.0;
      config var z = 0;
    }
    )"""";
  auto path = UniqueString::get(ctx, "test6.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 4);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test6.chpl", 3,
                     "configuration constants are allowed only at module "
                     "scope");
  assertErrorMatches(ctx, guard, 1, "test6.chpl", 4,
                     "configuration constants are allowed only at module "
                     "scope");
  assertErrorMatches(ctx, guard, 2, "test6.chpl", 5,
                     "configuration parameters are allowed only at module "
                     "scope");
  assertErrorMatches(ctx, guard, 3, "test6.chpl", 6,
                     "configuration variables are allowed only at module "
                     "scope");
  guard.clearErrors();
}

static void test7(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    export var x = 0;
    )"""";
  auto path = UniqueString::get(ctx, "test7.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 1);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test7.chpl", 2,
                     "export variables are not yet supported");
  guard.clearErrors();
}

static void test8(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    proc emptyBody();
    )"""";
  auto path = UniqueString::get(ctx, "test8.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 1);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test8.chpl", 2,
                     "no-op procedures are only legal for extern functions");
  guard.clearErrors();
}

static void test9(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    extern proc shouldNotHaveBody() { writeln(0); }
    extern proc shouldNotThrow() throws;
    extern proc shouldNotDoEither() throws { writeln(0); }
    )"""";
  auto path = UniqueString::get(ctx, "test9.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 4);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test9.chpl", 2,
                     "extern functions cannot have a body");
  assertErrorMatches(ctx, guard, 1, "test9.chpl", 3,
                     "extern functions cannot throw errors");
  assertErrorMatches(ctx, guard, 2, "test9.chpl", 4,
                     "extern functions cannot have a body");
  assertErrorMatches(ctx, guard, 3, "test9.chpl", 4,
                     "extern functions cannot throw errors");
  guard.clearErrors();
}

static void test10(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    export proc foo() where false {}
    )"""";
  auto path = UniqueString::get(ctx, "test10.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 1);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test10.chpl", 2,
                     "exported functions cannot have where clauses");
  guard.clearErrors();
}

static void test11(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    class C {
      proc this { return 0; }
      iter these { yield nil; }
    }
    )"""";
  auto path = UniqueString::get(ctx, "test11.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 2);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test11.chpl", 3,
                     "method 'this' must have parentheses");
  assertErrorMatches(ctx, guard, 1, "test11.chpl", 4,
                     "method 'these' must have parentheses");
  guard.clearErrors();
}

static void test12(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
    proc f1(out x: int) type {}
    proc f2(inout x: int) type {}
    proc f3(out x: int) param {}
    proc f4(inout x: int) param {}
    )"""";
  auto path = UniqueString::get(ctx, "test12.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 4);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test12.chpl", 2,
                     "cannot use 'out' intent in a function returning "
                     "with 'type' intent");
  assertErrorMatches(ctx, guard, 1, "test12.chpl", 3,
                     "cannot use 'inout' intent in a function returning "
                     "with 'type' intent");
  assertErrorMatches(ctx, guard, 2, "test12.chpl", 4,
                     "cannot use 'out' intent in a function returning "
                     "with 'param' intent");
  assertErrorMatches(ctx, guard, 3, "test12.chpl", 5,
                     "cannot use 'inout' intent in a function returning "
                     "with 'param' intent");
  guard.clearErrors();
}

// TODO: Cannot get the internal/bundled module stuff to work properly.
static void test13(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  // Turn on the --warn-unstable flag for some warnings.
  CompilerFlags list;
  list.set(CompilerFlags::WARN_UNSTABLE, true);
  ctx->advanceToNextRevision(false);
  setCompilerFlags(ctx, list);

  std::string text =
    R""""(
    proc _bad1() {}
    var _bad2 = 0;
    class _bad3 {}
    proc chpl_bad4() {}
    var chpl_bad5 = 0;
    class chpl_bad6 {}
    )"""";
  auto path = UniqueString::get(ctx, "test13.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 6);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test13.chpl", 2,
                     "symbol names with leading underscores (_bad1) "
                     "are unstable");
  assertErrorMatches(ctx, guard, 1, "test13.chpl", 3,
                     "symbol names with leading underscores (_bad2) "
                     "are unstable");
  assertErrorMatches(ctx, guard, 2, "test13.chpl", 4,
                     "symbol names with leading underscores (_bad3) "
                     "are unstable");
  assertErrorMatches(ctx, guard, 3, "test13.chpl", 5,
                     "symbol names beginning with 'chpl_' (chpl_bad4) "
                     "are unstable");
  assertErrorMatches(ctx, guard, 4, "test13.chpl", 6,
                     "symbol names beginning with 'chpl_' (chpl_bad5) "
                     "are unstable");
  assertErrorMatches(ctx, guard, 5, "test13.chpl", 7,
                     "symbol names beginning with 'chpl_' (chpl_bad6) "
                     "are unstable");
  guard.clearErrors();
}

static void test14(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  // Turn on the --warn-unstable flag for some warnings.
  CompilerFlags list;
  list.set(CompilerFlags::WARN_UNSTABLE, true);
  ctx->advanceToNextRevision(false);
  setCompilerFlags(ctx, list);
  std::string text =
    R""""(
    union foo {}
    )"""";
  auto path = UniqueString::get(ctx, "test14.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 1);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test14.chpl", 2,
                     "unions are currently unstable and are expected "
                     "to change in ways that will break their "
                     "current uses");
  guard.clearErrors();
}

// test that operators must have valid operator names
static void test15(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
      record R {
        var field: int;
      }
      operator notAnOpName(z: R, type t: int) { return z.field; }
    )"""";
  auto path = UniqueString::get(ctx, "test15.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 1);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test15.chpl", 5,
                     "'notAnOpName' is not a legal operator name");
  guard.clearErrors();
}

// test that non-operator procs cannot have operator names
static void test16(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
      record R {
        var field: int;
      }
      proc :(z: R, type t: int) { return z.field; }
    )"""";
  auto path = UniqueString::get(ctx, "test16.chpl");
  setFileText(ctx, path, text);
  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 1);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test16.chpl", 5,
                     "operators cannot be declared without the operator keyword");
  guard.clearErrors();
}

// multiple query expressions cannot appear in an array type's domain
static void test17(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
      proc foo(x: [?d1, ?d2] int) {}
      foo([[0, 1], [2, 3]]);
    )"""";

  auto path = UniqueString::get(ctx, "test17.chpl");
  setFileText(ctx, path, text);

  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 2);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test17.chpl", 2,
                     "cannot query part of a domain");
  assertErrorMatches(ctx, guard, 1, "test17.chpl", 2,
                     "cannot query part of a domain");
  guard.clearErrors();
}

// Limit where empty '[]' domain type expressions can appear.
static void test18(void) {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);
  std::string text =
    R""""(
      var w: [] bytes;
      var x: [] int, y, z: [] real;
      record r { var x: [] string; }
      type T = [] string;
    )"""";

  auto path = UniqueString::get(ctx, "test18.chpl");
  setFileText(ctx, path, text);

  parseFileToBuilderResultAndCheck(ctx, path, UniqueString());

  assert(guard.numErrors() == 5);
  displayErrors(ctx, guard);
  assertErrorMatches(ctx, guard, 0, "test18.chpl", 2,
                     "variables cannot specify generic array types");
  assertErrorMatches(ctx, guard, 1, "test18.chpl", 3,
                     "variables cannot specify generic array types");
  assertErrorMatches(ctx, guard, 2, "test18.chpl", 3,
                     "variables cannot specify generic array types");
  assertErrorMatches(ctx, guard, 3, "test18.chpl", 4,
                     "fields cannot specify generic array types");
  assertErrorMatches(ctx, guard, 4, "test18.chpl", 5,
                     "generic array types are unsupported in this context");
  assert(guard.realizeErrors());
}


// bug reproducer for an issue where post-parse-checks was not
// being run after the first revision
static void test19() {
  Context context;
  Context* ctx = &context;

  std::string text1 =
    R""""(
    var x = 5;
    )"""";
  std::string text2 =
    R""""(
    var this = 5;
    )"""";

  {
    auto path = UniqueString::get(ctx, "test19.chpl");
    ErrorGuard guard(ctx);
    setFileText(ctx, path, text1);
    parseFileToBuilderResultAndCheck(ctx, path, UniqueString());
    assert(guard.numErrors() == 0);
  }

  ctx->advanceToNextRevision(true);

  {
    auto path = UniqueString::get(ctx, "test19.chpl");
    ErrorGuard guard(ctx);
    setFileText(ctx, path, text2);
    parseFileToBuilderResultAndCheck(ctx, path, UniqueString());
    assert(guard.numErrors() == 1);
    guard.realizeErrors();
  }
}

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
  test15();
  test16();
  test17();
  test18();
  test19();

  return 0;
}
