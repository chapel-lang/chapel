use Dataframes;

var I = new TypedIndex(["A", "B", "C", "D", "E"]);

var oneDigit: TypedSeries(int) = new TypedSeries([1, 2, 3, 4, 5], I);
var twoDigit: TypedSeries(int) = new TypedSeries([10, 20, 30, 40, 50], I);
var doubledDigit = oneDigit.add(twoDigit, string);

writeln("addends:");
writeln(oneDigit);
writeln(twoDigit);

writeln("\nsum:");
writeln(doubledDigit);

var I2 = new TypedIndex(["A", "B", "C"]);
var I3 = new TypedIndex(["B", "C", "D", "E"]);
var X: TypedSeries(int) = new TypedSeries([0, 1, 2], I2);
var Y: TypedSeries(int) = new TypedSeries([10, 20, 0, 0], I3);
var XY = X.add(Y, string);

writeln("\naddends:");
writeln(X);
writeln(Y);

writeln("\nsum:");
writeln(XY);

var A: TypedSeries(string) = new TypedSeries(["hello ", "my ", "name", "is", "brad"]);
var B: TypedSeries(string) = new TypedSeries(["world", "real"]);
var AB = A.add(B);

writeln("\naddends:");
writeln(A);
writeln(B);

writeln("\nsum:");
writeln(AB);
