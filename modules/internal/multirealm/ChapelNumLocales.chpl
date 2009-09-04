_extern def chpl_localesPerRealm(r: int(32)): int(32);
_extern def chpl_baseUniqueLocaleID(r: int(32)): int(32);
_extern def chpl_realmType(r: int(32)): string;

def chpl_numLocales(r) {
  return chpl_localesPerRealm(r);
}

def chpl_baseLocaleID(r) {
  return chpl_baseUniqueLocaleID(r);
}

def chpl_getRealmType(r) {
  return chpl_realmType(r);
}
