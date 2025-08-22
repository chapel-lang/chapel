/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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
#include "chpl/types/CompositeType.h"

// helper functions



/* Check that when resolving program, catchIdStr maps to the catch block,
    the catch block's error variable has the argIdStr given, and the error
    variable maps to the type given by varIdStr. If expectedErrorCount is given,
    check that the number of errors is as expected. If validateClassType is
    true, check that the error variable's type is a class type with the given
    name in errorTypeName.
*/
static void testIt(const char* testName,
                   const char* program,
                   const char* errorTypeName,
                   const char* catchIdStr,
                   const char* argIdStr,
                   const char* varIdStr,
                   int expectedErrorCount=0,
                   bool validateClassType=true) {
  printf("test %s\n", testName);
  Context* context = buildStdContext();
  ErrorGuard guard(context);
  auto path = UniqueString::get(context, testName);
  std::string contents = program;
  setFileText(context, path, contents);
  // parse it so that Context knows about the IDs
  const ModuleVec& vec = parseToplevel(context, path);

  for (auto m : vec) {
    m->dump();
  }

  // TODO: what else, if anything, to verify for the catch block's resolution?
  ID catchId = ID::fromString(context, catchIdStr);
  auto catchAst = parsing::idToAst(context, catchId);
  assert(catchAst);
  assert(catchAst->isCatch());

  if (argIdStr) {
    ID argId = ID::fromString(context, argIdStr);
    auto argAst = parsing::idToAst(context, argId);
    assert(argAst);
    assert(argAst->isVariable());

    auto argRes = resolvedExpressionForAst(context, argAst, nullptr, false);
    auto argType = argRes->type().type();
    assert(argType);
    if (validateClassType) {
      assert(argType->isClassType());
      auto argName = argType->toClassType()->basicClassType()->name();
      assert(argName == UniqueString::get(context, errorTypeName));
    }
  }

  if (varIdStr) {
    ID varId = ID::fromString(context, varIdStr);
    auto varAst = parsing::idToAst(context, varId);
    assert(varAst);
    assert(varAst->isIdentifier());
    auto varRes = resolvedExpressionForAst(context, varAst, nullptr, false);
    auto varType = varRes->type().type();
    assert(varType);
    if (validateClassType) {
      assert(varType->isClassType());
      auto varName = varType->toClassType()->basicClassType()->name();
      assert(varName == UniqueString::get(context, errorTypeName));
    }
  }

  if (expectedErrorCount > 0) {
    assert(guard.realizeErrors() == expectedErrorCount);
  }
}


static void test1() {
  // test for a catch block with explicit error variable that inherits from
  // Error.
  testIt("test1.chpl",
         R""""(
            module M {
              class SomeErrorName: Error { }
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : SomeErrorName {

                } catch e { }
              }
            }
         )"""",
         "SomeErrorName",
         "M.main@6",
         "M.main@4",
         "M.main@3");
}

static void test2() {
  // test for a catch block with explicit error variable that inherits from
  // Error. Includes an assignment of the variable to a new variable in the block
  testIt("test2.chpl",
         R""""(
            module M {
              class SomeErrorName: Error { }
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : SomeErrorName {
                  var j = e;
                } catch e { }
              }
            }
         )"""",
         "SomeErrorName",
         "M.main@8",
         "M.main@4",
         "M.main@3");
}

static void test3() {
  // test for a catch-all block with explicit error variable but no type
  testIt("test3.chpl",
         R""""(
            module M {
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e {  }
              }
            }
         )"""",
         "Error",
         "M.main@5",
         "M.main@3",
         nullptr);
}

static void test4() {
  // test for a catch-all block with explicit error variable and type
  testIt("test4.chpl",
         R""""(
            module M {
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : Error {  }
              }
            }
         )"""",
         "Error",
         "M.main@6",
         "M.main@4",
         "M.main@3");
}

static void test5() {
  // test for a catch-all block with no error variable
  testIt("test5.chpl",
         R""""(
            module M {
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch {  }
              }
            }
         )"""",
         nullptr,
         "M.main@4",
         nullptr,
         nullptr);
}

static void test6() {
// test for a bad error type (int) in the catch block
  std::string testName="test6.chpl";
  auto program =
         R""""(
            module M {
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : int {
                } catch e { }
              }
            }
         )"""";
         testIt(testName.c_str(), program,
                "int",
                "M.main@6",
                "M.main@4",
                "M.main@3",
                1,
                false);
}

static void test7() {
  // test for a bad error type (MyClass doesn't inherit from Error) in the catch block
  std::string testName="test7.chpl";
  auto program =
         R""""(
            module M {
              proc foo() throws { }
              class MyClass {}
              proc main() {
                try {
                  foo();
                } catch e : MyClass {
                } catch e { }
              }
            }
         )"""";
    testIt(testName.c_str(), program,
        "MyClass",
        "M.main@6",
        "M.main@4",
        "M.main@3",
        1,
        true);
}

static void test8() {
  // test nested try-catch, call testIt twice and pass values for outer and
  // inner try-catch, respectively
  testIt("test8a.chpl",
         R""""(
          module M {
          proc foo() throws { }
          proc main() {
            try {
              try {
                foo();
              } catch e : Error {
                throw e;
              }
              foo();
            } catch e {

            }
          }
         }
         )"""",
         "Error",
         "M.main@8",
         "M.main@4",
         "M.main@3");

  testIt("test8b.chpl",
         R""""(
          module M {
          proc foo() throws { }
          proc main() {
            try {
              try {
                foo();
              } catch e : Error {
                throw e;
              }
              foo();
            } catch e {

            }
          }
         }
         )"""",
         "Error",
         "M.main@15",
         "M.main@13",
         nullptr);
}

static void test9() {
  // test for a catch block with explicit error variable with multiple
  // layers of inheritance before Error.
  testIt("test9.chpl",
         R""""(
            module M {
              class SomeErrorName: Error { }
              class AnotherErrorName: SomeErrorName { }
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : AnotherErrorName {
                } catch e { }
              }
            }
         )"""",
         "AnotherErrorName",
         "M.main@6",
         "M.main@4",
         "M.main@3");
}

// "try without a catchall in a non-throwing function"
static void test10(Parser* parser) {
  // test for a try without a catchall in a non-throwing function
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test10.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          proc test() {
                            var x:int;
                            try {
                              var y = x;
                            }
                          }
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto func = mod->stmt(0)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "Try without a catchall in a non-throwing function");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}


// catchall placed before the end of a catch list in non-throwing function
static void test11(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test11.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          class MyError: Error { }
                          proc test() {
                            var x:int;
                            try {
                              var y = x;
                            } catch e: Error {
                            } catch e: MyError {
                            }
                          }
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto func = mod->stmt(1)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "catchall placed before the end of a catch list");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}


// cannot throw in a non-throwing function
static void test12(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test12.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          proc test() {
                            throw new Error();
                          }
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto func = mod->stmt(0)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "cannot throw in a non-throwing function");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}

// cannot throw in a non-throwing function, but can if it returns early!
static void test12b(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test12.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          proc test() {
                            return true;
                            throw new Error();
                          }
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto func = mod->stmt(0)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);
}


// after compilerError, try-catch analysis should go and try access unresolved
// AST.
static void test12c(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test12.chpl");

  std::string program = R""""(
                        module M {
                          proc test() {
                            compilerError("oopsie!");

                            // this is a regression test; in the past,
                            // trying to branch-sensitively traverse this
                            // conditional (which wasn't resolved, since we
                            // produced a compiler error) caused a hard crash
                            // of the compiler.
                            if true {
                              throw new Error();
                            } else {
                              // or don't throw
                            }
                          }
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto func = mod->stmt(0)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);
}

// "is in a try but not handled"
static void test13(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test13.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module MyModule {
                          proc throwit(i:int):int throws {
                            if true then
                              throw new owned Error();
                            return i;
                          }

                          proc tryReturn(i:int):int {
                            return try throwit(i);
                          }
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto func = mod->stmt(0)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);
  auto func2 = mod->stmt(1)->toFunction();
  assert(func2);
  auto resFunc2 = resolveConcreteFunction(ctx, func2->id());
  assert(resFunc2);

  // TODO: Should this be two errors? It seems like the Try without catchall
  // is the real error here. What's the purpose of the first error?
  assert(guard.numErrors() == 2);
  assert(guard.error(0)->message() == "call to throwing function 'throwit' is in a try but not handled");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.error(1)->message() == "Try without a catchall in a non-throwing function");
  assert(guard.error(1)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 2);
}

//TODO: "Could not find error variable for handler"

// "deinit is not permitted to throw"
static void test14(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test14.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module MyModule {
                          proc deinit() throws{
                            throw new owned Error();
                          }
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto func = mod->stmt(0)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "deinit is not permitted to throw");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}

// test for a try wit a catch but not a catchall in a non-throwing function
static void test15(Parser* parser) {

  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test15.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          class MyError: Error { }
                          proc test() {
                            var x:int;
                            try {
                              var y = x;
                            } catch e : MyError {

                            }
                          }
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto func = mod->stmt(1)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "Try without a catchall in a non-throwing function");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}

// check relaxed error checking mode




// check fatal error handling mode
static void test16(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test16.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        proc canThrow(i: bool): bool throws {
                          if i then
                            throw new owned Error();

                          return i;
                        }
                        canThrow(true);
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto modRes = resolveModule(ctx, mod->id());
  auto canThrowFn = mod->stmt(0)->toFunction();
  assert(canThrowFn);
  auto call = mod->stmt(1)->toFnCall();
  assert(call);
  assert(!guard.realizeErrors());
}

// check relaxed error handling mode
static void test17(Parser* parser) {

  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test17.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          proc canThrow(i: bool): bool throws {
                            if i then
                              throw new owned Error();

                            return i;
                          }
                          canThrow(true);
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto call = mod->stmt(1)->toFnCall();
  assert(call);
  auto modRes = resolveModule(ctx, mod->id());
  auto canThrowFn = mod->stmt(0)->toFunction();
  assert(canThrowFn);
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "call to throwing function 'canThrow' without throws, try, or try! (relaxed mode)");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}

// check fatal error handling mode prototype module
static void test18(Parser* parser) {

  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test18.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        prototype module M {
                          proc canThrow(i: bool): bool throws {
                            if i then
                              throw new owned Error();

                            return i;
                          }
                          canThrow(true);
                        }
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto call = mod->stmt(1)->toFnCall();
  assert(call);
  auto modRes = resolveModule(ctx, mod->id());
  auto canThrowFn = mod->stmt(0)->toFunction();
  assert(canThrowFn);

  assert(!guard.realizeErrors());
}


// check fatal error handling mode - implicit module
static void test19(Parser* parser) {

  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test19.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        proc canThrow(i: bool): bool throws {
                          if i then
                            throw new owned Error();

                          return i;
                        }
                        canThrow(true);
              )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto call = mod->stmt(1)->toFnCall();
  assert(call);
  auto modRes = resolveModule(ctx, mod->id());
  auto canThrowFn = mod->stmt(0)->toFunction();
  assert(canThrowFn);

  assert(!guard.realizeErrors());
}


static void test20(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test20.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          class OnlyASubClass : Error {}
                          proc f() {
                            try {
                              proc g() {
                                  try {
                                    // maybe something that throws
                                  } catch e : OnlyASubClass {

                                  } // no catchall - a problem
                              }
                            } catch {
                              // catch-all, we're good
                            }
                          }
                        }
                       )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto modRes = resolveModule(ctx, mod->id());
  auto func = mod->stmt(1)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);
  auto fTry = func->stmt(0)->toTry();
  assert(fTry);
  auto gFunc = fTry->stmt(0)->toFunction();
  assert(gFunc);
  auto gRes = resolveConcreteFunction(ctx, gFunc->id());
  assert(gRes);
  assert(guard.numErrors() == 1);
  assert(guard.error(0)->message() == "Try without a catchall in a non-throwing function");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 1);
}


static void test21(Parser* parser) {
 auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test21.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          class SomeSubType : Error {}
                          proc f() throws {
                            throw new Error();
                          }
                          try {
                            f();
                          } catch e : SomeSubType {

                          } catch {
                            // catch-all, we're good
                          }
                        }
                       )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto modRes = resolveModule(ctx, mod->id());
  auto func = mod->stmt(1)->toFunction();
  assert(func);
  auto resFunc = resolveConcreteFunction(ctx, func->id());
  assert(resFunc);
  assert(guard.numErrors() == 0);
  assert(!guard.realizeErrors());
}

static void test22(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test22.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          try {
                            f();
                          } catch e : SomeSubType {
                            // no catch-all - a problem
                          }
                          class SomeSubType : Error {}
                          proc f() throws {
                            throw new owned Error();
                          }
                        }
                       )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto modRes = resolveModule(ctx, mod->id());
  auto errClass = mod->stmt(1)->toClass();
  assert(errClass);
  auto func = mod->stmt(2)->toFunction();
  assert(func);
  assert(guard.numErrors() == 2);
  assert(guard.error(0)->message() == "call to throwing function 'f' is in a try but not handled");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.error(1)->message() == "Try without a catchall in a non-throwing function");
  assert(guard.error(1)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 2);
}

// nested try where catchall is only in outermost try
static void test23(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test23.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          try {
                            try {
                              f();
                            } catch (e1 : SomeSubType) {}
                          } catch {}
                          class SomeSubType : Error {}
                          proc f() throws {
                            throw new owned Error();
                          }
                        }
                       )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto modRes = resolveModule(ctx, mod->id());
  assert(!guard.realizeErrors());
}

// nested try without a catchall
static void test24(Parser* parser) {
  auto ctx = buildStdContext();
  auto path = UniqueString::get(ctx, "test24.chpl");

  ErrorGuard guard(ctx);
  std::string program = R""""(
                        module M {
                          try {
                            try {
                              f();
                            } catch (e1 : SomeSubType) {}
                          } catch (e1 : SomeSubType) {}
                          class SomeSubType : Error {}
                          proc f() throws {
                            throw new owned Error();
                          }
                        }
                       )"""";
  setFileText(ctx, path, program);
  const ModuleVec& vec = parseToplevel(ctx, path);
  auto mod = vec[0]->toModule();
  assert(mod);
  auto modRes = resolveModule(ctx, mod->id());
  // guard.realizeErrors();
  assert(guard.numErrors() == 2);
  assert(guard.error(0)->message() == "call to throwing function 'f' is in a try but not handled");
  assert(guard.error(0)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.error(1)->message() == "Try without a catchall in a non-throwing function");
  assert(guard.error(1)->kind() == ErrorBase::Kind::ERROR);
  assert(guard.realizeErrors() == 2);
}

static void test25() {
  auto ctx = buildStdContext();
  ErrorGuard guard(ctx);

  auto t = resolveTypeOfX(ctx,
                R""""(
                  proc inner() throws { throw new Error(); }
                  proc outer() throws { try inner(); return 0; }
                  var x = try! outer();
                )"""");
  assert(t && t->isIntType());
  assert(!guard.realizeErrors());
}


// TODO: error handling in defer blocks must be complete


int main() {
  Context* ctx = buildStdContext();
  auto parser = Parser::createForTopLevelModule(ctx);

  Parser* p = &parser;

  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10(p);
  test11(p);
  test12(p);
  test12b(p);
  test12c(p);
  test13(p);
  test14(p);
  test15(p);
  test16(p);
  test17(p);
  test18(p);
  test19(p);
  test20(p);
  test21(p);
  test22(p);
  test23(p);
  test24(p);

  test25();

  return 0;
}
