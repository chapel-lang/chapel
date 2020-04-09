record GenericRecord {
  var field;
}

class GenericClass {
  var f:GenericRecord;
  proc init(arg:GenericRecord) {
    this.f = arg;
  }
}


proc test() {
  var x = new borrowed GenericClass(new GenericRecord(1));
  var y:borrowed GenericClass = new borrowed GenericClass(new GenericRecord(1));
  var z:borrowed GenericClass(GenericRecord(int)) = new borrowed GenericClass(new GenericRecord(1));

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);
  //var x:GenericClass; // no, not possible
}

test();
