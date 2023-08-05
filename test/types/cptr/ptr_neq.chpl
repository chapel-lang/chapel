use CTypes;
proc test_nil(x) {
  writeln(x != nil);
  writeln(nil != x);
}
proc test_neq(x, y) {
  writeln(x != y);
}

var a: c_ptr(void) = nil;
var tmp: c_ptr(void) = c_ptrTo(a):c_ptr(void);
var b: c_ptr(void);
b = tmp;
test_nil(a);
test_neq(a, b);
