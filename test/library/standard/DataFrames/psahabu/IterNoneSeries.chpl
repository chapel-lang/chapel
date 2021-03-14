use DataFrames;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];
var V = [true, false, true, false, true];

var letters = new owned TypedSeries(A, new shared TypedIndex(I), V);

writeln("these():");
for t in letters do
  writeln(t);

writeln();
writeln("items():");
for t in letters.items(string) do
  writeln(t);

writeln();
writeln("fast():");
for t in letters.fast() do
  writeln(t);

writeln();
writeln("items_fast():");
for t in letters.items_fast(string) do
  writeln(t);

writeln();
writeln("_these():");
for t in letters._these() do
  writeln(t);

writeln();
writeln("_items():");
for t in letters._items(string) do
  writeln(t);
