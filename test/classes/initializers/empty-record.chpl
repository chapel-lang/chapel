// Test a record initializer with an empty body

record MyRecord {
  var x : int;
  var y : int = 1;
  var z       = 2;

  proc init() {

  }
}

proc main() {
  var rec : MyRecord;

  writeln('rec: ', rec);
}
