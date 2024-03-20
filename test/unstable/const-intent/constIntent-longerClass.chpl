class C {
  var x: int;
  var y: bool;
}

var globalC = new C(15);

proc takeRec(const c: C) {
  writeln(c);
  // This should trigger the warning (but not on this line number)
  globalC = new C(15, true);
  writeln(c);
}

takeRec(globalC);
