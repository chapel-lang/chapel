record Rec {
  var x: int;
}

var globalRec = new Rec(15);

proc takeRec(r: Rec) {
  writeln(r);
  globalRec.x = 3;
  writeln(r);
}

takeRec(globalRec);
