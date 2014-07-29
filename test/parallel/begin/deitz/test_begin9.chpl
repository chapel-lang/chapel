use Time;

var s, t, u, v: sync bool;

begin {
  s = true;
}
s.readFE();

sleep(1);

begin {
  u.readFE();
  v = true;
}
begin {
  t.readFE();
  u = true;
}
begin {
  s.readFE();
  t = true;
}
begin {
  s = true;
}
v.readFE();

writeln("finished");
