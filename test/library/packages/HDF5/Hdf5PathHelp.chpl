use CTypes;
const pathPrefixEnvName = "CHPL_HDF5_FILE_PREFIX".c_str();

// The prefix read from the environment variable will be added
// to the path for HDF5 files. Make sure to include the trailing slash!
proc readPrefixEnv() {
  extern proc sys_getenv(name:c_ptrConst(c_char), ref string_out:c_ptrConst(c_char)):c_int;

  var prefix: c_ptrConst(c_char);
  if sys_getenv(pathPrefixEnvName, prefix) {
    return string.createCopyingBuffer(prefix);
  } else {
    return "";
  }
}
