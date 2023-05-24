use CTypes;
proc test(x: c_void_ptr) {
  writeln(x == nil);
}

var y: c_ptr(uint(8));
test(y);

var x = allocate(int, 1);

test(x);

deallocate(x);

