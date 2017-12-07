pragma "use default init"
record LotsOFields {
  var f1: int;
  var f2: real = 14.3;
}

proc main() {
  var c: LotsOFields = new LotsOFields(2, 6.3);

  writeln(c);
}
