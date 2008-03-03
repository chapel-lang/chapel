use Time;

sync {
  begin {
    sleep(1);
    writeln("1. hello, world");
  }
}

writeln("2. hello, world");
