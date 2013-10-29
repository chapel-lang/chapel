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
}
