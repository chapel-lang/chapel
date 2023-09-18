use IO;
import Time.stopwatch, FileSystem.remove;

config const n : uint(64) = 2048*8,
             numWrites = 10000;

config param PerfTest = true;

// write 'numWrites' arrays of 'n' uint(64) elements to a file
var wTime = bigWrite();
if PerfTest then writef("Write Time: %.10r\n", wTime);

// read the same array 'numWrites' times from the file
// (in the non-PerfTest case, check that the arrays match)
var (rTime, matches) = bigRead();
if PerfTest then writef("Read Time: %.10r\n", rTime);

writeln(matches);
remove("big.bin");

proc bigWrite(): real {
  const w = openWriter("big.bin", hints=ioHintSet.mmap(false), locking=false),
        x = [i in 1..n] i,
        smallArray = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
  var s = new stopwatch();

  // write something small to allocate buffer space in the fileWriter's buffer
  w.writeBinary(smallArray);

  // write the array 'numWrites' times
  s.start();
  for i in 0..#numWrites do
    w.writeBinary(x);
  s.stop();

  return s.elapsed();
}

proc bigRead() : (real, bool) {
  const r = openReader("big.bin", hints=ioHintSet.mmap(false), locking=false),
        x = [i in 1..n] i;
  var y : [1..n] uint(64),
      s = new stopwatch(),
      matches: bool,
      small: [1..10] int;

  r.readBinary(small);
  matches = (&& reduce (small == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]));

  // read the array 'numWrites' times
  s.start();
  for i in 0..#numWrites {
    r.readBinary(y);

    // check that the array matches
    if !PerfTest {
      if !y.equals(x) {
        writeln("Mismatch at ", i);
        matches = false;
        break;
      }
    }
  }
  s.stop();

  return (s.elapsed(), matches);
}
