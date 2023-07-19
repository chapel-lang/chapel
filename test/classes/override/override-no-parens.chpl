class Parent {
  proc foo { }
}

class Child : Parent {
  override proc foo { }
}

proc main() {
  var ownX = new owned Child();
  var x = ownX.borrow();
  x.foo;
}
