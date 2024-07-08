class Parent {
  proc g(arg:int(32)) { }
}

class Child : Parent {
  // not an override -- types differ
  override proc g(arg:int(16)) { }
  override proc g(arg:int(64)) { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow();
  var one: int = 1;
  y.g(one:int(32));
}
test();
