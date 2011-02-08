// the number of locales on which to run the program
config const numLocales: int = __primitive("chpl_comm_default_num_locales");

proc LocaleSpace var {
  return Realms(0).LocaleSpace;
}

proc Locales var {
  return Realms(0).Locales;
}

proc chpl_numLocales(r) {
  assert(r == 0);
  return numLocales;
}

proc chpl_baseLocaleID(r) {
  assert(r == 0);
  return 0;
}

proc chpl_getRealmType(r) {
  assert(r == 0);
  return CHPL_TARGET_PLATFORM;
}
