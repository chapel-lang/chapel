use IO;

testAdvancePast(openReader("singleByteInput.txt", locking=false), "");
testAdvancePast(openReader("singleByteInput.txt", locking=false), b"");

proc testAdvancePast(r, sep) {
  // advance past the delimiter
  try {
    r.advanceTo(sep);
  } catch e {
    writeln(e);
  }

  writeln("after advanceTo, offset is ", r.offset());
}
