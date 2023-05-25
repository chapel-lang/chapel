use Time;

const CORRECTNESS_GROUND_TRUTH = 2.88666e+05;

config const perfTest = false;

proc summarize(s: stopwatch, data: [] real) {
  if perfTest then writeln("elapsed: ", s.elapsed());
  const mean = + reduce data / (data.size: real);
  const stdDev = sqrt(+ reduce ((data - mean) ** 2) / data.size: real);

  // writef("stdDev: %.5dr, gt: %.5dr\n", stdDev, CORRECTNESS_GROUND_TRUTH);

  writeln("correct: ", abs(stdDev - CORRECTNESS_GROUND_TRUTH) < 0.5);
}
