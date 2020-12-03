const pathPrefixEnvName = c"CHPL_HDF5_FILE_PREFIX";

// The prefix read from the environment variable will be added
// to the path for HDF5 files. Make sure to include the trailing slash!
proc readPrefixEnv() {
  use Sys;
  var prefix: c_string;
  if sys_getenv(pathPrefixEnvName, prefix) {
    return createStringWithNewBuffer(prefix);
  } else {
    return "";
  }
}
