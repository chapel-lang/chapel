use MemDiagnostics, Time, CTypes;

config const oversubscription = 4;
const desiredTasks = oversubscription * here.maxTaskPar;

// At most use 1/memFraction of available/addressable memory
var memAvail = availMem();
if numBits(c_size_t) < 64 then
  memAvail = min(memAvail, 2**30);

config const memFraction = 10;
config var maxMem = memAvail / memFraction;
const arrSize = maxMem / desiredTasks;

extern proc chpl_comm_regMemAllocThreshold(): c_size_t;
extern var SIZE_MAX: c_size_t;
if chpl_comm_regMemAllocThreshold() != SIZE_MAX then
  if chpl_comm_regMemAllocThreshold():int > arrSize then
    writeln("Warning: Array size is too small to test registered allocation");

type elemType = int;
const n = arrSize / numBytes(elemType);


config const showPerf = false;
var timer: stopwatch;

timer.start();
coforall loc in Locales do on loc {
  coforall task in 1..desiredTasks {
    var A: [1..n] elemType;
  }
}
timer.stop();

if showPerf {
  writeln("Array size in bytes: ", arrSize);
  writeln("Time: ", timer.elapsed());
}

// Estimate for how much memory we can allocate. Based on
// chpl_comm_regMemHeapInfo if using a fixed heap, otherwise physical memory
proc availMem() {
  use CTypes;
  extern proc chpl_comm_regMemHeapInfo(ref start: c_void_ptr, ref size: c_size_t): void;
  var unused: c_void_ptr;
  var heap_size: c_size_t;
  chpl_comm_regMemHeapInfo(unused, heap_size);
  if heap_size != 0 then
    return heap_size.safeCast(int);
  return here.physicalMemory(unit = MemUnits.Bytes);
}
