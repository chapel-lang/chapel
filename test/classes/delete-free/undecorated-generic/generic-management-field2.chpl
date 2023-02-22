// I think this code should always work without error, or we've done something
// wrong
class FieldClass {
  var x: int;
}

record storesField {
  var f: FieldClass;

  proc init(in field: FieldClass) {
    f = field;
    writeln("In the explicit initializer");
  }
}

proc main() {
  var fc1 = new FieldClass(3);
  writeln(fc1);

  var sf1 = new storesField(fc1);
  writeln(sf1);
}
