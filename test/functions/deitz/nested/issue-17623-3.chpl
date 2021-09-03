proc genericFunction(callable) {
  cobegin {
    callable.function();
    callable.function();
  }
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
