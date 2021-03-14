var s, t: sync bool;

begin {
  s.writeEF(true);
}
s.readFE();

begin {
  s.readFE();
  t.writeEF(true);
}
begin {
  s.writeEF(true);
}
t.readFE();

writeln("finished");
