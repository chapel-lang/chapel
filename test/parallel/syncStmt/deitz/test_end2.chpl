use Time;

proc foo() {
  begin {
    sleep(1);
    writeln("1. hello, world");
  }
}

sync {
  foo();
}

writeln("2. hello, world");
