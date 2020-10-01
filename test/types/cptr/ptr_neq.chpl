use CPtr;
proc test_nil(x) {
  writeln(x != nil);
  writeln(nil != x);
}
proc test_neq(x, y) {
  writeln(x != y);
}

var a: c_void_ptr = c_nil;
var tmp: c_void_ptr = c_ptrTo(a):c_void_ptr;
var b: c_void_ptr;
b = tmp;
test_nil(a);
test_neq(a, b);
