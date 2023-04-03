use IO;
import Time.stopwatch, FileSystem.remove;

config const n : uint(64) = 2048,
             nn = 100000;

config param PerfTest = true;

// write nn arrays of n uint(64) elements to a file
var wTime = bigWrite();
if PerfTest then writef("Write Time: %.10r\n", wTime);

// read the same array nn times from the file
// (in the non-PerfTest case, check that the arrays match)
var (rTime, matches) = bigRead();
if PerfTest then writef("Read Time: %.10r\n", rTime);

writeln(matches);
remove("big.bin");

proc bigWrite(): real {
  const w = openWriter("big.bin", hints=ioHintSet.mmap(false), locking=false),
        x = [i in 1..n] i;
  var s = new stopwatch();

  // write something small to allocate buffer space in the channel
  w.writeBinary(111);

  // write the array nn times
  s.start();
  for i in 0..#nn do
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
      small: int;

  r.readBinary(small);
  matches = (small == 111);

  // read the array nn times
  s.start();
  for i in 0..#nn {
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
