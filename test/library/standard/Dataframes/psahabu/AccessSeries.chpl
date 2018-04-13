use Dataframes;

var I = ["A", "B", "C", "D", "E"];
var A = ["a", "b", "c", "d", "e"];

var letters: TypedSeries(string) = new TypedSeries(I, A);

writeln("A: " + letters["A"]);
writeln("C: " + letters["C"]);
writeln("E: " + letters["E"]);
writeln();
writeln("1: " + letters.at(1));
writeln("3: " + letters.at(3));
writeln("5: " + letters.at(5));
