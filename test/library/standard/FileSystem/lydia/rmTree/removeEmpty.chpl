use FileSystem;

var emptyName = "empty_dir";
if (!exists(emptyName)) {
  mkdir(emptyName);
} else if (isFile(emptyName)) {
  writeln("removing file named ", emptyName);
  remove(emptyName);
  mkdir(emptyName);
}

writeln("Before rmTree: exists = ", exists(emptyName));
rmTree(emptyName);
writeln("After rmTree: exists = ", exists(emptyName));
