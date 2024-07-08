

use Allocators;
use CTypes;

class C { var x: int; }

record myAllocator: allocator {
  proc ref allocate(n: int): c_ptr(void) {
    return CTypes.allocate(int(8), n.safeCast(c_size_t));
  }
  proc ref deallocate(p: c_ptr(void)) {
    CTypes.deallocate(p);
  }
}

var p = new myAllocator();
var x = newWithAllocator(p, unmanaged C, 1);
var y = newWithAllocator(p, unmanaged C, 1);
writeln(x, " ", y);
deleteWithAllocator(p, x, y);
