use CPtr;
proc test_nil(x) {
  writeln(x == nil);
  writeln(nil == x);
}
proc test_eq(x, y) {
  writeln(x == y);
}

var a: c_void_ptr = c_nil;
var b: c_void_ptr = a;

test_nil(a);
test_eq(a, b);
