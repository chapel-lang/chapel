use FileSystem;

/* Test that copying a directory containing root-owned filed works */

config const path = '/usr/include/curses';

proc main() {
  try {
    FileSystem.copyTree(path, './usercopy', metadata=true);
  } catch err {
    writeln(err);
  }
}
