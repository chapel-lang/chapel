/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Brad Chamberlain
   derived from the GNU C version by Аноним Легионов and Jeremy Zerfas
     as well as previous Chapel versions by Casey Battaglino, Kyle Brady,
     and Preston Sahabu.
*/

use IO;

config const n = 1000,            // the length of the generated strings
             lineLength = 60,     // the number of columns in the output
             blockSize = 1024,    // the parallelization granularity
             numTasks = min(4, here.maxTaskPar);  // how many tasks to use?
//
// the computational pipeline has 3 distinct stages, so ideally, we'd
// like to use 3 tasks.  However, there is one stage which does not
// require any coordination and it tends to be the slowest stage, so
// we could have multiple tasks working on it simultaneously.  In
// practice, though, that phase is not that much slower than the sum
// of the other two, and using too many tasks can just add overhead
// that isn't helpful.  So we go with 4 tasks to pick up some slack,
// and because it seems to work best on all the machine we've tried in
// practice.  If the locale can't support that much parallelism, we'll
// use a number of tasks equal to its maximum degree of task
// parallelism to avoid oversubscription.
//

config type randType = uint(32);  // type to use for random numbers

config param IM = 139968,         // parameters for random number generation
             IA = 3877,
             IC = 29573,
             seed: randType = 42;

//
// Nucleotide definitions
//
enum nucleotide {
  A = "A".toByte(), C = "C".toByte(), G = "G".toByte(), T = "T".toByte(),
  a = "a".toByte(), c = "c".toByte(), g = "g".toByte(), t = "t".toByte(),
  B = "B".toByte(), D = "D".toByte(), H = "H".toByte(), K = "K".toByte(),
  M = "M".toByte(), N = "N".toByte(), R = "R".toByte(), S = "S".toByte(),
  V = "V".toByte(), W = "W".toByte(), Y = "Y".toByte()
}
use nucleotide;

//
// Sequence to be repeated
//
const ALU: [0..286] nucleotide = [
  G, G, C, C, G, G, G, C, G, C, G, G, T, G, G, C, T, C, A, C,
  G, C, C, T, G, T, A, A, T, C, C, C, A, G, C, A, C, T, T, T,
  G, G, G, A, G, G, C, C, G, A, G, G, C, G, G, G, C, G, G, A,
  T, C, A, C, C, T, G, A, G, G, T, C, A, G, G, A, G, T, T, C,
  G, A, G, A, C, C, A, G, C, C, T, G, G, C, C, A, A, C, A, T,
  G, G, T, G, A, A, A, C, C, C, C, G, T, C, T, C, T, A, C, T,
  A, A, A, A, A, T, A, C, A, A, A, A, A, T, T, A, G, C, C, G,
  G, G, C, G, T, G, G, T, G, G, C, G, C, G, C, G, C, C, T, G,
  T, A, A, T, C, C, C, A, G, C, T, A, C, T, C, G, G, G, A, G,
  G, C, T, G, A, G, G, C, A, G, G, A, G, A, A, T, C, G, C, T,
  T, G, A, A, C, C, C, G, G, G, A, G, G, C, G, G, A, G, G, T,
  T, G, C, A, G, T, G, A, G, C, C, G, A, G, A, T, C, G, C, G,
  C, C, A, C, T, G, C, A, C, T, C, C, A, G, C, C, T, G, G, G,
  C, G, A, C, A, G, A, G, C, G, A, G, A, C, T, C, C, G, T, C,
  T, C, A, A, A, A, A
];

//
// Index aliases for use with (nucleotide, probability) tuples
//
param nucl = 0,
      prob = 1;

//
// Probability tables for sequences to be randomly generated
//
const IUB = [(a, 0.27), (c, 0.12), (g, 0.12), (t, 0.27),
             (B, 0.02), (D, 0.02), (H, 0.02), (K, 0.02),
             (M, 0.02), (N, 0.02), (R, 0.02), (S, 0.02),
             (V, 0.02), (W, 0.02), (Y, 0.02)];

const HomoSapiens = [(a, 0.3029549426680),
                     (c, 0.1979883004921),
                     (g, 0.1975473066391),
                     (t, 0.3015094502008)];

proc main() {
  repeatMake(">ONE Homo sapiens alu\n", ALU, 2*n);
  randomMake(">TWO IUB ambiguity codes\n", IUB, 3*n);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, 5*n);
}

//
// Redefine stdout to use lock-free binary I/O and capture a newline
//
const stdout = openfd(1).writer(kind=iokind.native, locking=false);
param newline = "\n".toByte();

//
// Repeat 'alu' to generate a sequence of length 'n'
//
proc repeatMake(desc, alu, n) {
  stdout.write(desc);

  const r = alu.size,
        s = [i in 0..(r+lineLength)] alu[i % r]: int(8);

  for i in 0..n by lineLength {
    const lo = i % r,
          len = min(lineLength, n-i);
    stdout.write(s[lo..#len], newline);
  }
}

//
// Use 'nuclInfo's probability distribution to generate a random
// sequence of length 'n'
//
proc randomMake(desc, nuclInfo: [?nuclSpace], n) {
  stdout.write(desc);

  // compute the cumulative probabilities of the nucleotides
  var cumulProb: [nuclSpace] randType,
      p = 0.0;
  for i in nuclSpace {
    p += nuclInfo[i](prob);
    cumulProb[i] = 1 + (p*IM):randType;
  }

  // guard when tasks can access the random numbers or output stream
  var randGo, outGo: [0..#numTasks] atomic int;

  randGo.write(0);
  outGo.write(0);

  // create tasks to pipeline the RNG, computation, and output
  coforall tid in 0..#numTasks {
    const chunkSize = lineLength*blockSize,
          nextTid = (tid + 1) % numTasks;

    var myBuff: [0..#(lineLength+1)*blockSize] int(8),
        myRands: [0..chunkSize] randType;

    // iterate over 0..n-1 in a round-robin fashion across tasks
    for i in tid*chunkSize..n-1 by numTasks*chunkSize {
      const nBytes = min(chunkSize, n-i);

      // Get 'nBytes' random numbers in a coordinated manner
      randGo[tid].waitFor(i);
      getRands(nBytes, myRands);
      randGo[nextTid].write(i+chunkSize);

      // Compute 'nBytes' nucleotides and store in 'myBuff'
      var col = 0,
          off = 0;

      for j in 0..#nBytes {
        const r = myRands[j];
        var nid = 0;
        for k in nuclSpace do
          if r >= cumulProb[k] then
            nid += 1;

        myBuff[off] = nuclInfo[nid](nucl):int(8);
        off += 1;
        col += 1;

        if (col == lineLength) {
          col = 0;
          myBuff[off] = newline;
          off += 1;
        }
      }
      if (col != 0) {
        myBuff[off] = newline;
        off += 1;
      }

      // Write the output in a coordinated manner
      outGo[tid].waitFor(i);
      stdout.write(myBuff[0..#off]);
      outGo[nextTid].write(i+chunkSize);
    }
  }
}

//
// Deterministic random number generator
// (lastRand really wants to be a local static...)
//
var lastRand = seed;

proc getRands(n, arr) {
  for i in 0..#n {
    lastRand = (lastRand * IA + IC) % IM;
    arr[i] = lastRand;
  }
}
