module ExternFoo {
  extern {
    #include "shadow.h"
  }

  writeln("In ExternFoo, foo(1) returns ", foo(1));
}

module LocalFoo {
  proc foo(x:int):string {
    return (x + 2):string;
  }

  writeln("In LocalFoo, foo(1) returns ", foo(1));
}

module shadow {
  use ExternFoo;
  use LocalFoo;
  proc main() {
    writeln("With both visible, foo(1) returns ", foo(1));
  }
}
