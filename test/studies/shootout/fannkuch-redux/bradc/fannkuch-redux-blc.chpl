/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Ben Harshbarger and Brad Chamberlain
   derived from the Swift implementation by Ralph Ganszky
*/

use DynamicIters;

config const n = 7,           // the array size over which to compute perms
             nchunks = 720;   // the number of chunks of parallelism to use

const fact: [1..n] int = computeFact(n);  // memoize n! (n-factorial)


proc main() {
  var checkSum = 0,
      maxFlips = 1;

  var chunksz = (fact(n) + nchunks - 1) / nchunks;
  chunksz += chunksz%2;
  const work = 0..(fact(n) - chunksz) by chunksz;

  forall i in dynamic(work, 1) with (+ reduce checkSum, 
                                     max reduce maxFlips) {
    for (j, flips) in fannkuch(i..#chunksz) {
      maxFlips = max(maxFlips, flips);
      checkSum += if j % 2 == 0 then flips else -flips;
    }
  }
  
  writeln(checkSum);
  writeln("Pfannkuchen(", n, ") = ", maxFlips);
}


iter fannkuch(inds) {
  var p, pp, count: [0..#n] int;
  for i in 0..#n do
    p[i] = i;

  firstPerm();

  for i in inds {
    if p[0] != 0 then
      yield (i, countFlips());
    
    if i != inds.high then
      nextPerm();
  }


  proc firstPerm() {
    var idx = inds.low;
    for i in 1..n-1 by -1 {
      const d = idx / fact(i);;
      count[i] = d;
      idx %= fact(i);

      pp[0..i] = p[0..i];

      for j in 0..i {
        if j + d <= i then
          p[j] = pp[j+d];
        else
          p[j] = pp[j+d-i-1];
      }
    }
  }

  proc countFlips() {
    var locflips = 1,
        first = p[0];

    if p[first] != 0 {
      for i in 0..#n do
        pp[i] = p[i];
      do {
        locflips += 1;
        var lo = 1, hi = first - 1;
        while lo < hi {
          pp[lo] <=> pp[hi];
          lo += 1;
          hi -= 1;
        }
        pp[first] <=> first;
      } while pp[first] != 0;
    }
    return locflips;
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
      const next = p[1];
      p[0] = p[1];
      for j in 1..i-1 do 
        p[j] = p[j+1];
      p[i] = first;
      first = next;
      
      count[i] += 1;
    }
  }
}

iter computeFact(n) {
  var f = 1;
  for i in 1..n {
    f *= i;
    yield f;
  }
}
