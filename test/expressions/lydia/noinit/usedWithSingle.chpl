var sing$: single int = noinit;
var sing2$: single bool = noinit;

begin {
  waitOn();
}
sing$ = 4;
sing2$;
writeln("Task 2 done!");
writeln(sing$.readFF());
writeln(sing2$.readFF());

proc waitOn() {
  sing$; // waits until the single variable is full
  writeln("Task 1 finishing");
  sing2$ = true;
}