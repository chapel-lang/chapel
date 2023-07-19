
class C {
  var x:int;
}


proc main() {
  use CTypes;
  var c = new unmanaged C(1);
  // cast it to a c_ptr(void)
  var ptr1 = c:c_ptr(void);
  // cast that back to an object
  var c2 = ptr1:unmanaged C?;
  // cast nil into a c_ptr(void)
  var mynil = nil:c_ptr(void);
  assert(ptr1 != mynil);
  assert(c == c2);

  var my_cstr:c_string = c"test";
  // cast c_string into c_ptr(void)
  var my_cstr_v = my_cstr:c_ptr(void);
  assert(my_cstr_v != mynil);
  // cast c_ptr(void) into c_string
  var my_cstr2 = my_cstr_v:c_string;
  assert(my_cstr == my_cstr2);

  delete c;
}

