var s, t: sync bool;

begin {
  s = true;
}
s;

begin {
  s;
  t = true;
}
begin {
  s = true;
}
t;

writeln("finished");
