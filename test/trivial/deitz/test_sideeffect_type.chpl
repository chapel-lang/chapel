def foo() {
  writeln("side effect");
  return 2;
}

var x: foo().type;

writeln(x);
