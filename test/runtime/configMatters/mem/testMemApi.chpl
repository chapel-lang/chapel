use CTypes;

// check a couple of sizes can be allocated and freed
var sizes = (0, 1, 16, 17, 1000);
for size in sizes {
  var m = allocate(int, size);
  var c = allocate(int, size, clear=true);
  var a = allocate(int, size, alignment=8);
  deallocate(m);
  deallocate(a);
  deallocate(c);
}

// check that allocate with clear (calloc) zeroes
var c = allocate(int, 1, clear=true);
assert(c[0] == 0);
c[0] = 1;
deallocate(c);
c = allocate(int, 1, clear=true);
assert(c[0] == 0);
deallocate(c);

// check that free with NULL pointer works
var n: c_ptr(int);
deallocate(n);
