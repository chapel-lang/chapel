use CPtr;
proc test(x: c_void_ptr) {
  writeln(x == nil);
}

var y: c_ptr(uint(8));
test(y);

var x = c_malloc(int, 1);

test(x);

c_free(x);

