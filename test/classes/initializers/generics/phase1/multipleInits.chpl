class LotsOFields {
  var f1: int;
  param f2: real = 14.3;
  const f3 = false;

  proc init(val1, param val2, val3) {
    f1 = val1;
    f2 = val2;
    f2 = val2*3; // uh oh!
    f3 = val3;
  }
}

proc main() {
  var c = new borrowed LotsOFields(2, 6.3, true);

  writeln(c.type: string);
  writeln(c);
}
