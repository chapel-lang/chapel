module Path {
  extern proc printf(x...);
  printf("In my personal copy of Path\n");

  // this procedure is a helper for tests warnUnstableStd and warnUnstableBoth
  // the pragma and unstable attribute would both trigger warnings if this
  // replacement module was not properly marked as internal by the compiler
  pragma "always RVF"
  @unstable
  proc testPathUnstableProc(doWrite:bool) {
    if doWrite then
      writeln("In personal Path - unstable proc");
  }
}
