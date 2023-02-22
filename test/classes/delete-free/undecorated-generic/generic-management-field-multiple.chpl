class FieldClass {
  var x: int;

  proc deinit() {
    writeln("I've been cleaned up with x= ", x);
  }
}

record storesField {
  var f: FieldClass;
}

proc main() {
  var fc1 = new FieldClass(1); // owned
  writeln(fc1);

  var sf1 = new storesField(fc1);
  writeln(sf1);

  var fc2 = new shared FieldClass(2); // shared
  writeln(fc2);

  var sf2 = new storesField(fc2);
  writeln(sf2);

  var fc3 = new owned FieldClass(3);
  writeln(fc3);

  var sf3 = new storesField(fc3.borrow()); // borrow
  writeln(sf3);

  var fc4 = new unmanaged FieldClass(4); // unmanaged
  writeln(fc4);

  var sf4 = new storesField(fc4);
  writeln(sf4);
  defer delete fc4;
}
