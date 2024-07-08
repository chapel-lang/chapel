class FieldClass {
  var x: int;
}

record storesField {
  var f: FieldClass;

  proc init(in field: FieldClass) {
    f = field;
    writeln("In the explicit initializer");
  }

  proc deinit() {
    delete f;
  }
}

proc main() {
  var fc1 = new unmanaged FieldClass(3);
  writeln(fc1);

  var sf1 = new storesField(fc1);
  writeln(sf1);
}
