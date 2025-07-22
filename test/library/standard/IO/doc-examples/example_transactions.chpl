//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Subprocess;

// prepare a file for the test
var myFile = open("file.txt", ioMode.cw);
var myFileWriter = myFile.writer(locking=false);

var A = for i in 0..<200 do i:uint(8);
myFileWriter.write(A);

myFileWriter.close();
myFile.close();

/* START_EXAMPLE */
use IO;

var fr = openReader("file.txt");

// mark the current channel position
fr.mark();

// read an array of bytes
var a: [0..<200] uint(8);
fr.read(a);

// try to match a pattern
if fr.matchLiteral("b") {
  fr.commit(); // "b" was found, continue reading from the current offset
} else {
  fr.revert(); // "b" was't found, revert back to the marked position

  // try to read something else from the file, throw an error, etc.
}
/* STOP_EXAMPLE */

writeln("offset is ", fr.offset());

// delete test file
var sub = spawn(["rm", "file.txt"]);
