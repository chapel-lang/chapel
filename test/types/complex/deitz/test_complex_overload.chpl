def foo(x : real) {
  writeln("It's a real!");
}

def foo(x : complex) {
  writeln("It's a complex!");
}

var y : real;

writeln(y);
foo(y);

var z : complex;

writeln(z);
foo(z);
