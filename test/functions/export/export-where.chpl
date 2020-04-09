// This test is intended to check the error message
// produced for export functions with where clauses
// that aren't supported.

export proc exportedFunction() where true {
  writeln("hi");
}

exportedFunction();
