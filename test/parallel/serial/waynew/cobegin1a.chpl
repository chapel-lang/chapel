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
    begin { s3; three();}
    begin { s2; two(); s3 = 1; }
    begin { s1; one(); s2 = 1; }
    begin { writeln("zero"); s1 = 1; }
  }
}


