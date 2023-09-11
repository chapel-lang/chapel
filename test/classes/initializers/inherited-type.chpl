class Derived : Base(?) {
  var  z : t;

  proc init(_x : int, _y : int, _z : int) {
    super.init(_x, _y);

    z = _z;

    init this;
  }
}


class Base {
  type t;

  var  x : t;
  var  y : t;

  proc init(_x : int, _y : int) {
    t = int;

    x = _x;
    y = _y;

    init this;
  }
}


proc main() {
  var ownC = new owned Derived(10, 20, 30);
  var c = ownC.borrow();

  writeln(c);
}
