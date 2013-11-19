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
    memMax: size_t = 0,
    memThreshold: size_t = 0,
    memLog: string = "";
  
  pragma "no auto destroy"
  config const
    memLeaksLog: string = "";

  //
  // This communicates the settings of the various memory tracking
  // config consts to the runtime code that actually implements the
  // memory tracking.
  //
  export
  proc chpl_memTracking_returnConfigVals(ref ret_memTrack: bool,
                                         ref ret_memStats: bool,
                                         ref ret_memLeaks: bool,
                                         ref ret_memLeaksTable: bool,
                                         ref ret_memMax: size_t,
                                         ref ret_memThreshold: size_t,
                                         ref ret_memLog: c_string,
                                         ref ret_memLeaksLog: c_string) {
    ret_memTrack = memTrack;
    ret_memStats = memStats;
    ret_memLeaks = memLeaks;
    ret_memLeaksTable = memLeaksTable;
    ret_memMax = memMax;
    ret_memThreshold = memThreshold;
    ret_memLog = __primitive("c_string_from_string", memLog);
    ret_memLeaksLog = __primitive("c_string_from_string", memLeaksLog);
  }
}
