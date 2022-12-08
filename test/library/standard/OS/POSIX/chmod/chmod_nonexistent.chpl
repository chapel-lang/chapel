use OS.POSIX;
// Test to ensure that calling chmod on a file that doesn't exist will break
// as expected.
const permissions = 0o777: c_int;
var filename = c"nonexistent";
var err = chmod(filename, permissions:mode_t);
if err != 0 then halt("chmod error");
