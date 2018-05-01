use Dataframes;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];
var V = [true, false, true, false, true];

var letters = new TypedSeries(A, new TypedIndex(I), V);

writeln();
for t in letters do
  writeln(t);

writeln();
for t in letters.items(string) do
  writeln(t);

writeln();
for t in letters.fast() do
  writeln(t);

writeln();
for t in letters.items_fast(string) do
  writeln(t);

writeln();
for t in letters._these() do
  writeln(t);

writeln();
for t in letters._items(string) do
  writeln(t);
