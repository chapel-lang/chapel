record S {
  var x = 42;
}

record R {
  forwarding var s: S;
}

proc R.foo() {
  writeln(x);
}

var myR: R;
myR.foo();
