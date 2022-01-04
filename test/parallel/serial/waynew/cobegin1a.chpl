use Time;

var s1, s2, s3: sync int;

proc one() {
  writeln("one");
}

proc two() {
  writeln("two");
}

proc three() {
  writeln("three");
}

serial (2==9) {
  cobegin {
    begin { s3.readFE(); three();}
    begin { s2.readFE(); two(); s3.writeEF(1); }
    begin { s1.readFE(); one(); s2.writeEF(1); }
    begin { writeln("zero"); s1.writeEF(1); }
  }
}


