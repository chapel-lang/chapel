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

//
// Using the obvious reduction in the initialization of totNumLocales
// results in a warning... :(
//
def chpl_computeTotNumLocalesWithoutWarning() {
  var retval = 0;
  for r in RealmSpace do
    retval += chpl_numLocales(r);
  return retval;
}

const totNumLocales = chpl_computeTotNumLocalesWithoutWarning();
const AllLocaleSpace = [0..#totNumLocales];
const AllLocales: [AllLocaleSpace] locale;
forall loc in AllLocaleSpace do
  AllLocales(loc) = chpl_setupLocale(loc);

const Realms: [RealmSpace] realm;
// TODO: Will eventually want to make this parallel, but it causes a warning
// today
for r in RealmSpace do
  Realms(r) = chpl_setupRealm(r, chpl_numLocales(r), chpl_baseLocaleID(r));

doneCreatingLocales = true;

//
// tree for recursive task invocation during privatization
//
record chpl_localeTreeRecord {
  var left, right: locale;
}
pragma "private" var chpl_localeTree: chpl_localeTreeRecord;

def chpl_initLocaleTree() {
  for i in AllLocaleSpace {
    var left: locale = nil;
    var right: locale = nil;
    var child = (i+1)*2-1;
    if child < totNumLocales {
      left = AllLocales[child];
      child += 1;
      if child < totNumLocales then
        right = AllLocales[child];
    }
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
  const Locales: [LocaleSpace] locale;

  def initialize() {
    if doneCreatingLocales {
      halt("realms cannot be created");
    }
  }

  def id return chpl_id;

  def writeThis(x:Writer) {
    x.write("{id = ", id, ", rtype = ", rtype, ", numLocales = ", numLocales, ", LocaleSpace = ", LocaleSpace, ", Locales = ", Locales, "}");
  }
}

def chpl_setupRealm(id, numLocales, baseID) {
  var tmp: realm;
  on __primitive("chpl_on_locale_num", baseID) {
    if (defaultDist._value == nil) {
      defaultDist = new dist(new DefaultDist());
    }
    tmp = new realm(id, chpl_getRealmType(id), numLocales, [0..numLocales-1], Locales=>AllLocales[baseID..#numLocales]);
    forall (loc, id) in (tmp.Locales, 0..) do on loc {
      loc.myRealm = tmp;
      loc.chpl_id = id;
      chpl_thisRealm = tmp;
    }
  }
  return tmp;
}
