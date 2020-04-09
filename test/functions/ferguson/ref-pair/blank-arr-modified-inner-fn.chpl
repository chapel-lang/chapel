var globalArray : [1..4] int;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}

proc set1(ref arg) {
  arg[1] = 1;
}

proc innerFnSets(arg) {

  proc helpSet() {
    set1(arg);
  }

  helpSet();
}

proc get1(const ref arg) {
  writeln(arg[1]);
}


proc innerFnGets(arg) {

  proc helpGet() {
    get1(arg);
  }

  helpGet();
}

writeln("innerFnSets: ");
innerFnSets(getter(globalArray));

writeln("innerFnGets: ");
innerFnGets(getter(globalArray));
