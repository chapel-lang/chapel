module M {
  use Reflection only ;

  proc main {
    writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), ": ",
            Reflection.getModuleName(), ".", Reflection.getRoutineName());
  }
}
