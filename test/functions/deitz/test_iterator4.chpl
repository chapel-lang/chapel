var x = ((1, 2), (3, 4), (5, 6));

iter x1s(x: 3*(int,int)) {
  for param i in 1..3 do
    yield x(i)(1);
}

for i in x1s(x) do
  writeln(i);
