//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Subprocess;

// prepare a file for the test
var myFile = open("file.dat", ioMode.cw);
var myFileWriter = myFile.writer(locking=false);

var A = for i in 0..<800 do i:uint(8);
myFileWriter.writeBinary(A);

myFileWriter.close();
myFile.close();

/* START_EXAMPLE */
use IO;

// the number of tasks to use
config const nWorkers = 8;

// open a (large) binary file
var f = open("file.dat", ioMode.r);

// compute how many bytes each worker will read
const nBytes = f.size,
      nPerLoc = nBytes/ nWorkers;

// create an array to hold the file contents
var a: [0..<nBytes] uint(8);

// concurrently read each worker's region into 'a'
coforall w in 0..<nWorkers {
  const myRegion = (w*nPerLoc)..<((w+1) * nPerLoc),
        fr = f.reader(region=myRegion, locking=false);

  fr.readBinary(a[myRegion]);
}
/* STOP_EXAMPLE */

writeln("nBytes = ", nBytes);
writeln("nPerLoc = ", nPerLoc);

// delete test file
var sub = spawn(["rm", "file.dat"]);
