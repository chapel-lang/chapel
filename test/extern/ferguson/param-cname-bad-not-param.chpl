proc getExportedProcName() {
  return "test";
}

export (getExportedProcName()) proc foo() {
  writeln("in foo");
}

foo();
