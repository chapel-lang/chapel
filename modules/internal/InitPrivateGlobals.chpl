//
// Initialize remote instances of globals declared with pragma "private"
//
pragma "no use ChapelStandard"
module InitPrivateGlobals {

  forall loc in Locales[1..numLocales-1] do on loc {
    chpl_defaultDistInitPrivate();
    chpl_rootLocaleInitPrivate();
  }

}
