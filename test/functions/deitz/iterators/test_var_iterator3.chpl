var x: 5*int;

iter xs ref {
  for param i in 0..4 do
    yield x(i);
}

writeln(x);

xs = 2;

writeln(x);
