/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Brad Chamberlain
   derived from the GNU C version by Аноним Легионов and Jeremy Zerfas
     as well as previous Chapel versions by Casey Battaglino, Kyle Brady,
     and Preston Sahabu.
*/

use IO;

config param lineLength = 60;

config type randType = uint(32);  // type to use for random numbers

config const n = 1000,            // the length of the generated strings
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

param IM = 139968,         // parameters for random number generation
      IA = 3877,
      IC = 29573,

      // Sequence to be repeated
      ALU = b"GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGGGAGGCCGAGGCGGGCGGA" +
            b"TCACCTGAGGTCAGGAGTTCGAGACCAGCCTGGCCAACATGGTGAAACCCCGTCTCTACT" +
            b"AAAAATACAAAAATTAGCCGGGCGTGGTGGCGCGCGCCTGTAATCCCAGCTACTCGGGAG" +
            b"GCTGAGGCAGGAGAATCGCTTGAACCCGGGAGGCGGAGGTTGCAGTGAGCCGAGATCGCG" +
            b"CCACTGCACTCCAGCCTGGGCGACAGAGCGAGACTCCGTCTCAAAAA";

//
// Probability tables for sequences to be randomly generated
//
const IUB = [(b("a"), 0.27), (b("c"), 0.12), (b("g"), 0.12), (b("t"), 0.27),
             (b("B"), 0.02), (b("D"), 0.02), (b("H"), 0.02), (b("K"), 0.02),
             (b("M"), 0.02), (b("N"), 0.02), (b("R"), 0.02), (b("S"), 0.02),
             (b("V"), 0.02), (b("W"), 0.02), (b("Y"), 0.02)],

      HomoSapiens = [(b("a"), 0.3029549426680),
                     (b("c"), 0.1979883004921),
                     (b("g"), 0.1975473066391),
                     (b("t"), 0.3015094502008)],

      newline = b("\n"),   // newline's byte value

      // Redefine stdout to use lock-free binary I/O
      stdout = openfd(1).writer(kind=iokind.native, locking=false);

proc main() {
  repeatMake(">ONE Homo sapiens alu", ALU, 2*n);
  randomMake(">TWO IUB ambiguity codes", IUB, 3*n);
  randomMake(">THREE Homo sapiens frequency", HomoSapiens, 5*n);
}

//
// Repeat 'alu' to generate a sequence of length 'n'
//
proc repeatMake(desc, alu, n) {
  stdout.writeln(desc);

  const r = alu.size,
        s = alu + alu;

  for i in 0..<n by lineLength {
    const lo = i % r,
          len = min(lineLength, n-i);
    stdout.write(s[lo..#len], newline);
  }
}

//
// Use 'nuclInfo's probability distribution to generate a random
// sequence of length 'n'
//
proc randomMake(desc, nuclInfo: [?nuclInds], n) {
  stdout.writeln(desc);

  // compute the cumulative probabilities of the nucleotides
  var cumulProb: [nuclInds] randType,
      p = 0.0;
  for (cp, (_,prob)) in zip(cumulProb, nuclInfo) {
    p += prob;
    cp = 1 + (p*IM): randType;
  }

  // guard when tasks can access the random numbers or output stream
  var randGo, outGo: [0..<numTasks] atomic int;

  // create tasks to pipeline the RNG, computation, and output
  coforall tid in 0..<numTasks {
    const chunkSize = lineLength*blockSize,
          nextTid = (tid + 1) % numTasks;

    var myBuff: [0..<(lineLength+1)*blockSize] uint(8),
        myRands: [0..chunkSize] randType;

    // iterate over 0..<n in a round-robin fashion across tasks
    for i in tid*chunkSize..<n by numTasks*chunkSize {
      const nBytes = min(chunkSize, n-i);

      // Get 'nBytes' random numbers in a coordinated manner
      randGo[tid].waitFor(i);
      getRands(nBytes, myRands);
      randGo[nextTid].write(i+chunkSize);

      // Compute 'nBytes' nucleotides and store in 'myBuff'
      var col = 0,
          off = 0;

      for r in myRands[..<nBytes] {
        var nid = 0;
        for p in cumulProb do
          nid += (r >= p);
        const (nucl,_) = nuclInfo[nid];

        myBuff[off] = nucl;
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
      stdout.write(myBuff[..<off]);
      outGo[nextTid].write(i+chunkSize);
    }
  }
}

proc b(s) {
  return s.toByte();
}

//
// Deterministic random number generator
// (lastRand really wants to be a local static...)
//
var lastRand = 42: randType;

proc getRands(n, arr) {
  for i in 0..<n {
    lastRand = (lastRand * IA + IC) % IM;
    arr[i] = lastRand;
  }
}
