var sync$: sync int = noinit;
var sync2$: sync bool = noinit;

begin {
  waitOn();
}
sync$ = 4;
writeln(sync2$.readFE());


proc waitOn() {
  writeln(sync$.readFE()); // waits until the sync variable is full
  sync2$ = true;
}