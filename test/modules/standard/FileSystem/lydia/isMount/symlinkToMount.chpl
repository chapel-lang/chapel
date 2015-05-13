use FileSystem;

symlink("/", "symlinkToRoot");
if (isMount("symlinkToRoot")) {
  writeln("Uh oh, symlinks aren't mounts!");
  writeln("Had trouble with one in the current working directory");
} else {
  writeln("Good, symlinks aren't considered mounts!");
}
remove("symlinkToRoot");

symlink("/", "notAMonkey/symlinkToRoot");
if (isMount("notAMonkey/symlinkToRoot")) {
  writeln("Uh oh, symlinks aren't mounts!");
  writeln("Had trouble with one in another directory other than cwd");
} else {
  writeln("Good, symlinks aren't considered mounts!");
}
remove("notAMonkey/symlinkToRoot");
