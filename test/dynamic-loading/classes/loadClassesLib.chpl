use myClasses;
use CTypes;

export proc updateArray(ptr: c_ptr(void)) {
  var c = (ptr : unmanaged C?)!;
  forall value in c.values do value += here.id + 1;
}

export proc callMyClasses() {
  var c = new C();
  var b = new B();
  var a = new A();
  callAllMethods(c, b, a);
}


export proc getAndCall(c: c_ptr(void), b: c_ptr(void), a: c_ptr(void)) {
  var cObj = (c : unmanaged C?)!;
  var bObj = (b : unmanaged B?)!;
  var aObj = (a : unmanaged A?)!;
  callAllMethods(cObj, bObj, aObj);
}

export proc getAndDyncCast(ptr: c_ptr(void)) {
  var baseClass = (ptr : unmanaged C?);
  doDynamicCasts(baseClass);
}

export proc getC(): c_ptr(void) {
  var cObj = new unmanaged C();
  return c_ptrTo(cObj);
}
export proc getB(): c_ptr(void) {
  var bObj = new unmanaged B();
  return c_ptrTo(bObj);
}
export proc getA(): c_ptr(void) {
  var aObj = new unmanaged A();
  return c_ptrTo(aObj);
}


