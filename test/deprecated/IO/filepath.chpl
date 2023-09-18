use IO, Path;

// this test is here to ensure that users get a warning when they try to use the
// 'filePathAbsolute' config param which is now inactive.

var f = open("filepath.chpl", ioMode.r);
writeln(f.path == absPath(f));
