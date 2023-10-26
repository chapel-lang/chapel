
use IO;
use Types;
use CTypes;

type eltType = int;

proc test(const n : int, seekRange: range(?)) {
  writeln("=== n=", n, "; ", seekRange, " ===");
  const D = {0..#n};
  const SENTINEL = 5;

  var f = openMemFile();
  {
    var A : [D] eltType = SENTINEL;
    var w = f.writer(serializer=new binarySerializer());
    w.write(A);
  }

  try {
    var B : [D] eltType = 1;
    var r = f.reader(locking=false, deserializer=new binaryDeserializer());
    r.seek(seekRange);
    r.read(B);
    if && reduce (B==SENTINEL) then writeln("SUCCESS");
    else writeln("FAILURE");
  } catch e {
    writeln(e.message());
  }

  try {
    var B : [D] eltType = 1;
    var r = f.reader(locking=false, deserializer=new binaryDeserializer());
    r.seek(seekRange);
    r.readBinary(B);
    if && reduce (B==SENTINEL) then writeln("SUCCESS");
    else writeln("FAILURE");
  } catch e {
    writeln(e.message());
  }

  try {
    var B : [D] eltType = 1;
    var r = f.reader(locking=false, deserializer=new binaryDeserializer());
    r.seek(seekRange);
    var ptr = c_addrOf(B[0]);
    r.readBinary(ptr, B.size*numBytes(eltType));
    if && reduce (B==SENTINEL) then writeln("SUCCESS");
    else writeln("FAILURE");
  } catch e {
    writeln(e.message());
  }
}

proc main() {
  const open = 0..;
  const full = 0..#(100 * numBytes(eltType));
  const half = 0..#(50 * numBytes(eltType));
  test(100, open);
  test(100, full);
  test(100, half);
}
