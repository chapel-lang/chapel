record Rec {
  var x: int;
}

var globalRec = new Rec(15);

// This should not trigger the warning, an explicit `const ref` implies that
// the user knows the value could change
proc takeRec(const ref r: Rec) {
  writeln(r);
  globalRec.x = 3;
  writeln(r);
}

takeRec(globalRec);
