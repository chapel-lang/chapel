//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Subprocess;

/* START_EXAMPLE_1 */
use IO;

try {
  // open the file "test-file.txt" for writing, creating it if
  // it does not exist yet.
  var myFile = open("test-file.txt", ioMode.cw);

  // create a fileWriter starting at the beginning of the file
  // (this fileWriter will not be used in parallel, so does not need to use
  // locking)
  var myFileWriter = myFile.writer(locking=false);

  var x: int = 17;

  // This function will write the human-readable text version of x;
  // binary I/O is also possible.
  myFileWriter.write(x);

  // Now test-file.txt contains:
  // 17

} catch e: Error {
  // Generally speaking, the I/O functions throw errors.  Handling these
  // errors is application-dependent and is left out of this example for
  // brevity.  Please see the documentation for individual functions for more
  // details about errors that they can throw.
}
/* STOP_EXAMPLE_1 */

/* START_EXAMPLE_2 */
use IO;

try {
  // open the file "test-file.txt" for reading only
  var myFile = open("test-file.txt", ioMode.r);

  // create a fileReader starting at the beginning of the file
  // (this fileReader will not be used in parallel, so does not need to use
  // locking)
  var myFileReader = myFile.reader(locking=false);

  var x: int;

  // Now read a textual integer. Note that the
  // fileReader.read function returns a bool to indicate
  // if it read something or if the end of the file
  // was reached before something could be read.
  var readSomething = myFileReader.read(x);

  writeln("Read integer ", x);
  // prints out:
  // Read integer 17

} catch e: Error {
  // Generally speaking, the I/O functions throw errors.  Handling these
  // errors is application-dependent and is left out of this example for
  // brevity.  Please see the documentation for individual functions for more
  // details about errors that they can throw.
}
/* STOP_EXAMPLE_2 */

// delete test file
var sub = spawn(["rm", "test-file.txt"]);
