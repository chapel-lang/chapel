use HDF5.C_HDF5;

module NativeHDF5 {
  extern {
    #include <hdf5.h>
  }
}

proc nativeIntType() {
  return NativeHDF5.H5T_NATIVE_INT;
}

for 1..2 {
  const nativeInt = nativeIntType();
  assert(nativeInt.type == hid_t);
  assert(H5Tget_size(nativeInt) == c_sizeof(c_int));
  const properties = H5Pcreate(NativeHDF5.H5P_FILE_ACCESS);
  assert(properties >= 0);
  assert(H5Pclose(properties) >= 0);
  assert(H5close() >= 0);
}
writeln("HDF5 reinitialization after close passed");
