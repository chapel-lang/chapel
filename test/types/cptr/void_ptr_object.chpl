
class C {
  var x:int;
}


proc main() {

  var c = new C(1);
  // cast it to a c_void_ptr
  var ptr1 = c:c_void_ptr;
  // cast that back to an object
  var c2 = ptr1:C;
  // cast nil into a c_void_ptr
  var mynil = nil:c_void_ptr;
  assert(ptr1 != mynil);
  assert(c == c2);
}

