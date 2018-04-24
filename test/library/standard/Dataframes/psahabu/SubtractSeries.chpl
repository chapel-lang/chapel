use Dataframes;

var I = new TypedIndex(["A", "B", "C", "D", "E"]);

var oneDigit: TypedSeries(int) = new TypedSeries([1, 2, 3, 4, 5], I);
var twoDigit: TypedSeries(int) = new TypedSeries([11, 22, 33, 44, 55], I);
var doubledDigit = twoDigit.subtr(oneDigit, string);

writeln("terms:");
writeln(oneDigit);
writeln(twoDigit);

writeln("\ndifference:");
writeln(doubledDigit);

var I2 = new TypedIndex(["A", "B", "C"]);
var I3 = new TypedIndex(["B", "C", "D", "E"]);
var X: TypedSeries(int) = new TypedSeries([5, 1, 2], I2);
var Y: TypedSeries(int) = new TypedSeries([10, 20, 6, 7], I3);
var XY = Y.subtr(X, string);

writeln("\nterms:");
writeln(Y);
writeln(X);

writeln("\ndifference:");
writeln(XY);
