var t = (1, 2.3, "hi");

for param i in 0..<3 do
  writeln(t(i));

param lo = 0,
      hi = 3;

for param i in lo..<hi do
  writeln(t(i));
