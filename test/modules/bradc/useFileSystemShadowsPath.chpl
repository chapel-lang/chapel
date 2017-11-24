proc bar(path: string) {
  writeln("In bar, path is: ", path);
}

proc foo(path: string) {
  use FileSystem;  // commenting this out makes this test work

  bar(path);
}

foo(".");
writeln("Done!");
