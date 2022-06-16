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
#define TEST_STRINGIFY_TYPE(val)                      \
{                                                     \
  std::ostringstream ss;                              \
  stringify<std::decay_t<decltype(val)>> stringifier; \
  stringifier(ss, DEBUG_DETAIL, val);                 \
  assert(!ss.str().empty());                          \
}

// Helper macro for testing output from printUserString
#define TEST_USER_STRING(funcDef, val)                           \
{                                                                \
  std::ostringstream ss;                                         \
  auto parseResult = parser->parseString("test3.chpl", funcDef); \
  auto mod = parseResult.singleModule();                         \
  auto funcDecl = mod->stmt(0)->toFunction();                    \
  assert(funcDecl);                                              \
  printFunctionSignature(ss, funcDecl);                          \
  std::cerr << ss.str() << std::endl;                            \
  assert(ss.str() == val);                                       \
}


#define TEST_CHPL_SYNTAX(src, val)                           \
{                                                            \
  std::ostringstream ss;                                     \
  auto parseResult = parser->parseString("test5.chpl", src); \
  auto mod = parseResult.singleModule();                     \
  assert(mod);                                               \
  printChapelSyntax(ss, mod);                                \
  std::cerr << ss.str() << std::endl;                        \
  assert(ss.str() == val);                                   \
}


static void stringifyNode(const AstNode* node, chpl::StringifyKind kind) {
  // recurse through the nodes and make sure each can call stringify()
  // and produce a non-empty result (for now)
  // this is a little convoluted as each AstNode is also calling
  // AstNode.dumpHelper() on all of its children
//  for (const AstNode* child : node->children()) {
//    stringifyNode(child, kind);
//  }
  std::ostringstream ss;
  node->stringify(ss, kind);
  // don't test an empty domains as in formal array decls like `proc main(args:[] string)`
  // don't test comments for now as they are not printed by the chpl-syntax-printer
  if (!(node->isDomain() && node->toDomain()->numChildren() == 0)
      && !node->isComment() && !node->isEmptyStmt())
    assert(!ss.str().empty());
  std::cerr << ss.str() << std::endl;
}


static void test1(Parser* parser) {
  // the one test to rule them all
  std::string testCode;
  testCode = R""""(
   use Map;
   require "foo.h", "foo.c";
   import Foo as X;
   include module Foo;
   include private module Bar;
   include prototype module Baz;
   include private prototype module Zing;
   include public prototype module A;
   include public module B;
   enum myEnum { a=ii, b=jj }
   extern record foo {}
   extern "struct bar" record bar {}
   export record dog { var x = 0; }
   export "meow" record cat { var x = 0; }
   extern union baz {}
   extern "union thing" union thing {}
   var x = !(this && that);
   const j = !this && that;
   const d : int = (((i + j) * (i + j + 1)) >> 1) + i + 1;
   Au(i)=+ reduce [j in 0..inRange-1 by 2] ((u(j) * eval_A(j,i)) + (u(j+1) * eval_A(j+1, i)));
   const vBv = + reduce (u*v);
   const vBv = + reduce [(u,v) in zip(U,V)] (u * v);
   config var x, y, z : int;
   config const (x, y): (int, int), (z, y): (bool, int), (x, z): (int, bool);
   config var (x, y) = (2, 3);
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
     var a, b:int, c, d = 1;
     var x: int, y = 3, z: real;
     proc df(arg) { }
     proc const cnst(arg) const { }
     proc const ref cnstrf(arg) const ref { }
     private proc ref rf(arg) ref : string where !(isDomainType(eltType) || isArrayType(eltType)) { }
     proc param prm(arg) param { }
     proc type tp(arg) type { }
   }
   var eval = a**(b**c) != (a**b)**c;
   var eval2 =  (-1)**2;
   var eval3 =  1**(-2);
   class D : C {
     override proc Cproc(val: int) { writeln(val:string); }
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
     } catch (e2: ErrorType2) {
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
     begin writeln(x);

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
   var myBytes = b"this string to bytes";
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
   do {
     writeln(i);
     i += 1;
   } while i < 5;
   forall i in 1..10 {
     x.fetchAdd(i);
   }
   proc magnitude( (x,y,z):3*real ) {
   }
   proc f(ref x: int ...?k) {
     writeln(x);
   }
   writeln(+ scan A);
   forall myIterator() with (+ reduce x) {
     x = 1;
   }
   forall elm in A with (PlusReduceOp(int) reduce sum) {
     sum reduce= elm;   // bools are implicitly coerced to 'int' input type
     writeln(sum);      // accumulation state: int
   }
   __primitive("=", x, y);
   let x = 0 in writeln(x);
   var x = if foo then bar else baz;
   manage myManager() as myResource do
     myResource.doSomething();
   var C = for 1..10 do 5;
   var C = forall 1..10 do 5;

   module DefinesOp {
     class Foo {
       var x: int;
     }

     operator align(r: range(?i), count: Foo) {
       writeln("In DefinesOp.align");
       return r align count.x;
     }

     operator by(r: range(?i), count: Foo) {
       writeln("In DefinesOp.by");
       return r by count.x;
     }
   }

   proc main() {

     {
       use DefinesOp only align, Foo;

       var r1 = 0..6;
       var a = new Foo(3);
       var r1b = r1 by 4 align a;
       writeln(r1b);
       for i in r1b {
         writeln(i);
       }
     }

     {
       use DefinesOp only by, Foo;

       var r1 = 0..6;
       var foo = new Foo(3);
       writeln(r1 by foo);
     }
   }
   module M2 {
     proc main() {
       import M1.{foo};
       import M1.foo;
       writeln("In M2's main.");
       foo();
     }
   }
   type t;
   var Array:[1..1] t;
   forwarding Array[1]!;
   const A: [1..10] int = [i in 1..10] if (i < 3) then 100 else i;
   writeln(A);
   var Cs = [i in nums] if i then new C[i] else nil: owned C?;
   proc accumulateOntoState(ref state, ((dist, ctr), datum, centers)) {  }
   var (x,y,(v,z)) :(string, real, (int, real));
   while true do
     ;
   proc noOp(x:int);
   {
     // not the body
   }
   proc noOp() { }
   hi();
   ;
   ;
   tester();
   ;
   ;
   ;
   bye();
   module C {
     public use A only;
   }

   import this.DefinesOp.{Foo, Foo2, Bar};
   import this.DefinesOp.align;
   import this.DefinesOp.+;

  private proc param R.prm2(arg) param : string { }
  iter foo() {
   for i in 1..10 do yield try! i;
  }
  XNew(ij) = (X(ij+north) + X(ij+south) + X(ij+east) + X(ij+west)) / 4.0;
  )"""";
  auto parseResult = parser->parseString("Test1.chpl",
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

  TEST_USER_STRING("proc bar(x: int) {\n}\n", "bar(x: int)")
  TEST_USER_STRING("proc foo(X: [?Dlocal] real) {\n}\n",
                   "foo(X: [?Dlocal] real)")
  TEST_USER_STRING("proc baz(A: borrowed C) {\n}\n", "baz(A: borrowed C)")
  TEST_USER_STRING("proc test(const ref arg:unmanaged MyClass) {\n}\n",
                   "test(const ref arg: unmanaged MyClass)")
  TEST_USER_STRING("inline proc (borrowed object?).hash(): uint {\n}\n",
                   "(borrowed object?).hash()")
  TEST_USER_STRING("proc bark(c = new C()) {\n}\n", "bark(c = new C())")
  TEST_USER_STRING("proc ref C.setClt2(rhs: borrowed C) {\n}\n",
                   "ref C.setClt2(rhs: borrowed C)")
  TEST_USER_STRING("proc main(args: [] string) {\n}", "main(args: [] string)")
  TEST_USER_STRING("proc MYPROC(FORMAL: single int) { }",
                   "MYPROC(FORMAL: single int)")
  TEST_USER_STRING("inline operator ==(a: _nilType, b: _nilType) param return true;",
                   "==(a: _nilType, b: _nilType)")
  TEST_USER_STRING("private proc param R.prm2(arg) param : string { }",
                   "private param R.prm2(arg)")
  TEST_USER_STRING("proc procRefC(ref arg: borrowed C?) { }",
                   "procRefC(ref arg: borrowed C?)")
  TEST_USER_STRING("operator =(ref r3:R, r4:R){}", " = (ref r3: R, r4: R)")
  TEST_USER_STRING("proc foo(x...) {}", "foo(x ...)")
  TEST_USER_STRING("proc bar(type x...) {}", "bar(type x ...)")
  TEST_USER_STRING("proc bar(type x...?k) {}", "bar(type x ...?k)")
  TEST_USER_STRING("proc foo(x: borrowed C(t=?tt, r=?rr), y: borrowed C(tt, rr)) {}",
                   "foo(x: borrowed C(t = ?tt, r = ?rr), y: borrowed C(tt, rr))")
  TEST_USER_STRING("proc (int(32)).foo() {\n}", "(int(32)).foo()")
  TEST_USER_STRING("proc proc1(arg: Monkey1?) { }", "proc1(arg: Monkey1?)" )
  TEST_USER_STRING("proc proc1(arg: 2*Monkey1?) { }", "proc1(arg: 2*Monkey1?)")
  TEST_USER_STRING("proc proc1(arg: owned 2*Monkey1?) { }",
                   "proc1(arg: owned 2*Monkey1?)")
  TEST_USER_STRING("proc init(fScore : borrowed [..] real) {}",
                   "init(fScore: borrowed [..] real)")
  TEST_USER_STRING("proc difDecl(arg1: 3*string) {}",
                   "difDecl(arg1: 3*string)")
  TEST_USER_STRING("proc multiDimension(arg2: 3*(4*complex)): 3*(4*complex) {}",
                   "multiDimension(arg2: 3*(4*complex))")

}

static void test4(Parser* parser) {
  auto parseResult = parser->parseString("test4.chpl",
                                          "class C {\n"
                                          "proc ref setClt(rhs: borrowed C) {\n}\n}\n");
  auto mod = parseResult.singleModule();
  std::ostringstream ss;
  auto cDecl = mod->stmt(0)->toClass();
  assert(cDecl);

  auto setCltDecl = cDecl->child(0)->toFunction();
  assert(setCltDecl);
  printFunctionSignature(ss, setCltDecl);
  assert(ss.str() == "ref setClt(rhs: borrowed C)");
}

static void testModules(Parser* parser) {
  TEST_CHPL_SYNTAX("", "")
  TEST_CHPL_SYNTAX("module M {}", "module M {\n\n}\n")
}

static void testUse(Parser* parser) {
  TEST_CHPL_SYNTAX("use Map;", "use Map;")
}

static void testRequire(Parser* parser) {
  TEST_CHPL_SYNTAX("require \"foo.h\", \"foo.c\";",
                   "require \"foo.h\", \"foo.c\";")
}

static void testInclude(Parser* parser) {
  TEST_CHPL_SYNTAX("include module Foo;", "include module Foo;")
  TEST_CHPL_SYNTAX("include private module Bar;", "include private module Bar;")
  TEST_CHPL_SYNTAX("include prototype module Baz;",
                   "include prototype module Baz;")
  TEST_CHPL_SYNTAX("include private prototype module Zing;",
                   "include private prototype module Zing;")
  TEST_CHPL_SYNTAX("include public prototype module A;",
                   "include public prototype module A;")
  TEST_CHPL_SYNTAX("include public module B;", "include public module B;")
}

static void testEnum(Parser* parser) {
  TEST_CHPL_SYNTAX("enum myEnum { a=ii, b=jj }",
                   "enum myEnum { a = ii, b = jj }")
}

static void testExtern(Parser* parser) {
  TEST_CHPL_SYNTAX("extern record foo {}", "extern record foo {\n\n}")
  TEST_CHPL_SYNTAX("extern \"struct bar\" record bar {}",
                   "extern \"struct bar\" record bar {\n\n}")
  TEST_CHPL_SYNTAX("extern union baz {}", "extern union baz {\n\n}")
  TEST_CHPL_SYNTAX("extern \"union thing\" union thing {}",
                   "extern \"union thing\" union thing {\n\n}")
}

static void testDecl(Parser* parser) {
  TEST_CHPL_SYNTAX("var x = !(this && that);", "var x = !(this && that);")
  TEST_CHPL_SYNTAX("const j = !this && that;", "const j = !this && that;")
  TEST_CHPL_SYNTAX("const d : int = (((i + j) * (i + j + 1)) >> 1) + i + 1;",
                   "const d: int = (i + j) * (i + j + 1) >> 1 + i + 1;")
  TEST_CHPL_SYNTAX("const vBv = + reduce (u*v);", "const vBv = + reduce (u * v);")
  TEST_CHPL_SYNTAX("const vBv = + reduce [(u,v) in zip(U,V)] (u * v);",
                   "const vBv = + reduce [(u, v) in zip(U, V)] (u * v);")
  TEST_CHPL_SYNTAX("config var x,y,z : int;", "config var x, y, z: int;")
  TEST_CHPL_SYNTAX("config const (x, y): (int, int), (z, y): (bool, int);",
                   "config const (x, y): (int, int), (z, y): (bool, int);")
  TEST_CHPL_SYNTAX("config var (x,y) = (2,3);", "config var (x, y) = (2, 3);")
}

//TODO: Write many more specific tests for the format of different node types

int main(int argc, char** argv) {
  Context context;
  Context* ctx = &context;

  auto parser = Parser::build(ctx);
  Parser* p = parser.get();

  test1(p);
  test2(p);
  test3(p);
  test4(p);
  testModules(p);
  testUse(p);
  testRequire(p);
  testInclude(p);
  testEnum(p);
  testExtern(p);
  testDecl(p);


  return 0;
}
