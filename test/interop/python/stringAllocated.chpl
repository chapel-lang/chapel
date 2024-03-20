use CTypes, OS.POSIX;

export proc g(size: int, ptr: c_ptr(uint(8))): int {
  var s = 'foo';
  if s.numBytes >= size {
    return -1;
  } else {
    memcpy(ptr, s.c_str(): c_ptr(void), s.numBytes);
    return s.numBytes;
  }
}
// TODO: Remove once c_string fully deprecated
export proc writeCstr(in x: chpl_c_string) {
  writeln(string.createCopyingBuffer(x:c_ptrConst(c_char)));
}

export proc writeStr(in x: c_ptrConst(c_char)) {
  writeln(string.createCopyingBuffer(x));
}
