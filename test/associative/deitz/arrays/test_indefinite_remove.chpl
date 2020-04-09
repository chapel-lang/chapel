var D: domain(int);

for j in 1..3 {
  for i in 1..8*j do
    D += i;
  for i in D do
    D -= i;
  writeln(D);
}
