use Time;

var s, t: sync bool;

begin {
  s = true;
}
s;

sleep(1);

begin {
  s = true;
}
begin {
  s;
  t = true;
}
t;

writeln("finished");
