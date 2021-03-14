use FileSystem;
// Test what happens when the value for permissions is invalid
// Should raise an IllegalArgumentError
config const permissions = 0o777;
var filename = "file";
chmod(filename, permissions);
