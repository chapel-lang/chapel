class Parent {
  proc f() { }
  proc g() where false { }
}

class Child : Parent {
  // overrides parent f()
  override proc f() where false { }
  // doesn't override parent b/c of parent where clause
  override proc g() { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow():borrowed Parent;
  y.f();
  x.g();
}
test();
