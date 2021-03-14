use DataFrames;

var I = new shared TypedIndex(["A", "B", "C", "D", "E"]);
var V1 = [true, false, true, false, true];

var oneDigit = new owned TypedSeries([1, 2, 3, 4, 5], I, V1);
var twoDigit = new owned TypedSeries([10, 20, 30, 40, 50], I, V1);

writeln("oneDigit:");
writeln(oneDigit);

writeln("\ntwoDigit:");
writeln(twoDigit);

writeln("\noneDigit < 3");
writeln(oneDigit[oneDigit < 3]);

writeln("\ntwoDigit > 30");
writeln(twoDigit[twoDigit > 30]);
