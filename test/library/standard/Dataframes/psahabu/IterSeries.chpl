use Dataframes;

var I = ["A", "B", "C", "D", "E"];
var A = ["a", "b", "c", "d", "e"];

var letters: TypedSeries(string, string) = new TypedSeries(I, A);

for i in letters.idx.set do
  writeln(i);

writeln();
for i in letters.idx.items() do
  writeln(i);

writeln();
for t in letters do
  writeln(t);

writeln();
for t in letters.items() do
  writeln(t);
