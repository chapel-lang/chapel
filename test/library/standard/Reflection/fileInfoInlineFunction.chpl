inline proc foo() {
  writeln(Reflection.fileName, ":", Reflection.lineNumber, " ",
          Reflection.moduleName, ".", Reflection.functionName);
}

proc main {
  writeln(Reflection.fileName, ":", Reflection.lineNumber, " ",
          Reflection.moduleName, ".", Reflection.functionName);
  foo();
}
