/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Brad Chamberlain

   derived from the GNU C version by Аноним Легионов and Jeremy Zerfas
     as well as from previous Chapel versions by Casey Battaglino,
     Kyle Brady, and Preston Sahabu.
*/

config const n = 1000,                   // the length of the generated strings
             lineLength = 60,            // the number of columns in the output
             blockSize = 1024,           // the parallelization granularity
             numTasks = here.maxTaskPar; // the degree of parallelization

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

param IM = 139968;

//
// Probability tables for sequences to be randomly generated
//
const IUB = [(a, 0.27), (c, 0.12), (g, 0.12),
                                     (t, 0.27), (B, 0.02), (D, 0.02),
                                     (H, 0.02), (K, 0.02), (M, 0.02),
                                     (N, 0.02), (R, 0.02), (S, 0.02),
                                     (V, 0.02), (W, 0.02), (Y, 0.02)];

const HomoSapiens = [(a, 0.3029549426680),
                                            (c, 0.1979883004921),
                                            (g, 0.1975473066391),
                                            (t, 0.3015094502008)];


proc main() {
  repeatMake(">ONE Homo sapiens alu\n", ALU, 2*n);
  randomMake(">TWO IUB ambiguity codes\n", IUB, 3*n);
  randomMake(">THREE Homo sapiens frequency\n", HomoSapiens, 5*n);
}

//
// Redefine stdout to use lock-free binary I/O and capture a newline
//
const stdout = openfd(1).writer(kind=iokind.native, locking=false);
param newline = ascii("\n"): int(8);

//
// Repeat string 'str' for 'n' characters
//
proc repeatMake(desc, str, n) {
  stdout.write(desc);

  const r = str.size,
        s = [i in 0..(r+lineLength)] str[i % r];

  for i in 0..n by lineLength {
    const lo = i % r + 1,
          len = min(lineLength, n-i);
    stdout.write(s[lo..#len], newline);
  }
}

//
// Output a random sequence of length 'n' using distribution 'a'
//
proc randomMake(desc, nuclInfo, n) {
  const numNucls = nuclInfo.size;

  stdout.write(desc);

  var cumul_p: [1..numNucls] int;
  //
  // Sum the probabilities of the nucleotide info
  //
  var p = 0.0;
  for i in 1..numNucls {
    p += nuclInfo[i](prob);
    cumul_p[i] = 1 + (p*IM):int;
  }

  var rngTid$, outTid$: [0..#numTasks] atomic int;

  rngTid$.write(1);
  outTid$.write(1);

  /*
  for i in 0..#numTasks {
    rngTid$[i].write(1);
    outTid$[i].write(1);
  }
*/

  coforall tid in 0..#numTasks {
    const chunkSize = lineLength*blockSize;
    const nextTask = (tid + 1) % numTasks;

    var line_buff: [0..(lineLength+1)*blockSize+1] int(8);
    var rands: [0..chunkSize] int/*(32)*/;

    //
    // TODO: Use some sort of chunking iterator?
    //
    //    writef("tid %i writing %i..%i\n", tid, lo, n);
    for i in 1..n by chunkSize*numTasks align (tid*chunkSize+1) {

      const bytes = min(chunkSize, n-i+1);
      //      writef("tid %i doing %i..#%i\n", tid, i, bytes);

      //      writef("tid %i working on bytes %i\n", tid, bytes);      
      //      stderr.writef("tid %i waiting for turn to say %i\n", tid, i);
      while (rngTid$[tid].read() != i) do ;
      //      stderr.writef("tid %i got turn\n", tid);
      getRands(bytes, rands);
      /*
      for (r,i) in zip(getRands(bytes, tid), 0..) do
        rands[i] = r;
      */
      //      stderr.writef("tid %i passing turn %i to %i\n", tid, i+1, nextTask);
      rngTid$[nextTask].write(i + chunkSize);

      var col = 0;
      var off = 0;
      for i in 0..#bytes {
        const r = rands[i];
        var ncnt = 1;
        for j in 1..numNucls do
          if r >= cumul_p[j] then
            ncnt += 1;

        line_buff[off] = nuclInfo[ncnt](nucl);

        off += 1;
        col += 1;
        if (col == lineLength) {
          col = 0;
          line_buff[off] = newline;
          off += 1;
        }
      }
      if (col != 0) {
        line_buff[off] = newline;
        off += 1;
      }


      //      writeln("tid = ", tid, "bytes = ", bytes, " off = ", off);
      {
        while (outTid$[tid].read() != i) do ;
        stdout.write(line_buff[0..#off]);
        outTid$[nextTask].write(i+chunkSize);
      }
    }
    
    
    
  }
}


//
// Deterministic random number generator
//
var lastRand = 42/*:int(32)*/;

proc getRands(n, arr) {
  param IA = 3877,
        IC = 29573;

  //  writef("tid %i got turn\n", tid);
  for i in 0..#n {
    lastRand = (lastRand * IA + IC) % IM;
    arr[i] = lastRand;
  }
}
