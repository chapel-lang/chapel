var sync$: sync int = noinit;
var sync2$: sync bool = noinit;

begin {
  waitOn();
}
sync$ = 4;
writeln(sync2$);


proc waitOn() {
  writeln(sync$); // waits until the sync variable is full
  sync2$ = true;
}