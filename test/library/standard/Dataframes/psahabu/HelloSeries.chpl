use Dataframes;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];

var just_letters = new TypedSeries(A);
var letters = new TypedSeries(A, new TypedIndex(I));

writeln(just_letters);
writeln(letters);
