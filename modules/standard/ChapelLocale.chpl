const numLocales = __primitive("_chpl_comm_num_locales");

// would like this to be the following, but it breaks about 20 tests:
//const LocaleDomain: domain(1) distributed(OnePer) = [0..numLocales-1];
const LocaleDomain: domain(1) = [0..numLocales-1];

class locale {
  const id: int;
}

def locale.writeThis(f: Writer) {
  f.write("LOCALE", id);
}

//
// was:
//   const Locale: [loc in LocaleDomain] locale = locale(id = loc);
//
// sjd: can be changed back now
//
const Locale: [LocaleDomain] locale;
[loc in LocaleDomain] Locale(loc) = locale(id = loc);


def locale.numCores {
  /* on this do */ return __primitive("_coresPerLocale");
  // what does it even mean to have a return in an on statement?  I
  // would prefer not to do this; certainly we do not migrate the
  // calling site.  This should probably be rewritten to call the
  // primitive and put the result in a temp in an on clause and then
  // return the temp.
}

def localeID() return __primitive("_chpl_comm_locale_id");

def _locale_to_id(l: locale) return l.id;

def _locale_to_id(l) {
  compilerError("invalid on clause");
}
