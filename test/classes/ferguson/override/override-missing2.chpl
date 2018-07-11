class Parent {
  proc f() { }
}

class Child : Parent {
  // overrides parent f()
  proc f() { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow();
  y.f();
}
test();
