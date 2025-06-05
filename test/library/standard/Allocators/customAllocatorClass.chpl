use IO;
use Allocators;
use CTypes;

config const debug = false;

class C { var x: int; }

class myAllocator: allocator {
  proc allocate(n: int): c_ptr(void) {
    var ptr = CTypes.allocate(int(8), n.safeCast(c_size_t));
    if debug then writef("allocated ptr=%? n=%?\n", ptr, n);
             else writef("allocated n=%?\n", n);
    return ptr;
  }
  proc deallocate(p: c_ptr(void)) {
    CTypes.deallocate(p);
    if debug then writef("deallocated ptr=%?\n", p);
              else writef("deallocated\n");
  }
}

var p = new myAllocator();
var x = newWithAllocator(p, unmanaged C, 1);
var y = newWithAllocator(p, unmanaged C, 2);
writeln(x, " ", y);
deleteWithAllocator(p, x, y);
