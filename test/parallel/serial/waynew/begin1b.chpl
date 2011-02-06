use Time;

proc one() {
  writeln("one");
}

proc two() {
  writeln("two");
}

proc three() {
  writeln("three");
}

serial (2==2) {
  begin {
    sleep(3);
    three();
  }

  begin {
    sleep(2);
    two();
  }

  begin {
    sleep(1);
    one();
  }
}

writeln("zero");
