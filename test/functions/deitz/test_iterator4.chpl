var x = ((1, 2), (3, 4), (5, 6));

iter x1s(x: 3*(int,int)) {
  for param i in 0..2 do
    yield x(i)(0);
}

for i in x1s(x) do
  writeln(i);
