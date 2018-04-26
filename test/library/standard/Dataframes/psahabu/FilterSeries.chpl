use Dataframes;

var I = new TypedIndex(["A", "B", "C", "D", "E"]);

var oneDigit = new TypedSeries([1, 2, 3, 4, 5], I);
var ltThree = oneDigit < 3;

writeln(oneDigit);
writeln(ltThree);
