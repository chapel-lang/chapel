
proc getBool(): bool {
  writeln("getBool called");
  return true;
}

writeln("TRUE && non-param:");
if true && getBool() {
  compilerWarning("resolve true branch");
  writeln("passed if");
} else {
  compilerWarning("resolve false branch");
  writeln("shouldn't reach");
}

writeln("FALSE && non-param:");
if false && getBool() {
  compilerError("shouldn't reach");
} else {
  compilerWarning("resolve false branch");
  writeln("passed else");
}

writeln("non-param && TRUE:");
if getBool() && true {
  compilerWarning("resolve true branch");
  writeln("passed if");
} else {
  compilerWarning("resolve false branch");
  writeln("shouldn't reach");
}

writeln("non-param && FALSE:");
if getBool() && false {
  compilerWarning("resolve true branch");
  writeln("shouldn't reach");
} else {
  compilerWarning("resolve false branch");
  writeln("passed else");
}
