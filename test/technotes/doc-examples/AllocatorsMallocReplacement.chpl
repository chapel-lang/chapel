//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//


/* START_EXAMPLE_0 */
extern {
  #include <stdlib.h>
  static int64_t* mymalloc(void) {
    // If this pointer is to be freed by the Chapel free function,
    // it's much better to use chpl_malloc instead!
    int64_t *ret = (int64_t*) malloc(1024);
    ret[0] = 5;
    return ret;
  }
}

// CTypes provides allocate and deallocate to call the Chapel allocator
// directly for C interoperability purposes
use CTypes;

// Allocate using the system allocator (malloc)
var x = mymalloc();

writeln(x.deref());

// Free using the Chapel allocator - deallocate calls the Chapel free function
// directly.
// This will generally cause a core dump unless:
//   * you have configured Chapel to use the system allocator, or
//   * you link this program with -lchplmalloc
deallocate(x);
/* STOP_EXAMPLE_0 */
