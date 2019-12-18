record R { var x: int = 42; }

var globalRecord:R;
f();
globalRecord = new R(2);

proc f() {
  writeln(globalRecord);
}
