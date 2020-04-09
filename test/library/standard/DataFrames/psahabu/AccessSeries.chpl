use DataFrames;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];

var letters = new owned TypedSeries(A, new shared TypedIndex(I));

writeln("A: " + letters["A"]);
writeln("C: " + letters["C"]);
writeln("E: " + letters["E"]);
writeln();
writeln("0: " + letters.at(0));
writeln("2: " + letters.at(2));
writeln("4: " + letters.at(4));
