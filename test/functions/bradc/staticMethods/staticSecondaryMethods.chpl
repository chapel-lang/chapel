record R {
  var x: real;
}

record R2 {
  var x: real;
}

proc type R.foo() {
  return 1.0;
}

proc type int.foo() {
  return 2.0;
}

proc type R2.foo() {
  return 3.0;
}

writeln(R.foo());
writeln(int.foo());
writeln(R2.foo());
