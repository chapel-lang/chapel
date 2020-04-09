use Time;

var s: sync int = 1;

s;
begin {
  sleep(1);
}
s; // should deadlock here
