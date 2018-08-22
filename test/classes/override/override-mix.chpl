class Parent {
  proc f(arg:int) { }
  proc g(arg:int(32)) { }
  proc h(x) { }
}

class Child : Parent {
  // not an override -- argument names differ
  override proc f(x:int) { }
  // not an override -- types differ
  override proc g(arg:int(16)) { }
  override proc g(arg:int(64)) { }
  // is a specific override of generic method
  proc h(x:int) { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow();
  y.f(1);
  y.g(1:int(16));
  y.g(1:int(32));
  y.h(1);
  y.h("string");
}
test();
