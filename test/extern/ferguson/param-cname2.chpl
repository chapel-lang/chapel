proc getExportedProcName() param {
  return "portedproc";
}
proc getExternProcName() param {
  return "ternproc";
}


export "ex"+getExportedProcName() proc foo() {
  writeln("in foo");
}

extern "ex"+getExternProcName() proc bar();

foo();
bar();
