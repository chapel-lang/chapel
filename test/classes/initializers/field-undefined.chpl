class Foo {
  var a : int;
  var b : int;
  var t : int;

  proc init(_a : int, _b : int, _c : int) {
    a = _a;
    b = _b;
    c = _c;

  }
}

proc main() {
  var c = new unmanaged Foo(10, 20, 3);

  writeln(c);

  delete c;
}
