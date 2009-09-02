module ChapelNumLocales {

// the number of locales on which to run the program
config const numLocales: int = __primitive("chpl_comm_default_num_locales");

def LocaleSpace var {
  return Realms(0).LocaleSpace;
}

def Locales var {
  return Realms(0).Locales;
}

def chpl_numLocales(r) {
  assert(r == 0);
  return numLocales;
}

def chpl_baseLocaleID(r) {
  assert(r == 0);
  return 0;
}

def chpl_getRealmType(r) {
  assert(r == 0);
  return CHPL_TARGET_PLATFORM;
}

}
