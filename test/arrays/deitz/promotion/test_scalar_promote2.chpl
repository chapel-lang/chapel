proc foo(i : int) {
  writeln(i);
}

var s : [1..4] int;

for i in 1..4 do
  s(i) = i;

serial {
  foo(s);
}
