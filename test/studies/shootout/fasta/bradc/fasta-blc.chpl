/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Casey, Battaglino, Kyle Brady, and Preston Sahabu
 */
use IO;

config const n = 1000;   // controls the length of the generated strings

config const lineLength = 60,
             lookupSize = 4*1024,
             lookupScale = lookupSize - 1.0;

//
// TODO: Could we eliminate these ascii initializers somehow?
//
enum Ntide {
  A = ascii("A"), C = ascii("C"), G = ascii("G"), T = ascii("T"),
  a = ascii("a"), c = ascii("c"), g = ascii("g"), t = ascii("t"),
  B = ascii("B"), D = ascii("D"), H = ascii("H"), K = ascii("K"),
  M = ascii("M"), N = ascii("N"), R = ascii("R"), S = ascii("S"),
  V = ascii("V"), W = ascii("W"), Y = ascii("Y")
}
use Ntide;

// Sequence to be repeated
const ALU: [0..286] Ntide = [
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
// index aliases for use with (nucleotide, probability) tuples
//
param nucl = 1,
      prob = 2;

// Sequences to be randomly generated (probability table)

const IUB: [0..14] (Ntide, real) = [
  (a, 0.27), (c, 0.12), (g, 0.12), (t, 0.27),
  (B, 0.02), (D, 0.02), (H, 0.02), (K, 0.02),
  (M, 0.02), (N, 0.02), (R, 0.02), (S, 0.02),
  (V, 0.02), (W, 0.02), (Y, 0.02)
];

const HomoSapiens: [0..3] (Ntide, real) = [
  (a, 0.3029549426680),
  (c, 0.1979883004921),
  (g, 0.1975473066391),
  (t, 0.3015094502008)
];

proc main() {
  sumAndScale(IUB);
  sumAndScale(HomoSapiens);
  repeatMake(">ONE Homo sapiens alu\n", ALU, n * 2);
  randomMake(">TWO IUB ambiguity codes\n", IUB, n * 3);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, n * 5);
}

// Scan operation
proc sumAndScale(alphabet: [?D]) {
  var p = 0.0;
  for letter in alphabet {
    p += letter(prob);
    letter(prob) = p * lookupScale;
  }
  alphabet[D.high](prob) = lookupScale;
}

const stdout = openfd(1).writer(kind=iokind.native, locking=false);
param newLine: int(8) = ascii("\n");

// Repeat sequence "alu" for n characters
proc repeatMake(desc, alu, n) {
  stdout.writef("%s", desc); // TODO: Why can't this be a write()?
  const r = alu.size;
  //
  // TODO; Can we reduce reliance on % below?
  //
  const s: [0..(r+lineLength)] int(8) = [i in 0..(r+lineLength)] alu[i % r];

  for i in 0..n by lineLength {
    const j = i % r;
    const len = min(lineLength, n-i);
    // TODO: Can we avoid this slice?
    stdout.write(s[j..#len], newLine);
  }
}

// Output a random sequence of length 'len' using distribution a
proc randomMake(desc, a, n) {
  var lookup: [0..#lookupSize] (Ntide, real);
  for (l,v) in zip(lookup, initLookup(a)) do
    l = v;
    
  stdout.writef("%s", desc);
  //  stdout.write(desc);
  for i in 1..n by lineLength do
    addLine(min(lineLength, n-i+1), lookup);
}

iter initLookup(a) {
  var j = 0;
  for i in 0..#lookupSize {
    while (a[j](prob) < i) do
      j += 1;

    yield a[j];
  }
}

// Add a line of random sequence
//
// TODO: This wants to be static
//
var line_buff: [0..lineLength] int(8);
proc addLine(bytes, lookup) {
  for (r, i) in zip(getRands(bytes), 0..) {
    var ai = r: int;
    while (lookup[ai](prob) < r) do
      ai += 1;

    line_buff[i] = lookup[ai](nucl);
  }
  line_buff[bytes] = 10;
  //
  // TODO: Any way to avoid the slice here?  %s with a control character?
  //
  stdout.write(line_buff[0..bytes]);
}

// Deterministic random number generator as specified

//
// TODO: This wants to be static
//
var lastRand = 42;

iter getRands(n) {
  param IA = 3877,
        IC = 29573,
        IM = 139968;
  const SCALE = lookupScale / IM;

  for 0..#n {
    lastRand = (lastRand * IA + IC) % IM;
    yield SCALE * lastRand;
  }
}
