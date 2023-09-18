use IO, CTypes, FileSystem;

// -------------------------------------------------------------------
// A variation of the "dynamic_buffering" test that executes multiple 
// small writes/reads in a row
// -------------------------------------------------------------------

config const unbuffThreshold = 64,
             iobuffSize = 64,
             smallWriteSize = 3,
             largeWriteSize = 128;

// modify the unbuffered threshold and io buffer size
extern var qio_write_unbuffered_threshold:c_ssize_t;
extern var qio_read_unbuffered_threshold:c_ssize_t;
extern var qbytes_iobuf_size:c_size_t;
qio_write_unbuffered_threshold = unbuffThreshold:c_ssize_t;
qio_read_unbuffered_threshold = unbuffThreshold:c_ssize_t;
qbytes_iobuf_size = iobuffSize:c_size_t;

const w = openWriter("./multi_ops.bin", locking=false, hints = ioHintSet.mmap(false));

// do a couple of small writes
if smallWriteSize > 0 {
  var a : [0..<smallWriteSize] uint(8) = 1;
  w.writeBinary(a);

  a = 2;
  w.writeBinary(a);
}

// do a "large" write operation
var b : [0..<largeWriteSize] uint(8) = 3;
w.writeBinary(b);

// do a couple more small writes
if smallWriteSize > 0 {
  var c : [0..<smallWriteSize] uint(8) = 4;
  w.writeBinary(c);

  c = 5;
  w.writeBinary(c);
}

w.close();

// validate the output
const r = openReader("./multi_ops.bin", locking=false, hints = ioHintSet.mmap(false));
var d : [0..<(largeWriteSize+4*smallWriteSize)] uint(8);

// read a small section
if smallWriteSize > 0 {
  r.readBinary(c_ptrTo(d[0..<smallWriteSize]), smallWriteSize);
  r.readBinary(c_ptrTo(d[smallWriteSize..#smallWriteSize]), smallWriteSize);
}

// read a large section
r.readBinary(c_ptrTo(d[2*smallWriteSize..#largeWriteSize]), largeWriteSize);

// read another small section
if smallWriteSize > 0 {
  r.readBinary(c_ptrTo(d[largeWriteSize+2*smallWriteSize..#smallWriteSize]), smallWriteSize);
  r.readBinary(c_ptrTo(d[largeWriteSize+3*smallWriteSize..]), smallWriteSize);
}

r.close();
remove("multi_ops.bin");

// writeln(d);
writeln(
  (&& reduce (d[0..<smallWriteSize] == 1)) &&
  (&& reduce (d[smallWriteSize..#smallWriteSize] == 2)) &&
  (&& reduce (d[2*smallWriteSize..#largeWriteSize] == 3)) &&
  (&& reduce (d[largeWriteSize+2*smallWriteSize..#smallWriteSize] == 4)) &&
  (&& reduce (d[largeWriteSize+3*smallWriteSize..] == 5))
);
