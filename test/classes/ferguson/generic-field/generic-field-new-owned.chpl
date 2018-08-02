record GenericRecord {
  var field;
}

class GenericClass {
  var f:GenericRecord;
  proc init(arg:GenericRecord) {
    this.f = arg;
  }
}

proc test1() {
  var x = new owned GenericClass(new GenericRecord(1));
  var y:owned = new owned GenericClass(new GenericRecord(1));
  var z:owned GenericClass(GenericRecord(int)) = new owned GenericClass(new GenericRecord(1));

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);
  //var x:GenericClass; // no, not possible
}

test1();
