use DataFrames;

var I = new shared TypedIndex(["A", "B", "C", "D", "E"]);

var oneDigit = new owned TypedSeries([1, 2, 3, 4, 5], I);
var twoDigit = new owned TypedSeries([10, 20, 30, 40, 50], I);

writeln("addends:");
writeln(oneDigit);
writeln(twoDigit);

writeln("\nsum:");
writeln(oneDigit + twoDigit);

var X = new owned TypedSeries([0, 1, 2], new shared TypedIndex(["A", "B", "C"]));
var Y = new owned TypedSeries([10, 20, 0, 0], new shared TypedIndex(["B", "C", "D", "E"]));

writeln("\naddends:");
writeln(X);
writeln(Y);

writeln("\nsum:");
writeln(X + Y);

var A = new owned TypedSeries(["hello ", "my ", "name", "is", "brad"]);
var B = new owned TypedSeries(["world", "real"]);

writeln("\naddends:");
writeln(A);
writeln(B);

writeln("\nsum:");
writeln(A + B);
