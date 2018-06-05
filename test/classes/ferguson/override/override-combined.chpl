class Parent {
  proc f() { }

  proc f_generic(x) { }
}

class Child : Parent {
  // not an override, no parent method with that name
  override proc g() { }

  // overrides no-args parent f
  proc f() { }

  // not an override, no parent method with that name 
  override proc g_generic(x) { }

  // overrides generic parent f_generic
  proc f_generic(x) { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow();
  y.f();
  y.g();
  y.g_generic(1);
  y.f_generic(1);
}
test();
