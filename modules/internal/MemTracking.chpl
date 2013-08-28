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

  // This function is called from the first locale immediately before
  // initializing user modules
  proc chpl_startTrackingMemory() {
    // Note for historical purposes.  This was originally written with
    // a coforall loop, which was likely because it was done before
    // the existence of parallel iterators for DefaultRectangular.  A
    // forall loop makes more sense now.  In addition, there was a
    // conditional immediately in the coforall loop for the loc==here
    // (Locales[0]) case.  This was put in to disable the coforall+on
    // statement optimization, because the resulting non-blocking
    // implementation would require an a additional thread which did
    // not work in cases where the user limited the number of threads
    // to 1.
    forall loc in Locales do on loc {
      __primitive("chpl_setMemFlags", memTrack, memStats, memLeaks, memLeaksTable, memMax, memThreshold, memLog, memLeaksLog);
    }
  }
  
}
