use DataFrames;

var I = new shared TypedIndex(["A", "B", "C", "D", "E"]);
var initial = new owned TypedSeries([1, 2, 3, 4, 5], I);
writeln("initial:");
writeln(initial);
writeln();

proc filterTest(title, filter1, filter2) {
  assert(filter1: string == filter2: string);
  writeln(title + ":");
  writeln(filter1);
  writeln("filtered(" + title + "):");
  writeln(initial[filter1]);
  writeln();
}

filterTest("less than 3", initial < 3, 3 > initial);
filterTest("greater than 3", initial > 3, 3 < initial);
filterTest("equal to 3", initial == 3, 3 == initial);
filterTest("less than or equal to 3", initial <= 3, 3 >= initial);
filterTest("greater than or equal to 3", initial >= 3, 3 <= initial);
