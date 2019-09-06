proc main {
  proc foo() {
    proc bar() {
      writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), " ",
              Reflection.getModuleName(), ".", Reflection.getRoutineName());
    }

    writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), " ",
            Reflection.getModuleName(), ".", Reflection.getRoutineName());
    bar();
  }

  writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), " ",
          Reflection.getModuleName(), ".", Reflection.getRoutineName());
  foo();
}

use Reflection only ;
