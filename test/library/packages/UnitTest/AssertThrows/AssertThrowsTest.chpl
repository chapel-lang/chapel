use Reflection;
use UnitTest;

const sep = "=="*40;

proc testAssert(func, type errorType, const args:?=none, const match:string="",
                const reason:string=""): void
                where isSubtype(errorType, Error) &&
                (isNothing(args) || isTuple(args)) {
  var test = new Test();
  try {
    test.assertThrows(func=func, errorType=errorType, args=args, match=match);
  } catch e {
    writeln("Error Caught in " + reason);
    writeln(e);
    writeln(sep);
  }
}

proc main() {
  expectedCustomError();
  expectedCustomErrorAnonymous();
  expectedErrorWrappedMethod();
  expectedSuperError();
  expectedCustomErrorWithArgs();
  expectedErrorRecordFunctor();
  expectedErrorClassFunctor();

  errorNeverThrown();
  errorNeverThrownWithArgs();
  expectedErrorBadMsg();
  unexpectedError();
  unexpectedUnmanagedClassFunctorError();
  unexpectedSharedClassFunctorError();
}

/*
 * Helper classses and functions
*/

class CustomError : Error {
  proc init() { super.init(); }
  proc init(msg:string) { super.init(msg); }
}

proc throwCustom() throws {
  throw new owned CustomError("Threw a custom error");
}
proc throwCustomWithArgs(const customMsg:string) throws {
  // changing proc name b/c cannot capture overloaded routine
  throw new owned CustomError(customMsg);
}

proc noThrow(): int { return 0; }
proc noThrowWithArgs(in a:real, in b:int): real { return a + b; }

record myRecordThrower {
  var state: int;
  proc this(in val:int) throws {
    if state != val then throw new owned CustomError();
  }
}

class MyClassThrower {
  var state: int;
  proc this(in val:int) throws {
    if state != val then throw new owned CustomError("watch out");
  }
}

/*
 * Test functions
*/

proc expectedCustomError() {
  var test = new Test();
  try! test.assertThrows(throwCustom, CustomError);
  try! test.assertThrows(throwCustom, CustomError,
                         match="Threw a custom error");
}
proc expectedCustomErrorAnonymous() {
  // ensure method works for anonymous procedures
  var test = new Test();
  const foo = proc(): void throws { throw new owned CustomError(); };
  try! test.assertThrows(foo, CustomError);
}

proc expectedErrorWrappedMethod() {
  // ensure method works for throwing class method, but they must be wrapped
  var test = new Test();
  class CustomClass {
    proc foo() throws {
        throw new owned CustomError();
    }
  }
  proc bar() throws {
    var cat = new owned CustomClass();
    cat.foo();
  }
  try! test.assertThrows(bar, CustomError);
}

proc expectedErrorRecordFunctor() {
  var test = new Test();
  var thrower = new myRecordThrower(5);
  try! test.assertThrows(thrower, CustomError, (4,));
}

proc expectedErrorClassFunctor() {
  var test = new Test();
  var thrower = new owned MyClassThrower(5);
  try! test.assertThrows(thrower, CustomError, (4,));
}

proc expectedCustomErrorWithArgs() {
  var test = new Test();
  try! test.assertThrows(throwCustomWithArgs, CustomError,
                         ("user-spec error msg",));
  try! test.assertThrows(throwCustomWithArgs, CustomError,
                         ("user-spec error msg",), match="user-spec error msg");
}

proc expectedSuperError() {
  // because Error is the super-class of CustomError, this is acceptable
  var test = new Test();
  try! test.assertThrows(throwCustom, Error);
  try! test.assertThrows(throwCustom, Error, match="Threw a custom error");
}

proc errorNeverThrown() throws {
  param thisProcName = getRoutineName();
  testAssert(noThrow, CustomError, reason=thisProcName);
}

proc errorNeverThrownWithArgs() throws {
  param thisProcName = getRoutineName();
  testAssert(noThrowWithArgs, CustomError, (5.1, 6), reason=thisProcName);
}

proc expectedErrorBadMsg() throws {
  param thisProcName = getRoutineName();
  testAssert(throwCustom, CustomError, match="incorrect message",
             reason=thisProcName);
}

proc unexpectedError() throws {
  param thisProcName = getRoutineName();
  testAssert(throwCustom, NilThrownError, reason=thisProcName);
}

proc unexpectedUnmanagedClassFunctorError() {
  param thisProcName = getRoutineName();
  var test = new Test();
  var thrower = new unmanaged MyClassThrower(5);
  testAssert(thrower, NilThrownError, (4,), reason=thisProcName);
  delete thrower;
}

proc unexpectedSharedClassFunctorError() {
  param thisProcName = getRoutineName();
  var test = new Test();
  var thrower = new shared MyClassThrower(5);
  testAssert(thrower, NilThrownError, (4,), reason=thisProcName);
}
