use CTypes;
proc test_nil(x) {
  writeln(x == nil);
  writeln(nil == x);
}
proc test_eq(x, y) {
  writeln(x == y);
}

var a: c_void_ptr = c_nil;
var b: c_void_ptr = a;
var a_cptr = a : c_ptr(int);
var b_cptr = b : c_ptr(int);
var a_cptrconst = a : c_ptrConst(int);
var b_cptrconst = b : c_ptrConst(int);

test_nil(a);
test_eq(a, b);
test_eq(a_cptr, b_cptr);
test_eq(a_cptrconst, b_cptrconst);
