// Bug from issue #13739.  Fixed by #14814.
proc test() throws {
  writeln("test");
  for i in 1..1 {
    try {
      return;
    }
  }
}
test();
