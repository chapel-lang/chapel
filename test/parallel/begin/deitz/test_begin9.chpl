var s, t, u, v: sync bool;

begin {
  s.writeEF(true);
}
s.readFE();

begin {
  u.readFE();
  v.writeEF(true);
}
begin {
  t.readFE();
  u.writeEF(true);
}
begin {
  s.readFE();
  t.writeEF(true);
}
begin {
  s.writeEF(true);
}
v.readFE();

writeln("finished");
