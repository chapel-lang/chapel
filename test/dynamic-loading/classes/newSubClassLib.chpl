use CTypes;
class A {
  var x:int;
  proc getX() {
    return x;
  }
}
class B : A {
  override proc getX() {
    return x + 1;
  }
}
// make sure B is resolved so A's methods are virtual
// I wish we had a virtual keyword.....
new B();

export proc callGetX(ptr: c_ptr(void)) {
  var baseClass = (ptr : unmanaged A?)!;
  // for now, this will halt
  // TODO: it gives us bad line linfo because `chpl_lookupFilename`
  // always uses the root program
  return baseClass.getX();
}
