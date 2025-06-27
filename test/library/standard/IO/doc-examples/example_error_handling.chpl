//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
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
