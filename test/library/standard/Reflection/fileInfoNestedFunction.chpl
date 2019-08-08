proc main {
  proc foo() {
    proc bar() {
      writeln(Reflection.fileName, ":", Reflection.lineNumber, " ",
              Reflection.moduleName, ".", Reflection.functionName);
    }

    writeln(Reflection.fileName, ":", Reflection.lineNumber, " ",
            Reflection.moduleName, ".", Reflection.functionName);
    bar();
  }

  writeln(Reflection.fileName, ":", Reflection.lineNumber, " ",
          Reflection.moduleName, ".", Reflection.functionName);
  foo();
}
