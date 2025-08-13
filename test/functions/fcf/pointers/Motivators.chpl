module Motivators {
  use Types;

  // Tests were written progressively with a pointer implementation in mind.
  compilerAssert(useProcedurePointers);

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

  proc testEnsureProcTypeIsNotClass() {
    type T1 = proc(): void;
    assert(!isClassType(T1));
  }

  proc testEnsureProcTypeIsNotGeneric() {
    type T1 = proc(): void;
    assert(!isGenericType(T1));
  }

  proc testNamedAndAnonProcEquivalentTypes() {
    proc myAdd(x: int, y: int): int { return x + y; }
    type T1 = myAdd.type;
    type T2 = proc(x: int, y: int): int;
    assert(T1 == T2);
  }

  proc testPassAndCallAnonProc1() {
    const f = proc(x: int, y: int): int { return x + y; };
    proc call(fn) { assert(fn(4, 4) == 8); }
    call(f);
  }

  proc testPassAndCallAnonProc2() {
    const p = proc(): void { globalCounter = 8; };
    globalCounter = 0;
    proc call(fn): void { fn(); }
    call(p);
    assert(globalCounter == 8);
    globalCounter = 0;
  }

  proc testNestedProcInGenericInstantiation() {
    proc foo(x) {
      proc bar(n : int) { return n; }
      return bar;
    }

    const a = foo(8);
    const b = foo(3.1415);
    const c = foo('Hello!');

    // Three different instantiations should have 3 different addresses?
    assert(a != b && b != c);
  }

  proc testPtrToNestedProcInMethod() {
    use CTypes;

    class C {
      proc foo() {
        proc helper(): real { return 0; }
        var gf = c_ptrTo(helper);
      }
    }
    var c = new C();
    c.foo();
  }

  proc testCallProcStoredInRecordField() {
    record r {
      var p: proc(_: int, _: int): int;
      proc call(x: int) do return p(x, x);
    }
    const p = proc(x: int, y: int) { return x + y; };
    var x = new r(p);
    var n = x.call(4);
    assert(n == 8);
  }

  proc testCallProcReturningByConstRef() {
    proc foo() const ref { return globalCounter; }
    const p1 = foo;
    const p2 = proc() const ref { return globalCounter; };

    proc bar(p, n: int) {
      const ref x = p();
      globalCounter = 0;
      assert(x == 0);
      globalCounter = n;
      assert(x == n);
    }

    bar(foo, 2);
    bar(p1, 4);
    bar(p2, 8);
  }

  proc testCallProcPassingAndReturningByRef() {
    proc foo(ref counter: int, const n: int) ref {
      counter = n;
      return globalCounter;
    }

    const p1 = foo;

    proc bar(p, n: int) {
      var counter = 0;
      ref x = p(counter, n);
      x = here.id;
      assert(counter == n);
      assert(globalCounter == here.id);
    }

    bar(p1, 4);
    bar(p1, 8);
  }

  proc testParenlessProcNotProcType() {
    proc f2 { return 2; }
    assert(!isProcedure(f2.type));
    assert(f2.type == int);
  }

  proc testProcValueThatThrows1() {
    proc foo() throws { throw new Error('P1'); }
    var p1 = foo;
    var p2 = proc() throws { throw new Error('P2'); };
    try { p1(); } catch e { assert(e.message() == 'P1'); }
    try { p2(); } catch e { assert(e.message() == 'P2'); }
  }

  proc testProcValueThatThrows2() {
    proc foo() throws { throw new Error('P1'); }
    record r {
      var p1 = foo;
      proc method1() throws { p1(); }
      proc method2() { try { p1(); } catch e { assert(e.message() == 'P1'); } }
    }
    var x = new r();
    try { x.p1(); } catch e { assert(e.message() == 'P1'); }
    try { x.method1(); } catch e { assert(e.message() == 'P1'); }
    x.method2();
  }

  proc testProcCallAsActualParsing1() {
    proc foo(x) { assert(x == 6); }
    const p = proc() const { return 6; };
    foo(p());
  }

  proc testProcCallAsActualParsing2() {
    proc foo(x) { assert(x == 8); }
    foo((proc() const { return 8; })());
  }

  proc testAnonProcAsForwardingCrazyProgram() {
    proc int.method() { globalCounter = 4; }
    proc string.method() { globalCounter = 8; }
    record r {
      var x = "8";
      forwarding((proc() {
        record inner {
          var x = 8;
          proc foo() { return x; }
        }
        var x: inner;
        return x.foo();
      })());
    }
    var x: r;
    globalCounter = 0;
    x.method();
    assert(globalCounter == 4);
  }

  proc testCallProcReturningProc() {
    proc makeProc(): proc(): proc(): int {
      return proc() { return proc() { return 8; }; };
    }
    var g = makeProc();
    assert(isProcedure(g));
    var h = g();
    assert(isProcedure(h));
    var x = h();
    assert(x == 8);
  }

  // Case 1: Procedure pointer returning ref should be widened.
  proc testWideRefPatterns0() {
    globalCounter = 0;
    const p = proc() const ref { return globalCounter; };
    const ref x = p();
    assert(globalCounter == x && x == 0);
    globalCounter = 8;
    assert(x == 8);
  }

  proc main() {
    use ChplConfig;

    testSimpleProcTypeEquivalence();
    testProcTypeFromNamedProcedure();
    testEnsureProcTypeIsNotClass();
    testEnsureProcTypeIsNotGeneric();
    testNamedAndAnonProcEquivalentTypes();
    testPassAndCallAnonProc1();
    testPassAndCallAnonProc2();
    testNestedProcInGenericInstantiation();
    testPtrToNestedProcInMethod();
    testCallProcStoredInRecordField();
    testCallProcReturningByConstRef();

    // TODO: Move stuff out of this branch as stuff passes with IWR.
    if compiledForSingleLocale() {
      testCallProcPassingAndReturningByRef();
      testParenlessProcNotProcType();
      testProcValueThatThrows1();
      testProcValueThatThrows2();
      testProcCallAsActualParsing1();
      testProcCallAsActualParsing2();
      testAnonProcAsForwardingCrazyProgram();
      testCallProcReturningProc();
    }

    testWideRefPatterns0();
  }
}
