// MemTracking.chpl
//
module MemTracking
{
  config const
    memTrack: bool = false,
    memStats: bool = false,
    memLeaks: bool = false,
    memLeaksTable: bool = false,
    memMax: size_t = 0,
    memThreshold: size_t = 0,
    memLog: c_string = "";

  pragma "no auto destroy"
  config const
    memLeaksLog: c_string = "";

  // Globally accessible copy of the corresponding c_string consts
  use NewString;
  const s_memLog: string_rec = memLog;
  const s_memLeaksLog: string_rec = memLeaksLog;

  //
  // This communicates the settings of the various memory tracking
  // config consts to the runtime code that actually implements the
  // memory tracking.
  //
  // This function is a little tricky as it is called from every
  // locale from the runtime.  Recall that c_string is considered a
  // local-only data type, so we must use some tricks to copy the
  // c_string from locale 0 to the remote locales.  We use the globals
  // s_memLog and s_memLeaksLog to create global Chapel strings to
  // make them available to all locales.
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

    if (here.id != 0) {
      // These c_strings are going to be leaked
      if s_memLog.len != 0 then
        ret_memLog = remoteStringCopy(s_memLog.home.id,
                                      s_memLog.base,
                                      s_memLog.len);
      else ret_memLog = "";
      if s_memLeaksLog.len != 0 then
        ret_memLeaksLog = remoteStringCopy(s_memLeaksLog.home.id,
                                           s_memLeaksLog.base,
                                           s_memLeaksLog.len);
      else ret_memLeaksLog = "";
    } else {
      ret_memLog = memLog;
      ret_memLeaksLog = memLeaksLog;
    }
  }
}
