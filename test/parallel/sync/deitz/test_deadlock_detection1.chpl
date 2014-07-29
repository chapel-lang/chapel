use Time;

var s: sync int = 1;

s.readFE();
begin {
  sleep(1);
}
s.readFE(); // should deadlock here
