use Time;

var s, t: sync bool;

begin {
  s = true;
}
s.readFE();

sleep(1);

begin {
  s;
  t = true;
}
begin {
  s = true;
}
t.readFE();

writeln("finished");
