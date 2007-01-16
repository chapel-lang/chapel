use Time;

def one() {
  writeln("one");
}

def two() {
  writeln("two");
}

def three() {
  writeln("three");
}

serial (2==3) {
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
