class Parent {
  proc f(arg:int) { }
  proc g(arg:int(32)) { }
  proc h(x) { }
}

class Child : Parent {
  override proc f(x:int) { }
  override proc g(arg:int(32)) { }
  override proc h(x) { }
}
