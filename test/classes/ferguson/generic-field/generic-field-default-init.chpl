pragma "use default init"
record GenericRecord {
  var field;
}

pragma "use default init"
class GenericClass {
  var f:GenericRecord;
}

proc test() {
  var x = new GenericClass(new GenericRecord(1));
  var y:GenericClass = new GenericClass(new GenericRecord(1));
  var z:GenericClass(GenericRecord(int)) = new GenericClass(new GenericRecord(1));

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);
  //var x:GenericClass; // no, not possible
}

test();
