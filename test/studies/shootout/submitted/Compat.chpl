use IO;

@deprecated("openfd is deprecated, please use the file initializer with a 'c_int' argument instead")
proc openfd(x) {
  return new file(x);
}

@deprecated(notes="'readstring' is deprecated; please use 'readString' instead")
proc fileReader.readstring(ref s, len = -1) do return this.readString(s, len);

@deprecated(notes="'readbytes' is deprecated; please use 'readBytes' instead")
proc fileReader.readbytes(ref b, len = -1) do return this.readBytes(b, len);
