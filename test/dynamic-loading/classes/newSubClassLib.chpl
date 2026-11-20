use CTypes;
use newSubClassCommon;

// Resolve B in the library so getX() dispatches virtually.
new B();

export proc callGetX(ptr: c_ptr(void)) {
  var baseClass = (ptr : unmanaged A?)!;
  return baseClass.getX();
}
