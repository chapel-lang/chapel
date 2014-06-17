var x : (int, string, real, bool);

x = (2, "three", 4.0, false);

writeln(x);

for param i in 1..4 do
  writeln(x(i));
