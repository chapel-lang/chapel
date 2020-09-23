proc f() {
  var x: [1..10] real = 5.0;
  return x;
}

class C {
  var x: [1..10] real = f();
}

var c = new unmanaged C();

writeln(c.x);

delete c;
