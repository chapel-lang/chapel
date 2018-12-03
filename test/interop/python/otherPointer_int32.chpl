export proc gimmePointer(x: c_ptr(int(32))) {
  var y: int(32) = 4;
  c_memcpy(x: c_void_ptr, c_ptrTo(y): c_void_ptr, c_sizeof(y.type));
}

export proc writeInt(x: int(32)) {
  writeln(x);
}
