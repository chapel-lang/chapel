// This module has two globals, so that one of them can be on the list of
// excluded symbols
module M1 {
  var a = 17;

  var c = 14.0;
}

// This module has only one global.  When used at the same time as M1, an
// except on M1 will allow the program to compile, but it won't work otherwise.
module M2 {
  var a = 32;
}

// This module provides one global and publicly uses M1 without an except
// clause.  Thus, if a module foo uses it and M1, all of M1's symbols will be
// available to foo, even if foo's use of M1 includes an except.
//
// Note: this does not change the behavior in the case where another used
// module also defines the 'except'ed symbol, merely the behavior where such
// a variable is not also defined elsewhere.  Private uses would prevent
// the current behavior
module M3 {
  public use M1;

  var b = true;
}
