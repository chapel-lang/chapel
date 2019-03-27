use FileSystem;
// Test to ensure that calling chmod on a file that doesn't exist will break
// as expected.
var filename = "nonexistent";
chmod(filename, 0o777);
