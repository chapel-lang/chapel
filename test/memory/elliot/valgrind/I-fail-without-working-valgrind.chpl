// This test is intended to verify that valgrind is working by explicitly
// accessing memory that hasn't been allocated

use CPtr;

proc main() {
  var c = c_malloc(int, 1);
  writeln(c[1]);            // invalid read
}
