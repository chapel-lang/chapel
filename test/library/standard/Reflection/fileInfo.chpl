//use Reflection;

module M {
  proc main {
    writeln(Reflection.fileName, ":", Reflection.lineNumber, ": ",
            Reflection.moduleName, ".", Reflection.functionName);
  }
}
