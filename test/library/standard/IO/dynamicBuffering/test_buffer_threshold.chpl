use IO, CTypes;

extern var qbytes_iobuf_size:c_size_t;
extern var qio_write_unbuffered_threshold:c_ssize_t;

config const start_write_size = 0,
             iobuf_size = 8;

const writeThreshold = 32,
      writeSize = 64;

// set qio parameters
qbytes_iobuf_size = iobuf_size:c_size_t;
qio_write_unbuffered_threshold = writeThreshold:c_ssize_t;

var w = openWriter("buf_test.txt", hints = ioHintSet.noMmap);

// put some initial bytes in the channel to allocate the first buffer piece
if start_write_size > 0 {
  var start_array : [0..<start_write_size] uint(8) = 1,
      sa_ptr = c_ptrTo(start_array);
  w.writeBinary(sa_ptr, start_array.size);
}

// write an array larger than the buffering threshold
var buff : [0..<writeSize] uint(8) = 2,
    buff_ptr = c_ptrTo(buff);
w.writeBinary(buff_ptr, buff.size);

w.close();

// ensure the writing was correct
var r = openReader("buf_test.txt"),
    buff_out : [0..<start_write_size+writeSize] uint(8);
r.readBinary(buff_out);
// writeln(buff_out);

writeln(&& reduce (buff_out[0..<start_write_size] == 1));
writeln(&& reduce (buff_out[start_write_size..] == 2));
