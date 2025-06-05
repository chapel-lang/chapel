use Time;

config const N = 2;

var s : sync nothing;
begin {
  writeln("Here I am in a separate task");
  sleep(N);
  s.writeEF(none);
}

writeln("Here I am before the barrier");
s.readFE();
writeln("Here I am after the barrier");
