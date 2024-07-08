use CTypes;


config const debug = false;

proc testType(type t, param val) {

  if debug then
    writef("testing %s with 0x%xu\n", t:string, val);

  param x: t = val;
  var y: t = x;
  var a: c_ptr(void) = x: c_ptr(void);
  var b: c_ptr(void) = val: t : c_ptr(void);
  var c: c_ptr(void) = x: c_intptr : c_ptr(void);
  var d: c_ptr(void) = x: c_uintptr : c_ptr(void);


  assert(a == b);
  assert(b == c);
  assert(c == d);

  if debug then
    writeln(a);

  if isIntType(t) {
    var back = a: c_intptr : t;
    if debug then
      writef("0x%xu\n", back);
    assert(back == y);
  }
  if isUintType(t)  && (c_sizeof(t) == c_sizeof(c_ptr(void)) || val == 1)  {
    var back = a: c_uintptr : t;
    if debug then
      writef("0x%xu\n", back);
    assert(back == y);
  }
}

proc testWidth(param w) {
  testType(int(w), -1);
  testType(int(w), 1);
  testType(uint(w), 1);
  testType(uint(w), 1:uint(w) << (w-1));
}

testWidth(64);
testWidth(32);
testWidth(16);
testWidth(8);

// also check c_intptr / c_uintptr for good measure.
// normally these are aliases for int/uint
// but we check them again in case that's not the case.
testType(c_intptr, -1);
testType(c_uintptr, 1);
