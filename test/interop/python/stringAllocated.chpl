export proc g(size: int, ptr: c_ptr(uint(8))): int {
  var s = 'foo';
  if s.size >= size {
    return -1;
  } else {
    c_memcpy(ptr, s.c_str(): c_void_ptr, s.size);
    return s.size;
  }
}

export proc writeStr(x: c_string) {
  writeln(x: string);
}
