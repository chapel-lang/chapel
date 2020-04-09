proc f() {
  var x: [1..10] real = 5.0;
  return x;
}

class C {
  var d = {1..10};
  var x: [d] real;
  
  proc init() {
    x = f();
  }
}

var c = new unmanaged C();

writeln(c.x);

delete c;
