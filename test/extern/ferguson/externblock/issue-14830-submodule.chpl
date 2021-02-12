module Program {

  module C {
    extern {
      #include "issue-14830.h"
      static int foo(int x) { return x + 1; }
      struct mystruct obj;
    }
    proc tryMe() {
      writeln("in tryMe obj.a is ", obj.a);
      return foo(3);
    }
  }

  writeln("C.tryMe() ", C.tryMe());
  writeln("C.foo(3)  ", C.foo(3));
  writeln("C.obj.a   ", C.obj.a);
}
