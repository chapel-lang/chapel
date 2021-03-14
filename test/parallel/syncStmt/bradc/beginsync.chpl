use Time;

var readyToGoOn$: sync bool;

begin sync {
  sleep(1);
  writeln("A print me second");
  readyToGoOn$.writeEF(true);
}
writeln("A print me first");

readyToGoOn$.readFE();

sync begin {
  sleep(1);
  writeln("B print me first");
}
writeln("B print me second");
