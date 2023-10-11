//
// Histogram: serial version
//
// Computes the histogram of an array of random numbers.
//
// Oct 7, 2023: copied from chapel/test/exercises/histogram1.chpl and made slight changes

// import standard modules to generate random number and use timers
use Random, Time, Math;

// configuration constants
config const printRandomNumbers: bool = true, // print random numbers to screen
             printHistogram: bool = true,     // print histogram to screen
             numNumbers: int = 8,             // number of random numbers
             numBuckets: int = 10;            // number of histogram buckets

// seed the random stream with something reproducible?
config const useRandomSeed = false,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 314159265;

// global variables
var X: [1..numNumbers] real,            // array of random numbers
    histogram: [1..numBuckets] int,     // histogram
    timer: stopwatch;                   // computation timer

// output startup message
writeln("Running Histogram Example");
writef(" Number of Random Numbers = %{########}\n", numNumbers);
writef(" Number of Buckets        = %{########}\n", numBuckets);
writeln();

// fill array with random numbers (using standard Random module)
fillRandom(X, seed, algorithm=RNG.NPB);

// output array of random numbers
if printRandomNumbers then
  writeln("Random Numbers\n\n", X, "\n");

// compute histogram
timer.start();
computeHistogram(X, histogram);
timer.stop();

// verify number of items in histogram is equal to number of random
// numbers and output timing results
if + reduce histogram != numNumbers then
  halt("Number of items in histogram does not match number of random numbers");
writeln("Histogram computed in ", timer.elapsed(), " seconds\n");

// output histogram
if printHistogram then
  outputHistogram(histogram);

proc computeHistogram(X: [] real, ref histogram: [] int) {
  for x in X do
    histogram[1 + (x / (1.0 / numBuckets)): int] += 1;
}

// outputHistogram: output histogram array
proc outputHistogram(histogram: [] int) {
  var bucketMax = max reduce histogram;
  var rowSize = divCeil(bucketMax,10);
  var numRows = divCeil(bucketMax, rowSize);
  for i in 1..numRows by -1 {
    write(" ");
    for j in 1..numBuckets do
      write(if histogram[j]/rowSize >= i then "X" else " ");
    writeln();
  }
  write("+");
  for j in 1..numBuckets do
    write("-");
  writeln("+");
  if rowSize > 1 then
    writeln(" Each 'X' can represent up to ", rowSize, " numbers.");
  writeln(" Raw Data");
  for j in 1..numBuckets {
    writef("  %0.2dr - %0.2dr: %n\n", (j-1)*(1.0/numBuckets), j*(1.0/numBuckets), histogram[j]);
  }
}
