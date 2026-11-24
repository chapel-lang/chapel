use CTypes;
use newSubClassCommon;

class C: B {
  override proc getX() do return x + 3;
}

// Resolve B and C in the library so getX() dispatches virtually.
new B();
new C();

export proc callGetX(ptr: c_ptr(void)) {
  var baseClass = (ptr : unmanaged A?)!;
  return baseClass.getX();
}
