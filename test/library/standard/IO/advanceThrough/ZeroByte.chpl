use IO;

testAdvancePast(openReader("singleByteInput.txt", locking=false), "");
testAdvancePast(openReader("singleByteInput.txt", locking=false), b"");

proc testAdvancePast(r, sep) {
  // advance past the delimiter
  try {
    r.advanceThrough(sep);
  } catch e {
    writeln(e);
  }

  writeln("after advanceThrough, offset is ", r.offset());
}
