/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Preston Sahabu
   derived from the Chapel fastaredux version by Casey Battaglino et al.
            and the GNU C version by Paul Hsieh
*/

config const n = 1000,   // controls the length of the generated strings
             lineLength = 60;

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
// Probability tables for sequences to be randomly generated
//
var IUB = [(a, 0.27), (c, 0.12), (g, 0.12), (t, 0.27),
           (B, 0.02), (D, 0.02), (H, 0.02), (K, 0.02),
           (M, 0.02), (N, 0.02), (R, 0.02), (S, 0.02),
           (V, 0.02), (W, 0.02), (Y, 0.02)];

var HomoSapiens = [(a, 0.3029549426680),
                   (c, 0.1979883004921),
                   (g, 0.1975473066391),
                   (t, 0.3015094502008)];


proc main() {
  sumProbs(IUB);
  sumProbs(HomoSapiens);
  repeatMake(">ONE Homo sapiens alu\n", ALU, n * 2);
  randomMake(">TWO IUB ambiguity codes\n", IUB, n * 3);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, n * 5);
}

//
// Scan the alphabets' probabilities to compute cut-offs
//
proc sumProbs(alphabet: []) {
  var p = 0.0;
  for letter in alphabet {
    ref (_,prob) = letter;
    p += prob;
    prob = p;
  }
}

//
// Redefine stdout to use lock-free binary I/O and capture a newline
//
use IO;
const stdout = openfd(1).writer(kind=iokind.native, locking=false);
param newline = "\n".toByte();

//
// Repeat sequence "alu" for n characters
//
proc repeatMake(desc, alu, n) {
  stdout.writef("%s", desc);

  const r = alu.size,
        s = [i in 0..r+lineLength] alu[i % r]: int(8);

  for i in 0..n by lineLength {
    const lo = i % r,
          len = min(lineLength, n-i);
    stdout.write(s[lo..#len], newline);
  }
}

//
// Output a random sequence of length 'n' using distribution a
//
proc randomMake(desc, a, n) {
  var line_buff: [0..lineLength] int(8);

  stdout.writef("%s", desc);
  for i in 1..n by lineLength do
    addLine(min(lineLength, n-i+1));

  //
  // Add a line of random sequence
  //
  proc addLine(numBytes) {
    for (r, i) in zip(getRands(numBytes), 0..) {
      const (nucl, prob) = a[0];
      if r < prob {
        line_buff[i] = nucl: int(8);
      } else {
        var lo = a.domain.low,
            hi = a.domain.high;
        while (hi > lo+1) {
          var ai = (hi + lo) / 2;
          const (_, prob) = a[ai];
          if r < prob then
            hi = ai;
          else
            lo = ai;
        }
        const (nucl, _) = a[hi];
        line_buff[i] = nucl: int(8);
      }
    }
    line_buff[numBytes] = newline;

    stdout.write(line_buff[0..numBytes]);
  }
}


//
// Deterministic random number generator
//
var lastRand = 42;

iter getRands(n) {
  param IA = 3877,
        IC = 29573,
        IM = 139968;

  for 0..#n {
    lastRand = (lastRand * IA + IC) % IM;
    yield lastRand: real / IM;
  }
}
