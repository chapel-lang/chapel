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

  proc chpl_int_to_locale(id) {
    return rootLocale.getChild(id);
  }
  
  // Expose the underlying locales array (and its domain) 
  // for user convenience and backward compatibility.
  const Locales => rootLocale.getChildArray();
  const LocaleSpace = rootLocale.getChildSpace();
}
