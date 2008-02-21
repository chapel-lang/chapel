use Time;

def foo() {
  begin {
    sleep(1);
    writeln("1. hello, world");
  }
}

end {
  foo();
}

writeln("2. hello, world");
