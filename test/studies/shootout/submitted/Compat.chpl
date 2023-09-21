use IO;

@deprecated("openfd is deprecated, please use the file initializer with a 'c_int' argument instead")
proc openfd(x) {
  return new file(x);
}
