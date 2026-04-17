//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Allocators;

/* START_EXAMPLE */
class MyClass { var x: int; }
var allocator = new mallocWrapper();
var x = newWithAllocator(allocator, unmanaged MyClass, 1);
writeln(x);
deleteWithAllocator(allocator, x);
/* STOP_EXAMPLE */
