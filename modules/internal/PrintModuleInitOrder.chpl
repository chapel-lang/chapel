//
// This module implements the printModuleInitOrder flag for Chapel
// executables.
//
// If true, it prints the name of the module as it is being
// initialized.  Note that we can't use writeln() here, as the IO
// module has not yet been initialized.
//
pragma "no use ChapelStandard"
pragma "export init"
module PrintModuleInitOrder {
  config const printModuleInitOrder = false;
  pragma "print module init indent level" var moduleInitLevel = 2:int(32);

  //
  // Called by all modules during initialization
  //
  pragma "print module init fn"
  proc printModuleInit(s1: c_string, s2: c_string, len: int) {
    extern proc printf(s1: c_string, len: int(32), s2: c_string);
    if printModuleInitOrder then
      printf(s1, moduleInitLevel+len:int(32)+2:int(32), s2);
  }

  proc initPrint() {
    extern proc printf(s: c_string);
    printf ("Initializing Modules:\n");
  }

  if printModuleInitOrder then initPrint();
}
