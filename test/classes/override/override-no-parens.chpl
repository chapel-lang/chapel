class Parent {
  proc foo { }
}

class Child : Parent {
  override proc foo { }
}

proc main() {
  var x = (new owned Child()).borrow();
  x.foo;
}
