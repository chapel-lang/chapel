use Print;

class C {
  var x : int;
  var y : real;

  proc init(_x: int = 0, _y: real = 0.0) {
    x = _x;
    y = _y;
  }
}

class Parent {
  var x : int;
}

class Child : Parent {
  var y : real;

  proc init(_x: int = 0, _y: real = 0.0) {
    super.init(_x);
    y = _y;
  }
}

proc main() {
  var c = new unmanaged C(10, 42.0);

  println(c.x);
  println(c.y);

  var z = new unmanaged Child(3, 9.0);
  println(z.x);
  println(z.y);
}
