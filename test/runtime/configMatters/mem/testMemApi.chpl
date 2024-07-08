use CTypes;

// check a couple of sizes can be allocated and freed
var sizes = (0, 1, 16, 17, 1000);
for size in sizes {
  var sizeAsCSizeT : c_size_t = size.safeCast(c_size_t);
  var m = allocate(int, sizeAsCSizeT);
  var c = allocate(int, sizeAsCSizeT, clear=true);
  var a = allocate(int, sizeAsCSizeT, alignment=8);
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
