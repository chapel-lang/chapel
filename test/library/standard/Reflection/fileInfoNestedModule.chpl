module Outer {
  proc foo() {
    writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), " ",
            Reflection.getModuleName(), ".", Reflection.getRoutineName());
  }
  module Inner {
    proc main {
      writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), " ",
              Reflection.getModuleName(), ".", Reflection.getRoutineName());
      foo();
      bar();
    }
    proc bar() {
      writeln(Reflection.getFileName(), ":", Reflection.getLineNumber(), " ",
              Reflection.getModuleName(), ".", Reflection.getRoutineName());
    }
    import Reflection;
    import Outer.foo;
  }
  use Reflection only ;
}
