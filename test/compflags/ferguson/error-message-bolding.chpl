proc h(arg) {
  arg = 11;
}

proc g(arg, otherArg:int, param p, param pp, type t) {
  h(arg);
}

record ConcreteRecord {
  var concreteField: int;
  proc methodA(arg) {
    g(arg, 1, 2, 3:int(8), int);
  }
}

record GenericRecord {
  var genericField;
  proc methodB(arg) {
    var r = new ConcreteRecord(1);
    r.methodA(arg);
  }
}

proc f(arg) {
  var r = new GenericRecord(1.0);
  r.methodB(arg);
}

f("hi");
