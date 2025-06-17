//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Allocators;

/* START_EXAMPLE */
class MyClass { var x: int; }
var allocator = new bumpPtrMemPool(1024);
// The following two lines are equivalent, but the second one uses the allocator
var x = new unmanaged MyClass(1);
var y = newWithAllocator(allocator, unmanaged MyClass, 1);
/* STOP_EXAMPLE */

writeln(x);
writeln(y);

delete x;
deleteWithAllocator(allocator, y);
