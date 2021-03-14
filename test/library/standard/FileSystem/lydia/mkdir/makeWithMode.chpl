use FileSystem;
// Test to ensure sent in directory permissions will be applied.  Assumes that
// the process umask is 0022.  Note: will break if umask is different.
config const permissions = 511;
var dirname = "useAMode";
mkdir(dirname, permissions);
