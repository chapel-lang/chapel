config const numLocales = 1;

if (numLocales != 1) then
  halt("Multiple locales not yet supported");

// would like this to be the following, but it breaks about 20 tests:
//const LocaleDomain: domain(1) distributed(OnePer) = [0..numLocales);
const LocaleDomain: domain(1) = [0..numLocales);

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
  on this do return __primitive("_coresPerLocale");
}

def localeID() return __primitive("_chpl_comm_locale_id");
