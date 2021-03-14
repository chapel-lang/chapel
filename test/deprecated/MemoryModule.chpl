import Memory;

proc test() {
  var memHere = here.physicalMemory();
  var memUsed = Memory.memoryUsed();
  Memory.printMemAllocs();
  Memory.printMemAllocsByType();
  Memory.printMemAllocStats();
  Memory.startVerboseMem();
  Memory.stopVerboseMem();
  Memory.startVerboseMemHere();
  Memory.stopVerboseMemHere();
}
test();

