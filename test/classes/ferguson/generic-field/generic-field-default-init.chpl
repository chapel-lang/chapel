record GenericRecord {
  var field;
}

class GenericClass {
  var f:GenericRecord;
}

proc test() {
  var x = (new owned GenericClass(new GenericRecord(1))).borrow();
  var y:borrowed GenericClass = (new owned GenericClass(new GenericRecord(1))).borrow();
  var z:borrowed GenericClass(GenericRecord(int)) = (new owned GenericClass(new GenericRecord(1))).borrow();

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
  writeln(z.type:string, " ", z);
  //var x:GenericClass; // no, not possible
}

test();
