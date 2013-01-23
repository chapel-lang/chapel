// RootLocale.chpl
//  
// Expose the global singleton root locale instance.
// Populate this with the default root locale if no architecture module has been loaded.
//
pragma "no use ChapelStandard"
module RootLocale {

  use ChapelLocale;
  use DefaultArchitecture;

  if (rootLocale == nil) then
    rootLocale = new DefaultRootLocale();

  // Now a misnomer; should be chpl_localeID_to_locale, or chpl_lookup_locale.
  // Returns a wide pointer to the locale with the given id.
  // When hierarchical locales are fully implemented, the lookup will be
  // done mostly in the runtime (through the sublocale registry),
  // rather than being a simple array lookup.  (The Locales[] array
  // will become obsolete -- for locale lookup purposes, at least.)
  proc chpl_int_to_locale(id) {
    // The primitive extracts just the node portion of the locale ID.
    // This is a hobbled implementation (sublocales are not yet implemented).
    return rootLocale.getChild(__primitive("_loc_get_node", id));
  }
  
  // Expose the underlying locales array (and its domain) 
  // for user convenience and backward compatibility.
  const Locales => rootLocale.getChildArray();
  const LocaleSpace = rootLocale.getChildSpace();
}
