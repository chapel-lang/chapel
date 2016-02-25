proc test(x: c_void_ptr) {
  writeln(x == nil);
}

var y: c_ptr(uint(8));
test(y);

test(c_malloc(int, 1));
