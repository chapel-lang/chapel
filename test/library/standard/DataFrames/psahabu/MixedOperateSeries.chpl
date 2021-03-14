use DataFrames;

var I = new shared TypedIndex(["A", "B", "C", "D", "E"]);

var oneDigit = new owned TypedSeries([1, 2, 3, 4, 5], I);
var twoDigit = new owned TypedSeries([10, 20, 30, 40, 50], I);

writeln("oneDigit:");
writeln(oneDigit);
writeln("\ntwoDigit:");
writeln(twoDigit);

writeln("\noneDigit + twoDigit:");
writeln(oneDigit + twoDigit);

writeln("\noneDigit < 3");
writeln(oneDigit[oneDigit < 3]);

/*
var bools = new TypedSeries([true, false, true, false, true], I);
writeln(bools);
 */
