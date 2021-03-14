// Validate the default initializer created for a generic record with no
// constructor or initializer defined
record GenericRecord {
  type t;
  param p = 3;
  var v;
  var nongeneric: int;
}

proc main() {
  var c1 = new GenericRecord(bool, 5, 3.0, 2);
  writeln(c1.type:string);
  writeln(c1);

  var c2: GenericRecord(int, 2, bool);
  writeln(c2.type:string);

  var c3: GenericRecord(real, 5, real) = new GenericRecord(real, 5, 2.4, 8);
  writeln(c3.type:string);
  writeln(c3);
}
