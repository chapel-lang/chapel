class Parent {
  proc f() { }
  proc g() where false { }
}

class Child : Parent {
  // overrides parent f()
  proc f() where false { }
  // doesn't override parent b/c of parent where clause
  proc g() { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow():borrowed Parent;
  y.f();
  x.g();
}
test();
