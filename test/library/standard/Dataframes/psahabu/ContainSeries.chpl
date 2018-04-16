use Dataframes;

var I = ["A", "B", "C", "D", "E"];
var A = ["a", "b", "c", "d", "e"];
var letters: TypedSeries(string) = new TypedSeries(I, A);

writeln("contains A: " + letters.indexContains("A"));
writeln("contains F: " + letters.indexContains("F"));
writeln(letters.idx.getType());
