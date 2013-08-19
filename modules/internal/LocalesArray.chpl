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

  // We want the Locales array to be private so that each locale can
  // point to its private copy of rootLocale, but we don't (yet) have
  // a good way to declare and initialize private aliases.  We could
  // do something by hand here, but the private broadcast of Locales
  // and LocaleSpace in the generated code is going to be a lot more
  // efficient than the on clauses that we'd probably need.  Instead,
  // we fake it (see chpl_private_init_rootLocale() for more info).
  //
  var Locales => (rootLocale:RootLocale).getDefaultLocaleArray();

  // We don't use the same private "trick" as with Locales above with
  // LocaleSpace/ because it's small enough to not matter.
  const LocaleSpace = Locales.domain;

}

