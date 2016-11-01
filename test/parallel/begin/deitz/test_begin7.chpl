var s, t: sync bool;

begin {
  s = true;
}
s;

begin {
  s = true;
}
begin {
  s;
  t = true;
}
t;

writeln("finished");
