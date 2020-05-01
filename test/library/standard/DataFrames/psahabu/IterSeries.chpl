use DataFrames;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];

var letters = new owned TypedSeries(A, new shared TypedIndex(I));

for i in letters.idx!.these(string) do
  writeln(i);

writeln();
for i in letters.idx!.items(string) do
  writeln(i);

writeln();
for t in letters do
  writeln(t);

writeln();
for t in letters.items(string) do
  writeln(t);
