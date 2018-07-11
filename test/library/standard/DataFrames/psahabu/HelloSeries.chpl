use DataFrames;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];

var just_letters = new unmanaged TypedSeries(A);
var letters = new unmanaged TypedSeries(A, new unmanaged TypedIndex(I));

writeln(just_letters);
writeln(letters);
