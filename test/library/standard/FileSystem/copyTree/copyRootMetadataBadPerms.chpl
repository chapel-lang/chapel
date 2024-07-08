use FileSystem;

/* Test that copying a directory containing root-owned files with metadata
   fails */

config const path = '/usr/include/curses';

proc main() {
  try {
    FileSystem.copyTree(path, './usercopy2', metadata=true);
  } catch err {
    writeln(err);
  }
}
