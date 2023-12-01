record Rec {
  var x: int;
  var y: bool;
}

var globalRec = new Rec(15);

proc takeRec(const r: Rec) {
  writeln(r);
  // This should trigger the warning (but not on this line number)
  globalRec.y = true;
  writeln(r);
}

on Locales[1] {
  takeRec(globalRec);
}
