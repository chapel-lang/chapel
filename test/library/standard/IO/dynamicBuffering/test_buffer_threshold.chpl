use IO, CTypes;

extern var qbytes_iobuf_size:c_size_t;
extern var qio_write_unbuffered_threshold:c_ssize_t;

config const start_write_size = 0,
             iobuf_size = 0;

const writeThreshold = 32,
      writeSize = 64;

// set qio parameters
qbytes_iobuf_size = iobuf_size:c_size_t;
qio_write_unbuffered_threshold = writeThreshold:c_ssize_t;

var w = openWriter("buf_test.txt", hints = ioHintSet.mmap(false));

// put some initial bytes in the channel to allocate the first buffer piece
const start_array : [0..<start_write_size] uint(8) = 1;
w.writeBinary(c_ptrTo(start_array), start_array.size);

// write an array larger than the buffering threshold
const buff : [0..<writeSize] uint(8) = 2;
w.writeBinary(c_ptrTo(buff), buff.size);

w.close();

var r = openReader("buf_test.txt"),
    buff_out : [0..<start_write_size+writeSize] uint(8);
writeln(r.readBinary(buff_out));
writeln(buff_out);
