use FileSystem;

/* Test that copying a directory containing root-owned filed works */

config const path = '/usr/include/curses';

proc main() {
  FileSystem.copyTree(path, './usercopy');
}
