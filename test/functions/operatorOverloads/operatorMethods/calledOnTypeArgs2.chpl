record Foo {
  var x: int;

  proc myMethod(x, y) {
    if (x.type != y.type) {
      writeln("They don't match");
    } else {
      writeln("They do");
    }
  }

  operator !=(lhs: Foo, rhs: Foo) {
    return lhs.x != rhs.x;
  }
}

proc main() {
  var foo: Foo;

  foo.myMethod(2, 3);
  foo.myMethod(2, false);
}
