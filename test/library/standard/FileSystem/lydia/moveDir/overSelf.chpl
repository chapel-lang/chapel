use FileSystem;

// Tests calling moveDir with the same source and destination.  The expected
// behavior is failure.
var dirName = "foo";
moveDir(dirName, dirName);
