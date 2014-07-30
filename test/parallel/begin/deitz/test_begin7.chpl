use Time;

var s, t: sync bool;

begin {
  s = true;
}
s.readFE();

sleep(1);

begin {
  s = true;
}
begin {
  s.readFE();
  t = true;
}
t.readFE();

writeln("finished");
