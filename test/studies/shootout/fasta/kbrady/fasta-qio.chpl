/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady
 * modified from the Chapel version by Casey Battaglino
 */
use IO;

config const LINE_LENGTH = 60;
config const LOOKUP_SIZE = 4*1024;
config const LOOKUP_SCALE : real = LOOKUP_SIZE - 1;
config const n = 1000;

const stdout = openfd(1).writer(kind=iokind.native, locking=false);

const A: int(8) = 65;
const C: int(8) = 67;
const G: int(8) = 71;
const T: int(8) = 84;
const a: int(8) = 97;
const c: int(8) = 99;
const g: int(8) = 103;
const t: int(8) = 116;

const B: int(8) = 66;
const D: int(8) = 68;
const H: int(8) = 72;
const K: int(8) = 75;
const M: int(8) = 77;
const N: int(8) = 78;
const R: int(8) = 82;
const S: int(8) = 83;
const V: int(8) = 86;
const W: int(8) = 87;
const Y: int(8) = 89;

// Sequence to be repeated
const ALU : [0..286] int(8) = [
  G, G, C, C, G, G, G, C, G, C, G, G, T, G,
  G, C, T, C, A, C, G, C, C, T, G, T, A, A,
  T, C, C, C, A, G, C, A, C, T, T, T, G, G,
  G, A, G, G, C, C, G, A, G, G, C, G, G, G,
  C, G, G, A, T, C, A, C, C, T, G, A, G, G,
  T, C, A, G, G, A, G, T, T, C, G, A, G, A,
  C, C, A, G, C, C, T, G, G, C, C, A, A, C,
  A, T, G, G, T, G, A, A, A, C, C, C, C, G,
  T, C, T, C, T, A, C, T, A, A, A, A, A, T,
  A, C, A, A, A, A, A, T, T, A, G, C, C, G,
  G, G, C, G, T, G, G, T, G, G, C, G, C, G,
  C, G, C, C, T, G, T, A, A, T, C, C, C, A,
  G, C, T, A, C, T, C, G, G, G, A, G, G, C,
  T, G, A, G, G, C, A, G, G, A, G, A, A, T,
  C, G, C, T, T, G, A, A, C, C, C, G, G, G,
  A, G, G, C, G, G, A, G, G, T, T, G, C, A,
  G, T, G, A, G, C, C, G, A, G, A, T, C, G,
  C, G, C, C, A, C, T, G, C, A, C, T, C, C,
  A, G, C, C, T, G, G, G, C, G, A, C, A, G,
  A, G, C, G, A, G, A, C, T, C, C, G, T, C,
  T, C, A, A, A, A, A
];

class Freq {
  var c: int(8);
  var p: real;
}

// Deterministic random number generator as specified
class Random {
  var IM : int = 139968;
  var IA : int = 3877;
  var IC : int = 29573;
  var SCALE : real = LOOKUP_SCALE / IM;
  var last : int = 42;

  proc next() : real {
    last = (last * IA + IC) % IM;
    return SCALE * last;
  }
}

// Sequences to be randomly generated (probability table)
var IUB : [0..14] Freq;
IUB[0] = new Freq(a, 0.27);
IUB[1] = new Freq(c, 0.12);
IUB[2] = new Freq(g, 0.12);
IUB[3] = new Freq(t, 0.27);
IUB[4] = new Freq(B, 0.02);
IUB[5] = new Freq(D, 0.02);
IUB[6] = new Freq(H, 0.02);
IUB[7] = new Freq(K, 0.02);
IUB[8] = new Freq(M, 0.02);
IUB[9] = new Freq(N, 0.02);
IUB[10] = new Freq(R, 0.02);
IUB[11] = new Freq(S, 0.02);
IUB[12] = new Freq(V, 0.02);
IUB[13] = new Freq(W, 0.02);
IUB[14] = new Freq(Y, 0.02);

var HomoSapiens : [0..3] Freq;
HomoSapiens[0] = new Freq(a, 0.3029549426680);
HomoSapiens[1] = new Freq(c, 0.1979883004921);
HomoSapiens[2] = new Freq(g, 0.1975473066391);
HomoSapiens[3] = new Freq(t, 0.3015094502008);

// Scan operation
proc sumAndScale(a :[?D]) {
  var p : real = 0;
  for item in a {
    p += item.p;
    item.p = p * LOOKUP_SCALE;
  }
  a[a.numElements-1].p = LOOKUP_SCALE;
}

// Make lookup table for random sequence generation
var lookup : [0..LOOKUP_SIZE-1] Freq;
proc makeLookup(a :[?D]) {
  var j: int = 0;
  for i in 0..LOOKUP_SIZE-1 {
    while (a[j].p < i) do
      j += 1;

    lookup[i] = a[j];
  }
}

// Add a line of random sequence
var random = new Random();
var line_buff : [0..LINE_LENGTH] int(8);
proc addLine(bytes: int) {
  for i in 0..bytes-1 {
    var r  = random.next();
    var ai = r : int;
    while (lookup[ai].p < r) do
      ai = ai + 1;

    line_buff[i] = lookup[ai].c;
  }
  line_buff[bytes] = 10;
  stdout.write(line_buff[0..bytes]);
}

// Output a random sequence of length n using distribution a
proc randomMake(desc : string, a :[], n : int) {
  var len : int = n;
  makeLookup(a);
  stdout.writef("%s", desc);
  while (len > 0) {
    var bytes : int = min(LINE_LENGTH, len);
    addLine(bytes);
    len = len - bytes;
  }
}

// Repeat sequence "alu" for n characters
proc repeatMake(desc : string, alu : [], n : int) {
  stdout.writef("%s", desc);
  var r : int = alu.size;
  var s : [0..(r+LINE_LENGTH)] int(8);
  for d in s.domain do 
    s[d] = alu[d % r];
  
  var j : int;
  for i in 0..(n / LINE_LENGTH)-1 {
    j = (i * LINE_LENGTH) % r;
    stdout.write(s[j..#LINE_LENGTH], 10:int(8));
  }

  var m = n % LINE_LENGTH;
  if m != 0 {
    j = ((n / LINE_LENGTH) * LINE_LENGTH) % r;
    stdout.write(s[j..#m], 10:int(8));
  }
}

proc main() {
  sumAndScale(IUB);
  sumAndScale(HomoSapiens);
  repeatMake(">ONE Homo sapiens alu\n", ALU, n * 2);
  randomMake(">TWO IUB ambiguity codes\n", IUB, n * 3);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, n * 5);

  delete random;
  for h in HomoSapiens do delete h;
  for i in IUB         do delete i;
}
