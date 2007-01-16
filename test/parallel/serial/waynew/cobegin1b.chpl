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

serial (9==9) {
  cobegin {
    begin {sleep(3); three();}
    begin {sleep(2); two();}
    begin {sleep(1); one();}
    begin writeln("zero");
  }
}


