// isx.chpl
// 
// Port of ISx to Chapel, co-developed by Brad Chamberlain,
// Lydia Duncan, and Jacob Hemstad on 2015-10-30.  Based on
// the OpenSHMEM version available from:
//
//   https://github.com/ParRes/ISx
//


// TODO:
// * variants that we should try
//   - multiple buckets per locale w/out any atomic ints
//   - some sort of hybrid between the current 1 bucket per locale scheme
//     and the previous?

use BlockDist, Barrier;

//enum scaling {strong, weak, weakiso, debug};

//const ScalingSpace = domain(scaling);


/*
config const keysPer = 2**23

const defaultTotalKeys: [ScalingSpace] int = [keysPer, 
                                              keysPer*numLocales, 
                                              keysPer*numLocales,
                                              32];
*/

config type keyType = int(32);

// TODO: replace 'numLocales' below with 'numBuckets' ??

config const // scalingMode = scaling.debug,
             keysPerLocale = 32, //2**27,
             totalKeys = keysPerLocale*numLocales,
             maxKeyVal = 32, // 2**23,
             bucketWidth = maxKeyVal / numLocales,
             recvBuffFactor = 2;

config const numBurnInRuns = 1,
             numTrials = 1;

config const printConfig = true,
             debug = false;

if printConfig {
  writeln("total keys = ", totalKeys);
  writeln("keys per locale = ", keysPerLocale);
  writeln("bucketWidth = ", bucketWidth);
  writeln("maxKeyVal = ", maxKeyVal);
  writeln("numLocales = ", numLocales);
}

//
// TODO: Need to make this distributed
//
const OnePerLocale = LocaleSpace dmapped Block(LocaleSpace);

var myBucketKeys: [OnePerLocale] [0..#totalKeys*recvBuffFactor] int;
var recvOffset: [OnePerLocale] atomic int;
var verifyKeyCount: atomic int;

// TODO: better name?
var barrier = new Barrier(numLocales);

coforall loc in Locales do
  on loc {
    // SPMD here
    if myBucketKeys[here.id][0].locale != here then
      warning("Need to distribute myBucketKeys");
    for i in 1..numBurnInRuns do
      bucketSort();
    for i in 1..numTrials {
      // TODO: Timing stuff
      bucketSort(verify = i==numTrials);
    }
  }

if debug {
  writeln("myBucketKeys =\n");
  for i in LocaleSpace do
    writeln(myBucketKeys[i]);
}
  

proc bucketSort(verify = false) {
  var myKeys = makeInput();

  var bucketSizes = countLocalBucketSizes(myKeys);
  if debug then writeln(here.id, ": bucketSizes = ", bucketSizes);

  //
  // TODO: Should we be able to support scans on arrays of atomics without a
  // .read()?
  //
  // TODO: We really want an exclusive scan, but Chapel's is inclusive... :(
  //
  var sendOffsets: [LocaleSpace] int = + scan bucketSizes.read();
  sendOffsets -= bucketSizes.read();
  if debug then writeln(here.id, ": sendOffsets = ", sendOffsets);

  //
  // TODO: should we pass our globals into/out of these routines
  //
  var myBucketedKeys = bucketizeLocalKeys(myKeys, sendOffsets);
  exchangeKeys(sendOffsets, bucketSizes, myBucketedKeys);

  barrier.barrier();

  const keysInMyBucket = recvOffset[here.id].read();
  var myLocalKeyCounts = countLocalKeys(keysInMyBucket);

  if (verify) then
    verifyResults(keysInMyBucket, myLocalKeyCounts);

  //
  // reset for next iteration
  //
  recvOffset[here.id].write(0);
  barrier.barrier();
}


// TODO: Is all this returning of local arrays going to cause problems?


//
// TODO: introduce BucketSpace domain instead of LocaleSpace
//
// const BucketSpace = {0..#numBuckets);

inline proc bucketizeLocalKeys(myKeys, sendOffsets) {
  var bucketOffsets: [LocaleSpace] atomic int;

  bucketOffsets.write(sendOffsets);

  var myBucketedKeys: [0..#keysPerLocale] keyType;
  
  forall key in myKeys {
    const bucketIndex = key / bucketWidth;
    const idx = bucketOffsets[bucketIndex].fetchAdd(1);
    myBucketedKeys[idx] = key; 
  }

  if debug then
    writeln(here.id, ": myBucketedKeys = ", myBucketedKeys);

  return myBucketedKeys;
}


inline proc countLocalBucketSizes(myKeys) {
  // TODO: if adding numBuckets, change to that here
  var bucketSizes: [LocaleSpace] atomic int;

  forall key in myKeys {
    const bucketIndex = key / bucketWidth;
    bucketSizes[bucketIndex].add(1);
  }

  return bucketSizes;
}

// TODO: does emacs not highlight 'here'?


inline proc exchangeKeys(sendOffsets, bucketSizes, myBucketedKeys) {
  forall locid in LocaleSpace {
    //
    // perturb the destination locale by our ID to avoid bottlenecks
    //
    const dstlocid = (locid+here.id) % numLocales;
    const transferSize = bucketSizes[dstlocid].read();
    const dstOffset = recvOffset[dstlocid].fetchAdd(transferSize);
    const srcOffset = sendOffsets[dstlocid];
    myBucketKeys[dstlocid][dstOffset..#transferSize] = 
            myBucketedKeys[srcOffset..#transferSize];
  }

}


inline proc countLocalKeys(myBucketSize) {
  // TODO: what if we used a global histogram here instead?
  // Note that if we did so and moved this outside of the coforall,
  // we could also remove the barrier from within the coforall
  const myMinKeyVal = here.id * bucketWidth;
  var myLocalKeyCounts: [myMinKeyVal..#bucketWidth] atomic int;
  //
  // TODO: Can we use a ref/array alias to myBucketKeys[here.id] to avoid
  // redundant indexing?
  //
  forall i in 0..#myBucketSize do
    myLocalKeyCounts[myBucketKeys[here.id][i]].add(1);

  if debug then
    writeln(here.id, ": myLocalKeyCounts[", myMinKeyVal, "..] = ", 
            myLocalKeyCounts);

  return myLocalKeyCounts;
}

inline proc verifyResults(myBucketSize, myLocalKeyCounts) {
  //
  // verify that all of my keys are in the expected range (myKeys)
  //
  const myMinKeyVal = here.id * bucketWidth;
  const myKeys = myMinKeyVal..#bucketWidth;
  forall i in 0..#myBucketSize {
    const key = myBucketKeys[here.id][i];
    if !myKeys.member(key) then
      halt("got key value outside my range: "+key + " not in " + myKeys);
  }

  //
  // verify that histogram sums properly
  //
  const myTotalLocalKeys = + reduce myLocalKeyCounts.read();
  if myTotalLocalKeys != myBucketSize then
    halt("local key count mismatch:" + myTotalLocalKeys + " != " + myBucketSize);

  //
  //
  //
  verifyKeyCount.add(myBucketSize);
  barrier.barrier();
  if verifyKeyCount.read() != totalKeys then
    halt("total key count mismatch: " + verifyKeyCount.read() + " != " + totalKeys);
}


inline proc makeInput() {
  //
  // TODO: can we get this to work?
  // extern {
  // #include "pcg_basic.h"
  // }
  require "pcg_basic.h", "pcg_basic.c";

  extern type pcg32_random_t;
  extern proc pcg32_srandom_r(ref rng: pcg32_random_t, 
                              initstate: uint(64),
                              initseq: uint(64));
  extern proc pcg32_boundedrand_r(ref rng: pcg32_random_t, 
                                  bound: uint(32)
                                 ): uint(32);

  var rng: pcg32_random_t;
  var myKeys: [0..#keysPerLocale] keyType;

  //
  // Seed RNG
  //
  if (debug) then
    writeln(here.id, ": Calling pcg32_srandom_r with ", here.id);

  pcg32_srandom_r(rng, here.id:uint(64), here.id:uint(64));


  //
  // Fill local array
  //
  for key in myKeys do
    key = pcg32_boundedrand_r(rng, maxKeyVal.safeCast(uint(32))).safeCast(keyType);

  if (debug) then
    writeln(here.id, ": myKeys: ", myKeys);

  return myKeys;
}

             
/*
const keysPerLocale = totalKeys / 

                                                      
proc defaultBucketWidth(mode: scaling) {
  select (mode) {
  when mode.strong:
  when mode.weak:
  when mode.weakiso:
  when mode.debug:
    return 32;
  otherwise:
    halt("Unexpected scaling mode in defaultMaxKeyVal()");
  }
}
                                                      
proc defaultMaxKeyVal(mode: scaling) {
  select (mode) {
  when mode.strong:
  when mode.weak:
  when mode.weakiso:
  when mode.debug:
    return 32;
  otherwise:
    halt("Unexpected scaling mode in defaultMaxKeyVal()");
  }
}

*/