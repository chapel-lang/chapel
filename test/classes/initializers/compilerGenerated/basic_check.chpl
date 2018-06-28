class LotsOFields {
  var f1: int;
  var f2: real = 14.3;
  const f3 = false;
}

proc main() {
  var c: unmanaged LotsOFields = new unmanaged LotsOFields(2, 6.3, true);

  writeln(c);
  delete c;
}
