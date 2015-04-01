use SysBasic;

record R {
  var x: int = 0;
  var ptr_to_x: c_ptr(int) = nil;
}

proc ref R.set_self_ptr() {
  ptr_to_x = c_ptrTo(x);
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


