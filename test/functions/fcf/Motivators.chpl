/***
  This test contains tests for function types as a language feature.
  It should be at parity with '$CHPL_HOME/test/functions/lambda',
  which contains tests for legacy stuff.

  These tests assume a non-pointer-based model.
*/
module Motivators {
  var globalCounter = 0;

  proc testSimpleProcTypeEquivalence() {
    type T1 = proc(x: int, y: int): int;
    type T2 = proc(x: int, y: int): int;
    assert(T1 == T2);
  }

  // TODO: 'myAdd' -> 'add' = 'cannot capture overloaded procedure'
  proc testProcTypeFromNamedProcedure() {
    proc myAdd(x: int, y: int): int { return x + y; }
    type T1 = myAdd.type;
    type T2 = proc(x: int, y: int): int;
    assert(T1 == T2);
  }

  proc testEnsureProcTypeIsNotGeneric() {
    type T1 = proc(): void;
    assert(!isGenericType(T1));
  }

  proc testPassAndCallAnonProc1() {
    const f = proc(x: int, y: int): int { return x + y; };
    proc call(fn) { assert(fn(4, 4) == 8); }
    call(f);
  }

  proc testPassAndCallAnonProc2() {
    const f = proc(): void { globalCounter = 8; };
    globalCounter = 0;
    proc call(fn): void { f(); }
    call(f);
    assert(globalCounter == 8);
    globalCounter = 0;
  }

  proc main() {
    testSimpleProcTypeEquivalence();
    testProcTypeFromNamedProcedure();
    testEnsureProcTypeIsNotGeneric();
    testPassAndCallAnonProc1();
    testPassAndCallAnonProc2();
  }
}
