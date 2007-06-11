var x: 5*int;

iterator xs var {
  for param i in 1..5 do
    yield x(i);
}

writeln(x);

for (e,i) in (xs,1..5) do
  e = i;

writeln(x);
