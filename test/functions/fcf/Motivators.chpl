//
// Passing tests for routines-as-values as a language feature. Should be
// at parity with 'test/functions/lambda', and include tests for new
// features as well.
//

// Globals used by tests.
var globalCounter = 0;
var A: [0..0] int;

/**
  Make sure that anonymous procedures are properly looked up in a generic
  context (note the 3 different instantiations of 'foo'). This fixed
  a bug in the old compiler where we would not recompute visible procs
  after performing an instantiation.
*/
proc test0() {
  proc foo(x) {
    writeln(x);
    proc bar(n : int) { return n; }
    return bar;
  }

  const a = foo(8);
  writeln(a);
  const b = foo(3.1415);
  writeln(b);
  const c = foo('Hello!');
  writeln(c);
}

/**
  Should be able to make a pointer to a non-capturing procedure in a method.
*/
proc test1() {
  use CTypes;

  class C {
    proc foo() {
      proc helper(): real { return 0; }
      var gf = c_ptrTo(helper);
      writeln(gf.type:string);
    }
  }
  var c = new C();
  c.foo();
}

/**
  Capture and call a proc that returns by const ref.
*/
proc test2a() {
  proc foo(x: int) const ref { return A[x]; }
  const p1 = foo;
  const p2 = proc(x: int) const ref { return A[x]; }; 
  proc bar(p, n: int) {
    const ref x = p(0);
    A = n;
    writeln(x);   // Update should be visible.
  }
  bar(foo, 2);
  bar(p1, 4);
  bar(p2, 8);
}

/**
  Capture and call a proc that returns by ref.
*/
proc test2b() {
  proc foo(x: int) ref { return A[x]; }
  const p1 = foo;
  const p2 = proc(x: int) ref { return A[x]; };
  proc bar(p, n: int) {
    ref x = p(0);
    x = n;
    writeln(A);   // Update should be visible.
  }
  bar(foo, 2);
  bar(p1, 4);
  bar(p2, 8);
}

/**
  Parenless proc and integer should not have a procedure type. Anonymous
  proc and named proc should have the same type.
*/
proc test3() {
  proc f1() { return 1; }
  proc f2 { return 2; }
  var f3 = 3;
  var f4 = proc() { return 4; };
  writeln(f1.type:string);
  writeln(f2.type:string);
  writeln(f3.type:string);
  writeln(f4.type:string);
  assert(f1.type != f2.type);
  assert(f2.type == f3.type);
  assert(f1.type == f4.type);
}

/**
  Procedures-as-values should be able to throw, both named and anonymous.
*/
proc test4() {
  proc foo() throws { throw new Error('P1'); }
  var p1 = foo;
  var p2 = proc() throws { throw new Error('P2'); };
  try { p1(); } catch e { writeln(e); }
  try { p2(); } catch e { writeln(e); }
}

/**
  Should be able to print out named and anonymous procedure names.
*/
proc test5() {
  proc foo() {}
  const p1 = foo;
  const p2 = proc() { return 0; };
  writeln(foo:string);
  writeln(p1:string);
  writeln(p2:string);
}

/**
  These two procedure types should be identical.
*/
proc test6() {
  type T1 = proc(x: int, y: int): int;
  type T2 = proc(x: int, y: int): int;
  assert(T1 == T2);
}

/**
  Type of named procedure and procedure type should be identical.
*/
proc test7() {
  proc myAdd(x: int, y: int): int { return x + y; }
  type T1 = myAdd.type;
  type T2 = proc(x: int, y: int): int;
  assert(T1 == T2);
}

/**
  Procedure type should not be generic.
*/
proc test8() {
  type T1 = proc(): void;
  type T2 = proc();
  assert(!isGenericType(T1));
  assert(!isGenericType(T2));
}

/**
  Call procedure stored as record field.
*/
proc test9() {
  record r {
    var p: proc(_: int, _: int): int;
    proc call(x: int) do return p(x, x);
  }
  const p = proc(x: int, y: int) { return x + y; };
  var x = new r(p);
  var n = x.call(4);
  writeln(n);
}

proc main() {
  const tests = [test0, test1, test2a, test2b, test3, test4, test5, test6,
                 test7, test8, test9];
  type T = proc(): void;
  for test in tests {
    assert(test.type == T);
    writeln("--- ", test:string, " ---");
    test();
  }
}

