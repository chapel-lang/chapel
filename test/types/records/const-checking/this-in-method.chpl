record R { var x: int; }
proc R.mod()   { x = 5; }
proc R.nomod() { writeln(x); }

var r1: R;
r1.nomod();
r1.mod();

const r2: R;
r2.nomod();
r2.mod();    // compiler must report error here; the previous calls are fine
writeln("should not see this");
