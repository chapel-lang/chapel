/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/Parser.h"
#include "chpl/queries/Context.h"
#include "chpl/uast/all-uast.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <iostream>

using namespace chpl;
using namespace uast;
using namespace parsing;

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

static void displayErrors(const BuilderResult& br) {
  for (const auto& err : br.errors()) {
    auto out = buildErrorStr(err.path().c_str(), err.firstLine(),
                             err.message().c_str());
    printf("%s\n", out.c_str());
  }
}

static void assertErrorMatches(const BuilderResult& br, int idx,
                               const char* file,
                               int line,
                               const char* msg) {
  const auto& err = br.error(idx);
  auto output = buildErrorStr(err.path().c_str(), err.firstLine(),
                              err.message().c_str());
  auto expect = buildErrorStr(file, line, msg);
  assert(output == expect);
}

static void test0(Parser* parser) {
  auto br = parser->parseString("test0.chpl",
    R""""(
    var x: [?d] int;
    )"""");
  assert(br.numErrors() == 1);
  displayErrors(br);
  assertErrorMatches(br, 0, "test0.chpl", 2,
                     "Domain query expressions may currently only be used "
                     "in formal argument types");
}

static void test1(Parser* parser) {
  auto br = parser->parseString("test1.chpl",
    R""""(
    foo(bar=0, bar=1);
    )"""");
  assert(br.numErrors() == 1);
  displayErrors(br);
  assertErrorMatches(br, 0, "test1.chpl", 2,
                     "The named argument 'bar' is used more than once in "
                     "the same function call.");
}

static void test2(Parser* parser) {
  auto br = parser->parseString("test2.chpl",
    R""""(
    var x = new owned shared borrowed unmanaged C();
    )"""");
  assert(br.numErrors() == 3);
  displayErrors(br);
  assertErrorMatches(br, 0, "test2.chpl", 2,
                     "Type expression uses multiple class kinds: "
                     "owned shared");
  assertErrorMatches(br, 1, "test2.chpl", 2,
                     "Type expression uses multiple class kinds: "
                     "shared borrowed");
  assertErrorMatches(br, 2, "test2.chpl", 2,
                     "Type expression uses multiple class kinds: "
                     "borrowed unmanaged");
}

static void test3(Parser* parser) {
  auto br = parser->parseString("test3.chpl",
    R""""(
    deinit(foo);
    a.deinit(foo, bar);
    a.b.deinit();
    )"""");
  assert(br.numErrors() == 3);
  displayErrors(br);
  assertErrorMatches(br, 0, "test3.chpl", 2,
                     "direct calls to deinit() are not allowed");
  assertErrorMatches(br, 1, "test3.chpl", 3,
                     "direct calls to deinit() are not allowed");
  assertErrorMatches(br, 2, "test3.chpl", 4,
                     "direct calls to deinit() are not allowed");
}

static void test4(Parser* parser) {
  auto br = parser->parseString("test4.chpl",
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
    )"""");
  assert(br.numErrors() == 9);
  displayErrors(br);
  assertErrorMatches(br, 0, "test4.chpl", 2,
                     "Can't apply private to types yet");
  assertErrorMatches(br, 1, "test4.chpl", 3,
                     "Can't apply private to types yet");
  assertErrorMatches(br, 2, "test4.chpl", 4,
                     "Can't apply private to types yet");
  assertErrorMatches(br, 3, "test4.chpl", 6,
                     "Private declarations within function bodies "
                     "are meaningless");
  assertErrorMatches(br, 4, "test4.chpl", 9,
                     "Can't apply private to the fields or methods of "
                     "a class or record yet");
  assertErrorMatches(br, 5, "test4.chpl", 10,
                     "Can't apply private to the fields or methods of "
                     "a class or record yet");
  assertErrorMatches(br, 6, "test4.chpl", 12,
                     "Can't apply private to the fields or methods of "
                     "a class or record yet");
  assertErrorMatches(br, 7, "test4.chpl", 14,
                     "Private declarations within nested blocks are "
                     "meaningless");
  assertErrorMatches(br, 8, "test4.chpl", 16,
                     "Private declarations are meaningless outside of "
                     "module level declarations");
}

static void test5(Parser* parser) {
  auto br = parser->parseString("test5.chpl",
    R""""(
    const x = noinit;
    const ref y = noinit;
    )"""");
  assert(br.numErrors() == 2);
  displayErrors(br);
  assertErrorMatches(br, 0, "test5.chpl", 2,
                     "const variables specified with noinit must be "
                     "explicitly initialized");
  assertErrorMatches(br, 1, "test5.chpl", 3,
                     "const variables specified with noinit must be "
                     "explicitly initialized");
}

static void test6(Parser* parser) {
  auto br = parser->parseString("test6.chpl",
    R""""(
    proc foo() {
      config const x = 0;
      config const ref y = 0;
      config param p = 0.0;
      config var z = 0;
    }
    )"""");
  assert(br.numErrors() == 4);
  displayErrors(br);
  assertErrorMatches(br, 0, "test6.chpl", 3,
                     "Configuration constants are allowed only at module "
                     "scope");
  assertErrorMatches(br, 1, "test6.chpl", 4,
                     "Configuration constants are allowed only at module "
                     "scope");
  assertErrorMatches(br, 2, "test6.chpl", 5,
                     "Configuration parameters are allowed only at module "
                     "scope");
  assertErrorMatches(br, 3, "test6.chpl", 6,
                     "Configuration variables are allowed only at module "
                     "scope");
}

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);
  test1(p);
  test2(p);
  test3(p);
  test4(p);
  test5(p);
  test6(p);

  return 0;
}
