use Random;
use List;
use Set;
use SHA256Implementation;

config const M = 64; // Number of possible integers for collision
config const N = 2 * 1024 * 1024; // Number of iterations 
var useSHASeeds:bool;
var useSHA:bool;
var useTop:bool;
config const verbose = false;
config const skipSHA = false;


proc sha256hash(a:uint, b:uint=0, c:uint=0, d:uint=0,
                e:uint=0, f:uint=0, g:uint=0, h:uint=0) {
  var msg:16*uint(32);
  var state:SHA256State;

  msg[ 0] = (a >> 32         ):uint(32);
  msg[ 1] = (a & 0xffffffff  ):uint(32);
  msg[ 2] = (b >> 32         ):uint(32);
  msg[ 3] = (b & 0xffffffff  ):uint(32);
  msg[ 4] = (c >> 32         ):uint(32);
  msg[ 5] = (c & 0xffffffff  ):uint(32);
  msg[ 6] = (d >> 32         ):uint(32);
  msg[ 7] = (d & 0xffffffff  ):uint(32);
  msg[ 8] = (e >> 32         ):uint(32);
  msg[ 9] = (e & 0xffffffff  ):uint(32);
  msg[10] = (f >> 32         ):uint(32);
  msg[11] = (f & 0xffffffff  ):uint(32);
  msg[12] = (g >> 32         ):uint(32);
  msg[13] = (g & 0xffffffff  ):uint(32);
  msg[14] = (h >> 32         ):uint(32);
  msg[15] = (h & 0xffffffff  ):uint(32);

  var hash:8*uint(32) = state.lastblock(msg, 16*32);
  var ret: uint(64);
  ret = hash(1);
  ret <<= 32;
  ret |= hash(2);
  return ret;
}


proc shrink(x:uint) {
  const Muint = M:uint;
  if useTop then
    return (x >> 58):int;
  else
    return (x%Muint):int;
}

proc doHash(x:int) {
  if useSHA {
    return sha256hash(x:uint);
  } else {
    return chpl__defaultHash(x);
  }
}

proc doCombine(a, b, n) {
  if useSHA {
    return sha256hash(a:uint, b:uint, n:uint);
  } else {
    return chpl__defaultHashCombine(a, b, n);
  }
}

proc printHisto(counts) {
  for (i, count) in zip(counts.domain, counts) {
    if count != 0 then
      writef("[% 8i] = %i\n", i, count);
  }
}

proc summarize(histo) {
  var theMin = max(int);
  var theMax = min(int);
  var theAvg = 0;
  for count in histo {
    if count < theMin then
      theMin = count;
    if count > theMax then
      theMax = count;
    theAvg += count;
  }
  theAvg = round(theAvg * 1.0 / histo.size):int;
  return (theMin, theAvg, theMax);
}

proc check(name:string, histo, alwaysPrintAvg:bool = false) {
  var (theMin, theAvg, theMax) = summarize(histo);
  if verbose then
    writef("% 32s  min=% 8i  avg=% 8i  max=% 8i\n",
           name, theMin, theAvg, theMax);
  else if alwaysPrintAvg then
    writef("% 32s  avg=% 8i\n", name, theAvg);

  // This comparison is simply a heuristic and is not
  // based on concrete statistics; just trying to get
  // the test to fail if the statistics become really bad.
  if theMax > 10 && theMax > 4*theAvg then
    writeln(" SPATIAL ANOMALY DETECTED in test ", name);
}

proc runTest() {
  
  var seeds : [1..3] uint(64);

  var topBits = 1 + log2(N);
  var botBits = topBits;
  assert(topBits < 32);
  var nTops = 1 << topBits;
  var nBots = 1 << botBits;
  var topCounts:[0..nTops] int;
  var botCounts:[0..nBots] int;

  var counts0: [0..#M] int;
  var counts1: [0..#M] int;
  var counts2: [0..#M] int;
  var counts3: [0..#M] int;

  // pairs[j,k] = # of collisions of 2 hashes
  var pairs : [0..#M, 0..#M] int;
  var pairsInput: [0..#M, 0..#M] list(int);
  // triples[j,k,l] = # of collisions of 3 hashes
  var triples : [0..#M, 0..#M, 0..#M] int(64);

  var hashSet:set(uint);

  if useSHA || useSHASeeds {
    for i in 1..3 {
      seeds[i] = sha256hash(i:uint);
    }
  } else {
    var rng = new RandomStream(uint(64), seed=0x1BAD5EED);
    rng.fillRandom(seeds);
  }

  for i in 1..N {
    var hash = doHash(i);
    counts0[shrink(hash)] += 1;
    if hashSet.contains(hash) {
      // Error
      writeln(" SPATIAL ANOMALY DETECTED: hashSet contains duplicate");
    }
    hashSet.add(hash);

    // check top bits & bottom bits
    var top = hash >> (64 - topBits);
    var ones = ~(0:uint);
    var bot = hash & (ones >> (64 - botBits));
    topCounts[top:int] += 1;
    botCounts[bot:int] += 1;
    
    var j = doCombine(hash, seeds[1], 1);
    counts1[shrink(j)] += 1;
    var k = doCombine(hash, seeds[2], 2);
    counts2[shrink(k)] += 1;
    var l = doCombine(hash, seeds[3], 3);
    counts3[shrink(l)] += 1;

    pairs[shrink(j), shrink(k)] += 1;
    pairsInput[shrink(j), shrink(k)].append(i);

    triples[shrink(j), shrink(k), shrink(l)] += 1;
  }

  check("chpl__defaultHash of 1..N", counts0);

  check("hash top " + topBits:string + " bits", topCounts,    true);
  check("hash bottom " + botBits:string + " bits", botCounts, true);

  check("HashCombine with seed 1", counts1);

  check("HashCombine with seed 2", counts2);

  check("HashCombine with seed 3", counts3);

  check("Pairs", pairs, true);
  /*for (i, count) in zip(pairs.domain, pairs) {
    if count != 0 {
      writef("[% 8t] = %i ", i, count);
      var n = pairsInput[i].size;
      if n > 10 then n = 10;
      for j in 1..n {
        writef("%016xu ", pairsInput[i][j]);
      }
      writeln();
    }
  }*/


  check("Triples", triples, true);
  /*
  for ((j,k,l), x) in zip(collisions.domain, collisions) {
          if collisions[j,k,l] != 0 then 
                  writeln("collisions[", j, ", ", k, ", ", l, "] = ", collisions[j,k,l]);  
  }

  var hasZero : int;
  var notHasZero : int;
  for i in collisions do if i == 0 then hasZero += 1; else notHasZero += 1;
  writeln("hasZero = ", hasZero, ", notHasZero = ", notHasZero);
  */
}

if skipSHA == false {
  useSHASeeds = true;
  useSHA = true;
  useTop = false;
  writeln("Using SHA256, modulus");
  runTest();
  writeln();

  useSHASeeds = true;
  useSHA = true;
  useTop = true;
  writeln("Using SHA256, shift top down");
  runTest();
  writeln();
}


useSHASeeds = true;
useSHA = false;
useTop = false;
writeln("Using SHA256 seeds, defaultHash, defaultHashCombine, modulus");
runTest();
writeln();

useSHASeeds = true;
useSHA = false;
useTop = true;
writeln("Using SHA256 seeds, defaultHash, defaultHashCombine, shift top down");
runTest();
writeln();

useSHASeeds = false;
useSHA = false;
useTop = false;
writeln("Using PCG seeds, defaultHash, defaultHashCombine, modulus");
runTest();
writeln();

useSHASeeds = false;
useSHA = false;
useTop = true;
writeln("Using PCG seeds, defaultHash, defaultHashCombine, shift top down");
runTest();
writeln();


