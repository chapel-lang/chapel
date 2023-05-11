// ChapelNumLocales.chpl
//
module ChapelNumLocales {
  private use ChapelBase;

  extern proc chpl_comm_default_num_locales(): int;

  extern proc printf(x...);
  printf("In personal NumLocales module\n");

  //
  // the number of locales on which to run the program
  //
  config const numLocales = chpl_comm_default_num_locales();

  // this procedure is a helper for tests warnUnstableInt and warnUnstableBoth
  // the pragma and unstable attribute would both trigger warnings if this
  // replacement module was not properly marked as internal by the compiler
  pragma "always RVF"
  @unstable
  proc testNumLocalsUnstableProc(doWrite:bool) {
    if doWrite then
      writeln("In my personal NumLocales - unstable proc");
  }
}
