record Foo {
  type t;
  var x: t;

  operator ==(const ref lhs: Foo(?t), const ref rhs: Foo(t)) {
    if (lhs.t != rhs.t) {
      return false;
    } else {
      return lhs.x == rhs.x;
    }
  }
}

proc main() {
  var r1: Foo(int);
  var r2 = new Foo(int, 1);
  var r3 = new Foo(int, 0);
  writeln(r1 == r2);
  writeln(r1 == r3);
}
