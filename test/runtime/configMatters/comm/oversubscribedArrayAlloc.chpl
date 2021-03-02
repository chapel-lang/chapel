use Memory.Diagnostics, Time, SysCTypes;

config const oversubscription = 4;
const desiredTasks = oversubscription * here.maxTaskPar;

// At most use 1/memFraction of available/addressable memory
var memAvail = here.physicalMemory(unit=MemUnits.Bytes);
if numBits(size_t) < 64 then
  memAvail = min(memAvail, 2**30);

config const memFraction = 10;
config var maxMem = memAvail / memFraction;
const arrSize = maxMem / desiredTasks;

extern proc chpl_comm_regMemAllocThreshold(): size_t;
extern var SIZE_MAX: size_t;
if chpl_comm_regMemAllocThreshold() != SIZE_MAX then
  if chpl_comm_regMemAllocThreshold():int > arrSize then
    writeln("Warning: Array size is too small to test registered allocation");

type elemType = int;
const n = arrSize / numBytes(elemType);


config const showPerf = false;
var timer: Timer;

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
