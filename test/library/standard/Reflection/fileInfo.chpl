module M {
  use Reflection only ;

  proc main {
    writeln(Reflection.fileName, ":", Reflection.lineNumber, ": ",
            Reflection.moduleName, ".", Reflection.functionName);
  }
}
