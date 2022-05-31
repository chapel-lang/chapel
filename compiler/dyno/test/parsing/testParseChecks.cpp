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

int main() {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test0(p);
  test1(p);
  test2(p);

  return 0;
}
