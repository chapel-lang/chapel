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
  warning("Note that isoBucketWidth only impacts weakISO scaling mode");

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

var allBucketKeys: [OnePerLocale] [0..#recvBuffSize] keyType;
var recvOffset: [OnePerLocale] atomic int;
var totalTime, inputTime, bucketCountTime, bucketOffsetTime, bucketizeTime,
    exchangeKeysTime, countKeysTime: [OnePerLocale] [1..numTrials] real;
var verifyKeyCount: atomic int;

var barrier = new Barrier(numLocales);


coforall loc in Locales do
  on loc {
    //
    // Execution is SPMD-style across locales here
    //

    //
    // The non-positive iterations represent burn-in runs, so don't
    // time those.  To reduce time spent in verification, verify only
    // the final timed run.
    //
    for i in 1-numBurnInRuns..numTrials do
      bucketSort(trial=i, time=printTimings && (i>0), verify=(i==numTrials));

  }

if debug {
  writeln("final buckets =\n");
  for (i,b) in zip(LocaleSpace, allBucketKeys) do
    writeln("Bucket ", i, " (owned by ", b.locale.id, "): ", b);
}

if printTimings then
  printTimingData("locales");


proc bucketSort(trial: int, time = false, verify = false) {
  const subtime = time && useSubTimers;
  var totalTimer: Timer;
  var subTimer: Timer;

  if time {
    totalTimer.start();
    if useSubTimers then
      subTimer.start();
  }

  var myKeys = makeInput();

  if subtime {
    inputTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var bucketSizes = countLocalBucketSizes(myKeys);
  if debug then writeln(here.id, ": bucketSizes = ", bucketSizes);

  if subtime {
    bucketCountTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var sendOffsets: [LocaleSpace] int = + scan bucketSizes.read();
  sendOffsets -= bucketSizes.read();
  if debug then writeln(here.id, ": sendOffsets = ", sendOffsets);

  if subtime {
    bucketOffsetTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var myBucketedKeys = bucketizeLocalKeys(myKeys, sendOffsets);

  if subtime {
    bucketizeTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }
  
  exchangeKeys(sendOffsets, bucketSizes, myBucketedKeys);
  barrier.barrier();

  if subtime {
    exchangeKeysTime.localAccess[here.id][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  const keysInMyBucket = recvOffset[here.id].read();
  var myLocalKeyCounts = countLocalKeys(keysInMyBucket);

  if time {
    if subtime then
      countKeysTime.localAccess[here.id][trial] = subTimer.elapsed();
    totalTime.localAccess[here.id][trial] = totalTimer.elapsed();
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

    allBucketKeys[dstlocid][dstOffset..#transferSize] = 
             myBucketedKeys[srcOffset..#transferSize];
  }

}


proc countLocalKeys(myBucketSize) {
  const myMinKeyVal = here.id * bucketWidth;
  var myLocalKeyCounts: [myMinKeyVal..#bucketWidth] atomic int;

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
    writeln("\nVerification successful!");
}


proc makeInput() {
  use Random.PCGRandom;

  var myKeys: [0..#keysPerLocale] keyType;

  //
  // Seed RNG
  //
  if (debug) then
    writeln(here.id, ": Calling pcg32_srandom_r with ", here.id);

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
  writeln("averages across ", units, " of min across trials:");
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
