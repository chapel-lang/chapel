use Dataframes;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];

var letters = new TypedSeries(A, new TypedIndex(I));

writeln("A: " + letters["A"]);
writeln("C: " + letters["C"]);
writeln("E: " + letters["E"]);
writeln();
writeln("1: " + letters.at(1));
writeln("3: " + letters.at(3));
writeln("5: " + letters.at(5));
