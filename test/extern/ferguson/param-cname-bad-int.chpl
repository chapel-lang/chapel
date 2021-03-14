proc getExportedProcName() param {
  return 5;
}

export (getExportedProcName()) proc foo() {
  writeln("in foo");
}

foo();
