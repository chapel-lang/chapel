class LotsOFields {
  var f1: int;
  param f2: real = 14.3;
  const f3 = false;

  proc init(val1, param val2, val3) {
    f1 = val1;
    f2 = val2;
    f3 = val3;
    f2 = val2*3; // uh oh! Could error with "out of order" or "multiple init"
  }
}

proc main() {
  var c = new owned LotsOFields(2, 6.3, true);

  writeln(c.type: string);
  writeln(c);
}
