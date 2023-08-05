class S {
  var x = 42;
}

class R {
  forwarding var s: S;
}

proc R.foo() {
  writeln(x);
}

var myR: R;
myR.foo();
