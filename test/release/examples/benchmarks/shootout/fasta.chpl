/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Preston Sahabu
   derived from the Chapel fastaredux version by Casey Battaglino et al.
            and the GNU C version by Paul Hsieh
*/

config const n = 1000,   // controls the length of the generated strings
             lineLength = 60;

config param IM = 139968, // parameters for the RNG
             IA = 3877,
             IC = 29573,
             seed = 42;

//
// Nucleotide definitions
//
enum nucleotide {
  A = ascii("A"), C = ascii("C"), G = ascii("G"), T = ascii("T"),
  a = ascii("a"), c = ascii("c"), g = ascii("g"), t = ascii("t"),
  B = ascii("B"), D = ascii("D"), H = ascii("H"), K = ascii("K"),
  M = ascii("M"), N = ascii("N"), R = ascii("R"), S = ascii("S"),
  V = ascii("V"), W = ascii("W"), Y = ascii("Y")
}
use nucleotide;

//
// Sequence to be repeated
//
const ALU: [0..286] int(8) = [
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
param nucl = 1,
      prob = 2;

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
  repeatMake(">ONE Homo sapiens alu", ALU, n * 2);
  randomMake(">TWO IUB ambiguity codes", IUB, n * 3);
  randomMake(">THREE Homo sapiens frequency", HomoSapiens, n * 5);
}

//
// Redefine stdout to use lock-free binary I/O and capture a newline
//
const stdout = openfd(1).writer(kind=iokind.native, locking=false);
param newline = ascii("\n");

//
// Repeat sequence "alu" for n characters
//
proc repeatMake(desc, alu, n) {
  stdout.writeln(desc);

  const r = alu.size,
        s = [i in 0..(r+lineLength)] alu[i % r];

  for i in 0..n by lineLength {
    const lo = i % r + 1,
          len = min(lineLength, n-i);
    stdout.write(s[lo..#len], newline);
  }
}

//
// Output a random sequence of length 'n' using distribution a
//
proc randomMake(desc, nucleotides, n) {
  stdout.writeln(desc);

  const numNucls = nucleotides.size;
  var cumulProb: [1..numNucls] int;
  var p = 0.0;
  for i in 1..numNucls {
    p += nucleotides[i](prob);
    cumulProb[i] = 1 + (p*IM):int;
  }

  var line_buff: [0..lineLength] int(8);

  for i in 1..n by lineLength {
    const bytes = min(lineLength, n-i+1);

    for (r, i) in zip(getRands(bytes), 0..) {
      for j in 1..numNucls {
        if r < cumulProb[j] {
          line_buff[i] = nucleotides[j](nucl);
          break;
        }
      }
    }
    line_buff[bytes] = newline:int(8);

    stdout.write(line_buff[0..bytes]);
  }
}


//
// Deterministic random number generator
//
var lastRand = seed;

iter getRands(n) {
  for 0..#n {
    lastRand = (lastRand * IA + IC) % IM;
    yield lastRand;
  }
}
