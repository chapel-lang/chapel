// isx-hand-optimized.chpl
// 
// This is a port of ISx to Chapel, developed by Ben Harshbarger as a
// variant on the isx-bucket-spmd.chpl version co-developed by Brad Chamberlain,
// Lydia Duncan, and Jacob Hemstad.
//
// This version is based on the OpenSHMEM version available from:
//
//   https://github.com/ParRes/ISx
//
// This version is fully SPMD, creating a task per locale and a task
// per physical core on each locale. Of the current Chapel implementations
// of ISx, it is the most like the SHMEM reference version.
//

//
// We want to use block-distributed arrays (BlockDist), barrier
// synchronization (Barriers), and timers (Time).
//
use BlockDist, AllLocalesBarriers, Time;

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
// The total number of tasks across all locales
//
const numTasks = numLocales * perBucketMultiply;

//
// The total number of keys
//
config const totalKeys = if mode == scaling.strong then n
                                                   else n * numTasks;

//
// The number of keys per task -- this is approximate for strong
// scaling if the number of locales doesn't divide 'n' evenly.
//
// TODO: Should this really be a config const?
//
config const keysPerTask = if mode == scaling.strong then n/numTasks
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
                            then (numTasks * isoBucketWidth)
                            else (if testrun then 32 else 2**28)): keyType;

//
// When running in the weakISO scaling mode, the width of each bucket
// is fixed.  Otherwise, it's the largest key value divided by the
// number of locales.
//
config const bucketWidth = if mode == scaling.weakISO
                             then isoBucketWidth
                             else maxKeyVal/numTasks;

//
// This tells how large the receive buffer should be relative to the
// average number of keys per locale.
//
config const recvBuffFactor = 2.0,
             recvBuffSize = (keysPerTask * recvBuffFactor): int;

//
// These specify the number of burn-in runs and number of experimental
// trials, respectively.  If the number of trials is zero, we exit
// after printing the configuration (useful for debugging problem size
// logic without doing anything intense).
//
config const numBurnInRuns = 1,
             numTrials = 1;

// Horrible hack to help https://github.com/chapel-lang/chapel/issues/9414
record TimerArr {
  var A: [1..numTrials] real;
  proc this(i) ref { return A[i]; }
  // Forwarding these causes some kind of problem
  //iter these() ref { for a in A do yield a; }
  forwarding A only these;
}

if printConfig then
  printConfiguration();



const LocTaskSpace = {0..#numTasks};
const DistTaskSpace = LocTaskSpace dmapped Block(LocTaskSpace);

var allBucketKeys: [DistTaskSpace] [0..#recvBuffSize] keyType;
var recvOffset: [DistTaskSpace] atomic int;
var totalTime, inputTime, bucketCountTime, bucketOffsetTime, bucketizeTime,
    exchangeKeysTime, exchangeKeysOnlyTime, exchangeKeysBarrierTime,
    countKeysTime: [DistTaskSpace] TimerArr;
var verifyKeyCount: atomic int;

allLocalesBarrier.reset(perBucketMultiply);

// should result in one loop iteration per task
proc main() {
  coforall loc in Locales do on loc {
    coforall tid in 0..#perBucketMultiply {
      //
      // The non-positive iterations represent burn-in runs, so don't
      // time those.  To reduce time spent in verification, verify only
      // the final timed run.
      //
      const taskID = (loc.id * perBucketMultiply) + tid;
      for i in 1-numBurnInRuns..numTrials do
        bucketSort(taskID, trial=i, time=printTimings && (i>0), verify=(i==numTrials));
    }
  }

  if debug {
    writeln("final buckets =\n");
    for (i,b) in zip(LocTaskSpace, allBucketKeys) do
      writeln("Bucket ", i, " (owned by ", b.locale.id, "): ", b);
  }

  if printTimings then
    printTimingData("locales");
}


proc bucketSort(taskID : int, trial: int, time = false, verify = false) {
  const subtime = time && useSubTimers;
  var totalTimer: Timer;
  var subTimer: Timer;

  if time {
    totalTimer.start();
    if useSubTimers then
      subTimer.start();
  }

  var myKeys: [0..#keysPerTask] keyType;
  makeInput(taskID, myKeys);

  if subtime {
    inputTime.localAccess[taskID][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var bucketSizes: [0..#numTasks] int;
  local do countLocalBucketSizes(myKeys, bucketSizes);
  if debug then writeln(taskID, ": bucketSizes = ", bucketSizes);

  if subtime {
    bucketCountTime.localAccess[taskID][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var sendOffsets: [0..#numTasks] int = + scan bucketSizes;
  for (s, b) in zip(sendOffsets, bucketSizes) {
    s = s - b;
  }
  if debug then writeln(taskID, ": sendOffsets = ", sendOffsets);

  if subtime {
    bucketOffsetTime.localAccess[taskID][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  var myBucketedKeys: [0..#keysPerTask] keyType;
  local do bucketizeLocalKeys(taskID, myKeys, sendOffsets, myBucketedKeys);

  if subtime {
    bucketizeTime.localAccess[taskID][trial] = subTimer.elapsed();
    subTimer.clear();
  }
  
  exchangeKeys(taskID, sendOffsets, bucketSizes, myBucketedKeys);

  if subtime {
    exchangeKeysOnlyTime.localAccess[taskID][trial] = subTimer.elapsed();
    exchangeKeysTime.localAccess[taskID][trial] = subTimer.elapsed();
    subTimer.clear();
  }

  allLocalesBarrier.barrier();

  if subtime {
    exchangeKeysBarrierTime.localAccess[taskID][trial] = subTimer.elapsed();
    exchangeKeysTime.localAccess[taskID][trial] += subTimer.elapsed();
    subTimer.clear();
  }

  const keysInMyBucket = recvOffset[taskID].read();
  const myMinKeyVal = taskID * bucketWidth;
  var myLocalKeyCounts: [myMinKeyVal..#bucketWidth] int;
  local do countLocalKeys(taskID, keysInMyBucket, myLocalKeyCounts);

  if time {
    if subtime then
      countKeysTime.localAccess[taskID][trial] = subTimer.elapsed();
    totalTime.localAccess[taskID][trial] = totalTimer.elapsed();
  }    

  if (verify) then
    verifyResults(taskID, keysInMyBucket, myLocalKeyCounts);

  //
  // reset the receive offsets for the next iteration
  //
  recvOffset[taskID].write(0);
  allLocalesBarrier.barrier();
}


proc bucketizeLocalKeys(taskID, myKeys, sendOffsets, myBucketedKeys) {
  var bucketOffsets: [0..#numTasks] int;

  bucketOffsets = sendOffsets;

  for key in myKeys {
    const bucketIndex = key / bucketWidth;
    ref idx = bucketOffsets[bucketIndex];
    myBucketedKeys[idx] = key;
    idx += 1;
  }

  if debug then
    writeln(taskID, ": myBucketedKeys = ", myBucketedKeys);
}


proc countLocalBucketSizes(myKeys, bucketSizes) {
  for key in myKeys {
    const bucketIndex = key / bucketWidth;
    bucketSizes[bucketIndex] += 1;
  }
}


proc exchangeKeys(taskID, sendOffsets, bucketSizes, myBucketedKeys) {
  for locid in 0..#numTasks {
    //
    // perturb the destination locale by our ID to avoid bottlenecks
    //
    const dstlocid = (locid+taskID) % numTasks;
    const transferSize = bucketSizes[dstlocid];
    const dstOffset = recvOffset[dstlocid].fetchAdd(transferSize);
    const srcOffset = sendOffsets[dstlocid];

    //allBucketKeys[dstlocid][dstOffset..#transferSize] =
    //         myBucketedKeys[srcOffset..#transferSize];
    ref LHS = allBucketKeys[dstlocid]._instance;
    var Lidx = LHS.getDataIndex(dstOffset);
    var Ldata = _ddata_shift(keyType, LHS.theData, Lidx);

    var Ridx = myBucketedKeys._instance.getDataIndex(srcOffset);
    var Rdata = _ddata_shift(keyType, myBucketedKeys._instance.theData, Ridx);
    __primitive("chpl_comm_array_put", Rdata[0], Ldata.locale.id, Ldata[0], transferSize);
  }

}


proc countLocalKeys(taskID, myBucketSize, myLocalKeyCounts) {
  const myMinKeyVal = taskID * bucketWidth;

  ref myBucket = allBucketKeys[taskID];
  for i in 0..#myBucketSize do
    myLocalKeyCounts[myBucket[i]] += 1;

  if debug then
    writeln(taskID, ": myLocalKeyCounts[", myMinKeyVal, "..] = ", 
            myLocalKeyCounts);
}


proc verifyResults(taskID, myBucketSize, myLocalKeyCounts) {
  //
  // verify that all of my keys are in the expected range (myKeys)
  //
  const myMinKeyVal = taskID * bucketWidth;
  const myKeys = myMinKeyVal..#bucketWidth;
  ref myBucket = allBucketKeys[taskID];
  for i in 0..#myBucketSize {
    const key = myBucket[i];
    if !myKeys.contains(key) then
      halt("got key value outside my range: ", key, " not in ", myKeys);
  }

  //
  // verify that histogram sums properly
  //
  const myTotalLocalKeys = + reduce myLocalKeyCounts;
  if myTotalLocalKeys != myBucketSize then
    halt("local key count mismatch:", myTotalLocalKeys, " != ", myBucketSize);

  //
  //
  //
  verifyKeyCount.add(myBucketSize);
  allLocalesBarrier.barrier();
  if verifyKeyCount.read() != totalKeys then
    halt("total key count mismatch: ", verifyKeyCount.read(), " != ", totalKeys);

  if (!quiet && taskID == 0) then
    writeln("\nVerification successful!");
}


proc makeInput(taskID, myKeys) {
  use Random.PCGRandom;
  use Random.PCGRandomLib;

  //
  // Seed RNG
  //
  if (debug) then
    writeln(taskID, ": Initializing random stream with seed = ", taskID);

  var pcg : pcg_setseq_64_xsh_rr_32_rng;
  const tid = taskID:uint(64);
  const inc = pcg_getvalid_inc(tid);
  pcg.srandom(tid, inc);
  //
  // Fill local array
  //

  local {
    for key in myKeys do key = pcg.bounded_random(inc, maxKeyVal:uint(32)):keyType;
  }
    
  if (debug) then
    writeln(taskID, ": myKeys: ", myKeys);

}


proc printConfiguration() {
  writeln("scaling mode = ", mode);
  writelnPotentialPowerOfTwo("total keys = ", totalKeys);
  writelnPotentialPowerOfTwo("keys per bucket = ", keysPerTask);
  writelnPotentialPowerOfTwo("maxKeyVal = ", maxKeyVal);
  writelnPotentialPowerOfTwo("bucketWidth = ", bucketWidth);
  writeln("numTrials = ", numTrials);
  writeln("numBuckets = ", numTasks);
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
      printTimeTable(exchangeKeysOnlyTime, units, "exchange only");
      printTimeTable(exchangeKeysBarrierTime, units, "exchange barrier");
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
    printTimingStats(exchangeKeysOnlyTime, "exchange only");
    printTimingStats(exchangeKeysBarrierTime, "exchange barrier");
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
  var avgTime = totTime / (numTasks * numTrials) ;
  writeln(timerName, " = ", avgTime, " (", minTime, "..", maxTime, ")");
}
