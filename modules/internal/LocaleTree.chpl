// LocaleTree.chpl
//
// A tree of locales used for recursive task invocation during privatization
//
pragma "no use ChapelStandard"
module LocaleTree {

  use ChapelLocale; // For declaration of rootLocale.

  record chpl_localeTreeRecord {
    var left, right: locale;
  }

  pragma "private" var chpl_localeTree: chpl_localeTreeRecord;
  
  proc chpl_initLocaleTree() {
    for i in LocaleSpace {
      var left: locale = nil;
      var right: locale = nil;
      var child = (i+1)*2-1;    // Assumes that indices are dense.
      if child < numLocales {
        left = rootLocale.getChild(child);
        child += 1;
        if child < numLocales then
          right = rootLocale.getChild(child);
      }
      on rootLocale.getChild(i) {
        chpl_localeTree.left = left;
        chpl_localeTree.right = right;
      }
    }
  }
  
  chpl_initLocaleTree();
  
}
