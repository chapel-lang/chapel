use FileSystem;

// Tests moving a directory when the intended destination exists and is a
// file (expecting error)
var srcDir = "foo";
var destFile = "existed.txt";

if (!exists(srcDir) || !exists(destFile) || !isFile(destFile)) {
  writeln("Set up for this test is not correct!  Please make sure that ");
  writeln(srcDir + " exists, " + destFile + " exists, and that ");
  writeln(destFile + " is a file, not a directory");
}

moveDir(srcDir, destFile);
