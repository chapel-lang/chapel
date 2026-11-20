use myClasses;
use CTypes;

export proc makeC(): c_ptr(void) {
  var c = new unmanaged C();
  c.values = 10;
  return c_ptrTo(c);
}

export proc checkAndDeleteC(ptr: c_ptr(void)) {
  const c = (ptr: unmanaged C?)!;
  for i in c.values.domain do
    assert(c.values[i] == 11 + (i-1)/2);
  delete c;
}
