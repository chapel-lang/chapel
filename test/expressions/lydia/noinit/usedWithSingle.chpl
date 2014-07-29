var sing$: single int = noinit;
var sing2$: single bool = noinit;

begin {
  waitOn();
}
sing$ = 4;
sing2$.readFF();
writeln("Task 2 done!");
writeln(sing$);
writeln(sing2$);

proc waitOn() {
  sing$.readFF(); // waits until the single variable is full
  writeln("Task 1 finishing");
  sing2$.readFF() = true;
}