//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;
use Subprocess;

// prepare a file for the test
var myFile = open("ints.txt", ioMode.cw);
var myFileWriter = myFile.writer(locking=false);

for i in 0..<100 do
  myFileWriter.writeln(i);

myFileWriter.close();
myFile.close();

/* START_EXAMPLE */
var r = openReader("ints.txt"),
    sum = 0;

forall line in r.lines() with (+ reduce sum) {
  sum += line:int;
}
/* STOP_EXAMPLE */

writeln("sum = ", sum);

// delete test file
var sub = spawn(["rm", "ints.txt"]);
