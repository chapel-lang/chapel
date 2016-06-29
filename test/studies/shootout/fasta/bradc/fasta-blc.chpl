/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Casey, Battaglino, Kyle Brady, and Preston Sahabu
 */
use IO;

config const n = 1000,   // controls the length of the generated strings

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
const ALU : [0..286] Ntide = [
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

record Freq {
  var c: Ntide;
  var p: real;
}

// Sequences to be randomly generated (probability table)

const IUB : [0..14] Freq = [
  new Freq(a, 0.27), new Freq(c, 0.12), new Freq(g, 0.12), new Freq(t, 0.27),
  new Freq(B, 0.02), new Freq(D, 0.02), new Freq(H, 0.02), new Freq(K, 0.02),
  new Freq(M, 0.02), new Freq(N, 0.02), new Freq(R, 0.02), new Freq(S, 0.02),
  new Freq(V, 0.02), new Freq(W, 0.02), new Freq(Y, 0.02)
];

const HomoSapiens : [0..3] Freq = [
  new Freq(a, 0.3029549426680),
  new Freq(c, 0.1979883004921),
  new Freq(g, 0.1975473066391),
  new Freq(t, 0.3015094502008)
];

proc main() {
  sumAndScale(IUB);
  sumAndScale(HomoSapiens);
  repeatMake(">ONE Homo sapiens alu\n", ALU, n * 2);
  randomMake(">TWO IUB ambiguity codes\n", IUB, n * 3);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, n * 5);
}

// Scan operation
proc sumAndScale(alphabet) {
  var p = 0.0;
  for letter in alphabet {
    p += letter.p;
    letter.p = p * lookupScale;
  }
  alphabet[a.high].p = lookupScale;
}

// Make lookup table for random sequence generation
var lookup : [0..#lookupSize] Freq;
proc makeLookup(a) {
  var j: int = 0;
  for i in 0..#lookupSize {
    while (a[j].p < i) do
      j += 1;

    lookup[i] = a[j];
  }
}

// Deterministic random number generator as specified
record Random {
  const IA = 3877;
  const IC = 29573;
  const IM = 139968;
  const SCALE = lookupScale / IM;

  var last = 42;
  iter get(n) {
    for 0..#n {
      last = (last * IA + IC) % IM;
      yield SCALE * last;
    }
  }
}

var random: Random;

const stdout = openfd(1).writer(kind=iokind.native, locking=false);
param newLine: int(8) = ascii("\n");


// Add a line of random sequence
var line_buff : [0..lineLength] int(8);
proc addLine(bytes: int) {
  for (i, r) in zip(0..#n, random.get(bytes)) {
    var ai = r: int;
    while (lookup[ai].p < r) do
      ai = ai + 1;

    line_buff[i] = lookup[ai].c;
  }
  line_buff[bytes] = 10;
  stdout.write(line_buff[0..bytes]);
}

// Output a random sequence of length n using distribution a
proc randomMake(desc: string, a: [], n: int) {
  var len : int = n;
  makeLookup(a);
  stdout.writef("%s", desc);
  while (len > 0) {
    var bytes : int = min(lineLength, len);
    addLine(bytes);
    len = len - bytes;
  }
}

// Repeat sequence "alu" for n characters
proc repeatMake(desc: string, alu: [], n: int) {
  stdout.writef("%s", desc);
  var r : int = alu.size;
  var s : [0..(r+lineLength)] int(8);
  for d in s.domain do 
    s[d] = alu[d % r];
  
  var j : int;
  for i in 0..#(n / lineLength) {
    j = (i * lineLength) % r;
    stdout.write(s[j..#lineLength], newLine);
  }

  const remain = n % lineLength;
  if remain != 0 {
    j = ((n / lineLength) * lineLength) % r;
    stdout.write(s[j..#remain], newLine);
  }
}

