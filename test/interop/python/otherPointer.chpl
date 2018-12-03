export proc gimmePointer(x: c_ptr(real)) {
  var y = 4.0;
  c_memcpy(x: c_void_ptr, c_ptrTo(y): c_void_ptr, c_sizeof(y.type));
}

export proc writeReal(x: real) {
  writeln(x);
}
