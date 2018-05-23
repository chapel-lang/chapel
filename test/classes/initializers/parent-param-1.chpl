class Derived : Base {
  var y : int = x + 1;

  proc init(param _x) {
    super.init(_x);

    complete();
  }
}


class Base {
  param x;

  proc init(param _x) {
    x = _x;

    complete();
  }
}


proc main() {
  var c = new owned Derived(20);

  writeln(c);
}
