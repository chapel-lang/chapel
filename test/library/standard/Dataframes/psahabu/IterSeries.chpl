use Dataframes;

var I = ["A", "B", "C", "D", "E"];
var A = ["a", "b", "c", "d", "e"];

var letters: TypedSeries(string) = new TypedSeries(I, A);

for i in letters.idx:TypedIndex(string) do
  writeln(i);

writeln();
for i in (letters.idx:TypedIndex(string)).items() do
  writeln(i);

writeln();
for t in letters do
  writeln(t);

writeln();
for t in letters.items(string) do
  writeln(t);
