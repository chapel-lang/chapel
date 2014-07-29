//
// Expose the underlying locales array (and its domain) for user
// convenience and backward compatibility.
//
// RootLocale is a mechanism to provide this convenience.
//
// Note that rootLocale must be initialized before the Locales array
// can be initialized, but in order to initialize the replicated
// private versions of rootLocale, the Locales array and
// DefaultRectangular must be initialized.
//
// To solve this problem, the rootLocale is initialized on locale 0
// first, with all other locales pointing to this rootLocale.  After
// the Locales array and DefaultRectangular are initialized, we go
// back replicate the rootLocale on every other locale and update the
// Locales array to point to the local copy (if replicateRootLocale is
// true).  See chpl_rootLocaleInitPrivate() in ChapelLocale.chpl for
// more info on how this is done.
//
pragma "no use ChapelStandard"
module LocalesArray {
  // Initialize the rootLocale
  chpl_init_rootLocale();

  // The Locales array should be private so that each locale can
  // point to its private copy of rootLocale, but we don't (yet) have
  // a good way to declare and initialize private aliases.  Instead,
  // we set up the version on all other locales during LocaleModel
  // initialization (see chpl_rootLocaleInitPrivate()).  The copy for
  // locale 0 is set up here for the declaration.
  pragma "private"
  var Locales => (rootLocale:RootLocale).getDefaultLocaleArray();

  // We don't use the same private "trick" as with Locales above with
  // LocaleSpace/ because it's small enough to not matter.
  const LocaleSpace = Locales.domain;

}

