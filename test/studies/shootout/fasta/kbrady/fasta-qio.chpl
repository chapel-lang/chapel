/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady, Preston Sahabu
 * modified from the Chapel version by Casey Battaglino
 */
use IO;

config const LINE_LENGTH = 60;
config const LOOKUP_SIZE = 4*1024;
config const LOOKUP_SCALE: real = LOOKUP_SIZE - 1;
config const n = 1000;

const stdout = openfd(1).writer(kind=iokind.native, locking=false);
param newLine: int(8) = "\n".toByte();

param
  A = "A".toByte(), C = "C".toByte(), G = "G".toByte(), T = "T".toByte(),
  a = "a".toByte(), c = "c".toByte(), g = "g".toByte(), t = "t".toByte(),
  B = "B".toByte(), D = "D".toByte(), H = "H".toByte(), K = "K".toByte(),
  M = "M".toByte(), N = "N".toByte(), R = "R".toByte(), S = "S".toByte(),
  V = "V".toByte(), W = "W".toByte(), Y = "Y".toByte();

// Sequence to be repeated
const ALU: [0..286] int = [
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

// Deterministic random number generator as specified
class Random {
  const IA = 3877;
  const IC = 29573;
  const IM = 139968;
  const SCALE = LOOKUP_SCALE / IM;

  var last = 42;
  iter get(n: int): (int, real) {
    for i in 0..#n {
      last = (last * IA + IC) % IM;
      yield (i, SCALE * last);
    }
  }
}

// Sequences to be randomly generated (probability table)

record Freq {
  var c: int;
  var p: real;
}

var IUB: [0..14] Freq = [
  new Freq(a, 0.27), new Freq(c, 0.12), new Freq(g, 0.12), new Freq(t, 0.27),
  new Freq(B, 0.02), new Freq(D, 0.02), new Freq(H, 0.02), new Freq(K, 0.02),
  new Freq(M, 0.02), new Freq(N, 0.02), new Freq(R, 0.02), new Freq(S, 0.02),
  new Freq(V, 0.02), new Freq(W, 0.02), new Freq(Y, 0.02)
];

var HomoSapiens: [0..3] Freq = [
  new Freq(a, 0.3029549426680),
  new Freq(c, 0.1979883004921),
  new Freq(g, 0.1975473066391),
  new Freq(t, 0.3015094502008)
];

// Scan operation
proc sumAndScale(a) {
  var p: real = 0;
  for item in a {
    p += item.p;
    item.p = p * LOOKUP_SCALE;
  }
  a[a.size-1].p = LOOKUP_SCALE;
}

// Make lookup table for random sequence generation
var lookup: [0..#LOOKUP_SIZE] Freq;
proc makeLookup(a) {
  var j: int = 0;
  for i in 0..#LOOKUP_SIZE {
    while (a[j].p < i) do
      j += 1;

    lookup[i] = a[j];
  }
}

// Add a line of random sequence
var random = new unmanaged Random();
var line_buff: [0..LINE_LENGTH] int(8);
proc addLine(nBytes: int) {
  for (i, r) in random.get(nBytes) {
    var ai = r: int;
    while (lookup[ai].p < r) do
      ai = ai + 1;

    line_buff[i] = lookup[ai].c: int(8);
  }
  line_buff[nBytes] = 10;
  stdout.write(line_buff[0..nBytes]);
}

// Output a random sequence of length n using distribution a
proc randomMake(desc: string, a: [], n: int) {
  var len: int = n;
  makeLookup(a);
  stdout.writef("%s", desc);
  while (len > 0) {
    var nBytes: int = min(LINE_LENGTH, len);
    addLine(nBytes);
    len = len - nBytes;
  }
}

// Repeat sequence "alu" for n characters
proc repeatMake(desc: string, alu: [], n: int) {
  stdout.writef("%s", desc);
  var r: int = alu.size;
  var s: [0..(r+LINE_LENGTH)] int(8);
  for d in s.domain do 
    s[d] = alu[d % r]: int(8);
  
  var j: int;
  for i in 0..#(n / LINE_LENGTH) {
    j = (i * LINE_LENGTH) % r;
    stdout.write(s[j..#LINE_LENGTH], newLine);
  }

  const remain = n % LINE_LENGTH;
  if remain != 0 {
    j = ((n / LINE_LENGTH) * LINE_LENGTH) % r;
    stdout.write(s[j..#remain], newLine);
  }
}

proc main() {
  sumAndScale(IUB);
  sumAndScale(HomoSapiens);
  repeatMake(">ONE Homo sapiens alu\n", ALU, n * 2);
  randomMake(">TWO IUB ambiguity codes\n", IUB, n * 3);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, n * 5);

  delete random;
}
