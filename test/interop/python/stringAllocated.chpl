use CTypes, OS.POSIX;

export proc g(size: int, ptr: c_ptr(uint(8))): int {
  var s = 'foo';
  if s.numBytes >= size {
    return -1;
  } else {
    memcpy(ptr, c_ptrToConst_helper(s):c_string: c_ptr(void), s.numBytes);
    return s.numBytes;
  }
}

export proc writeStr(in x: c_string) {
  writeln(string.createCopyingBuffer(x));
}
