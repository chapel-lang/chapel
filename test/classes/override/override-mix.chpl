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
  var one: int = 1;
  y.f(one);
  y.g(one:int(16));
  //y.g(one:int(32)); // see override-mix-ambiguity.chpl
  y.h(one);
  y.h("string");
}
test();
