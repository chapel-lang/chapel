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

#include <climits>
#include <map>
#include <vector>
#include "chpl/queries/Context.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/all-uast.h"
#include "chpl/parsing/Parser.h"
#include "chpl/uast/chpl-syntax-printer.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;
using namespace uast;
using namespace parsing;


// This macro just helps keep the tests a little cleaner
// rather than redefine all this for every type we want to test
#define TEST_STRINGIFY_TYPE(val)                          \
    {                                                     \
      std::ostringstream ss;                              \
      stringify<std::decay_t<decltype(val)>> stringifier; \
      stringifier(ss, DEBUG_DETAIL, val);                 \
      assert(!ss.str().empty());                          \
    }


static void stringifyNode(const ASTNode* node, chpl::StringifyKind kind) {
    // recurse through the nodes and make sure each can call stringify()
    // and produce a non-empty result (for now)
    // this is a little convoluted as each ASTNode is also calling
    // ASTNode.dumpHelper() on all of its children
    for (const ASTNode* child : node->children()) {
      stringifyNode(child, kind);
    }
    std::ostringstream ss;
    node->stringify(ss, kind);
//    assert(!ss.str().empty());
//    std::cerr << ss.str() << std::endl;
}

static void test0(Parser* parser) {
  // stringify an empty Module
  auto parseResult = parser->parseString("test0.chpl", "");
  auto mod = parseResult.singleModule();
  std::ostringstream ss;
  mod->stringify(ss, CHPL_SYNTAX);
  assert(!ss.str().empty());
}

static void test1(Parser* parser) {
  // the one test to rule them all
  std::string testCode;
  testCode = R""""(
             module Test4 {
               use Map;
               import Foo as X ;
               include module Foo;
               include private module Bar;
               include prototype module Baz;
               include private prototype module Zing;
               include public prototype module A;
               include public module B;
               enum myEnum { a=ii, b=jj }
               extern record foo {};
               extern "struct bar" record bar {};
               export record dog { var x = 0; }
               export "meow" record cat { var x = 0; }
               extern union baz {}
               extern "union thing" union thing {}
               class C {
                 /* this class does nothing */
                 var x;
                 var (x, y) = tup;
                 proc Cproc(val: int) {
                   if val > x then x = val;
                 }
               }
               record R {
                 // my record comment
                 var x,y :int;
                 proc df(arg) { }
                 proc const cnst(arg) const { }
                 proc const ref cnstrf(arg) const ref { }
                 proc ref rf(arg) ref { }
                 proc param prm(arg) param { }
                 proc type tp(arg) type { }
               }
               proc R.df2(arg) { }
               proc const R.cnst2(arg) const { }
               proc const ref R.cnstrf2(arg) const ref { }
               proc ref R.rf2(arg) ref { }
               proc param R.prm2(arg) param { }
               proc type R.tp2(arg) type { }
               record MyCircle {
                 var impl: MyCircleImpl;
                 proc getImplOrFail() {
                   if impl == nil then
                     halt('impl is nil');
                   else
                     return impl;
                 }
                 forwarding getImplOrFail();
               }
               proc main(args:[]string) {
                 try! {
                   var a:int;
                   a = args[1];
                 }
                 try {
                   throw fooError();
                 } catch e1: ErrorType1 {
                   writeln('E1');
                 } catch e2: ErrorType2 {
                   writeln('E2');
                   return 1;
                 } catch {
                   halt('Unknown error');
                 }
                 on foo {
                   var a;
                 }
                 local do
                   var a;
                 begin
                 with (ref a, var b = foo())  {
                   writeln(a);
                 }
                 [x in foo with (ref thing)] {
                   foo();
                 }
                 while foo() do {
                   /* comment 2 */
                   bar();
                 }
               }
               sync {
                 begin foo();
               }
               serial do
                 var a;
               iter foo(): int {
                 yield bar() ;
               }
               var thing = 0;
               label outer for i in myRange1 {
                 for j in myRange2 do
                   if !(i%j) { found = j; break outer; }
               }
               var r = new R(9,10);
               writeln(r.x:string);
               type t = r.y.type;
               writeln(t:string);
               select foo {
                 when x do f1();
                 otherwise do f2();
               }

               delete foo, bar, baz;
               defer
                 foo();
               coforall x in foo with (ref thing) do {
                 foo();
               }
               cobegin {
                 writeln(a);
               }
             }
             )"""";
  auto parseResult = parser->parseString("Test4.chpl",
                                         testCode.c_str());
  assert(!parseResult.numErrors());
  auto mod = parseResult.singleModule();
  assert(mod);
  stringifyNode(mod, CHPL_SYNTAX);
}

static void test2(Parser* parser) {

  TEST_STRINGIFY_TYPE(std::numeric_limits<float>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<double>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<long double>::max())

  TEST_STRINGIFY_TYPE(std::numeric_limits<int>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<long int>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<short int>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<unsigned int>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<unsigned short int>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<unsigned long int>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<unsigned long long int>::max())
  TEST_STRINGIFY_TYPE(std::numeric_limits<long long int>::max())

  TEST_STRINGIFY_TYPE(std::string("so long, and thanks for all the phish"))
  TEST_STRINGIFY_TYPE(true)

  TEST_STRINGIFY_TYPE(std::make_pair(42, std::string("the answer to life, "
                                                     "the universe, "
                                                     "and everything")))
  TEST_STRINGIFY_TYPE(std::make_tuple(std::string("blue"),
                                      true, 35,
                                      std::make_pair(42, true)))
  std::vector<int> v{7, 5, 16, 8};
  TEST_STRINGIFY_TYPE(v)
  std::set<double> dSet = {1.23, 3.14, 2.789};
  TEST_STRINGIFY_TYPE(dSet)
  std::unordered_map<int, std::string> months = {{3, std::string("Mar.")},
                                                 {1, std::string("Jan.")},
                                                 {2, std::string("Feb.")}};
  TEST_STRINGIFY_TYPE(months)
  std::map<int, std::string> weekDays = {{2, std::string("Tue.")},
                                         {0, std::string("Sun.")},
                                         {1, std::string("Mon.")},
                                         {3, std::string("Wed.")}};
  TEST_STRINGIFY_TYPE(weekDays)

}


static void test3(Parser* parser) {
  // stringify an empty Module
  auto parseResult = parser->parseString("test3.chpl", "proc bar(x: int) {\n}\n"
                                                       "proc foo(X: [?Dlocal] real) {\n}\n"
                                                       "proc baz(A: borrowed C) {\n}\n"
                                                       "class C {\n"
                                                       "  proc ref setClt(rhs: borrowed C) {\n}\n}\n"
                                                       "proc test(const ref arg:unmanaged MyClass) {\n}\n"
                                                       "inline proc (borrowed object?).hash(): uint {\n}\n"
                                                       "proc bark(c = new C()) {\n}\n");
  auto mod = parseResult.singleModule();
  std::ostringstream ss;
//  mod->stringify(ss, CHPL_SYNTAX);
//  assert(!ss.str().empty());
//  stringifyNode(mod, CHPL_SYNTAX);
  auto barDecl = mod->stmt(0)->toFunction();
  auto fooDecl = mod->stmt(1)->toFunction();
  auto bazDecl = mod->stmt(2)->toFunction();
  auto cDecl = mod->stmt(3)->toClass();
  auto testDecl = mod->stmt(4)->toFunction();
  auto inlineDecl = mod->stmt(5)->toFunction();
  auto barkDecl =mod->stmt(6)->toFunction();
  assert(cDecl);
  assert(inlineDecl);
  auto setCltDecl = cDecl->child(0)->toFunction();
  assert(setCltDecl);
  assert(barDecl);
  assert(fooDecl);
  assert(bazDecl);
  assert(testDecl);
  assert(barkDecl);
  printUserString(ss, inlineDecl);
//  std::cerr << ss.str() << std::endl;
  assert(!ss.str().empty());
  ss.str("");
  ss.clear();
  printUserString(ss, testDecl);
  assert(!ss.str().empty());
//  std::cerr << ss.str() << std::endl;
  ss.str("");
  ss.clear();
  printUserString(ss, setCltDecl);
  assert(!ss.str().empty());
//  std::cerr << ss.str() << std::endl;
  printUserString(ss,barDecl);
  assert(!ss.str().empty());
//  std::cerr << ss.str() << std::endl;
  ss.str("");
  ss.clear();
  printUserString(ss,barkDecl);
  assert(!ss.str().empty());
//  std::cerr << ss.str() << std::endl;
  ss.str("");
  ss.clear();
  printUserString(ss, fooDecl);
  assert(!ss.str().empty());
//  std::cerr << ss.str() << std::endl;
  ss.str("");
  ss.clear();

  printUserString(ss, bazDecl);
  assert(!ss.str().empty());
//  std::cerr << ss.str() << std::endl;
}


int main(int argc, char** argv) {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();
  test0(p);
  test1(p);
  test2(p);
  test3(p);

  return 0;
}
