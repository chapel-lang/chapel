use FileSystem;

// Tests the removal of a large and complicated directory tree.
var dirToRemove = "removeMe";
if (!exists(dirToRemove)) {
  copyTree("copyToKeep", dirToRemove);
}

writeln("Pre-removal, removeMe/ exists: ", exists(dirToRemove));
rmTree(dirToRemove);
writeln("Post-removal, removeMe/ exists: ", exists(dirToRemove));
