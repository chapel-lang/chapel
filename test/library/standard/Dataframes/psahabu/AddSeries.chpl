use Dataframes;

var I = ["A", "B", "C", "D", "E"];

var oneDigit: TypedSeries(int) = new TypedSeries(I, [1, 2, 3, 4, 5]);
var twoDigit: TypedSeries(int) = new TypedSeries(I, [10, 20, 30, 40, 50]);
var doubledDigit = oneDigit.add(twoDigit, string);

writeln("addends:");
writeln(oneDigit);
writeln(twoDigit);

writeln("\nsum:");
writeln(doubledDigit);

var I2 = ["A", "B", "C"];
var I3 = ["B", "C", "D", "E"];
var X: TypedSeries(int) = new TypedSeries(I2, [0, 1, 2]);
var Y: TypedSeries(int) = new TypedSeries(I3, [10, 20, 0, 0]);
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
