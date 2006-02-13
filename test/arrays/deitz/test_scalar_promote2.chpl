function foo(i : integer) {
  writeln(i);
}

var s : [1..4] integer;

for i in 1..4 do
  s(i) = i;

foo(s);
