pragma "use default init"
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
  var x = new GenericClass(new GenericRecord(1));
  var y:GenericClass = new GenericClass(new GenericRecord(1));

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  //var x:GenericClass; // no, not possible
}

test();
