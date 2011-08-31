use Time;

proc testit(x: volatile int) {
  while (x==0) {};
  writeln(x);
}

var x: volatile int;

begin testit(x);
sleep(1);
x=1;
