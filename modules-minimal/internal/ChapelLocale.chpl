// ChapelLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelLocale {
  //
  // The runtime calls these to adjust the running task count.
  //
  pragma "insert line file info"
  export
  proc chpl_taskRunningCntInc() { }

  pragma "insert line file info"
  export
  proc chpl_taskRunningCntDec() { }
}
