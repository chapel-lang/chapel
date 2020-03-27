// isx-bucket-spmd.chpl
// 
// This is a port of ISx to Chapel, co-developed by Brad Chamberlain,
// Lydia Duncan, and Jacob Hemstad on 2015-10-30.  Additional cleanups
// were done later by Brad.
//
// This version is SPMD, like the original port, but is written in terms
// of more abstract "buckets" rather than locales, permitting multiple
// buckets to be mapped to each locale, potentially.
//

//
// We want to use block-distributed arrays (BlockDist), barrier
// synchronization (Barriers), and timers (Time).
//
use BlockDist, Barriers, Time;

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

config const perBucketMultiply = here.maxTaskPar;
//
// The number of buckets per locale and total number of buckets.
//
const bucketsPerLocale = 1;
const numBuckets = numLocales * bucketsPerLocale;

//
// The total number of keys
//
config const totalKeys = if mode == scaling.strong then n
                                                   else n * numBuckets * perBucketMultiply;

//
// The number of keys per bucket -- this is approximate for strong
// scaling if the number of buckets doesn't divide 'n' evenly.
//
config const keysPerBucket = if mode == scaling.strong then n/numBuckets
                                                       else n * perBucketMultiply;


//
// The bucket width to use per bucket when running in weakISO mode
//
config const isoBucketWidth = if mode == scaling.weakISO then 8192 else 0;


//
// (Issue a warning if this has been set in modes other than weakISO)
//
if !quiet && mode != scaling.weakISO && isoBucketWidth != 0 then
  warning("Note that 'isoBucketWidth' only impacts weakISO scaling mode");

//
// The maximum key value to use.  When debugging, use a small size.
//
config const maxKeyVal = (if mode == scaling.weakISO 
                            then (numBuckets * perBucketMultiply * isoBucketWidth)
                            else (if testrun then 32 else 2**28)): keyType;

//
// When running in the weakISO scaling mode, the width of each bucket
// is fixed.  Otherwise, it's the largest key value divided by the
// number of buckets.
//
config const bucketWidth = if mode == scaling.weakISO
                             then isoBucketWidth * perBucketMultiply
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


const LocBucketSpace = {0..#numBuckets};
const BucketDist = new dmap(new Block(LocBucketSpace));
const BucketSpace = LocBucketSpace dmapped BucketDist;

var allBucketKeys: [BucketSpace] [0..#recvBuffSize] keyType;
var recvOffset: [BucketSpace] atomic int;
var totalTime, inputTime, bucketCountTime, bucketOffsetTime, bucketizeTime,
    exchangeKeysTime, countKeysTime: [BucketSpace] [1..numTrials] real;
var verifyKeyCount: atomic int;

var barrier = new Barrier(numBuckets);

proc main() {
  coforall bucketID in BucketSpace do
    on BucketDist.idxToLocale(bucketID) {
      //
      // Execution is SPMD-style across buckets here
      //

      //
      // The non-positive iterations represent burn-in runs, so don't
      // time those.  To reduce time spent in verification, verify only
      // the final timed run.
      //
      for i in 1-numBurnInRuns..numTrials do
        bucketSort(bucketID, trial=i, time=printTimings && (i>0),
                   verify=(i==numTrials));
    }

  if debug {
    writeln("final buckets =\n");
    for (i,b) in zip(BucketSpace, allBucketKeys) do
      writeln("Bucket ", i, " (owned by locale ", b.locale.id, "): ", b);
  }

  if printTimings then
    printTimingData("buckets");
}


proc bucketSort(bucketID, trial: int, time = false, verify = false) {
  const subtime = time && useSubTimers;
  var totalTimer: Timer;
  var subTimer: Timer;

  if time {
    totalTimer.start();
    if useSubTimers then
      subTimer.start();
  }

  var myKeys = makeInput(bucketID);

  if subtime {
    inputTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var bucketSizes = countLocalBucketSizes(myKeys);
  if debug then writeln(bucketID, ": bucketSizes = ", bucketSizes);

  if subtime {
    bucketCountTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var sendOffsets: [LocBucketSpace] int = + scan bucketSizes.read();
  sendOffsets -= bucketSizes.read();
  if debug then writeln(bucketID, ": sendOffsets = ", sendOffsets);

  if subtime {
    bucketOffsetTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var myBucketedKeys = bucketizeLocalKeys(bucketID, myKeys, sendOffsets);

  if subtime {
    bucketizeTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }
  
  exchangeKeys(bucketID, sendOffsets, bucketSizes, myBucketedKeys);
  barrier.barrier();

  if subtime {
    exchangeKeysTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  const keysInMyBucket = recvOffset[bucketID].read();
  var myLocalKeyCounts = countLocalKeys(bucketID, keysInMyBucket);

  if time {
    if subtime then
      countKeysTime.localAccess[here.id][trial] = subTimer.elapsed();
    totalTime.localAccess[here.id][trial] = totalTimer.elapsed();
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

    allBucketKeys[dstBucketID][dstOffset..#transferSize] = 
             myBucketedKeys[srcOffset..#transferSize];
  }

}


proc countLocalKeys(bucketID, myBucketSize) {
  const myMinKeyVal = bucketID * bucketWidth;
  var myLocalKeyCounts: [myMinKeyVal..#bucketWidth] atomic int;

  ref myBucket = allBucketKeys[bucketID];
  forall i in 0..#myBucketSize do
    myLocalKeyCounts[myBucket[i]].add(1);

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
    if !myKeys.contains(key) then
      halt("got key value outside my range: " + key:string + " not in " + myKeys:string);
  }

  //
  // verify that histogram sums properly
  //
  const myTotalLocalKeys = + reduce myLocalKeyCounts.read();
  if myTotalLocalKeys != myBucketSize then
    halt("local key count mismatch:" + myTotalLocalKeys:string + " != " + myBucketSize:string);

  //
  //
  //
  verifyKeyCount.add(myBucketSize);
  barrier.barrier();
  if verifyKeyCount.read() != totalKeys then
    halt("total key count mismatch: " + verifyKeyCount.read():string + " != " + totalKeys:string);

  if (!quiet && bucketID == 0) then
    writeln("\nVerification successful!");
}


proc makeInput(bucketID) {
  use Random;

  var myKeys: [0..#keysPerBucket] keyType;

  //
  // Seed RNG
  //
  if (debug) then
    writeln(here.id, ": Initializing random stream with seed = ", here.id);

  var pcg : pcg_setseq_64_xsh_rr_32_rng;
  const tid = bucketID:uint(64);
  const inc = pcg_getvalid_inc(tid);
  pcg.srandom(tid, inc);

  //
  // Fill local array
  //

  for key in myKeys do key = pcg.bounded_random(inc, maxKeyVal:uint(32)):keyType;

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
    if useSubTimers {
      printTimeTable(inputTime, units, "input");
      printTimeTable(bucketCountTime, units, "bucket count");
      printTimeTable(bucketOffsetTime, units, "bucket offset");
      printTimeTable(bucketizeTime, units, "bucketize");
      printTimeTable(exchangeKeysTime, units, "exchange");
      printTimeTable(countKeysTime, units, "count keys");
    }
    printTimeTable(totalTime, units, "total");
  }

  writeln();
  writeln("averages across ", units, " of min across trials (min..max):");
  if useSubTimers {
    printTimingStats(inputTime, "input");
    printTimingStats(bucketCountTime, "bucket count");
    printTimingStats(bucketOffsetTime, "bucket offset");
    printTimingStats(bucketizeTime, "bucketize");
    printTimingStats(exchangeKeysTime, "exchange");
    printTimingStats(countKeysTime, "count keys");
  }
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
// print timing statistics (avg/min/max across tasks and trials)
//
proc printTimingStats(timeTable, timerName) {
  var minTime = max(real),
      maxTime = min(real),
      totTime: real;

  //
  // iterate over the buckets, computing the min/max/total
  //
  forall timings in timeTable with (min reduce minTime,
                                    max reduce maxTime,
                                    + reduce totTime) {
    totTime += + reduce timings;
    minTime = min(minTime, min reduce timings);
    maxTime = max(maxTime, max reduce timings);
  }
  var avgTime = totTime / (numBuckets * numTrials) ;
  writeln(timerName, " = ", avgTime, " (", minTime, "..", maxTime, ")");
}
