use DataFrames;

var I = new shared TypedIndex(["A", "B", "C", "D", "E"]);
var V1 = [true, false, true, false, true];
var V2 = [false, true, false, true, false];

var oneDigit = new owned TypedSeries([1, 2, 3, 4, 5], I, V1);
var twoDigit = new owned TypedSeries([10, 20, 30, 40, 50], I, V1);
var twoDigitInv = new owned TypedSeries([10, 20, 30, 40, 50], I, V2);

writeln("oneDigit:");
writeln(oneDigit);

writeln("\ntwoDigit:");
writeln(twoDigit);

writeln("\ntwoDigitInv:");
writeln(twoDigitInv);

writeln("\noneDigit + twoDigit:");
writeln(oneDigit + twoDigit);

writeln("\noneDigit - twoDigit:");
writeln(oneDigit - twoDigit);

writeln("\noneDigit + twoDigitInv:");
writeln(oneDigit + twoDigitInv);

writeln("\noneDigit - twoDigitInv:");
writeln(oneDigit - twoDigitInv);
