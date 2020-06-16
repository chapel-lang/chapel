var x: 5*int;

iter xs ref {
  for param i in 0..4 do
    yield x(i);
}

writeln(x);

for (e,i) in zip(xs,1..5) do
  e = i;

writeln(x);
