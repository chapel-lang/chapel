//
// Histogram Solution 1
//
// Compute the histogram of an array of random numbers.
//

// import standard modules to generate random number and use timers
use Random, Time;

// configuration constants
config const printRandomNumbers: bool = true, // print random numbers to screen
             printHistogram: bool = true,     // print histogram to screen
             numNumbers: int = 8,             // number of random numbers
             numBuckets: int = 10;            // number of histogram buckets

// seed the random stream with something reproducable?
config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.currentTime else 314159265;

// global variables
var X: [1..numNumbers] real, // array of random numbers
    Y: [1..numBuckets] int,  // histogram
    timer: Timer;            // computation timer

// output startup message
writeln("Running Histogram Example");
writeln(" Number of Random Numbers = ", format("########", numNumbers));
writeln(" Number of Buckets        = ", format("########", numBuckets));
writeln();

// fill array with random numbers (using standard Random module)
fillRandom(X, seed);

// output array of random numbers
if printRandomNumbers then
  writeln("Random Numbers\n\n", X, "\n");

// compute histogram
timer.start();
computeHistogram(X, Y);
timer.stop();

// verify number of items in histogram is equal to number of random
// numbers and output timing results
if + reduce Y != numNumbers then
  halt("Number of items in histogram does not match number of random numbers");
writeln("Histogram computed in ", timer.elapsed(), " seconds\n");

// output histogram
if printHistogram then
  outputHistogram(Y);

proc computeHistogram(X: [] real, Y: [] int) {
  for x in X do
    Y(1 + (x / (1.0 / numBuckets)): int) += 1;
}

// outputHistogram: output histogram array
proc outputHistogram(Y: [] int) {
  var bucketMax = max reduce Y;
  var rowSize = divceil(bucketMax,10);
  var numRows = divceil(bucketMax, rowSize);
  for i in 1..numRows by -1 {
    write(" ");
    for j in 1..numBuckets do
      write(if Y(j)/rowSize >= i then "X" else " ");
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
    writeln("  ",
            format("%0.2dr", (j-1)*(1.0/numBuckets)),
            " - ",
            format("%0.2dr", j*(1.0/numBuckets)),
            ": ",
            Y(j));
  }
}
