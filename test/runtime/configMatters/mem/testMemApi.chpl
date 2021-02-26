use CPtr;

// check a couple of sizes can be allocated and freed
var sizes = (0, 1, 16, 17, 1000);
for size in sizes {
  var m = c_malloc(int, size);
  var c = c_calloc(int, size);
  var a = c_aligned_alloc(int, 8, size);
  c_free(m);
  c_free(a);
  c_free(c);
}

// check that calloc zeros
var c = c_calloc(int, 1);
assert(c[0] == 0);
c[0] = 1;
c_free(c);
c = c_calloc(int, 1);
assert(c[0] == 0);
c_free(c);

// check that free with NULL pointer works
var n: c_ptr(int);
c_free(n);
