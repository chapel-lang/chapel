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
  // empty
}
