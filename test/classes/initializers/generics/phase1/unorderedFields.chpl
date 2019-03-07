class LotsOFields {
  var f1: int;
  param f2: real = 14.3;
  param f3 = false;

  proc init(val1, param val2, param val3) {
    f1 = val1;
    f3 = val3;
    f2 = val2; // uh oh!
  }
}

proc main() {
  var c: owned LotsOFields = new owned LotsOFields(2, 6.3, true);

  writeln(c);
}
