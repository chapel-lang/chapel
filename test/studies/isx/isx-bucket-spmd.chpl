// isx-bucket-spmd.chpl
// 
// This is a port of ISx to Chapel, developed by Brad Chamberlain as a
// variant on the isx-spmd.chpl version co-developed by Brad Chamberlain,
// Lydia Duncan, and Jacob Hemstad.
//
// This version is SPMD, like the original port, but is written in terms
// of more abstract "buckets" rather than locales, permitting multiple
// buckets to be mapped to each locale, potentially.
//

//
// Top priorities:
// - timer insertion
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
             printConfig = !quiet;
  
//
// The following options control timing related information.
// - whether or not to use sub-timers to time each program phase
// - whether or not to print out (summary) timings
// - whether or not to print out full timing tables (when printing timings)
//
config const useSubTimers = false,
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
// TODO: permit these to be configurable
//
const bucketsPerLocale = 1;
const numBuckets = numLocales;

//
// The total number of keys
//
config const totalKeys = if mode == scaling.strong then n
                                                   else n * numBuckets;

//
// The number of keys per bucket -- this is approximate for strong
// scaling if the number of buckets doesn't divide 'n' evenly.
//
config const keysPerBucket = if mode == scaling.strong then n/numBuckets
                                                       else n;


//
// The bucket width to use per bucket when running in weakISO mode
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
                            then (numBuckets * isoBucketWidth)
                            else (if testrun then 32 else 2**28)): keyType;

//
// When running in the weakISO scaling mode, the width of each bucket
// is fixed.  Otherwise, it's the largest key value divided by the
// number of buckets.
//
config const bucketWidth = if mode == scaling.weakISO
                             then isoBucketWidth
                             else maxKeyVal/numBuckets;

//
// This tells how large the receive buffer should be relative to the
// average number of keys per bucket.
//
config const recvBuffFactor = 2.0,
             recvBuffSize = (keysPerBucket * recvBuffFactor): int;

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

const LocBucketSpace = {0..#numBuckets};
const BucketDist = new dmap(new Block(LocBucketSpace));
const BucketSpace = LocBucketSpace dmapped BucketDist;

var allBucketKeys: [BucketSpace] [0..#recvBuffSize] int;
var recvOffset: [BucketSpace] atomic int;
var totalTime: [BucketSpace] [1..numTrials] real;
var verifyKeyCount: atomic int;

//
// TODO: better name?
//
var barrier = new Barrier(numBuckets);

//
// TODO: introduce a main()
//

// TODO: This seems attractive, but also like it will break the barrier:
// forall bucketID in BucketSpace {
coforall bucketID in BucketSpace do
  on BucketDist.idxToLocale(bucketID) {
    //
    // Execution is SPMD-style across buckets here
    //
    // TODO: remove this
    //
    if allBucketKeys[bucketID][0].locale != here then
      warning("Need to distribute allBucketKeys");

    //
    // The non-positive iterations represent burn-in runs, so don't
    // time those.  To reduce time spent in verification, verify only
    // the final timed run.
    //
    for i in 1-numBurnInRuns..numTrials do
      //
      // TODO: Could make all other procedures nested in this loop
      // to let them refer to bucketID non-locally?  Or is that
      // crazy?
      //
      bucketSort(bucketID, i, time=(i>0), verify=(i==numTrials));
  }

if debug {
  writeln("final buckets =\n");
  for (i,b) in zip(BucketSpace, allBucketKeys) do
    writeln("Bucket ", i, " (owned by locale ", b.locale.id, "): ", b);
}
  
if printTimings then
  printTimingData("buckets");


proc bucketSort(bucketID, trial: int, time = false, verify = false) {
  var myKeys = makeInput(bucketID);
  var totalTimer: Timer;

  if time then
    totalTimer.start();

  var bucketSizes = countLocalBucketSizes(myKeys);
  if debug then writeln(bucketID, ": bucketSizes = ", bucketSizes);

  //
  // TODO: Should we be able to support scans on arrays of atomics without a
  // .read()?
  //
  // TODO: We really want an exclusive scan, but Chapel's is inclusive... :(
  // Weren't we planning on supporting both?
  //
  var sendOffsets: [LocBucketSpace] int = + scan bucketSizes.read();
  sendOffsets -= bucketSizes.read();
  if debug then writeln(bucketID, ": sendOffsets = ", sendOffsets);

  //
  // TODO: should we pass our globals into/out of these routines?
  //
  var myBucketedKeys = bucketizeLocalKeys(bucketID, myKeys, sendOffsets);
  exchangeKeys(bucketID, sendOffsets, bucketSizes, myBucketedKeys);

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

  const keysInMyBucket = recvOffset[bucketID].read();
  var myLocalKeyCounts = countLocalKeys(bucketID, keysInMyBucket);

  if time {
    totalTimer.stop();
    totalTime.localAccess[bucketID][trial] = totalTimer.elapsed();
  }    

  if (verify) then
    verifyResults(bucketID, keysInMyBucket, myLocalKeyCounts);

  //
  // reset the receive offsets for the next iteration
  //
  recvOffset[bucketID].write(0);
  barrier.barrier();
}


proc bucketizeLocalKeys(bucketID, myKeys, sendOffsets) {
  var bucketOffsets: [LocBucketSpace] atomic int;

  bucketOffsets.write(sendOffsets);

  var myBucketedKeys: [0..#keysPerBucket] keyType;
  
  forall key in myKeys {
    const bucketIndex = key / bucketWidth;
    const idx = bucketOffsets[bucketIndex].fetchAdd(1);
    myBucketedKeys[idx] = key; 
  }

  if debug then
    writeln(bucketID, ": myBucketedKeys = ", myBucketedKeys);

  return myBucketedKeys;
}


proc countLocalBucketSizes(myKeys) {
  // TODO: if adding numBuckets, change to that here
  var bucketSizes: [LocBucketSpace] atomic int;

  forall key in myKeys {
    const bucketIndex = key / bucketWidth;
    bucketSizes[bucketIndex].add(1);
  }

  return bucketSizes;
}


proc exchangeKeys(bucketID, sendOffsets, bucketSizes, myBucketedKeys) {
  forall rawDstBucketID in LocBucketSpace {
    //
    // perturb the destination bucket by our ID to avoid bottlenecks
    //
    const dstBucketID = (rawDstBucketID+bucketID) % numBuckets;
    const transferSize = bucketSizes[dstBucketID].read();
    const dstOffset = recvOffset[dstBucketID].fetchAdd(transferSize);
    const srcOffset = sendOffsets[dstBucketID];
    //
    // TODO: are we implementing this with one communication?
    // Answer (BenH): No, due to int(32)/int(64) mismatch.  Leaving
    // it as is now in order to measure the perf difference in the
    // test system.
    //
    allBucketKeys[dstBucketID][dstOffset..#transferSize] = 
             myBucketedKeys[srcOffset..#transferSize];
  }

}


proc countLocalKeys(bucketID, myBucketSize) {
  //
  // TODO: what if we used a global histogram here instead?
  // Note that if we did so and moved this outside of the coforall,
  // we could also remove the barrier from within the coforall
  //
  const myMinKeyVal = bucketID * bucketWidth;
  var myLocalKeyCounts: [myMinKeyVal..#bucketWidth] atomic int;
  //
  // TODO: Can we use a ref/array alias to myBucketKeys[bucketID] to avoid
  // redundant indexing?
  //
  forall i in 0..#myBucketSize do
    myLocalKeyCounts[allBucketKeys[bucketID][i]].add(1);

  if debug then
    writeln(bucketID, ": myLocalKeyCounts[", myMinKeyVal, "..] = ", 
            myLocalKeyCounts);

  return myLocalKeyCounts;
}


proc verifyResults(bucketID, myBucketSize, myLocalKeyCounts) {
  //
  // verify that all of my keys are in the expected range (myKeys)
  //
  const myMinKeyVal = bucketID * bucketWidth;
  const myKeys = myMinKeyVal..#bucketWidth;
  forall i in 0..#myBucketSize {
    const key = allBucketKeys[bucketID][i];
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

  if (!quiet && bucketID == 0) then
    writeln("\nVerification successful!");
}


proc makeInput(bucketID) {
  use Random.PCGRandom;

  var myKeys: [0..#keysPerBucket] keyType;

  //
  // Seed RNG
  //
  if (debug) then
    writeln(bucketID, ": Calling pcg32_srandom_r with ", bucketID);

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
    writeln(bucketID, ": myKeys: ", myKeys);

  return myKeys;
}


proc printConfiguration() {
  writeln("scaling mode = ", mode);
  writelnPotentialPowerOfTwo("total keys = ", totalKeys);
  writelnPotentialPowerOfTwo("keys per bucket = ", keysPerBucket);
  writelnPotentialPowerOfTwo("maxKeyVal = ", maxKeyVal);
  writelnPotentialPowerOfTwo("bucketWidth = ", bucketWidth);
  writeln("numTrials = ", numTrials);
  writeln("numBuckets = ", numBuckets);
}


proc writelnPotentialPowerOfTwo(desc, n) {
  write(desc, n);
  const lgn = log2(n);
  if 2**lgn == n then
    write(" (2**", lgn, ")");
  writeln();
}

//
// Print out timings for the run, if requested
//
proc printTimingData(units) {
  if printTimingTables {
    printTimeTable(totalTime, units, "total");
  }

  writeln();
  writeln("averages across ", units, " of min across trials:");
  printTimingStats(totalTime, "total");
}

//
// Print out one timer's table of locales/buckets x trials data
//
proc printTimeTable(timeTable, units, timerName) {
  //
  // print entire table, if user requested it
  //
  writeln();
  writeln(timerName, " time (", units, " / trials)");
  for (timings, i) in zip(timeTable, 0..) {
    write(i, ": ");
    for t in timings do
      write(t, "\t");
    writeln();
  }
}

//
// print timing statistics (currently only avg across locales/buckets
// of min across trials)
//
proc printTimingStats(timeTable, timerName) {
  //  var minMinTime: real = max(real);
  var /* maxMinTime, */ totMinTime: real;
  forall timings in timeTable with (/*min reduce minMinTime,
                                      max reduce maxMinTime,*/
                                    + reduce totMinTime) {
    totMinTime += min reduce timings;
  }
  var avgTime = totMinTime / numLocales;
  writeln(timerName, " = ", avgTime);
}
