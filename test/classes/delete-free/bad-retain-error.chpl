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
  x.retain(new unmanaged B());
}
