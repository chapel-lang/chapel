enum testCases {
  compare,
  compareParam,
  min,
  maxParam
}
config param testCase = testCases.compare;

if testCase == testCases.compare {
  const x: complex = 1.0 + 2.0i;
  const y: complex = 4.0 + 3.0i;
  writeln("x < y: ", x < y);
} else if testCase == testCases.compareParam {
  param x: complex = 1.0 + 2.0i;
  param y: complex = 4.0 + 3.0i;
  param z = x >= y;
  writeln("x >= y: ", z);
} else if testCase == testCases.min {
  const x: complex = 1.0 + 2.0i;
  const y: complex = 4.0 + 3.0i;
  writeln("min(x, y): ", min(x, y));
} else if testCase == testCases.maxParam {
  param x: complex = 1.0 + 2.0i;
  param y: complex = 4.0 + 3.0i;
  param z = max(x, y);
  writeln("max(x, y): ", z);
}
