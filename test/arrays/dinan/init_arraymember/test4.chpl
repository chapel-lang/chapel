proc f(d) {
  var x: [d] real = 5.0;
  return x;
}

class C {
  var d = {1..10};
  var x: [d] real = f(d);
}

var c = new unmanaged C();

writeln(c.x);

delete c;
