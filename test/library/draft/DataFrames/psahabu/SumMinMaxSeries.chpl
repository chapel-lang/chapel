use DataFrames;

var I = new shared TypedIndex(["A", "B", "C", "D", "E"]);
var V1 = [true, false, true, false, true];
var V2 = [false, true, false, true, false];

var noNone = new owned TypedSeries([1, 2, 3, 4, 5], I);
var someNone = new owned TypedSeries([1, 2, 3, 4, 5], I, V1);
var moreNone = new owned TypedSeries([10, 20, 30, 40, 50], I, V2);

writeln("noNone:");
writeln(noNone);
writeln();
writeln("someNone:");
writeln(someNone);
writeln();
writeln("moreNone:");
writeln(moreNone);

writeln();
writeln("noNone.sum(): ", noNone.sum());
writeln();
writeln("someNone.sum(): ", someNone.sum());
writeln();
writeln("moreNone.sum(): ", moreNone.sum());

writeln();
writeln("noNone.min(): ", noNone.min());
writeln();
writeln("someNone.min(): ", someNone.min());
writeln();
writeln("moreNone.min(): ", moreNone.min());

writeln();
writeln("noNone.max(): ", noNone.max());
writeln();
writeln("someNone.max(): ", someNone.max());
writeln();
writeln("moreNone.max(): ", moreNone.max());
