use CTypes;
proc f_ptr():c_ptrConst(c_char) {
  return "Hello";
}

var x_ptr = f_ptr();
writeln(string.createCopyingBuffer(x_ptr));
assert(x_ptr.type == c_ptrConst(c_char));
