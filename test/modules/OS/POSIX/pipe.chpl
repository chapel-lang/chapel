use OS.POSIX;

var fildes:2*c_int;
const ret = pipe(c_ptrTo(fildes));
writeln(ret == 0);
if ret == 0 {
  // The pipe was created properly.  Here we create two tasks.  One
  // writes to the write end and the other reads from the read end.
  var vals: [0..3] int;
  var order$:single bool; // this is just to keep the output ordered
  cobegin {
    { // this is the writer task
      vals = [ 1, 2, 3, max(int) ]; // thanks, George Gamow!
      writeln(POSIX.write(fildes[1], c_ptrTo(vals),
                          (vals.size * numBytes(vals.eltType)):c_size_t));
      order$.writeEF(true);
    }
    { // and this is the reader task
      if order$.readFF() {
        var buf: [vals.domain] int;
        writeln(POSIX.read(fildes[0], c_ptrTo(buf),
                           (buf.size * numBytes(buf.eltType)):c_size_t));
        writeln(buf);
      }
    }
  }
}
