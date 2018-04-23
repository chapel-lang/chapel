use Dataframes;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];
var letters: TypedSeries(string) = new TypedSeries(A, I);

writeln(letters);
writeln();
writeln("contains A: " + letters.indexContains("A"));
writeln("contains F: " + letters.indexContains("F"));
