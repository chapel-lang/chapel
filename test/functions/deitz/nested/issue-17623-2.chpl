proc genericFunction(callable) {
  proc innerf() {
    callable.function();
  }
  innerf();
}

proc foo() {
  var fooVariable = 1;
  record R {
  }
  proc R.function() {
    writeln(fooVariable);
  }
  var r = new R();
  genericFunction(r);
}

foo();
