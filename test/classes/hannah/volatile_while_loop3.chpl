use Time;

proc testit(in x: volatile int) {
  begin {
    while (x==0) {};
    writeln(x);
  }
  sleep(1);
  x=1;
}

var x: volatile int;

testit(x);
