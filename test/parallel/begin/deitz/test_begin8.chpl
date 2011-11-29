use Time;

var s, t: sync bool;

begin {
  s = true;
}
s;

sleep(1);

begin {
  s;
  t = true;
}
begin {
  s = true;
}
t;

writeln("finished");
