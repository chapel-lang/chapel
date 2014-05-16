/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Ben Harshbarger
 * based on Go implementation by Oleg Mazurov
 *
 */

use AdvancedIters;

config const N = 7;

assert(N >=3 && N <= 12, "N must be in 3..12");

const D = {0..N};
const R = 0..#N;

const nchunks = 720;
var Fact : [D] int;

proc main() {
  Fact[0] = 1;
  for i in 1..N do
    Fact[i] = Fact[i-1] * i;

  var chunksz = (Fact[N] + nchunks - 1) / nchunks;
  chunksz += chunksz%2;

  const work = 0 .. (Fact[N] - chunksz) by chunksz;
  
  var flips : [work] int;
  var checks : [work] int;

  forall idx in dynamic(work, 1) {
    (flips[idx], checks[idx]) = fannkuch(idx, idx+chunksz);
  }

  const c = + reduce checks;
  const r = max reduce flips;
  
  writeln(c, "\nPfannkuchen(", N, ") = ", r);
}

proc fannkuch(idxMin:int, idxMax:int) {
  var p, pp, count : [R] int;
  p = R;

  // first permutation
  var idx = idxMin;
  for i in 1..N-1 by -1 {
    const d = idx / Fact[i];
    count[i] = d;
    idx = idx % Fact[i];

    const slice = 0..#(i + 1);
    pp(slice) = p(slice);
    for j in 0..i {
      if j + d <= i then
        p[j] = pp[j+d];
      else
        p[j] = pp[j+d-i-1];
    }
  }

  var maxFlips = 1;
  var checkSum = 0;

  idx = idxMin;

  while true {
    if p[0] != 0 {
      //countFlips
      var flips = 1;
      var first = p[0];

      if p[first] != 0 {
        pp = p;
        do {
          flips += 1;
          var lo = 1, hi = first - 1;
          while lo < hi {
            pp[lo] <=> pp[hi];
            lo += 1;
            hi -= 1;
          }
          pp[first] <=> first;
        } while pp[first] != 0;
      }

      maxFlips = max(maxFlips, flips);
      if idx%2 == 0 then checkSum += flips;
      else checkSum -= flips;
    }

    idx += 1;

    if idx == idxMax then break;

    // nextPerm
    var first = p[1];
    p[1] = p[0];
    p[0] = first;

    var i = 1;
    count[i] += 1;
    while count[i] > i {
      count[i] = 0;
      i += 1;
      const next = p[1];
      p[0] = p[1];
      for j in 1..i-1 do p[j] = p[j+1];
      p[i] = first;
      first = next;
      
      count[i] += 1;
    }
  }

  return (maxFlips, checkSum);
}
