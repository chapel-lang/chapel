var s, t, u, v: sync bool;

begin {
  s = true;
}
s;

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
