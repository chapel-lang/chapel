var s, t: sync bool;

begin {
  s.writeEF(true);
}
s.readFE();

begin {
  s.writeEF(true);
}
begin {
  s.readFE();
  t.writeEF(true);
}
t.readFE();

writeln("finished");
