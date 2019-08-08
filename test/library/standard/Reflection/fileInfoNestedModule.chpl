module Outer {
  proc foo() {
    writeln(Reflection.fileName, ":", Reflection.lineNumber, " ",
            Reflection.moduleName, ".", Reflection.functionName);
  }
  module Inner {
    proc main {
      writeln(Reflection.fileName, ":", Reflection.lineNumber, " ",
              Reflection.moduleName, ".", Reflection.functionName);
      foo();
      bar();
    }
    proc bar() {
      writeln(Reflection.fileName, ":", Reflection.lineNumber, " ",
              Reflection.moduleName, ".", Reflection.functionName);
    }
  }
}
