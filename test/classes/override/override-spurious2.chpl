class Parent {
  proc f() { }
}

class Child : Parent {
  // not an override - no parent method g()
  override proc g() { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow();
}
test();
