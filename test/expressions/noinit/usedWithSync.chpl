var sync1: sync int = noinit;
var sync2: sync bool = noinit;

begin {
  waitOn();
}
sync1.writeEF(4);
writeln(sync2.readFE());


proc waitOn() {
  writeln(sync1.readFE()); // waits until the sync1 variable is full
  sync2.writeEF(true);
}
