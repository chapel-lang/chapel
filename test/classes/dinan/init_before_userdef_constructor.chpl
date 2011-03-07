proc f(x) {
  return x*2;
}

class C {
  var x: int = 2;
  var y: int = f(x);
  
  // This works around overriding the default constructor
  proc C(w: bool) {
    writeln("In user defined constructor: x=", x, " y=", y);
  }
}

var c: C = new C(true);

writeln("Got back: x=", c.x, " y=", c.y);

delete c;
