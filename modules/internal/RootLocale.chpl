// RootLocale.chpl
//  
// Expose the global singleton root locale instance.
// Populate this with the default root locale if no architecture module has been loaded.
//
pragma "no use ChapelStandard"
module RootLocale {

  use ChapelLocale;
  use DefaultRectangular;

  const emptyLocaleSpace: domain(1) = {1..0};
  const emptyLocales: [emptyLocaleSpace] locale;

  class RootLocale : locale {

    // These functions are used to establish values for Locales[] and 
    // LocaleSpace -- an array of locales and its correponding domain
    // which are used as the default set of targetLocales in many 
    // distributions.
    proc getDefaultLocaleSpace() {
      _throwPVFCError();
      return emptyLocaleSpace;
    }      
  
    proc getDefaultLocaleArray() {
      _throwPVFCError();
      return emptyLocales;
    }

    proc localeIDtoLocale(id : chpl_localeID_t) : locale {
      _throwPVFCError();
      return this;
    }
  }

  // Returns a wide pointer to the locale with the given id.
  // When hierarchical locales are fully implemented, the lookup may be
  // done mostly in the runtime (through the sublocale registry).
  proc chpl_localeID_to_locale(id : chpl_localeID_t) : locale {
    // The _is_here test examines only localeIDs, so is local and very fast.
    // Evaluating "here" is also local and very fast.
    if __primitive("_is_here", id) then return here;
    var ret:locale;
    on rootLocale do ret = (rootLocale:RootLocale).localeIDtoLocale(id);
    return ret;
  }
}
