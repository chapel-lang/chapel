proc warn() {
  compilerWarning("warn");
}
proc err() {
  compilerError("err");
}
proc errWarn() {
  compilerError("err1");
  compilerWarning("warn1");
}

errWarn();
errWarn();
errWarn();

warn();
warn();

err();
err();

errWarn();
