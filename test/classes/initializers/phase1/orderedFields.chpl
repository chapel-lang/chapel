class LotsOFields {
  var f1: int;
  var f2: real = 14.3;
  const f3 = false;

  proc init(val1, val2, val3) {
    f1 = val1;
    f2 = val2;
    f3 = val3;
    super.init();
  }
}

proc main() {
  var c: LotsOFields = new LotsOFields(2, 6.3, true);

  writeln(c);
  delete c;
}
