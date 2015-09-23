proc type int.foo() {
  writeln("In foo()");
}

record R {
  var x: int;
}

var myR: R;

int.foo();
myR.x.type.foo();
R.foo();
