//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Subprocess;

// prepare a file for the test
var myFile = open("test.txt", ioMode.cw);
var myFileWriter = myFile.writer(locking=false);

myFileWriter.writeln("this is a test");

myFileWriter.close();
myFile.close();

/* START_EXAMPLE */
use IO;

const r = openReader("test.txt");

try {
  var i = r.read(int);
  // ...
} catch e: EofError {
  writeln("r is at EOF");
  // we're done reading

} catch e: UnexpectedEofError {
  writeln("unable to read an 'int'");
  // try to read something else? ...

} catch e: SystemError {
  writeln("system error in IO implementation: ", e);
  // try to recover from the error? ...

} catch e: Error {
  writeln("something else went wrong...");
}
/* STOP_EXAMPLE */

// delete test file
var sub = spawn(["rm", "test.txt"]);
