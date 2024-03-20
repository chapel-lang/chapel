record Rec {
  type t;
  var x: int;
}

var globalRec = new Rec(int, 15);

proc takeRec(const r: Rec(?)) {
  writeln(r);
  // This should trigger the warning (but not on this line number)
  globalRec.x = 3;
  writeln(r);
}

takeRec(globalRec);
