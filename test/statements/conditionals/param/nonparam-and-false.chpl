
proc doRADOpt {
  compilerWarning("in doRADOpt");
  return true;
}

param stridable = true;

proc CyclicArr_dsiAccess() {
  if doRADOpt && !stridable {
    compilerError("got !stridable");
  }
}

CyclicArr_dsiAccess();
compilerError("success");
