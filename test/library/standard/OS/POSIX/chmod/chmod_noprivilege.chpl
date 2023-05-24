use OS.POSIX;

// Test when the user tries to modify a file or a directory without privilege
// Should throw PermissionError assuming user cannot modify root folder
config const permissions = 0o777: c_int;
var dirname = "/";
var structStat: struct_stat;
var err = stat(c_ptrToConst_helper(dirname), c_ptrTo(structStat));
if err != 0 then halt("error with stat");

const original = structStat.st_mode;

err = chmod(c_ptrToConst_helper(dirname), permissions: mode_t);
if err != 0 then halt("error with chmod");

// if succeeds then set it back to what it was
err = chmod(c_ptrToConst_helper(dirname), original);
if err != 0 then halt("error with second chmod");
