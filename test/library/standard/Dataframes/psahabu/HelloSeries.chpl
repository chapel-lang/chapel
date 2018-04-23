use Dataframes;

var A = ["a", "b", "c", "d", "e"];
var I = ["A", "B", "C", "D", "E"];

var just_letters: TypedSeries(string) = new TypedSeries(A);
var letters: TypedSeries(string) = new TypedSeries(A, I);

writeln(just_letters);
writeln(letters);
