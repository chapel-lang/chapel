use SysBasic;

/* This record does something probably not allowed in the language:
   it uses the C interop mechanisms to store in one field a pointer
   to another field. That pointer will break every time the record
   is bit-copied. This test exists just so that we can see cases in
   which records are bit-copied.
 */
config const debug = false;

record R {
  var x: int = 0;
  var ptr_to_x: c_ptr(int) = nil;
}

proc ref R.set_self_ptr() {
  ptr_to_x = c_ptrTo(x);
}

proc ref R.increment() {
  x += 1;
}

// custom constructor setting self-pointer
// This won't work as long as constructors
// return the record (vs being methods on the record).
/*
proc R.R(x:int) {
  this.x = x;
  set_self_ptr();
}
*/

proc ref R.init(x = 0) {
  this.x = x;
  set_self_ptr();
}

proc ref R.verify() {
  extern proc printf(fmt:c_string, arg:c_ptr(int), arg2:c_ptr(int));

  // default initialized records have nil ptr, OK
  if ptr_to_x == nil then return;

  var expect_ptr = c_ptrTo(x);

  if ptr_to_x != expect_ptr {
    printf("R.verify failed - got pointer %p but expected pointer %p\n",
           ptr_to_x, expect_ptr);
    assert(false);
  }
}

// We'd like this to be by ref, but doing so leads to an internal
// compiler error.  See
// $CHPL_HOME/test/types/records/sungeun/recordWithRefCopyFns.future
pragma "donor fn"
pragma "auto copy fn"
proc chpl__autoCopy(arg: R) {
  extern proc printf(fmt:c_string, arg:c_ptr(int));
  extern proc printf(fmt:c_string, arg:c_ptr(int), arg2:c_ptr(int));
  if debug then
    printf("in auto copy from %p\n", arg.ptr_to_x);

  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  ret.init(x = arg.x);

  if debug then
    printf("leaving auto copy from %p to %p\n", arg.ptr_to_x, ret.ptr_to_x);

  return ret;
}

// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R) {
  extern proc printf(fmt:c_string, arg:c_ptr(int));
  extern proc printf(fmt:c_string, arg:c_ptr(int), arg2:c_ptr(int));
  if debug then
    printf("in init copy from %p\n", arg.ptr_to_x);

  var ret: R;

  ret.init(x = arg.x);

  if debug then
    printf("leaving init copy from %p to %p\n", arg.ptr_to_x, ret.ptr_to_x);

  return ret;
}

proc =(ref lhs: R, rhs: R) {
  extern proc printf(fmt:c_string, arg:c_ptr(int));
  extern proc printf(fmt:c_string, arg:c_ptr(int), arg2:c_ptr(int));
  if debug then
    printf("in assign lhs = %p\n", rhs.ptr_to_x);

  lhs.init(x = rhs.x);

  if debug then
    printf("leaving assign %p = %p\n", lhs.ptr_to_x, rhs.ptr_to_x);
}

proc verify() {
}
