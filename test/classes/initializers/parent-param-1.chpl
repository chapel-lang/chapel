class Derived : Base {
  var y : int = x + 1;

  proc init(param _x) {
    super.init(_x);

    initDone();
  }
}


class Base {
  param x;

  proc init(param _x) {
    x = _x;

    initDone();
  }
}


proc main() {
  var c = new Derived(20);

  writeln(c);

  delete c;
}