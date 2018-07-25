record GenericRecord {
  var field;
}

record Wrapper {
  var f:GenericRecord;
  proc init(arg:GenericRecord) {
    this.f = arg;
  }
}

proc test1() {
  var x = new Wrapper(new GenericRecord(1));
  var y:Wrapper = new Wrapper(new GenericRecord(1));
  var z:Wrapper(GenericRecord(int)) = new Wrapper(new GenericRecord(1));

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);
  //var x:Wrapper; // no, not possible
}

test1();
