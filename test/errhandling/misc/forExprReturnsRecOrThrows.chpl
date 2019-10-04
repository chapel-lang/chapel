config const testForExpr = false;
config const testForAllExpr = false;
config const testBracketForAllExpr = false;

record r {
  var x: int;

  proc init() { }

  proc init(i: int) {
    this.x = i;
  }

  proc deinit() { }

}

proc foo(i, shouldThrow=false) throws {
  if shouldThrow && i == 0 then throw new Error();
  else return new r(i);
}

proc main() {
  try {
    var a1 = for i in -5..5 do foo(i, shouldThrow=testForExpr);
    var a2 = forall i in -5..5 do foo(i, shouldThrow=testForAllExpr);
    var a3 = [i in -5..5] foo(i, shouldThrow=testBracketForAllExpr);
    
    writeln(a1);
    writeln(a2);
    writeln(a3);
  }
  catch e {
    writeln("Error caught");
  }
}
