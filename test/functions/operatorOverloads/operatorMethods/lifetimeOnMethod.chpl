record Foo {
  var x: int;

  operator <(lhs: Foo, rhs: Foo) {
    return lhs.x < rhs.x;
  }

  proc otherMethod(arg1: Other) lifetime this < arg1 {
    x += arg1.y;
  }
}

class Other {
  var y: int;
}

proc main() {
  var foo: Foo;
  writeln(foo);
  var other = new Other(3);
  foo.otherMethod(other.borrow());
  writeln(foo);
  var foo2: Foo;
  writeln(foo < foo2);
}
