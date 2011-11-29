use Time;

var s, t, u, v: sync bool;

begin {
  s = true;
}
s;

sleep(1);

begin {
  u;
  v = true;
}
begin {
  t;
  u = true;
}
begin {
  s;
  t = true;
}
begin {
  s = true;
}
v;

writeln("finished");
