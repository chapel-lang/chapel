/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Brad Chamberlain and Engin Kayraklioglu
   derived from the C gcc #9 version by Drake Diedrich
   and the Chapel #5 version
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
const IUB = [("a", 0.27), ("c", 0.12), ("g", 0.12), ("t", 0.27),
             ("b", 0.02), ("D", 0.02), ("H", 0.02), ("K", 0.02),
             ("M", 0.02), ("N", 0.02), ("R", 0.02), ("S", 0.02),
             ("V", 0.02), ("W", 0.02), ("Y", 0.02)],

      HomoSapiens = [("a", 0.3029549426680),
                     ("c", 0.1979883004921),
                     ("g", 0.1975473066391),
                     ("t", 0.3015094502008)],

      newline = "\n".toByte(),   // newline's byte value

      // Redefine stdout to use lock-free binary I/O
      stdout = openfd(1).writer(kind=iokind.native, locking=false);


proc main() {
  stdout.writeln(">ONE Homo sapiens alu");
  repeatMake(ALU, 2*n);

  stdout.writeln(">TWO IUB ambiguity codes");
  randomMake(IUB, 3*n);

  stdout.writeln(">THREE Homo sapiens frequency");
  randomMake(HomoSapiens, 5*n);
}


//
// Repeat 'alu' to generate a sequence of length 'n'
//
proc repeatMake(param alu, n) {
  param len = alu.size,
        alu2 = alu + alu,
        buffLen = len * bytesPerLine;

  var buffer: bytes;
  for i in 0..<len {
    buffer += alu2[(i*lineLen)%len..#lineLen];
  }

  const wholeBuffers = n / (len*lineLen);
  for i in 0..<wholeBuffers {
    stdout.write(buffer);
  }

  var extra = n - wholeBuffers*len*lineLen;
  extra += extra/lineLen;
  stdout.write(buffer[..<extra]);

  if n % lineLen != 0 {
    stdout.write("\n");
  }
}

//
// Use 'nuclInfo's probability distribution to generate a random
// sequence of length 'n'
//
proc randomMake(nuclInfo, n) {
  var hash: [0..<IM] uint(8),
      (ch, prob) = nuclInfo[0],
      sum = prob;

  var j = 0;
  for i in 0..<IM {
    if i:real / IM >= sum {
      j += 1;
      (ch, prob) = nuclInfo[j];
      sum += prob;
    }
    hash[i] = ch.toByte();
  }

  param buffSize = buffLines * bytesPerLine;

  var numLines = n / lineLen,
      numBuffs = numLines / buffLines,
      buffer: [0..<buffSize] uint(8);

  // add linefeeds
  for i in lineLen..<buffSize by bytesPerLine {
    buffer[i] = newline;
  }

  // write out most of the data as full buffers
  for 0..<numBuffs {
    for j in 0..<buffLines {
      for k in 0..<lineLen {
        buffer[j*bytesPerLine + k] = hash[getNextRand()];
      }
    }
    stdout.write(buffer);
  }

  // compute number of complete lines remaining and fill them in
  numLines -= numBuffs * buffLines;

  for j in 0..<numLines {
    for k in 0..<lineLen {
      buffer[j*bytesPerLine + k] = hash[getNextRand()];
    }
  }

  // compute number of extra characters and fill them in
  var extra = n % lineLen,
      offset = numLines * bytesPerLine;

  for k in 0..<extra {
    buffer[offset + k] = hash[getNextRand()];
  }

  // add a final linefeed if needed
  if (extra != 0) {
    buffer[offset + extra] = newline;
    extra += 1;
  }

  stdout.write(buffer[0..<offset+extra]);
}

//
// Deterministic random number generator
//
var lastRand = 42: uint(32);

inline proc getNextRand() {
  lastRand = (lastRand * IA + IC) % IM;
  return lastRand;
}
