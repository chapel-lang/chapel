//Fasta Shootout
//http://benchmarksgame.alioth.debian.org
//Casey Battaglino
use IO;

config const LINE_LENGTH = 60;
config const LOOKUP_SIZE = 4*1024;
config const LOOKUP_SCALE : real = LOOKUP_SIZE - 1;
config const n = 1000;

var outfd = openfd(1);
var stdout = outfd.writer(locking=false);

record Freq {
  var c: string;
  var p: real;
}

//Sequence to be repeated
var ALU : string = "GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCAC" +
                   "TTTGGGAGGCCGAGGCGGGCGGATCACCTGAGGTCAG" +
                   "GAGTTCGAGACCAGCCTGGCCAACATGGTGAAACCCC" +
                   "GTCTCTACTAAAAATACAAAAATTAGCCGGGCGTGGT" +
                   "GGCGCGCGCCTGTAATCCCAGCTACTCGGGAGGCTGA" +
                   "GGCAGGAGAATCGCTTGAACCCGGGAGGCGGAGGTTG" +
                   "CAGTGAGCCGAGATCGCGCCACTGCACTCCAGCCTGG" +
                   "GCGACAGAGCGAGACTCCGTCTCAAAAA";

//Sequences to be randomly generated (probability table)
var IUB : [0..14] Freq;
IUB[0] = new Freq('a', 0.27);
IUB[1] = new Freq('c', 0.12);
IUB[2] = new Freq('g', 0.12);
IUB[3] = new Freq('t', 0.27);
IUB[4] = new Freq('B', 0.02);
IUB[5] = new Freq('D', 0.02);
IUB[6] = new Freq('H', 0.02);
IUB[7] = new Freq('K', 0.02);
IUB[8] = new Freq('M', 0.02);
IUB[9] = new Freq('N', 0.02);
IUB[10] = new Freq('R', 0.02);
IUB[11] = new Freq('S', 0.02);
IUB[12] = new Freq('V', 0.02);
IUB[13] = new Freq('W', 0.02);
IUB[14] = new Freq('Y', 0.02);

var HomoSapiens : [0..3] Freq;
HomoSapiens[0] = new Freq('a', 0.3029549426680);
HomoSapiens[1] = new Freq('c', 0.1979883004921);
HomoSapiens[2] = new Freq('g', 0.1975473066391);
HomoSapiens[3] = new Freq('t', 0.3015094502008);

// (Scan operation)
proc sumAndScale(a :[?D]) {
  var p : real = 0;
  for item in a {
    p += item.p;
    item.p = p * LOOKUP_SCALE;
  }
  a[a.size-1].p = LOOKUP_SCALE;
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
proc addLine(nBytes: int) {
  for i in 0..nBytes-1 {
    var r  = random.next();
    var ai = r : int; 
    while (lookup[ai].p < r) {
      ai = ai + 1;
    }
    stdout.write(lookup[ai].c); //A faster way of writing would make this code viable
  }
  stdout.writeln();
}

// Output a random sequence of length n using distribution a
proc randomMake(desc : string, a :[?D], n : int) {
  var len : int = n;
  makeLookup(a);
  stdout.write(desc);
  while (len > 0) {
    var nBytes : int = min(LINE_LENGTH, len);
    addLine(nBytes);
    len = len - nBytes;
  }
}

// Repeat sequence "alu" for n characters
proc repeatMake(desc : string, alu : string, n : int) {
  stdout.write(desc);
  var r : int = alu.size;
  var s : string = alu + alu + alu[1..n%r];
  var j : int;

  for i in 0..(n / LINE_LENGTH)-1 {
    j = i*LINE_LENGTH % r;
    stdout.writeln(s[j + 1..j + LINE_LENGTH]);
  }
  if (n % LINE_LENGTH) {
    j = (n / LINE_LENGTH)*LINE_LENGTH % r;
    stdout.writeln(s[j + 1..j + (n % LINE_LENGTH)]);
  }
}

proc main() {
  sumAndScale(IUB);
  sumAndScale(HomoSapiens);
  repeatMake(">ONE Homo sapiens alu\n", ALU, n * 2);
  randomMake(">TWO IUB ambiguity codes\n", IUB, n * 3);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, n * 5);
}
