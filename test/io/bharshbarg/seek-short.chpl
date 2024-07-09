
use IO;
use Types;
use CTypes;

type eltType = int;

proc test(const n : int, seekRange: range(?), endian: endianness) {
  writeln("=== n=", n, "; ", seekRange, " ===");
  const D = {0..#n};
  const SENTINEL = 5;

  var f = openMemFile();
  {
    var A : [D] eltType = SENTINEL;
    var w = f.writer(serializer=new binarySerializer(endian), locking=false);
    w.write(A);
  }

  write("fileReader.read: ");
  try {
    var B : [D] eltType = 1;
    var r = f.reader(locking=false, deserializer=new binaryDeserializer(endian));
    r.seek(seekRange);
    const got = r.read(B);
    write("return = ", got, "; ");
    if && reduce (B==SENTINEL) then writeln("SUCCESS");
    else writeln("FAILURE");
  } catch e {
    writeln("ERROR: ", e.message());
  }

  write("readBinary([]): ");
  try {
    var B : [D] eltType = 1;
    var r = f.reader(locking=false);
    r.seek(seekRange);
    const n = r.readBinary(B, endian);
    write("got ", n, "; ");
    if && reduce (B==SENTINEL) then writeln("SUCCESS");
    else writeln("FAILURE");
  } catch e {
    writeln("ERROR: ", e.message());
  }

  write("readBinary(int): ");
  try {
    var B : [D] eltType = 1;
    var r = f.reader(locking=false);
    r.seek(seekRange);
    for b in B do r.readBinary(b, endian);
    if && reduce (B==SENTINEL) then writeln("SUCCESS");
    else writeln("FAILURE");
  } catch e {
    writeln("ERROR: ", e.message());
  }

  write("readBinary(c_ptr(int)): ");
  try {
    var B : [D] eltType = 1;
    var r = f.reader(locking=false);
    r.seek(seekRange);
    var ptr = c_addrOf(B[0]);
    const n = r.readBinary(ptr, B.size*numBytes(eltType));
    writeln("got ", n, "; ");
  } catch e {
    writeln("ERROR: ", e.message());
  }
}

proc runTest(endian:endianness) {
  writeln("===== ", endian, " =====");
  const open = 0..;
  const full = 0..#(100 * numBytes(eltType));
  const half = 0..#(50 * numBytes(eltType));
  const off = 0..#(50 * numBytes(eltType) - 1);
  test(100, open, endian);
  test(100, full, endian);
  test(100, half, endian);
  test(100, off, endian);
  writeln();
}

proc main() {
  runTest(endianness.native);
  runTest(endianness.little);
  runTest(endianness.big);
}
