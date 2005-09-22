function foo(inout t : integer) {
  writeln(t);
  t += 1;
}

function bar(inout t : integer) {
  foo(t);
  writeln(t);
  t += 1;
}

var t : integer = 1;
bar(t);
writeln(t);
