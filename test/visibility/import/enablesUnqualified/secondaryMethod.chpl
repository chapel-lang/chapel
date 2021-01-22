class Foo {
  var field = 14;

  proc innerMethod(x: int) {
    return field * x;
  }
}

proc Foo.outerMethod(y: int) {
  return field - y;
}

proc other() {
  writeln("I don't involve Foo at all!");
}

proc returnAFoo(x: int) {
  return new owned Foo(x);
}
