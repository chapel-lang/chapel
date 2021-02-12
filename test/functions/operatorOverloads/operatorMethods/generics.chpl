record Foo {
  type t;
  var x: t;

  operator ==(const ref lhs: Foo, const ref rhs: Foo) {
    if (lhs.t != rhs.t) {
      return false;
    } else {
      return lhs.x == rhs.x;
    }
  }
}

proc main() {
  var r1: Foo(int);
  var r2: Foo(real);
  var r3 = new Foo(int, 1);
  var r4 = new Foo(int, 0);
  writeln(r1 == r2);
  writeln(r1 == r3);
  writeln(r1 == r4);
}
