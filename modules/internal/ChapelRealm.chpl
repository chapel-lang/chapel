use ChapelBaseLocale;

// <2011-07-06 hilde> These variables would make better sense in DefaultRectangular.chpl
// but they need to go here to avoid the "initialization sequencing" bug.
config param debugDefaultDist = false;
config param debugDataPar = false;


// End comment
//var myrealm : realm = chpl_setupRealm(1, 1, 0);



const RealmSpace: domain(1) = [0..0];
const Realms: [RealmSpace] realm;
pragma "private" var chpl_thisRealm: realm;

const totNumLocales = chpl_computeTotNumLocalesWithoutWarning();
const AllLocaleSpace = [0..#totNumLocales];
const AllLocales: [AllLocaleSpace] locale;
// We cannot use a forall here because the default leader iterator will
// access data structures that are not yet initialized (i.e., Locales
// array/here).  An alternative would be to use a coforall+on and refactor
// chpl_setupLocale().
for loc in AllLocaleSpace do
  AllLocales(loc) = chpl_setupLocale(loc) : locale;

const AllSubLocales: [AllLocaleSpace] locale;


for loc in AllLocaleSpace do  // This should turn into a tree traversal
  AllSubLocales(loc) = AllLocales(loc).getChild();

//
// Using the obvious reduction in the initialization of totNumLocales
// results in a warning... :(
//
proc chpl_computeTotNumLocalesWithoutWarning() {
  var retval = 0;
  for r in RealmSpace do
    retval += chpl_numLocales(r);
  return retval;
}





////////////////const Realms: [RealmSpace] realm;
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

proc chpl_initLocaleTree() {
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


proc thisRealm() : realm{
  return chpl_thisRealm : realm;
}


proc set_here(loc : int) {
 // _here = chpl_int_to_locale(loc);
 //task-private accessor here ...
  _here = thisRealm().Locales[loc];
}

chpl_initLocaleTree();

pragma "initialize prelocale" class realm {
  const chpl_id: int(32);
  const rtype: string;
  const numLocales: int;
  const LocaleSpace: domain(1);
  const Locales: [LocaleSpace] locale;
  const SubLocales: [LocaleSpace] locale;

  proc initialize() {
    if doneCreatingLocales {
      halt("realms cannot be created");
    }
  }

  proc id return chpl_id;

  proc writeThis(x:Writer) {
    x.write("{id = ", id, ", rtype = ", rtype, ", numLocales = ", numLocales, ", LocaleSpace = ", LocaleSpace, ", Locales = ", Locales, "}");
  }
}

proc chpl_setupRealm(id, numLocales, baseID) {
  var tmp: realm;
  on __primitive("chpl_on_locale_num", baseID) {
    if (defaultDist._value == nil) {
      defaultDist = new dmap(new DefaultDist());
    }
    tmp = new realm(id, chpl_getRealmType(id), numLocales, [0..numLocales-1], Locales=>AllLocales[baseID..#numLocales], SubLocales=>AllSubLocales[baseID..#numLocales]);
    forall (loc, id) in (tmp.Locales, 0..) do on loc {
      //loc.myRealm = tmp;
      loc.chpl_id = id;
      chpl_thisRealm = tmp;
    }
  }
  return tmp;
}
