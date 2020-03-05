/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Kyle Brady
 * modified from the Chapel version by Casey Battaglino
 */

config const LINE_LENGTH = 60;
config const LOOKUP_SIZE = 4*1024;
config const LOOKUP_SCALE : real = LOOKUP_SIZE - 1;
config const n = 1000;

//extern proc printf(s:c_string, ref args ...);
extern proc printf(s:c_string, len:int(32), ref args ...);
extern proc printf(s:c_string, ref args ...);

record Freq {
  var c: int(8);
  var p: real;
}

//Sequence to be repeated
// A -> 65  G -> 71
// C -> 67  T -> 84
const ALU : [0..286] int(8) = [
  71:int(8), 71:int(8), 67:int(8), 67:int(8), 71:int(8), 71:int(8), 71:int(8),
  67:int(8), 71:int(8), 67:int(8), 71:int(8), 71:int(8), 84:int(8), 71:int(8),
  71:int(8), 67:int(8), 84:int(8), 67:int(8), 65:int(8), 67:int(8), 71:int(8),
  67:int(8), 67:int(8), 84:int(8), 71:int(8), 84:int(8), 65:int(8), 65:int(8),
  84:int(8), 67:int(8), 67:int(8), 67:int(8), 65:int(8), 71:int(8), 67:int(8),
  65:int(8), 67:int(8), 84:int(8), 84:int(8), 84:int(8), 71:int(8), 71:int(8),
  71:int(8), 65:int(8), 71:int(8), 71:int(8), 67:int(8), 67:int(8), 71:int(8),
  65:int(8), 71:int(8), 71:int(8), 67:int(8), 71:int(8), 71:int(8), 71:int(8),
  67:int(8), 71:int(8), 71:int(8), 65:int(8), 84:int(8), 67:int(8), 65:int(8),
  67:int(8), 67:int(8), 84:int(8), 71:int(8), 65:int(8), 71:int(8), 71:int(8),
  84:int(8), 67:int(8), 65:int(8), 71:int(8), 71:int(8), 65:int(8), 71:int(8),
  84:int(8), 84:int(8), 67:int(8), 71:int(8), 65:int(8), 71:int(8), 65:int(8),
  67:int(8), 67:int(8), 65:int(8), 71:int(8), 67:int(8), 67:int(8), 84:int(8),
  71:int(8), 71:int(8), 67:int(8), 67:int(8), 65:int(8), 65:int(8), 67:int(8),
  65:int(8), 84:int(8), 71:int(8), 71:int(8), 84:int(8), 71:int(8), 65:int(8),
  65:int(8), 65:int(8), 67:int(8), 67:int(8), 67:int(8), 67:int(8), 71:int(8),
  84:int(8), 67:int(8), 84:int(8), 67:int(8), 84:int(8), 65:int(8), 67:int(8),
  84:int(8), 65:int(8), 65:int(8), 65:int(8), 65:int(8), 65:int(8), 84:int(8),
  65:int(8), 67:int(8), 65:int(8), 65:int(8), 65:int(8), 65:int(8), 65:int(8),
  84:int(8), 84:int(8), 65:int(8), 71:int(8), 67:int(8), 67:int(8), 71:int(8),
  71:int(8), 71:int(8), 67:int(8), 71:int(8), 84:int(8), 71:int(8), 71:int(8),
  84:int(8), 71:int(8), 71:int(8), 67:int(8), 71:int(8), 67:int(8), 71:int(8),
  67:int(8), 71:int(8), 67:int(8), 67:int(8), 84:int(8), 71:int(8), 84:int(8),
  65:int(8), 65:int(8), 84:int(8), 67:int(8), 67:int(8), 67:int(8), 65:int(8),
  71:int(8), 67:int(8), 84:int(8), 65:int(8), 67:int(8), 84:int(8), 67:int(8),
  71:int(8), 71:int(8), 71:int(8), 65:int(8), 71:int(8), 71:int(8), 67:int(8),
  84:int(8), 71:int(8), 65:int(8), 71:int(8), 71:int(8), 67:int(8), 65:int(8),
  71:int(8), 71:int(8), 65:int(8), 71:int(8), 65:int(8), 65:int(8), 84:int(8),
  67:int(8), 71:int(8), 67:int(8), 84:int(8), 84:int(8), 71:int(8), 65:int(8),
  65:int(8), 67:int(8), 67:int(8), 67:int(8), 71:int(8), 71:int(8), 71:int(8),
  65:int(8), 71:int(8), 71:int(8), 67:int(8), 71:int(8), 71:int(8), 65:int(8),
  71:int(8), 71:int(8), 84:int(8), 84:int(8), 71:int(8), 67:int(8), 65:int(8),
  71:int(8), 84:int(8), 71:int(8), 65:int(8), 71:int(8), 67:int(8), 67:int(8),
  71:int(8), 65:int(8), 71:int(8), 65:int(8), 84:int(8), 67:int(8), 71:int(8),
  67:int(8), 71:int(8), 67:int(8), 67:int(8), 65:int(8), 67:int(8), 84:int(8),
  71:int(8), 67:int(8), 65:int(8), 67:int(8), 84:int(8), 67:int(8), 67:int(8),
  65:int(8), 71:int(8), 67:int(8), 67:int(8), 84:int(8), 71:int(8), 71:int(8),
  71:int(8), 67:int(8), 71:int(8), 65:int(8), 67:int(8), 65:int(8), 71:int(8),
  65:int(8), 71:int(8), 67:int(8), 71:int(8), 65:int(8), 71:int(8), 65:int(8),
  67:int(8), 84:int(8), 67:int(8), 67:int(8), 71:int(8), 84:int(8), 67:int(8),
  84:int(8), 67:int(8), 65:int(8), 65:int(8), 65:int(8), 65:int(8), 65:int(8)
];

//Sequences to be randomly generated (probability table)
var IUB : [0..14] Freq;
IUB[0] = new Freq(97, 0.27);  // a -> 97
IUB[1] = new Freq(99, 0.12);  // c -> 99
IUB[2] = new Freq(103, 0.12); // g -> 103
IUB[3] = new Freq(116, 0.27); // t -> 116
IUB[4] = new Freq(66, 0.02);  // B -> 66
IUB[5] = new Freq(68, 0.02);  // D -> 68
IUB[6] = new Freq(72, 0.02);  // H -> 72
IUB[7] = new Freq(75, 0.02);  // K -> 75
IUB[8] = new Freq(77, 0.02);  // M -> 77
IUB[9] = new Freq(78, 0.02);  // N -> 78
IUB[10] = new Freq(82, 0.02); // R -> 82
IUB[11] = new Freq(83, 0.02); // S -> 83
IUB[12] = new Freq(86, 0.02); // V -> 86
IUB[13] = new Freq(87, 0.02); // W -> 87
IUB[14] = new Freq(89, 0.02); // Y -> 89

var HomoSapiens : [0..3] Freq;
HomoSapiens[0] = new Freq(97, 0.3029549426680);  // a -> 97
HomoSapiens[1] = new Freq(99, 0.1979883004921);  // c -> 99
HomoSapiens[2] = new Freq(103, 0.1975473066391); // g -> 103
HomoSapiens[3] = new Freq(116, 0.3015094502008); // t -> 116

// (Scan operation)
proc sumAndScale(a :[?D]) {
  var p : real = 0;
  for item in a {
    p += item.p;
    item.p = p * LOOKUP_SCALE;
  }
  a[a.numElements-1].p = LOOKUP_SCALE;
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

var lookup : [0..LOOKUP_SIZE-1] Freq;
var random = new Random();

// Make lookup table for random sequence generation
proc makeLookup(a :[?D]) {
  var j: int = 0;
  for i in 0..LOOKUP_SIZE-1 {
    while (a[j].p < i) do j = j + 1;
    lookup[i] = a[j];
  }
}

// Add a line of random sequence
var line_buff : [0..LINE_LENGTH+1] int(8);
proc addLine(nBytes: int) {
  for i in 0..nBytes-1 {
    var r  = random.next();
    var ai = r : int;
    while (lookup[ai].p < r) {
      ai = ai + 1;
    }
    line_buff[i] = lookup[ai].c; //A faster way of writing would make this code viable
  }
  line_buff[nBytes] = 10;
  line_buff[nBytes+1] = 0;
  printf("%s", line_buff[0]);
}

// Output a random sequence of length n using distribution a
proc randomMake(desc : string, a :[], n : int) {
  var len : int = n;
  makeLookup(a);
  write(desc);
  while (len > 0) {
    var nBytes : int = min(LINE_LENGTH, len);
    addLine(nBytes);
    len = len - nBytes;
  }
}

// Repeat sequence "alu" for n characters
proc repeatMake(desc : string, alu : [], n : int) {
  write(desc);
  var r : int = alu.size;
  var s : [0..(r+LINE_LENGTH)] int(8);
  for indx in s.domain {
    s[indx] = alu[indx % r];
  }
  var j : int;

  for i in 0..(n / LINE_LENGTH)-1 {
    j = (i * LINE_LENGTH) % r;
    printf("%.*s\n", LINE_LENGTH:int(32), s[j]);

  }
  if (n % LINE_LENGTH) {
    j = ((n / LINE_LENGTH)*LINE_LENGTH) % r;
    printf("%.*s\n", (n % LINE_LENGTH):int(32), s[j]);
  }
}

proc main() {
  sumAndScale(IUB);
  sumAndScale(HomoSapiens);
  repeatMake(">ONE Homo sapiens alu\n", ALU, n * 2);
  randomMake(">TWO IUB ambiguity codes\n", IUB, n * 3);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, n * 5);
}
