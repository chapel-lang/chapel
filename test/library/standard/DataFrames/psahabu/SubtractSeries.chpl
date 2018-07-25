use DataFrames;

var I = new unmanaged TypedIndex(["A", "B", "C", "D", "E"]);

var oneDigit = new unmanaged TypedSeries([1, 2, 3, 4, 5], I);
var twoDigit = new unmanaged TypedSeries([11, 22, 33, 44, 55], I);

writeln("terms:");
writeln(oneDigit);
writeln(twoDigit);

writeln("\ndifference:");
writeln(twoDigit - oneDigit);

var X = new unmanaged TypedSeries([5, 1, 2], new unmanaged TypedIndex(["A", "B", "C"]));
var Y = new unmanaged TypedSeries([10, 20, 6, 7], new unmanaged TypedIndex(["B", "C", "D", "E"]));

writeln("\nterms:");
writeln(Y);
writeln(X);

writeln("\ndifference:");
writeln(X - Y);
