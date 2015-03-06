var finished: sync bool;
begin {
  // This aborts the test if it takes too long.
  use Time;
  sleep(5);
  if (!finished.isFull) then
    halt("Timed out");
}


type t = sync bool;
writeln("about to create sync tuple");
var a: (t, t, t);

writeln("created it");
a(1) = true;
writeln("set sync tuple field");
writeln("sync value is: ", a(1).readFE());
writeln("read it");
finished = true;
