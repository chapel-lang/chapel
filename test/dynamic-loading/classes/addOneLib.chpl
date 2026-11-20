use BlockEntry;
use CTypes;

export proc addOne(ptr: c_ptr(void)) {
  const entry = (ptr: unmanaged Entry(int)?)!;
  const result = entry.values + 1;
  var output = new owned Entry();
  output.values = result;
  for i in output.values.domain do assert(output.values[i] == i + 1);
}

proc stream(aPtr: c_ptr(void), bPtr: c_ptr(void), alpha: ?t): c_ptr(void) {
  const a = (aPtr: unmanaged Entry(t)?)!;
  const b = (bPtr: unmanaged Entry(t)?)!;
  var output = new unmanaged Entry(t);
  output.values = a.values + b.values * alpha;
  return c_ptrTo(output);
}

export proc streamInt(a: c_ptr(void), b: c_ptr(void), alpha: int): c_ptr(void) {
  return stream(a, b, alpha);
}

export proc streamReal(a: c_ptr(void), b: c_ptr(void), alpha: real): c_ptr(void) {
  return stream(a, b, alpha);
}
