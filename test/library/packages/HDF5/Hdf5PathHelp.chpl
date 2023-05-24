use CTypes;
const pathPrefixEnvName = c_ptrToConst_helper("CHPL_HDF5_FILE_PREFIX");

// The prefix read from the environment variable will be added
// to the path for HDF5 files. Make sure to include the trailing slash!
proc readPrefixEnv() {
  use CTypes;
  extern proc sys_getenv(name:c_ptrConst(c_uchar), ref string_out:c_ptrConst(c_uchar)):c_int;

  var prefix: c_ptrConst(c_uchar);
  if sys_getenv(pathPrefixEnvName, prefix) {
    return string.createCopyingBuffer(prefix);
  } else {
    return "";
  }
}
