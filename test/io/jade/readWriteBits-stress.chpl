// stress test readBits/writeBits with fuzzed input

use IO;
use Random;

config const n = 10_000_000;
config const repeat = 20;
config const filename = "test.bin";

for 1..repeat {
  var f = open(filename, ioMode.cwr);
  var writes: [1..n] (uint(64), int(64));
  {
    var w = f.writer(serializer=new binarySerializer(), locking=false);
    var rs = new randomStream(uint);
    for i in 1..n {
      const val = rs.next();
      const nbits = rs.next(0, 64):int;
      w.writeBits(val, nbits);
      writes[i] = (val, nbits);
    }
  }
  {
    var r = f.reader(deserializer=new binaryDeserializer(), locking=false);
    for i in 1..n {
      const nbits = writes[i](1);
      const mask = if nbits == 64 then ~0:uint(64) else (1:uint(64) << nbits) - 1;
      const expected = writes[i](0) & mask;
      var tmp:uint(64);
      r.readBits(tmp, nbits);
      assert(tmp == expected, "Expected ", expected, " but got ", tmp,
                              " for write of ", nbits, " bits");
    }
  }
}
