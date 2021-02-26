
type typeAlias = int;
proc typeAlias.methodTA() {
  compilerWarning("typeAlias.methodTA");
}

proc parenlessFun type return int;
proc parenlessFun.methodPLF() {
  compilerWarning("parenlessFun.methodPLF");
}

proc parenfulFun() type return int;
proc (parenfulFun()).methodPFF() {
  compilerWarning("parenfulFun.methodPFF");
}

1.methodTA();
2.methodPLF();
3.methodPFF();
compilerError("done");
