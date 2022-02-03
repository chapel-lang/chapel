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

#include "chpl/queries/Context.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/all-uast.h"
#include "chpl/parsing/Parser.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;
using namespace uast;
using namespace parsing;

static void test0(Parser* parser) {
  // test Module stringify
  auto parseResult = parser->parseString("test0.chpl", "");
  auto mod = parseResult.singleModule();
  std::ostringstream ss;
  mod->stringify(ss, CHPL_SYNTAX);
  assert(!ss.str().empty());
}

static void test1(Parser* parser) {
  // test Identifier stringify
  auto parseResult = parser->parseString("test1.chpl", "x;");
  auto mod = parseResult.singleModule();
  auto identifier = mod->stmt(0)->toIdentifier();
  std::ostringstream ss;
  identifier->stringify(ss, CHPL_SYNTAX);
  assert(!ss.str().empty());
}

static void test2(Parser* parser) {
  // test Class stringify
  auto parseResult = parser->parseString("test2.chpl", "class C { }");
  auto mod = parseResult.singleModule();
  const AggregateDecl* agg = mod->stmt(0)->toAggregateDecl();
  auto cls = agg->toClass();
  std::ostringstream ss;
  cls->stringify(ss, CHPL_SYNTAX);
  assert(!ss.str().empty());
}

static void test3(Parser* parser) {
  // test record stringify
  auto parseResult = parser->parseString("test3.chpl",
                                         "record R { var x; proc method() { } }");
  auto mod = parseResult.singleModule();
  const AggregateDecl* agg = mod->stmt(0)->toAggregateDecl();
  auto rec = agg->toRecord();
  std::ostringstream ss;
  rec->stringify(ss, CHPL_SYNTAX);
  assert(!ss.str().empty());
}

static void test4(Parser* parser) {
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
               foreach x in zip(a, b) {
                 foo();
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
  std::ostringstream ss;
  mod->stringify(ss, CHPL_SYNTAX);
  assert(!ss.str().empty());
  std::cerr << ss.str();
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
  test4(p);

  return 0;
}
