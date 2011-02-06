_extern proc chpl_localesPerRealm(r: int(32)): int(32);
_extern proc chpl_baseUniqueLocaleID(r: int(32)): int(32);
_extern proc chpl_realmType(r: int(32)): string;

proc chpl_numLocales(r) {
  return chpl_localesPerRealm(r);
}

proc chpl_baseLocaleID(r) {
  return chpl_baseUniqueLocaleID(r);
}

proc chpl_getRealmType(r) {
  return chpl_realmType(r);
}
