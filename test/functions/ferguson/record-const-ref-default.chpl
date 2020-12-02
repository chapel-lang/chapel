use CPtr;
record R {
  var x:int;
}

record RR {
  var r:R;
}

var ptr1:c_void_ptr;
var ptr2:c_void_ptr;
config const debug = false;

proc check1(r:R) {
  extern proc c_pointer_return(const ref x:?t):c_ptr(t);
  ptr1 = c_pointer_return(r);
}

proc run() {
  var rr:RR;
  ptr2 = c_ptrTo(rr.r);
  check1(rr.r);
  writeln(rr);

  if debug {
    extern proc printf(fmt:c_string, ptr);
    printf("in check1, address of r is %p\n", ptr1);
    printf("address of rr.r is %p\n", ptr2);
  }

  assert(ptr1 == ptr2);
}

run();
