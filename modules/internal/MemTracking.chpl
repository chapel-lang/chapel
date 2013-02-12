// MemTracking.chpl
//
pragma "no use ChapelStandard"
module MemTracking
{
  config const
    memTrack: bool = false,
    memStats: bool = false, 
    memLeaks: bool = false,
    memLeaksTable: bool = false,
    memMax: int = 0,
    memThreshold: int = 0,
    memLog: string = "";
  
  pragma "no auto destroy"
  config const
    memLeaksLog: string = "";
  
  proc chpl_startTrackingMemory() {
    if Locales[0] == here then
      coforall loc in Locales do
//
// This code does not work because there is a race condition between starting
// memory tracking on node 0 and the allocation/deallocation of the task list
// at the end of _waitEndCount().
// Running the enable code on node 0 without "on loc do" causes the execution of the primitive
// and the coforall cleanup to run serially (in that order), thus avoiding the race.
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//        on loc do
//          __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
        if loc == here {
          __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
        } else on loc {
          __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
        }
  }
  
}
