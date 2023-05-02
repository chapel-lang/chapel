use IO, CTypes, FileSystem;

extern var qbytes_iobuf_size:c_size_t;
qbytes_iobuf_size = 1024:c_size_t;

extern var qio_write_unbuffered_threshold:c_ssize_t;
qio_write_unbuffered_threshold = 2048:c_ssize_t;

config const maxBufSize = 16384;

var bufSize = 128,
    bufDom = {0..<bufSize},
    buf : [bufDom] uint(8);

const fw = openWriter("gb.bin", locking=false, hints=ioHintSet.mmap(false));

var i : uint(8) = 1;
while bufSize < maxBufSize {
  buf = i;
  fw.writeBinary(buf);
  bufSize *= 2;
  bufDom = {0..<bufSize};
  i += 1;
}

fw.close();

const fr = openReader("gb.bin", locking=false, hints=ioHintSet.mmap(false));

bufSize = 128;
bufDom = {0..<bufSize};
i = 1;
while bufSize < maxBufSize {
  const numRead = fr.readBinary(buf);
  assert(numRead == bufSize);

  if ! && reduce (i == buf) {
    writeln("mismatched write/read: ", bufSize, " for i=", i);
    writeln(buf);
  }

  bufSize *= 2;
  bufDom = {0..<bufSize};
  i += 1;
}

remove("gb.bin");
