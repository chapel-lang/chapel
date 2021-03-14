/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Ben Harshbarger
 * based on Java implementation by Oleg Mazurov
 *
 */

const NCHUNKS = 150;
config const n : int = 7;
const ntasks = 0..#here.maxTaskPar;

var Fact : [0..n] int;
Fact[0] = 1;
for i in 1..n do
  Fact[i] = Fact[i-1] * i;

const CHUNKSZ = (Fact[n] + NCHUNKS - 1) / NCHUNKS;
const NTASKS = (Fact[n] + CHUNKSZ - 1) / CHUNKSZ;

var maxFlips, checkSums : [0..#NTASKS] int;

var taskCount : atomic int;

var Fanns : [ntasks] owned Fann = for ntasks do new owned Fann();

coforall i in ntasks {
  var task = taskCount.fetchAdd(1);
  while task < NTASKS {
    Fanns[i].work(task);
    task = taskCount.fetchAdd(1);
  }
}
const c = + reduce checkSums;
const r = max reduce maxFlips;
writeln(c, "\nPfannkuchen(", n, ") = ", r);

class Fann {
  const D = {0..#n};
  var p, pp, count : [D] int;

  proc work(task : int) {
    const idxMin = task * CHUNKSZ;
    const idxMax = min(Fact[n], idxMin + CHUNKSZ);

    firstPerm(idxMin);

    var maxf = 1;
    var check = 0;
    var i = idxMin;
    while true {
      if p[0] != 0 {
        var flips = countFlips();
        maxf = max(maxf, flips);
        if i%2 == 0 then check += flips;
        else check -= flips;
      }

      i += 1;

      if i == idxMax then break;

      nextPerm();
    }

    maxFlips[task] = maxf;
    checkSums[task] = check;
  }

  proc firstPerm(in idx : int) {
    p = D;

    for i in 1..n-1 by -1 {
      const d = idx / Fact[i];
      count[i] = d;
      idx = idx % Fact[i];

      const slice = 0..#(i+1);
      pp(slice) = p(slice);
      for j in 0..i {
        p[j] = if j + d <= i then pp[j+d] else pp[j+d-i-1];
      }
    }
  }

  proc nextPerm() {
    var first = p[1];
    p[1] = p[0];
    p[0] = first;

    var i = 1;
    count[i] += 1;
    while count[i] > i {
      count[i] = 0;
      i += 1;
      var next = p[1];
      p[0] = p[1];
      for j in 1..i-1 do p[j] = p[j+1];
      p[i] = first;
      first = next;

      count[i] += 1;
    }
  }

  proc countFlips() {
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

    return flips;
  }
}
