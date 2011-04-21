use Time;

var x : volatile int;

begin {
  while (x==0) {};
  writeln(x);
}
sleep(1);
x=1;
