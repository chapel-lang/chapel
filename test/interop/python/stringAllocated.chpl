use CPtr;
export proc g(size: int, ptr: c_ptr(uint(8))): int {
  var s = 'foo';
  if s.numBytes >= size {
    return -1;
  } else {
    c_memcpy(ptr, s.c_str(): c_void_ptr, s.numBytes);
    return s.numBytes;
  }
}

export proc writeStr(in x: c_string) {
  writeln(createStringWithNewBuffer(x));
}
