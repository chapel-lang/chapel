use FileSystem;
// This test demonstrates that a chdir call on a single locale will not affect
// any other locales in the program, even if called before the first access of
// other locales.
here.chdir("foo/bar/"); // not the launch directory
coforall loc in Locales do on loc do
  writeln(cwd());
// With 4 locales, expect 3 to output the launch directory and 1 to output
