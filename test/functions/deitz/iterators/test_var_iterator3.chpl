var x: 5*int;

iter xs ref {
  for param i in 1..5 do
    yield x(i);
}

writeln(x);

xs = 2;

writeln(x);
