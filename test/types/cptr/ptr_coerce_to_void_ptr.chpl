use CTypes;
proc test(x: c_ptr(void)) {
  writeln(x == nil);
}

var y: c_ptr(uint(8));
test(y);

var x = allocate(int, 1);

test(x);

deallocate(x);

