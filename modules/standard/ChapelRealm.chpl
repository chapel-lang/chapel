use DefaultArithmetic;

// These three variables would be more logical in ChapelLocale.chpl,
// but I ran into the nasty "sensitive to function resolution order"
// bug when I did that.
//
var doneCreatingLocales: bool;

pragma "private" var _here: locale;
def here return _here;

// End comment

pragma "private" var chpl_thisRealm: realm;
def thisRealm return chpl_thisRealm;

const RealmBox: domain(1) = [0..numRealms-1];

const RealmSpace: sparse subdomain(RealmBox) = chpl_genNonEmptyRealms();

def chpl_genNonEmptyRealms() {
  for i in RealmBox do
    if chpl_numLocales(i) > 0 then
      yield i;
}

const Realms: [r in RealmSpace] realm = chpl_setupRealm(r, chpl_numLocales(r), 
                                                        chpl_baseLocaleID(r));

doneCreatingLocales = true;

class realm {
  const chpl_id: int(32);
  const rtype: string;
  const numLocales: int;
  const LocaleSpace: domain(1);
  const Locales: [loc in LocaleSpace] locale;

  def realm(id: int = -1, init_rtype: string, init_numLocales: int, 
            baseID: int(32)) {
    if doneCreatingLocales {
      halt("realms cannot be created");
    }
    chpl_id = id;
    rtype = init_rtype;
    numLocales = init_numLocales;
    LocaleSpace = [0..numLocales-1];
    [loc in LocaleSpace] Locales(loc) = chpl_setupLocale(this, loc, baseID+loc);
  }

  def id {
    return chpl_id;
  }
}

def chpl_setupRealm(id, numLocales, baseID) {
  var tmp: realm;
  on __primitive("chpl_on_locale_num", baseID) {
    if (defaultDist == nil) {
      defaultDist = new DefaultDist();
    }
    tmp = new realm(id, chpl_getRealmType(id), numLocales, baseID);
  }
  return tmp;
}
