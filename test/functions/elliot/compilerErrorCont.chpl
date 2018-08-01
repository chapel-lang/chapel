proc warn() {
  compilerWarning("warn");
}
proc errCont() {
  compilerErrorCont("errCont");
}
proc errContWarn() {
  compilerErrorCont("errCont1");
  compilerWarning("warn1");
}

errContWarn();
errContWarn();
errContWarn();

warn();
warn();

errCont();
errCont();

errContWarn();
