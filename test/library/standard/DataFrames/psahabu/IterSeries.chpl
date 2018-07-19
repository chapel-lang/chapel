use DataFrames;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];

var letters = new unmanaged TypedSeries(A, new unmanaged TypedIndex(I));

for i in letters.idx:unmanaged TypedIndex(string) do
  writeln(i);

writeln();
for i in (letters.idx:unmanaged TypedIndex(string)).items() do
  writeln(i);

writeln();
for t in letters do
  writeln(t);

writeln();
for t in letters.items(string) do
  writeln(t);
