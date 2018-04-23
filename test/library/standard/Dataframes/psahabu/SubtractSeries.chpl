use Dataframes;

var I = ["A", "B", "C", "D", "E"];

var oneDigit: TypedSeries(int) = new TypedSeries(I, [1, 2, 3, 4, 5]);
var twoDigit: TypedSeries(int) = new TypedSeries(I, [11, 22, 33, 44, 55]);
var doubledDigit = twoDigit.subtr(oneDigit, string);

writeln("terms:");
writeln(oneDigit);
writeln(twoDigit);

writeln("\ndifference:");
writeln(doubledDigit);

var I2 = ["A", "B", "C"];
var I3 = ["B", "C", "D", "E"];
var X: TypedSeries(int) = new TypedSeries(I2, [5, 1, 2]);
var Y: TypedSeries(int) = new TypedSeries(I3, [10, 20, 6, 7]);
var XY = Y.subtr(X, string);

writeln("\nterms:");
writeln(X);
writeln(Y);

writeln("\ndifference:");
writeln(XY);
