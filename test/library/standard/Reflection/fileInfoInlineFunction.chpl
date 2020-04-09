inline proc foo() {
  writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), " ",
          Reflection.getModuleName(), ".", Reflection.getRoutineName());
}

proc main {
  writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), " ",
          Reflection.getModuleName(), ".", Reflection.getRoutineName());
  foo();
}

use Reflection only ;
