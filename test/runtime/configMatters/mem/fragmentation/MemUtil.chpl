use Memory.Diagnostics;

// Estimate for how much memory we can allocate. Based on
// chpl_comm_regMemHeapInfo if using a fixed heap, otherwise physical memory
proc availMem() {
  use CTypes;
  extern proc chpl_comm_regMemHeapInfo(ref start: c_void_ptr, ref size: size_t): void;
  var unused: c_void_ptr;
  var heap_size: size_t;
  chpl_comm_regMemHeapInfo(unused, heap_size);
  if heap_size != 0 then
    return heap_size.safeCast(int);
  return here.physicalMemory(unit = MemUnits.Bytes);
}
