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
        on loc do
          __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
  }
  
}
