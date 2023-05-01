class A { }
class B { }

config param testShared = false;

proc makeA() {
  if testShared {
    return new shared A();
  } else {
    return new owned A();
  }
}
proc main() {
  var x = makeA();
  if testShared {
    x = shared.adopt(new unmanaged B());
  }
  else {
    x = owned.adopt(new unmanaged B());
  }
}
