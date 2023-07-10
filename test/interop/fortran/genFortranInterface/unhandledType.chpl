use CTypes;
export proc takesCstring(s: c_string) {
  writeln(string.createCopyingBuffer(s));
}

export proc takesCptr(s: c_ptrConst(c_char)) {
  writeln(string.createCopyingBuffer(s));
}

