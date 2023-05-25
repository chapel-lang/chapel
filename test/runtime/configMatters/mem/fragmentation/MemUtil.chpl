use MemDiagnostics;

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

  var physMem = here.physicalMemory(unit = MemUnits.Bytes);
  if numBits(c_size_t) < 64 then
    physMem = min(physMem, 2**30);
  return physMem;
}
