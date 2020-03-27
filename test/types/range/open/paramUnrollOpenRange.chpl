var t = (1, 2.3, "hi");

for param i in 1..<4 do
  writeln(t(i));

param lo = 1,
      hi = 4;

for param i in lo..<hi do
  writeln(t(i));
