use Time;

config param perfTest = false;
const SOLUTION = if perfTest then 2.886724e+06 else 2.88666e+05;

proc summarize(s: stopwatch, data: [] real) {
  if perfTest then writeln("elapsed: ", s.elapsed());
  const mean = + reduce data / (data.size: real);
  const stdDev = sqrt(+ reduce ((data - mean) ** 2) / data.size: real);

  // writef("stdDev: %.6dr, gt: %.6dr\n", stdDev, SOLUTION);
  writeln((abs(stdDev - SOLUTION) / SOLUTION) < 1e-4);
}
