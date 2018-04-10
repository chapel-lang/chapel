use Dataframes;

var I = ["A", "B", "C", "D", "E"];
var A = ["a", "b", "c", "d", "e"];

var just_letters: TypedSeries(string) = new TypedSeries(A);
var letters: TypedSeries(string) = new TypedSeries(I, A);

writeln(just_letters);
writeln(letters);
