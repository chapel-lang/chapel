use OS.POSIX;
// Test the correctness of changing the mode of files and directories.  Assumes
// that the process umask is 0022.  Note: will break if umask is different.
config const permissions = 0o777: c_int;
var filename = "file".c_str();
var dirname = "dir".c_str();
chmod(filename, permissions: mode_t);
chmod(dirname, permissions: mode_t);
