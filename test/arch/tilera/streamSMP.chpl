// tilera.chpl
//

extern type cpu_set_t;
extern proc tmc_cpus_get_online_cpus(inout cpus:cpu_set_t): int;
extern proc tmc_cpus_count(inout cpus:cpu_set_t): int;

// A TileraPart is a partition of the whole Tilera chip.
class TileraPart : locale
{
  var my_cpus : cpu_set_t;
  var my_subloc_id : int;

  // This constructor takes a partition description string
  // and uses it to initialize the CPU set available to this partition.
  proc TileraPart(part_desc:string)
  {
    // Tilera();	// Base-class initializer call.
    chpl_id = __primitive("chpl_localeID");
    var cpus : cpu_set_t;
    numCores = 1;
    if tmc_cpus_get_online_cpus(cpus) == 0 then
      numCores = tmc_cpus_count(cpus);
    
    extern proc tmc_cpus_from_string(inout cpus:cpu_set_t, s:string):int;
    if tmc_cpus_from_string(my_cpus, part_desc) != 0 then
      halt("Problem converting Tilera CPU descriptor string to type cpu_set_t.");
  }

  proc TileraPart(cpu: int)
  {
    // Tilera();	// Base-class initializer call.
    chpl_id = __primitive("chpl_localeID");
    var cpus : cpu_set_t;
    numCores = 1;
    if tmc_cpus_get_online_cpus(cpus) == 0 then
      numCores = tmc_cpus_count(cpus);

    extern proc tmc_cpus_clear(inout cpus:cpu_set_t) : void;
    extern proc tmc_cpus_add_cpu(inout cpus:cpu_set_t, cpu:int(32)) : int;
    tmc_cpus_clear(my_cpus);
    if tmc_cpus_add_cpu(my_cpus, cpu:int(32)) != 0 then
      halt("Could not add to cpu set.");
  }

  proc initTask() : void
  {
    extern proc tmc_cpus_set_my_affinity(inout cpus:cpu_set_t) : int;
    if tmc_cpus_set_my_affinity(my_cpus) != 0 then
      halt("Problem setting CPU affinity for this task.");
  }

  proc readWriteThis(f) {
    extern proc tmc_cpus_get_my_current_cpu() : int;
    f <~> new ioLiteral("TileraPart ") <~> my_subloc_id <~> " CPU " <~> tmc_cpus_get_my_current_cpu();
  }
}

class Tilera : locale
{
  var sublocDom : domain(int);
  var subLocales : [sublocDom] locale;

  proc Tilera()
  {
    chpl_id = __primitive("chpl_localeID");
    var cpus : cpu_set_t;
    numCores = 1;
    if tmc_cpus_get_online_cpus(cpus) == 0 then
      numCores = tmc_cpus_count(cpus);
  }

  // Override the generic behavior.
  // Prints "NODE<i>" instead of "LOCALE<i>".
  proc readWriteThis(f) {
    // This prints out the actual node ID used by GASNet.
    f <~> new ioLiteral("NODE") <~> chpl_id;
  }

  proc addChild(child : locale) : void
  {
    var my_id = __primitive("_wide_get_subloc", child);
    sublocDom.add(my_id);
    subLocales[my_id] = child;
    var dcChild = child : TileraPart;
    dcChild.my_subloc_id = my_id;
  }

  proc getChild(subloc_id:int) : locale
  {
    // This code is sensitive to initialization ordering --
    // can be called before here is initialized.
    // The subloc_id > 0 test lets us slide past that (though we'd like a better solution).
    if subloc_id > 0 && sublocDom.member(subloc_id) then
      return subLocales[subloc_id];
    return this;
  }

  iter getChildren() : locale
  {
    for idx in sublocDom do
      yield subLocales[idx];
  }
}

//################################################################################

// Override default locales with our own.
// Note that we can initialize the locale array in parallel!

coforall loc in rootLocale.getLocales() do
  on loc do
    rootLocale.setLocale(loc.id, new Tilera());

// var my_subloc : locale = nil;

extern proc chpl_task_getSubLoc() : int(32);

for idx in {1..36} do
  on __primitive("chpl_on_locale_num", 0, idx)
  {
    var my_subloc = new TileraPart(idx % 36);
    here.addChild(my_subloc);
  }

// SURPRISE! iterators are statically bound.
//coforall loc in (here:Tilera).getChildren() do
//  on loc do
//    writeln(loc);

//################################################################################{
//# Streams copied verbatim here.
//#
use Time;
use Types;
use Random;

use HPCCProblemSize;


param numVectors = 3;
type elemType = real(64);

config const m = computeProblemSize(elemType, numVectors),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 0.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.currentTime else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;


proc main() {
  printConfiguration();

  const ProblemSpace: domain(1) = {1..m};
  var A, B, C: [ProblemSpace] elemType;

  initVectors(B, C);

  var execTime: [1..numTrials] real;

  for trial in 1..numTrials {
    const startTime = getCurrentTime();
//    [i in ProblemSpace] A(i) = B(i) + alpha * C(i);
    // Parallelize across sublocales
    coforall loc in (here:Tilera).getChildren() do
//      for t in {0..1} do
      {
        var chunk = getChunk((loc:TileraPart).my_subloc_id, 36, ProblemSpace);
        for i in chunk do
          A(i) = B(i) + alpha * C(i);
      }
    execTime(trial) = getCurrentTime() - startTime;
  }

  const validAnswer = verifyResults(A, B, C);
  printResults(validAnswer, execTime);
}


proc getChunk(nChunk:int, chunkCount, problemSpace)
{
  // The CPU number is one-based
  if nChunk < 1 || nChunk > chunkCount then
    halt("Invalid chunk number");
  var low = problemSpace.low;
  var high = problemSpace.high;
  var size = 1 + (high - low);
  var start = ((nChunk-1) * size) / chunkCount + low;
  var end = (nChunk * size) / chunkCount + low - 1;
  return {start..end};
}

proc printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}


proc initVectors(B, C) {
  var randlist = new RandomStream(seed);

  randlist.fillRandom(B);
  randlist.fillRandom(C);

  if (printArrays) {
    writeln("B is: ", B, "\n");
    writeln("C is: ", C, "\n");
  }

  delete randlist;
}


proc verifyResults(A, B, C) {
  if (printArrays) then writeln("A is: ", A, "\n");

  const infNorm = max reduce [i in A.domain] abs(A(i) - (B(i) + alpha * C(i)));

  return (infNorm <= epsilon);
}

// I key off the "~=" (approximately equals) to strip off lines that are expected
// to vary from run to run using a prediff file.
proc printResults(successful, execTimes) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    const totalTime = + reduce execTimes,
          avgTime = totalTime / numTrials,
          minTime = min reduce execTimes;
    writeln("Execution time:");
    writeln("  tot ~= ", totalTime);
    writeln("  avg ~= ", avgTime);
    writeln("  min ~= ", minTime);

    const GBPerSec = numVectors * numBytes(elemType) * (m/minTime) * 1.0e-9;
    writeln("Performance (GB/s) ~= ", GBPerSec);
  }
}

//################################################################################}

writeln("Done.");

