use IO, CTypes, FileSystem;

config const unbuffThreshold = 64,
             iobuffSize = 64,
             smallWriteSize = 0;

// modify the unbuffered threshold and io buffer size
extern var qio_write_unbuffered_threshold:c_ssize_t;
extern var qio_read_unbuffered_threshold:c_ssize_t;
extern var qbytes_iobuf_size:c_size_t;
qio_write_unbuffered_threshold = unbuffThreshold:c_ssize_t;
qio_read_unbuffered_threshold = unbuffThreshold:c_ssize_t;
qbytes_iobuf_size = iobuffSize:c_size_t;

proc testDB(s: int, verbose: bool = false): bool {
  const w = openWriter("./db.bin", locking=false, hints = ioHintSet.mmap(false));

  // put something small in the buffer to start with
  if smallWriteSize > 0 {
    var a : [0..<smallWriteSize] uint(8) = 1;
    w.writeBinary(c_ptrTo(a), a.size);
  }

  // do a "large" write operation
  var b : [0..<s] uint(8) = 2;
  w.writeBinary(c_ptrTo(b), b.size);

  // do another small write
  if smallWriteSize > 0 {
    var c : [0..<smallWriteSize] uint(8) = 3;
    w.writeBinary(c_ptrTo(c), c.size);
  }

  w.close();

  // validate the output
  const r = openReader("./db.bin", locking=false, hints = ioHintSet.mmap(false));
  var d : [0..<(s+2*smallWriteSize)] uint(8);

  // read a small section
  if smallWriteSize > 0 then
    r.readBinary(c_ptrTo(d[0..<smallWriteSize]), smallWriteSize);

  // read a large section
  r.readBinary(c_ptrTo(d[smallWriteSize..#s]), s);

  // read another small section
  if smallWriteSize > 0 then
    r.readBinary(c_ptrTo(d[s+smallWriteSize..]), smallWriteSize);

  r.close();

  if verbose then writeln(d);

  return (&& reduce (d[0..<smallWriteSize] == 1)) &&
         (&& reduce (d[smallWriteSize..#s] == 2)) &&
         (&& reduce (d[s+smallWriteSize..] == 3));
}

for size in (unbuffThreshold-1)..(2*unbuffThreshold) {
  if !testDB(size) {
    writeln("failed on: ", size);
    testDB(size, true);
    break;
  }
}

remove("db.bin");
