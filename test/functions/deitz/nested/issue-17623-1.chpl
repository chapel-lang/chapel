proc genericFunction(param p) {
  proc innerf() {
    fooFunction();
  }
  innerf();
}

proc foo() {
  var fooVariable = 1;
  proc fooFunction() {
    writeln(fooVariable);
  }
  genericFunction(1);
}

foo();
