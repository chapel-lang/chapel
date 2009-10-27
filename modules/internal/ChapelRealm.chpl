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

const Realms: [r in RealmSpace] realm = chpl_setupRealm(r, chpl_numLocales(r), 
                                                        chpl_baseLocaleID(r));

doneCreatingLocales = true;

//
// tree for recursive task invocation during privatization
//
record chpl_localeTreeRecord {
  var left, right: locale;
}
pragma "private" var chpl_localeTree: chpl_localeTreeRecord;

def chpl_initLocaleTree() {
  var numAllLocales = 0;
  for r in Realms do
    numAllLocales += r.numLocales;

  var AllLocales: [1..numAllLocales] locale;
  var i = 1;
  for r in Realms {
    for loc in r.Locales {
      AllLocales[i] = loc;
      i += 1;
    }
  }

  for i in 1..numAllLocales {
    var left: locale = nil;
    var right: locale = nil;
    if i*2 <= numAllLocales then
      left = AllLocales[i*2];
    if i*2+1 <= numAllLocales then
      right = AllLocales[i*2+1];
    if left != nil then
      on AllLocales(i) {
        chpl_localeTree.left = left;
        chpl_localeTree.right = right;
      }
  }
}

chpl_initLocaleTree();

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
    if (defaultDist._value == nil) {
      defaultDist = distributionValue(new DefaultDist());
    }
    tmp = new realm(id, chpl_getRealmType(id), numLocales, baseID);
  }
  return tmp;
}
