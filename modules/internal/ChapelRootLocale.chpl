// ChapelRootLocale.chpl
//
pragma "no use ChapelStandard"
module ChapelRootLocale {
  use ChapelLocale;
  use DefaultRectangular;

class RootLocale
{
  // would like this to be the following, but it breaks about 20 tests:
  //const LocaleSpace: domain(1) distributed(OnePer) = [0..numLocales-1];
  const LocaleSpace: domain(1) = {0..numLocales-1};
  const Locales: [LocaleSpace] locale;

  proc setLocale(idx:int, loc:locale)
  {
    on __primitive("chpl_on_locale_num", idx)
    {
      // When you use "on <object>", this goes to the locale
      // represented by the locale field of the wide pointer used to store
      // the object's address.  Therefore, we expect the locale ID associated
      // with the passed-in loc to equal the ID of the current node.
      if __primitive("chpl_localeID") != __primitive("_wide_get_node", loc) then
        halt(".locale field of locale object must match node ID");
      _here = loc;
    }
    Locales[idx] = loc;
  }

  // We cannot use a forall here because the default leader iterator will
  // access data structures that are not yet initialized (i.e., Locales
  // array/here).  An alternative would be to use a coforall+on and refactor
  // chpl_setupLocale().
  proc RootLocale()
  {
    for locIdx in LocaleSpace do
      on __primitive("chpl_on_locale_num", locIdx)
      {
        _here = new locale();
        Locales[locIdx] = _here;
      }
  }

  proc getLocaleSpace() return this.LocaleSpace;
  proc getLocales() return this.Locales;
  proc getLocale(idx:int) return this.Locales[idx];
}

var rootLocale = new RootLocale();

//proc locale.numCores {
//  var numCores: int;
//  on this do numCores = __primitive("chpl_coresPerLocale");
//  return numCores;
//}

proc chpl_int_to_locale(id) {
  return rootLocale.getLocale(id);
}


//
// tree for recursive task invocation during privatization
//
record chpl_localeTreeRecord {
  var left, right: locale;
}
pragma "private" var chpl_localeTree: chpl_localeTreeRecord;

proc chpl_initLocaleTree() {
  for i in rootLocale.getLocaleSpace() {
    var left: locale = nil;
    var right: locale = nil;
    var child = (i+1)*2-1;
    if child < numLocales {
      left = rootLocale.getLocale(child);
      child += 1;
      if child < numLocales then
        right = rootLocale.getLocale(child);
    }
    on rootLocale.getLocale(i) {
      chpl_localeTree.left = left;
      chpl_localeTree.right = right;
    }
  }
}

chpl_initLocaleTree();

}
