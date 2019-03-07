proc getExportedProcName() param {
  return "exportedproc";
}
proc getExternProcName() param {
  return "externproc";
}


export getExportedProcName() proc foo() {
  writeln("in foo");
}

extern getExternProcName() proc bar();

foo();
bar();
