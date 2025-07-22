//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Subprocess;

// prepare a file for the test
var myFile = open("file.txt", ioMode.cw);
myFile.close();

/* START_EXAMPLE */
use IO;

// define a set of hints using a union operation
var hints = ioHintSet.sequential | ioHintSet.prefetch;

// open a file using the hints
var f: file;
try! {
  f = open("file.txt", ioMode.r, hints=hints);
}
/* STOP_EXAMPLE */

// delete test file
var sub = spawn(["rm", "file.txt"]);
