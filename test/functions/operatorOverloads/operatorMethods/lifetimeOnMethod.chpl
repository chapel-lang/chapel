record Foo {
  var x: int;

  operator <(lhs: Foo, rhs: Foo) {
    return lhs.x < rhs.x;
  }

  proc otherMethod(arg1: int) lifetime this < arg1 {
    x += arg1;
  }
}

proc main() {
  var foo: Foo;
  writeln(foo);
  var myVal = 3;
  foo.otherMethod(myVal);
  writeln(foo);
  var foo2: Foo;
  writeln(foo < foo2);
}
