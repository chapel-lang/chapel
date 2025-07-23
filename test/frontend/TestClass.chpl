use Print;

class C {
  var x : int;
  var y : real;

  proc init(_x: int = 0, _y: real = 42.0) {
    x = _x;
    y = _y;
  }
}

proc main() {
  var c = new unmanaged C(10, 42.0);

  println(c.x);
  println(c.y);
}
