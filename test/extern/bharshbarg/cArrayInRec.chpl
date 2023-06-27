use CTypes;

require "cArrayInRec.h";

extern record myBuffer {
  var foo : c_ptr(c_char);
}

record wrapper {
  var buf : myBuffer;
}

proc main() {
  var x : wrapper;
  for i in 0..9 do x.buf.foo[i] = 65:c_char;
  var s = string.createBorrowingBuffer(x.buf.foo, 10, 10);
  writeln(s);
}
