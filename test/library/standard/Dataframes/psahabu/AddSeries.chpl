use Dataframes;

var I = new TypedIndex(["A", "B", "C", "D", "E"]);

var oneDigit = new TypedSeries([1, 2, 3, 4, 5], I);
var twoDigit = new TypedSeries([10, 20, 30, 40, 50], I);

writeln("addends:");
writeln(oneDigit);
writeln(twoDigit);

writeln("\nsum:");
writeln(oneDigit + twoDigit);

var X = new TypedSeries([0, 1, 2], new TypedIndex(["A", "B", "C"]));
var Y = new TypedSeries([10, 20, 0, 0], new TypedIndex(["B", "C", "D", "E"]));

writeln("\naddends:");
writeln(X);
writeln(Y);

writeln("\nsum:");
writeln(X + Y);

var A = new TypedSeries(["hello ", "my ", "name", "is", "brad"]);
var B = new TypedSeries(["world", "real"]);

writeln("\naddends:");
writeln(A);
writeln(B);

writeln("\nsum:");
writeln(A + B);
