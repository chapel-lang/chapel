use CTypes;
export proc takesCstring(s: c_string) {
  writeln(string.createCopyingBuffer(s));
}

export proc takesCptr(s: c_ptrConst(c_uchar)) {
  writeln(string.createCopyingBuffer(s));
}

