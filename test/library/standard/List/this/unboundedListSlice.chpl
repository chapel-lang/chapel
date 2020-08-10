use List;
var l: list(string) = ["a", "b", "c"];

for li in l[l.indices.low+1..] do
  writeln(li);

for li in l[..<l.indices.high] do
  writeln(li);

for li in l[..] do
  writeln(li);
