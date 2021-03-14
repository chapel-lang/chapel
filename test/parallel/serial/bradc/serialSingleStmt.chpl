use Time;

serial (1==1) do begin foo();
writeln("Second");

proc foo() {
  sleep(2);
  writeln("First");
}
