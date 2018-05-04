pragma "use default init"
record GenericRecord {
  var field;
}

class GenericClass {
  // this case happens somewhere else
  //  default initializer is handled during build default functions
  //   after scope resolve after normalize but before resolve
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
