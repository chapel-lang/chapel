// isx-spmd.chpl
// 
// This is a port of ISx to Chapel, co-developed by Brad Chamberlain,
// Lydia Duncan, and Jacob Hemstad on 2015-10-30.  Additional cleanups
// were done later by Brad.
//
// This version is based on the OpenSHMEM version available from:
//
//   https://github.com/ParRes/ISx
//
// This version is very literally SPMD, using a task per locale
// (and parallel loops to get multicore parallelism within each
// locale).
//

//
// Top priorities:
// - timer insertion and printout of timings
// - performance analysis / chplvis

// TODO:
// * variants that we should try
//   - multiple buckets per locale w/out any atomic ints
//   - some sort of hybrid between the current 1 bucket per locale scheme
//     and the previous?

//
// TODO: What are the potential performance issues?
// * put as one message?
//   - not currently happening due to int(32)/int(64) mismatch that
//     prevents direct copying -- will fix in future revision to
//     see performance impact.
// * how do Ben's locality optimizations do?
// * does returning arrays cost us anything?  Do we leak them?
// * other?
// * (this would be a good chplvis demo)
//

//
// We want to use block-distributed arrays (BlockDist), barrier
// synchronization (Barrier), and timers (Time).
//
use BlockDist, Barrier, Time;

//
// The type of key to use when sorting.
//
config type keyType = int(32);

//
// The following options respectively control...
// - whether or not to print debug information
// - whether or not to do a test run (results in small problem sizes)
// - whether or not to run quietly (squashes successful verification messages)
// - whether or not to print the execution time configuration
//
config const debug = false,
             testrun = debug,
             quiet = false,
             printConfig = !quiet,
             printTimings = !quiet,
             printTimingTables = false;


//
// Define three scaling modes: strong scaling, weak scaling, and
// weakISO (in which the width of the buckets is held constant).
//
enum scaling {strong, weak, weakISO};

//
// Which scaling mode should the program be run in?
//
config const mode = scaling.weak;

//
// The number of keys is defined in terms of 'n', though whether this
// represents the total number of keys or the number of keys per
// bucket depends on whether we're running in a strong or weak scaling
// mode.  When debugging, we run a smaller problem size.
//
config const n = if testrun then 32 else 2**27;

//
// The total number of keys
//
config const totalKeys = if mode == scaling.strong then n
                                                   else n * numLocales;

//
// The number of keys per locale -- this is approximate for strong
// scaling if the number of locales doesn't divide 'n' evenly.
//
config const keysPerLocale = if mode == scaling.strong then n/numLocales
                                                       else n;


//
// The bucket width to use per locale when running in weakISO mode
//
config const isoBucketWidth = if mode == scaling.weakISO then 8192 else 0;


//
// (Issue a warning if this has been set in modes other than weakISO)
//
if !quiet && mode != scaling.weakISO && isoBucketWidth != 0 then
  warning("Note that isoBucketWidth has no effect for weakISO scaling mode");

//
// The maximum key value to use.  When debugging, use a small size.
//
config const maxKeyVal = (if mode == scaling.weakISO 
                            then (numLocales * isoBucketWidth)
                            else (if testrun then 32 else 2**28)): keyType;

//
// When running in the weakISO scaling mode, the width of each bucket
// is fixed.  Otherwise, it's the largest key value divided by the
// number of locales.
//
config const bucketWidth = if mode == scaling.weakISO
                             then isoBucketWidth
                             else maxKeyVal/numLocales;

//
// This tells how large the receive buffer should be relative to the
// average number of keys per locale.
//
config const recvBuffFactor = 2.0,
             recvBuffSize = (keysPerLocale * recvBuffFactor): int;

//
// These specify the number of burn-in runs and number of experimental
// trials, respectively.  If the number of trials is zero, we exit
// after printing the configuration (useful for debugging problem size
// logic without doing anything intense).
//
config const numBurnInRuns = 1,
             numTrials = 1;


if printConfig then
  printConfiguration();

if numTrials == 0 then
  exit(0);



const OnePerLocale = LocaleSpace dmapped Block(LocaleSpace);

var allBucketKeys: [OnePerLocale] [0..#recvBuffSize] int;
var recvOffset: [OnePerLocale] atomic int;
var totalTime: [OnePerLocale] [1..numTrials] real;
var verifyKeyCount: atomic int;

//
// TODO: better name?
//
var barrier = new Barrier(numLocales);

coforall loc in Locales do
  on loc {
    //
    // SPMD-style across locales
    //
    // TODO: remove this once we feel confident it's working
    //
    if allBucketKeys[here.id][0].locale != here then
      warning("Need to distribute allBucketKeys");

    //
    // The non-positive iterations represent burn-in runs, so don't
    // time those.  To reduce time spent in verification, verify only
    // the final timed run.
    //
    for i in 1-numBurnInRuns..numTrials do
      bucketSort(trial=i, time=(i>0), verify=(i==numTrials));
  }

if debug {
  writeln("final buckets =\n");
  for (i,b) in zip(LocaleSpace, allBucketKeys) do
    writeln("Bucket ", i, " (owned by ", b.locale.id, "): ", b);
}

if printTimings {
  if printTimingTables {
    writeln("Locales / Trials");
    for i in OnePerLocale {
      write(here.id, ": ");
      for t in totalTime[i] {
        write(t, "\t");
      }
      writeln();
    }
  }
  var minMinTime: real = max(real);
  var maxMinTime, totMinTime: real;
  forall loc in OnePerLocale with (/*min reduce minMinTime,
                                     max reduce maxMinTime,*/
                                   + reduce totMinTime) {
    const minTime = min reduce totalTime[loc];
    totMinTime += minTime;
  }
  var avgTime = totMinTime / numLocales;
  //
  // TODO: Did I get that right?
  //
  writeln("average across locales of min time across trials = ",
          avgTime / numLocales);
}
  

proc bucketSort(trial: int, time = false, verify = false) {
  var myKeys = makeInput();
  var totalTimer: Timer;

  if time then
    totalTimer.start();

  var bucketSizes = countLocalBucketSizes(myKeys);
  if debug then writeln(here.id, ": bucketSizes = ", bucketSizes);

  //
  // TODO: Should we be able to support scans on arrays of atomics without a
  // .read()?
  //
  // TODO: We really want an exclusive scan, but Chapel's is inclusive... :(
  // Weren't we planning on supporting both?
  //
  var sendOffsets: [LocaleSpace] int = + scan bucketSizes.read();
  sendOffsets -= bucketSizes.read();
  if debug then writeln(here.id, ": sendOffsets = ", sendOffsets);

  //
  // TODO: should we pass our globals into/out of these routines?
  //
  var myBucketedKeys = bucketizeLocalKeys(myKeys, sendOffsets);
  exchangeKeys(sendOffsets, bucketSizes, myBucketedKeys);

  barrier.barrier();

  //
  // TODO: discussed with Jake a version in which the histogramming
  // (countLocalKeys) was done in parallel with the exchangeKeys;
  // the exchange keys task would write a "done"-style sync variable
  // when a put was complete and the task could begin aggressively
  // histogramming the next buffer's worth of data.  Use a cobegin
  // to kick off both of these tasks in parallel and know when they're
  // both done.
  //

  const keysInMyBucket = recvOffset[here.id].read();
  var myLocalKeyCounts = countLocalKeys(keysInMyBucket);

  if time {
    totalTimer.stop();
    totalTime[here.id][trial] = totalTimer.elapsed();
  }    

  if (verify) then
    verifyResults(keysInMyBucket, myLocalKeyCounts);

  //
  // reset the receive offsets for the next iteration
  //
  recvOffset[here.id].write(0);
  barrier.barrier();
}


proc bucketizeLocalKeys(myKeys, sendOffsets) {
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


proc countLocalBucketSizes(myKeys) {
  // TODO: if adding numBuckets, change to that here
  var bucketSizes: [LocaleSpace] atomic int;

  forall key in myKeys {
    const bucketIndex = key / bucketWidth;
    bucketSizes[bucketIndex].add(1);
  }

  return bucketSizes;
}


proc exchangeKeys(sendOffsets, bucketSizes, myBucketedKeys) {
  forall locid in LocaleSpace {
    //
    // perturb the destination locale by our ID to avoid bottlenecks
    //
    const dstlocid = (locid+here.id) % numLocales;
    const transferSize = bucketSizes[dstlocid].read();
    const dstOffset = recvOffset[dstlocid].fetchAdd(transferSize);
    const srcOffset = sendOffsets[dstlocid];
    //
    // TODO: are we implementing this with one communication?
    // Answer (BenH): No, due to int(32)/int(64) mismatch.  Leaving
    // it as is now in order to measure the perf difference in the
    // test system.
    //
    allBucketKeys[dstlocid][dstOffset..#transferSize] = 
             myBucketedKeys[srcOffset..#transferSize];
  }

}


proc countLocalKeys(myBucketSize) {
  //
  // TODO: what if we used a global histogram here instead?
  // Note that if we did so and moved this outside of the coforall,
  // we could also remove the barrier from within the coforall
  //
  const myMinKeyVal = here.id * bucketWidth;
  var myLocalKeyCounts: [myMinKeyVal..#bucketWidth] atomic int;
  //
  // TODO: Can we use a ref/array alias to myBucketKeys[here.id] to avoid
  // redundant indexing?
  //
  forall i in 0..#myBucketSize do
    myLocalKeyCounts[allBucketKeys[here.id][i]].add(1);

  if debug then
    writeln(here.id, ": myLocalKeyCounts[", myMinKeyVal, "..] = ", 
            myLocalKeyCounts);

  return myLocalKeyCounts;
}


proc verifyResults(myBucketSize, myLocalKeyCounts) {
  //
  // verify that all of my keys are in the expected range (myKeys)
  //
  const myMinKeyVal = here.id * bucketWidth;
  const myKeys = myMinKeyVal..#bucketWidth;
  forall i in 0..#myBucketSize {
    const key = allBucketKeys[here.id][i];
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

  if (!quiet && here.id == 0) then
    writeln("Verification successful!");
}


proc makeInput() {
  use Random.PCGRandom;

  var myKeys: [0..#keysPerLocale] keyType;

  //
  // Seed RNG
  //
  if (debug) then
    writeln(here.id, ": Calling pcg32_srandom_r with ", here.id);

  //
  // TODO: The reference version of ISx uses pcg32_srandom_r(), which
  // takes a second argument in addition to the seed to specify a stream
  // ID.  Should/can our PCG interface do that as well?
  //
  var MyRandStream = new PCGRandomStream(seed = here.id,
                                         parSafe=false,
                                         eltType = keyType);

  //
  // Fill local array
  //

  //
  // The following code ensures that the value we get from the stream
  // is in [0, maKeyVal) before storing it to key.  This is tricky when
  // maxKeyVal isn't a power of two and so we take some care to keep the
  // value distributed evenly.
  //
  // TODO: Double-check that I don't have an off-by-one error here.
  // Having one would not make the code break, but would imply
  // that we're not evenly distributing the random numbers.
  //
  // TODO: The C PCG takes care of this bounding automatically.  Should
  // our PCG interface do so as well?  (Can it if it's being called
  // in parallel?  Seems unlikely...)
  //
  const maxModdableKeyVal = max(keyType) - max(keyType)%maxKeyVal;
  for key in myKeys do
    do {
      key = MyRandStream.getNext();
      if key <= maxModdableKeyVal
        then key %= maxKeyVal;
    } while (key >= maxKeyVal || key < 0);
    
  if (debug) then
    writeln(here.id, ": myKeys: ", myKeys);

  return myKeys;
}


proc printConfiguration() {
  writeln("scaling mode = ", mode);
  writelnPotentialPowerOfTwo("total keys = ", totalKeys);
  writelnPotentialPowerOfTwo("keys per locale = ", keysPerLocale);
  writelnPotentialPowerOfTwo("maxKeyVal = ", maxKeyVal);
  writelnPotentialPowerOfTwo("bucketWidth = ", bucketWidth);
  writeln("numTrials = ", numTrials);
  writeln("numLocales = ", numLocales);
}


proc writelnPotentialPowerOfTwo(desc, n) {
  write(desc, n);
  const lgn = log2(n);
  if 2**lgn == n then
    write(" (2**", lgn, ")");
  writeln();
}
