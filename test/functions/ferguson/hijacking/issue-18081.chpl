module G {
  proc foo(param arg:int, val:int) {
    bar(arg);
  }
  proc foo(param arg:int, val:real) {
    bar(arg);
  }
}
module M1 {
  use G;
  proc bar(arg: int) {
    writeln("In M1.bar ", arg);
  }
  proc runM1() {
    writeln("In runM1");
    foo(1, 20);
  }
}

module M2 {
  use G;
  proc bar(arg: int) {
    writeln("In M2.bar ", arg);
  }
  proc runM2() {
    writeln("In runM2");
    foo(1, 20.0);
  }
}

module User {
  use M1, M2;
  proc main() {
    runM1();
    runM2();
  }
}
