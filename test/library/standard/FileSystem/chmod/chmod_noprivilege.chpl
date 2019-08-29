use FileSystem;
// Test when the user tries to modify a file or a directory without privilege
// Should throw PermissionError assuming user cannot modify root folder
config const permissions = 0o777;
var dirname = "/";
const original = getMode(dirname);
chmod(dirname, permissions);
// if succeeds then set it back to what it was
chmod(dirname, original);
