use FileSystem;
// Test the correctness of changing the mode of files and directories.  Assumes
// that the process umask is 0022.  Note: will break if umask is different.
config const permissions = 0o777;
var filename = "file";
var dirname = "dir";
chmod(filename, permissions);
chmod(dirname, permissions);
