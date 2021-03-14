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
  var r2: Foo(real);
  writeln(r1 == r2);
}
