// MemTracking.chpl
//
pragma "no use ChapelStandard"
module MemTracking
{
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
                                         ref ret_memMax: uint(64),       // **
                                         ref ret_memThreshold: uint(64), // **
                                         ref ret_memLog: c_string,
                                         ref ret_memLeaksLog: c_string) {

    // ** In minimal-modules mode, I've hard-coded these size_t
    // arguments to uint(64) rather than using the size_t aliases
    // in SysCTypes.chpl because doing that requires dragging in a
    // bunch of other ChapelBase code.  My assumption here is that
    // size_t will either be, or be compatible with, uint(64) for
    // most developers.  If that turns out not to be the case, we
    // can reconsider this choice.
  }
}
