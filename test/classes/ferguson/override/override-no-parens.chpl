class Parent {
  proc foo { }
}

class Child : Parent {
  override proc foo { }
}

proc main() {
  var x = new borrowed Child();
  x.foo;
}
