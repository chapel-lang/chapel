class Derived : Base(?) {
  var y : int = x + 1;

  proc init(param _x) {
    super.init(_x);

    init this;
  }
}


class Base {
  param x;

  proc init(param _x) {
    x = _x;

    init this;
  }
}


proc main() {
  var c = new owned Derived(20);

  writeln(c);
}
