use IO;
import Time.stopwatch, FileSystem.remove;

config const n : uint(64) = 2048,
             nn = 100000;

config param PerfTest = true;

var wTime = bigWrite();
if PerfTest then writeln("Write Time: ", wTime);

var (rTime, matches) = bigRead();
if PerfTest then writeln("Read Time: ", rTime);

writeln(matches);
remove("big.bin");

proc bigWrite(): real {
  const w = openWriter("big.bin", hints=ioHintSet.mmap(false), locking=false),
        x = [i in 1..n] i;
  var s = new stopwatch();

  w.writeBinary(111);

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

  s.start();
  for i in 0..#nn {
    r.readBinary(y);
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
