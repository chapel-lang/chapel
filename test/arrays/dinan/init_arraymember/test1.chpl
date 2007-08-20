def f() {
  var x: [1..10] real = 5.0;
  return x;
}

class C {
  var x: [1..10] real = f();
}

var c = C();

writeln(c.x);
