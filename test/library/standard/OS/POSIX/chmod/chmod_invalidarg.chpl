use OS.POSIX;
// Test what happens when the value for permissions is invalid
// Should raise an IllegalArgumentError
config const permissions = 0o777: c_int;
var filename = c"file";
var err = chmod(filename, permissions: mode_t);
if err != 0 then halt("Illegal Argument");
