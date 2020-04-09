proc foo(inout t : int) {
  writeln(t);
  t += 1;
}

proc bar(inout t : int) {
  foo(t);
  writeln(t);
  t += 1;
}

var t : int = 1;
bar(t);
writeln(t);
