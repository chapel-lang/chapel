/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Brad Chamberlain
   derived from the Chapel #5 version
   and the C gcc #9 version by Drake Diedrich
*/

use IO;

config param lineLen = 60,              // length of each generated line
             bytesPerLine = lineLen+1,  // ...plus the linefeed
             buffLines = 100;           // number of lines to buffer

config const n = 1000;                  // length of the generated sequences

param IM = 139968,                      // values for random number generation
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
  // TODO: move writes of descriptions here?
  repeatMake(">ONE Homo sapiens alu", ALU, 2*n);
  randomMake(">TWO IUB ambiguity codes", IUB, 3*n);
  randomMake(">THREE Homo sapiens frequency", HomoSapiens, 5*n);
}

//
// Repeat 'alu' to generate a sequence of length 'n'
//
proc repeatMake(desc, param alu, n) {
  stdout.writeln(desc);

  param len = alu.size,
        alu2 = alu + alu,
        buffLen = len*bytesPerLine;

  var buffer: [0..<buffLen] uint(8);
  for i in 0..buffLen by bytesPerLine do
    buffer[i..#lineLen] = alu2[i%len..#lineLen];

  const wholeBuffers = n / (len*lineLen);
  for i in 0..<wholeBuffers do
    stdout.write(buffer);

  var extra = n - wholeBuffers * len * lineLen;
  extra += extra / lineLen;
  stdout.write(buffer[..<extra]);

  if n % lineLen != 0 then
    stdout.write("\n");
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

  param buffSize = buffLines * bytesPerLine;

  var numLines = n/lineLen,
      numBuffs = numLines/buffLines,
      buffer: [0..<buffSize] uint(8);

  // add linefeeds
  for i in lineLen..<buffSize by lineLen+1 do  // TODO: try forall?
    buffer[i] = newline;

  // write out most of the data in full buffers
  for 0..<numBuffs {
    for j in 0..<buffLines do
      for k in 0..<lineLen do
        buffer[j*bytesPerLine + k] = hash[getNextRand()];
    stdout.write(buffer);
  }

  // compute number of complete lines remaining and fill them in
  numLines -= numBuffs * buffLines;

  for j in 0..<numLines do
    for k in 0..<lineLen do
      buffer[j*bytesPerLine + k] = hash[getNextRand()];

  // compute number of extra characters and fill them in
  var extra = n % lineLen,
      offset = numLines * bytesPerLine;

  for k in 0..<extra do
    buffer[offset + k] = hash[getNextRand()];

  // add a final linefeed if needed
  if (extra != 0) {
    buffer[offset + extra] = newline;
    extra += 1;
  }

  stdout.write(buffer[0..<offset+extra]);
}

// Utility to convert single-character strings to bytes
proc b(s) {
  return s.toByte();
}

//
// Deterministic random number generator
// (lastRand really wants to be a local static...)
//
var lastRand = 42: uint(32);

inline proc getNextRand() {
  lastRand = (lastRand * IA + IC) % IM;
  return lastRand;
}
