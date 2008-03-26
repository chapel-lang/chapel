// would like this to be the following, but it breaks about 20 tests:
//const LocaleSpace: domain(1) distributed(OnePer) = [0..numLocales-1];
const LocaleSpace: domain(1) = [0..numLocales-1];

// BLC: This should be a single, but there's currently no way to
// query a single variable in a non-blocking manner
var doneCreatingLocales$: sync bool;

pragma "locale"
class locale {
  const chpl_id: int;

  def locale(id = -1) {
    if (doneCreatingLocales$.isFull) {
      halt("locales cannot be created");
    }
    chpl_id = id;
  }

  def id {
    return chpl_id;
  }

  def writeThis(f: Writer) {
    f.write("LOCALE", id);
  }
}

const Locales: [loc in LocaleSpace] locale = new locale(id = loc);

doneCreatingLocales$ = true;

def locale.numCores {
  var numCores: int;
  on this do numCores = __primitive("chpl_coresPerLocale");
  return numCores;
}

def localeID() return __primitive("_localeID");

def chpl_int_to_locale(id) {
  return Locales(id);
}
