use Dataframes;

var I = ["A", "B", "C", "D", "E"];
var A = ["a", "b", "c", "d", "e"];

var letters: TypedSeries(string) = new TypedSeries(I, A);

for i in letters.idx do
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
