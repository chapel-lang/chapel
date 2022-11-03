/***
*/
module Motivators {

  // Test with only default intents - should achieve status quo.
  proc testDefaultIntentProcExpr() {
    var add = proc(x: int, y: int): int { return x + y; };
    var sum = add(4, 4);
    var val = add(4, 4);
    assert(val == 8);
  }

  // Now use ref intent to prove the implementation supports intents.
  proc testRefIntentProcExpr() {
    var inc = proc(ref x: int): void { x += 1; };
    var x = 0;
    inc(x);
    assert(x == 1);
  }

  // Test printing function types, one with named formals and one without.
  proc testPrintingFunctionTypes() {
    type F1 = proc(_: int, _: int): int;
    assert(F1:string == "proc(int, int): int");

    type F2 = proc(ref _: int, ref _: int): int;
    assert(F2:string == "proc(ref: int, ref: int): int");

    type F3 = proc(const x: real, in y: complex): void;
    assert(F3:string == "proc(x: real, in y: complex)");

    // Check to make sure printing temporaries also works as expected.
    assert(proc(ref x: real): int:string == "proc(ref x: real): int");
  }

  proc testFunctionTypeEqualityConsideringFormalNames() {
    type F1 = proc(_: int, _: int): int;
    type F2 = proc(x: int, _: int): int;
    assert(F1 != F2);
  }

  proc testThrowingFunctionExpr() {
    const f1 = proc(n: int) throws { return n*n; };
    const f2 = proc(n: int) throws { throw new Error(n:string); return n; };
    const n = 4;
    try {
      assert(f1(n) == n*n);
      assert(f2(n) == n);
    } catch e : Error {
      assert(e.message() == n:string);
    }
  }

  /*
  proc test() {
    type F = proc(int, int): int;
    var a: F; // Default value should be 'nil'?
    assert(a == nil);
    var b: F = proc(x: int, y: int): int { return x + y; };
    var c = b;
    assert(b.type == c.type);
  }
  */

  /*
  // Should be a typing error.
  proc test() {
    type F = proc(int, int): int;
    var a: F = proc(x: int, y: real) { return x + y; };
  }
  */

  /*
  // Print a function type.
  proc test() {
    type F = proc(int, int): int;
    writeln(F:string);
    var a: F;
    writeln(a.type:string);
  }
  */

  /*
  // Parsing error - procedure expressions cannot have unnamed formals.
  proc test() {
    var fn = proc(int, y: int, ref: int): void;
  }
  */

  proc main() {
    testDefaultIntentProcExpr();
    testRefIntentProcExpr();
    testPrintingFunctionTypes();
    testFunctionTypeEqualityConsideringFormalNames();
    testThrowingFunctionExpr();
  }
}

