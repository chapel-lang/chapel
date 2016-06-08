use FileSystem;

// Tests behavior of sameFile when provided a broken link.
var filename = "moveMe.txt";
var newName = "moved.txt";
var linkname = "linkToIt";

// moved.txt will be saved in the repository.  If something happens that
// interrupts the execution of this test, this check should enable the test
// to work correctly next time (by not needing to move newName over).
if (!exists(filename) && exists(newName)) {
  copy(newName, filename);
  remove(newName);
}
// If we've already run this test once, linkname will still be around, so
// we need to remove it.
if (exists(linkname)) {
  remove(linkname);
}

symlink(filename, linkname);
copy(filename, newName);
remove(filename);

writeln(sameFile(newName, linkname));
// Expected to throw "no such file" error, as python does.

remove(linkname); // Don't expect to reach this line.
