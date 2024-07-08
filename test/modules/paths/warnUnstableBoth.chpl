module M {
  use Path;
  use ChapelNumLocales;

  writeln("inside M");

  testNumLocalsUnstableProc(true);
  testPathUnstableProc(true);

}
