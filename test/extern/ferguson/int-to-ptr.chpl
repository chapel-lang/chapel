use CTypes;
use CPtr;

config const debug = false;

proc testType(type t, param val) {

  if debug then
    writef("testing %s with 0x%xu\n", t:string, val);

  param x: t = val;
  var y: t = x;
  var a: c_void_ptr = x: c_void_ptr;
  var b: c_void_ptr = val: t : c_void_ptr;
  var c: c_void_ptr = x: c_intptr : c_void_ptr;
  var d: c_void_ptr = x: c_uintptr : c_void_ptr;


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
  if isUintType(t)  && (c_sizeof(t) == c_sizeof(c_void_ptr) || val == 1)  {
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
