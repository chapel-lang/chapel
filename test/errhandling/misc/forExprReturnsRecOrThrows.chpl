module M {
  config const testForExpr = false;

  /*
   forall and bracketed versions are blocked by other bugs. See below.

  config const testForAllExpr = false;
  config const testBracketForAllExpr = false;
   */

  var initCounter = 0;

  record r {
    var x: int;

    proc init() { initCounter += 1; }
    proc init=(other: r) { initCounter += 1; this.complete(); this.x = other.x;}
    proc init(i: int) { initCounter += 1; this.x = i; }

    proc deinit() { initCounter -= 1; }

  }

  proc foo(i, shouldThrow=false) throws {
    if shouldThrow && i == 0 then throw new Error();
    else return new r(i);
  }

  proc main() {
    try {
      var a1 = for i in -5..5 do foo(i, shouldThrow=testForExpr);
      writeln(a1);

      /*

      This should be tested for leaks. But blocked by #14268

      var a2 = forall i in -5..5 do foo(i, shouldThrow=testForAllExpr);
      writeln(a2);
       */

      /* 

      This should be tested for leaks. But blocked by #14233

      var a3 = [i in -5..5] foo(i, shouldThrow=testBracketForAllExpr);
      writeln(a3);
      */
    }
    catch e {
      writeln("Error caught");
    }

    if initCounter == 0 {
      writeln("Success");
    }
    else {
      if initCounter > 0 {
        writeln("Init called more than deinit. Possible leak");
      }
      else {
        writeln("Deinit called more than init. Possible double-free");

      }
    }
  }
}
