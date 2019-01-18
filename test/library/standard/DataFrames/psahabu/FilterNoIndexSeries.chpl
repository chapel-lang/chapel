use DataFrames;

var noIndex = new owned TypedSeries([1, 2, 3, 4, 5]);
writeln("noIndex:");
writeln(noIndex);
writeln();

proc filterTest(title, filter1, filter2) {
  assert(filter1: string == filter2: string);
  writeln(title + ":");
  writeln(filter1);
  writeln("filtered(" + title + "):");
  writeln(noIndex[filter1]);
  writeln();
}

filterTest("less than 3", noIndex < 3, 3 > noIndex);
filterTest("greater than 3", noIndex > 3, 3 < noIndex);
filterTest("equal to 3", noIndex == 3, 3 == noIndex);
filterTest("less than or equal to 3", noIndex <= 3, 3 >= noIndex);
filterTest("greater than or equal to 3", noIndex >= 3, 3 <= noIndex);
