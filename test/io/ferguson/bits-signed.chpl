// tests convenience signatures for bit-I/O routines
// these help avoid casting

use FileSystem;
use IO;

config const test=1;
config const testfile="test.bin";
var f = open(testfile, ioMode.cwr);

if test == 1 { // ints
  {
      var w = f.writer(kind=ionative);

      // Write 011
      var x = 0b011;
      var nbits = 3;
      // now x, nbits are int
      w.writeBits(x, nbits);
      w.close();
  }

  // Try reading it back the way we wrote it.
  {
      var r = f.reader(kind=ionative);
      var tmp:int;
      var nbits = 3;
      // now tmp, nbits are int
      r.readBits(tmp, nbits);
      writeln("Read ", tmp);
      assert(tmp == 0b011);
  }
}

if test == 2 { // uints
  {
      var w = f.writer(kind=ionative);

      // Write 011
      var x:uint = 0b011;
      var nbits:int = 3;
      // now x, nbits are int
      w.writeBits(x, nbits);
      w.close();
  }

  // Try reading it back the way we wrote it.
  {
      var r = f.reader(kind=ionative);
      var tmp:uint;
      var nbits:int = 3;
      // now tmp, nbits are int
      r.readBits(tmp, nbits);
      writeln("Read ", tmp);
      assert(tmp == 0b011);
  }
}

if test == 3 { // int(8)s
  {
      var w = f.writer(kind=ionative);

      // Write 011
      var x:int(8) = 0b011;
      var nbits:int(8) = 3;
      // now x, nbits are int
      w.writeBits(x, nbits);
      w.close();
  }

  // Try reading it back the way we wrote it.
  {
      var r = f.reader(kind=ionative);
      var tmp:int(8);
      var nbits:int(8) = 3;
      // now tmp, nbits are int
      r.readBits(tmp, nbits);
      writeln("Read ", tmp);
      assert(tmp == 0b011);
  }
}

if test == 4 { // uint(8)s
  {
      var w = f.writer(kind=ionative);

      // Write 011
      var x:uint(8) = 0b011;
      var nbits:uint(8) = 3;
      // now x, nbits are int
      w.writeBits(x, nbits);
      w.close();
  }

  // Try reading it back the way we wrote it.
  {
      var r = f.reader(kind=ionative);
      var tmp:uint(8);
      var nbits:uint(8) = 3;
      // now tmp, nbits are int
      r.readBits(tmp, nbits);
      writeln("Read ", tmp);
      assert(tmp == 0b011);
  }
}

FileSystem.remove(testfile);

