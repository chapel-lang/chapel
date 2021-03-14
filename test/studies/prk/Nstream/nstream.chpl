/*
   Chapel's parallel Nstream implementation

   Contributed by Engin Kayraklioglu (GWU)
*/

use Time;
use BlockDist;

param PRKVERSION = "2.17";

config param useBlockDist = false;

config const iterations = 100,
             length = 100,
             correctness = false, //being run in start_test
             validate = true;

config var SCALAR = 3.0;

//
// Process and test input configs
//
if iterations < 1 then
  halt("ERROR: iterations must be >= 1: ", iterations);

if length < 0 then
  halt("ERROR: vector length must be >= 1: ", length);

// Domains
const space = {0.. # length};
const vectorDom = space dmapped if useBlockDist then
                            new dmap(new Block(boundingBox=space)) else
                            defaultDist;

var A: [vectorDom] real,
    B: [vectorDom] real,
    C: [vectorDom] real;

if !correctness {
  writeln("Parallel Research Kernels version ", PRKVERSION);
  writeln("Serial stream triad: A = B + SCALAR*C");
  writeln("Max parallelism        = ", here.maxTaskPar);
  writeln("Vector length          = ", length);
  writeln("Number of iterations   = ", iterations);
}

// initialization
A = 0.0;
B = 2.0;
C = 2.0;

var timer = new Timer();

//
// Main loop
//
for iteration in 0..iterations {
  if iteration == 1 then
    timer.start(); //Start timer after a warmup lap

  A += B+SCALAR*C;
}

// Timings
timer.stop();
var avgTime = timer.elapsed() / iterations;
timer.clear();

//
// Analyze and output results
//
if validate {
  config const epsilon = 1.e-8;

  var aj=0.0, bj=2.0, cj=2.0;
  for 0..iterations do
    aj += bj+SCALAR*cj;

  aj = aj * length:real;

  var asum = + reduce A;

  if abs(aj-asum)/asum <= epsilon then
    writeln("Validation successful");
  else
    halt("VALIDATION FAILED! Reference checksum = ", aj,
                           " Checksum = ", asum);
}

if !correctness {
  const nBytes = 4 * 8 * length;
  writeln("Rate (MB/s): ", 1.0E-06*nBytes/avgTime,
         " Avg time (s): ",avgTime);
}
