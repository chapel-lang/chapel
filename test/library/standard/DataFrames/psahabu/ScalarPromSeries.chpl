use DataFrames;

var oneDigit = new TypedSeries([1, 2, 3, 4, 5],
                               new TypedIndex(["A", "B", "C", "D", "E"]));
var n = 12;

writeln("series:");
writeln(oneDigit);
writeln("scalar:");
writeln(n);

writeln();
writeln("sum:");
writeln(oneDigit + n);
writeln("difference:");
writeln(oneDigit - n);
writeln("product:");
writeln(oneDigit * n);
