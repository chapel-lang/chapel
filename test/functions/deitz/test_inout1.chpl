function foo(inout t : int) {
  writeln(t);
  t += 1;
}

function bar(inout t : int) {
  foo(t);
  writeln(t);
  t += 1;
}

var t : int = 1;
bar(t);
writeln(t);
