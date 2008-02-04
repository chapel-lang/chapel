// would like this to be the following, but it breaks about 20 tests:
//const LocalesDomain: domain(1) distributed(OnePer) = [0..numLocales-1];
const LocalesDomain: domain(1) = [0..numLocales-1];

pragma "locale"
class locale {
  const id: int;
}

def locale.writeThis(f: Writer) {
  f.write("LOCALE", id);
}

const Locales: [loc in LocalesDomain] locale = new locale(id = loc);

def locale.numCores {
  /* on this do */ return __primitive("_coresPerLocale");
  // what does it even mean to have a return in an on statement?  I
  // would prefer not to do this; certainly we do not migrate the
  // calling site.  This should probably be rewritten to call the
  // primitive and put the result in a temp in an on clause and then
  // return the temp.
}

def localeID() return __primitive("_localeID");
