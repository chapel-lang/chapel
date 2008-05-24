// would like this to be the following, but it breaks about 20 tests:
//const LocaleSpace: domain(1) distributed(OnePer) = [0..numLocales-1];
const LocaleSpace: domain(1) = [0..numLocales-1];

// BLC: This should be a single, but there's currently no way to
// query a single variable in a non-blocking manner
var doneCreatingLocales: bool;

class locale {
  const chpl_id: int;

  def locale(id: int = -1) {
    if doneCreatingLocales {
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

pragma "private" var _here: locale;

def here return _here;

def chpl_setupLocale(id) {
  var tmp: locale;
  on __primitive("chpl_on_locale_num", id) {
    tmp = new locale(id);
    _here = tmp;
  }
  return tmp;
}

const Locales: [loc in LocaleSpace] locale = chpl_setupLocale(loc);

doneCreatingLocales = true;

def locale.numCores {
  var numCores: int;
  on this do numCores = __primitive("chpl_coresPerLocale");
  return numCores;
}

def chpl_int_to_locale(id) {
  return Locales(id);
}
