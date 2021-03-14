class Parent {
  proc f() { }
}

class Child : Parent {
  // overrides parent f()
  proc f() { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow():borrowed Parent;
  y.f();
}
test();
