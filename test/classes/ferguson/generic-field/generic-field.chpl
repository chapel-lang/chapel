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
  var ownX = new owned GenericClass(new GenericRecord(1));
  var x = ownX.borrow();
  var ownY = new owned GenericClass(new GenericRecord(1));
  var y:borrowed GenericClass = ownY.borrow();
  var ownZ = new owned GenericClass(new GenericRecord(1));
  var z:borrowed GenericClass(GenericRecord(int)) = ownZ.borrow();

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);
  //var x:GenericClass; // no, not possible
}

test();
