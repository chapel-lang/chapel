use DataFrames;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];

var just_letters = new owned TypedSeries(A);
var letters = new owned TypedSeries(A, new shared TypedIndex(I));

writeln(just_letters);
writeln(letters);
