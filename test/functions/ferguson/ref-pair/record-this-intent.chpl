

record R {
  var field:int;

  proc setsField() {
    field = 1;
  }

  proc readsField() {
    writeln(field);
  }

}

var globalR = new R();

proc getter() ref {
  writeln("REF");
  return globalR;
}

proc getter() const ref {
  writeln("CONST REF");
  return globalR;
}

getter().setsField();

getter().readsField();


