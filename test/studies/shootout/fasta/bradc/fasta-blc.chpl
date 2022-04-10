/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Brad Chamberlain
   derived from the GNU C version by Аноним Легионов and Jeremy Zerfas
     as well as previous Chapel versions by Casey Battaglino, Kyle Brady,
     and Preston Sahabu.
*/

use IO;

config param lineLen = 60,
             buffLines = 100;

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
proc repeatMake(desc, param alu, n) {
  stdout.writeln(desc);

  param s = alu + alu;

  for i in 0..<n by lineLen {
    const lo = i % alu.size,
          len = min(lineLen, n-i);
    stdout.write(s[lo..#len], newline);
  }
}

//
// Use 'nuclInfo's probability distribution to generate a random
// sequence of length 'n'
//
proc randomMake(desc, nuclInfo, n) {
  stdout.writeln(desc);

  var hash: [0..<IM] uint(8),
      (ch, prob) = nuclInfo[0],
      sum = prob;

  var j = 0;
  for i in 0..<IM {
    if 1.0 * i / IM >= sum {
      j += 1;
      (ch, prob) = nuclInfo[j];
      sum += prob;
    }
    hash[i] = ch;
  }

  param lfLineLen = lineLen + 1,
        buffSize = lfLineLen * buffLines;

  var numLines = n/lineLen,
      numBuffs = numLines/buffLines,
      buffer: [0..<buffSize] uint(8);

  // add linefeeds
  for i in lineLen..<buffSize by lineLen+1 do  // TODO: try forall?
    buffer[i] = newline;

  // write out most of the data in full buffers
  for 0..<numBuffs {
    for j in 0..<buffLines do // TODO: make PARAM?
      for k in 0..<lineLen do // TODO: make PARAM?
        buffer[j*lfLineLen + k] = hash[getNextRand()];
    stdout.write(buffer);
  }

  // fill in any remaining lines
  numLines -= numBuffs * buffLines;

  for j in 0..<numLines do
    for k in 0..<lineLen do
      buffer[j*lfLineLen + k] = hash[getNextRand()];

  // and any extra characters on the last line
  var extra = n % lineLen,
      offset = numLines * lfLineLen;

  for k in 0..<extra do
    buffer[offset + k] = hash[getNextRand()];

  if (extra != 0) {
    buffer[offset + extra] = newline;
    extra += 1;
  }

  stdout.write(buffer[0..<offset+extra]);
}

proc b(s) {
  return s.toByte();
}

//
// Deterministic random number generator
// (lastRand really wants to be a local static...)
//
var lastRand = 42: randType;

inline proc getNextRand() {
  lastRand = (lastRand * IA + IC) % IM;
  return lastRand;
}
